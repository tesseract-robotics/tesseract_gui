/*
 * Copyright (C) 2021 Open Source Robotics Foundation
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

#include <string>
#include <mutex>
#include <QGuiApplication>

#include <ignition/common/MouseEvent.hh>
#include <ignition/common/Console.hh>

#include <ignition/rendering/Camera.hh>
#include <ignition/rendering/OrbitViewController.hh>
#include <ignition/rendering/OrthoViewController.hh>
#include <ignition/rendering/RenderingIface.hh>
#include <ignition/rendering/RayQuery.hh>
#include <ignition/rendering/Utils.hh>

#include <tesseract_ignition/gui_events.h>
#include <tesseract_ignition/gui_utils.h>
#include <tesseract_ignition/interactive_view_control.h>

/** @brief Private data class for InteractiveViewControl */
class tesseract_gui::InteractiveViewControlPrivate
{
public:
  /** @brief Perform rendering calls in the rendering thread */
  void onRender();

  /** @brief The scene key the control is associated with */
  std::string scene_name;

  /** @brief Flag to indicate if mouse event is dirty */
  bool mouse_dirty = false;

  /** @brief True to block orbiting with the mouse */
  bool block_orbit = false;

  /** @brief Mouse event */
  ignition::common::MouseEvent mouse_event;

  /** @brief Mouse move distance since last event */
  ignition::math::Vector2d drag;

  /** @brief User camera */
  ignition::rendering::CameraPtr camera{ nullptr };

  /** @brief View control focus target */
  ignition::math::Vector3d target;

  /** @brief Orbit view controller */
  ignition::rendering::OrbitViewController orbit_view_control;

  /** @brief Ortho view controller */
  ignition::rendering::OrthoViewController ortho_view_control;

  /** @brief Camera view controller */
  ignition::rendering::ViewController* view_control{ nullptr };

  /** @brief View controller */
  ViewControlType view_control_type{ ViewControlType::ORBIT };

  /** @brief Ray query for mouse clicks */
  ignition::rendering::RayQueryPtr rayQuery{ nullptr };

  /** @brief Pointer to the rendering scene */
  ignition::rendering::ScenePtr scene{ nullptr };

  /** @brief Mutex to protect View Controllers */
  std::mutex mutex;
};

using namespace tesseract_gui;

/////////////////////////////////////////////////
void InteractiveViewControlPrivate::onRender()
{
  if (!this->scene)
  {
    this->scene = sceneFromFirstRenderEngine(this->scene_name);
    if (!this->scene)
      return;

    for (unsigned int i = 0; i < this->scene->NodeCount(); ++i)
    {
      auto cam = std::dynamic_pointer_cast<ignition::rendering::Camera>(this->scene->NodeByIndex(i));
      if (cam)
      {
        bool isUserCamera = false;
        try
        {
          isUserCamera = std::get<bool>(cam->UserData("user-camera"));
        }
        catch (std::bad_variant_access&)
        {
          continue;
        }
        if (isUserCamera)
        {
          this->camera = cam;
          igndbg << "InteractiveViewControl plugin is moving camera [" << this->camera->Name() << "]" << std::endl;
          break;
        }
      }
    }

    if (!this->camera)
    {
      ignerr << "InteractiveViewControl camera is not available" << std::endl;
      return;
    }
    this->rayQuery = this->camera->Scene()->CreateRayQuery();
  }

  if (this->block_orbit)
  {
    this->drag = { 0, 0 };
    return;
  }

  if (!this->mouse_dirty)
    return;

  if (!this->camera)
    return;

  std::lock_guard<std::mutex> lock(this->mutex);

  if (this->view_control_type == ViewControlType::ORTHO)
    this->view_control = &this->ortho_view_control;
  else
    this->view_control = &this->orbit_view_control;

  this->view_control->SetCamera(this->camera);

  if (this->mouse_event.Type() == ignition::common::MouseEvent::SCROLL)
  {
    this->target = ignition::rendering::screenToScene(this->mouse_event.Pos(), this->camera, this->rayQuery);

    this->view_control->SetTarget(this->target);
    double distance = this->camera->WorldPosition().Distance(this->target);
    double amount = -this->drag.Y() * distance / 20.0;
    this->view_control->Zoom(amount);
  }
  else if (this->mouse_event.Type() == ignition::common::MouseEvent::PRESS)
  {
    this->target = ignition::rendering::screenToScene(this->mouse_event.PressPos(), this->camera, this->rayQuery);
    this->view_control->SetTarget(this->target);
  }
  else
  {
    // Pan with left button
    if (this->mouse_event.Buttons() & ignition::common::MouseEvent::LEFT)
    {
      if (Qt::ShiftModifier == QGuiApplication::queryKeyboardModifiers())
        this->view_control->Orbit(this->drag);
      else
        this->view_control->Pan(this->drag);
    }
    // Orbit with middle button
    else if (this->mouse_event.Buttons() & ignition::common::MouseEvent::MIDDLE)
    {
      this->view_control->Orbit(this->drag);
    }
    // Zoom with right button
    else if (this->mouse_event.Buttons() & ignition::common::MouseEvent::RIGHT)
    {
      double hfov = this->camera->HFOV().Radian();
      double vfov = 2.0f * atan(tan(hfov / 2.0F) / this->camera->AspectRatio());
      double distance = this->camera->WorldPosition().Distance(this->target);
      double amount =
          ((-this->drag.Y() / static_cast<double>(this->camera->ImageHeight())) * distance * tan(vfov / 2.0) * 6.0);
      this->view_control->Zoom(amount);
    }
  }
  this->drag = 0;
  this->mouse_dirty = false;
}

