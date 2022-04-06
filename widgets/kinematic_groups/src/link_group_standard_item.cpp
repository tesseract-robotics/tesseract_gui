
#include <tesseract_gui/widgets/kinematic_groups/link_group_standard_item.h>
#include <tesseract_gui/widgets/common/standard_item_utils.h>
#include <tesseract_gui/common/standard_item_type.h>

namespace tesseract_gui
{

LinkGroupStandardItem::LinkGroupStandardItem(tesseract_srdf::LinkGroup group)
  : QStandardItem(QIcon(":/tesseract_gui/png/robotic-arm.png"), "Joint Group")
  , group(std::move(group))
{
  ctor();
}

LinkGroupStandardItem::LinkGroupStandardItem(const QString &text, tesseract_srdf::LinkGroup group)
  : QStandardItem(QIcon(":/tesseract_gui/png/robotic-arm.png"), text)
  , group(std::move(group))
{
  ctor();
}

LinkGroupStandardItem::LinkGroupStandardItem(const QIcon &icon, const QString &text, tesseract_srdf::LinkGroup group)
  : QStandardItem(icon, text)
  , group(std::move(group))
{
  ctor();
}

int LinkGroupStandardItem::type() const
{
  return static_cast<int>(StandardItemType::LINK_GROUP);
}

void LinkGroupStandardItem::ctor()
{
  for (const auto& link : group)
    appendRow(createStandardItemString("link", link));
}
}

