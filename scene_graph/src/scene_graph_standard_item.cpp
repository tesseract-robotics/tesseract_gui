#include <tesseract_gui/scene_graph/scene_graph_standard_item.h>
#include <tesseract_gui/scene_graph/link_standard_item.h>
#include <tesseract_gui/scene_graph/joint_standard_item.h>
#include <tesseract_gui/scene_graph/types.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, SCENE_GRAPH_ICON, (":/tesseract_gui/ignition/model.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, LINK_VECTOR_ICON, (":/tesseract_gui/ignition/link_vector.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, JOINT_VECTOR_ICON, (":/tesseract_gui/ignition/joint_vector.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, TEXT_ICON, (":/tesseract_gui/png/text.png"));

namespace tesseract_gui
{
SceneGraphStandardItem::SceneGraphStandardItem(tesseract_scene_graph::SceneGraph::UPtr scene_graph)
  : QStandardItem(*SCENE_GRAPH_ICON(), "SceneGraph")
  , scene_graph(std::move(scene_graph))
{
  ctor();
}

SceneGraphStandardItem::SceneGraphStandardItem(const QString &text, tesseract_scene_graph::SceneGraph::UPtr scene_graph)
  : QStandardItem(*SCENE_GRAPH_ICON(), text)
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
  return static_cast<int>(SceneGraphItemType::SCENE_GRAPH);
}

void SceneGraphStandardItem::ctor()
{
  {
    auto* item = new QStandardItem(*TEXT_ICON(), "name");
    auto* value = new QStandardItem(QString::fromStdString(scene_graph->getName()));
    appendRow({item, value});
  }

  {
    auto* item = new QStandardItem(*TEXT_ICON(), "root_link");
    auto* value = new QStandardItem(QString::fromStdString(scene_graph->getRoot()));
    appendRow({item, value});
  }

  auto* links_item = new QStandardItem(*LINK_VECTOR_ICON(), "Links");
  for (const auto& link : scene_graph->getLinks())
  {
    auto* item = new LinkStandardItem(std::make_shared<tesseract_scene_graph::Link>(link->clone()));
    links_item->appendRow(item);
  }
  appendRow(links_item);

  auto* joints_item = new QStandardItem(*JOINT_VECTOR_ICON(), "Joints");
  for (const auto& joint : scene_graph->getJoints())
  {
    auto* item = new JointStandardItem(std::make_shared<tesseract_scene_graph::Joint>(joint->clone()));
    joints_item->appendRow(item);
  }
  appendRow(joints_item);
}
}
