#include <tesseract_gui/trajectory/joint_trajectory_model.h>

namespace tesseract_gui
{
JointTrajectoryModel::JointTrajectoryModel(QObject *parent)
  : QStandardItemModel(parent)
{
  clear();
  trajectory_icon_ = QIcon(":/tesseract_gui/trajectory.png");
  trajectory_state_icon_ = QIcon(":/tesseract_gui/robotic-arm.png");
  trajectory_set_icon_ = QIcon(":/tesseract_gui/programming.png");
}
JointTrajectoryModel::JointTrajectoryModel(const JointTrajectoryModel &other)
: QStandardItemModel(other.d_ptr->parent)
{
  this->trajectory_sets_ = other.trajectory_sets_;
  this->trajectory_icon_ = other.trajectory_icon_;
  this->trajectory_state_icon_ = other.trajectory_state_icon_;
  this->trajectory_set_icon_ = other.trajectory_set_icon_;
}
JointTrajectoryModel &JointTrajectoryModel::operator=(const JointTrajectoryModel &other)
{
  this->trajectory_sets_ = other.trajectory_sets_;
  this->trajectory_icon_ = other.trajectory_icon_;
  this->trajectory_state_icon_ = other.trajectory_state_icon_;
  this->trajectory_set_icon_ = other.trajectory_set_icon_;
  return *this;
}

void JointTrajectoryModel::clear()
{
  QStandardItemModel::clear();
  setColumnCount(2);
  setHorizontalHeaderLabels({"Name", "Values"});
}

void JointTrajectoryModel::addJointTrajectorySet(const QString& key, const tesseract_common::JointTrajectorySet& trajectories)
{
  trajectory_sets_[key] = trajectories;
  tesseract_common::JointTrajectorySet& trajector_set = trajectory_sets_.at(key);
  QStandardItem* trajectory_container_item = new JointTrajectorySetItem(trajectory_set_icon_, key, trajector_set);
  for (std::size_t i = 0; i < trajector_set.size(); ++i)
  {
    tesseract_common::JointTrajectoryInfo& traj_info = trajector_set[i];
    QStandardItem* trajectory_item = new JointTrajectoryItem(trajectory_icon_, QString("trajectory[%1]").arg(i), traj_info);

    for (std::size_t j = 0; j < traj_info.trajectory.size(); ++j)
    {
      tesseract_common::JointState& state = traj_info.trajectory[j];
      QStandardItem* trajectory_state = new JointStateItem(trajectory_state_icon_, QString("state[%1]").arg(j), traj_info, state);

      // Add State Joint Names
      QStandardItem* state_joint_names = new QStandardItem("joint_names");
      state_joint_names->setColumnCount(2);
      for (std::size_t k = 0; k < state.joint_names.size(); ++k)
      {
        QStandardItem* p_name = new QStandardItem(QString("[%1]").arg(k));
        QStandardItem* p_value = new QStandardItem(QString::fromStdString(state.joint_names[k]));
        state_joint_names->appendRow({p_name, p_value});
      }
      trajectory_state->appendRow(state_joint_names);

      // Add State Position
      QStandardItem* state_position = new QStandardItem("position");
      state_position->setColumnCount(2);
      for (Eigen::Index k = 0; k < state.position.rows(); ++k)
      {
        QStandardItem* p_name = new QStandardItem(QString("[%1]").arg(k));
        QStandardItem* p_value = new QStandardItem(QString("%1").arg(state.position[k]));
        state_position->appendRow({p_name, p_value});
      }
      trajectory_state->appendRow(state_position);

      // Add State Velocity
      QStandardItem* state_velocity = new QStandardItem("velocity");
      state_velocity->setColumnCount(2);
      for (int k = 0; k < state.velocity.rows(); ++k)
      {
        QStandardItem* p_name = new QStandardItem(QString("[%1]").arg(k));
        QStandardItem* p_value = new QStandardItem(QString("%1").arg(state.velocity[k]));
        state_velocity->appendRow({p_name, p_value});
      }
      trajectory_state->appendRow(state_velocity);

      // Add State Velocity
      QStandardItem* state_acceleration = new QStandardItem("acceleration");
      state_acceleration->setColumnCount(2);
      for (int k = 0; k < state.acceleration.rows(); ++k)
      {
        QStandardItem* p_name = new QStandardItem(QString("[%1]").arg(k));
        QStandardItem* p_value = new QStandardItem(QString("%1").arg(state.acceleration[k]));
        state_acceleration->appendRow({p_name, p_value});
      }
      trajectory_state->appendRow(state_acceleration);

      // Add time from start
      QStandardItem* p_name = new QStandardItem("time");
      QStandardItem* p_value = new QStandardItem(QString("%1").arg(state.time));
      trajectory_state->appendRow({p_name, p_value});

      trajectory_item->appendRow(trajectory_state);
    }
    trajectory_container_item->appendRow(trajectory_item);
  }
  appendRow(trajectory_container_item);
}

void JointTrajectoryModel::removeJointTrajectorySet(const QString& key)
{
  auto it = trajectory_sets_.find(key);
  if (it == trajectory_sets_.end())
    throw std::runtime_error("Tried to remove trajectory set '" + key.toStdString() + "' which does not exist!");

  std::size_t row = std::distance(trajectory_sets_.begin(), it);
  trajectory_sets_.erase(it);
  removeRow(row);
}

const tesseract_common::JointState& JointTrajectoryModel::getJointState(const QModelIndex& row) const
{
  QStandardItem* item = itemFromIndex(row);
  return dynamic_cast<JointStateItem*>(item)->state;
}
const tesseract_common::JointTrajectoryInfo &JointTrajectoryModel::getJointTrajectory(const QModelIndex& row) const
{
  QStandardItem* item = itemFromIndex(row);
  return dynamic_cast<JointTrajectoryItem*>(item)->trajectory;
}
const tesseract_common::JointTrajectorySet& JointTrajectoryModel::getJointTrajectorySet(const QModelIndex& row) const
{
  QStandardItem* item = itemFromIndex(row);
  return dynamic_cast<JointTrajectorySetItem*>(item)->trajectory_set;
}

JointStateItem::JointStateItem(tesseract_common::JointTrajectoryInfo& parent_trajectory, tesseract_common::JointState &state)
  : QStandardItem()
  , parent_trajectory(parent_trajectory)
  , state(state)
{

}

JointStateItem::JointStateItem(const QString &text, tesseract_common::JointTrajectoryInfo& parent_trajectory, tesseract_common::JointState &state)
  : QStandardItem(text)
  , parent_trajectory(parent_trajectory)
  , state(state)
{

}
JointStateItem::JointStateItem(const QIcon &icon, const QString &text, tesseract_common::JointTrajectoryInfo& parent_trajectory, tesseract_common::JointState &state)
  : QStandardItem(icon, text)
  , parent_trajectory(parent_trajectory)
  , state(state)
{

}

int JointStateItem::type() const
{
  return JointTrajectoryModel::STATE;
}

JointTrajectoryItem::JointTrajectoryItem(tesseract_common::JointTrajectoryInfo& trajectory)
  : QStandardItem()
  , trajectory(trajectory)
{

}

JointTrajectoryItem::JointTrajectoryItem(const QString &text, tesseract_common::JointTrajectoryInfo& trajectory)
  : QStandardItem(text)
  , trajectory(trajectory)
{

}
JointTrajectoryItem::JointTrajectoryItem(const QIcon &icon, const QString &text, tesseract_common::JointTrajectoryInfo &trajectory)
  : QStandardItem(icon, text)
  , trajectory(trajectory)
{

}

int JointTrajectoryItem::type() const
{
  return JointTrajectoryModel::TRAJECTORY;
}

JointTrajectorySetItem::JointTrajectorySetItem(tesseract_common::JointTrajectorySet& trajectory_set)
  : QStandardItem()
  , trajectory_set(trajectory_set)
{

}

JointTrajectorySetItem::JointTrajectorySetItem(const QString &text, tesseract_common::JointTrajectorySet& trajectory_set)
  : QStandardItem(text)
  , trajectory_set(trajectory_set)
{

}
JointTrajectorySetItem::JointTrajectorySetItem(const QIcon &icon, const QString &text, tesseract_common::JointTrajectorySet &trajectory_set)
  : QStandardItem(icon, text)
  , trajectory_set(trajectory_set)
{

}

int JointTrajectorySetItem::type() const
{
  return JointTrajectoryModel::TRAJECTORY_SET;
}

}
