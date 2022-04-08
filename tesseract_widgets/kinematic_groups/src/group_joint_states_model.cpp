#include <tesseract_widgets/kinematic_groups/group_joint_states_model.h>
#include <tesseract_widgets/common/standard_item_type.h>

namespace tesseract_gui
{

GroupJointStatesModel::GroupJointStatesModel(QObject *parent)
  : QStandardItemModel(parent)
{
  clear();
}

GroupJointStatesModel::GroupJointStatesModel(const GroupJointStatesModel &other)
: QStandardItemModel(other.d_ptr->parent)
{
}

GroupJointStatesModel &GroupJointStatesModel::operator=(const GroupJointStatesModel &other)
{
  return *this;
}

void GroupJointStatesModel::clear()
{
  QStandardItemModel::clear();
  setColumnCount(2);
  setHorizontalHeaderLabels({"Name", "Values"});
  appendRow(new GroupJointStatesStandardItem());
}

void GroupJointStatesModel::set(const tesseract_srdf::GroupJointStates& group_joint_states)
{
  QStandardItemModel::clear();
  setColumnCount(2);
  setHorizontalHeaderLabels({"Name", "Values"});
  appendRow(new GroupJointStatesStandardItem(group_joint_states));
}

void GroupJointStatesModel::addGroupJointState(QString group_name, QString state_name, tesseract_srdf::GroupsJointState state)
{
  getRoot()->addGroupJointState(group_name, state_name, state);
}

void GroupJointStatesModel::removeGroupJointState(QString group_name, QString state_name)
{
  getRoot()->removeGroupJointState(group_name, state_name);
}

const tesseract_srdf::GroupJointStates& GroupJointStatesModel::getGroupsJointStates() const
{
  return getRoot()->getGroupJointStates();
}

GroupJointStatesStandardItem* GroupJointStatesModel::getRoot()
{
  return dynamic_cast<GroupJointStatesStandardItem*>(item(0));
}

const GroupJointStatesStandardItem* GroupJointStatesModel::getRoot() const
{
  return dynamic_cast<const GroupJointStatesStandardItem*>(item(0));
}
}
