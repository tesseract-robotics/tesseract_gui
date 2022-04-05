#ifndef TESSERACT_GUI_WIDGETS_COMMON_PAIRS_COLLISION_MARGIN_DATA_STANDARD_ITEM_H
#define TESSERACT_GUI_WIDGETS_COMMON_PAIRS_COLLISION_MARGIN_DATA_STANDARD_ITEM_H


#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_common/collision_margin_data.h>
#include <unordered_map>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class PairsCollisionMarginDataStandardItem : public QStandardItem
{
public:
  PairsCollisionMarginDataStandardItem(tesseract_common::PairsCollisionMarginData pairs_margin_data);
  explicit PairsCollisionMarginDataStandardItem(const QString &text, tesseract_common::PairsCollisionMarginData pairs_margin_data);
  PairsCollisionMarginDataStandardItem(const QIcon &icon, const QString &text, tesseract_common::PairsCollisionMarginData pairs_margin_data);
  int type() const override;


  tesseract_common::PairsCollisionMarginData pairs_margin_data;

private:
  void ctor();

  void addPairCollisionMargin(const tesseract_common::LinkNamesPair& pair, double collision_margin);

  std::unordered_map<std::string, QStandardItem*> items_;
};
}

#endif // TESSERACT_GUI_WIDGETS_COMMON_PAIRS_COLLISION_MARGIN_DATA_STANDARD_ITEM_H
