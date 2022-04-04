#ifndef TESSERACT_GUI_GROUPS_JOINT_STATES_MODEL_H
#define TESSERACT_GUI_GROUPS_JOINT_STATES_MODEL_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_srdf/kinematics_information.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItemModel>
#include <tesseract_gui/widgets/kinematic_groups/group_joint_states_standard_item.h>

namespace tesseract_gui
{

class GroupJointStatesModel : public QStandardItemModel
{
  Q_OBJECT

public:

  explicit GroupJointStatesModel(QObject *parent = nullptr);
  GroupJointStatesModel(const GroupJointStatesModel &other);
  GroupJointStatesModel &operator=(const GroupJointStatesModel &other);

  void set(const tesseract_srdf::GroupJointStates& group_joint_states);
  void addGroupJointState(QString group_name, QString state_name, tesseract_srdf::GroupsJointState state);
  void removeGroupJointState(QString group_name, QString state_name);
  void clear();

  const tesseract_srdf::GroupJointStates& getGroupsJointStates() const;

private:
  GroupJointStatesStandardItem* getRoot();
  const GroupJointStatesStandardItem* getRoot() const;
};

}

#endif // TESSERACT_GUI_GROUPS_JOINT_STATES_MODEL_H
