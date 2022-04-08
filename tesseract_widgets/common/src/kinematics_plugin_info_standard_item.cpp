
#include <tesseract_widgets/common/kinematics_plugin_info_standard_item.h>
#include <tesseract_widgets/common/plugin_info_container_standard_item.h>
#include <tesseract_widgets/common/standard_item_utils.h>
#include <tesseract_widgets/common/standard_item_type.h>

namespace tesseract_gui
{

KinematicsPluginInfoStandardItem::KinematicsPluginInfoStandardItem(tesseract_common::KinematicsPluginInfo plugin_info)
  : QStandardItem(QIcon(":/tesseract_widgets/png/cube.png"), "Kinematics Plugin Info")
  , plugin_info(std::move(plugin_info))
{
  ctor();
}

KinematicsPluginInfoStandardItem::KinematicsPluginInfoStandardItem(const QString &text, tesseract_common::KinematicsPluginInfo plugin_info)
  : QStandardItem(QIcon(":/tesseract_widgets/png/cube.png"), text)
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
    search_paths->appendRow(createStandardItemString(QString("[%1]").arg(cnt++).toStdString(), search_path));

  appendRow({search_paths, new QStandardItem()});

  auto* search_libraries = new QStandardItem("search libraries");
  cnt = 0;
  for (const auto& search_library : plugin_info.search_libraries)
    search_libraries->appendRow(createStandardItemString(QString("[%1]").arg(cnt++).toStdString(), search_library));

  appendRow({search_libraries, new QStandardItem()});

  QIcon robot_icon(":/tesseract_widgets/png/robotic-arm.png");
  auto* fwd_plugins = new QStandardItem("Fwd Kinematics Plugins");
  for (const auto& group : plugin_info.fwd_plugin_infos)
    fwd_plugins->appendRow(new PluginInfoContainerStandardItem(robot_icon, group.first.c_str(), group.second)); // NOLIN

  appendRow({fwd_plugins, new QStandardItem()});

  auto* inv_plugins = new QStandardItem("Inv Kinematics Plugins");
  for (const auto& group : plugin_info.inv_plugin_infos)
    inv_plugins->appendRow(new PluginInfoContainerStandardItem(robot_icon, group.first.c_str(), group.second)); // NOLINT

  appendRow({inv_plugins, new QStandardItem()});
}
}

