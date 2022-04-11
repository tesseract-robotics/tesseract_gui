#ifndef TESSERACT_WIDGETS_ENVIRONMENT_MOVE_JOINT_COMMAND_STANDARD_ITEM_H
#define TESSERACT_WIDGETS_ENVIRONMENT_MOVE_JOINT_COMMAND_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_environment/commands/move_joint_command.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class MoveJointCommandStandardItem : public QStandardItem
{
public:
  explicit MoveJointCommandStandardItem(tesseract_environment::MoveJointCommand::ConstPtr command);
  explicit MoveJointCommandStandardItem(const QString &text, tesseract_environment::MoveJointCommand::ConstPtr command);
  explicit MoveJointCommandStandardItem(const QIcon &icon, const QString &text, tesseract_environment::MoveJointCommand::ConstPtr command);
  int type() const override;


  tesseract_environment::MoveJointCommand::ConstPtr command;

private:
  void ctor();
};
}

#endif // TESSERACT_WIDGETS_ENVIRONMENT_MOVE_JOINT_COMMAND_STANDARD_ITEM_H