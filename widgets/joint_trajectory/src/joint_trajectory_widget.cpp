#include "ui_joint_trajectory_widget.h"
#include <tesseract_gui/widgets/joint_trajectory/joint_trajectory_plot_dialog.h>
#include <tesseract_gui/widgets/joint_trajectory/joint_trajectory_widget.h>
#include <tesseract_visualization/trajectory_player.h>
#include <QTimer>
#include <set>

#include <tesseract_gui/widgets/plot/transforms/first_derivative.h>
#include <tesseract_gui/widgets/plot/transforms/integral_transform.h>
#include <tesseract_gui/widgets/plot/transforms/moving_average_filter.h>
#include <tesseract_gui/widgets/plot/transforms/moving_rms.h>
#include <tesseract_gui/widgets/plot/transforms/outlier_removal.h>
#include <tesseract_gui/widgets/plot/transforms/scale_transform.h>

const double SLIDER_RESOLUTION = 0.001;

namespace tesseract_gui
{
JointTrajectoryWidget::JointTrajectoryWidget(QWidget *parent)
  : QWidget(parent)
  , ui_(std::make_unique<Ui::JointTrajectoryWidget>())
{

  ui_->setupUi(this);

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

void JointTrajectoryWidget::onCurrentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
  QStandardItem* item = model_->itemFromIndex(current);
  switch (item->type())
  {
    case JointTrajectoryModel::STATE:
    {
      disablePlayer();
      const tesseract_common::JointState& state = model_->getJointState(current);
      emit showState(state);
      break;
    }
    case JointTrajectoryModel::TRAJECTORY:
    {
      current_trajectory_ = model_->getJointTrajectory(current);
      /** @todo need to use the initial state */
      player_->setTrajectory(current_trajectory_.trajectory);

      enablePlayer();

      break;
    }
    case JointTrajectoryModel::TRAJECTORY_SET:
    {
      const tesseract_common::JointTrajectorySet& trajectory_set = model_->getJointTrajectorySet(current);
      current_trajectory_ = tesseract_common::JointTrajectoryInfo();
      current_trajectory_.initial_state = trajectory_set.getInitialState();
      for (const auto& t : trajectory_set.getJointTrajectories())
        current_trajectory_.trajectory.insert(current_trajectory_.trajectory.end(), t.trajectory.begin(), t.trajectory.end());

      player_->setTrajectory(current_trajectory_.trajectory);
      enablePlayer();
      break;
    }
    default:
    {
      disablePlayer();
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
