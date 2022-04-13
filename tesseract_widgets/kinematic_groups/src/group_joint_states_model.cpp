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
