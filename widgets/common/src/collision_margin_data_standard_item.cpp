
#include <tesseract_gui/widgets/common/collision_margin_data_standard_item.h>
#include <tesseract_gui/widgets/common/pairs_collision_margin_data_standard_item.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, CUBE_ICON, (":/tesseract_gui/png/cube.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, NUMERIC_ICON, (":/tesseract_gui/png/numeric.png"));

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
  auto* default_name = new QStandardItem(*NUMERIC_ICON(), "Default Margin");
  auto* default_value = new QStandardItem(QString("%1").arg(collision_margin_data.getDefaultCollisionMargin()));
  appendRow({default_name, default_value});

  auto* max_name = new QStandardItem(*NUMERIC_ICON(), "Max Margin");
  auto* max_value = new QStandardItem(QString("%1").arg(collision_margin_data.getMaxCollisionMargin()));
  appendRow({max_name, max_value});

  auto* pairs_item = new PairsCollisionMarginDataStandardItem(collision_margin_data.getPairCollisionMargins());
  appendRow({pairs_item, new QStandardItem()});
}
}

