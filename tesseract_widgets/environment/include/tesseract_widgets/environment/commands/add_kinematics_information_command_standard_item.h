#ifndef TESSERACT_WIDGETS_ENVIRONMENT_ADD_KINEMATICS_INFORMATION_COMMAND_STANDARD_ITEM_H
#define TESSERACT_WIDGETS_ENVIRONMENT_ADD_KINEMATICS_INFORMATION_COMMAND_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_environment/commands/add_kinematics_information_command.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class AddKinematicsInformationCommandStandardItem : public QStandardItem
{
public:
  explicit AddKinematicsInformationCommandStandardItem(tesseract_environment::AddKinematicsInformationCommand::ConstPtr command);
  explicit AddKinematicsInformationCommandStandardItem(const QString &text, tesseract_environment::AddKinematicsInformationCommand::ConstPtr command);
  explicit AddKinematicsInformationCommandStandardItem(const QIcon &icon, const QString &text, tesseract_environment::AddKinematicsInformationCommand::ConstPtr command);
  int type() const override;


  tesseract_environment::AddKinematicsInformationCommand::ConstPtr command;

private:
  void ctor();
};
}

#endif // TESSERACT_WIDGETS_ENVIRONMENT_ADD_KINEMATICS_INFORMATION_COMMAND_STANDARD_ITEM_H
