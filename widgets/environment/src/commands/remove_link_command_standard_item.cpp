
#include <tesseract_gui/widgets/environment/commands/remove_link_command_standard_item.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, COMMAND_ICON, (":/tesseract_gui/png/merge.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, TEXT_ICON, (":/tesseract_gui/png/text.png"));

namespace tesseract_gui
{

RemoveLinkCommandStandardItem::RemoveLinkCommandStandardItem(tesseract_environment::RemoveLinkCommand::ConstPtr command)
  : QStandardItem(*COMMAND_ICON(), "Remove Link")
  , command(std::move(command))
{
  ctor();
}

RemoveLinkCommandStandardItem::RemoveLinkCommandStandardItem(const QString &text, tesseract_environment::RemoveLinkCommand::ConstPtr command)
  : QStandardItem(*COMMAND_ICON(), text)
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
  auto* name = new QStandardItem(*TEXT_ICON(), "link name");
  auto* value = new QStandardItem(command->getLinkName().c_str());
  appendRow({name, value});
}
}
