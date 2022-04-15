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
#include <tesseract_widgets/joint_trajectory/joint_trajectory_model.h>

#include <tesseract_widgets/plot/transforms/first_derivative.h>
#include <tesseract_widgets/plot/transforms/integral_transform.h>
#include <tesseract_widgets/plot/transforms/moving_average_filter.h>
#include <tesseract_widgets/plot/transforms/moving_rms.h>
#include <tesseract_widgets/plot/transforms/outlier_removal.h>
#include <tesseract_widgets/plot/transforms/scale_transform.h>

#include <tesseract_widgets/common/standard_item_type.h>

#include <tesseract_common/joint_state.h>
#include <tesseract_visualization/trajectory_player.h>
#include <QTimer>
#include <set>

const double SLIDER_RESOLUTION = 0.001;

namespace tesseract_gui
{

struct JointTrajectoryWidgetPrivate
{
  JointTrajectoryModel* model{nullptr};
  std::unique_ptr<tesseract_visualization::TrajectoryPlayer> player;
  std::unique_ptr<QTimer> player_timer;
  std::unique_ptr<JointTrajectoryPlotDialog> plot_dialog;
  double current_duration{0};
  tesseract_common::JointTrajectoryInfo current_trajectory;
};

JointTrajectoryWidget::JointTrajectoryWidget(QWidget *parent)
  : QWidget(parent)
  , ui_(std::make_unique<Ui::JointTrajectoryWidget>())
  , data_(std::make_unique<JointTrajectoryWidgetPrivate>())
{
  ui_->setupUi(this);
  ui_->trajectoryPlotButton->setIcon(QIcon(":/tesseract_widgets/png/chart.png"));

  data_->player = std::make_unique<tesseract_visualization::TrajectoryPlayer>();
  data_->player_timer = std::make_unique<QTimer>(this);
  data_->player_timer->start(10);

  connect(ui_->trajectoryPlayButton, SIGNAL(clicked()), this, SLOT(onPlayButtonClicked()));
  connect(ui_->trajectoryPauseButton, SIGNAL(clicked()), this, SLOT(onPauseButtonClicked()));
  connect(ui_->trajectorySlider, SIGNAL(valueChanged(int)), this, SLOT(onSliderValueChanged(int)));
  connect(data_->player_timer.get(), SIGNAL(timeout()), this, SLOT(onPlayerTimerTimeout()));
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
  data_->model = model;

  connect(ui_->trajectoryTreeView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(onCurrentRowChanged(QModelIndex,QModelIndex)));
}

QString JointTrajectoryWidget::addJointTrajectorySet(const tesseract_common::JointTrajectorySet& trajectory_set)
{
  return data_->model->addJointTrajectorySet(trajectory_set);
}

void JointTrajectoryWidget::removeJointTrajectorySet(const QString& key)
{
  data_->model->removeJointTrajectorySet(key);
  emit jointTrajectorySetRemoved(key);
}

bool JointTrajectoryWidget::hasJointTrajectorySet(const QString& key)
{
  return data_->model->hasJointTrajectorySet(key);
}

void JointTrajectoryWidget::onCurrentRowChanged(const QModelIndex &current, const QModelIndex &/*previous*/)
{
  QStandardItem* item = data_->model->itemFromIndex(current);
  switch (item->type())
  {
    case static_cast<int>(tesseract_gui::StandardItemType::JOINT_TRAJECTORY_SET_TRAJECTORY):
    {
      data_->current_trajectory = data_->model->getJointTrajectory(current);

      auto details = data_->model->getJointTrajectorySetDetails(current);
      emit configureJointTrajectorySet(details.first, details.second);

      data_->player->setTrajectory(data_->current_trajectory.trajectory);

      onEnablePlayer();

      break;
    }
    case static_cast<int>(tesseract_gui::StandardItemType::JOINT_TRAJECTORY_SET):
    {
      auto details = data_->model->getJointTrajectorySetDetails(current);
      const tesseract_common::JointTrajectorySet& traj_set = details.second;

      emit configureJointTrajectorySet(details.first, details.second);

      data_->current_trajectory = tesseract_common::JointTrajectoryInfo();
      data_->current_trajectory.initial_state = traj_set.getInitialState();
      for (const auto& t : traj_set.getJointTrajectories())
        data_->current_trajectory.trajectory.insert(data_->current_trajectory.trajectory.end(), t.trajectory.begin(), t.trajectory.end());


      data_->player->setTrajectory(data_->current_trajectory.trajectory);
      onEnablePlayer();
      break;
    }
    default:
    {
      onDisablePlayer();
      const tesseract_common::JointState& state = data_->model->getJointState(current);
      auto details = data_->model->getJointTrajectorySetDetails(current);
      emit configureJointTrajectorySet(details.first, details.second);
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
  data_->player->setCurrentDuration(data_->current_duration);
}

void JointTrajectoryWidget::onPlayerTimerTimeout()
{
  if (ui_->trajectoryPlayerFrame->isEnabled() && ui_->trajectoryPauseButton->isEnabled())
  {
    data_->player->getNext();
    ui_->trajectorySlider->setSliderPosition(data_->player->currentDuration() / SLIDER_RESOLUTION);
    if (data_->player->isFinished())
      onPauseButtonClicked();
  }
}

void JointTrajectoryWidget::onSliderValueChanged(int value)
{
  data_->current_duration = value * SLIDER_RESOLUTION;
  tesseract_common::JointState state = data_->player->setCurrentDuration(data_->current_duration);
  ui_->trajectoryCurrentDurationLabel->setText(QString().sprintf("%0.3f", data_->current_duration));
  emit showState(state);
}

void JointTrajectoryWidget::onPlotTrajectoryClicked()
{
  if (data_->current_trajectory.trajectory.empty())
    return;

  data_->plot_dialog = nullptr;
  data_->plot_dialog = std::make_unique<JointTrajectoryPlotDialog>(data_->current_trajectory);
  data_->plot_dialog->setWindowFlags(Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
  data_->plot_dialog->show();
}

void JointTrajectoryWidget::onEnablePlayer()
{
  ui_->trajectoryPlayerFrame->setEnabled(true);
  ui_->trajectoryPlayButton->setEnabled(true);
  ui_->trajectoryPauseButton->setEnabled(false);
  ui_->trajectorySlider->setMinimum(0);
  ui_->trajectorySlider->setMaximum(data_->player->trajectoryDuration() / SLIDER_RESOLUTION);
  ui_->trajectorySlider->setSliderPosition(0);
  ui_->trajectoryCurrentDurationLabel->setText(QString().sprintf("%0.3f", data_->player->currentDuration()));
  ui_->trajectoryDurationLabel->setText(QString().sprintf("%0.3f", data_->player->trajectoryDuration()));
  data_->current_duration = 0;
}

void JointTrajectoryWidget::onDisablePlayer()
{
  ui_->trajectoryPlayerFrame->setEnabled(false);
}

}
