#ifndef TESSERACT_GUI_KINEMATIC_GROUP_TCP_STANDARD_ITEM_H
#define TESSERACT_GUI_KINEMATIC_GROUP_TCP_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_kinematics/opw/opw_inv_kin.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <tesseract_gui/scene_graph/origin_standard_item.h>

#include <QStandardItem>

namespace tesseract_gui
{
class TCPStandardItem : public QStandardItem
{
public:
  TCPStandardItem(QString name, const Eigen::Isometry3d& tcp);
  explicit TCPStandardItem(const QString &text, QString name, const Eigen::Isometry3d& tcp);
  TCPStandardItem(const QIcon &icon, const QString &text, QString name, const Eigen::Isometry3d& tcp);
  int type() const override;

  QString name;
  Eigen::Isometry3d tcp;

private:
  void ctor();
};
}

#endif // TESSERACT_GUI_KINEMATIC_GROUP_TCP_STANDARD_ITEM_H
