#ifndef TESSERACT_GUI_SCENE_GRAPH_ORIGIN_STANDARD_ITEM_H
#define TESSERACT_GUI_SCENE_GRAPH_ORIGIN_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <Eigen/Geometry>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class OriginStandardItem : public QStandardItem
{
public:
  explicit OriginStandardItem(Eigen::Isometry3d& origin);
  explicit OriginStandardItem(const QString &text, Eigen::Isometry3d& origin);
  explicit OriginStandardItem(const QIcon &icon, const QString &text, Eigen::Isometry3d& origin);
  int type() const override;

  Eigen::Isometry3d& origin;

private:
  void ctor();
};
}

#endif // TESSERACT_GUI_SCENE_GRAPH_ORIGIN_STANDARD_ITEM_H
