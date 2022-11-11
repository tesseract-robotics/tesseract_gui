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

#include <tesseract_ignition/qpainter_render_widget.h>
#include <tesseract_ignition/gui_utils.h>
#include <tesseract_ignition/gui_events.h>
#include <tesseract_ignition/conversions.h>

#include <algorithm>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include <QPainter>

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
#include <ignition/common/Image.hh>

#ifdef _MSC_VER
#pragma warning(pop)
#endif

namespace tesseract_gui
{
// \brief Private data class for QPainterRenderer
class QPainterRendererImpl
{
public:
  /// \brief Flag to indicate if mouse event is dirty
  bool mouseDirty{ false };

  /// \brief Flag to indicate if hover event is dirty
  bool hoverDirty{ false };

  /// \brief Flag to indicate if drop event is dirty
  bool dropDirty{ false };

  /// \brief Mouse event
  ignition::common::MouseEvent mouseEvent;

  /// \brief Key event
  ignition::common::KeyEvent keyEvent;

  /// \brief Mutex to protect mouse events
  std::mutex mutex;

  /// \brief User camera
  ignition::rendering::CameraPtr camera{ nullptr };

  /// \brief The currently hovered mouse position in screen coordinates
  ignition::math::Vector2i mouseHoverPos{ ignition::math::Vector2i::Zero };

  /// \brief The currently drop mouse position in screen coordinates
  ignition::math::Vector2i mouseDropPos{ ignition::math::Vector2i::Zero };

  /// \brief The dropped text in the scene
  std::string dropText;

  /// \brief Ray query for mouse clicks
  ignition::rendering::RayQueryPtr rayQuery{ nullptr };

