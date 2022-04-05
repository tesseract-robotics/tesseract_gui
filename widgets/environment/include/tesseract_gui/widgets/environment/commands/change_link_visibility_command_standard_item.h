#ifndef TESSERACT_GUI_ENVIRONMENT_CHANGE_LINK_VISIBILITY_COMMAND_STANDARD_ITEM_H
#define TESSERACT_GUI_ENVIRONMENT_CHANGE_LINK_VISIBILITY_COMMAND_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_environment/commands/change_link_visibility_command.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class ChangeLinkVisibilityCommandStandardItem : public QStandardItem
{
public:
  explicit ChangeLinkVisibilityCommandStandardItem(tesseract_environment::ChangeLinkVisibilityCommand::ConstPtr command);
  explicit ChangeLinkVisibilityCommandStandardItem(const QString &text, tesseract_environment::ChangeLinkVisibilityCommand::ConstPtr command);
  explicit ChangeLinkVisibilityCommandStandardItem(const QIcon &icon, const QString &text, tesseract_environment::ChangeLinkVisibilityCommand::ConstPtr command);
  int type() const override;


  tesseract_environment::ChangeLinkVisibilityCommand::ConstPtr command;

private:
  void ctor();
};
}

#endif // TESSERACT_GUI_ENVIRONMENT_CHANGE_LINK_VISIBILITY_COMMAND_STANDARD_ITEM_H
