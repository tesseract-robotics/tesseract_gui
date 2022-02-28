#ifndef TESSERACT_GUI_SCENE_GRAPH_LINK_STANDARD_ITEM_H
#define TESSERACT_GUI_SCENE_GRAPH_LINK_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_scene_graph/link.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class LinkStandardItem : public QStandardItem
{
public:
  LinkStandardItem(tesseract_scene_graph::Link::Ptr link);
  explicit LinkStandardItem(const QString &text, tesseract_scene_graph::Link::Ptr link);
  LinkStandardItem(const QIcon &icon, const QString &text, tesseract_scene_graph::Link::Ptr link);
  int type() const override;

  tesseract_scene_graph::Link::Ptr link;

private:
  void ctor();
};
}

#endif // TESSERACT_GUI_SCENE_GRAPH_LINK_STANDARD_ITEM_H
