#ifndef TESSERACT_GUI_SCENE_GRAPH_LIMITS_STANDARD_ITEM_H
#define TESSERACT_GUI_SCENE_GRAPH_LIMITS_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_scene_graph/joint.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class LimitsStandardItem : public QStandardItem
{
public:
  LimitsStandardItem(tesseract_scene_graph::JointLimits::Ptr limits);
  explicit LimitsStandardItem(const QString &text, tesseract_scene_graph::JointLimits::Ptr limits);
  LimitsStandardItem(const QIcon &icon, const QString &text, tesseract_scene_graph::JointLimits::Ptr limits);
  int type() const override;

  tesseract_scene_graph::JointLimits::Ptr limits;

private:
  void ctor();
};
}

#endif // TESSERACT_GUI_SCENE_GRAPH_LIMITS_STANDARD_ITEM_H
