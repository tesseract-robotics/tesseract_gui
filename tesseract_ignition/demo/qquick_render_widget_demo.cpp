/**
 * @author Levi Armstrong <levi.armstrong@gmail.com>
 *
 * @copyright Copyright (C) 2022 Levi Armstrong <levi.armstrong@gmail.com>
 *
 * @par License
 * GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
 * @par
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 * @par
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * @par
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#include <QApplication>
#include <QDebug>
#include <QtQuickWidgets/QtQuickWidgets>
#include <QOpenGLContext>
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <tesseract_ignition/minimal_scene.h>
#include <tesseract_ignition/interactive_view_control.h>

int main(int argc, char** argv)
{
  QApplication app(argc, argv);

  Q_INIT_RESOURCE(tesseract_qt_resources);
  Q_INIT_RESOURCE(minimal_scene);

  qmlRegisterType<tesseract_gui::RenderWindowItem>("RenderWindow", 1, 0, "RenderWindow");

  QUrl url("/home/levi/catkin_ws/tesseract_gui_ws/devel/include/tesseract_gui/rendering/MinimalScene.qml");

  //    QQuickWidget widget;
  //    widget.setResizeMode(QQuickWidget::SizeRootObjectToView);
  ////    QUrl url(":/tesseract_gui/MinimalScene.qml");
  //    widget.setSource(url);
  //    widget.show();

  QQuickView view;
  view.setResizeMode(QQuickView::SizeRootObjectToView);
  view.setSource(url);
  view.show();

  tesseract_gui::InteractiveViewControl view_control("scene");
  app.installEventFilter(&view_control);

  //    QList<QQmlError> errors = view.errors();
  //    for (const auto& error : errors)
  //      qCritical() << error.toString();
  return app.exec();
}
