/*
 * Copyright (C) 2017 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#include <tesseract_ignition/render_widget.h>
#include <tesseract_ignition/gui_utils.h>
#include <tesseract_ignition/gui_events.h>
#include <tesseract_ignition/conversions.h>

#include <algorithm>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <QWidget>

// TODO(louise) Remove these pragmas once ign-rendering
// is disabling the warnings
#ifdef _MSC_VER
#pragma warning(push, 0)
#endif

#include <ignition/rendering/Camera.hh>
#include <ignition/rendering/RayQuery.hh>
#include <ignition/rendering/RenderEngine.hh>
#include <ignition/rendering/RenderingIface.hh>
#include <ignition/rendering/Scene.hh>
#include <ignition/rendering/Grid.hh>
#include <ignition/common/Console.hh>

#ifdef _MSC_VER
#pragma warning(pop)
#endif

Q_DECLARE_METATYPE(tesseract_gui::TesseractRenderSync*)

/// \brief Private data class for TesseractRenderer
class tesseract_gui::TesseractRenderer::Implementation
{
public:
  /// \brief Flag to indicate if mouse event is dirty
  bool mouseDirty{false};

  /// \brief Flag to indicate if hover event is dirty
  bool hoverDirty{false};

  /// \brief Flag to indicate if drop event is dirty
  bool dropDirty{false};

  /// \brief Mouse event
  ignition::common::MouseEvent mouseEvent;

  /// \brief Key event
  ignition::common::KeyEvent keyEvent;

  /// \brief Mutex to protect mouse events
  std::mutex mutex;

  /// \brief User camera
  ignition::rendering::CameraPtr camera{nullptr};

  /// \brief The currently hovered mouse position in screen coordinates
  ignition::math::Vector2i mouseHoverPos{ignition::math::Vector2i::Zero};

  /// \brief The currently drop mouse position in screen coordinates
  ignition::math::Vector2i mouseDropPos{ignition::math::Vector2i::Zero};

  /// \brief The dropped text in the scene
  std::string dropText;

  /// \brief Ray query for mouse clicks
  ignition::rendering::RayQueryPtr rayQuery{nullptr};

  /// \brief View control focus target
  ignition::math::Vector3d target;
};

/// \brief Qt and Ogre rendering is happening in different threads
/// The original sample 'textureinthread' from Qt used a double-buffer
/// scheme so that the worker (Ogre) thread write to FBO A, while
/// Qt is displaying FBO B.
///
/// However Qt's implementation doesn't handle all the edge cases
/// (like resizing a window), and also it increases our VRAM
/// consumption in multiple ways (since we have to double other
/// resources as well or re-architect certain parts of the code
/// to avoid it)
///
/// Thus we just serialize both threads so that when Qt reaches
/// drawing preparation, it halts and Ogre worker thread starts rendering,
/// then resumes when Ogre is done.
///
/// This code is admitedly more complicated than it should be
/// because Qt's synchronization using signals and slots causes
/// deadlocks when other means of synchronization are introduced.
/// The whole threaded loop should be rewritten.
///
/// All RenderSync does is conceptually:
///
/// \code
///   TextureNode::PrepareNode()
///   {
///     renderSync.WaitForWorkerThread(); // Qt thread
///       // WaitForQtThreadAndBlock();
///       // Now worker thread begins executing what's between
///       // ReleaseQtThreadFromBlock();
///     continue with qt code...
///   }
/// \endcode
///
///
/// For more info see
/// https://github.com/ignitionrobotics/ign-rendering/issues/304
class tesseract_gui::TesseractRenderSync
{
public:
  /// \brief Cond. variable to synchronize rendering on specific events
  /// (e.g. texture resize) or for debugging (e.g. keep
  /// all API calls sequential)
  std::mutex mutex;

  /// \brief Cond. variable to synchronize rendering on specific events
  /// (e.g. texture resize) or for debugging (e.g. keep
  /// all API calls sequential)
  std::condition_variable cv;

  enum class RenderStallState
          {
            /// Qt is stuck inside WaitForWorkerThread
            /// Worker thread can proceed
            WorkerCanProceed,
            /// Qt is stuck inside WaitForWorkerThread
            /// Worker thread is between WaitForQtThreadAndBlock
            /// and ReleaseQtThreadFromBlock
            WorkerIsProceeding,
            /// Worker is stuck inside WaitForQtThreadAndBlock
            /// Qt can proceed
            QtCanProceed,
            /// Do not block
            ShuttingDown,
          };

  /// \brief See TextureNode::RenderSync::RenderStallState
  RenderStallState renderStallState =
      RenderStallState::QtCanProceed /*GUARDED_BY(sharedRenderMutex)*/;

  /// \brief Must be called from worker thread when we want to block
  /// \param[in] lock Acquired lock. Must be based on this->mutex
  void WaitForQtThreadAndBlock(std::unique_lock<std::mutex> &_lock);

  /// \brief Must be called from worker thread when we are done
  /// \param[in] lock Acquired lock. Must be based on this->mutex
  void ReleaseQtThreadFromBlock(std::unique_lock<std::mutex> &_lock);

  /// \brief Must be called from Qt thread periodically
  void WaitForWorkerThread();

  /// \brief Must be called from GUI thread when shutting down
  void Shutdown();
};

/// \brief Private data class for RenderWindowItem
class tesseract_gui::RenderWidget::Implementation
{
public:
  /// \brief Keep latest mouse event
  ignition::common::MouseEvent mouseEvent;

  /// \brief Render thread
  RenderThread *renderThread = nullptr;

  /// \brief See RenderSync
  TesseractRenderSync renderSync;

  /// \brief See TextureNode
  TextureNode* textureNode;

  /// \brief List of threads
  static QList<QThread *> threads;

  /// \brief List of our QT connections.
  QList<QMetaObject::Connection> connections;
};

using namespace tesseract_gui;

QList<QThread *> RenderWidget::Implementation::threads;

