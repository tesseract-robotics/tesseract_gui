
#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#include <QApplication>
#include <QStandardItemModel>
#include <QDebug>
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <tesseract_gui/trajectory/joint_trajectory_model.h>
#include <tesseract_gui/trajectory/trajectory_widget.h>

int main(int argc, char ** argv)
{
    QApplication app(argc, argv);

    std::vector<tesseract_common::JointTrajectory> trajectories;
    for (int i = 0; i < 5; ++i)
    {
      tesseract_common::JointTrajectory trajectory;
      for (int j = 0; j < 5; ++j)
      {
        tesseract_common::JointState state;
        state.joint_names = {"joint_1", "joint_2", "joint_3", "joint_4", "joint_5"};
        state.position.resize(5);
        state.velocity.resize(5);
        state.acceleration.resize(5);
        state.time = j;

        // Add State Position
        for (Eigen::Index k = 0; k < 5; ++k)
        {
          state.position[k] = k+j;
          state.velocity[k] = 2 * (k + j);
          state.acceleration[k] = 3 * (k + j);
        }
        trajectory.push_back(state);
      }
      trajectories.push_back(trajectory);
    }

    auto* model = new tesseract_gui::JointTrajectoryModel();
    model->addTrajectorySet("set1", trajectories);
    model->addTrajectorySet("set2", trajectories);
    model->addTrajectorySet("set3", trajectories);

    model->removeTrajectorySet("set2");

    tesseract_gui::TrajectoryWidget widget;
    widget.setModel(model);
    widget.show();

    return app.exec();
}
