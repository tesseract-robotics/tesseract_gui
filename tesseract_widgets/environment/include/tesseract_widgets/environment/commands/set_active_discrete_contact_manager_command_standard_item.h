#ifndef TESSERACT_WIDGETS_ENVIRONMENT_SET_ACTIVE_DISCRETE_CONTACT_MANAGER_COMMAND_STANDARD_ITEM_H
#define TESSERACT_WIDGETS_ENVIRONMENT_SET_ACTIVE_DISCRETE_CONTACT_MANAGER_COMMAND_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_environment/commands/set_active_discrete_contact_manager_command.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class SetActiveDiscreteContactManagerCommandStandardItem : public QStandardItem
{
public:
  explicit SetActiveDiscreteContactManagerCommandStandardItem(tesseract_environment::SetActiveDiscreteContactManagerCommand::ConstPtr command);
  explicit SetActiveDiscreteContactManagerCommandStandardItem(const QString &text, tesseract_environment::SetActiveDiscreteContactManagerCommand::ConstPtr command);
  explicit SetActiveDiscreteContactManagerCommandStandardItem(const QIcon &icon, const QString &text, tesseract_environment::SetActiveDiscreteContactManagerCommand::ConstPtr command);
  int type() const override;


  tesseract_environment::SetActiveDiscreteContactManagerCommand::ConstPtr command;

private:
  void ctor();
};
}

#endif // TESSERACT_WIDGETS_ENVIRONMENT_SET_ACTIVE_DISCRETE_CONTACT_MANAGER_COMMAND_STANDARD_ITEM_H
