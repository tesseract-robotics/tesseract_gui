
#include <tesseract_widgets/common/contact_managers_plugin_info_standard_item.h>
#include <tesseract_widgets/common/plugin_info_container_standard_item.h>
#include <tesseract_widgets/common/standard_item_utils.h>
#include <tesseract_widgets/common/standard_item_type.h>

namespace tesseract_gui
{

ContactManagersPluginInfoStandardItem::ContactManagersPluginInfoStandardItem(tesseract_common::ContactManagersPluginInfo plugin_info)
  : QStandardItem(QIcon(":/tesseract_widgets/png/cube.png"), "Contact Managers Plugin Info")
  , plugin_info(std::move(plugin_info))
{
  ctor();
}

ContactManagersPluginInfoStandardItem::ContactManagersPluginInfoStandardItem(const QString &text, tesseract_common::ContactManagersPluginInfo plugin_info)
  : QStandardItem(QIcon(":/tesseract_widgets/png/cube.png"), text)
  , plugin_info(std::move(plugin_info))
{
  ctor();
}

ContactManagersPluginInfoStandardItem::ContactManagersPluginInfoStandardItem(const QIcon &icon, const QString &text, tesseract_common::ContactManagersPluginInfo plugin_info)
  : QStandardItem(icon, text)
  , plugin_info(std::move(plugin_info))
{
  ctor();
}

int ContactManagersPluginInfoStandardItem::type() const
{
  return static_cast<int>(StandardItemType::KINEMATICS_PLUGIN_INFO);
}

void ContactManagersPluginInfoStandardItem::ctor()
{
  auto* search_paths = new QStandardItem("search paths");
  std::size_t cnt {0};
  for (const auto& search_path : plugin_info.search_paths)
    search_paths->appendRow(createStandardItemString(QString("[%1]").arg(cnt++).toStdString(), search_path));

  appendRow({search_paths, new QStandardItem()});

  auto* search_libraries = new QStandardItem("search libraries");
  cnt = 0;
  for (const auto& search_library : plugin_info.search_libraries)
    search_libraries->appendRow(createStandardItemString(QString("[%1]").arg(cnt++).toStdString(), search_library));

  appendRow({search_libraries, new QStandardItem()});

  QIcon collision_icon(":/tesseract_widgets/ignition/collision.png");
  appendRow(new PluginInfoContainerStandardItem(collision_icon, "Discrete Plugins", plugin_info.discrete_plugin_infos));
  appendRow(new PluginInfoContainerStandardItem(collision_icon, "Continuous Plugins", plugin_info.continuous_plugin_infos));
}
}

