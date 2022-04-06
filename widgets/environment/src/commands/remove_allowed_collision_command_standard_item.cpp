
#include <tesseract_gui/widgets/environment/commands/remove_allowed_collision_command_standard_item.h>
#include <tesseract_gui/widgets/common/standard_item_utils.h>
#include <tesseract_gui/common/standard_item_type.h>

namespace tesseract_gui
{

RemoveAllowedCollisionCommandStandardItem::RemoveAllowedCollisionCommandStandardItem(tesseract_environment::RemoveAllowedCollisionCommand::ConstPtr command)
  : QStandardItem(QIcon(":/tesseract_gui/png/merge.png"), "Remove Allowed Collision")
  , command(std::move(command))
{
  ctor();
}

RemoveAllowedCollisionCommandStandardItem::RemoveAllowedCollisionCommandStandardItem(const QString &text, tesseract_environment::RemoveAllowedCollisionCommand::ConstPtr command)
  : QStandardItem(QIcon(":/tesseract_gui/png/merge.png"), text)
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
  appendRow(createStandardItemString("Link Name 1", command->getLinkName1()));
  appendRow(createStandardItemString("Link Name 2", command->getLinkName2()));
}
}
