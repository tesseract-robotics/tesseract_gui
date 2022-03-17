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

#include <tesseract_gui/common/gui_events.h>
#include <string>

namespace tesseract_gui::events
{

class Render::Implementation
{
public:
  /// The scene the event is associated with
  std::string scene_name;
};

class SnapIntervals::Implementation
{
public:
  /// \brief XYZ snapping values in meters, these values must be positive.
  ignition::math::Vector3d xyz;

  /// \brief RPY snapping values in degrees, these values must be
  /// positive.
  ignition::math::Vector3d rpy;

  /// \brief Scale snapping values - a multiplier of the current size,
  /// these values must be positive.
  ignition::math::Vector3d scale;

  /// The scene the event is associated with
  std::string scene_name;
};

class SpawnFromDescription::Implementation
{
public:
  /// \brief The string of the resource to be spawned.
  std::string description;

  /// The scene the event is associated with
  std::string scene_name;
};

class SpawnFromPath::Implementation
{
public:
  /// \brief The path of file to be previewed.
  std::string filePath;

  /// The scene the event is associated with
  std::string scene_name;
};

class HoverToScene::Implementation
{
public:
  /// \brief The 3D point over which the user is hovering.
  ignition::math::Vector3d point;

  /// The scene the event is associated with
  std::string scene_name;
};

class HoverOnScene::Implementation
{
public:
  /// \brief The 2D point over which the user is hovering.
  ignition::common::MouseEvent mouse;

  /// The scene the event is associated with
  std::string scene_name;
};

class LeftClickToScene::Implementation
{
public:
  /// \brief The 3D point that the user clicked within the scene.
  ignition::math::Vector3d point;

  /// The scene the event is associated with
  std::string scene_name;
};

class RightClickToScene::Implementation
{
public:
  /// \brief The 3D point that the user clicked within the scene.
  ignition::math::Vector3d point;

  /// The scene the event is associated with
  std::string scene_name;
};

class DropdownMenuEnabled::Implementation
{
public:
  /// \brief The boolean indicating whether the menu is disabled or not
  /// for this event.
  bool menuEnabled;

  /// The scene the event is associated with
  std::string scene_name;
};

class LeftClickOnScene::Implementation
{
public:
  /// \brief Mouse event
  ignition::common::MouseEvent mouse;

  /// The scene the event is associated with
  std::string scene_name;
};

class RightClickOnScene::Implementation
{
public:
  /// \brief Mouse event
  ignition::common::MouseEvent mouse;

  /// The scene the event is associated with
  std::string scene_name;
};

class BlockOrbit::Implementation
{
public:
  bool block;

  /// The scene the event is associated with
  std::string scene_name;
};

class KeyReleaseOnScene::Implementation
{
public:
  /// \brief Key event
  ignition::common::KeyEvent key;

  /// The scene the event is associated with
  std::string scene_name;
};

class KeyPressOnScene::Implementation
{
public:
  /// \brief Key event
  ignition::common::KeyEvent key;

  /// The scene the event is associated with
  std::string scene_name;
};

class SpawnCloneFromName::Implementation
{
public:
  /// \brief The name of the resource to be cloned
  std::string name;

  /// The scene the event is associated with
  std::string scene_name;
};

class DropOnScene::Implementation
{
public:
  /// \brief The name of the dropped thing
  std::string dropText;

  /// \brief X and Y position of the mouse
  ignition::math::Vector2i mouse;

  /// The scene the event is associated with
  std::string scene_name;
};

class ScrollOnScene::Implementation
{
public:
  /// \brief Mouse event with scroll information.
  ignition::common::MouseEvent mouse;

  /// The scene the event is associated with
  std::string scene_name;
};

class DragOnScene::Implementation
{
public:
  /// \brief Mouse event with drag information.
  ignition::common::MouseEvent mouse;

  /// The scene the event is associated with
  std::string scene_name;
};

class MousePressOnScene::Implementation
{
public:
  /// \brief Mouse event with press information.
  ignition::common::MouseEvent mouse;

