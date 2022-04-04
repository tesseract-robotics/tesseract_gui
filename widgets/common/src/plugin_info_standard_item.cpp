
#include <tesseract_gui/widgets/common/plugin_info_standard_item.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, TEXT_ICON, (":/tesseract_gui/png/text.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, CUBE_ICON, (":/tesseract_gui/png/cube.png"));

namespace tesseract_gui
{

PluginInfoStandardItem::PluginInfoStandardItem(tesseract_common::PluginInfo plugin_info)
  : QStandardItem(*CUBE_ICON(), "Plugin Info")
  , plugin_info(std::move(plugin_info))
{
  ctor();
}

PluginInfoStandardItem::PluginInfoStandardItem(const QString &text, tesseract_common::PluginInfo plugin_info)
  : QStandardItem(*CUBE_ICON(), text)
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
  auto* class_name = new QStandardItem(*TEXT_ICON, "link_name1");
  auto* class_value = new QStandardItem(QString("%1").arg(plugin_info.class_name.c_str()));
  appendRow({class_name, class_value});

  auto* config_name = new QStandardItem(*TEXT_ICON, "config");
  auto* config_value = new QStandardItem(QString("%1").arg(plugin_info.getConfigString().c_str()));
  appendRow({config_name, config_value});
}
}

