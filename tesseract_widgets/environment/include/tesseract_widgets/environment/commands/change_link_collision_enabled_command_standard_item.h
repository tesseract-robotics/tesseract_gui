#ifndef TESSERACT_WIDGETS_ENVIRONMENT_CHANGE_LINK_COLLISION_ENABLED_COMMAND_STANDARD_ITEM_H
#define TESSERACT_WIDGETS_ENVIRONMENT_CHANGE_LINK_COLLISION_ENABLED_COMMAND_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_environment/commands/change_link_collision_enabled_command.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class ChangeLinkCollisionEnabledCommandStandardItem : public QStandardItem
{
public:
  explicit ChangeLinkCollisionEnabledCommandStandardItem(tesseract_environment::ChangeLinkCollisionEnabledCommand::ConstPtr command);
  explicit ChangeLinkCollisionEnabledCommandStandardItem(const QString &text, tesseract_environment::ChangeLinkCollisionEnabledCommand::ConstPtr command);
  explicit ChangeLinkCollisionEnabledCommandStandardItem(const QIcon &icon, const QString &text, tesseract_environment::ChangeLinkCollisionEnabledCommand::ConstPtr command);
  int type() const override;


  tesseract_environment::ChangeLinkCollisionEnabledCommand::ConstPtr command;

private:
  void ctor();
};
}

#endif // TESSERACT_WIDGETS_ENVIRONMENT_CHANGE_LINK_COLLISION_ENABLED_COMMAND_STANDARD_ITEM_H
