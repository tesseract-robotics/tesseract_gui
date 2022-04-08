#ifndef TESSERACT_WIDGETS_SCENE_GRAPH_OCTREE_STANDARD_ITEM_H
#define TESSERACT_WIDGETS_SCENE_GRAPH_OCTREE_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_geometry/impl/octree.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class OctreeStandardItem : public QStandardItem
{
public:
  OctreeStandardItem(tesseract_geometry::Octree::Ptr octree);
  explicit OctreeStandardItem(const QString &text, tesseract_geometry::Octree::Ptr octree);
  OctreeStandardItem(const QIcon &icon, const QString &text, tesseract_geometry::Octree::Ptr octree);
  int type() const override;

  tesseract_geometry::Octree::Ptr octree;

private:
  void ctor();
};
}

#endif // TESSERACT_WIDGETS_SCENE_GRAPH_OCTREE_STANDARD_ITEM_H
