#ifndef TESSERACT_GUI_ENVIRONMENT_REPLACE_JOINT_COMMAND_STANDARD_ITEM_H
#define TESSERACT_GUI_ENVIRONMENT_REPLACE_JOINT_COMMAND_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_environment/commands/replace_joint_command.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class ReplaceJointCommandStandardItem : public QStandardItem
{
public:
  ReplaceJointCommandStandardItem(tesseract_environment::ReplaceJointCommand::ConstPtr command);
  explicit ReplaceJointCommandStandardItem(const QString &text, tesseract_environment::ReplaceJointCommand::ConstPtr command);
  ReplaceJointCommandStandardItem(const QIcon &icon, const QString &text, tesseract_environment::ReplaceJointCommand::ConstPtr command);
  int type() const override;

  tesseract_environment::ReplaceJointCommand::ConstPtr command;

private:
  void ctor();
};
}

#endif // TESSERACT_GUI_ENVIRONMENT_REPLACE_JOINT_COMMAND_STANDARD_ITEM_H
