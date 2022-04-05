
#include <tesseract_gui/widgets/environment/commands/remove_joint_command_standard_item.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, COMMAND_ICON, (":/tesseract_gui/png/merge.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, TEXT_ICON, (":/tesseract_gui/png/text.png"));

namespace tesseract_gui
{

RemoveJointCommandStandardItem::RemoveJointCommandStandardItem(tesseract_environment::RemoveJointCommand::ConstPtr command)
  : QStandardItem(*COMMAND_ICON(), "Remove Joint")
  , command(std::move(command))
{
  ctor();
}

RemoveJointCommandStandardItem::RemoveJointCommandStandardItem(const QString &text, tesseract_environment::RemoveJointCommand::ConstPtr command)
  : QStandardItem(*COMMAND_ICON(), text)
  , command(std::move(command))
{
  ctor();
}

RemoveJointCommandStandardItem::RemoveJointCommandStandardItem(const QIcon &icon, const QString &text, tesseract_environment::RemoveJointCommand::ConstPtr command)
  : QStandardItem(icon, text)
  , command(std::move(command))
{
  ctor();
}

int RemoveJointCommandStandardItem::type() const
{
  return static_cast<int>(StandardItemType::ENV_COMMAND_REMOVE_JOINT);
}

void RemoveJointCommandStandardItem::ctor()
{
  auto* name = new QStandardItem(*TEXT_ICON(), "joint name");
  auto* value = new QStandardItem(command->getJointName().c_str());
  appendRow({name, value});
}
}