/////////////////////////////////////////////////
InteractiveViewControl::InteractiveViewControl(const std::string& scene_name, ViewControlType type)
  : data_(std::make_unique<InteractiveViewControlPrivate>())
{
  data_->scene_name = scene_name;
  data_->view_control_type = type;

  // Install event filter for interactive view controller
  getApp()->installEventFilter(this);
}

/////////////////////////////////////////////////
InteractiveViewControl::~InteractiveViewControl() = default;

/////////////////////////////////////////////////
void InteractiveViewControl::setViewController(ViewControlType type)
{
  std::lock_guard<std::mutex> lock(this->data_->mutex);
  this->data_->view_control_type = type;
}

/////////////////////////////////////////////////
bool InteractiveViewControl::eventFilter(QObject* obj, QEvent* event)
{
  if (event->type() == events::Render::kType)
  {
    assert(dynamic_cast<events::Render*>(event) != nullptr);
    auto* e = static_cast<events::Render*>(event);
    if (e->getSceneName() == this->data_->scene_name)
    {
      this->data_->onRender();
    }
  }
  else if (event->type() == events::LeftClickOnScene::kType)
  {
    assert(dynamic_cast<events::LeftClickOnScene*>(event) != nullptr);
    auto* e = static_cast<events::LeftClickOnScene*>(event);
    if (e->getSceneName() == this->data_->scene_name)
    {
      this->data_->mouse_dirty = true;
      this->data_->drag = ignition::math::Vector2d::Zero;
      this->data_->mouse_event = e->Mouse();
    }
  }
  else if (event->type() == events::MousePressOnScene::kType)
  {
    assert(dynamic_cast<events::MousePressOnScene*>(event) != nullptr);
    auto* e = static_cast<events::MousePressOnScene*>(event);
    if (e->getSceneName() == this->data_->scene_name)
    {
      this->data_->mouse_dirty = true;
      this->data_->drag = ignition::math::Vector2d::Zero;
      this->data_->mouse_event = e->Mouse();
    }
  }
  else if (event->type() == events::DragOnScene::kType)
  {
    assert(dynamic_cast<events::DragOnScene*>(event) != nullptr);
    auto* e = reinterpret_cast<events::DragOnScene*>(event);
    if (e->getSceneName() == this->data_->scene_name)
    {
      this->data_->mouse_dirty = true;

      auto dragStart = this->data_->mouse_event.Pos();
      auto dragInt = e->Mouse().Pos() - dragStart;
      auto dragDistance = ignition::math::Vector2d(dragInt.X(), dragInt.Y());

      this->data_->drag += dragDistance;

      this->data_->mouse_event = e->Mouse();
    }
  }
  else if (event->type() == events::ScrollOnScene::kType)
  {
    assert(dynamic_cast<events::ScrollOnScene*>(event) != nullptr);
    auto* e = static_cast<events::ScrollOnScene*>(event);
    if (e->getSceneName() == this->data_->scene_name)
    {
      this->data_->mouse_dirty = true;
      this->data_->drag += ignition::math::Vector2d(e->Mouse().Scroll().X(), e->Mouse().Scroll().Y());
      this->data_->mouse_event = e->Mouse();
    }
  }
  else if (event->type() == events::BlockOrbit::kType)
  {
    assert(dynamic_cast<events::BlockOrbit*>(event) != nullptr);
    auto* e = static_cast<events::BlockOrbit*>(event);
    if (e->getSceneName() == this->data_->scene_name)
    {
      this->data_->block_orbit = e->Block();
    }
  }

  // Standard event processing
  return QObject::eventFilter(obj, event);
}
