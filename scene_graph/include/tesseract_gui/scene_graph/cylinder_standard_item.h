#ifndef TESSERACT_GUI_SCENE_GRAPH_CYLINDER_STANDARD_ITEM_H
#define TESSERACT_GUI_SCENE_GRAPH_CYLINDER_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_geometry/impl/cylinder.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class CylinderStandardItem : public QStandardItem
{
public:
  CylinderStandardItem(tesseract_geometry::Cylinder::Ptr cylinder);
  explicit CylinderStandardItem(const QString &text, tesseract_geometry::Cylinder::Ptr cylinder);
  CylinderStandardItem(const QIcon &icon, const QString &text, tesseract_geometry::Cylinder::Ptr cylinder);
  int type() const override;

  tesseract_geometry::Cylinder::Ptr cylinder;

private:
  void ctor();
};
}

#endif // TESSERACT_GUI_SCENE_GRAPH_CYLINDER_STANDARD_ITEM_H
