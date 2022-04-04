#ifndef TESSERACT_GUI_ENVIRONMENT_CHANGE_COLLISION_MARGINS_COMMAND_STANDARD_ITEM_H
#define TESSERACT_GUI_ENVIRONMENT_CHANGE_COLLISION_MARGINS_COMMAND_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_environment/commands/change_collision_margins_command.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class ChangeCollisionMarginsCommandStandardItem : public QStandardItem
{
public:
  ChangeCollisionMarginsCommandStandardItem(tesseract_environment::ChangeCollisionMarginsCommand::ConstPtr command);
  explicit ChangeCollisionMarginsCommandStandardItem(const QString &text, tesseract_environment::ChangeCollisionMarginsCommand::ConstPtr command);
  ChangeCollisionMarginsCommandStandardItem(const QIcon &icon, const QString &text, tesseract_environment::ChangeCollisionMarginsCommand::ConstPtr command);
  int type() const override;

  tesseract_environment::ChangeCollisionMarginsCommand::ConstPtr command;

private:
  void ctor();
};
}

#endif // TESSERACT_GUI_ENVIRONMENT_CHANGE_COLLISION_MARGINS_COMMAND_STANDARD_ITEM_H
