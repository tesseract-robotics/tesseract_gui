#ifndef TESSERACT_WIDGETS_SCENE_GRAPH_SCENE_STATE_MODEL_H
#define TESSERACT_WIDGETS_SCENE_GRAPH_SCENE_STATE_MODEL_H

#include <memory>
#include <QStandardItem>

namespace tesseract_scene_graph
{
class SceneState;
}

namespace tesseract_gui
{
class SceneStateModelPrivate;
class SceneStateModel : public QStandardItemModel
{
  Q_OBJECT

public:
  explicit SceneStateModel(QObject* parent = nullptr);
  ~SceneStateModel() override;
  SceneStateModel(const SceneStateModel& other);
  SceneStateModel& operator=(const SceneStateModel& other);

  void setState(const tesseract_scene_graph::SceneState& scene_state);

  void clear();

private:
  std::unique_ptr<SceneStateModelPrivate> data_;
};

}  // namespace tesseract_gui

#endif  // TESSERACT_WIDGETS_SCENE_GRAPH_SCENE_STATE_MODEL_H
