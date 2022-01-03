#include <tesseract_gui/kinematic_groups/groups_joint_states_model.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, ROBOT_ICON, (":/tesseract_gui/png/robotic-arm.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, TEXT_ICON, (":/tesseract_gui/png/text.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, NUMERIC_ICON, (":/tesseract_gui/png/numeric.png"));

namespace tesseract_gui
{
GroupsJointStateStandardItem::GroupsJointStateStandardItem(tesseract_srdf::GroupsJointState state)
  : QStandardItem(*ROBOT_ICON(), "State")
  , state(std::move(state))
{
  ctor();
}

GroupsJointStateStandardItem::GroupsJointStateStandardItem(const QString &text, tesseract_srdf::GroupsJointState state)
  : QStandardItem(*ROBOT_ICON(), text)
  , state(std::move(state))
{
  ctor();
}

GroupsJointStateStandardItem::GroupsJointStateStandardItem(const QIcon &icon, const QString &text, tesseract_srdf::GroupsJointState state)
  : QStandardItem(icon, text)
  , state(std::move(state))
{
  ctor();
}

int GroupsJointStateStandardItem::type() const
{
  return static_cast<int>(StandardItemType::GROUPS_JOINT_STATE);
}

void GroupsJointStateStandardItem::ctor()
{
  setColumnCount(2);
  for (const auto& s : state)
  {
    QStandardItem* p_name = new QStandardItem(*NUMERIC_ICON(), QString::fromStdString(s.first));
    QStandardItem* p_value = new QStandardItem(QString("%1").arg(s.second));
    appendRow({p_name, p_value});
  }
}

GroupsJointStatesModel::GroupsJointStatesModel(QObject *parent)
  : QStandardItemModel(parent)
{
  clear();
}
GroupsJointStatesModel::GroupsJointStatesModel(const GroupsJointStatesModel &other)
: QStandardItemModel(other.d_ptr->parent)
{
  this->groups_joint_states_ = other.groups_joint_states_;
}
GroupsJointStatesModel &GroupsJointStatesModel::operator=(const GroupsJointStatesModel &other)
{
  this->groups_joint_states_ = other.groups_joint_states_;
  return *this;
}

void GroupsJointStatesModel::clear()
{
  QStandardItemModel::clear();
  setColumnCount(2);
  setHorizontalHeaderLabels({"Name", "Values"});
}

void GroupsJointStatesModel::addGroupJointState(QString group_name, QString state_name, tesseract_srdf::GroupsJointState state)
{
  removeGroupJointState(group_name, state_name);

  QStandardItem* group_item;
  if(groups_joint_states_.find(group_name.toStdString()) != groups_joint_states_.end())
  {
    group_item = findItems(group_name).at(0);
  }
  else
  {
    group_item = new QStandardItem(group_name);
    appendRow(group_item);
  }

  group_item->appendRow(new GroupsJointStateStandardItem(state_name, state));
  groups_joint_states_[group_name.toStdString()][state_name.toStdString()] = state;
}

void GroupsJointStatesModel::removeGroupJointState(QString group_name, QString state_name)
{
  auto group_it = groups_joint_states_.find(group_name.toStdString());
  if(group_it != groups_joint_states_.end())
  {
    QStandardItem* item = findItems(group_name).at(0);
    for (int i = item->rowCount(); i > 0; i--)
    {
      QStandardItem* child_item = item->child(i-1);
      if (child_item->text() == state_name)
        item->removeRow(i - 1);
    }

    auto state_it = group_it->second.find(state_name.toStdString());
    if(state_it != group_it->second.end())
      group_it->second.erase(state_it);

    if (group_it->second.empty())
    {
      groups_joint_states_.erase(group_it);
      removeRow(indexFromItem(item).row());
    }
  }
}

const tesseract_srdf::GroupJointStates& GroupsJointStatesModel::getGroupsJointStates() const
{
  return groups_joint_states_;
}

}
