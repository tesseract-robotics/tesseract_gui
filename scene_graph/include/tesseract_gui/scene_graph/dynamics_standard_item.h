#ifndef TESSERACT_GUI_SCENE_GRAPH_DYNAMICS_STANDARD_ITEM_H
#define TESSERACT_GUI_SCENE_GRAPH_DYNAMICS_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_scene_graph/joint.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class DynamicsStandardItem : public QStandardItem
{
public:
  DynamicsStandardItem(tesseract_scene_graph::JointDynamics::Ptr dynamics);
  explicit DynamicsStandardItem(const QString &text, tesseract_scene_graph::JointDynamics::Ptr dynamics);
  DynamicsStandardItem(const QIcon &icon, const QString &text, tesseract_scene_graph::JointDynamics::Ptr dynamics);
  int type() const override;

  tesseract_scene_graph::JointDynamics::Ptr dynamics;

private:
  void ctor();
};
}

#endif // TESSERACT_GUI_SCENE_GRAPH_DYNAMICS_STANDARD_ITEM_H
