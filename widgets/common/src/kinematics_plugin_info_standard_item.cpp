
#include <tesseract_gui/widgets/common/kinematics_plugin_info_standard_item.h>
#include <tesseract_gui/widgets/common/plugin_info_container_standard_item.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, TEXT_ICON, (":/tesseract_gui/png/text.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, CUBE_ICON, (":/tesseract_gui/png/cube.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, ROBOT_ICON, (":/tesseract_gui/png/robotic-arm.png"));

namespace tesseract_gui
{

KinematicsPluginInfoStandardItem::KinematicsPluginInfoStandardItem(tesseract_common::KinematicsPluginInfo plugin_info)
  : QStandardItem(*CUBE_ICON(), "Kinematics Plugin Info")
  , plugin_info(std::move(plugin_info))
{
  ctor();
}

KinematicsPluginInfoStandardItem::KinematicsPluginInfoStandardItem(const QString &text, tesseract_common::KinematicsPluginInfo plugin_info)
  : QStandardItem(*CUBE_ICON(), text)
  , plugin_info(std::move(plugin_info))
{
  ctor();
}

KinematicsPluginInfoStandardItem::KinematicsPluginInfoStandardItem(const QIcon &icon, const QString &text, tesseract_common::KinematicsPluginInfo plugin_info)
  : QStandardItem(icon, text)
  , plugin_info(std::move(plugin_info))
{
  ctor();
}

int KinematicsPluginInfoStandardItem::type() const
{
  return static_cast<int>(StandardItemType::KINEMATICS_PLUGIN_INFO);
}

void KinematicsPluginInfoStandardItem::ctor()
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

  auto* fwd_plugins = new QStandardItem("Fwd Kinematics Plugins");
  for (const auto& group : plugin_info.fwd_plugin_infos)
    fwd_plugins->appendRow(new PluginInfoContainerStandardItem(*ROBOT_ICON, group.first.c_str(), group.second));

  appendRow({fwd_plugins, new QStandardItem()});

  auto* inv_plugins = new QStandardItem("Inv Kinematics Plugins");
  for (const auto& group : plugin_info.inv_plugin_infos)
    inv_plugins->appendRow(new PluginInfoContainerStandardItem(*ROBOT_ICON, group.first.c_str(), group.second));

  appendRow({inv_plugins, new QStandardItem()});
}
}

