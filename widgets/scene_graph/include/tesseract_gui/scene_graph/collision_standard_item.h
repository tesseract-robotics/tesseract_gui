#ifndef TESSERACT_GUI_SCENE_GRAPH_COLLISION_STANDARD_ITEM_H
#define TESSERACT_GUI_SCENE_GRAPH_COLLISION_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_scene_graph/link.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class CollisionStandardItem : public QStandardItem
{
public:
  CollisionStandardItem(tesseract_scene_graph::Collision::Ptr collision);
  explicit CollisionStandardItem(const QString &text, tesseract_scene_graph::Collision::Ptr collision);
  CollisionStandardItem(const QIcon &icon, const QString &text, tesseract_scene_graph::Collision::Ptr collision);
  int type() const override;

  tesseract_scene_graph::Collision::Ptr collision;

private:
  void ctor();
};
}

#endif // TESSERACT_GUI_SCENE_GRAPH_COLLISION_STANDARD_ITEM_H
