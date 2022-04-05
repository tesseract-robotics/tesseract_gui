#ifndef TESSERACT_GUI_ENVIRONMENT_REMOVE_LINK_COMMAND_STANDARD_ITEM_H
#define TESSERACT_GUI_ENVIRONMENT_REMOVE_LINK_COMMAND_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_environment/commands/remove_link_command.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class RemoveLinkCommandStandardItem : public QStandardItem
{
public:
  explicit RemoveLinkCommandStandardItem(tesseract_environment::RemoveLinkCommand::ConstPtr command);
  explicit RemoveLinkCommandStandardItem(const QString &text, tesseract_environment::RemoveLinkCommand::ConstPtr command);
  explicit RemoveLinkCommandStandardItem(const QIcon &icon, const QString &text, tesseract_environment::RemoveLinkCommand::ConstPtr command);
  int type() const override;


  tesseract_environment::RemoveLinkCommand::ConstPtr command;

private:
  void ctor();
};
}

#endif // TESSERACT_GUI_ENVIRONMENT_REMOVE_LINK_COMMAND_STANDARD_ITEM_H