/////////////////////////////////////////////////
void TesseractRenderSync::WaitForQtThreadAndBlock(std::unique_lock<std::mutex> &_lock)
{
  this->cv.wait(_lock, [this]
  { return this->renderStallState == RenderStallState::WorkerCanProceed ||
           this->renderStallState == RenderStallState::ShuttingDown; });

  this->renderStallState = RenderStallState::WorkerIsProceeding;
}

/////////////////////////////////////////////////
void TesseractRenderSync::ReleaseQtThreadFromBlock(std::unique_lock<std::mutex> &_lock)
{
  this->renderStallState = RenderStallState::QtCanProceed;
  _lock.unlock();
  this->cv.notify_one();
}

/////////////////////////////////////////////////
void TesseractRenderSync::WaitForWorkerThread()
{
  std::unique_lock<std::mutex> lock(this->mutex);

  // Wait until we're clear to go
  this->cv.wait( lock, [this]
  {
    return this->renderStallState == RenderStallState::QtCanProceed ||
           this->renderStallState == RenderStallState::ShuttingDown;
  } );

  // Worker thread asked us to wait!
  this->renderStallState = RenderStallState::WorkerCanProceed;

  lock.unlock();
  // Wake up worker thread
  this->cv.notify_one();
  lock.lock();

  // Wait until we're clear to go
  this->cv.wait( lock, [this]
  {
    return this->renderStallState == RenderStallState::QtCanProceed ||
           this->renderStallState == RenderStallState::ShuttingDown;
  } );
}

/////////////////////////////////////////////////
void TesseractRenderSync::Shutdown()
{
  {
    std::unique_lock<std::mutex> lock(this->mutex);

    this->renderStallState = RenderStallState::ShuttingDown;

    lock.unlock();
    this->cv.notify_one();
  }
}

/////////////////////////////////////////////////
TesseractRenderer::TesseractRenderer()
  : dataPtr(ignition::utils::MakeUniqueImpl<Implementation>())
{
}

/////////////////////////////////////////////////
void TesseractRenderer::Render(TesseractRenderSync *_renderSync)
{
  std::unique_lock<std::mutex> lock(_renderSync->mutex);
  _renderSync->WaitForQtThreadAndBlock(lock);

  if (this->textureDirty)
  {
    // TODO(anyone) If SwapFromThread gets implemented,
    // then we only need to lock when texture is dirty
    // (but we still need to lock the whole routine if
    // debugging from RenderDoc or if user is not willing
    // to sacrifice VRAM)
    //
    // std::unique_lock<std::mutex> lock(renderSync->mutex);
    // _renderSync->WaitForQtThreadAndBlock(lock);
    this->dataPtr->camera->SetImageWidth(this->textureSize.width());
    this->dataPtr->camera->SetImageHeight(this->textureSize.height());
    this->dataPtr->camera->SetAspectRatio(this->textureSize.width() / this->textureSize.height());
    // setting the size should cause the render texture to be rebuilt
    this->dataPtr->camera->PreRender();
    this->textureDirty = false;

    // TODO(anyone) See SwapFromThread comments
    // _renderSync->ReleaseQtThreadFromBlock(lock);
  }

  this->textureId = this->dataPtr->camera->RenderTextureGLId();

  // view control
  this->HandleMouseEvent();

  if (tesseract_gui::getApp())
  {
    tesseract_gui::getApp()->sendEvent(tesseract_gui::getApp(), new events::PreRender(this->sceneName));

//    ignition::gui::App()->sendEvent(
//        ignition::gui::App()->findChild<ignition::gui::MainWindow *>(),
//        new ignition::gui::events::PreRender());
  }

  // update and render to texture
  this->dataPtr->camera->Update();

  if (tesseract_gui::getApp())
  {
    tesseract_gui::getApp()->sendEvent(tesseract_gui::getApp(), new events::Render(this->sceneName));
//    ignition::gui::App()->sendEvent(
//        ignition::gui::App()->findChild<ignition::gui::MainWindow *>(),
//        new ignition::gui::events::Render());
  }

  _renderSync->ReleaseQtThreadFromBlock(lock);
}

/////////////////////////////////////////////////
void TesseractRenderer::HandleMouseEvent()
{
  std::lock_guard<std::mutex> lock(this->dataPtr->mutex);
  this->BroadcastHoverPos();
  this->BroadcastDrag();
  this->BroadcastMousePress();
  this->BroadcastLeftClick();
  this->BroadcastRightClick();
  this->BroadcastScroll();
  this->BroadcastKeyPress();
  this->BroadcastKeyRelease();
  this->BroadcastDrop();
  this->dataPtr->mouseDirty = false;
}

////////////////////////////////////////////////
void TesseractRenderer::HandleKeyPress(const ignition::common::KeyEvent &_e)
{
  std::lock_guard<std::mutex> lock(this->dataPtr->mutex);

  this->dataPtr->keyEvent = _e;

  this->dataPtr->mouseEvent.SetControl(this->dataPtr->keyEvent.Control());
  this->dataPtr->mouseEvent.SetShift(this->dataPtr->keyEvent.Shift());
  this->dataPtr->mouseEvent.SetAlt(this->dataPtr->keyEvent.Alt());
}

////////////////////////////////////////////////
void TesseractRenderer::HandleKeyRelease(const ignition::common::KeyEvent &_e)
{
  std::lock_guard<std::mutex> lock(this->dataPtr->mutex);

  this->dataPtr->keyEvent = _e;

  this->dataPtr->mouseEvent.SetControl(this->dataPtr->keyEvent.Control());
  this->dataPtr->mouseEvent.SetShift(this->dataPtr->keyEvent.Shift());
  this->dataPtr->mouseEvent.SetAlt(this->dataPtr->keyEvent.Alt());
}

