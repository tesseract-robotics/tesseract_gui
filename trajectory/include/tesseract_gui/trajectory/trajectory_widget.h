#ifndef TESSERACT_GUI_TRAJECTORY_TRAJECTORY_WIDGET_H
#define TESSERACT_GUI_TRAJECTORY_TRAJECTORY_WIDGET_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_common/joint_state.h>
#include <memory>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QDockWidget>
#include <QItemSelectionModel>
#include <tesseract_gui/trajectory/joint_trajectory_model.h>

namespace Ui {
class TrajectoryWidget;
}

namespace tesseract_visualization
{
class TrajectoryPlayer;
}

class QwtPlot;

namespace tesseract_gui
{
class TrajectoryWidget : public QDockWidget
{
  Q_OBJECT

public:
  explicit TrajectoryWidget(QWidget *parent = nullptr);
  ~TrajectoryWidget();

  void setModel(JointTrajectoryModel* model);

Q_SIGNALS:
  void showState(const tesseract_common::JointState& state);

private Q_SLOTS:
  void onCurrentRowChanged(const QModelIndex &current, const QModelIndex &previous);
  void onPauseButtonClicked();
  void onPlayButtonClicked();
  void onPlayerTimerTimeout();
  void onSliderValueChanged(int value);
  void onPlotTrajectoryClicked();

private:
  std::unique_ptr<Ui::TrajectoryWidget> ui_;
  JointTrajectoryModel* model_;
  std::unique_ptr<tesseract_visualization::TrajectoryPlayer> player_;
  std::unique_ptr<QTimer> player_timer_;
  std::unique_ptr<QwtPlot> position_plot_;
  std::unique_ptr<QwtPlot> velocity_plot_;
  std::unique_ptr<QwtPlot> acceleration_plot_;
  double current_duration_{0};
  tesseract_common::JointTrajectory current_trajectory_;

  void enablePlayer();
  void disablePlayer();
};

}

#endif // TESSERACT_GUI_TRAJECTORY_TRAJECTORY_WIDGET_H
