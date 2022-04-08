#ifndef TESSERACT_WIDGETS_SCENE_GRAPH_SPHERE_STANDARD_ITEM_H
#define TESSERACT_WIDGETS_SCENE_GRAPH_SPHERE_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_geometry/impl/sphere.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class SphereStandardItem : public QStandardItem
{
public:
  SphereStandardItem(tesseract_geometry::Sphere::Ptr sphere);
  explicit SphereStandardItem(const QString &text, tesseract_geometry::Sphere::Ptr sphere);
  SphereStandardItem(const QIcon &icon, const QString &text, tesseract_geometry::Sphere::Ptr sphere);
  int type() const override;

  tesseract_geometry::Sphere::Ptr sphere;

private:
  void ctor();
};
}

#endif // TESSERACT_WIDGETS_SCENE_GRAPH_SPHERE_STANDARD_ITEM_H
