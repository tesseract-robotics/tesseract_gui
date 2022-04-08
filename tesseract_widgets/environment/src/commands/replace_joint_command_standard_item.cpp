
#include <tesseract_widgets/environment/commands/replace_joint_command_standard_item.h>
#include <tesseract_widgets/scene_graph/joint_standard_item.h>
#include <tesseract_widgets/common/standard_item_utils.h>
#include <tesseract_widgets/common/standard_item_type.h>

namespace tesseract_gui
{

ReplaceJointCommandStandardItem::ReplaceJointCommandStandardItem(tesseract_environment::ReplaceJointCommand::ConstPtr command)
  : QStandardItem(QIcon(":/tesseract_widgets/png/merge.png"), "Replace Joint")
  , command(std::move(command))
{
  ctor();
}

ReplaceJointCommandStandardItem::ReplaceJointCommandStandardItem(const QString &text, tesseract_environment::ReplaceJointCommand::ConstPtr command)
  : QStandardItem(QIcon(":/tesseract_widgets/png/merge.png"), text)
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
