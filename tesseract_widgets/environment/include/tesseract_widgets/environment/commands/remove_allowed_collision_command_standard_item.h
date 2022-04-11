#ifndef TESSERACT_WIDGETS_ENVIRONMENT_REMOVE_ALLOWED_COLLISION_COMMAND_STANDARD_ITEM_H
#define TESSERACT_WIDGETS_ENVIRONMENT_REMOVE_ALLOWED_COLLISION_COMMAND_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_environment/commands/remove_allowed_collision_command.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class RemoveAllowedCollisionCommandStandardItem : public QStandardItem
{
public:
  explicit RemoveAllowedCollisionCommandStandardItem(tesseract_environment::RemoveAllowedCollisionCommand::ConstPtr command);
  explicit RemoveAllowedCollisionCommandStandardItem(const QString &text, tesseract_environment::RemoveAllowedCollisionCommand::ConstPtr command);
  explicit RemoveAllowedCollisionCommandStandardItem(const QIcon &icon, const QString &text, tesseract_environment::RemoveAllowedCollisionCommand::ConstPtr command);
  int type() const override;


  tesseract_environment::RemoveAllowedCollisionCommand::ConstPtr command;

private:
  void ctor();
};
}

#endif // TESSERACT_WIDGETS_ENVIRONMENT_REMOVE_ALLOWED_COLLISION_COMMAND_STANDARD_ITEM_H