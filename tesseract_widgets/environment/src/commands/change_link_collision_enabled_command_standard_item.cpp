
#include <tesseract_widgets/environment/commands/change_link_collision_enabled_command_standard_item.h>
#include <tesseract_widgets/common/standard_item_utils.h>
#include <tesseract_widgets/common/standard_item_type.h>

namespace tesseract_gui
{

ChangeLinkCollisionEnabledCommandStandardItem::ChangeLinkCollisionEnabledCommandStandardItem(tesseract_environment::ChangeLinkCollisionEnabledCommand::ConstPtr command)
  : QStandardItem(QIcon(":/tesseract_widgets/png/merge.png"), "Change Link Collision Enabled")
  , command(std::move(command))
{
  ctor();
}

ChangeLinkCollisionEnabledCommandStandardItem::ChangeLinkCollisionEnabledCommandStandardItem(const QString &text, tesseract_environment::ChangeLinkCollisionEnabledCommand::ConstPtr command)
  : QStandardItem(QIcon(":/tesseract_widgets/png/merge.png"), text)
  , command(std::move(command))
{
  ctor();
}

ChangeLinkCollisionEnabledCommandStandardItem::ChangeLinkCollisionEnabledCommandStandardItem(const QIcon &icon, const QString &text, tesseract_environment::ChangeLinkCollisionEnabledCommand::ConstPtr command)
  : QStandardItem(icon, text)
  , command(std::move(command))
{
  ctor();
}

int ChangeLinkCollisionEnabledCommandStandardItem::type() const
{
  return static_cast<int>(StandardItemType::ENV_COMMAND_CHANGE_LINK_COLLISION_ENABLED);
}

void ChangeLinkCollisionEnabledCommandStandardItem::ctor()
{
  appendRow(createStandardItemString(QIcon(":/tesseract_widgets/ignition/link.png"), command->getLinkName(), (command->getEnabled()) ? "True" : "False"));
}
}
