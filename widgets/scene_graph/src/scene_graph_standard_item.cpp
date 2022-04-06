#include <tesseract_gui/widgets/scene_graph/scene_graph_standard_item.h>
#include <tesseract_gui/widgets/scene_graph/link_standard_item.h>
#include <tesseract_gui/widgets/scene_graph/joint_standard_item.h>
#include <tesseract_gui/widgets/common/standard_item_utils.h>
#include <tesseract_gui/common/standard_item_type.h>

namespace tesseract_gui
{
SceneGraphStandardItem::SceneGraphStandardItem(tesseract_scene_graph::SceneGraph::UPtr scene_graph)
  : QStandardItem(QIcon(":/tesseract_gui/ignition/model.png"), "SceneGraph")
  , scene_graph(std::move(scene_graph))
{
  ctor();
}

SceneGraphStandardItem::SceneGraphStandardItem(const QString &text, tesseract_scene_graph::SceneGraph::UPtr scene_graph)
  : QStandardItem(QIcon(":/tesseract_gui/ignition/model.png"), text)
  , scene_graph(std::move(scene_graph))
{
  ctor();
}

SceneGraphStandardItem::SceneGraphStandardItem(const QIcon &icon, const QString &text, tesseract_scene_graph::SceneGraph::UPtr scene_graph)
  : QStandardItem(icon, text)
  , scene_graph(std::move(scene_graph))
{
  ctor();
}

int SceneGraphStandardItem::type() const
{
  return static_cast<int>(StandardItemType::SCENE_GRAPH);
}

void SceneGraphStandardItem::ctor()
{
  appendRow(createStandardItemString("name", scene_graph->getName()));
  appendRow(createStandardItemString("root_link", scene_graph->getRoot()));

  auto* links_item = new QStandardItem(QIcon(":/tesseract_gui/ignition/link_vector.png"), "Links");
  for (const auto& link : scene_graph->getLinks())
  {
    auto* item = new LinkStandardItem(QString::fromStdString(link->getName()), std::make_shared<tesseract_scene_graph::Link>(link->clone()));
    links_item->appendRow(item);
  }
  appendRow(links_item);

  auto* joints_item = new QStandardItem(QIcon(":/tesseract_gui/ignition/joint_vector.png"), "Joints");
  for (const auto& joint : scene_graph->getJoints())
  {
    auto* item = new JointStandardItem(QString::fromStdString(joint->getName()), std::make_shared<tesseract_scene_graph::Joint>(joint->clone()));
    joints_item->appendRow(item);
  }
  appendRow(joints_item);
}
}
