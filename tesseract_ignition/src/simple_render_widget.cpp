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

#include <tesseract_ignition/simple_render_widget.h>
#include <tesseract_ignition/gui_utils.h>
#include <tesseract_ignition/gui_events.h>
#include <tesseract_ignition/conversions.h>
#include <tesseract_ignition/interactive_view_control.h>

#include <algorithm>
#include <map>
#include <sstream>
#include <string>
#include <vector>

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
#include <ignition/rendering/AxisVisual.hh>
#include <ignition/common/Console.hh>

#include <QOpenGLTexture>
#include <QOpenGLTextureBlitter>
#include <QOffscreenSurface>

#ifdef _MSC_VER
#pragma warning(pop)
#endif

namespace tesseract_gui
{
/** @brief Private data class for SimpleRenderer */
class SimpleRendererImpl
{
public:
  /** @brief Flag to indicate if mouse event is dirty */
  bool mouseDirty{ false };

  /** @brief Flag to indicate if hover event is dirty */
  bool hoverDirty{ false };

  /** @brief Flag to indicate if drop event is dirty */
  bool dropDirty{ false };

  /** @brief Mouse event */
  ignition::common::MouseEvent mouseEvent;

  /** @brief Key event */
  ignition::common::KeyEvent keyEvent;

  /** @brief Mutex to protect mouse events */
  std::mutex mutex;

  /** @brief User camera */
  ignition::rendering::CameraPtr camera{ nullptr };

  /** @brief The currently hovered mouse position in screen coordinates */
  ignition::math::Vector2i mouseHoverPos{ ignition::math::Vector2i::Zero };

  /** @brief The currently drop mouse position in screen coordinates */
  ignition::math::Vector2i mouseDropPos{ ignition::math::Vector2i::Zero };

  /** @brief The dropped text in the scene */
  std::string dropText;

  /** @brief Ray query for mouse clicks */
  ignition::rendering::RayQueryPtr rayQuery{ nullptr };

