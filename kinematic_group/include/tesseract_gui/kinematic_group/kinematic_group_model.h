#ifndef TESSERACT_GUI_KINEMATIC_GROUP_MODEL_H
#define TESSERACT_GUI_KINEMATIC_GROUP_MODEL_H


#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_srdf/kinematics_information.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class ChainGroupStandardItem : public QStandardItem
{
public:
  ChainGroupStandardItem(tesseract_srdf::ChainGroup group);
  explicit ChainGroupStandardItem(const QString &text, tesseract_srdf::ChainGroup group);
  ChainGroupStandardItem(const QIcon &icon, const QString &text, tesseract_srdf::ChainGroup group);
  int type() const override;

  tesseract_srdf::ChainGroup group;

private:
  void ctor();
};

class JointGroupStandardItem : public QStandardItem
{
public:
  JointGroupStandardItem(tesseract_srdf::JointGroup group);
  explicit JointGroupStandardItem(const QString &text, tesseract_srdf::JointGroup group);
  JointGroupStandardItem(const QIcon &icon, const QString &text, tesseract_srdf::JointGroup group);
  int type() const override;

  tesseract_srdf::JointGroup group;

private:
  void ctor();
};

class LinkGroupStandardItem : public QStandardItem
{
public:
  LinkGroupStandardItem(tesseract_srdf::LinkGroup group);
  explicit LinkGroupStandardItem(const QString &text, tesseract_srdf::LinkGroup group);
  LinkGroupStandardItem(const QIcon &icon, const QString &text, tesseract_srdf::LinkGroup group);
  int type() const override;

  tesseract_srdf::LinkGroup group;

private:
  void ctor();
};

class KinematicGroupModel : public QStandardItemModel
{
  Q_OBJECT

public:

  explicit KinematicGroupModel(QObject *parent = nullptr);
  KinematicGroupModel(const KinematicGroupModel &other);
  KinematicGroupModel &operator=(const KinematicGroupModel &other);

  void addChainGroup(QString group_name, tesseract_srdf::ChainGroup group);
  void addJointGroup(QString group_name, tesseract_srdf::JointGroup group);
  void addLinkGroup(QString group_name, tesseract_srdf::LinkGroup group);

  void removeGroup(QString group_name);

  const tesseract_srdf::ChainGroups& getChainGroups() const;
  const tesseract_srdf::JointGroups& getJointGroups() const;
  const tesseract_srdf::LinkGroups& getLinkGroups() const;

//  const tesseract_common::JointState& getJointState(const QModelIndex& row) const;
//  const tesseract_common::JointTrajectoryInfo& getJointTrajectory(const QModelIndex& row) const;
//  const tesseract_common::JointTrajectorySet& getJointTrajectorySet(const QModelIndex& row) const;

  void clear();

private:
  tesseract_srdf::GroupNames group_names_;
  tesseract_srdf::ChainGroups chain_groups_;
  tesseract_srdf::JointGroups joint_groups_;
  tesseract_srdf::LinkGroups link_groups_;
};

}

#endif // TESSERACT_GUI_KINEMATIC_GROUP_MODEL_H
