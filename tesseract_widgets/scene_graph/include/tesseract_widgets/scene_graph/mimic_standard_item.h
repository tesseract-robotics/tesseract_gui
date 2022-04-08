#ifndef TESSERACT_WIDGETS_SCENE_GRAPH_MIMIC_STANDARD_ITEM_H
#define TESSERACT_WIDGETS_SCENE_GRAPH_MIMIC_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_scene_graph/joint.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class MimicStandardItem : public QStandardItem
{
public:
  MimicStandardItem(tesseract_scene_graph::JointMimic::Ptr mimic);
  explicit MimicStandardItem(const QString &text, tesseract_scene_graph::JointMimic::Ptr mimic);
  MimicStandardItem(const QIcon &icon, const QString &text, tesseract_scene_graph::JointMimic::Ptr mimic);
  int type() const override;

  tesseract_scene_graph::JointMimic::Ptr mimic;

private:
  void ctor();
};
}

#endif // TESSERACT_WIDGETS_SCENE_GRAPH_MIMIC_STANDARD_ITEM_H