/////////////////////////////////////////////////
void TesseractRenderer::BroadcastDrop()
{
  if (!this->dataPtr->dropDirty)
    return;
  events::DropOnScene dropOnSceneEvent(
    this->dataPtr->dropText, this->dataPtr->mouseDropPos, this->sceneName);
  tesseract_gui::getApp()->sendEvent(tesseract_gui::getApp(), &dropOnSceneEvent);
//  ignition::gui::App()->sendEvent(ignition::gui::App()->findChild<ignition::gui::MainWindow *>(), &dropOnSceneEvent);
  this->dataPtr->dropDirty = false;
}

/////////////////////////////////////////////////
void TesseractRenderer::BroadcastHoverPos()
{
  if (!this->dataPtr->hoverDirty)
    return;

  auto pos = this->ScreenToScene(this->dataPtr->mouseHoverPos);

  events::HoverToScene hoverToSceneEvent(pos, this->sceneName);
  tesseract_gui::getApp()->sendEvent(tesseract_gui::getApp(), &hoverToSceneEvent);
//  ignition::gui::App()->sendEvent(ignition::gui::App()->findChild<ignition::gui::MainWindow *>(), &hoverToSceneEvent);

  ignition::common::MouseEvent hoverMouseEvent = this->dataPtr->mouseEvent;
  hoverMouseEvent.SetPos(this->dataPtr->mouseHoverPos);
  hoverMouseEvent.SetDragging(false);
  hoverMouseEvent.SetType(ignition::common::MouseEvent::MOVE);
  events::HoverOnScene hoverOnSceneEvent(hoverMouseEvent, this->sceneName);
  tesseract_gui::getApp()->sendEvent(tesseract_gui::getApp(), &hoverOnSceneEvent);
//  ignition::gui::App()->sendEvent(ignition::gui::App()->findChild<ignition::gui::MainWindow *>(), &hoverOnSceneEvent);

  this->dataPtr->hoverDirty = false;
}

/////////////////////////////////////////////////
void TesseractRenderer::BroadcastDrag()
{
  if (!this->dataPtr->mouseDirty)
    return;

  // Only broadcast drag if dragging
  if (!this->dataPtr->mouseEvent.Dragging())
    return;

  events::DragOnScene dragEvent(this->dataPtr->mouseEvent, this->sceneName);
  tesseract_gui::getApp()->sendEvent(tesseract_gui::getApp(), &dragEvent);
//  ignition::gui::App()->sendEvent(ignition::gui::App()->findChild<ignition::gui::MainWindow *>(), &dragEvent);

  this->dataPtr->mouseDirty = false;
}

/////////////////////////////////////////////////
void TesseractRenderer::BroadcastLeftClick()
{
  if (!this->dataPtr->mouseDirty)
    return;

  if (this->dataPtr->mouseEvent.Button() != ignition::common::MouseEvent::LEFT ||
      this->dataPtr->mouseEvent.Type() != ignition::common::MouseEvent::RELEASE)
    return;

  auto pos = this->ScreenToScene(this->dataPtr->mouseEvent.Pos());

  events::LeftClickToScene leftClickToSceneEvent(pos, this->sceneName);
  tesseract_gui::getApp()->sendEvent(tesseract_gui::getApp(), &leftClickToSceneEvent);
//  ignition::gui::App()->sendEvent(ignition::gui::App()->findChild<ignition::gui::MainWindow *>(), &leftClickToSceneEvent);

  events::LeftClickOnScene leftClickOnSceneEvent(this->dataPtr->mouseEvent, this->sceneName);
  tesseract_gui::getApp()->sendEvent(tesseract_gui::getApp(), &leftClickOnSceneEvent);
//  ignition::gui::App()->sendEvent(ignition::gui::App()->findChild<ignition::gui::MainWindow *>(), &leftClickOnSceneEvent);

  this->dataPtr->mouseDirty = false;
}

/////////////////////////////////////////////////
void TesseractRenderer::BroadcastRightClick()
{
  if (!this->dataPtr->mouseDirty)
    return;

  if (this->dataPtr->mouseEvent.Button() != ignition::common::MouseEvent::RIGHT ||
      this->dataPtr->mouseEvent.Type() != ignition::common::MouseEvent::RELEASE)
    return;

  auto pos = this->ScreenToScene(this->dataPtr->mouseEvent.Pos());

  events::RightClickToScene rightClickToSceneEvent(pos, this->sceneName);
  tesseract_gui::getApp()->sendEvent(tesseract_gui::getApp(), &rightClickToSceneEvent);
//  ignition::gui::App()->sendEvent(ignition::gui::App()->findChild<ignition::gui::MainWindow *>(), &rightClickToSceneEvent);

  events::RightClickOnScene rightClickOnSceneEvent(this->dataPtr->mouseEvent, this->sceneName);
  tesseract_gui::getApp()->sendEvent(tesseract_gui::getApp(), &rightClickOnSceneEvent);
//  ignition::gui::App()->sendEvent(ignition::gui::App()->findChild<ignition::gui::MainWindow *>(), &rightClickOnSceneEvent);

  this->dataPtr->mouseDirty = false;
}

/////////////////////////////////////////////////
void TesseractRenderer::BroadcastMousePress()
{
  if (!this->dataPtr->mouseDirty)
    return;

  if (this->dataPtr->mouseEvent.Type() != ignition::common::MouseEvent::PRESS)
    return;

  events::MousePressOnScene event(this->dataPtr->mouseEvent, this->sceneName);
  tesseract_gui::getApp()->sendEvent(tesseract_gui::getApp(), &event);
//  ignition::gui::App()->sendEvent(ignition::gui::App()->findChild<ignition::gui::MainWindow *>(), &event);

  this->dataPtr->mouseDirty = false;
}

