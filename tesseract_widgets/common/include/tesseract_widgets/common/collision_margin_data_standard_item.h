#ifndef TESSERACT_WIDGETS_COMMON_COLLISION_MARGIN_DATA_STANDARD_ITEM_H
#define TESSERACT_WIDGETS_COMMON_COLLISION_MARGIN_DATA_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_common/collision_margin_data.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class CollisionMarginDataStandardItem : public QStandardItem
{
public:
  CollisionMarginDataStandardItem(tesseract_common::CollisionMarginData collision_margin_data);
  explicit CollisionMarginDataStandardItem(const QString &text, tesseract_common::CollisionMarginData collision_margin_data);
  CollisionMarginDataStandardItem(const QIcon &icon, const QString &text, tesseract_common::CollisionMarginData collision_margin_data);
  int type() const override;

  tesseract_common::CollisionMarginData collision_margin_data;

private:
  void ctor();
};
}

#endif // TESSERACT_WIDGETS_COMMON_COLLISION_MARGIN_DATA_STANDARD_ITEM_H
