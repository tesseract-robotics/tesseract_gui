
#include <tesseract_gui/widgets/environment/commands/move_joint_command_standard_item.h>
#include <tesseract_gui/widgets/common/standard_item_utils.h>
#include <tesseract_gui/common/standard_item_type.h>

namespace tesseract_gui
{

MoveJointCommandStandardItem::MoveJointCommandStandardItem(tesseract_environment::MoveJointCommand::ConstPtr command)
  : QStandardItem(QIcon(":/tesseract_gui/png/merge.png"), "Move Joint")
  , command(std::move(command))
{
  ctor();
}

MoveJointCommandStandardItem::MoveJointCommandStandardItem(const QString &text, tesseract_environment::MoveJointCommand::ConstPtr command)
  : QStandardItem(QIcon(":/tesseract_gui/png/merge.png"), text)
  , command(std::move(command))
{
  ctor();
}

MoveJointCommandStandardItem::MoveJointCommandStandardItem(const QIcon &icon, const QString &text, tesseract_environment::MoveJointCommand::ConstPtr command)
  : QStandardItem(icon, text)
  , command(std::move(command))
{
  ctor();
}

int MoveJointCommandStandardItem::type() const
{
  return static_cast<int>(StandardItemType::ENV_COMMAND_MOVE_JOINT);
}

void MoveJointCommandStandardItem::ctor()
{
  appendRow(createStandardItemString("joint name", command->getJointName()));
  appendRow(createStandardItemString("parent link", command->getParentLink()));
}
}