/////////////////////////////////////////////////
void TesseractRenderer::BroadcastScroll()
{
  if (!this->dataPtr->mouseDirty)
    return;

  if (this->dataPtr->mouseEvent.Type() != ignition::common::MouseEvent::SCROLL)
    return;

  events::ScrollOnScene scrollOnSceneEvent(this->dataPtr->mouseEvent, this->sceneName);
  tesseract_gui::getApp()->sendEvent(tesseract_gui::getApp(), &scrollOnSceneEvent);
//  ignition::gui::App()->sendEvent(ignition::gui::App()->findChild<ignition::gui::MainWindow *>(), &scrollOnSceneEvent);

  this->dataPtr->mouseDirty = false;
}

/////////////////////////////////////////////////
void TesseractRenderer::BroadcastKeyRelease()
{
  if (this->dataPtr->keyEvent.Type() != ignition::common::KeyEvent::RELEASE)
    return;

  events::KeyReleaseOnScene keyRelease(this->dataPtr->keyEvent, this->sceneName);
  tesseract_gui::getApp()->sendEvent(tesseract_gui::getApp(), &keyRelease);
//  ignition::gui::App()->sendEvent(ignition::gui::App()->findChild<ignition::gui::MainWindow *>(), &keyRelease);

  this->dataPtr->keyEvent.SetType(ignition::common::KeyEvent::NO_EVENT);
}

/////////////////////////////////////////////////
void TesseractRenderer::BroadcastKeyPress()
{
  if (this->dataPtr->keyEvent.Type() != ignition::common::KeyEvent::PRESS)
    return;

  events::KeyPressOnScene keyPress(this->dataPtr->keyEvent, this->sceneName);
  tesseract_gui::getApp()->sendEvent(tesseract_gui::getApp(), &keyPress);
//  ignition::gui::App()->sendEvent(ignition::gui::App()->findChild<ignition::gui::MainWindow *>(), &keyPress);

  this->dataPtr->keyEvent.SetType(ignition::common::KeyEvent::NO_EVENT);
}

/////////////////////////////////////////////////
void TesseractRenderer::Initialize()
{
  if (this->initialized)
    return;

  std::map<std::string, std::string> params;
  params["useCurrentGLContext"] = "1";
//  params["winID"] = std::to_string(tesseract_gui::getApp()->activeWindow()->winId());
//  params["winID"] = std::to_string(
//    ignition::gui::App()->findChild<ignition::gui::MainWindow *>()->
//      QuickWindow()-winId());
  auto engine = ignition::rendering::engine(this->engineName, params);
  if (!engine)
  {
    ignerr << "Engine [" << this->engineName << "] is not supported"
           << std::endl;
    return;
  }

  // Scene
  auto scene = engine->SceneByName(this->sceneName);
  if (!scene)
  {
    igndbg << "Create scene [" << this->sceneName << "]" << std::endl;
    scene = engine->CreateScene(this->sceneName);
    scene->SetAmbientLight(this->ambientLight);
    scene->SetBackgroundColor(this->backgroundColor);
  }

  if (this->skyEnable)
  {
    scene->SetSkyEnabled(true);
  }

  if (this->gridEnable)
  {
    ignition::rendering::VisualPtr visual = scene->VisualByName("tesseract_grid");
    if (visual == nullptr)
    {
      ignition::rendering::VisualPtr root = scene->RootVisual();

      // create gray material
      ignition::rendering::MaterialPtr gray = scene->CreateMaterial();
      gray->SetAmbient(0.7, 0.7, 0.7);
      gray->SetDiffuse(0.7, 0.7, 0.7);
      gray->SetSpecular(0.7, 0.7, 0.7);

      // create grid visual
      unsigned id = 1000; //static_cast<unsigned>(this->dataPtr->entity_manager.addVisual("tesseract_grid"));
      ignition::rendering::VisualPtr visual = scene->CreateVisual(id, "tesseract_grid");
      ignition::rendering::GridPtr gridGeom = scene->CreateGrid();
      if (!gridGeom)
      {
        ignwarn << "Failed to create grid for scene ["
          << scene->Name() << "] on engine ["
            << scene->Engine()->Name() << "]"
              << std::endl;
        return;
      }
      gridGeom->SetCellCount(20);
      gridGeom->SetCellLength(1);
      gridGeom->SetVerticalCellCount(0);
      visual->AddGeometry(gridGeom);
      visual->SetLocalPosition(0, 0, 0.015);
      visual->SetMaterial(gray);
      root->AddChild(visual);
    }
    else
    {
      visual->SetVisible(true);
    }
  }

  auto root = scene->RootVisual();

  // Camera
  this->dataPtr->camera = scene->CreateCamera();
  this->dataPtr->camera->SetUserData("user-camera", true);
  root->AddChild(this->dataPtr->camera);
  this->dataPtr->camera->SetLocalPose(this->cameraPose);
  this->dataPtr->camera->SetNearClipPlane(this->cameraNearClip);
  this->dataPtr->camera->SetFarClipPlane(this->cameraFarClip);
  this->dataPtr->camera->SetImageWidth(this->textureSize.width());
  this->dataPtr->camera->SetImageHeight(this->textureSize.height());
  this->dataPtr->camera->SetAntiAliasing(8);
  this->dataPtr->camera->SetHFOV(M_PI * 0.5);
  // setting the size and calling PreRender should cause the render texture to be rebuilt
  this->dataPtr->camera->PreRender();
  this->textureId = this->dataPtr->camera->RenderTextureGLId();

  // Ray Query
  this->dataPtr->rayQuery = this->dataPtr->camera->Scene()->CreateRayQuery();

  this->initialized = true;
}

/////////////////////////////////////////////////
void TesseractRenderer::Destroy()
{
  auto engine = ignition::rendering::engine(this->engineName);
  if (!engine)
    return;
  auto scene = engine->SceneByName(this->sceneName);
  if (!scene)
    return;
  scene->DestroySensor(this->dataPtr->camera);

  // If that was the last sensor, destroy scene
  if (scene->SensorCount() == 0)
  {
    igndbg << "Destroy scene [" << scene->Name() << "]" << std::endl;
    engine->DestroyScene(scene);

    // TODO(anyone) If that was the last scene, terminate engine?
  }
}

