
#include <tesseract_gui/widgets/common/collision_margin_data_standard_item.h>
#include <tesseract_gui/widgets/common/pairs_collision_margin_data_standard_item.h>
#include <tesseract_gui/widgets/common/standard_item_utils.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, CUBE_ICON, (":/tesseract_gui/png/cube.png"));

namespace tesseract_gui
{

CollisionMarginDataStandardItem::CollisionMarginDataStandardItem(tesseract_common::CollisionMarginData collision_margin_data)
  : QStandardItem(*CUBE_ICON(), "Collision Margin Data")
  , collision_margin_data(std::move(collision_margin_data))
{
  ctor();
}

CollisionMarginDataStandardItem::CollisionMarginDataStandardItem(const QString &text, tesseract_common::CollisionMarginData collision_margin_data)
  : QStandardItem(*CUBE_ICON(), text)
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