  /** @brief View control focus target */
  ignition::math::Vector3d target;
};

/////////////////////////////////////////////////
SimpleRenderer::SimpleRenderer() : dataPtr(std::make_unique<SimpleRendererImpl>()) {}

/////////////////////////////////////////////////
void SimpleRenderer::render()
{
  if (!initialized)
    return;

  if (this->texture_dirty)
  {
    std::lock_guard<std::mutex> lock(this->dataPtr->mutex);
    this->dataPtr->camera->SetImageWidth(this->texture_size.width());
    this->dataPtr->camera->SetImageHeight(this->texture_size.height());
    this->dataPtr->camera->SetAspectRatio(static_cast<double>(this->texture_size.width()) /
                                          static_cast<double>(this->texture_size.height()));
    // setting the size should cause the render texture to be rebuilt
    this->dataPtr->camera->PreRender();
    this->texture_id = this->dataPtr->camera->RenderTextureGLId();
    this->texture_dirty = false;
  }

  // view control
  this->handleMouseEvent();

  if (tesseract_gui::getApp() != nullptr)
  {
    QApplication::sendEvent(tesseract_gui::getApp(), new events::PreRender(this->scene_name));
  }

  {
    std::lock_guard<std::mutex> lock(this->dataPtr->mutex);
    this->dataPtr->camera->Update();
    assert(this->texture_id == this->dataPtr->camera->RenderTextureGLId());
  }

  if (tesseract_gui::getApp() != nullptr)
  {
    QApplication::sendEvent(tesseract_gui::getApp(), new events::Render(this->scene_name));
  }

  return;
}

void SimpleRenderer::resize(int width, int height)
{
  std::lock_guard<std::mutex> lock(this->dataPtr->mutex);
  this->texture_size = QSize(width, height);
  this->texture_dirty = true;
}

/////////////////////////////////////////////////
void SimpleRenderer::handleMouseEvent()
{
  std::lock_guard<std::mutex> lock(this->dataPtr->mutex);
  this->broadcastHoverPos();
  this->broadcastDrag();
  this->broadcastMousePress();
  this->broadcastLeftClick();
  this->broadcastRightClick();
  this->broadcastScroll();
  this->broadcastKeyPress();
  this->broadcastKeyRelease();
  this->broadcastDrop();
  this->dataPtr->mouseDirty = false;
}

////////////////////////////////////////////////
void SimpleRenderer::handleKeyPress(const ignition::common::KeyEvent& event)
{
  std::lock_guard<std::mutex> lock(this->dataPtr->mutex);

  this->dataPtr->keyEvent = event;

  this->dataPtr->mouseEvent.SetControl(this->dataPtr->keyEvent.Control());
  this->dataPtr->mouseEvent.SetShift(this->dataPtr->keyEvent.Shift());
  this->dataPtr->mouseEvent.SetAlt(this->dataPtr->keyEvent.Alt());
}

////////////////////////////////////////////////
void SimpleRenderer::handleKeyRelease(const ignition::common::KeyEvent& event)
{
  std::lock_guard<std::mutex> lock(this->dataPtr->mutex);

  this->dataPtr->keyEvent = event;

  this->dataPtr->mouseEvent.SetControl(this->dataPtr->keyEvent.Control());
  this->dataPtr->mouseEvent.SetShift(this->dataPtr->keyEvent.Shift());
  this->dataPtr->mouseEvent.SetAlt(this->dataPtr->keyEvent.Alt());
}

/////////////////////////////////////////////////
void SimpleRenderer::broadcastDrop()
{
  if (!this->dataPtr->dropDirty)
    return;
  events::DropOnScene dropOnSceneEvent(this->dataPtr->dropText, this->dataPtr->mouseDropPos, this->scene_name);
  QApplication::sendEvent(tesseract_gui::getApp(), &dropOnSceneEvent);
  this->dataPtr->dropDirty = false;
}

/////////////////////////////////////////////////
void SimpleRenderer::broadcastHoverPos()
{
  if (!this->dataPtr->hoverDirty)
    return;

  auto pos = this->screenToScene(this->dataPtr->mouseHoverPos);

  events::HoverToScene hoverToSceneEvent(pos, this->scene_name);
  QApplication::sendEvent(tesseract_gui::getApp(), &hoverToSceneEvent);

  ignition::common::MouseEvent hoverMouseEvent = this->dataPtr->mouseEvent;
  hoverMouseEvent.SetPos(this->dataPtr->mouseHoverPos);
  hoverMouseEvent.SetDragging(false);
  hoverMouseEvent.SetType(ignition::common::MouseEvent::MOVE);
  events::HoverOnScene hoverOnSceneEvent(hoverMouseEvent, this->scene_name);
  QApplication::sendEvent(tesseract_gui::getApp(), &hoverOnSceneEvent);

  this->dataPtr->hoverDirty = false;
}

/////////////////////////////////////////////////
void SimpleRenderer::broadcastDrag()
{
  if (!this->dataPtr->mouseDirty)
    return;

  // Only broadcast drag if dragging
  if (!this->dataPtr->mouseEvent.Dragging())
    return;

  events::DragOnScene dragEvent(this->dataPtr->mouseEvent, this->scene_name);
  QApplication::sendEvent(tesseract_gui::getApp(), &dragEvent);

  this->dataPtr->mouseDirty = false;
}

/////////////////////////////////////////////////
void SimpleRenderer::broadcastLeftClick()
{
  if (!this->dataPtr->mouseDirty)
    return;

  if (this->dataPtr->mouseEvent.Button() != ignition::common::MouseEvent::LEFT ||
      this->dataPtr->mouseEvent.Type() != ignition::common::MouseEvent::RELEASE)
    return;

  auto pos = this->screenToScene(this->dataPtr->mouseEvent.Pos());

  events::LeftClickToScene leftClickToSceneEvent(pos, this->scene_name);
  QApplication::sendEvent(tesseract_gui::getApp(), &leftClickToSceneEvent);

  events::LeftClickOnScene leftClickOnSceneEvent(this->dataPtr->mouseEvent, this->scene_name);
  QApplication::sendEvent(tesseract_gui::getApp(), &leftClickOnSceneEvent);

  this->dataPtr->mouseDirty = false;
}

/////////////////////////////////////////////////
void SimpleRenderer::broadcastRightClick()
{
  if (!this->dataPtr->mouseDirty)
    return;

  if (this->dataPtr->mouseEvent.Button() != ignition::common::MouseEvent::RIGHT ||
      this->dataPtr->mouseEvent.Type() != ignition::common::MouseEvent::RELEASE)
    return;

  auto pos = this->screenToScene(this->dataPtr->mouseEvent.Pos());

  events::RightClickToScene rightClickToSceneEvent(pos, this->scene_name);
  QApplication::sendEvent(tesseract_gui::getApp(), &rightClickToSceneEvent);

  events::RightClickOnScene rightClickOnSceneEvent(this->dataPtr->mouseEvent, this->scene_name);
  QApplication::sendEvent(tesseract_gui::getApp(), &rightClickOnSceneEvent);

  this->dataPtr->mouseDirty = false;
}

/////////////////////////////////////////////////
void SimpleRenderer::broadcastMousePress()
{
  if (!this->dataPtr->mouseDirty)
    return;

  if (this->dataPtr->mouseEvent.Type() != ignition::common::MouseEvent::PRESS)
    return;

  events::MousePressOnScene event(this->dataPtr->mouseEvent, this->scene_name);
  QApplication::sendEvent(tesseract_gui::getApp(), &event);

  this->dataPtr->mouseDirty = false;
}

/////////////////////////////////////////////////
void SimpleRenderer::broadcastScroll()
{
  if (!this->dataPtr->mouseDirty)
    return;

  if (this->dataPtr->mouseEvent.Type() != ignition::common::MouseEvent::SCROLL)
    return;

  events::ScrollOnScene scrollOnSceneEvent(this->dataPtr->mouseEvent, this->scene_name);
  QApplication::sendEvent(tesseract_gui::getApp(), &scrollOnSceneEvent);

  this->dataPtr->mouseDirty = false;
}

/////////////////////////////////////////////////
void SimpleRenderer::broadcastKeyRelease()
{
  if (this->dataPtr->keyEvent.Type() != ignition::common::KeyEvent::RELEASE)
    return;

  events::KeyReleaseOnScene keyRelease(this->dataPtr->keyEvent, this->scene_name);
  QApplication::sendEvent(tesseract_gui::getApp(), &keyRelease);

  this->dataPtr->keyEvent.SetType(ignition::common::KeyEvent::NO_EVENT);
}

/////////////////////////////////////////////////
void SimpleRenderer::broadcastKeyPress()
{
  if (this->dataPtr->keyEvent.Type() != ignition::common::KeyEvent::PRESS)
    return;

  events::KeyPressOnScene keyPress(this->dataPtr->keyEvent, this->scene_name);
  QApplication::sendEvent(tesseract_gui::getApp(), &keyPress);

  this->dataPtr->keyEvent.SetType(ignition::common::KeyEvent::NO_EVENT);
}

/////////////////////////////////////////////////
void SimpleRenderer::initialize()
{
  if (this->initialized)
    return;

  std::lock_guard<std::mutex> lock(this->dataPtr->mutex);
  std::map<std::string, std::string> params;
  params["useCurrentGLContext"] = "1";
  auto* engine = ignition::rendering::engine(this->engine_name, params);
  if (engine == nullptr)
  {
    ignerr << "Engine [" << this->engine_name << "] is not supported" << std::endl;
    return;
  }

  // Scene
  auto scene = engine->SceneByName(this->scene_name);
  if (!scene)
  {
    igndbg << "Create scene [" << this->scene_name << "]" << std::endl;
    scene = engine->CreateScene(this->scene_name);
    scene->SetAmbientLight(this->ambient_light);
    scene->SetBackgroundColor(this->background_color);
  }

  if (this->sky_enable)
  {
    scene->SetSkyEnabled(true);
  }

  if (this->grid_enable)
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
      unsigned id = 1000;  // static_cast<unsigned>(this->dataPtr->entity_manager.addVisual("tesseract_grid"));
      ignition::rendering::VisualPtr visual = scene->CreateVisual(id, "tesseract_grid");
      ignition::rendering::GridPtr gridGeom = scene->CreateGrid();
      if (!gridGeom)
      {
        ignwarn << "Failed to create grid for scene [" << scene->Name() << "] on engine [" << scene->Engine()->Name()
                << "]" << std::endl;
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
  this->dataPtr->camera->SetNearClipPlane(this->camera_near_clip);
  this->dataPtr->camera->SetFarClipPlane(this->camera_far_clip);
  this->dataPtr->camera->SetImageWidth(this->texture_size.width());
  this->dataPtr->camera->SetImageHeight(this->texture_size.height());
  this->dataPtr->camera->SetAntiAliasing(8);
  this->dataPtr->camera->SetHFOV(M_PI * 0.5);

  /** @todo LEVI Need figure out haw to get the camera by name. */
  // create directional light
  ignition::rendering::DirectionalLightPtr light0 = scene->CreateDirectionalLight();
  light0->SetDirection(1, 0, 0);
  light0->SetDiffuseColor(0.8, 0.8, 0.8);
  light0->SetSpecularColor(0.5, 0.5, 0.5);
  this->dataPtr->camera->AddChild(light0);

  ignition::rendering::AxisVisualPtr axis = scene->CreateAxisVisual("world_axis");
  axis->SetInheritScale(false);
  axis->Scale(0.5, 0.5, 0.5);
  axis->SetVisible(true);
  root->AddChild(axis);

  // Generate initial texture
  this->dataPtr->camera->PreRender();
  this->texture_id = this->dataPtr->camera->RenderTextureGLId();
  this->dataPtr->camera->Update();

  // Ray Query
  this->dataPtr->rayQuery = this->dataPtr->camera->Scene()->CreateRayQuery();

  this->initialized = true;
}

/////////////////////////////////////////////////
void SimpleRenderer::destroy()
{
  auto* engine = ignition::rendering::engine(this->engine_name);
  if (engine == nullptr)
    return;
  auto scene = engine->SceneByName(this->scene_name);
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
void SimpleRenderer::newHoverEvent(const ignition::math::Vector2i& hover_pos)
{
  std::lock_guard<std::mutex> lock(this->dataPtr->mutex);
  this->dataPtr->mouseHoverPos = hover_pos;
  this->dataPtr->hoverDirty = true;
}

/////////////////////////////////////////////////
void SimpleRenderer::newDropEvent(const std::string& drop_text, const ignition::math::Vector2i& drop_pos)
{
  std::lock_guard<std::mutex> lock(this->dataPtr->mutex);
  this->dataPtr->dropText = drop_text;
  this->dataPtr->mouseDropPos = drop_pos;
  this->dataPtr->dropDirty = true;
}

/////////////////////////////////////////////////
void SimpleRenderer::newMouseEvent(const ignition::common::MouseEvent& event)
{
  std::lock_guard<std::mutex> lock(this->dataPtr->mutex);
  this->dataPtr->mouseEvent = event;
  this->dataPtr->mouseDirty = true;
}

/////////////////////////////////////////////////
ignition::math::Vector3d SimpleRenderer::screenToScene(const ignition::math::Vector2i& screen_pos) const
{
  // TODO(ahcorde): Replace this code with function in ign-rendering
  // Require this commit
  // https://github.com/ignitionrobotics/ign-rendering/pull/363
  // in ign-rendering6

  // Normalize point on the image
  double width = this->dataPtr->camera->ImageWidth();
  double height = this->dataPtr->camera->ImageHeight();

  double nx = 2.0 * screen_pos.X() / width - 1.0;
  double ny = 1.0 - 2.0 * screen_pos.Y() / height;

  // Make a ray query
  this->dataPtr->rayQuery->SetFromCamera(this->dataPtr->camera, ignition::math::Vector2d(nx, ny));

  auto result = this->dataPtr->rayQuery->ClosestPoint();
  if (result)
    return result.point;

  // Set point to be 10m away if no intersection found
  return this->dataPtr->rayQuery->Origin() + this->dataPtr->rayQuery->Direction() * 10;
}

/// \brief Private data class for RenderWindowItem
class SimpleRenderWidgetImpl
{
public:
  /** @brief Keep latest mouse event */
  ignition::common::MouseEvent mouseEvent;

  /** @brief The rendering renderer */
  SimpleRenderer renderer;

  /** @brief The view controller */
  InteractiveViewControl::Ptr view_controller;

  /** @brief The texture blitter to draw the texture on the widget */
  QOpenGLTextureBlitter texture_blitter;

  /** @brief The active render texture id */
  unsigned int texture_id {0};

  /** @brief The offscreen surface used by the render engine */
  QOffscreenSurface* surface{ nullptr };

  /** @brief The shared context used by the render engine */
  QOpenGLContext* context{ nullptr };

  /** @brief List of our QT connections. */
  QList<QMetaObject::Connection> connections;
};

/////////////////////////////////////////////////
SimpleRenderWidget::SimpleRenderWidget(const std::string& scene_name, QWidget* parent)
  : QOpenGLWidget(parent), data_(std::make_unique<SimpleRenderWidgetImpl>())
{
  data_->renderer.scene_name = scene_name;
  data_->view_controller = std::make_shared<InteractiveViewControl>(scene_name);

  // Force render update on resize
  connect(this, &QOpenGLWidget::resized, this, &SimpleRenderWidget::onResized);
}

/////////////////////////////////////////////////
SimpleRenderWidget::~SimpleRenderWidget()
{
  this->data_->context->makeCurrent(this->data_->surface);

  //  delete this->renderer;
  //  this->renderer = nullptr;

  this->data_->context->doneCurrent();

  // schedule this to be deleted only after we're done cleaning up
  this->data_->surface->deleteLater();

  delete this->data_->context;
  this->data_->context = nullptr;

  // Disconnect our QT connections.
  for (const auto& conn : this->data_->connections)
    QObject::disconnect(conn);
}

void SimpleRenderWidget::initializeGL()
{
  initializeOpenGLFunctions();

  // Create the texture blitter
  this->data_->texture_blitter.create();

  {  // Initialize renderer
    // Some GL implementations requres that the currently bound context is
    // made non-current before we set up sharing, so we doneCurrent here
    // and makeCurrent down below while setting up our own context.
    doneCurrent();

    this->data_->context = new QOpenGLContext();

    // set the surface format (this is managed globally in Main.cpp)
    // auto surfaceFormat = RenderThread::createSurfaceFormat();
    // m_renderThread->context->setFormat(surfaceFormat);
    this->data_->context->setFormat(context()->format());

    this->data_->context->setShareContext(context());
    this->data_->context->create();

    // Run on the Main (GUI = QML) thread
    this->data_->surface = new QOffscreenSurface();
    this->data_->surface->setFormat(this->data_->context->format());
    this->data_->surface->create();

    // carry out any initialisation before moving to thread
    this->data_->context->makeCurrent(this->data_->surface);

    // create renderer
    if (!this->data_->renderer.initialized)
      this->data_->renderer.initialize();

    this->data_->context->doneCurrent();
  }

  // check if engine has been successfully initialized
  if (!this->data_->renderer.initialized)
  {
    ignerr << "Unable to initialize renderer" << std::endl;
  }
}

void SimpleRenderWidget::resizeGL(int w, int h)
{
  this->data_->renderer.resize(w, h);
}

/////////////////////////////////////////////////
void SimpleRenderWidget::paintGL()
{
  this->data_->context->makeCurrent(this->data_->surface);
  this->data_->renderer.render();
  this->data_->context->doneCurrent();

  makeCurrent();
  this->data_->texture_blitter.bind(GL_TEXTURE_2D);
  const QRect target_rect(QPoint(0, 0), size());
  const QRect viewport_rect = QRect(QPoint(0, 0), this->data_->renderer.texture_size);
  QMatrix4x4 target = QOpenGLTextureBlitter::targetTransform(target_rect, viewport_rect);
  this->data_->texture_blitter.blit(this->data_->renderer.texture_id, target, QOpenGLTextureBlitter::OriginTopLeft);
  this->data_->texture_blitter.release();
  doneCurrent();
}

/////////////////////////////////////////////////
void SimpleRenderWidget::setBackgroundColor(const ignition::math::Color& color)
{
  this->data_->renderer.background_color = color;
}

/////////////////////////////////////////////////
void SimpleRenderWidget::setAmbientLight(const ignition::math::Color& ambient)
{
  this->data_->renderer.ambient_light = ambient;
}

/////////////////////////////////////////////////
void SimpleRenderWidget::setEngineName(const std::string& name) { this->data_->renderer.engine_name = name; }

/////////////////////////////////////////////////
void SimpleRenderWidget::setSceneName(const std::string& name) { this->data_->renderer.scene_name = name; }

/////////////////////////////////////////////////
void SimpleRenderWidget::setCameraPose(const ignition::math::Pose3d& pose)
{
  this->data_->renderer.cameraPose = pose;
}

/////////////////////////////////////////////////
void SimpleRenderWidget::setCameraNearClip(double near) { this->data_->renderer.camera_near_clip = near; }

/////////////////////////////////////////////////
void SimpleRenderWidget::setCameraFarClip(double far) { this->data_->renderer.camera_far_clip = far; }

/////////////////////////////////////////////////
void SimpleRenderWidget::setSkyEnabled(bool enabled) { this->data_->renderer.sky_enable = enabled; }

/////////////////////////////////////////////////
void SimpleRenderWidget::setGridEnabled(bool enabled) { this->data_->renderer.grid_enable = enabled; }

/////////////////////////////////////////////////
// void MinimalScene::LoadConfig(const tinyxml2::XMLElement *_pluginElem)
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
void SimpleRenderWidget::onHovered(int mouse_x, int mouse_y)
{
  this->data_->renderer.newHoverEvent({ mouse_x, mouse_y });
  update();
}

/////////////////////////////////////////////////
void SimpleRenderWidget::onDropped(const QString& drop, int mouse_x, int mouse_y)
{
  this->data_->renderer.newDropEvent(drop.toStdString(), { mouse_x, mouse_y });
  update();
}

void SimpleRenderWidget::onResized() { update(); }

/////////////////////////////////////////////////
void SimpleRenderWidget::mousePressEvent(QMouseEvent* event)
{
  this->data_->mouseEvent = convert(*event);
  this->data_->mouseEvent.SetPressPos(this->data_->mouseEvent.Pos());

  this->data_->renderer.newMouseEvent(this->data_->mouseEvent);
  update();
}

////////////////////////////////////////////////
void SimpleRenderWidget::keyPressEvent(QKeyEvent* event)
{
  if (event->isAutoRepeat())
    return;

  auto e = convert(*event);
  this->handleKeyPress(e);
  update();
}

////////////////////////////////////////////////
void SimpleRenderWidget::keyReleaseEvent(QKeyEvent* event)
{
  if (event->isAutoRepeat())
    return;

  auto e = convert(*event);
  this->handleKeyPress(e);
  update();
}

////////////////////////////////////////////////
void SimpleRenderWidget::mouseReleaseEvent(QMouseEvent* event)
{
  // Store values that depend on previous events
  auto pressPos = this->data_->mouseEvent.PressPos();
  auto dragging = this->data_->mouseEvent.Dragging();

  this->data_->mouseEvent = convert(*event);
  this->data_->mouseEvent.SetPressPos(pressPos);
  this->data_->mouseEvent.SetDragging(dragging);

  this->data_->renderer.newMouseEvent(this->data_->mouseEvent);
  update();
}

////////////////////////////////////////////////
void SimpleRenderWidget::mouseMoveEvent(QMouseEvent* event)
{
  // Store values that depend on previous events
  auto pressPos = this->data_->mouseEvent.PressPos();

  this->data_->mouseEvent = convert(*event);

  if (this->data_->mouseEvent.Dragging())
    this->data_->mouseEvent.SetPressPos(pressPos);

  this->data_->renderer.newMouseEvent(this->data_->mouseEvent);
  update();
}

////////////////////////////////////////////////
void SimpleRenderWidget::wheelEvent(QWheelEvent* event)
{
  //  this->forceActiveFocus();

  this->data_->mouseEvent = convert(*event);
  this->data_->renderer.newMouseEvent(this->data_->mouseEvent);
  update();
}

////////////////////////////////////////////////
void SimpleRenderWidget::handleKeyPress(const ignition::common::KeyEvent& event)
{
  this->data_->renderer.handleKeyPress(event);
  update();
}

////////////////////////////////////////////////
void SimpleRenderWidget::handleKeyRelease(const ignition::common::KeyEvent& event)
{
  this->data_->renderer.handleKeyRelease(event);
  update();
}

}  // namespace tesseract_gui
