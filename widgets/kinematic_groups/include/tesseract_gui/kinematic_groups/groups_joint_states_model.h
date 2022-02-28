#ifndef TESSERACT_GUI_GROUPS_JOINT_STATES_MODEL_H
#define TESSERACT_GUI_GROUPS_JOINT_STATES_MODEL_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_srdf/kinematics_information.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItemModel>

namespace tesseract_gui
{

class GroupsJointStateStandardItem : public QStandardItem
{
public:
  GroupsJointStateStandardItem(tesseract_srdf::GroupsJointState state);
  explicit GroupsJointStateStandardItem(const QString &text, tesseract_srdf::GroupsJointState state);
  GroupsJointStateStandardItem(const QIcon &icon, const QString &text, tesseract_srdf::GroupsJointState state);
  int type() const override;

  tesseract_srdf::GroupsJointState state;

private:
  void ctor();
};

class GroupsJointStatesModel : public QStandardItemModel
{
  Q_OBJECT

public:

  explicit GroupsJointStatesModel(QObject *parent = nullptr);
  GroupsJointStatesModel(const GroupsJointStatesModel &other);
  GroupsJointStatesModel &operator=(const GroupsJointStatesModel &other);

  void addGroupJointState(QString group_name, QString state_name, tesseract_srdf::GroupsJointState state);
  void removeGroupJointState(QString group_name, QString state_name);
  void clear();

  const tesseract_srdf::GroupJointStates& getGroupsJointStates() const;

private:
  tesseract_srdf::GroupJointStates groups_joint_states_;
};

}

#endif // TESSERACT_GUI_GROUPS_JOINT_STATES_MODEL_H
