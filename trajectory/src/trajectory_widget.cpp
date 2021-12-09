#include "ui_trajectory_widget.h"
#include <tesseract_gui/trajectory/trajectory_widget.h>
#include <tesseract_gui/plot/plot_subplots.h>
#include <tesseract_visualization/trajectory_player.h>
#include <QTimer>
#include <set>
//#include <qwt/qwt_plot.h>
//#include <qwt/qwt_plot_grid.h>
//#include <qwt/qwt_plot_curve.h>


const double SLIDER_RESOLUTION = 0.001;

namespace tesseract_gui
{
TrajectoryWidget::TrajectoryWidget(QWidget *parent)
  : QDockWidget(parent)
{
  ui_ = std::make_unique<Ui::TrajectoryWidget>();
  ui_->setupUi(this);

  player_ = std::make_unique<tesseract_visualization::TrajectoryPlayer>();
  player_timer_ = std::make_unique<QTimer>(this);
  player_timer_->start(10);

  connect(ui_->trajectoryPlayButton, SIGNAL(clicked()), this, SLOT(onPlayButtonClicked()));
  connect(ui_->trajectoryPauseButton, SIGNAL(clicked()), this, SLOT(onPauseButtonClicked()));
  connect(ui_->trajectorySlider, SIGNAL(valueChanged(int)), this, SLOT(onSliderValueChanged(int)));
  connect(player_timer_.get(), SIGNAL(timeout()), this, SLOT(onPlayerTimerTimeout()));
  connect(ui_->trajectoryPlotButton, SIGNAL(clicked()), this, SLOT(onPlotTrajectoryClicked()));
}
TrajectoryWidget::~TrajectoryWidget() = default;

void TrajectoryWidget::setModel(JointTrajectoryModel* model)
{
  ui_->trajectoryTreeView->setModel(model);
  ui_->trajectoryTreeView->setColumnWidth(0, 175);
  model_ = model;

  connect(ui_->trajectoryTreeView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(onCurrentRowChanged(QModelIndex,QModelIndex)));
}

void TrajectoryWidget::onCurrentRowChanged(const QModelIndex &current, const QModelIndex &previous)
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
//      const tesseract_common::JointTrajectorySet& trajectory_set = model_->getJointTrajectorySet(current);
//      current_trajectory_.clear();
//      for (const auto& t : trajectory_set)
//        current_trajectory_.insert(current_trajectory_.end(), t.begin(), t.end());

//      player_->setTrajectory(current_trajectory_);
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

void TrajectoryWidget::onPauseButtonClicked()
{
  ui_->trajectoryPlayButton->setEnabled(true);
  ui_->trajectorySlider->setEnabled(true);
  ui_->trajectoryPauseButton->setEnabled(false);
}
void TrajectoryWidget::onPlayButtonClicked()
{
  ui_->trajectoryPlayButton->setEnabled(false);
  ui_->trajectorySlider->setEnabled(false);
  ui_->trajectoryPauseButton->setEnabled(true);
  player_->setCurrentDuration(current_duration_);
}

void TrajectoryWidget::onPlayerTimerTimeout()
{
  if (ui_->trajectoryPlayerFrame->isEnabled() && ui_->trajectoryPauseButton->isEnabled())
  {
    player_->getNext();
    ui_->trajectorySlider->setSliderPosition(player_->currentDuration() / SLIDER_RESOLUTION);
    if (player_->isFinished())
      onPauseButtonClicked();
  }
}

void TrajectoryWidget::onSliderValueChanged(int value)
{
  current_duration_ = value * SLIDER_RESOLUTION;
  tesseract_common::JointState state = player_->setCurrentDuration(current_duration_);
  ui_->trajectoryCurrentDurationLabel->setText(QString().sprintf("%0.3f", current_duration_));
  emit showState(state);
}

void TrajectoryWidget::onPlotTrajectoryClicked()
{
  plot_data_map_.clear();
//  tesseract_gui::PlotData& cosine_data = plot_data_map_.getOrCreateNumeric("cosine");
//  tesseract_gui::PlotData& sine_data = plot_data_map_.getOrCreateNumeric("sine");

//  for (int i = 0; i < int((2 * M_PI)/0.01); ++i)
//  {
//    cosine_data.pushBack(tesseract_gui::PlotDataXY::Point(i * 0.01, std::cos(i*0.01)));
//    sine_data.pushBack(tesseract_gui::PlotDataXY::Point(i * 0.01, std::sin(i*0.01)));
//  }



  std::size_t cnt = current_trajectory_.trajectory.size();
  std::vector<double> x;
  std::vector<std::vector<double>> position;
  std::vector<std::vector<double>> velocity;
  std::vector<std::vector<double>> acceleration;
  position.resize(current_trajectory_.trajectory[0].joint_names.size());
  velocity.resize(current_trajectory_.trajectory[0].joint_names.size());
  acceleration.resize(current_trajectory_.trajectory[0].joint_names.size());

  double ct{0};
  for (const auto& state : current_trajectory_.trajectory)
  {
    x.push_back(++ct);
    for (Eigen::Index i = 0; i < position.size(); ++i)
    {
      position[i].push_back(state.position(i));
      velocity[i].push_back(state.velocity(i));
      acceleration[i].push_back(state.acceleration(i));
    }
  }

//  auto* position_subplots = new tesseract_gui::PlotSubplots(data)

  position_plot_ = std::make_unique<QwtPlot>();
  velocity_plot_ = std::make_unique<QwtPlot>();
  acceleration_plot_ = std::make_unique<QwtPlot>();
  for (Eigen::Index i = 0; i < position.size(); ++i)
  {
    auto* position_curve = new QwtPlotCurve( "Position" );
    position_curve->setSamples( x.data(), position[i].data(), x.size());
    position_curve->attach( position_plot_.get() );

    auto* velocity_curve = new QwtPlotCurve( "Velocity" );
    velocity_curve->setSamples( x.data(), velocity[i].data(), x.size());
    velocity_curve->attach( velocity_plot_.get() );

    auto* acceleration_curve = new QwtPlotCurve( "Acceleration" );
    acceleration_curve->setSamples( x.data(), velocity[i].data(), x.size());
    acceleration_curve->attach( acceleration_plot_.get() );
  }

  position_plot_->replot();
  velocity_plot_->replot();
  acceleration_plot_->replot();

  position_plot_->show();
  velocity_plot_->show();
  acceleration_plot_->show();
}

void TrajectoryWidget::enablePlayer()
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

void TrajectoryWidget::disablePlayer()
{
  ui_->trajectoryPlayerFrame->setEnabled(false);
}

}
