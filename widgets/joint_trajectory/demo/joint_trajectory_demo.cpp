
#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#include <QApplication>
#include <QStandardItemModel>
#include <QDebug>
#include <QTreeView>
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <tesseract_gui/joint_trajectory/joint_trajectory_model.h>
#include <tesseract_gui/joint_trajectory/joint_trajectory_widget.h>

int main(int argc, char ** argv)
{
    QApplication app(argc, argv);

    Q_INIT_RESOURCE(tesseract_gui_resources);

    std::unordered_map<std::string, double> initial_state {{"joint_1", 0}, {"joint_2",0}, {"joint_3",0}, {"joint_4",0}, {"joint_5",0}};

    tesseract_common::JointTrajectorySet trajectory_set(initial_state);
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
      trajectory_set.appendJointTrajectory(trajectory, "description");
    }

    auto* model = new tesseract_gui::JointTrajectoryModel();
    model->addJointTrajectorySet("set1", trajectory_set);
    model->addJointTrajectorySet("set2", trajectory_set);
    model->addJointTrajectorySet("set3", trajectory_set);

    model->removeJointTrajectorySet("set2");

    tesseract_gui::JointTrajectoryWidget widget;
    widget.setModel(model);
    widget.show();

    return app.exec();
}
