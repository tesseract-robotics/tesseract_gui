
#include <tesseract_gui/widgets/environment/commands/set_active_discrete_contact_manager_command_standard_item.h>
#include <tesseract_gui/widgets/common/standard_item_utils.h>
#include <tesseract_gui/common/standard_item_type.h>

namespace tesseract_gui
{

SetActiveDiscreteContactManagerCommandStandardItem::SetActiveDiscreteContactManagerCommandStandardItem(tesseract_environment::SetActiveDiscreteContactManagerCommand::ConstPtr command)
  : QStandardItem(QIcon(":/tesseract_gui/png/merge.png"), "Set Active Discrete Contact Manager")
  , command(std::move(command))
{
  ctor();
}

SetActiveDiscreteContactManagerCommandStandardItem::SetActiveDiscreteContactManagerCommandStandardItem(const QString &text, tesseract_environment::SetActiveDiscreteContactManagerCommand::ConstPtr command)
  : QStandardItem(QIcon(":/tesseract_gui/png/merge.png"), text)
  , command(std::move(command))
{
  ctor();
}

SetActiveDiscreteContactManagerCommandStandardItem::SetActiveDiscreteContactManagerCommandStandardItem(const QIcon &icon, const QString &text, tesseract_environment::SetActiveDiscreteContactManagerCommand::ConstPtr command)
  : QStandardItem(icon, text)
  , command(std::move(command))
{
  ctor();
}

int SetActiveDiscreteContactManagerCommandStandardItem::type() const
{
  return static_cast<int>(StandardItemType::ENV_COMMAND_SET_ACTIVE_DISCRETE_CONTACT_MANAGER);
}

void SetActiveDiscreteContactManagerCommandStandardItem::ctor()
{
  appendRow(createStandardItemString("contact manager", command->getName()));
}
}
