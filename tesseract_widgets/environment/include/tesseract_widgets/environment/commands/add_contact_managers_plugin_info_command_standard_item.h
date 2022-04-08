#ifndef TESSERACT_WIDGETS_ENVIRONMENT_ADD_CONTACT_MANAGERS_PLUGIN_INFO_COMMAND_STANDARD_ITEM_H
#define TESSERACT_WIDGETS_ENVIRONMENT_ADD_CONTACT_MANAGERS_PLUGIN_INFO_COMMAND_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_environment/commands/add_contact_managers_plugin_info_command.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class AddContactManagersPluginInfoCommandStandardItem : public QStandardItem
{
public:
  explicit AddContactManagersPluginInfoCommandStandardItem(tesseract_environment::AddContactManagersPluginInfoCommand::ConstPtr command);
  explicit AddContactManagersPluginInfoCommandStandardItem(const QString &text, tesseract_environment::AddContactManagersPluginInfoCommand::ConstPtr command);
  explicit AddContactManagersPluginInfoCommandStandardItem(const QIcon &icon, const QString &text, tesseract_environment::AddContactManagersPluginInfoCommand::ConstPtr command);
  int type() const override;


  tesseract_environment::AddContactManagersPluginInfoCommand::ConstPtr command;

private:
  void ctor();
};
}

#endif // TESSERACT_WIDGETS_ENVIRONMENT_ADD_CONTACT_MANAGERS_PLUGIN_INFO_COMMAND_STANDARD_ITEM_H
