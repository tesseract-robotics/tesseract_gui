
#include <tesseract_gui/widgets/environment/commands/change_link_visibility_command_standard_item.h>
#include <tesseract_gui/widgets/common/standard_item_utils.h>
#include <tesseract_gui/common/standard_item_type.h>

namespace tesseract_gui
{

ChangeLinkVisibilityCommandStandardItem::ChangeLinkVisibilityCommandStandardItem(tesseract_environment::ChangeLinkVisibilityCommand::ConstPtr command)
  : QStandardItem(QIcon(":/tesseract_gui/png/merge.png"), "Change Link Visibility")
  , command(std::move(command))
{
  ctor();
}

ChangeLinkVisibilityCommandStandardItem::ChangeLinkVisibilityCommandStandardItem(const QString &text, tesseract_environment::ChangeLinkVisibilityCommand::ConstPtr command)
  : QStandardItem(QIcon(":/tesseract_gui/png/merge.png"), text)
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
  appendRow(createStandardItemString(QIcon(":/tesseract_gui/ignition/link.png"), command->getLinkName(), (command->getEnabled()) ? "True" : "False"));
}
}
