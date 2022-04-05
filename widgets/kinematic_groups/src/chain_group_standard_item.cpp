
#include <tesseract_gui/widgets/kinematic_groups/chain_group_standard_item.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, ROBOT_ICON, (":/tesseract_gui/png/robotic-arm.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, TEXT_ICON, (":/tesseract_gui/png/text.png"));

namespace tesseract_gui
{

ChainGroupStandardItem::ChainGroupStandardItem(tesseract_srdf::ChainGroup group)
  : QStandardItem(*ROBOT_ICON(), "Chain Group")
  , group(std::move(group))
{
  ctor();
}

ChainGroupStandardItem::ChainGroupStandardItem(const QString &text, tesseract_srdf::ChainGroup group)
  : QStandardItem(*ROBOT_ICON(), text)
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
  {
    auto* name = new QStandardItem(*TEXT_ICON(), "base link");
    auto* value = new QStandardItem(QString::fromStdString(group.front().first));
    appendRow({name, value});
  }

  {
    auto* name = new QStandardItem(*TEXT_ICON(), "tip link");
    auto* value = new QStandardItem(QString::fromStdString(group.front().second));
    appendRow({name, value});
  }
}
}