/////////////////////////////////////////////////
void TesseractRenderer::NewHoverEvent(const ignition::math::Vector2i &_hoverPos)
{
  std::lock_guard<std::mutex> lock(this->dataPtr->mutex);
  this->dataPtr->mouseHoverPos = _hoverPos;
  this->dataPtr->hoverDirty = true;
}

/////////////////////////////////////////////////
void TesseractRenderer::NewDropEvent(const std::string &_dropText,
  const ignition::math::Vector2i &_dropPos)
{
  std::lock_guard<std::mutex> lock(this->dataPtr->mutex);
  this->dataPtr->dropText = _dropText;
  this->dataPtr->mouseDropPos = _dropPos;
  this->dataPtr->dropDirty = true;
}

/////////////////////////////////////////////////
void TesseractRenderer::NewMouseEvent(const ignition::common::MouseEvent &_e)
{
  std::lock_guard<std::mutex> lock(this->dataPtr->mutex);
  this->dataPtr->mouseEvent = _e;
  this->dataPtr->mouseDirty = true;
}

/////////////////////////////////////////////////
ignition::math::Vector3d TesseractRenderer::ScreenToScene(const ignition::math::Vector2i &_screenPos) const
{
  // TODO(ahcorde): Replace this code with function in ign-rendering
  // Require this commit
  // https://github.com/ignitionrobotics/ign-rendering/pull/363
  // in ign-rendering6

  // Normalize point on the image
  double width = this->dataPtr->camera->ImageWidth();
  double height = this->dataPtr->camera->ImageHeight();

  double nx = 2.0 * _screenPos.X() / width - 1.0;
  double ny = 1.0 - 2.0 * _screenPos.Y() / height;

  // Make a ray query
  this->dataPtr->rayQuery->SetFromCamera(this->dataPtr->camera, ignition::math::Vector2d(nx, ny));

  auto result = this->dataPtr->rayQuery->ClosestPoint();
  if (result)
    return result.point;

  // Set point to be 10m away if no intersection found
  return this->dataPtr->rayQuery->Origin() +
      this->dataPtr->rayQuery->Direction() * 10;
}

/////////////////////////////////////////////////
RenderThread::RenderThread()
{
  RenderWidget::Implementation::threads << this;
  qRegisterMetaType<TesseractRenderSync*>("TesseractRenderSync*");
}

/////////////////////////////////////////////////
void RenderThread::RenderNext(TesseractRenderSync *_renderSync)
{
  this->context->makeCurrent(this->surface);

  if (!this->renderer.initialized)
  {
    // Initialize renderer
    this->renderer.Initialize();
  }

  // check if engine has been successfully initialized
  if (!this->renderer.initialized)
  {
    ignerr << "Unable to initialize renderer" << std::endl;
    return;
  }

  emit ContextWanted();

  this->renderer.Render(_renderSync);

  emit TextureReady(this->renderer.textureId, this->renderer.textureSize);
}

/////////////////////////////////////////////////
void RenderThread::ShutDown()
{
  this->context->makeCurrent(this->surface);

  this->renderer.Destroy();

  this->context->doneCurrent();
  delete this->context;

  // schedule this to be deleted only after we're done cleaning up
  this->surface->deleteLater();

  // Stop event processing, move the thread to GUI and make sure it is deleted.
  this->exit();
  this->moveToThread(QGuiApplication::instance()->thread());
}

/////////////////////////////////////////////////
void RenderThread::SizeChanged()
{
  auto item = qobject_cast<QWidget *>(this->sender());
  if (!item)
  {
    ignerr << "Internal error, sender is not QQuickItem." << std::endl;
    return;
  }

  if (item->width() <= 0 || item->height() <= 0)
    return;

  this->renderer.textureSize = QSize(item->width(), item->height());
  this->renderer.textureDirty = true;
}

/////////////////////////////////////////////////
TextureNode::TextureNode(RenderWidget *_window, TesseractRenderSync &_renderSync)
    : renderSync(_renderSync), window(_window)
{

  // Our texture node must have a texture, so use the default 0 texture.
  this->texture = new QOpenGLTexture(QImage());
//  this->texture->bind(0);
//  this->texture->setSize(1, 1);
  // this->window->createTextureFromId(0, QSize(1, 1));
}

/////////////////////////////////////////////////
TextureNode::~TextureNode()
{
  delete this->texture;
}

/////////////////////////////////////////////////
void TextureNode::NewTexture(uint _id, const QSize &_size)
{
  this->mutex.lock();
  this->id = _id;
  this->size = _size;
  this->mutex.unlock();

  // We cannot call QQuickWindow::update directly here, as this is only allowed
  // from the rendering thread or GUI thread.
  emit PendingNewTexture();
}

/////////////////////////////////////////////////
void TextureNode::PrepareNode()
{
  this->mutex.lock();
  uint newId = this->id;
  QSize sz = this->size;
  this->id = 0;
  this->mutex.unlock();
  if (newId)
  {
    delete this->texture;
    // note: include QQuickWindow::TextureHasAlphaChannel if the rendered
    // content has alpha.
    this->texture = new QOpenGLTexture(QImage());
//    this->texture->bind(newId);
//    this->texture->setSize(sz.width(), sz.height());
//    this->setTexture(this->texture);

//    this->markDirty(DirtyMaterial);

    // This will notify the rendering thread that the texture is now being
    // rendered and it can start rendering to the other one.
    // emit TextureInUse(&this->renderSync); See comment below
  }
  // NOTE: The original code from Qt samples only emitted when
  // newId is not null.
  //
  // This is correct... for their case.
  // However we need to synchronize the threads when resolution changes,
  // and we're also currently doing everything in lockstep (i.e. both Qt
  // and worker thread are serialized,
  // see https://github.com/ignitionrobotics/ign-rendering/issues/304 )
  //
  // We need to emit even if newId == 0 because it's safe as long as both
  // threads are forcefully serialized and otherwise we may get a
  // deadlock (this func. called twice in a row with the worker thread still
  // finishing the 1st iteration, may result in a deadlock for newer versions
  // of Qt; as WaitForWorkerThread will be called with no corresponding
  // WaitForQtThreadAndBlock as the worker thread thinks there are
  // no more jobs to do.
  //
  // If we want these to run in worker thread and stay resolution-synchronized,
  // we probably should use a different method of signals and slots
  // to send work to the worker thread and get results back
  emit TextureInUse(&this->renderSync);

  this->renderSync.WaitForWorkerThread();
}

