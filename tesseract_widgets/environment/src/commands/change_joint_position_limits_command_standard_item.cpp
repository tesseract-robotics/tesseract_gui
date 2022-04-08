
#include <tesseract_widgets/environment/commands/change_joint_position_limits_command_standard_item.h>
#include <tesseract_widgets/common/standard_item_utils.h>
#include <tesseract_widgets/common/standard_item_type.h>

namespace tesseract_gui
{

ChangeJointPositionLimitsCommandStandardItem::ChangeJointPositionLimitsCommandStandardItem(tesseract_environment::ChangeJointPositionLimitsCommand::ConstPtr command)
  : QStandardItem(QIcon(":/tesseract_widgets/png/merge.png"), "Change Joint Position Limits")
  , command(std::move(command))
{
  ctor();
}

ChangeJointPositionLimitsCommandStandardItem::ChangeJointPositionLimitsCommandStandardItem(const QString &text, tesseract_environment::ChangeJointPositionLimitsCommand::ConstPtr command)
  : QStandardItem(QIcon(":/tesseract_widgets/png/merge.png"), text)
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
    auto* item = new QStandardItem(QIcon(":/tesseract_widgets/ignition/joint.png"), joint.first.c_str());

    item->appendRow(createStandardItemFloat("lower", joint.second.first));
    item->appendRow(createStandardItemFloat("upper", joint.second.second));

    appendRow({item, new QStandardItem()});
  }
}
}
