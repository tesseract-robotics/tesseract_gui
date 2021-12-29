#ifndef TESSERACT_GUI_SCENE_GRAPH_INERTIAL_STANDARD_ITEM_H
#define TESSERACT_GUI_SCENE_GRAPH_INERTIAL_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_scene_graph/link.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class InertialStandardItem : public QStandardItem
{
public:
  InertialStandardItem(tesseract_scene_graph::Inertial::Ptr inertial);
  explicit InertialStandardItem(const QString &text, tesseract_scene_graph::Inertial::Ptr inertial);
  InertialStandardItem(const QIcon &icon, const QString &text, tesseract_scene_graph::Inertial::Ptr inertial);
  int type() const override;

  tesseract_scene_graph::Inertial::Ptr inertial;

private:
  void ctor();
};
}

#endif // TESSERACT_GUI_SCENE_GRAPH_INERTIAL_STANDARD_ITEM_H