/////////////////////////////////////////////////
RenderWidget::RenderWidget(QWidget *_parent)
  : QOpenGLWidget(_parent)
  , dataPtr(ignition::utils::MakeUniqueImpl<Implementation>())
{
  connect(this, &QOpenGLWidget::aboutToCompose, this, &RenderWidget::onAboutToCompose);
  connect(this, &QOpenGLWidget::frameSwapped, this, &RenderWidget::onFrameSwapped);
  connect(this, &QOpenGLWidget::aboutToResize, this, &RenderWidget::onAboutToResize);
  connect(this, &QOpenGLWidget::resized, this, &RenderWidget::onResized);

//  this->setAcceptedMouseButtons(Qt::AllButtons);
//  this->setFlag(ItemHasContents);
  this->dataPtr->renderThread = new RenderThread();

  connect(this->dataPtr->renderThread, &RenderThread::TextureReady, this, &RenderWidget::onTextureReady);
  connect(this->dataPtr->renderThread, &RenderThread::ContextWanted, this, &RenderWidget::grabContext);
}

/////////////////////////////////////////////////
RenderWidget::~RenderWidget()
{
  // Disconnect our QT connections.
  for(auto conn : this->dataPtr->connections)
    QObject::disconnect(conn);

  this->dataPtr->renderSync.Shutdown();
  QMetaObject::invokeMethod(this->dataPtr->renderThread,
                            "ShutDown",
                            Qt::QueuedConnection);

  this->dataPtr->renderThread->wait();
}

/////////////////////////////////////////////////
void RenderWidget::Ready()
{
  this->dataPtr->renderThread->surface = new QOffscreenSurface();
  this->dataPtr->renderThread->surface->setFormat(
      this->dataPtr->renderThread->context->format());
  this->dataPtr->renderThread->surface->create();

  this->dataPtr->renderThread->renderer.textureSize =
      QSize(std::max(this->width(), 1), std::max(this->height(), 1));

  this->dataPtr->renderThread->moveToThread(this->dataPtr->renderThread);

  this->connect(this, &QOpenGLWidget::resized,
      this->dataPtr->renderThread, &RenderThread::SizeChanged);

  this->dataPtr->renderThread->start();
  this->dataPtr->renderThread->RenderNext(&this->dataPtr->renderSync);
  this->update();
}

void RenderWidget::onAboutToCompose()
{
//  this->dataPtr->renderSync.WaitForWorkerThread();
}

void RenderWidget::onFrameSwapped()
{
  if (!this->dataPtr->renderThread->context)
  {
    QOpenGLContext *current = context();
    // Some GL implementations require that the currently bound context is
    // made non-current before we set up sharing, so we doneCurrent here
    // and makeCurrent down below while setting up our own context.
    doneCurrent();

    this->dataPtr->renderThread->context = new QOpenGLContext();
    this->dataPtr->renderThread->context->setFormat(current->format());
    this->dataPtr->renderThread->context->setShareContext(current);
    this->dataPtr->renderThread->context->create();
    this->dataPtr->renderThread->context->moveToThread(this->dataPtr->renderThread);

    makeCurrent();

    QMetaObject::invokeMethod(this, "Ready");
    return;
  }

//  if (!this->dataPtr->textureNode)
//  {
//    this->dataPtr->textureNode = new TextureNode(this, this->dataPtr->renderSync);
//    this->dataPtr->connections << this->connect(this->dataPtr->renderThread,
//        &RenderThread::TextureReady, this->dataPtr->textureNode, &TextureNode::NewTexture,
//        Qt::DirectConnection);
//    this->dataPtr->connections << this->connect(this->dataPtr->textureNode,
//        &tesseract_gui::TextureNode::PendingNewTexture, this, [this](){this->update();}, Qt::QueuedConnection);
//    this->dataPtr->connections << this->connect(this,
//        &QOpenGLWidget::aboutToCompose, this->dataPtr->textureNode, &TextureNode::PrepareNode,
//        Qt::DirectConnection);
//    this->dataPtr->connections << this->connect(this->dataPtr->textureNode,
//        &TextureNode::TextureInUse, this->dataPtr->renderThread,
//        &RenderThread::RenderNext, Qt::QueuedConnection);

//    // Get the production of FBO textures started..
//    QMetaObject::invokeMethod(this->dataPtr->renderThread, "RenderNext",
//      Qt::QueuedConnection,
//      Q_ARG(TesseractRenderSync*, &this->dataPtr->textureNode->renderSync));
//  }
}
void RenderWidget::onAboutToResize()
{

}

void RenderWidget::onResized()
{

}

void RenderWidget::onTextureReady()
{
  QOpenGLContext *current = context();
  current->moveToThread(qGuiApp->thread());
  update();
  this->dataPtr->renderThread->RenderNext(&this->dataPtr->renderSync);
}

void RenderWidget::grabContext()
{
  this->dataPtr->renderSync.WaitForWorkerThread();
  context()->moveToThread(this->dataPtr->renderThread);
}


/////////////////////////////////////////////////
//void RenderWidget::paintGL()
//{
//  if (!this->dataPtr->renderThread->context)
//  {
//    QOpenGLContext *current = context();
//    // Some GL implementations require that the currently bound context is
//    // made non-current before we set up sharing, so we doneCurrent here
//    // and makeCurrent down below while setting up our own context.
//    doneCurrent();

