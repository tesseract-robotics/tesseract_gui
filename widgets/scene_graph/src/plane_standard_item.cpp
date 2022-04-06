#include <tesseract_gui/widgets/scene_graph/plane_standard_item.h>
#include <tesseract_gui/widgets/common/standard_item_utils.h>
#include <tesseract_gui/common/standard_item_type.h>

namespace tesseract_gui
{
PlaneStandardItem::PlaneStandardItem(tesseract_geometry::Plane::Ptr plane)
  : QStandardItem(QIcon(":/tesseract_gui/png/plane.png"), "Plane")
  , plane(std::move(plane))
{
  ctor();
}

PlaneStandardItem::PlaneStandardItem(const QString &text, tesseract_geometry::Plane::Ptr plane)
  : QStandardItem(QIcon(":/tesseract_gui/png/plane.png"), text)
  , plane(std::move(plane))
{
  ctor();
}

PlaneStandardItem::PlaneStandardItem(const QIcon &icon, const QString &text, tesseract_geometry::Plane::Ptr plane)
  : QStandardItem(icon, text)
  , plane(std::move(plane))
{
  ctor();
}

int PlaneStandardItem::type() const
{
  return static_cast<int>(StandardItemType::PLANE);
}

void PlaneStandardItem::ctor()
{
  appendRow(createStandardItemFloat("a", plane->getA()));
  appendRow(createStandardItemFloat("b", plane->getB()));
  appendRow(createStandardItemFloat("c", plane->getC()));
  appendRow(createStandardItemFloat("d", plane->getD()));
}
}

