
#include <tesseract_gui/widgets/environment/commands/add_contact_managers_plugin_info_command_standard_item.h>
#include <tesseract_gui/widgets/common/contact_managers_plugin_info_standard_item.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, COMMAND_ICON, (":/tesseract_gui/png/merge.png"));

namespace tesseract_gui
{

AddContactManagersPluginInfoCommandStandardItem::AddContactManagersPluginInfoCommandStandardItem(tesseract_environment::AddContactManagersPluginInfoCommand::ConstPtr command)
  : QStandardItem(*COMMAND_ICON(), "Add Contact Managers Plugin Info")
  , command(std::move(command))
{
  ctor();
}

AddContactManagersPluginInfoCommandStandardItem::AddContactManagersPluginInfoCommandStandardItem(const QString &text, tesseract_environment::AddContactManagersPluginInfoCommand::ConstPtr command)
  : QStandardItem(*COMMAND_ICON(), text)
  , command(std::move(command))
{
  ctor();
}

AddContactManagersPluginInfoCommandStandardItem::AddContactManagersPluginInfoCommandStandardItem(const QIcon &icon, const QString &text, tesseract_environment::AddContactManagersPluginInfoCommand::ConstPtr command)
  : QStandardItem(icon, text)
  , command(std::move(command))
{
  ctor();
}

int AddContactManagersPluginInfoCommandStandardItem::type() const
{
  return static_cast<int>(StandardItemType::ENV_COMMAND_ADD_CONTACT_MANAGERS_PLUGIN_INFO);
}

void AddContactManagersPluginInfoCommandStandardItem::ctor()
{
  appendRow(new ContactManagersPluginInfoStandardItem(command->getContactManagersPluginInfo()));
}
}
