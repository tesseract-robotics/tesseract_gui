
#include <tesseract_gui/widgets/environment/commands/change_joint_origin_command_standard_item.h>
#include <tesseract_gui/widgets/common/transform_standard_item.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, COMMAND_ICON, (":/tesseract_gui/png/merge.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, JOINT_ICON, (":/tesseract_gui/ignition/joint.png"));

namespace tesseract_gui
{

ChangeJointOriginCommandStandardItem::ChangeJointOriginCommandStandardItem(tesseract_environment::ChangeJointOriginCommand::ConstPtr command)
  : QStandardItem(*COMMAND_ICON(), "Change Joint Origin")
  , command(std::move(command))
{
  ctor();
}

ChangeJointOriginCommandStandardItem::ChangeJointOriginCommandStandardItem(const QString &text, tesseract_environment::ChangeJointOriginCommand::ConstPtr command)
  : QStandardItem(*COMMAND_ICON(), text)
  , command(std::move(command))
{
  ctor();
}

ChangeJointOriginCommandStandardItem::ChangeJointOriginCommandStandardItem(const QIcon &icon, const QString &text, tesseract_environment::ChangeJointOriginCommand::ConstPtr command)
  : QStandardItem(icon, text)
  , command(std::move(command))
{
  ctor();
}

int ChangeJointOriginCommandStandardItem::type() const
{
  return static_cast<int>(StandardItemType::ENV_COMMAND_CHANGE_JOINT_ORIGIN);
}

void ChangeJointOriginCommandStandardItem::ctor()
{
  auto* item = new TransformStandardItem(*JOINT_ICON(), command->getJointName().c_str(), command->getOrigin());
  appendRow({item, new QStandardItem()});
}
}
