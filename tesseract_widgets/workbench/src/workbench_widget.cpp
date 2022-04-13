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
#include <tesseract_widgets/workbench/workbench_widget.h>
#include <tesseract_widgets/environment/environment_widget.h>
#include <tesseract_widgets/joint_trajectory/joint_trajectory_widget.h>

#include "ui_workbench_widget.h"

#include <QVBoxLayout>

namespace tesseract_gui
{

struct WorkbenchWidgetImpl
{
  EnvironmentWidget* environment_widget;
  JointTrajectoryWidget* joint_trajectory_widget;
};

WorkbenchWidget::WorkbenchWidget(EnvironmentWidget* environment_widget, QWidget *parent)
  : QWidget(parent)
  , ui(std::make_unique<Ui::WorkbenchWidget>())
  , data_(std::make_unique<WorkbenchWidgetImpl>())
{
  ui->setupUi(this);
  data_->environment_widget = environment_widget;
  data_->joint_trajectory_widget = new JointTrajectoryWidget(); // NOLINT

  { // Add environment widget
    auto* layout = new QVBoxLayout(); // NOLINT
    layout->setMargin(0);
    layout->addWidget(environment_widget);
    ui->environment_tab->setLayout(layout);
  }

  { // Add joint trajectory widget
    auto* layout = new QVBoxLayout(); // NOLINT
    layout->setMargin(0);
    layout->addWidget(data_->joint_trajectory_widget);
    ui->trajectory_tab->setLayout(layout);
  }
}

WorkbenchWidget::~WorkbenchWidget() = default;

EnvironmentWidget& WorkbenchWidget::getEnvironmentWidget() { return *data_->environment_widget; }
const EnvironmentWidget& WorkbenchWidget::getEnvironmentWidget() const { return *data_->environment_widget; }

JointTrajectoryWidget& WorkbenchWidget::getJointTrajectoryWidget() { return *data_->joint_trajectory_widget; }
const JointTrajectoryWidget& WorkbenchWidget::getJointTrajectorWidget() const { return *data_->joint_trajectory_widget; }

void WorkbenchWidget::onConfigureJointTrajectoryEnvironment(tesseract_environment::Environment::Ptr environment,
                                                            const tesseract_environment::Commands& commands,
                                                            const tesseract_common::JointState& initial_state)
{

}
}
