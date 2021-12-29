#ifndef TESSERACT_GUI_SCENE_GRAPH_SAFETY_STANDARD_ITEM_H
#define TESSERACT_GUI_SCENE_GRAPH_SAFETY_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_scene_graph/joint.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class SafetyStandardItem : public QStandardItem
{
public:
  SafetyStandardItem(tesseract_scene_graph::JointSafety::Ptr safety);
  explicit SafetyStandardItem(const QString &text, tesseract_scene_graph::JointSafety::Ptr safety);
  SafetyStandardItem(const QIcon &icon, const QString &text, tesseract_scene_graph::JointSafety::Ptr safety);
  int type() const override;

  tesseract_scene_graph::JointSafety::Ptr safety;

private:
  void ctor();
};
}

#endif // TESSERACT_GUI_SCENE_GRAPH_SAFETY_STANDARD_ITEM_H
