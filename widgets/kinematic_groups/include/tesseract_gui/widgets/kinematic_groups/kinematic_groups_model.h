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

class KinematicGroupsModel : public QStandardItemModel
{
  Q_OBJECT

public:

  explicit KinematicGroupsModel(QObject *parent = nullptr);
  KinematicGroupsModel(const KinematicGroupsModel &other);
  KinematicGroupsModel &operator=(const KinematicGroupsModel &other);

  void set(const tesseract_srdf::ChainGroups& chain_groups,
           const tesseract_srdf::JointGroups& joint_groups,
           const tesseract_srdf::LinkGroups& link_groups);
  void addChainGroup(QString group_name, tesseract_srdf::ChainGroup group);
  void addJointGroup(QString group_name, tesseract_srdf::JointGroup group);
  void addLinkGroup(QString group_name, tesseract_srdf::LinkGroup group);

  void removeGroup(QString group_name);
  void clear();

  const tesseract_srdf::GroupNames& getGroupNames() const;
  const tesseract_srdf::ChainGroups& getChainGroups() const;
  const tesseract_srdf::JointGroups& getJointGroups() const;
  const tesseract_srdf::LinkGroups& getLinkGroups() const;

Q_SIGNALS:
  void groupRemoved(QString group_name);
  void groupAdded(QString group_name);
  void cleared();

private:
  tesseract_srdf::GroupNames group_names_;
  tesseract_srdf::ChainGroups chain_groups_;
  tesseract_srdf::JointGroups joint_groups_;
  tesseract_srdf::LinkGroups link_groups_;
};

}

#endif // TESSERACT_GUI_KINEMATIC_GROUP_MODEL_H
