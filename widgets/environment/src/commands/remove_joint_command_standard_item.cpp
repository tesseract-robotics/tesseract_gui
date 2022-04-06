
#include <tesseract_gui/widgets/environment/commands/remove_joint_command_standard_item.h>
#include <tesseract_gui/widgets/common/standard_item_utils.h>
#include <tesseract_gui/common/standard_item_type.h>

namespace tesseract_gui
{

RemoveJointCommandStandardItem::RemoveJointCommandStandardItem(tesseract_environment::RemoveJointCommand::ConstPtr command)
  : QStandardItem(QIcon(":/tesseract_gui/png/merge.png"), "Remove Joint")
  , command(std::move(command))
{
  ctor();
}

RemoveJointCommandStandardItem::RemoveJointCommandStandardItem(const QString &text, tesseract_environment::RemoveJointCommand::ConstPtr command)
  : QStandardItem(QIcon(":/tesseract_gui/png/merge.png"), text)
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
  appendRow(createStandardItemString("joint name", command->getJointName()));
}
}
