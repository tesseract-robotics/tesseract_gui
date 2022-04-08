#ifndef TESSERACT_WIDGETS_KINEMATIC_GROUP_GROUP_JOINT_STATES_STANDARD_ITEM_H
#define TESSERACT_WIDGETS_KINEMATIC_GROUP_GROUP_JOINT_STATES_STANDARD_ITEM_H
#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_srdf/kinematics_information.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class GroupJointStatesStandardItem : public QStandardItem
{
public:
  GroupJointStatesStandardItem();
  explicit GroupJointStatesStandardItem(tesseract_srdf::GroupJointStates group_joint_states);
  explicit GroupJointStatesStandardItem(const QString &text, tesseract_srdf::GroupJointStates group_joint_states);
  explicit GroupJointStatesStandardItem(const QIcon &icon, const QString &text, tesseract_srdf::GroupJointStates group_joint_states);
  int type() const override;

  void addGroupJointState(const QString& group_name, const QString& state_name, const tesseract_srdf::GroupsJointState& state);
  void removeGroupJointState(const QString& group_name, const QString& state_name);

  const tesseract_srdf::GroupJointStates& getGroupJointStates() const;

private:
  void ctor();

  tesseract_srdf::GroupJointStates group_joint_states_;
  std::unordered_map<std::string, QStandardItem*> group_items_;

  void addGroupJointStateItem(const QString& group_name, const QString& tcp_name, const tesseract_srdf::GroupsJointState& state);
};
}
#endif // TESSERACT_WIDGETS_KINEMATIC_GROUP_GROUP_JOINT_STATES_STANDARD_ITEM_H