  /// \brief View control focus target
  ignition::math::Vector3d target;
};

/////////////////////////////////////////////////
QPainterRenderer::QPainterRenderer() : dataPtr(std::make_unique<QPainterRendererImpl>()) {}

/////////////////////////////////////////////////
QImage QPainterRenderer::Render()
{
  if (!initialized)
    return {};

  if (this->textureDirty)
  {
    std::lock_guard<std::mutex> lock(this->dataPtr->mutex);
    this->dataPtr->camera->SetImageWidth(this->textureSize.width());
    this->dataPtr->camera->SetImageHeight(this->textureSize.height());
    this->dataPtr->camera->SetAspectRatio(static_cast<double>(this->textureSize.width()) /
                                          static_cast<double>(this->textureSize.height()));
    // setting the size should cause the render texture to be rebuilt
    this->dataPtr->camera->PreRender();
    this->textureDirty = false;
  }

  // view control
  this->HandleMouseEvent();

  if (tesseract_gui::getApp() != nullptr)
  {
    QApplication::sendEvent(tesseract_gui::getApp(), new events::PreRender(this->sceneName));
  }

  ignition::rendering::Image image;
  {
    std::lock_guard<std::mutex> lock(this->dataPtr->mutex);
    image = this->dataPtr->camera->CreateImage();
    this->dataPtr->camera->Capture(image);
  }

  if (tesseract_gui::getApp() != nullptr)
  {
    QApplication::sendEvent(tesseract_gui::getApp(), new events::Render(this->sceneName));
  }

  return QImage(image.Data<unsigned char>(), image.Width(), image.Height(), QImage::Format_RGB888);
}

void QPainterRenderer::Resize(int width, int height)
{
  std::lock_guard<std::mutex> lock(this->dataPtr->mutex);
  this->textureSize = QSize(width, height);
  this->textureDirty = true;
}

/////////////////////////////////////////////////
void QPainterRenderer::HandleMouseEvent()
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
void QPainterRenderer::HandleKeyPress(const ignition::common::KeyEvent& _e)
{
  std::lock_guard<std::mutex> lock(this->dataPtr->mutex);

  this->dataPtr->keyEvent = _e;

  this->dataPtr->mouseEvent.SetControl(this->dataPtr->keyEvent.Control());
  this->dataPtr->mouseEvent.SetShift(this->dataPtr->keyEvent.Shift());
  this->dataPtr->mouseEvent.SetAlt(this->dataPtr->keyEvent.Alt());
}

////////////////////////////////////////////////
void QPainterRenderer::HandleKeyRelease(const ignition::common::KeyEvent& _e)
{
  std::lock_guard<std::mutex> lock(this->dataPtr->mutex);

  this->dataPtr->keyEvent = _e;

  this->dataPtr->mouseEvent.SetControl(this->dataPtr->keyEvent.Control());
  this->dataPtr->mouseEvent.SetShift(this->dataPtr->keyEvent.Shift());
  this->dataPtr->mouseEvent.SetAlt(this->dataPtr->keyEvent.Alt());
}

/////////////////////////////////////////////////
void QPainterRenderer::BroadcastDrop()
{
  if (!this->dataPtr->dropDirty)
    return;
  events::DropOnScene dropOnSceneEvent(this->dataPtr->dropText, this->dataPtr->mouseDropPos, this->sceneName);
  QApplication::sendEvent(tesseract_gui::getApp(), &dropOnSceneEvent);
  this->dataPtr->dropDirty = false;
}

/////////////////////////////////////////////////
void QPainterRenderer::BroadcastHoverPos()
{
  if (!this->dataPtr->hoverDirty)
    return;

  auto pos = this->ScreenToScene(this->dataPtr->mouseHoverPos);

  events::HoverToScene hoverToSceneEvent(pos, this->sceneName);
  QApplication::sendEvent(tesseract_gui::getApp(), &hoverToSceneEvent);

  ignition::common::MouseEvent hoverMouseEvent = this->dataPtr->mouseEvent;
  hoverMouseEvent.SetPos(this->dataPtr->mouseHoverPos);
  hoverMouseEvent.SetDragging(false);
  hoverMouseEvent.SetType(ignition::common::MouseEvent::MOVE);
  events::HoverOnScene hoverOnSceneEvent(hoverMouseEvent, this->sceneName);
  QApplication::sendEvent(tesseract_gui::getApp(), &hoverOnSceneEvent);

  this->dataPtr->hoverDirty = false;
}

/////////////////////////////////////////////////
void QPainterRenderer::BroadcastDrag()
{
  if (!this->dataPtr->mouseDirty)
    return;

  // Only broadcast drag if dragging
  if (!this->dataPtr->mouseEvent.Dragging())
    return;

  events::DragOnScene dragEvent(this->dataPtr->mouseEvent, this->sceneName);
  QApplication::sendEvent(tesseract_gui::getApp(), &dragEvent);

  this->dataPtr->mouseDirty = false;
}

/////////////////////////////////////////////////
void QPainterRenderer::BroadcastLeftClick()
{
  if (!this->dataPtr->mouseDirty)
    return;

  if (this->dataPtr->mouseEvent.Button() != ignition::common::MouseEvent::LEFT ||
      this->dataPtr->mouseEvent.Type() != ignition::common::MouseEvent::RELEASE)
    return;

  auto pos = this->ScreenToScene(this->dataPtr->mouseEvent.Pos());

  events::LeftClickToScene leftClickToSceneEvent(pos, this->sceneName);
  QApplication::sendEvent(tesseract_gui::getApp(), &leftClickToSceneEvent);

  events::LeftClickOnScene leftClickOnSceneEvent(this->dataPtr->mouseEvent, this->sceneName);
  QApplication::sendEvent(tesseract_gui::getApp(), &leftClickOnSceneEvent);

  this->dataPtr->mouseDirty = false;
}

/////////////////////////////////////////////////
void QPainterRenderer::BroadcastRightClick()
{
  if (!this->dataPtr->mouseDirty)
    return;

  if (this->dataPtr->mouseEvent.Button() != ignition::common::MouseEvent::RIGHT ||
      this->dataPtr->mouseEvent.Type() != ignition::common::MouseEvent::RELEASE)
    return;

  auto pos = this->ScreenToScene(this->dataPtr->mouseEvent.Pos());

  events::RightClickToScene rightClickToSceneEvent(pos, this->sceneName);
  QApplication::sendEvent(tesseract_gui::getApp(), &rightClickToSceneEvent);

  events::RightClickOnScene rightClickOnSceneEvent(this->dataPtr->mouseEvent, this->sceneName);
  QApplication::sendEvent(tesseract_gui::getApp(), &rightClickOnSceneEvent);

  this->dataPtr->mouseDirty = false;
}

/////////////////////////////////////////////////
void QPainterRenderer::BroadcastMousePress()
{
  if (!this->dataPtr->mouseDirty)
    return;

  if (this->dataPtr->mouseEvent.Type() != ignition::common::MouseEvent::PRESS)
    return;

  events::MousePressOnScene event(this->dataPtr->mouseEvent, this->sceneName);
  QApplication::sendEvent(tesseract_gui::getApp(), &event);

  this->dataPtr->mouseDirty = false;
}

/////////////////////////////////////////////////
void QPainterRenderer::BroadcastScroll()
{
  if (!this->dataPtr->mouseDirty)
    return;

  if (this->dataPtr->mouseEvent.Type() != ignition::common::MouseEvent::SCROLL)
    return;

  events::ScrollOnScene scrollOnSceneEvent(this->dataPtr->mouseEvent, this->sceneName);
  QApplication::sendEvent(tesseract_gui::getApp(), &scrollOnSceneEvent);

  this->dataPtr->mouseDirty = false;
}

/////////////////////////////////////////////////
void QPainterRenderer::BroadcastKeyRelease()
{
  if (this->dataPtr->keyEvent.Type() != ignition::common::KeyEvent::RELEASE)
    return;

  events::KeyReleaseOnScene keyRelease(this->dataPtr->keyEvent, this->sceneName);
  QApplication::sendEvent(tesseract_gui::getApp(), &keyRelease);

  this->dataPtr->keyEvent.SetType(ignition::common::KeyEvent::NO_EVENT);
}

/////////////////////////////////////////////////
void QPainterRenderer::BroadcastKeyPress()
{
  if (this->dataPtr->keyEvent.Type() != ignition::common::KeyEvent::PRESS)
    return;

  events::KeyPressOnScene keyPress(this->dataPtr->keyEvent, this->sceneName);
  QApplication::sendEvent(tesseract_gui::getApp(), &keyPress);

  this->dataPtr->keyEvent.SetType(ignition::common::KeyEvent::NO_EVENT);
}

/////////////////////////////////////////////////
void QPainterRenderer::Initialize()
{
  if (this->initialized)
    return;

  std::lock_guard<std::mutex> lock(this->dataPtr->mutex);
  std::map<std::string, std::string> params;
  params["useCurrentGLContext"] = "1";
  //  params["winID"] = std::to_string(tesseract_gui::getApp()->activeWindow()->winId());
  //  params["winID"] = std::to_string(
  //    ignition::gui::App()->findChild<ignition::gui::MainWindow *>()->
  //      QuickWindow()-winId());
  auto* engine = ignition::rendering::engine(this->engineName, params);
  if (engine == nullptr)
  {
    ignerr << "Engine [" << this->engineName << "] is not supported" << std::endl;
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
  this->dataPtr->camera->SetNearClipPlane(this->cameraNearClip);
  this->dataPtr->camera->SetFarClipPlane(this->cameraFarClip);
  this->dataPtr->camera->SetImageWidth(this->textureSize.width());
  this->dataPtr->camera->SetImageHeight(this->textureSize.height());
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

  {
    auto image = this->dataPtr->camera->CreateImage();
    this->dataPtr->camera->Capture(image);
    auto* data = image.Data<unsigned char>();
    ignition::common::Image tmp;
    tmp.SetFromData(data, image.Width(), image.Height(), ignition::common::Image::RGB_INT8);
    tmp.SavePNG("/tmp/initial_image.png");
  }

  // Ray Query
  this->dataPtr->rayQuery = this->dataPtr->camera->Scene()->CreateRayQuery();

  this->initialized = true;
}

/////////////////////////////////////////////////
void QPainterRenderer::Destroy()
{
  auto* engine = ignition::rendering::engine(this->engineName);
  if (engine == nullptr)
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
void QPainterRenderer::NewHoverEvent(const ignition::math::Vector2i& _hoverPos)
{
  std::lock_guard<std::mutex> lock(this->dataPtr->mutex);
  this->dataPtr->mouseHoverPos = _hoverPos;
  this->dataPtr->hoverDirty = true;
}

/////////////////////////////////////////////////
void QPainterRenderer::NewDropEvent(const std::string& _dropText, const ignition::math::Vector2i& _dropPos)
{
  std::lock_guard<std::mutex> lock(this->dataPtr->mutex);
  this->dataPtr->dropText = _dropText;
  this->dataPtr->mouseDropPos = _dropPos;
  this->dataPtr->dropDirty = true;
}

/////////////////////////////////////////////////
void QPainterRenderer::NewMouseEvent(const ignition::common::MouseEvent& _e)
{
  std::lock_guard<std::mutex> lock(this->dataPtr->mutex);
  this->dataPtr->mouseEvent = _e;
  this->dataPtr->mouseDirty = true;
}

/////////////////////////////////////////////////
ignition::math::Vector3d QPainterRenderer::ScreenToScene(const ignition::math::Vector2i& _screenPos) const
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
  return this->dataPtr->rayQuery->Origin() + this->dataPtr->rayQuery->Direction() * 10;
}

/// \brief Private data class for RenderWindowItem
class QPainterRenderWidgetImpl
{
public:
  /// \brief Keep latest mouse event
  ignition::common::MouseEvent mouseEvent;

  /// \brief Ign-rendering renderer
  QPainterRenderer renderer;

  QImage image;

  /// \brief List of our QT connections.
  QList<QMetaObject::Connection> connections;
};

/////////////////////////////////////////////////
QPainterRenderWidget::QPainterRenderWidget(const std::string& scene_name, QWidget* _parent)
  : QOpenGLWidget(_parent), dataPtr(std::make_unique<QPainterRenderWidgetImpl>())
{
  dataPtr->renderer.sceneName = scene_name;

  connect(this, &QOpenGLWidget::resized, this, &QPainterRenderWidget::OnResized);
}

/////////////////////////////////////////////////
QPainterRenderWidget::~QPainterRenderWidget()
{
  // Disconnect our QT connections.
  for (const auto& conn : this->dataPtr->connections)
    QObject::disconnect(conn);
}

void QPainterRenderWidget::initializeGL()
{
  //  initializeOpenGLFunctions();

  //  // Initialize renderer
  //  if (!this->dataPtr->renderer.initialized)
  //    this->dataPtr->renderer.Initialize();

  //  // check if engine has been successfully initialized
  //  if (!this->dataPtr->renderer.initialized)
  //  {
  //    ignerr << "Unable to initialize renderer" << std::endl;
  //  }
}

/////////////////////////////////////////////////
void QPainterRenderWidget::paintEvent(QPaintEvent* event)
{
  //  makeCurrent();
  //  this->dataPtr->image = this->dataPtr->renderer.Render();
  //  this->dataPtr->image.save("/tmp/ign_qimage.png");
  //  doneCurrent();

  QPainter painter;
  painter.begin(this);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.drawImage(QRect(0, 0, width(), height()), this->dataPtr->image);
  painter.drawImage(QRect(0, 0, width(), height()), QImage("/tmp/ign_qimage.png"));
  QRectF rectangle(10.0, 20.0, 80.0, 60.0);
  int startAngle = 30 * 16;
  int spanAngle = 120 * 16;

  painter.drawArc(rectangle, startAngle, spanAngle);
  painter.end();
}

/////////////////////////////////////////////////
void QPainterRenderWidget::SetBackgroundColor(const ignition::math::Color& _color)
{
  this->dataPtr->renderer.backgroundColor = _color;
}

/////////////////////////////////////////////////
void QPainterRenderWidget::SetAmbientLight(const ignition::math::Color& _ambient)
{
  this->dataPtr->renderer.ambientLight = _ambient;
}

/////////////////////////////////////////////////
void QPainterRenderWidget::SetEngineName(const std::string& _name) { this->dataPtr->renderer.engineName = _name; }

/////////////////////////////////////////////////
void QPainterRenderWidget::SetSceneName(const std::string& _name) { this->dataPtr->renderer.sceneName = _name; }

/////////////////////////////////////////////////
void QPainterRenderWidget::SetCameraPose(const ignition::math::Pose3d& _pose)
{
  this->dataPtr->renderer.cameraPose = _pose;
}

/////////////////////////////////////////////////
void QPainterRenderWidget::SetCameraNearClip(double _near) { this->dataPtr->renderer.cameraNearClip = _near; }

/////////////////////////////////////////////////
void QPainterRenderWidget::SetCameraFarClip(double _far) { this->dataPtr->renderer.cameraFarClip = _far; }

/////////////////////////////////////////////////
void QPainterRenderWidget::SetSkyEnabled(const bool& _sky) { this->dataPtr->renderer.skyEnable = _sky; }

void QPainterRenderWidget::SetGridEnabled(bool _grid) { this->dataPtr->renderer.gridEnable = _grid; }

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
void QPainterRenderWidget::OnHovered(int _mouseX, int _mouseY)
{
  this->dataPtr->renderer.NewHoverEvent({ _mouseX, _mouseY });
  update();
}

/////////////////////////////////////////////////
void QPainterRenderWidget::OnDropped(const QString& _drop, int _mouseX, int _mouseY)
{
  this->dataPtr->renderer.NewDropEvent(_drop.toStdString(), { _mouseX, _mouseY });
  update();
}

void QPainterRenderWidget::OnResized() { update(); }

/////////////////////////////////////////////////
void QPainterRenderWidget::mousePressEvent(QMouseEvent* _e)
{
  this->dataPtr->mouseEvent = convert(*_e);
  this->dataPtr->mouseEvent.SetPressPos(this->dataPtr->mouseEvent.Pos());

  this->dataPtr->renderer.NewMouseEvent(this->dataPtr->mouseEvent);
  update();
}

////////////////////////////////////////////////
void QPainterRenderWidget::keyPressEvent(QKeyEvent* _e)
{
  if (_e->isAutoRepeat())
    return;

  auto event = convert(*_e);
  this->HandleKeyPress(event);
  update();
}

////////////////////////////////////////////////
void QPainterRenderWidget::keyReleaseEvent(QKeyEvent* _e)
{
  if (_e->isAutoRepeat())
    return;

  auto event = convert(*_e);
  this->HandleKeyPress(event);
  update();
}

////////////////////////////////////////////////
void QPainterRenderWidget::mouseReleaseEvent(QMouseEvent* _e)
{
  // Store values that depend on previous events
  auto pressPos = this->dataPtr->mouseEvent.PressPos();
  auto dragging = this->dataPtr->mouseEvent.Dragging();

  this->dataPtr->mouseEvent = convert(*_e);
  this->dataPtr->mouseEvent.SetPressPos(pressPos);
  this->dataPtr->mouseEvent.SetDragging(dragging);

  this->dataPtr->renderer.NewMouseEvent(this->dataPtr->mouseEvent);
  update();
}

////////////////////////////////////////////////
void QPainterRenderWidget::mouseMoveEvent(QMouseEvent* _e)
{
  // Store values that depend on previous events
  auto pressPos = this->dataPtr->mouseEvent.PressPos();

  this->dataPtr->mouseEvent = convert(*_e);

  if (this->dataPtr->mouseEvent.Dragging())
    this->dataPtr->mouseEvent.SetPressPos(pressPos);

  this->dataPtr->renderer.NewMouseEvent(this->dataPtr->mouseEvent);
  update();
}

////////////////////////////////////////////////
void QPainterRenderWidget::wheelEvent(QWheelEvent* _e)
{
  //  this->forceActiveFocus();

  this->dataPtr->mouseEvent = convert(*_e);
  this->dataPtr->renderer.NewMouseEvent(this->dataPtr->mouseEvent);
  update();
}

////////////////////////////////////////////////
void QPainterRenderWidget::HandleKeyPress(const ignition::common::KeyEvent& _e)
{
  this->dataPtr->renderer.HandleKeyPress(_e);
  update();
}

////////////////////////////////////////////////
void QPainterRenderWidget::HandleKeyRelease(const ignition::common::KeyEvent& _e)
{
  this->dataPtr->renderer.HandleKeyRelease(_e);
  update();
}

}  // namespace tesseract_gui
