
#include <tesseract_widgets/environment/commands/add_contact_managers_plugin_info_command_standard_item.h>
#include <tesseract_widgets/common/contact_managers_plugin_info_standard_item.h>
#include <tesseract_widgets/common/standard_item_type.h>

namespace tesseract_gui
{

AddContactManagersPluginInfoCommandStandardItem::AddContactManagersPluginInfoCommandStandardItem(tesseract_environment::AddContactManagersPluginInfoCommand::ConstPtr command)
  : QStandardItem(QIcon(":/tesseract_widgets/png/merge.png"), "Add Contact Managers Plugin Info")
  , command(std::move(command))
{
  ctor();
}

AddContactManagersPluginInfoCommandStandardItem::AddContactManagersPluginInfoCommandStandardItem(const QString &text, tesseract_environment::AddContactManagersPluginInfoCommand::ConstPtr command)
  : QStandardItem(QIcon(":/tesseract_widgets/png/merge.png"), text)
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
