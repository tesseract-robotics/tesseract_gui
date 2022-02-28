#ifndef TESSERACT_GUI_SCENE_GRAPH_PLANE_STANDARD_ITEM_H
#define TESSERACT_GUI_SCENE_GRAPH_PLANE_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_geometry/impl/plane.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class PlaneStandardItem : public QStandardItem
{
public:
  PlaneStandardItem(tesseract_geometry::Plane::Ptr plane);
  explicit PlaneStandardItem(const QString &text, tesseract_geometry::Plane::Ptr plane);
  PlaneStandardItem(const QIcon &icon, const QString &text, tesseract_geometry::Plane::Ptr plane);
  int type() const override;

  tesseract_geometry::Plane::Ptr plane;

private:
  void ctor();
};
}

#endif // TESSERACT_GUI_SCENE_GRAPH_PLANE_STANDARD_ITEM_H
