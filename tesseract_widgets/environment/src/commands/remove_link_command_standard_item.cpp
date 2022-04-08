
#include <tesseract_widgets/environment/commands/remove_link_command_standard_item.h>
#include <tesseract_widgets/common/standard_item_utils.h>
#include <tesseract_widgets/common/standard_item_type.h>

namespace tesseract_gui
{

RemoveLinkCommandStandardItem::RemoveLinkCommandStandardItem(tesseract_environment::RemoveLinkCommand::ConstPtr command)
  : QStandardItem(QIcon(":/tesseract_widgets/png/merge.png"), "Remove Link")
  , command(std::move(command))
{
  ctor();
}

RemoveLinkCommandStandardItem::RemoveLinkCommandStandardItem(const QString &text, tesseract_environment::RemoveLinkCommand::ConstPtr command)
  : QStandardItem(QIcon(":/tesseract_widgets/png/merge.png"), text)
  , command(std::move(command))
{
  ctor();
}

RemoveLinkCommandStandardItem::RemoveLinkCommandStandardItem(const QIcon &icon, const QString &text, tesseract_environment::RemoveLinkCommand::ConstPtr command)
  : QStandardItem(icon, text)
  , command(std::move(command))
{
  ctor();
}

int RemoveLinkCommandStandardItem::type() const
{
  return static_cast<int>(StandardItemType::ENV_COMMAND_REMOVE_LINK);
}

void RemoveLinkCommandStandardItem::ctor()
{
  appendRow(createStandardItemString("link name", command->getLinkName()));
}
}
