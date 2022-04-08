#ifndef TESSERACT_WIDGETS_ENVIRONMENT_ADD_ALLOWED_COLLISION_COMMAND_STANDARD_ITEM_H
#define TESSERACT_WIDGETS_ENVIRONMENT_ADD_ALLOWED_COLLISION_COMMAND_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_environment/commands/add_allowed_collision_command.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class AddAllowedCollisionCommandStandardItem : public QStandardItem
{
public:
  explicit AddAllowedCollisionCommandStandardItem(tesseract_environment::AddAllowedCollisionCommand::ConstPtr command);
  explicit AddAllowedCollisionCommandStandardItem(const QString &text, tesseract_environment::AddAllowedCollisionCommand::ConstPtr command);
  explicit AddAllowedCollisionCommandStandardItem(const QIcon &icon, const QString &text, tesseract_environment::AddAllowedCollisionCommand::ConstPtr command);
  int type() const override;

  tesseract_environment::AddAllowedCollisionCommand::ConstPtr command;

private:
  void ctor();
};
}

#endif // TESSERACT_WIDGETS_ENVIRONMENT_ADD_ALLOWED_COLLISION_COMMAND_STANDARD_ITEM_H
