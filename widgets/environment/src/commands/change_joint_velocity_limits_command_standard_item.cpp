
#include <tesseract_gui/widgets/environment/commands/change_joint_velocity_limits_command_standard_item.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, COMMAND_ICON, (":/tesseract_gui/png/merge.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, JOINT_ICON, (":/tesseract_gui/ignition/joint.png"));

namespace tesseract_gui
{

ChangeJointVelocityLimitsCommandStandardItem::ChangeJointVelocityLimitsCommandStandardItem(tesseract_environment::ChangeJointVelocityLimitsCommand::ConstPtr command)
  : QStandardItem(*COMMAND_ICON(), "Change Joint Velocity Limits")
  , command(std::move(command))
{
  ctor();
}

ChangeJointVelocityLimitsCommandStandardItem::ChangeJointVelocityLimitsCommandStandardItem(const QString &text, tesseract_environment::ChangeJointVelocityLimitsCommand::ConstPtr command)
  : QStandardItem(*COMMAND_ICON(), text)
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
  {
    auto* x_name = new QStandardItem(*JOINT_ICON(), joint.first.c_str());
    auto* x_value = new QStandardItem(QString("%1").arg(joint.second));
    appendRow({x_name, x_value});
  }
  sortChildren(0);
}
}
