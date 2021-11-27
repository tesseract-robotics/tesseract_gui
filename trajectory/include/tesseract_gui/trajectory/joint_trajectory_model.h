#ifndef TESSERACT_GUI_TRAJECTORY_JOINT_TRAJECTORY_MODEL_H
#define TESSERACT_GUI_TRAJECTORY_JOINT_TRAJECTORY_MODEL_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_common/joint_state.h>
#include <QStandardItemModel>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP


namespace tesseract_gui
{


class JointTrajectoryModel : public QStandardItemModel
{
  Q_OBJECT

public:
  enum ItemType
  {
    STATE = QStandardItem::UserType + 1,
    TRAJECTORY = QStandardItem::UserType + 2,
    TRAJECTORY_SET = QStandardItem::UserType + 3
  };

  explicit JointTrajectoryModel(QObject *parent = nullptr);
  JointTrajectoryModel(const JointTrajectoryModel &other);
  JointTrajectoryModel &operator=(const JointTrajectoryModel &other);

  /**
   * @brief setTrajectories
   * @param trajectories
   */
  void addTrajectorySet(const QString& key, const std::vector<tesseract_common::JointTrajectory>& trajectories);

  void removeTrajectorySet(const QString& key);

  const tesseract_common::JointState& getJointState(const QModelIndex& row) const;
  const tesseract_common::JointTrajectory& getJointTrajectory(const QModelIndex& row) const;
  const std::vector<tesseract_common::JointTrajectory>& getJointTrajectorySet(const QModelIndex& row) const;

  void clear();

private:
  std::map<QString, std::vector<tesseract_common::JointTrajectory>> trajectory_sets_;
  QIcon trajectory_icon_;
  QIcon trajectory_state_icon_;
  QIcon trajectory_set_icon_;
};

class JointStateItem : public QStandardItem
{
public:
  JointStateItem(tesseract_common::JointState& state);
  explicit JointStateItem(const QString &text, tesseract_common::JointState& state);
  JointStateItem(const QIcon &icon, const QString &text, tesseract_common::JointState& state);
  int type() const override;

  tesseract_common::JointState& state;
};

class JointTrajectoryItem : public QStandardItem
{
public:
  JointTrajectoryItem(tesseract_common::JointTrajectory& trajectory);
  explicit JointTrajectoryItem(const QString &text, tesseract_common::JointTrajectory& trajectory);
  JointTrajectoryItem(const QIcon &icon, const QString &text, tesseract_common::JointTrajectory& trajectory);
  int type() const override;
  tesseract_common::JointTrajectory& trajectory;
};

class JointTrajectorySetItem : public QStandardItem
{
public:
  JointTrajectorySetItem(std::vector<tesseract_common::JointTrajectory>& trajectory_set);
  explicit JointTrajectorySetItem(const QString &text, std::vector<tesseract_common::JointTrajectory>& trajectory_set);
  JointTrajectorySetItem(const QIcon &icon, const QString &text, std::vector<tesseract_common::JointTrajectory>& trajectory_set);
  int type() const override;
  std::vector<tesseract_common::JointTrajectory>& trajectory_set;
};
}

#endif // TESSERACT_GUI_TRAJECTORY_JOINT_TRAJECTORY_MODEL_H
