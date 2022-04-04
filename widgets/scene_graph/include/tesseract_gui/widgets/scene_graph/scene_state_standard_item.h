#ifndef TESSERACT_GUI_SCENE_GRAPH_SCENE_STATE_STANDARD_ITEM_H
#define TESSERACT_GUI_SCENE_GRAPH_SCENE_STATE_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_scene_graph/scene_state.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class SceneStateStandardItem : public QStandardItem
{
public:
  explicit SceneStateStandardItem(tesseract_scene_graph::SceneState scene_state);
  explicit SceneStateStandardItem(const QString &text, tesseract_scene_graph::SceneState scene_state);
  explicit SceneStateStandardItem(const QIcon &icon, const QString &text, tesseract_scene_graph::SceneState scene_state);
  int type() const override;

  tesseract_scene_graph::SceneState scene_state;

private:
  void ctor();
};
}

#endif // TESSERACT_GUI_SCENE_GRAPH_SCENE_STATE_STANDARD_ITEM_H
