#ifndef TESSERACT_GUI_JOINT_TRAJECTORY_PLOT_DIALOG_H
#define TESSERACT_GUI_JOINT_TRAJECTORY_PLOT_DIALOG_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
//#include <tesseract_common/joint_trajectory_set.h>
#include <tesseract_gui/common/joint_trajectory_set.h>
#include <memory>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QDialog>
#include <tesseract_gui/plot/plot_data.h>

namespace Ui {
class JointTrajectoryPlotDialog;
}

namespace tesseract_gui
{
class PlotSubplots;

class JointTrajectoryPlotDialog : public QDialog
{
  Q_OBJECT

public:
  explicit JointTrajectoryPlotDialog(tesseract_common::JointTrajectoryInfo current_trajectory, QWidget *parent = nullptr);
  ~JointTrajectoryPlotDialog();

private Q_SLOTS:
  void onSliderMoved(int value);
  void onTabIndexChanged(int value);

private:
  std::unique_ptr<Ui::JointTrajectoryPlotDialog> ui_;
  tesseract_gui::PlotDataMapRef plot_data_map_;
  tesseract_gui::PlotSubplots* position_subplots_;
  tesseract_gui::PlotSubplots* velocity_subplots_;
  tesseract_gui::PlotSubplots* acceleration_subplots_;
};
}
#endif // TESSERACT_GUI_JOINT_TRAJECTORY_PLOT_DIALOG_H
