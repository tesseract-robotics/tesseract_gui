
#include <tesseract_widgets/environment/commands/add_allowed_collision_command_standard_item.h>
#include <tesseract_widgets/common/standard_item_utils.h>
#include <tesseract_widgets/common/standard_item_type.h>

namespace tesseract_gui
{

AddAllowedCollisionCommandStandardItem::AddAllowedCollisionCommandStandardItem(tesseract_environment::AddAllowedCollisionCommand::ConstPtr command)
  : QStandardItem(QIcon(":/tesseract_widgets/png/merge.png"), "Add Allowed Collision")
  , command(std::move(command))
{
  ctor();
}

AddAllowedCollisionCommandStandardItem::AddAllowedCollisionCommandStandardItem(const QString &text, tesseract_environment::AddAllowedCollisionCommand::ConstPtr command)
  : QStandardItem(QIcon(":/tesseract_widgets/png/merge.png"), text)
  , command(std::move(command))
{
  ctor();
}

AddAllowedCollisionCommandStandardItem::AddAllowedCollisionCommandStandardItem(const QIcon &icon, const QString &text, tesseract_environment::AddAllowedCollisionCommand::ConstPtr command)
  : QStandardItem(icon, text)
  , command(std::move(command))
{
  ctor();
}

int AddAllowedCollisionCommandStandardItem::type() const
{
  return static_cast<int>(StandardItemType::ENV_COMMAND_ADD_ALLOWED_COLLISION);
}

void AddAllowedCollisionCommandStandardItem::ctor()
{
  appendRow(createStandardItemString("link_name1", command->getLinkName1()));
  appendRow(createStandardItemString("link_name2", command->getLinkName2()));
  appendRow(createStandardItemString("reason", command->getReason()));
}
}

