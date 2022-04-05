#ifndef TESSERACT_GUI_WIDGETS_COMMON_TRANSFORM_STANDARD_ITEM_H
#define TESSERACT_GUI_WIDGETS_COMMON_TRANSFORM_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <Eigen/Geometry>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class TransformStandardItem : public QStandardItem
{
public:
  explicit TransformStandardItem(const Eigen::Isometry3d& transform);
  explicit TransformStandardItem(const QString &text, const Eigen::Isometry3d& transform);
  explicit TransformStandardItem(const QIcon &icon, const QString &text, const Eigen::Isometry3d& transform);
  int type() const override;

  Eigen::Isometry3d transform;

private:
  void ctor();
};
}

#endif // TESSERACT_GUI_WIDGETS_COMMON_TRANSFORM_STANDARD_ITEM_H
