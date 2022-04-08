
#include <tesseract_widgets/environment/commands/set_active_continuous_contact_manager_command_standard_item.h>
#include <tesseract_widgets/common/standard_item_utils.h>
#include <tesseract_widgets/common/standard_item_type.h>

namespace tesseract_gui
{

SetActiveContinuousContactManagerCommandStandardItem::SetActiveContinuousContactManagerCommandStandardItem(tesseract_environment::SetActiveContinuousContactManagerCommand::ConstPtr command)
  : QStandardItem(QIcon(":/tesseract_widgets/png/merge.png"), "Set Active Continuous Contact Manager")
  , command(std::move(command))
{
  ctor();
}

SetActiveContinuousContactManagerCommandStandardItem::SetActiveContinuousContactManagerCommandStandardItem(const QString &text, tesseract_environment::SetActiveContinuousContactManagerCommand::ConstPtr command)
  : QStandardItem(QIcon(":/tesseract_widgets/png/merge.png"), text)
  , command(std::move(command))
{
  ctor();
}

SetActiveContinuousContactManagerCommandStandardItem::SetActiveContinuousContactManagerCommandStandardItem(const QIcon &icon, const QString &text, tesseract_environment::SetActiveContinuousContactManagerCommand::ConstPtr command)
  : QStandardItem(icon, text)
  , command(std::move(command))
{
  ctor();
}

int SetActiveContinuousContactManagerCommandStandardItem::type() const
{
  return static_cast<int>(StandardItemType::ENV_COMMAND_SET_ACTIVE_CONTINUOUS_CONTACT_MANAGER);
}

void SetActiveContinuousContactManagerCommandStandardItem::ctor()
{
  appendRow(createStandardItemString("contact manager", command->getName()));
}
}
