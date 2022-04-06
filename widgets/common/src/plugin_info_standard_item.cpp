
#include <tesseract_gui/widgets/common/plugin_info_standard_item.h>
#include <tesseract_gui/widgets/common/standard_item_utils.h>
#include <tesseract_gui/common/standard_item_type.h>

namespace tesseract_gui
{

PluginInfoStandardItem::PluginInfoStandardItem(tesseract_common::PluginInfo plugin_info)
  : QStandardItem(QIcon(":/tesseract_gui/png/plugin.png"), "Plugin Info")
  , plugin_info(std::move(plugin_info))
{
  ctor();
}

PluginInfoStandardItem::PluginInfoStandardItem(const QString &text, tesseract_common::PluginInfo plugin_info)
  : QStandardItem(QIcon(":/tesseract_gui/png/plugin.png"), text)
  , plugin_info(std::move(plugin_info))
{
  ctor();
}

PluginInfoStandardItem::PluginInfoStandardItem(const QIcon &icon, const QString &text, tesseract_common::PluginInfo plugin_info)
  : QStandardItem(icon, text)
  , plugin_info(std::move(plugin_info))
{
  ctor();
}

int PluginInfoStandardItem::type() const
{
  return static_cast<int>(StandardItemType::PLUGIN_INFO);
}

void PluginInfoStandardItem::ctor()
{
  appendRow(createStandardItemString("link_name1", plugin_info.class_name));
  appendRow(createStandardItemString("config", plugin_info.getConfigString()));
}
}