  /// The scene the event is associated with
  std::string scene_name;
};

//class WorldControl::Implementation
//{
//public:
//  /// \brief WorldControl information.
//  ignition::msgs::WorldControl worldControl;
//};

class PreRender::Implementation
{
public:
  /// The scene the event is associated with
  std::string scene_name;
};

/////////////////////////////////////////////////
Render::Render(const std::string& scene_name)
  : QEvent(kType)
  , dataPtr(std::make_unique<Implementation>())
{
  this->dataPtr->scene_name = scene_name;
}
Render::~Render() = default;

/////////////////////////////////////////////////
const std::string& Render::getSceneName() const { return this->dataPtr->scene_name;}

/////////////////////////////////////////////////
SnapIntervals::SnapIntervals(const ignition::math::Vector3d &_xyz,
                             const ignition::math::Vector3d &_rpy,
                             const ignition::math::Vector3d &_scale,
                             const std::string &scene_name)
  : QEvent(kType)
  , dataPtr(std::make_unique<Implementation>())
{
  this->dataPtr->xyz = _xyz;
  this->dataPtr->rpy = _rpy;
  this->dataPtr->scale = _scale;
  this->dataPtr->scene_name = scene_name;
}

SnapIntervals::~SnapIntervals() = default;

/////////////////////////////////////////////////
const std::string& SnapIntervals::getSceneName() const { return this->dataPtr->scene_name;}

/////////////////////////////////////////////////
ignition::math::Vector3d SnapIntervals::Position() const
{
  return this->dataPtr->xyz;
}

/////////////////////////////////////////////////
ignition::math::Vector3d SnapIntervals::Rotation() const
{
  return this->dataPtr->rpy;
}

/////////////////////////////////////////////////
ignition::math::Vector3d SnapIntervals::Scale() const
{
  return this->dataPtr->scale;
}

/////////////////////////////////////////////////
SpawnFromDescription::SpawnFromDescription(const std::string &_description, const std::string &scene_name)
  : QEvent(kType)
  , dataPtr(std::make_unique<Implementation>())
{
  this->dataPtr->description = _description;
  this->dataPtr->scene_name = scene_name;
}
SpawnFromDescription::~SpawnFromDescription() = default;

/////////////////////////////////////////////////
const std::string& SpawnFromDescription::getSceneName() const { return this->dataPtr->scene_name;}

/////////////////////////////////////////////////
const std::string &SpawnFromDescription::Description() const
{
  return this->dataPtr->description;
}

/////////////////////////////////////////////////
SpawnFromPath::SpawnFromPath(const std::string &_filePath, const std::string &scene_name)
    : QEvent(kType)
    , dataPtr(std::make_unique<Implementation>())
{
  this->dataPtr->filePath = _filePath;
  this->dataPtr->scene_name = scene_name;
}
SpawnFromPath::~SpawnFromPath() = default;

/////////////////////////////////////////////////
const std::string &SpawnFromPath::FilePath() const
{
  return this->dataPtr->filePath;
}

/////////////////////////////////////////////////
HoverToScene::HoverToScene(const ignition::math::Vector3d &_point, const std::string &scene_name)
    : QEvent(kType)
    , dataPtr(std::make_unique<Implementation>())
{
  this->dataPtr->point = _point;
  this->dataPtr->scene_name = scene_name;
}
HoverToScene::~HoverToScene() = default;

/////////////////////////////////////////////////
const std::string& HoverToScene::getSceneName() const { return this->dataPtr->scene_name;}

/////////////////////////////////////////////////
ignition::math::Vector3d HoverToScene::Point() const
{
  return this->dataPtr->point;
}

/////////////////////////////////////////////////
HoverOnScene::HoverOnScene(const ignition::common::MouseEvent &_mouse, const std::string &scene_name)
    : QEvent(kType)
    , dataPtr(std::make_unique<Implementation>())
{
  this->dataPtr->mouse = _mouse;
  this->dataPtr->scene_name = scene_name;
}
HoverOnScene::~HoverOnScene() = default;

/////////////////////////////////////////////////
const std::string& HoverOnScene::getSceneName() const { return this->dataPtr->scene_name;}

/////////////////////////////////////////////////
ignition::common::MouseEvent HoverOnScene::Mouse() const
{
  return this->dataPtr->mouse;
}

/////////////////////////////////////////////////
LeftClickToScene::LeftClickToScene(const ignition::math::Vector3d &_point, const std::string &scene_name)
    : QEvent(kType)
    , dataPtr(std::make_unique<Implementation>())
{
  this->dataPtr->point = _point;
  this->dataPtr->scene_name = scene_name;
}
LeftClickToScene::~LeftClickToScene() = default;

/////////////////////////////////////////////////
const std::string& LeftClickToScene::getSceneName() const { return this->dataPtr->scene_name;}

/////////////////////////////////////////////////
ignition::math::Vector3d LeftClickToScene::Point() const
{
  return this->dataPtr->point;
}

/////////////////////////////////////////////////
RightClickToScene::RightClickToScene(const ignition::math::Vector3d &_point, const std::string &scene_name)
    : QEvent(kType)
    , dataPtr(std::make_unique<Implementation>())
{
  this->dataPtr->point = _point;
  this->dataPtr->scene_name = scene_name;
}
RightClickToScene::~RightClickToScene() = default;

/////////////////////////////////////////////////
const std::string& RightClickToScene::getSceneName() const { return this->dataPtr->scene_name;}

/////////////////////////////////////////////////
ignition::math::Vector3d RightClickToScene::Point() const
{
  return this->dataPtr->point;
}

/////////////////////////////////////////////////
DropdownMenuEnabled::DropdownMenuEnabled(bool _menuEnabled, const std::string &scene_name)
    : QEvent(kType)
    , dataPtr(std::make_unique<Implementation>())
{
  this->dataPtr->menuEnabled = _menuEnabled;
  this->dataPtr->scene_name = scene_name;
}
DropdownMenuEnabled::~DropdownMenuEnabled() = default;

/////////////////////////////////////////////////
const std::string& DropdownMenuEnabled::getSceneName() const { return this->dataPtr->scene_name;}

/////////////////////////////////////////////////
bool DropdownMenuEnabled::MenuEnabled() const
{
  return this->dataPtr->menuEnabled;
}

/////////////////////////////////////////////////
RightClickOnScene::RightClickOnScene(const ignition::common::MouseEvent &_mouse, const std::string &scene_name)
    : QEvent(kType)
    , dataPtr(std::make_unique<Implementation>())
{
  this->dataPtr->mouse = _mouse;
  this->dataPtr->scene_name = scene_name;
}
RightClickOnScene::~RightClickOnScene() = default;

/////////////////////////////////////////////////
const std::string& RightClickOnScene::getSceneName() const { return this->dataPtr->scene_name;}

/////////////////////////////////////////////////
const ignition::common::MouseEvent &RightClickOnScene::Mouse() const
{
  return this->dataPtr->mouse;
}

/////////////////////////////////////////////////
LeftClickOnScene::LeftClickOnScene(const ignition::common::MouseEvent &_mouse, const std::string &scene_name)
    : QEvent(kType)
    , dataPtr(std::make_unique<Implementation>())
{
  this->dataPtr->mouse = _mouse;
  this->dataPtr->scene_name = scene_name;
}
LeftClickOnScene::~LeftClickOnScene() = default;

/////////////////////////////////////////////////
const std::string& LeftClickOnScene::getSceneName() const { return this->dataPtr->scene_name;}

/////////////////////////////////////////////////
const ignition::common::MouseEvent &LeftClickOnScene::Mouse() const
{
  return this->dataPtr->mouse;
}

/////////////////////////////////////////////////
BlockOrbit::BlockOrbit(const bool &_block, const std::string &scene_name)
  : QEvent(kType)
  , dataPtr(std::make_unique<Implementation>())
{
  this->dataPtr->block = _block;
  this->dataPtr->scene_name = scene_name;
}
BlockOrbit::~BlockOrbit() = default;

/////////////////////////////////////////////////
const std::string& BlockOrbit::getSceneName() const { return this->dataPtr->scene_name;}

/////////////////////////////////////////////////
bool BlockOrbit::Block() const
{
  return this->dataPtr->block;
}

/////////////////////////////////////////////////
KeyReleaseOnScene::KeyReleaseOnScene(const ignition::common::KeyEvent &_key, const std::string &scene_name)
    : QEvent(kType)
    , dataPtr(std::make_unique<Implementation>())
{
  this->dataPtr->key = _key;
  this->dataPtr->scene_name = scene_name;
}
KeyReleaseOnScene::~KeyReleaseOnScene() = default;

/////////////////////////////////////////////////
const std::string& KeyReleaseOnScene::getSceneName() const { return this->dataPtr->scene_name;}

/////////////////////////////////////////////////
ignition::common::KeyEvent KeyReleaseOnScene::Key() const
{
  return this->dataPtr->key;
}

/////////////////////////////////////////////////
KeyPressOnScene::KeyPressOnScene(const ignition::common::KeyEvent &_key, const std::string &scene_name)
    : QEvent(kType), dataPtr(std::make_unique<Implementation>())
{
  this->dataPtr->key = _key;
  this->dataPtr->scene_name = scene_name;
}
KeyPressOnScene::~KeyPressOnScene() = default;

/////////////////////////////////////////////////
const std::string& KeyPressOnScene::getSceneName() const { return this->dataPtr->scene_name;}

/////////////////////////////////////////////////
ignition::common::KeyEvent KeyPressOnScene::Key() const
{
  return this->dataPtr->key;
}

/////////////////////////////////////////////////
SpawnCloneFromName::SpawnCloneFromName(const std::string &_name, const std::string &scene_name)
    : QEvent(kType)
    , dataPtr(std::make_unique<Implementation>())
{
  this->dataPtr->name = _name;
  this->dataPtr->scene_name = scene_name;
}

SpawnCloneFromName::~SpawnCloneFromName() = default;

/////////////////////////////////////////////////
const std::string& SpawnCloneFromName::getSceneName() const { return this->dataPtr->scene_name;}

/////////////////////////////////////////////////
const std::string &SpawnCloneFromName::Name() const
{
  return this->dataPtr->name;
}

/////////////////////////////////////////////////
DropOnScene::DropOnScene(const std::string &_dropText, const ignition::math::Vector2i &_dropMouse, const std::string &scene_name)
    : QEvent(kType)
    , dataPtr(std::make_unique<Implementation>())
{
  this->dataPtr->dropText = _dropText;
  this->dataPtr->mouse = _dropMouse;
  this->dataPtr->scene_name = scene_name;
}
DropOnScene::~DropOnScene() = default;

/////////////////////////////////////////////////
const std::string& DropOnScene::getSceneName() const { return this->dataPtr->scene_name;}

/////////////////////////////////////////////////
const std::string &DropOnScene::DropText() const
{
  return this->dataPtr->dropText;
}

/////////////////////////////////////////////////
const ignition::math::Vector2i &DropOnScene::Mouse() const
{
  return this->dataPtr->mouse;
}

/////////////////////////////////////////////////
ScrollOnScene::ScrollOnScene(const ignition::common::MouseEvent &_mouse, const std::string &scene_name)
    : QEvent(kType)
    , dataPtr(std::make_unique<Implementation>())
{
  this->dataPtr->mouse = _mouse;
  this->dataPtr->scene_name = scene_name;
}
ScrollOnScene::~ScrollOnScene() = default;

/////////////////////////////////////////////////
const std::string& ScrollOnScene::getSceneName() const { return this->dataPtr->scene_name;}

/////////////////////////////////////////////////
const ignition::common::MouseEvent &ScrollOnScene::Mouse() const
{
  return this->dataPtr->mouse;
}

/////////////////////////////////////////////////
DragOnScene::DragOnScene(const ignition::common::MouseEvent &_mouse, const std::string &scene_name)
    : QEvent(kType)
    , dataPtr(std::make_unique<Implementation>())
{
  this->dataPtr->mouse = _mouse;
  this->dataPtr->scene_name = scene_name;
}
DragOnScene::~DragOnScene() = default;

/////////////////////////////////////////////////
const std::string& DragOnScene::getSceneName() const { return this->dataPtr->scene_name;}

/////////////////////////////////////////////////
ignition::common::MouseEvent DragOnScene::Mouse() const
{
  return this->dataPtr->mouse;
}

/////////////////////////////////////////////////
MousePressOnScene::MousePressOnScene(const ignition::common::MouseEvent &_mouse, const std::string &scene_name)
    : QEvent(kType)
    , dataPtr(std::make_unique<Implementation>())
{
  this->dataPtr->mouse = _mouse;
  this->dataPtr->scene_name = scene_name;
}
MousePressOnScene::~MousePressOnScene() = default;

/////////////////////////////////////////////////
const std::string& MousePressOnScene::getSceneName() const { return this->dataPtr->scene_name;}

/////////////////////////////////////////////////
const ignition::common::MouseEvent &MousePressOnScene::Mouse() const
{
  return this->dataPtr->mouse;
}

/////////////////////////////////////////////////
//WorldControl::WorldControl(const ignition::msgs::WorldControl &_worldControl)
//  : QEvent(kType)
//  , dataPtr(std::make_unique<Implementation>())
//{
//  this->dataPtr->worldControl = _worldControl;
//}

/////////////////////////////////////////////////
//const ignition::msgs::WorldControl &WorldControl::WorldControlInfo() const
//{
//  return this->dataPtr->worldControl;
//}

/////////////////////////////////////////////////
PreRender::PreRender(const std::string &scene_name)
  : QEvent(kType)
  , dataPtr(std::make_unique<Implementation>())
{
  this->dataPtr->scene_name = scene_name;
}
PreRender::~PreRender() = default;

/////////////////////////////////////////////////
const std::string& PreRender::getSceneName() const { return this->dataPtr->scene_name;}
}
