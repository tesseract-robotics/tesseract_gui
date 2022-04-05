
#include <tesseract_gui/widgets/common/pairs_collision_margin_data_standard_item.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, CUBE_ICON, (":/tesseract_gui/png/cube.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, NUMERIC_ICON, (":/tesseract_gui/png/numeric.png"));

namespace tesseract_gui
{

PairsCollisionMarginDataStandardItem::PairsCollisionMarginDataStandardItem(tesseract_common::PairsCollisionMarginData pairs_margin_data)
  : QStandardItem(*CUBE_ICON(), "Pairs Collision Margin Data")
  , pairs_margin_data(std::move(pairs_margin_data))
{
  ctor();
}

PairsCollisionMarginDataStandardItem::PairsCollisionMarginDataStandardItem(const QString &text, tesseract_common::PairsCollisionMarginData pairs_margin_data)
  : QStandardItem(*CUBE_ICON(), text)
  , pairs_margin_data(std::move(pairs_margin_data))
{
  ctor();
}

PairsCollisionMarginDataStandardItem::PairsCollisionMarginDataStandardItem(const QIcon &icon, const QString &text, tesseract_common::PairsCollisionMarginData pairs_margin_data)
  : QStandardItem(icon, text)
  , pairs_margin_data(std::move(pairs_margin_data))
{
  ctor();
}

int PairsCollisionMarginDataStandardItem::type() const
{
  return static_cast<int>(StandardItemType::PAIRS_COLLISION_MARGIN_DATA);
}

void PairsCollisionMarginDataStandardItem::ctor()
{
  for (const auto& pair : pairs_margin_data)
    addPairCollisionMargin(pair.first, pair.second);

  sortChildren(0);
}

void PairsCollisionMarginDataStandardItem::addPairCollisionMargin(const tesseract_common::LinkNamesPair& pair, double collision_margin)
{
  QStandardItem* item;
  auto it = items_.find(pair.first);
  if (it == items_.end())
  {
    item = new QStandardItem(pair.first.c_str());
    appendRow({item, new QStandardItem()});
    items_[pair.first] = item;
  }
  else
  {
    item = it->second;
  }

  auto* name = new QStandardItem(*NUMERIC_ICON(), pair.second.c_str());
  auto* value = new QStandardItem(QString("%1").arg(collision_margin));
  item->appendRow({name, name});
}
}

