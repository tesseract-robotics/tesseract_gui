#include <tesseract_gui/widgets/joint_trajectory/joint_trajectory_model.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, TRAJECTORY_ICON, (":/tesseract_gui/png/trajectory.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, TRAJECTORY_STATE_ICON, (":/tesseract_gui/png/robotic-arm.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, TRAJECTORY_SET_ICON, (":/tesseract_gui/png/programming.png"));

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

void JointTrajectoryModel::addJointTrajectorySet(const QString& key, const tesseract_common::JointTrajectorySet& trajectories)
{
  trajectory_sets_[key] = trajectories;
  QStandardItem* trajectory_container_item = new JointTrajectorySetItem(key, trajectory_sets_.at(key));
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
  return dynamic_cast<JointTrajectoryItem*>(item)->trajectory_info;
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
  ctor();
}

JointStateItem::JointStateItem(const QString &text, tesseract_common::JointTrajectoryInfo& parent_trajectory, tesseract_common::JointState &state)
  : QStandardItem(*TRAJECTORY_STATE_ICON(), text)
  , parent_trajectory(parent_trajectory)
  , state(state)
{
  ctor();
}

JointStateItem::JointStateItem(const QIcon &icon, const QString &text, tesseract_common::JointTrajectoryInfo& parent_trajectory, tesseract_common::JointState &state)
  : QStandardItem(icon, text)
  , parent_trajectory(parent_trajectory)
  , state(state)
{
  ctor();
}

int JointStateItem::type() const
{
  return JointTrajectoryModel::STATE;
}

void JointStateItem::ctor()
{
  // Add State Joint Names
  QStandardItem* state_joint_names = new QStandardItem("joint_names");
  state_joint_names->setColumnCount(2);
  for (std::size_t k = 0; k < state.joint_names.size(); ++k)
  {
    QStandardItem* p_name = new QStandardItem(QString("[%1]").arg(k));
    QStandardItem* p_value = new QStandardItem(QString::fromStdString(state.joint_names[k]));
    state_joint_names->appendRow({p_name, p_value});
  }
  appendRow(state_joint_names);

  // Add State Position
  QStandardItem* state_position = new QStandardItem("position");
  state_position->setColumnCount(2);
  for (Eigen::Index k = 0; k < state.position.rows(); ++k)
  {
    QStandardItem* p_name = new QStandardItem(QString("[%1]").arg(k));
    QStandardItem* p_value = new QStandardItem(QString("%1").arg(state.position[k]));
    state_position->appendRow({p_name, p_value});
  }
  appendRow(state_position);

  // Add State Velocity
  QStandardItem* state_velocity = new QStandardItem("velocity");
  state_velocity->setColumnCount(2);
  for (int k = 0; k < state.velocity.rows(); ++k)
  {
    QStandardItem* p_name = new QStandardItem(QString("[%1]").arg(k));
    QStandardItem* p_value = new QStandardItem(QString("%1").arg(state.velocity[k]));
    state_velocity->appendRow({p_name, p_value});
  }
  appendRow(state_velocity);

  // Add State Velocity
  QStandardItem* state_acceleration = new QStandardItem("acceleration");
  state_acceleration->setColumnCount(2);
  for (int k = 0; k < state.acceleration.rows(); ++k)
  {
    QStandardItem* p_name = new QStandardItem(QString("[%1]").arg(k));
    QStandardItem* p_value = new QStandardItem(QString("%1").arg(state.acceleration[k]));
    state_acceleration->appendRow({p_name, p_value});
  }
  appendRow(state_acceleration);

  // Add time from start
  QStandardItem* p_name = new QStandardItem("time");
  QStandardItem* p_value = new QStandardItem(QString("%1").arg(state.time));
  appendRow({p_name, p_value});
}

JointTrajectoryItem::JointTrajectoryItem(tesseract_common::JointTrajectoryInfo& trajectory_info)
  : QStandardItem()
  , trajectory_info(trajectory_info)
{
  ctor();
}

JointTrajectoryItem::JointTrajectoryItem(const QString &text, tesseract_common::JointTrajectoryInfo& trajectory_info)
  : QStandardItem(*TRAJECTORY_ICON(), text)
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
  return JointTrajectoryModel::TRAJECTORY;
}

void JointTrajectoryItem::ctor()
{
  for (std::size_t j = 0; j < trajectory_info.trajectory.size(); ++j)
  {
    QStandardItem* trajectory_state = new JointStateItem(QString("state[%1]").arg(j), trajectory_info, trajectory_info.trajectory[j]);
    appendRow(trajectory_state);
  }
}

JointTrajectorySetItem::JointTrajectorySetItem(tesseract_common::JointTrajectorySet& trajectory_set)
  : QStandardItem()
  , trajectory_set(trajectory_set)
{
  ctor();
}

JointTrajectorySetItem::JointTrajectorySetItem(const QString &text, tesseract_common::JointTrajectorySet& trajectory_set)
  : QStandardItem(*TRAJECTORY_SET_ICON(), text)
  , trajectory_set(trajectory_set)
{
  ctor();
}
JointTrajectorySetItem::JointTrajectorySetItem(const QIcon &icon, const QString &text, tesseract_common::JointTrajectorySet &trajectory_set)
  : QStandardItem(icon, text)
  , trajectory_set(trajectory_set)
{
  ctor();
}

int JointTrajectorySetItem::type() const
{
  return JointTrajectoryModel::TRAJECTORY_SET;
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
