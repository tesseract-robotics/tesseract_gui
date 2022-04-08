
#include <tesseract_widgets/environment/commands/change_joint_acceleration_limits_command_standard_item.h>
#include <tesseract_widgets/common/standard_item_utils.h>
#include <tesseract_widgets/common/standard_item_type.h>

namespace tesseract_gui
{

ChangeJointAccelerationLimitsCommandStandardItem::ChangeJointAccelerationLimitsCommandStandardItem(tesseract_environment::ChangeJointAccelerationLimitsCommand::ConstPtr command)
  : QStandardItem(QIcon(":/tesseract_widgets/png/merge.png"), "Change Joint Acceleration Limits")
  , command(std::move(command))
{
  ctor();
}

ChangeJointAccelerationLimitsCommandStandardItem::ChangeJointAccelerationLimitsCommandStandardItem(const QString &text, tesseract_environment::ChangeJointAccelerationLimitsCommand::ConstPtr command)
  : QStandardItem(QIcon(":/tesseract_widgets/png/merge.png"), text)
  , command(std::move(command))
{
  ctor();
}

ChangeJointAccelerationLimitsCommandStandardItem::ChangeJointAccelerationLimitsCommandStandardItem(const QIcon &icon, const QString &text, tesseract_environment::ChangeJointAccelerationLimitsCommand::ConstPtr command)
  : QStandardItem(icon, text)
  , command(std::move(command))
{
  ctor();
}

int ChangeJointAccelerationLimitsCommandStandardItem::type() const
{
  return static_cast<int>(StandardItemType::ENV_COMMAND_CHANGE_JOINT_ACCELERATION_LIMITS);
}

void ChangeJointAccelerationLimitsCommandStandardItem::ctor()
{
  for (const auto& joint : command->getLimits())
    appendRow(createStandardItemFloat(QIcon(":/tesseract_widgets/ignition/joint.png"), joint.first, joint.second));

  sortChildren(0);
}
}
