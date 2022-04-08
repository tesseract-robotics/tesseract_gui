#ifndef TESSERACT_WIDGETS_SCENE_GRAPH_VISUAL_STANDARD_ITEM_H
#define TESSERACT_WIDGETS_SCENE_GRAPH_VISUAL_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_scene_graph/link.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class VisualStandardItem : public QStandardItem
{
public:
  VisualStandardItem(tesseract_scene_graph::Visual::Ptr visual);
  explicit VisualStandardItem(const QString &text, tesseract_scene_graph::Visual::Ptr visual);
  VisualStandardItem(const QIcon &icon, const QString &text, tesseract_scene_graph::Visual::Ptr visual);
  int type() const override;

  tesseract_scene_graph::Visual::Ptr visual;

private:
  void ctor();
};
}

#endif // TESSERACT_WIDGETS_SCENE_GRAPH_VISUAL_STANDARD_ITEM_H
