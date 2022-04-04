#include <tesseract_gui/widgets/kinematic_groups/group_joint_states_standard_item.h>
#include <tesseract_gui/widgets/kinematic_groups/joint_state_standard_item.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, ROBOT_ICON, (":/tesseract_gui/png/robotic-arm.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, TEXT_ICON, (":/tesseract_gui/png/text.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, NUMERIC_ICON, (":/tesseract_gui/png/numeric.png"));

namespace tesseract_gui
{
GroupJointStatesStandardItem::GroupJointStatesStandardItem()
: QStandardItem("Group Joint States")
{
  ctor();
}

GroupJointStatesStandardItem::GroupJointStatesStandardItem(tesseract_srdf::GroupJointStates group_joint_states)
  : QStandardItem("Group Joint States")
  , group_joint_states_(std::move(group_joint_states))
{
  ctor();
}

GroupJointStatesStandardItem::GroupJointStatesStandardItem(const QString &text, tesseract_srdf::GroupJointStates group_joint_states)
  : QStandardItem(text)
  , group_joint_states_(std::move(group_joint_states))
{
  ctor();
}

GroupJointStatesStandardItem::GroupJointStatesStandardItem(const QIcon &icon, const QString &text, tesseract_srdf::GroupJointStates group_joint_states)
  : QStandardItem(icon, text)
  , group_joint_states_(std::move(group_joint_states))
{
  ctor();
}

int GroupJointStatesStandardItem::type() const
{
  return static_cast<int>(StandardItemType::GROUP_JOINT_STATES);
}

void GroupJointStatesStandardItem::addGroupJointStateItem(const QString& group_name,
                                                          const QString& state_name,
                                                          const tesseract_srdf::GroupsJointState& state)
{
  QStandardItem* group_item {nullptr};
  auto it = group_items_.find(group_name.toStdString());
  if (it != group_items_.end())
  {
    group_item = it->second;
  }
  else
  {
    group_item = new QStandardItem(*ROBOT_ICON(), group_name);
    group_items_[group_name.toStdString()] = group_item;
    appendRow({group_item, new QStandardItem()});// Must have two columns so QTreeView::setRootIndex will have two columns
  }

  group_item->appendRow(new JointStateStandardItem(state_name, state));
}

void GroupJointStatesStandardItem::addGroupJointState(const QString& group_name,
                                                      const QString& state_name,
                                                      const tesseract_srdf::GroupsJointState& state)
{
  removeGroupJointState(group_name, state_name);
  addGroupJointStateItem(group_name, state_name, state);

  group_joint_states_[group_name.toStdString()][state_name.toStdString()] = state;
}

void GroupJointStatesStandardItem::removeGroupJointState(const QString &group_name, const QString &state_name)
{
  auto group_it = group_joint_states_.find(group_name.toStdString());
  if(group_it != group_joint_states_.end())
  {
    QStandardItem* item = group_items_.at(group_name.toStdString());
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
      group_joint_states_.erase(group_it);
      removeRow(item->index().row());
    }
  }
}

const tesseract_srdf::GroupJointStates& GroupJointStatesStandardItem::getGroupJointStates() const
{
  return group_joint_states_;
}

void GroupJointStatesStandardItem::ctor()
{
  for(auto& group : group_joint_states_)
  {
    for (auto& state : group.second)
      addGroupJointStateItem(QString::fromStdString(group.first), QString::fromStdString(state.first), state.second);
  }
}
}

