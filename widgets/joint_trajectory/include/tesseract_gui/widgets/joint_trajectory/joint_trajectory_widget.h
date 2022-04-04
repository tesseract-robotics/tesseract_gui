#ifndef TESSERACT_GUI_JOINT_TRAJECTORY_JOINT_TRAJECTORY_WIDGET_H
#define TESSERACT_GUI_JOINT_TRAJECTORY_JOINT_TRAJECTORY_WIDGET_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_common/joint_state.h>
#include <memory>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QWidget>
#include <QItemSelectionModel>
#include <tesseract_gui/widgets/joint_trajectory/joint_trajectory_model.h>

namespace Ui {
class JointTrajectoryWidget;
}

namespace tesseract_visualization
{
class TrajectoryPlayer;
}

namespace tesseract_gui
{
class JointTrajectoryPlotDialog;
class JointTrajectoryWidget : public QWidget
{
  Q_OBJECT

public:
  explicit JointTrajectoryWidget(QWidget *parent = nullptr);
  ~JointTrajectoryWidget();

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
  std::unique_ptr<Ui::JointTrajectoryWidget> ui_;
  JointTrajectoryModel* model_;
  std::unique_ptr<tesseract_visualization::TrajectoryPlayer> player_;
  std::unique_ptr<QTimer> player_timer_;
  std::unique_ptr<JointTrajectoryPlotDialog> plot_dialog_;
  double current_duration_{0};
  tesseract_common::JointTrajectoryInfo current_trajectory_;

  void enablePlayer();
  void disablePlayer();
};

}

#endif // TESSERACT_GUI_JOINT_TRAJECTORY_JOINT_TRAJECTORY_WIDGET_H
