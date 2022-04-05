#ifndef TESSERACT_GUI_ENVIRONMENT_CHANGE_JOINT_ACCELERATION_LIMITS_COMMAND_STANDARD_ITEM_H
#define TESSERACT_GUI_ENVIRONMENT_CHANGE_JOINT_ACCELERATION_LIMITS_COMMAND_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_environment/commands/change_joint_acceleration_limits_command.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class ChangeJointAccelerationLimitsCommandStandardItem : public QStandardItem
{
public:
  explicit ChangeJointAccelerationLimitsCommandStandardItem(tesseract_environment::ChangeJointAccelerationLimitsCommand::ConstPtr command);
  explicit ChangeJointAccelerationLimitsCommandStandardItem(const QString &text, tesseract_environment::ChangeJointAccelerationLimitsCommand::ConstPtr command);
  explicit ChangeJointAccelerationLimitsCommandStandardItem(const QIcon &icon, const QString &text, tesseract_environment::ChangeJointAccelerationLimitsCommand::ConstPtr command);
  int type() const override;


  tesseract_environment::ChangeJointAccelerationLimitsCommand::ConstPtr command;

private:
  void ctor();
};
}

#endif // TESSERACT_GUI_ENVIRONMENT_CHANGE_JOINT_ACCELERATION_LIMITS_COMMAND_STANDARD_ITEM_H
