#ifndef TESSERACT_WIDGETS_ENVIRONMENT_MOVE_LINK_COMMAND_STANDARD_ITEM_H
#define TESSERACT_WIDGETS_ENVIRONMENT_MOVE_LINK_COMMAND_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_environment/commands/move_link_command.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class MoveLinkCommandStandardItem : public QStandardItem
{
public:
  explicit MoveLinkCommandStandardItem(tesseract_environment::MoveLinkCommand::ConstPtr command);
  explicit MoveLinkCommandStandardItem(const QString &text, tesseract_environment::MoveLinkCommand::ConstPtr command);
  explicit MoveLinkCommandStandardItem(const QIcon &icon, const QString &text, tesseract_environment::MoveLinkCommand::ConstPtr command);
  int type() const override;


  tesseract_environment::MoveLinkCommand::ConstPtr command;

private:
  void ctor();
};
}

#endif // TESSERACT_WIDGETS_ENVIRONMENT_MOVE_LINK_COMMAND_STANDARD_ITEM_H