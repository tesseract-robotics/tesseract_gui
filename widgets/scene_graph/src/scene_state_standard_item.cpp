#include <tesseract_gui/widgets/scene_graph/scene_state_standard_item.h>
#include <tesseract_gui/widgets/scene_graph/origin_standard_item.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, SCENE_GRAPH_ICON, (":/tesseract_gui/ignition/model.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, ORIGIN_ICON, (":/tesseract_gui/png/origin.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, LINK_VECTOR_ICON, (":/tesseract_gui/ignition/link_vector.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, JOINT_VECTOR_ICON, (":/tesseract_gui/ignition/joint_vector.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, NUMERIC_ICON, (":/tesseract_gui/png/numeric.png"));

namespace tesseract_gui
{
SceneStateStandardItem::SceneStateStandardItem(tesseract_scene_graph::SceneState scene_state)
  : QStandardItem(*SCENE_GRAPH_ICON(), "Scene State")
  , scene_state(std::move(scene_state))
{
  ctor();
}

SceneStateStandardItem::SceneStateStandardItem(const QString &text, tesseract_scene_graph::SceneState scene_state)
  : QStandardItem(*SCENE_GRAPH_ICON(), text)
  , scene_state(std::move(scene_state))
{
  ctor();
}

SceneStateStandardItem::SceneStateStandardItem(const QIcon &icon, const QString &text, tesseract_scene_graph::SceneState scene_state)
  : QStandardItem(icon, text)
  , scene_state(std::move(scene_state))
{
  ctor();
}

int SceneStateStandardItem::type() const
{
  return static_cast<int>(StandardItemType::SCENE_STATE);
}

void SceneStateStandardItem::ctor()
{
  auto* values_item = new QStandardItem(*JOINT_VECTOR_ICON(), "Values");
  for (auto& joint : scene_state.joints)
  {
    auto* x_name = new QStandardItem(*NUMERIC_ICON(), QString::fromStdString(joint.first));
    auto* x_value = new QStandardItem(QString("%1").arg(joint.second));
    values_item->appendRow({x_name, x_value});
  }
  values_item->sortChildren(0);
  appendRow(values_item);

  auto* links_item = new QStandardItem(*LINK_VECTOR_ICON(), "Links");
  for (auto& link : scene_state.link_transforms)
  {
    auto* item = new OriginStandardItem(*ORIGIN_ICON(), QString::fromStdString(link.first), link.second);
    links_item->appendRow(item);
  }
  links_item->sortChildren(0);
  appendRow(links_item);

  auto* joints_item = new QStandardItem(*JOINT_VECTOR_ICON(), "Joints");
  for (auto& joint : scene_state.joint_transforms)
  {
    auto* item = new OriginStandardItem(*ORIGIN_ICON(), QString::fromStdString(joint.first), joint.second);
    joints_item->appendRow(item);
  }
  joints_item->sortChildren(0);
  appendRow(joints_item);
}
}
