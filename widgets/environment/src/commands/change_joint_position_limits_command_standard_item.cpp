
#include <tesseract_gui/widgets/environment/commands/change_joint_position_limits_command_standard_item.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, COMMAND_ICON, (":/tesseract_gui/png/merge.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, JOINT_ICON, (":/tesseract_gui/ignition/joint.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, NUMERIC_ICON, (":/tesseract_gui/png/numeric.png"));

namespace tesseract_gui
{

ChangeJointPositionLimitsCommandStandardItem::ChangeJointPositionLimitsCommandStandardItem(tesseract_environment::ChangeJointPositionLimitsCommand::ConstPtr command)
  : QStandardItem(*COMMAND_ICON(), "Change Joint Position Limits")
  , command(std::move(command))
{
  ctor();
}

ChangeJointPositionLimitsCommandStandardItem::ChangeJointPositionLimitsCommandStandardItem(const QString &text, tesseract_environment::ChangeJointPositionLimitsCommand::ConstPtr command)
  : QStandardItem(*COMMAND_ICON(), text)
  , command(std::move(command))
{
  ctor();
}

ChangeJointPositionLimitsCommandStandardItem::ChangeJointPositionLimitsCommandStandardItem(const QIcon &icon, const QString &text, tesseract_environment::ChangeJointPositionLimitsCommand::ConstPtr command)
  : QStandardItem(icon, text)
  , command(std::move(command))
{
  ctor();
}

int ChangeJointPositionLimitsCommandStandardItem::type() const
{
  return static_cast<int>(StandardItemType::ENV_COMMAND_CHANGE_JOINT_POSITION_LIMITS);
}

void ChangeJointPositionLimitsCommandStandardItem::ctor()
{
  for (const auto& joint : command->getLimits())
  {
    auto* item = new QStandardItem(*JOINT_ICON(), joint.first.c_str());
    auto* min_name = new QStandardItem(*NUMERIC_ICON(), "lower");
    auto* min_value = new QStandardItem(QString("%1").arg(joint.second.first));
    item->appendRow({min_name, min_value});

    auto* max_name = new QStandardItem(*NUMERIC_ICON(), "upper");
    auto* max_value = new QStandardItem(QString("%1").arg(joint.second.second));
    item->appendRow({max_name, max_value});

    appendRow({item, new QStandardItem()});
  }
}
}
