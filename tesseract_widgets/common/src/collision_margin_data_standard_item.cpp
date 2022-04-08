
#include <tesseract_widgets/common/collision_margin_data_standard_item.h>
#include <tesseract_widgets/common/pairs_collision_margin_data_standard_item.h>
#include <tesseract_widgets/common/standard_item_utils.h>
#include <tesseract_widgets/common/standard_item_type.h>

namespace tesseract_gui
{

CollisionMarginDataStandardItem::CollisionMarginDataStandardItem(tesseract_common::CollisionMarginData collision_margin_data)
  : QStandardItem(QIcon(":/tesseract_widgets/png/cube.png"), "Collision Margin Data")
  , collision_margin_data(std::move(collision_margin_data))
{
  ctor();
}

CollisionMarginDataStandardItem::CollisionMarginDataStandardItem(const QString &text, tesseract_common::CollisionMarginData collision_margin_data)
  : QStandardItem(QIcon(":/tesseract_widgets/png/cube.png"), text)
  , collision_margin_data(std::move(collision_margin_data))
{
  ctor();
}

CollisionMarginDataStandardItem::CollisionMarginDataStandardItem(const QIcon &icon, const QString &text, tesseract_common::CollisionMarginData collision_margin_data)
  : QStandardItem(icon, text)
  , collision_margin_data(std::move(collision_margin_data))
{
  ctor();
}

int CollisionMarginDataStandardItem::type() const
{
  return static_cast<int>(StandardItemType::COLLISION_MARGIN_DATA);
}

void CollisionMarginDataStandardItem::ctor()
{
  appendRow(createStandardItemFloat("Default Margin", collision_margin_data.getDefaultCollisionMargin()));
  appendRow(createStandardItemFloat("Max Margin", collision_margin_data.getMaxCollisionMargin()));
  appendRow(new PairsCollisionMarginDataStandardItem(collision_margin_data.getPairCollisionMargins()));
}
}

