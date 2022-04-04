#ifndef TESSERACT_GUI_ENVIRONMENT_REMOVE_ALLOWED_COLLISION_LINK_COMMAND_STANDARD_ITEM_H
#define TESSERACT_GUI_ENVIRONMENT_REMOVE_ALLOWED_COLLISION_LINK_COMMAND_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_environment/commands/remove_allowed_collision_link_command.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class RemoveAllowedCollisionLinkCommandStandardItem : public QStandardItem
{
public:
  RemoveAllowedCollisionLinkCommandStandardItem(tesseract_environment::RemoveAllowedCollisionLinkCommand::ConstPtr command);
  explicit RemoveAllowedCollisionLinkCommandStandardItem(const QString &text, tesseract_environment::RemoveAllowedCollisionLinkCommand::ConstPtr command);
  RemoveAllowedCollisionLinkCommandStandardItem(const QIcon &icon, const QString &text, tesseract_environment::RemoveAllowedCollisionLinkCommand::ConstPtr command);
  int type() const override;

  tesseract_environment::RemoveAllowedCollisionLinkCommand::ConstPtr command;

private:
  void ctor();
};
}

#endif // TESSERACT_GUI_ENVIRONMENT_REMOVE_ALLOWED_COLLISION_LINK_COMMAND_STANDARD_ITEM_H
