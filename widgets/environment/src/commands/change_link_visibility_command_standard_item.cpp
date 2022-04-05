
#include <tesseract_gui/widgets/environment/commands/change_link_visibility_command_standard_item.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, COMMAND_ICON, (":/tesseract_gui/png/merge.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, LINK_ICON, (":/tesseract_gui/ignition/link.png"));

namespace tesseract_gui
{

ChangeLinkVisibilityCommandStandardItem::ChangeLinkVisibilityCommandStandardItem(tesseract_environment::ChangeLinkVisibilityCommand::ConstPtr command)
  : QStandardItem(*COMMAND_ICON(), "Change Link Visibility")
  , command(std::move(command))
{
  ctor();
}

ChangeLinkVisibilityCommandStandardItem::ChangeLinkVisibilityCommandStandardItem(const QString &text, tesseract_environment::ChangeLinkVisibilityCommand::ConstPtr command)
  : QStandardItem(*COMMAND_ICON(), text)
  , command(std::move(command))
{
  ctor();
}

ChangeLinkVisibilityCommandStandardItem::ChangeLinkVisibilityCommandStandardItem(const QIcon &icon, const QString &text, tesseract_environment::ChangeLinkVisibilityCommand::ConstPtr command)
  : QStandardItem(icon, text)
  , command(std::move(command))
{
  ctor();
}

int ChangeLinkVisibilityCommandStandardItem::type() const
{
  return static_cast<int>(StandardItemType::ENV_COMMAND_CHANGE_LINK_VISIBILITY);
}

void ChangeLinkVisibilityCommandStandardItem::ctor()
{
  auto* x_name = new QStandardItem(*LINK_ICON(), command->getLinkName().c_str());
  auto* x_value = new QStandardItem((command->getEnabled()) ? "true" : "false");
  appendRow({x_name, x_value});
}
}
