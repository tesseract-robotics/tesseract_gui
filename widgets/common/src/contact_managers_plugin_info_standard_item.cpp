
#include <tesseract_gui/widgets/common/contact_managers_plugin_info_standard_item.h>
#include <tesseract_gui/widgets/common/plugin_info_container_standard_item.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, TEXT_ICON, (":/tesseract_gui/png/text.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, CUBE_ICON, (":/tesseract_gui/png/cube.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, COLLISION_ICON, (":/tesseract_gui/ignition/collision.png"));

namespace tesseract_gui
{

ContactManagersPluginInfoStandardItem::ContactManagersPluginInfoStandardItem(tesseract_common::ContactManagersPluginInfo plugin_info)
  : QStandardItem(*CUBE_ICON(), "Contact Managers Plugin Info")
  , plugin_info(std::move(plugin_info))
{
  ctor();
}

ContactManagersPluginInfoStandardItem::ContactManagersPluginInfoStandardItem(const QString &text, tesseract_common::ContactManagersPluginInfo plugin_info)
  : QStandardItem(*CUBE_ICON(), text)
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
  {
    auto* name = new QStandardItem(*TEXT_ICON, QString("[%1]").arg(cnt));
    auto* value = new QStandardItem(search_path.c_str());
    search_paths->appendRow({name, value});
    ++cnt;
  }
  appendRow({search_paths, new QStandardItem()});

  auto* search_libraries = new QStandardItem("search libraries");
  cnt = 0;
  for (const auto& search_library : plugin_info.search_libraries)
  {
    auto* name = new QStandardItem(*TEXT_ICON, QString("[%1]").arg(cnt));
    auto* value = new QStandardItem(search_library.c_str());
    search_libraries->appendRow({name, value});
    ++cnt;
  }
  appendRow({search_libraries, new QStandardItem()});

  appendRow(new PluginInfoContainerStandardItem(*COLLISION_ICON, "Discrete Plugins", plugin_info.discrete_plugin_infos));
  appendRow(new PluginInfoContainerStandardItem(*COLLISION_ICON, "Continuous Plugins", plugin_info.continuous_plugin_infos));
}
}

