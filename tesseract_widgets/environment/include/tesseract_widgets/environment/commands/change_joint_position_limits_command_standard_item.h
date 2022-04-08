#ifndef TESSERACT_WIDGETS_ENVIRONMENT_CHANGE_JOINT_POSITION_LIMITS_COMMAND_STANDARD_ITEM_H
#define TESSERACT_WIDGETS_ENVIRONMENT_CHANGE_JOINT_POSITION_LIMITS_COMMAND_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_environment/commands/change_joint_position_limits_command.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class ChangeJointPositionLimitsCommandStandardItem : public QStandardItem
{
public:
  explicit ChangeJointPositionLimitsCommandStandardItem(tesseract_environment::ChangeJointPositionLimitsCommand::ConstPtr command);
  explicit ChangeJointPositionLimitsCommandStandardItem(const QString &text, tesseract_environment::ChangeJointPositionLimitsCommand::ConstPtr command);
  explicit ChangeJointPositionLimitsCommandStandardItem(const QIcon &icon, const QString &text, tesseract_environment::ChangeJointPositionLimitsCommand::ConstPtr command);
  int type() const override;


  tesseract_environment::ChangeJointPositionLimitsCommand::ConstPtr command;

private:
  void ctor();
};
}

#endif // TESSERACT_WIDGETS_ENVIRONMENT_CHANGE_JOINT_POSITION_LIMITS_COMMAND_STANDARD_ITEM_H
