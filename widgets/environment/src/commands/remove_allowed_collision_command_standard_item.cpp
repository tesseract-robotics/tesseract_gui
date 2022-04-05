
#include <tesseract_gui/widgets/environment/commands/remove_allowed_collision_command_standard_item.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, COMMAND_ICON, (":/tesseract_gui/png/merge.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, TEXT_ICON, (":/tesseract_gui/png/text.png"));

namespace tesseract_gui
{

RemoveAllowedCollisionCommandStandardItem::RemoveAllowedCollisionCommandStandardItem(tesseract_environment::RemoveAllowedCollisionCommand::ConstPtr command)
  : QStandardItem(*COMMAND_ICON(), "Remove Allowed Collision")
  , command(std::move(command))
{
  ctor();
}

RemoveAllowedCollisionCommandStandardItem::RemoveAllowedCollisionCommandStandardItem(const QString &text, tesseract_environment::RemoveAllowedCollisionCommand::ConstPtr command)
  : QStandardItem(*COMMAND_ICON(), text)
  , command(std::move(command))
{
  ctor();
}

RemoveAllowedCollisionCommandStandardItem::RemoveAllowedCollisionCommandStandardItem(const QIcon &icon, const QString &text, tesseract_environment::RemoveAllowedCollisionCommand::ConstPtr command)
  : QStandardItem(icon, text)
  , command(std::move(command))
{
  ctor();
}

int RemoveAllowedCollisionCommandStandardItem::type() const
{
  return static_cast<int>(StandardItemType::ENV_COMMAND_REMOVE_ALLOWED_COLLISION);
}

void RemoveAllowedCollisionCommandStandardItem::ctor()
{
  auto* l1_name = new QStandardItem(*TEXT_ICON(), "Link Name 1");
  auto* l1_value = new QStandardItem(command->getLinkName1().c_str());
  appendRow({l1_name, l1_value});

  auto* l2_name = new QStandardItem(*TEXT_ICON(), "Link Name 2");
  auto* l2_value = new QStandardItem(command->getLinkName2().c_str());
  appendRow({l2_name, l2_value});
}
}
