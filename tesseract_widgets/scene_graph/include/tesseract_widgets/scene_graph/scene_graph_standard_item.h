#ifndef TESSERACT_WIDGETS_SCENE_GRAPH_SCENE_GRAPH_STANDARD_ITEM_H
#define TESSERACT_WIDGETS_SCENE_GRAPH_SCENE_GRAPH_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_scene_graph/graph.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class SceneGraphStandardItem : public QStandardItem
{
public:
  SceneGraphStandardItem(tesseract_scene_graph::SceneGraph::UPtr scene_graph);
  explicit SceneGraphStandardItem(const QString &text, tesseract_scene_graph::SceneGraph::UPtr scene_graph);
  SceneGraphStandardItem(const QIcon &icon, const QString &text, tesseract_scene_graph::SceneGraph::UPtr scene_graph);
  int type() const override;

  tesseract_scene_graph::SceneGraph::UPtr scene_graph;

private:
  void ctor();
};
}

#endif // TESSERACT_WIDGETS_SCENE_GRAPH_SCENE_GRAPH_STANDARD_ITEM_H
