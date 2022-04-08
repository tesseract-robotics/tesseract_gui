#ifndef TESSERACT_WIDGETS_ENVIRONMENT_REMOVE_JOINT_COMMAND_STANDARD_ITEM_H
#define TESSERACT_WIDGETS_ENVIRONMENT_REMOVE_JOINT_COMMAND_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_environment/commands/remove_joint_command.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class RemoveJointCommandStandardItem : public QStandardItem
{
public:
  explicit RemoveJointCommandStandardItem(tesseract_environment::RemoveJointCommand::ConstPtr command);
  explicit RemoveJointCommandStandardItem(const QString &text, tesseract_environment::RemoveJointCommand::ConstPtr command);
  explicit RemoveJointCommandStandardItem(const QIcon &icon, const QString &text, tesseract_environment::RemoveJointCommand::ConstPtr command);
  int type() const override;


  tesseract_environment::RemoveJointCommand::ConstPtr command;

private:
  void ctor();
};
}

#endif // TESSERACT_WIDGETS_ENVIRONMENT_REMOVE_JOINT_COMMAND_STANDARD_ITEM_H
