#ifndef TESSERACT_GUI_KINEMATIC_GROUP_TCP_STANDARD_ITEM_H
#define TESSERACT_GUI_KINEMATIC_GROUP_TCP_STANDARD_ITEM_H

#include <tesseract_gui/scene_graph/origin_standard_item.h>

#include <QStandardItem>

namespace tesseract_gui
{
class TCPStandardItem : public QStandardItem
{
public:
  explicit TCPStandardItem(QString name, const Eigen::Isometry3d& tcp);
  explicit TCPStandardItem(const QString &text, QString name, const Eigen::Isometry3d& tcp);
  explicit TCPStandardItem(const QIcon &icon, const QString &text, QString name, const Eigen::Isometry3d& tcp);
  int type() const override;

  QString name;
  Eigen::Isometry3d tcp;

private:
  void ctor();
};
}

#endif // TESSERACT_GUI_KINEMATIC_GROUP_TCP_STANDARD_ITEM_H
