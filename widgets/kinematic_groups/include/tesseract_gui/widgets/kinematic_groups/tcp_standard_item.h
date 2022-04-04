#ifndef TESSERACT_GUI_KINEMATIC_GROUP_TCP_STANDARD_ITEM_H
#define TESSERACT_GUI_KINEMATIC_GROUP_TCP_STANDARD_ITEM_H

#include <QStandardItem>
#include <Eigen/Geometry>

namespace tesseract_gui
{
class TCPStandardItem : public QStandardItem
{
public:
  explicit TCPStandardItem(const Eigen::Isometry3d& tcp);
  explicit TCPStandardItem(const QString &text, const Eigen::Isometry3d& tcp);
  explicit TCPStandardItem(const QIcon &icon, const QString &text, const Eigen::Isometry3d& tcp);
  int type() const override;

private:
  void ctor(const Eigen::Isometry3d& tcp);
};
}

#endif // TESSERACT_GUI_KINEMATIC_GROUP_TCP_STANDARD_ITEM_H
