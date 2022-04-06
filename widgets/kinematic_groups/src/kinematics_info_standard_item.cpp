
#include <tesseract_gui/widgets/kinematic_groups/kinematics_info_standard_item.h>
#include <tesseract_gui/widgets/kinematic_groups/chain_group_standard_item.h>
#include <tesseract_gui/widgets/kinematic_groups/joint_group_standard_item.h>
#include <tesseract_gui/widgets/kinematic_groups/link_group_standard_item.h>
#include <tesseract_gui/widgets/kinematic_groups/group_joint_states_standard_item.h>
#include <tesseract_gui/widgets/kinematic_groups/group_tcps_standard_item.h>
#include <tesseract_gui/widgets/common/kinematics_plugin_info_standard_item.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, CUBE_ICON, (":/tesseract_gui/png/cube.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, TEXT_ICON, (":/tesseract_gui/png/text.png"));

namespace tesseract_gui
{

KinematicsInfoStandardItem::KinematicsInfoStandardItem(tesseract_srdf::KinematicsInformation kinematics_info)
  : QStandardItem(*CUBE_ICON(), "Kinematics Info")
  , kinematics_info(std::move(kinematics_info))
{
  ctor();
}

KinematicsInfoStandardItem::KinematicsInfoStandardItem(const QString &text, tesseract_srdf::KinematicsInformation kinematics_info)
  : QStandardItem(*CUBE_ICON(), text)
  , kinematics_info(std::move(kinematics_info))
{
  ctor();
}

KinematicsInfoStandardItem::KinematicsInfoStandardItem(const QIcon &icon, const QString &text, tesseract_srdf::KinematicsInformation kinematics_info)
  : QStandardItem(icon, text)
  , kinematics_info(std::move(kinematics_info))
{
  ctor();
}

int KinematicsInfoStandardItem::type() const
{
  return static_cast<int>(StandardItemType::KINEMATICS_INFO);
}

void KinematicsInfoStandardItem::ctor()
{
  auto* group_names = new QStandardItem("Group Names");
  std::size_t cnt {0};
  for (const auto& group_name : kinematics_info.group_names)
  {
    auto* name = new QStandardItem(*TEXT_ICON, QString("[%1]").arg(cnt));
    auto* value = new QStandardItem(group_name.c_str());
    group_names->appendRow({name, value});
    ++cnt;
  }
  appendRow({group_names, new QStandardItem()});

  auto* chain_groups = new QStandardItem("Chain Groups");
  for (const auto& chain_group : kinematics_info.chain_groups)
    chain_groups->appendRow(new ChainGroupStandardItem(chain_group.first.c_str(), chain_group.second));

  appendRow({chain_groups, new QStandardItem()});

  auto* joint_groups = new QStandardItem("Joint Groups");
  for (const auto& joint_group : kinematics_info.joint_groups)
    joint_groups->appendRow(new JointGroupStandardItem(joint_group.first.c_str(), joint_group.second));

  appendRow({joint_groups, new QStandardItem()});

  auto* link_groups = new QStandardItem("Link Groups");
  for (const auto& link_group : kinematics_info.link_groups)
    link_groups->appendRow(new LinkGroupStandardItem(link_group.first.c_str(), link_group.second));

  appendRow({link_groups, new QStandardItem()});

  auto* group_states = new GroupJointStatesStandardItem(kinematics_info.group_states);
  appendRow({group_states, new QStandardItem()});

  auto* group_tcps = new GroupTCPsStandardItem(kinematics_info.group_tcps);
  appendRow({group_tcps, new QStandardItem()});

  auto* kin_plugins = new KinematicsPluginInfoStandardItem(kinematics_info.kinematics_plugin_info);
  appendRow({kin_plugins, new QStandardItem()});
}
}

