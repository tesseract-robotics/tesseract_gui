#ifndef TESSERACT_GUI_ENVIRONMENT_CHANGE_LINK_ORIGIN_COMMAND_STANDARD_ITEM_H
#define TESSERACT_GUI_ENVIRONMENT_CHANGE_LINK_ORIGIN_COMMAND_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_environment/commands/change_link_origin_command.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class ChangeLinkOriginCommandStandardItem : public QStandardItem
{
public:
  explicit ChangeLinkOriginCommandStandardItem(tesseract_environment::ChangeLinkOriginCommand::ConstPtr command);
  explicit ChangeLinkOriginCommandStandardItem(const QString &text, tesseract_environment::ChangeLinkOriginCommand::ConstPtr command);
  explicit ChangeLinkOriginCommandStandardItem(const QIcon &icon, const QString &text, tesseract_environment::ChangeLinkOriginCommand::ConstPtr command);
  int type() const override;


  tesseract_environment::ChangeLinkOriginCommand::ConstPtr command;

private:
  void ctor();
};
}

#endif // TESSERACT_GUI_ENVIRONMENT_CHANGE_LINK_ORIGIN_COMMAND_STANDARD_ITEM_H
