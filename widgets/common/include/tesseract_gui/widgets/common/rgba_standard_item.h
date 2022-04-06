#ifndef TESSERACT_GUI_WIDGETS_COMMON_RGBA_STANDARD_ITEM_H
#define TESSERACT_GUI_WIDGETS_COMMON_RGBA_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <Eigen/Geometry>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class RGBAStandardItem : public QStandardItem
{
public:
  explicit RGBAStandardItem(const Eigen::Vector4d& rgba);
  explicit RGBAStandardItem(const QString &text, const Eigen::Vector4d& rgba);
  explicit RGBAStandardItem(const QIcon &icon, const QString &text, const Eigen::Vector4d& rgba);
  int type() const override;

  Eigen::Vector4d rgba;

private:
  void ctor();
};
}

#endif // TESSERACT_GUI_WIDGETS_COMMON_RGBA_STANDARD_ITEM_H
