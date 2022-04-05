#ifndef TESSERACT_GUI_KINEMATIC_GROUP_GROUP_TCPS_MODEL_H
#define TESSERACT_GUI_KINEMATIC_GROUP_GROUP_TCPS_MODEL_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_srdf/kinematics_information.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItemModel>
#include <tesseract_gui/widgets/kinematic_groups/group_tcps_standard_item.h>

namespace tesseract_gui
{

class GroupTCPsModel : public QStandardItemModel
{
  Q_OBJECT

public:

  explicit GroupTCPsModel(QObject *parent = nullptr);
  GroupTCPsModel(const GroupTCPsModel &other);
  GroupTCPsModel &operator=(const GroupTCPsModel &other);

  void set(const tesseract_srdf::GroupTCPs& group_tcps);
  void addGroupTCP(const QString& group_name, const QString& tcp_name, const Eigen::Isometry3d& tcp);
  void removeGroupTCP(const QString& group_name, const QString& tcp_name);
  void clear();

  const tesseract_srdf::GroupTCPs& getGroupTCPs() const;

private:
  GroupTCPsStandardItem* getRoot();
  const GroupTCPsStandardItem* getRoot() const;
};

}

#endif // TESSERACT_GUI_KINEMATIC_GROUP_GROUP_TCPS_MODEL_H
