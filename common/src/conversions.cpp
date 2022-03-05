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

#include <tesseract_gui/common/conversions.h>

namespace tesseract_gui
{

//////////////////////////////////////////////////
QColor convert(const ignition::math::Color &_color)
{
  return QColor(_color.R()*255.0,
                _color.G()*255.0,
                _color.B()*255.0,
                _color.A()*255.0);
}

//////////////////////////////////////////////////
ignition::math::Color convert(const QColor &_color)
{
  return ignition::math::Color(_color.red() / 255.0,
                       _color.green() / 255.0,
                       _color.blue() / 255.0,
                       _color.alpha() / 255.0);
}

//////////////////////////////////////////////////
QPointF convert(const ignition::math::Vector2d &_pt)
{
  return QPointF(_pt.X(), _pt.Y());
}

//////////////////////////////////////////////////
ignition::math::Vector2d convert(const QPointF &_pt)
{
  return ignition::math::Vector2d(_pt.x(), _pt.y());
}

//////////////////////////////////////////////////
QVector3D convert(const ignition::math::Vector3d &_vec)
{
  return QVector3D(_vec.X(), _vec.Y(), _vec.Z());
}

//////////////////////////////////////////////////
ignition::math::Vector3d convert(const QVector3D &_vec)
{
  return ignition::math::Vector3d(_vec.x(), _vec.y(), _vec.z());
}

//////////////////////////////////////////////////
ignition::common::MouseEvent convert(const QMouseEvent &_e)
{
  ignition::common::MouseEvent event;
  event.SetPos(_e.pos().x(), _e.pos().y());

  // Button
  if (_e.button() == Qt::LeftButton)
    event.SetButton(ignition::common::MouseEvent::LEFT);
  else if (_e.button() == Qt::RightButton)
    event.SetButton(ignition::common::MouseEvent::RIGHT);
  else if (_e.button() == Qt::MiddleButton)
    event.SetButton(ignition::common::MouseEvent::MIDDLE);

  // Buttons
  if (_e.buttons() & Qt::LeftButton)
    event.SetButtons(event.Buttons() | ignition::common::MouseEvent::LEFT);

  if (_e.buttons() & Qt::RightButton)
    event.SetButtons(event.Buttons() | ignition::common::MouseEvent::RIGHT);

  if (_e.buttons() & Qt::MiddleButton)
    event.SetButtons(event.Buttons() | ignition::common::MouseEvent::MIDDLE);

  // Type
  if (_e.type() == QEvent::MouseButtonPress)
    event.SetType(ignition::common::MouseEvent::PRESS);
  else if (_e.type() == QEvent::MouseButtonRelease)
    event.SetType(ignition::common::MouseEvent::RELEASE);
  else if (_e.type() == QEvent::MouseMove)
  {
    event.SetType(ignition::common::MouseEvent::MOVE);

    // Dragging
    if (_e.buttons() || _e.button())
      event.SetDragging(true);
  }

  // Modifiers
  if (_e.modifiers() & Qt::ShiftModifier)
    event.SetShift(true);

  if (_e.modifiers() & Qt::ControlModifier)
    event.SetControl(true);

  if (_e.modifiers() & Qt::AltModifier)
    event.SetAlt(true);

  return event;
}

//////////////////////////////////////////////////
ignition::common::MouseEvent convert(const QWheelEvent &_e)
{
  ignition::common::MouseEvent event;

  event.SetType(ignition::common::MouseEvent::SCROLL);
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
  event.SetPos(_e.x(), _e.y());
#else
  event.SetPos(_e.position().x(), _e.position().y());
#endif
  double scroll = (_e.angleDelta().y() > 0) ? -1.0 : 1.0;
  event.SetScroll(scroll, scroll);

  // Buttons
  if (_e.buttons() & Qt::LeftButton)
    event.SetButtons(event.Buttons() | ignition::common::MouseEvent::LEFT);

  if (_e.buttons() & Qt::RightButton)
    event.SetButtons(event.Buttons() | ignition::common::MouseEvent::RIGHT);

  if (_e.buttons() & Qt::MiddleButton)
    event.SetButtons(event.Buttons() | ignition::common::MouseEvent::MIDDLE);

  // Modifiers
  if (_e.modifiers() & Qt::ShiftModifier)
    event.SetShift(true);

  if (_e.modifiers() & Qt::ControlModifier)
    event.SetControl(true);

  if (_e.modifiers() & Qt::AltModifier)
    event.SetAlt(true);

  return event;
}

//////////////////////////////////////////////////
ignition::common::KeyEvent convert(const QKeyEvent &_e)
{
  ignition::common::KeyEvent event;
  event.SetKey(_e.key());
  event.SetText(_e.text().toStdString());

  if (_e.type() == QEvent::KeyPress)
  {
    event.SetType(ignition::common::KeyEvent::PRESS);
  }
  else if (_e.type() == QEvent::KeyRelease)
  {
    event.SetType(ignition::common::KeyEvent::RELEASE);
  }
  else
  {
    event.SetType(ignition::common::KeyEvent::NO_EVENT);
  }

  event.SetControl(_e.modifiers() & Qt::ControlModifier);
  event.SetShift(_e.modifiers() & Qt::ShiftModifier);
  event.SetAlt(_e.modifiers() & Qt::AltModifier);

  return event;
}
}
