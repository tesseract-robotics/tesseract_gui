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
#ifndef TESSERACT_WIDGETS_WORKBENCH_WORKBENCH_WIDGET_H
#define TESSERACT_WIDGETS_WORKBENCH_WORKBENCH_WIDGET_H

#include <QWidget>
#include <memory>
#include <tesseract_environment/environment.h>
#include <tesseract_common/joint_state.h>

namespace Ui {
class WorkbenchWidget;
}

namespace tesseract_gui
{
class EnvironmentWidget;
class JointTrajectoryWidget;

struct WorkbenchWidgetImpl;

class WorkbenchWidget : public QWidget
{
  Q_OBJECT

public:
  explicit WorkbenchWidget(EnvironmentWidget* environment_widget, QWidget *parent = nullptr);
  ~WorkbenchWidget();

  EnvironmentWidget& getEnvironmentWidget();
  const EnvironmentWidget& getEnvironmentWidget() const;

  JointTrajectoryWidget& getJointTrajectoryWidget();
  const JointTrajectoryWidget& getJointTrajectoryWidget() const;

public Q_SLOTS:
  virtual void onRender();

private Q_SLOTS:
  void onConfigureJointTrajectoryEnvironment(tesseract_environment::Environment::Ptr environment,
                                             const tesseract_environment::Commands& commands,
                                             const tesseract_common::JointState& initial_state);
private:
  std::unique_ptr<Ui::WorkbenchWidget> ui;
  std::unique_ptr<WorkbenchWidgetImpl> data_;
};
}
#endif // TESSERACT_WIDGETS_WORKBENCH_WORKBENCH_WIDGET_H
