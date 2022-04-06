
#include <tesseract_gui/widgets/common/plugin_info_container_standard_item.h>
#include <tesseract_gui/widgets/common/plugin_info_standard_item.h>
#include <tesseract_gui/widgets/common/standard_item_utils.h>
#include <tesseract_gui/common/standard_item_type.h>

namespace tesseract_gui
{

PluginInfoContainerStandardItem::PluginInfoContainerStandardItem(tesseract_common::PluginInfoContainer plugin_info_container)
  : QStandardItem(QIcon(":/tesseract_gui/png/cube.png"), "Plugin Info Container")
  , plugin_info_container(std::move(plugin_info_container))
{
  ctor();
}

PluginInfoContainerStandardItem::PluginInfoContainerStandardItem(const QString &text, tesseract_common::PluginInfoContainer plugin_info_container)
  : QStandardItem(QIcon(":/tesseract_gui/png/cube.png"), text)
  , plugin_info_container(std::move(plugin_info_container))
{
  ctor();
}

PluginInfoContainerStandardItem::PluginInfoContainerStandardItem(const QIcon &icon, const QString &text, tesseract_common::PluginInfoContainer plugin_info_container)
  : QStandardItem(icon, text)
  , plugin_info_container(std::move(plugin_info_container))
{
  ctor();
}

int PluginInfoContainerStandardItem::type() const
{
  return static_cast<int>(StandardItemType::PLUGIN_INFO_CONTAINER);
}

void PluginInfoContainerStandardItem::ctor()
{
  appendRow(createStandardItemString("default", plugin_info_container.default_plugin));

  auto* plugins = new QStandardItem(QIcon(":/tesseract_gui/png/cube.png"), "plugins");
  for (const auto& plugin : plugin_info_container.plugins)
    plugins->appendRow(new PluginInfoStandardItem(plugin.first.c_str(), plugin.second));

  appendRow(plugins);
}
}

