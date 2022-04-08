
#include <tesseract_widgets/kinematic_groups/chain_group_standard_item.h>
#include <tesseract_widgets/common/standard_item_utils.h>
#include <tesseract_widgets/common/standard_item_type.h>

namespace tesseract_gui
{

ChainGroupStandardItem::ChainGroupStandardItem(tesseract_srdf::ChainGroup group)
  : QStandardItem(QIcon(":/tesseract_widgets/png/robotic-arm.png"), "Chain Group")
  , group(std::move(group))
{
  ctor();
}

ChainGroupStandardItem::ChainGroupStandardItem(const QString &text, tesseract_srdf::ChainGroup group)
  : QStandardItem(QIcon(":/tesseract_widgets/png/robotic-arm.png"), text)
  , group(std::move(group))
{
  ctor();
}

ChainGroupStandardItem::ChainGroupStandardItem(const QIcon &icon, const QString &text, tesseract_srdf::ChainGroup group)
  : QStandardItem(icon, text)
  , group(std::move(group))
{
  ctor();
}

int ChainGroupStandardItem::type() const
{
  return static_cast<int>(StandardItemType::CHAIN_GROUP);
}

void ChainGroupStandardItem::ctor()
{
  appendRow(createStandardItemString("base link", group.front().first));
  appendRow(createStandardItemString("tip link", group.front().second));
}
}