//    this->dataPtr->renderThread->context = new QOpenGLContext();
//    this->dataPtr->renderThread->context->setFormat(current->format());
//    this->dataPtr->renderThread->context->setShareContext(current);
//    this->dataPtr->renderThread->context->create();
//    this->dataPtr->renderThread->context->moveToThread(this->dataPtr->renderThread);

//    makeCurrent();

//    QMetaObject::invokeMethod(this, "Ready");
//    return;
//  }

//  if (!this->dataPtr->textureNode)
//  {
//    this->dataPtr->textureNode = new TextureNode(this, this->dataPtr->renderSync);
//    this->dataPtr->connections << this->connect(this->dataPtr->renderThread,
//        &RenderThread::TextureReady, this->dataPtr->textureNode, &TextureNode::NewTexture,
//        Qt::DirectConnection);
//    this->dataPtr->connections << this->connect(this->dataPtr->textureNode,
//        &tesseract_gui::TextureNode::PendingNewTexture, this, [this](){this->update();}, Qt::QueuedConnection);
//    this->dataPtr->connections << this->connect(this,
//        &QOpenGLWidget::aboutToCompose, this->dataPtr->textureNode, &TextureNode::PrepareNode,
//        Qt::DirectConnection);
//    this->dataPtr->connections << this->connect(this->dataPtr->textureNode,
//        &TextureNode::TextureInUse, this->dataPtr->renderThread,
//        &RenderThread::RenderNext, Qt::QueuedConnection);

//    // Get the production of FBO textures started..
//    QMetaObject::invokeMethod(this->dataPtr->renderThread, "RenderNext",
//      Qt::QueuedConnection,
//      Q_ARG(TesseractRenderSync*, &this->dataPtr->textureNode->renderSync));
//  }

////  node->setRect(this->boundingRect());
//}

/////////////////////////////////////////////////
void RenderWidget::SetBackgroundColor(const ignition::math::Color &_color)
{
  this->dataPtr->renderThread->renderer.backgroundColor = _color;
}

/////////////////////////////////////////////////
void RenderWidget::SetAmbientLight(const ignition::math::Color &_ambient)
{
  this->dataPtr->renderThread->renderer.ambientLight = _ambient;
}

/////////////////////////////////////////////////
void RenderWidget::SetEngineName(const std::string &_name)
{
  this->dataPtr->renderThread->renderer.engineName = _name;
}

/////////////////////////////////////////////////
void RenderWidget::SetSceneName(const std::string &_name)
{
  this->dataPtr->renderThread->renderer.sceneName = _name;
}

/////////////////////////////////////////////////
void RenderWidget::SetCameraPose(const ignition::math::Pose3d &_pose)
{
  this->dataPtr->renderThread->renderer.cameraPose = _pose;
}

/////////////////////////////////////////////////
void RenderWidget::SetCameraNearClip(double _near)
{
  this->dataPtr->renderThread->renderer.cameraNearClip = _near;
}

/////////////////////////////////////////////////
void RenderWidget::SetCameraFarClip(double _far)
{
  this->dataPtr->renderThread->renderer.cameraFarClip = _far;
}

/////////////////////////////////////////////////
void RenderWidget::SetSkyEnabled(const bool &_sky)
{
  this->dataPtr->renderThread->renderer.skyEnable = _sky;
}

void RenderWidget::SetGridEnabled(bool _grid)
{
  this->dataPtr->renderThread->renderer.gridEnable = _grid;
}


/////////////////////////////////////////////////
//void MinimalScene::LoadConfig(const tinyxml2::XMLElement *_pluginElem)
//{
//  RenderWindowItem *renderWindow =
//      this->PluginItem()->findChild<RenderWindowItem *>();
//  if (!renderWindow)
//  {
//    ignerr << "Unable to find Render Window item. "
//           << "Render window will not be created" << std::endl;
//    return;
//  }

//  if (this->title.empty())
//    this->title = "3D Scene";

//  std::string cmdRenderEngine = ignition::gui::renderEngineName();
//  // Custom parameters
//  if (_pluginElem)
//  {
//    // Only pick engine from XML if none is set on the Window
//    if (cmdRenderEngine.empty())
//    {
//      auto elem = _pluginElem->FirstChildElement("engine");
//      if (nullptr != elem && nullptr != elem->GetText())
//      {
//        cmdRenderEngine = elem->GetText();
//      }
//    }

//    auto elem = _pluginElem->FirstChildElement("scene");
//    if (nullptr != elem && nullptr != elem->GetText())
//      renderWindow->SetSceneName(elem->GetText());

//    elem = _pluginElem->FirstChildElement("ambient_light");
//    if (nullptr != elem && nullptr != elem->GetText())
//    {
//      math::Color ambient;
//      std::stringstream colorStr;
//      colorStr << std::string(elem->GetText());
//      colorStr >> ambient;
//      renderWindow->SetAmbientLight(ambient);
//    }

//    elem = _pluginElem->FirstChildElement("background_color");
//    if (nullptr != elem && nullptr != elem->GetText())
//    {
//      math::Color bgColor;
//      std::stringstream colorStr;
//      colorStr << std::string(elem->GetText());
//      colorStr >> bgColor;
//      renderWindow->SetBackgroundColor(bgColor);
//    }

//    elem = _pluginElem->FirstChildElement("camera_pose");
//    if (nullptr != elem && nullptr != elem->GetText())
//    {
//      math::Pose3d pose;
//      std::stringstream poseStr;
//      poseStr << std::string(elem->GetText());
//      poseStr >> pose;
//      renderWindow->SetCameraPose(pose);
//    }

//    elem = _pluginElem->FirstChildElement("camera_clip");
//    if (nullptr != elem && !elem->NoChildren())
//    {
//      auto child = elem->FirstChildElement("near");
//      if (nullptr != child && nullptr != child->GetText())
//      {
//        double n;
//        std::stringstream nearStr;
//        nearStr << std::string(child->GetText());
//        nearStr >> n;
//        if (nearStr.fail())
//        {
//          ignerr << "Unable to set <near> to '" << nearStr.str()
//                 << "' using default near clip distance" << std::endl;
//        }
//        else
//        {
//          renderWindow->SetCameraNearClip(n);
//        }
//      }

