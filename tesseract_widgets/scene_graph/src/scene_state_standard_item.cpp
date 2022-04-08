#include <tesseract_widgets/scene_graph/scene_state_standard_item.h>
#include <tesseract_widgets/common/transform_standard_item.h>
#include <tesseract_widgets/common/standard_item_utils.h>
#include <tesseract_widgets/common/standard_item_type.h>

namespace tesseract_gui
{
SceneStateStandardItem::SceneStateStandardItem(tesseract_scene_graph::SceneState scene_state)
  : QStandardItem(QIcon(":/tesseract_widgets/ignition/model.png"), "Scene State")
  , scene_state(std::move(scene_state))
{
  ctor();
}

SceneStateStandardItem::SceneStateStandardItem(const QString &text, tesseract_scene_graph::SceneState scene_state)
  : QStandardItem(QIcon(":/tesseract_widgets/ignition/model.png"), text)
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
  auto* values_item = new QStandardItem(QIcon(":/tesseract_widgets/ignition/joint_vector.png"), "Values");
  for (auto& joint : scene_state.joints)
    values_item->appendRow(createStandardItemFloat(joint.first, joint.second));

  values_item->sortChildren(0);
  appendRow(values_item);

  auto* links_item = new QStandardItem(QIcon(":/tesseract_widgets/ignition/link_vector.png"), "Links");
  for (auto& link : scene_state.link_transforms)
  {
    auto* item = new TransformStandardItem(QString::fromStdString(link.first), link.second);
    links_item->appendRow(item);
  }
  links_item->sortChildren(0);
  appendRow(links_item);

  auto* joints_item = new QStandardItem(QIcon(":/tesseract_widgets/ignition/joint_vector.png"), "Joints");
  for (auto& joint : scene_state.joint_transforms)
  {
    auto* item = new TransformStandardItem(QString::fromStdString(joint.first), joint.second);
    joints_item->appendRow(item);
  }
  joints_item->sortChildren(0);
  appendRow(joints_item);
}
}
