
#include <tesseract_gui/widgets/environment/commands/set_active_discrete_contact_manager_command_standard_item.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, COMMAND_ICON, (":/tesseract_gui/png/merge.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, TEXT_ICON, (":/tesseract_gui/png/text.png"));

namespace tesseract_gui
{

SetActiveDiscreteContactManagerCommandStandardItem::SetActiveDiscreteContactManagerCommandStandardItem(tesseract_environment::SetActiveDiscreteContactManagerCommand::ConstPtr command)
  : QStandardItem(*COMMAND_ICON(), "Set Active Discrete Contact Manager")
  , command(std::move(command))
{
  ctor();
}

SetActiveDiscreteContactManagerCommandStandardItem::SetActiveDiscreteContactManagerCommandStandardItem(const QString &text, tesseract_environment::SetActiveDiscreteContactManagerCommand::ConstPtr command)
  : QStandardItem(*COMMAND_ICON(), text)
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
  auto* name = new QStandardItem(*TEXT_ICON(), "contact manager");
  auto* value = new QStandardItem(command->getName().c_str());
  appendRow({name, value});
}
}
