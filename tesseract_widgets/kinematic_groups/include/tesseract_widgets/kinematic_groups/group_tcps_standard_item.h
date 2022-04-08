#ifndef TESSERACT_WIDGETS_KINEMATIC_GROUP_GROUP_TCPS_STANDARD_ITEM_H
#define TESSERACT_WIDGETS_KINEMATIC_GROUP_GROUP_TCPS_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_srdf/kinematics_information.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class GroupTCPsStandardItem : public QStandardItem
{
public:
  GroupTCPsStandardItem();
  explicit GroupTCPsStandardItem(tesseract_srdf::GroupTCPs group_tcps);
  explicit GroupTCPsStandardItem(const QString &text, tesseract_srdf::GroupTCPs group_tcps);
  explicit GroupTCPsStandardItem(const QIcon &icon, const QString &text, tesseract_srdf::GroupTCPs group_tcps);
  int type() const override;

  void addGroupTCP(const QString& group_name, const QString& tcp_name, const Eigen::Isometry3d& tcp);
  void removeGroupTCP(const QString& group_name, const QString& tcp_name);

  const tesseract_srdf::GroupTCPs& getGroupTCPs() const;

private:
  void ctor();

  tesseract_srdf::GroupTCPs group_tcps_;
  std::unordered_map<std::string, QStandardItem*> group_items_;

  void addGroupTCPItem(const QString& group_name, const QString& tcp_name, const Eigen::Isometry3d& tcp);
};
}
#endif // TESSERACT_WIDGETS_KINEMATIC_GROUP_GROUPS_TCPS_STANDARD_ITEM_H
