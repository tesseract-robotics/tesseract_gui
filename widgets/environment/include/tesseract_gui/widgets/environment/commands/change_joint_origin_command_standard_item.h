#ifndef TESSERACT_GUI_ENVIRONMENT_CHANGE_JOINT_ORIGIN_COMMAND_STANDARD_ITEM_H
#define TESSERACT_GUI_ENVIRONMENT_CHANGE_JOINT_ORIGIN_COMMAND_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_environment/commands/change_joint_origin_command.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class ChangeJointOriginCommandStandardItem : public QStandardItem
{
public:
  explicit ChangeJointOriginCommandStandardItem(tesseract_environment::ChangeJointOriginCommand::ConstPtr command);
  explicit ChangeJointOriginCommandStandardItem(const QString &text, tesseract_environment::ChangeJointOriginCommand::ConstPtr command);
  explicit ChangeJointOriginCommandStandardItem(const QIcon &icon, const QString &text, tesseract_environment::ChangeJointOriginCommand::ConstPtr command);
  int type() const override;


  tesseract_environment::ChangeJointOriginCommand::ConstPtr command;

private:
  void ctor();
};
}

#endif // TESSERACT_GUI_ENVIRONMENT_CHANGE_JOINT_ORIGIN_COMMAND_STANDARD_ITEM_H