//      child = elem->FirstChildElement("far");
//      if (nullptr != child && nullptr != child->GetText())
//      {
//        double f;
//        std::stringstream farStr;
//        farStr << std::string(child->GetText());
//        farStr >> f;
//        if (farStr.fail())
//        {
//          ignerr << "Unable to set <far> to '" << farStr.str()
//                 << "' using default far clip distance" << std::endl;
//        }
//        else
//        {
//          renderWindow->SetCameraFarClip(f);
//        }
//      }
//    }

//    elem = _pluginElem->FirstChildElement("sky");
//    if (nullptr != elem && nullptr != elem->GetText())
//    {
//      renderWindow->SetSkyEnabled(true);
//      if (!elem->NoChildren())
//        ignwarn << "Child elements of <sky> are not supported yet" << std::endl;
//    }
//  }

//  renderWindow->SetEngineName(cmdRenderEngine);
//  // there is a problem with displaying ogre2 render textures that are in
//  // sRGB format. Workaround for now is to apply gamma correction
//  // manually.
//  // There maybe a better way to solve the problem by making OpenGL calls.
//  if (cmdRenderEngine == std::string("ogre2"))
//    this->PluginItem()->setProperty("gammaCorrect", true);
//}

/////////////////////////////////////////////////
void RenderWidget::OnHovered(int _mouseX, int _mouseY)
{
  this->dataPtr->renderThread->renderer.NewHoverEvent({_mouseX, _mouseY});
  update();
}

/////////////////////////////////////////////////
void RenderWidget::OnDropped(const QString &_drop, int _mouseX, int _mouseY)
{
  this->dataPtr->renderThread->renderer.NewDropEvent(_drop.toStdString(), {_mouseX, _mouseY});
  update();
}

/////////////////////////////////////////////////
void RenderWidget::mousePressEvent(QMouseEvent *_e)
{
  this->dataPtr->mouseEvent = convert(*_e);
  this->dataPtr->mouseEvent.SetPressPos(this->dataPtr->mouseEvent.Pos());

  this->dataPtr->renderThread->renderer.NewMouseEvent(this->dataPtr->mouseEvent);
  update();
}

////////////////////////////////////////////////
void RenderWidget::keyPressEvent(QKeyEvent *_e)
{
  if (_e->isAutoRepeat())
    return;

  auto event = convert(*_e);
  this->HandleKeyPress(event);
  update();
}

////////////////////////////////////////////////
void RenderWidget::keyReleaseEvent(QKeyEvent *_e)
{
  if (_e->isAutoRepeat())
    return;

  auto event = convert(*_e);
  this->HandleKeyPress(event);
  update();
}

////////////////////////////////////////////////
void RenderWidget::mouseReleaseEvent(QMouseEvent *_e)
{
  // Store values that depend on previous events
  auto pressPos = this->dataPtr->mouseEvent.PressPos();
  auto dragging = this->dataPtr->mouseEvent.Dragging();

  this->dataPtr->mouseEvent = convert(*_e);
  this->dataPtr->mouseEvent.SetPressPos(pressPos);
  this->dataPtr->mouseEvent.SetDragging(dragging);

  this->dataPtr->renderThread->renderer.NewMouseEvent(this->dataPtr->mouseEvent);
  update();
}

////////////////////////////////////////////////
void RenderWidget::mouseMoveEvent(QMouseEvent *_e)
{
  // Store values that depend on previous events
  auto pressPos = this->dataPtr->mouseEvent.PressPos();

  this->dataPtr->mouseEvent = convert(*_e);

  if (this->dataPtr->mouseEvent.Dragging())
    this->dataPtr->mouseEvent.SetPressPos(pressPos);

  this->dataPtr->renderThread->renderer.NewMouseEvent(this->dataPtr->mouseEvent);
  update();
}

////////////////////////////////////////////////
void RenderWidget::wheelEvent(QWheelEvent *_e)
{
//  this->forceActiveFocus();

  this->dataPtr->mouseEvent = convert(*_e);
  this->dataPtr->renderThread->renderer.NewMouseEvent(this->dataPtr->mouseEvent);
  update();
}

////////////////////////////////////////////////
void RenderWidget::HandleKeyPress(const ignition::common::KeyEvent &_e)
{
  this->dataPtr->renderThread->renderer.HandleKeyPress(_e);
  update();
}

////////////////////////////////////////////////
void RenderWidget::HandleKeyRelease(const ignition::common::KeyEvent &_e)
{
  this->dataPtr->renderThread->renderer.HandleKeyRelease(_e);
  update();
}

///////////////////////////////////////////////////
//void MinimalScene::onHovered(int _mouseX, int _mouseY)
//{
////  auto renderWindow = this->PluginItem()->findChild<RenderWindowItem *>();
//  auto renderWindow = tesseract_gui::getApp()->findChild<RenderWindowItem *>();
//  renderWindow->OnHovered(_mouseX, _mouseY);
//}

///////////////////////////////////////////////////
//void MinimalScene::onDropped(const QString &_drop, int _mouseX, int _mouseY)
//{
////  auto renderWindow = this->PluginItem()->findChild<RenderWindowItem *>();
//  auto renderWindow = tesseract_gui::getApp()->findChild<RenderWindowItem *>();
//  renderWindow->OnDropped(_drop, _mouseX, _mouseY);
//}

///////////////////////////////////////////////////
//void MinimalScene::onFocusWindow()
//{
////  auto renderWindow = this->PluginItem()->findChild<RenderWindowItem *>();
//  auto renderWindow = tesseract_gui::getApp()->findChild<RenderWindowItem *>();
//  renderWindow->forceActiveFocus();
//}
