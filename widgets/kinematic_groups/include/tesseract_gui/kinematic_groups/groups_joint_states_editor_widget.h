#ifndef TESSERACT_GUI_GROUPS_JOINT_STATES_EDITOR_WIDGET_H
#define TESSERACT_GUI_GROUPS_JOINT_STATES_EDITOR_WIDGET_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_srdf/kinematics_information.h>
#include <tesseract_scene_graph/joint.h>
#include <memory>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QWidget>
#include <QStringListModel>

#include <tesseract_gui/kinematic_groups/groups_joint_states_model.h>

namespace Ui {
class GroupsJointStatesEditorWidget;
}

namespace tesseract_gui
{
using GroupJointsRetriever = std::function<std::vector<tesseract_scene_graph::Joint::ConstPtr>(QString)>;

class GroupsJointStatesEditorWidget : public QWidget
{
  Q_OBJECT

public:
  explicit GroupsJointStatesEditorWidget(QStringListModel *group_names_model,
                                         GroupJointsRetriever group_joints_retriever,
                                         QWidget *parent = nullptr);
  ~GroupsJointStatesEditorWidget();

  void setModel(GroupsJointStatesModel* model);

Q_SIGNALS:
  void jointStateChanged(std::unordered_map<std::string, double> state);

private Q_SLOTS:
  void onGroupNameChanged();
  void onAddJointState();
  void onRemoveJointState();

private:
  std::unique_ptr<Ui::GroupsJointStatesEditorWidget> ui_;
  GroupJointsRetriever group_joints_retriever_;
  GroupsJointStatesModel* model_;
};
}

#endif // TESSERACT_GUI_GROUPS_JOINT_STATES_EDITOR_WIDGET_H
