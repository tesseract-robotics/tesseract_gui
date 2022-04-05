
#include <tesseract_gui/widgets/kinematic_groups/link_group_standard_item.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, ROBOT_ICON, (":/tesseract_gui/png/robotic-arm.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, TEXT_ICON, (":/tesseract_gui/png/text.png"));

namespace tesseract_gui
{

LinkGroupStandardItem::LinkGroupStandardItem(tesseract_srdf::LinkGroup group)
  : QStandardItem(*ROBOT_ICON(), "Joint Group")
  , group(std::move(group))
{
  ctor();
}

LinkGroupStandardItem::LinkGroupStandardItem(const QString &text, tesseract_srdf::LinkGroup group)
  : QStandardItem(*ROBOT_ICON(), text)
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
  {
    auto* name = new QStandardItem(*TEXT_ICON(), "link");
    auto* value = new QStandardItem(QString::fromStdString(link));
    appendRow({name, value});
  }
}
}

