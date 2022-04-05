
#include <tesseract_gui/widgets/environment/commands/move_joint_command_standard_item.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, COMMAND_ICON, (":/tesseract_gui/png/merge.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, TEXT_ICON, (":/tesseract_gui/png/text.png"));

namespace tesseract_gui
{

MoveJointCommandStandardItem::MoveJointCommandStandardItem(tesseract_environment::MoveJointCommand::ConstPtr command)
  : QStandardItem(*COMMAND_ICON(), "Move Joint")
  , command(std::move(command))
{
  ctor();
}

MoveJointCommandStandardItem::MoveJointCommandStandardItem(const QString &text, tesseract_environment::MoveJointCommand::ConstPtr command)
  : QStandardItem(*COMMAND_ICON(), text)
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
  auto* jn_name = new QStandardItem(*TEXT_ICON(), "joint name");
  auto* jn_value = new QStandardItem(command->getJointName().c_str());
  appendRow({jn_name, jn_value});

  auto* pl_name = new QStandardItem(*TEXT_ICON(), "parent link");
  auto* pl_value = new QStandardItem(command->getParentLink().c_str());
  appendRow({pl_name, pl_value});
}
}
