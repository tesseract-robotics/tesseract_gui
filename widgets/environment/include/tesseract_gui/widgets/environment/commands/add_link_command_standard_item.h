#ifndef TESSERACT_GUI_ENVIRONMENT_ADD_LINK_COMMAND_STANDARD_ITEM_H
#define TESSERACT_GUI_ENVIRONMENT_ADD_LINK_COMMAND_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_environment/commands/add_link_command.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class AddLinkCommandStandardItem : public QStandardItem
{
public:
  explicit AddLinkCommandStandardItem(tesseract_environment::AddLinkCommand::ConstPtr command);
  explicit AddLinkCommandStandardItem(const QString &text, tesseract_environment::AddLinkCommand::ConstPtr command);
  explicit AddLinkCommandStandardItem(const QIcon &icon, const QString &text, tesseract_environment::AddLinkCommand::ConstPtr command);
  int type() const override;


  tesseract_environment::AddLinkCommand::ConstPtr command;

private:
  void ctor();
};
}

#endif // TESSERACT_GUI_ENVIRONMENT_ADD_LINK_COMMAND_STANDARD_ITEM_H
