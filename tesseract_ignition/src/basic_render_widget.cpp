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

#include <tesseract_ignition/basic_render_widget.h>
#include <QPainter>

namespace tesseract_gui
{
/////////////////////////////////////////////////
BasicRenderWidget::BasicRenderWidget(QWidget* _parent) : QOpenGLWidget(_parent) { setAutoFillBackground(false); }

void BasicRenderWidget::paintEvent(QPaintEvent* event)
{
  QImage q_image("/tmp/initial_image.png");
  QPainter painter;
  painter.begin(this);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.drawImage(QRect(0, 0, width(), height()), q_image);
  painter.save();
  painter.end();
}

}  // namespace tesseract_gui
