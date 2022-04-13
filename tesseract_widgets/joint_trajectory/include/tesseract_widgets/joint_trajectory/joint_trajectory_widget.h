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
#ifndef TESSERACT_WIDGETS_JOINT_TRAJECTORY_JOINT_TRAJECTORY_WIDGET_H
#define TESSERACT_WIDGETS_JOINT_TRAJECTORY_JOINT_TRAJECTORY_WIDGET_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_common/joint_state.h>
#include <memory>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QWidget>
#include <QItemSelectionModel>
#include <tesseract_widgets/joint_trajectory/joint_trajectory_model.h>

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
  void configureEnvironment(tesseract_environment::Environment::Ptr environment,
                            const tesseract_environment::Commands& commands,
                            const tesseract_common::JointState& initial_state);

private Q_SLOTS:
  void onCurrentRowChanged(const QModelIndex &current, const QModelIndex &previous);
  void onPauseButtonClicked();
  void onPlayButtonClicked();
  void onPlayerTimerTimeout();
  void onSliderValueChanged(int value);
  void onPlotTrajectoryClicked();

private:
  std::unique_ptr<Ui::JointTrajectoryWidget> ui_;
  JointTrajectoryModel* model_{nullptr};
  std::unique_ptr<tesseract_visualization::TrajectoryPlayer> player_;
  std::unique_ptr<QTimer> player_timer_;
  std::unique_ptr<JointTrajectoryPlotDialog> plot_dialog_;
  double current_duration_{0};
  tesseract_common::JointTrajectoryInfo current_trajectory_;

  void enablePlayer();
  void disablePlayer();
};

}

#endif // TESSERACT_WIDGETS_JOINT_TRAJECTORY_JOINT_TRAJECTORY_WIDGET_H
