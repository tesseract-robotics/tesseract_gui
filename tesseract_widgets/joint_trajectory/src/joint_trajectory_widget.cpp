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
#include "ui_joint_trajectory_widget.h"
#include <tesseract_widgets/joint_trajectory/joint_trajectory_plot_dialog.h>
#include <tesseract_widgets/joint_trajectory/joint_trajectory_widget.h>

#include <tesseract_widgets/plot/transforms/first_derivative.h>
#include <tesseract_widgets/plot/transforms/integral_transform.h>
#include <tesseract_widgets/plot/transforms/moving_average_filter.h>
#include <tesseract_widgets/plot/transforms/moving_rms.h>
#include <tesseract_widgets/plot/transforms/outlier_removal.h>
#include <tesseract_widgets/plot/transforms/scale_transform.h>

#include <tesseract_widgets/common/standard_item_type.h>

#include <tesseract_visualization/trajectory_player.h>
#include <QTimer>
#include <set>

const double SLIDER_RESOLUTION = 0.001;

namespace tesseract_gui
{
JointTrajectoryWidget::JointTrajectoryWidget(QWidget *parent)
  : QWidget(parent)
  , ui_(std::make_unique<Ui::JointTrajectoryWidget>())
{

  ui_->setupUi(this);
  ui_->trajectoryPlotButton->setIcon(QIcon(":/tesseract_widgets/png/chart.png"));

  player_ = std::make_unique<tesseract_visualization::TrajectoryPlayer>();
  player_timer_ = std::make_unique<QTimer>(this);
  player_timer_->start(10);

  connect(ui_->trajectoryPlayButton, SIGNAL(clicked()), this, SLOT(onPlayButtonClicked()));
  connect(ui_->trajectoryPauseButton, SIGNAL(clicked()), this, SLOT(onPauseButtonClicked()));
  connect(ui_->trajectorySlider, SIGNAL(valueChanged(int)), this, SLOT(onSliderValueChanged(int)));
  connect(player_timer_.get(), SIGNAL(timeout()), this, SLOT(onPlayerTimerTimeout()));
  connect(ui_->trajectoryPlotButton, SIGNAL(clicked()), this, SLOT(onPlotTrajectoryClicked()));

  TransformFactory::registerTransform<FirstDerivative>();
  TransformFactory::registerTransform<IntegralTransform>();
  TransformFactory::registerTransform<MovingAverageFilter>();
  TransformFactory::registerTransform<MovingRMS>();
  TransformFactory::registerTransform<OutlierRemovalFilter>();
  TransformFactory::registerTransform<ScaleTransform>();
}

JointTrajectoryWidget::~JointTrajectoryWidget() = default;

void JointTrajectoryWidget::setModel(JointTrajectoryModel* model)
{
  ui_->trajectoryTreeView->setModel(model);
  ui_->trajectoryTreeView->setColumnWidth(0, 175);
  model_ = model;

  connect(ui_->trajectoryTreeView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(onCurrentRowChanged(QModelIndex,QModelIndex)));
}

void JointTrajectoryWidget::onCurrentRowChanged(const QModelIndex &current, const QModelIndex &/*previous*/)
{
  QStandardItem* item = model_->itemFromIndex(current);
  switch (item->type())
  {
    case static_cast<int>(tesseract_gui::StandardItemType::JOINT_TRAJECTORY_SET_TRAJECTORY):
    {
      current_trajectory_ = model_->getJointTrajectory(current);

      const tesseract_common::JointTrajectorySet& traj_set = model_->getJointTrajectorySet(current);
      emit configureEnvironment(traj_set.getEnvironment(), traj_set.getEnvironmentCommands(), traj_set.getInitialState());

      player_->setTrajectory(current_trajectory_.trajectory);

      enablePlayer();

      break;
    }
    case static_cast<int>(tesseract_gui::StandardItemType::JOINT_TRAJECTORY_SET):
    {
      const tesseract_common::JointTrajectorySet& trajectory_set = model_->getJointTrajectorySet(current);
      current_trajectory_ = tesseract_common::JointTrajectoryInfo();
      current_trajectory_.initial_state = trajectory_set.getInitialState();
      for (const auto& t : trajectory_set.getJointTrajectories())
        current_trajectory_.trajectory.insert(current_trajectory_.trajectory.end(), t.trajectory.begin(), t.trajectory.end());

      const tesseract_common::JointTrajectorySet& traj_set = model_->getJointTrajectorySet(current);
      emit configureEnvironment(traj_set.getEnvironment(), traj_set.getEnvironmentCommands(), traj_set.getInitialState());

      player_->setTrajectory(current_trajectory_.trajectory);
      enablePlayer();
      break;
    }
    default:
    {
      disablePlayer();
      const tesseract_common::JointState& state = model_->getJointState(current);
      const tesseract_common::JointTrajectorySet& traj_set = model_->getJointTrajectorySet(current);
      emit configureEnvironment(traj_set.getEnvironment(), traj_set.getEnvironmentCommands(), traj_set.getInitialState());
      emit showState(state);
      break;
    }
  }
}

void JointTrajectoryWidget::onPauseButtonClicked()
{
  ui_->trajectoryPlayButton->setEnabled(true);
  ui_->trajectorySlider->setEnabled(true);
  ui_->trajectoryPauseButton->setEnabled(false);
}
void JointTrajectoryWidget::onPlayButtonClicked()
{
  ui_->trajectoryPlayButton->setEnabled(false);
  ui_->trajectorySlider->setEnabled(false);
  ui_->trajectoryPauseButton->setEnabled(true);
  player_->setCurrentDuration(current_duration_);
}

void JointTrajectoryWidget::onPlayerTimerTimeout()
{
  if (ui_->trajectoryPlayerFrame->isEnabled() && ui_->trajectoryPauseButton->isEnabled())
  {
    player_->getNext();
    ui_->trajectorySlider->setSliderPosition(player_->currentDuration() / SLIDER_RESOLUTION);
    if (player_->isFinished())
      onPauseButtonClicked();
  }
}

void JointTrajectoryWidget::onSliderValueChanged(int value)
{
  current_duration_ = value * SLIDER_RESOLUTION;
  tesseract_common::JointState state = player_->setCurrentDuration(current_duration_);
  ui_->trajectoryCurrentDurationLabel->setText(QString().sprintf("%0.3f", current_duration_));
  emit showState(state);
}

void JointTrajectoryWidget::onPlotTrajectoryClicked()
{
  if (current_trajectory_.trajectory.empty())
    return;

  plot_dialog_ = nullptr;
  plot_dialog_ = std::make_unique<JointTrajectoryPlotDialog>(current_trajectory_);
  plot_dialog_->setWindowFlags(Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
  plot_dialog_->show();
}

void JointTrajectoryWidget::enablePlayer()
{
  ui_->trajectoryPlayerFrame->setEnabled(true);
  ui_->trajectoryPlayButton->setEnabled(true);
  ui_->trajectoryPauseButton->setEnabled(false);
  ui_->trajectorySlider->setMinimum(0);
  ui_->trajectorySlider->setMaximum(player_->trajectoryDuration() / SLIDER_RESOLUTION);
  ui_->trajectorySlider->setSliderPosition(0);
  ui_->trajectoryCurrentDurationLabel->setText(QString().sprintf("%0.3f", player_->currentDuration()));
  ui_->trajectoryDurationLabel->setText(QString().sprintf("%0.3f", player_->trajectoryDuration()));
  current_duration_ = 0;
}

void JointTrajectoryWidget::disablePlayer()
{
  ui_->trajectoryPlayerFrame->setEnabled(false);
}

}
