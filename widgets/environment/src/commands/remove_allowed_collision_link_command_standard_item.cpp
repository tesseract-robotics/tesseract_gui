
#include <tesseract_gui/widgets/environment/commands/remove_allowed_collision_link_command_standard_item.h>
#include <tesseract_gui/widgets/common/standard_item_utils.h>
#include <tesseract_gui/common/standard_item_type.h>

namespace tesseract_gui
{

RemoveAllowedCollisionLinkCommandStandardItem::RemoveAllowedCollisionLinkCommandStandardItem(tesseract_environment::RemoveAllowedCollisionLinkCommand::ConstPtr command)
  : QStandardItem(QIcon(":/tesseract_gui/png/merge.png"), "Remove Allowed Collision Link")
  , command(std::move(command))
{
  ctor();
}

RemoveAllowedCollisionLinkCommandStandardItem::RemoveAllowedCollisionLinkCommandStandardItem(const QString &text, tesseract_environment::RemoveAllowedCollisionLinkCommand::ConstPtr command)
  : QStandardItem(QIcon(":/tesseract_gui/png/merge.png"), text)
  , command(std::move(command))
{
  ctor();
}

RemoveAllowedCollisionLinkCommandStandardItem::RemoveAllowedCollisionLinkCommandStandardItem(const QIcon &icon, const QString &text, tesseract_environment::RemoveAllowedCollisionLinkCommand::ConstPtr command)
  : QStandardItem(icon, text)
  , command(std::move(command))
{
  ctor();
}

int RemoveAllowedCollisionLinkCommandStandardItem::type() const
{
  return static_cast<int>(StandardItemType::ENV_COMMAND_REMOVE_ALLOWED_COLLISION_LINK);
}

void RemoveAllowedCollisionLinkCommandStandardItem::ctor()
{
  appendRow(createStandardItemString("link name", command->getLinkName()));
}
}
