
#include <tesseract_gui/widgets/environment/commands/change_joint_velocity_limits_command_standard_item.h>
#include <tesseract_gui/widgets/common/standard_item_utils.h>
#include <tesseract_gui/common/standard_item_type.h>

namespace tesseract_gui
{

ChangeJointVelocityLimitsCommandStandardItem::ChangeJointVelocityLimitsCommandStandardItem(tesseract_environment::ChangeJointVelocityLimitsCommand::ConstPtr command)
  : QStandardItem(QIcon(":/tesseract_gui/png/merge.png"), "Change Joint Velocity Limits")
  , command(std::move(command))
{
  ctor();
}

ChangeJointVelocityLimitsCommandStandardItem::ChangeJointVelocityLimitsCommandStandardItem(const QString &text, tesseract_environment::ChangeJointVelocityLimitsCommand::ConstPtr command)
  : QStandardItem(QIcon(":/tesseract_gui/png/merge.png"), text)
  , command(std::move(command))
{
  ctor();
}

ChangeJointVelocityLimitsCommandStandardItem::ChangeJointVelocityLimitsCommandStandardItem(const QIcon &icon, const QString &text, tesseract_environment::ChangeJointVelocityLimitsCommand::ConstPtr command)
  : QStandardItem(icon, text)
  , command(std::move(command))
{
  ctor();
}

int ChangeJointVelocityLimitsCommandStandardItem::type() const
{
  return static_cast<int>(StandardItemType::ENV_COMMAND_CHANGE_JOINT_VELOCITY_LIMITS);
}

void ChangeJointVelocityLimitsCommandStandardItem::ctor()
{
  for (const auto& joint : command->getLimits())
    appendRow(createStandardItemFloat(QIcon(":/tesseract_gui/ignition/joint.png"), joint.first, joint.second));

  sortChildren(0);
}
}
