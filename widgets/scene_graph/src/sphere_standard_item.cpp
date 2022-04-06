#include <tesseract_gui/widgets/scene_graph/sphere_standard_item.h>
#include <tesseract_gui/widgets/common/standard_item_utils.h>
#include <tesseract_gui/common/standard_item_type.h>

namespace tesseract_gui
{
SphereStandardItem::SphereStandardItem(tesseract_geometry::Sphere::Ptr sphere)
  : QStandardItem(QIcon(":/tesseract_gui/png/sphere.png"), "Sphere")
  , sphere(std::move(sphere))
{
  ctor();
}

SphereStandardItem::SphereStandardItem(const QString &text, tesseract_geometry::Sphere::Ptr sphere)
  : QStandardItem(QIcon(":/tesseract_gui/png/sphere.png"), text)
  , sphere(std::move(sphere))
{
  ctor();
}

SphereStandardItem::SphereStandardItem(const QIcon &icon, const QString &text, tesseract_geometry::Sphere::Ptr sphere)
  : QStandardItem(icon, text)
  , sphere(std::move(sphere))
{
  ctor();
}

int SphereStandardItem::type() const
{
  return static_cast<int>(StandardItemType::SPHERE);
}

void SphereStandardItem::ctor()
{
  appendRow(createStandardItemFloat("radius", sphere->getRadius()));
}
}

