#ifndef TESSERACT_WIDGETS_TRAJECTORY_JOINT_TRAJECTORY_MODEL_H
#define TESSERACT_WIDGETS_TRAJECTORY_JOINT_TRAJECTORY_MODEL_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_common/joint_state.h>
//#include <tesseract_common/joint_trajectory_set.h>
#include <tesseract_widgets/common/joint_trajectory_set.h>
#include <QStandardItemModel>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP


namespace tesseract_gui
{


class JointTrajectoryModel : public QStandardItemModel
{
  Q_OBJECT

public:
  explicit JointTrajectoryModel(QObject *parent = nullptr);
  JointTrajectoryModel(const JointTrajectoryModel &other);
  JointTrajectoryModel &operator=(const JointTrajectoryModel &other);

  /**
   * @brief setTrajectories
   * @param trajectories
   */
  void addJointTrajectorySet(const QString& key, const tesseract_common::JointTrajectorySet& trajectory_set);

  void removeJointTrajectorySet(const QString& key);

  const tesseract_common::JointState& getJointState(const QModelIndex& row) const;
  const tesseract_common::JointTrajectoryInfo& getJointTrajectory(const QModelIndex& row) const;
  const tesseract_common::JointTrajectorySet& getJointTrajectorySet(const QModelIndex& row) const;

  void clear();

private:
  std::map<QString, tesseract_common::JointTrajectorySet> trajectory_sets_;
};

class JointStateItem : public QStandardItem
{
public:
  JointStateItem(tesseract_common::JointState& state);
  explicit JointStateItem(const QString &text, tesseract_common::JointState& state);
  JointStateItem(const QIcon &icon, const QString &text, tesseract_common::JointState& state);
  int type() const override;

  tesseract_common::JointState& state;
private:
  void ctor();
};

class JointTrajectoryItem : public QStandardItem
{
public:
  JointTrajectoryItem(tesseract_common::JointTrajectoryInfo& trajectory_info);
  explicit JointTrajectoryItem(const QString &text, tesseract_common::JointTrajectoryInfo& trajectory_info);
  JointTrajectoryItem(const QIcon &icon, const QString &text, tesseract_common::JointTrajectoryInfo& trajectory_info);
  int type() const override;

  tesseract_common::JointTrajectoryInfo& trajectory_info;
private:
  void ctor();
};

class JointTrajectorySetItem : public QStandardItem
{
public:
  JointTrajectorySetItem(tesseract_common::JointTrajectorySet& trajectory_set);
  explicit JointTrajectorySetItem(const QString &text, tesseract_common::JointTrajectorySet &trajectory_set);
  JointTrajectorySetItem(const QIcon &icon, const QString &text, tesseract_common::JointTrajectorySet& trajectory_set);
  int type() const override;

  tesseract_common::JointTrajectorySet& trajectory_set;
private:
  void ctor();
};
}

#endif // TESSERACT_WIDGETS_TRAJECTORY_JOINT_TRAJECTORY_MODEL_H
