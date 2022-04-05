
#include <tesseract_gui/widgets/environment/commands/replace_joint_command_standard_item.h>
#include <tesseract_gui/widgets/scene_graph/joint_standard_item.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, COMMAND_ICON, (":/tesseract_gui/png/merge.png"));

namespace tesseract_gui
{

ReplaceJointCommandStandardItem::ReplaceJointCommandStandardItem(tesseract_environment::ReplaceJointCommand::ConstPtr command)
  : QStandardItem(*COMMAND_ICON(), "Replace Joint")
  , command(std::move(command))
{
  ctor();
}

ReplaceJointCommandStandardItem::ReplaceJointCommandStandardItem(const QString &text, tesseract_environment::ReplaceJointCommand::ConstPtr command)
  : QStandardItem(*COMMAND_ICON(), text)
  , command(std::move(command))
{
  ctor();
}

ReplaceJointCommandStandardItem::ReplaceJointCommandStandardItem(const QIcon &icon, const QString &text, tesseract_environment::ReplaceJointCommand::ConstPtr command)
  : QStandardItem(icon, text)
  , command(std::move(command))
{
  ctor();
}

int ReplaceJointCommandStandardItem::type() const
{
  return static_cast<int>(StandardItemType::ENV_COMMAND_REPLACE_JOINT);
}

void ReplaceJointCommandStandardItem::ctor()
{
  auto* item = new JointStandardItem(std::make_shared<tesseract_scene_graph::Joint>(command->getJoint()->clone()));
  appendRow({item, new QStandardItem()});
}
}
