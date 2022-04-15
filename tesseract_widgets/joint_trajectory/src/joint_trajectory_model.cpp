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
#include <tesseract_widgets/joint_trajectory/joint_trajectory_model.h>
#include <tesseract_widgets/common/standard_item_type.h>
#include <tesseract_widgets/common/standard_item_utils.h>

#include <QUuid>

namespace tesseract_gui
{
JointTrajectoryModel::JointTrajectoryModel(QObject *parent)
  : QStandardItemModel(parent)
{
  clear();
}
JointTrajectoryModel::JointTrajectoryModel(const JointTrajectoryModel &other)
: QStandardItemModel(other.d_ptr->parent)
{
  this->trajectory_sets_ = other.trajectory_sets_;
}
JointTrajectoryModel &JointTrajectoryModel::operator=(const JointTrajectoryModel &other)
{
  this->trajectory_sets_ = other.trajectory_sets_;
  return *this;
}

void JointTrajectoryModel::clear()
{
  QStandardItemModel::clear();
  setColumnCount(2);
  setHorizontalHeaderLabels({"Name", "Values"});
}

QString JointTrajectoryModel::addJointTrajectorySet(const tesseract_common::JointTrajectorySet& trajectory_set)
{
  QString key = QUuid::createUuid().toString();
  trajectory_sets_[key] = trajectory_set;
  auto* trajectory_container_item = new JointTrajectorySetItem(key, trajectory_sets_.at(key));
  auto* trajectory_description_item = new QStandardItem(QString::fromStdString(trajectory_set.getDescription()));
  appendRow({trajectory_container_item, trajectory_description_item});
  return key;
}

void JointTrajectoryModel::removeJointTrajectorySet(const QString& key)
{
  auto it = trajectory_sets_.find(key);
  if (it == trajectory_sets_.end())
    throw std::runtime_error("Tried to remove trajectory set '" + key.toStdString() + "' which does not exist!");

  auto row = static_cast<int>(std::distance(trajectory_sets_.begin(), it));
  trajectory_sets_.erase(it);
  removeRow(row);
}

bool JointTrajectoryModel::hasJointTrajectorySet(const QString& key)
{
  return (trajectory_sets_.find(key) != trajectory_sets_.end());
}

JointStateItem* findJointStateItem(QStandardItem* item)
{
  if (item->type() == static_cast<int>(StandardItemType::JOINT_TRAJECTORY_SET_STATE))
    return dynamic_cast<JointStateItem*>(item);

  return findJointStateItem(item->parent());
}

const tesseract_common::JointState& JointTrajectoryModel::getJointState(const QModelIndex& row) const
{
  QStandardItem* item = itemFromIndex(row);

  if (item->type() == static_cast<int>(StandardItemType::JOINT_TRAJECTORY_SET_TRAJECTORY))
    throw std::runtime_error("Cannot get joint state from selected joint trajectory standard item");

  if (item->type() == static_cast<int>(StandardItemType::JOINT_TRAJECTORY_SET))
    throw std::runtime_error("Cannot get joint state from selected joint trajectory set standard item");

  return findJointStateItem(item)->state;
}

JointTrajectoryItem* findJointTrajectoryItem(QStandardItem* item)
{
  if (item->type() == static_cast<int>(StandardItemType::JOINT_TRAJECTORY_SET_TRAJECTORY))
    return dynamic_cast<JointTrajectoryItem*>(item);

  return findJointTrajectoryItem(item->parent());
}

const tesseract_common::JointTrajectoryInfo &JointTrajectoryModel::getJointTrajectory(const QModelIndex& row) const
{
  QStandardItem* item = itemFromIndex(row);

  if (item->type() == static_cast<int>(StandardItemType::JOINT_TRAJECTORY_SET))
    throw std::runtime_error("Cannot get joint trajectory from selected trajectory set standard item");

  return findJointTrajectoryItem(item)->trajectory_info;
}

JointTrajectorySetItem* findJointTrajectorySetItem(QStandardItem* item)
{
  if (item->type() == static_cast<int>(StandardItemType::JOINT_TRAJECTORY_SET))
    return dynamic_cast<JointTrajectorySetItem*>(item);

  return findJointTrajectorySetItem(item->parent());
}

const tesseract_common::JointTrajectorySet& JointTrajectoryModel::getJointTrajectorySet(const QModelIndex& row) const
{
  QStandardItem* item = itemFromIndex(row);
  return findJointTrajectorySetItem(item)->trajectory_set;
}

std::pair<const QString&, const tesseract_common::JointTrajectorySet&> JointTrajectoryModel::getJointTrajectorySetDetails(const QModelIndex &row) const
{
  QStandardItem* item = itemFromIndex(row);
  JointTrajectorySetItem* jts_item = findJointTrajectorySetItem(item);
  return std::pair<const QString&, const tesseract_common::JointTrajectorySet&>(jts_item->uuid, jts_item->trajectory_set);
}

JointStateItem::JointStateItem(tesseract_common::JointState &state)
  : state(state)
{
  ctor();
}

JointStateItem::JointStateItem(const QString &text, tesseract_common::JointState &state)
  : QStandardItem(QIcon(":/tesseract_widgets/png/robotic-arm.png"), text)
  , state(state)
{
  ctor();
}

JointStateItem::JointStateItem(const QIcon &icon, const QString &text, tesseract_common::JointState &state)
  : QStandardItem(icon, text)
  , state(state)
{
  ctor();
}

int JointStateItem::type() const
{
  return static_cast<int>(StandardItemType::JOINT_TRAJECTORY_SET_STATE);
}

void JointStateItem::ctor()
{
  // Add State Joint Names
  auto* state_joint_names = new QStandardItem("joint_names");
  state_joint_names->setColumnCount(2);
  for (std::size_t k = 0; k < state.joint_names.size(); ++k)
    state_joint_names->appendRow(createStandardItemString(QString("[%1]").arg(k).toStdString(), state.joint_names[k]));

  appendRow(state_joint_names);

  // Add State Position
  auto* state_position = new QStandardItem("position");
  state_position->setColumnCount(2);
  for (Eigen::Index k = 0; k < state.position.rows(); ++k)
    state_position->appendRow(createStandardItemFloat(QString("[%1]").arg(k).toStdString(), state.position[k]));

  appendRow(state_position);

  // Add State Velocity
  auto* state_velocity = new QStandardItem("velocity");
  state_velocity->setColumnCount(2);
  for (int k = 0; k < state.velocity.rows(); ++k)
    state_velocity->appendRow(createStandardItemFloat(QString("[%1]").arg(k).toStdString(), state.velocity[k]));

  appendRow(state_velocity);

  // Add State Velocity
  auto* state_acceleration = new QStandardItem("acceleration");
  state_acceleration->setColumnCount(2);
  for (int k = 0; k < state.acceleration.rows(); ++k)
    state_acceleration->appendRow(createStandardItemFloat(QString("[%1]").arg(k).toStdString(), state.acceleration[k]));

  appendRow(state_acceleration);

  // Add time from start
  appendRow(createStandardItemFloat("time", state.time));
}

JointTrajectoryItem::JointTrajectoryItem(tesseract_common::JointTrajectoryInfo& trajectory_info)
  : trajectory_info(trajectory_info)
{
  ctor();
}

JointTrajectoryItem::JointTrajectoryItem(const QString &text, tesseract_common::JointTrajectoryInfo& trajectory_info)
  : QStandardItem(QIcon(":/tesseract_widgets/png/trajectory.png"), text)
  , trajectory_info(trajectory_info)
{
  ctor();
}

JointTrajectoryItem::JointTrajectoryItem(const QIcon &icon, const QString &text, tesseract_common::JointTrajectoryInfo &trajectory_info)
  : QStandardItem(icon, text)
  , trajectory_info(trajectory_info)
{
  ctor();
}

int JointTrajectoryItem::type() const
{
  return static_cast<int>(StandardItemType::JOINT_TRAJECTORY_SET_TRAJECTORY);
}

void JointTrajectoryItem::ctor()
{
  for (std::size_t j = 0; j < trajectory_info.trajectory.size(); ++j)
  {
    QStandardItem* trajectory_state = new JointStateItem(QString("state[%1]").arg(j), trajectory_info.trajectory[j]);
    appendRow(trajectory_state);
  }
}

JointTrajectorySetItem::JointTrajectorySetItem(QString uuid, tesseract_common::JointTrajectorySet& trajectory_set)
  : QStandardItem(QIcon(":/tesseract_widgets/png/programming.png"), "Trajectory Set")
  , uuid(std::move(uuid))
  , trajectory_set(trajectory_set)
{
  ctor();
}

JointTrajectorySetItem::JointTrajectorySetItem(const QString &text, QString uuid, tesseract_common::JointTrajectorySet& trajectory_set)
  : QStandardItem(QIcon(":/tesseract_widgets/png/programming.png"), text)
  , uuid(std::move(uuid))
  , trajectory_set(trajectory_set)
{
  ctor();
}
JointTrajectorySetItem::JointTrajectorySetItem(const QIcon &icon, const QString &text, QString uuid, tesseract_common::JointTrajectorySet &trajectory_set)
  : QStandardItem(icon, text)
  , uuid(std::move(uuid))
  , trajectory_set(trajectory_set)
{
  ctor();
}

int JointTrajectorySetItem::type() const
{
  return static_cast<int>(StandardItemType::JOINT_TRAJECTORY_SET);
}

void JointTrajectorySetItem::ctor()
{
  for (std::size_t i = 0; i < trajectory_set.size(); ++i)
  {
    tesseract_common::JointTrajectoryInfo& traj_info = trajectory_set[i];
    QStandardItem* trajectory_item = new JointTrajectoryItem(QString("trajectory[%1]").arg(i), traj_info);
    appendRow(trajectory_item);
  }
}
}
