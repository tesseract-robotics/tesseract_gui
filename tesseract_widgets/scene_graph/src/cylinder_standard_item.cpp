#include <tesseract_widgets/scene_graph/cylinder_standard_item.h>
#include <tesseract_widgets/common/standard_item_utils.h>
#include <tesseract_widgets/common/standard_item_type.h>

namespace tesseract_gui
{
CylinderStandardItem::CylinderStandardItem(tesseract_geometry::Cylinder::Ptr cylinder)
  : QStandardItem(QIcon(":/tesseract_widgets/png/cylinder.png"), "Cylinder")
  , cylinder(std::move(cylinder))
{
  ctor();
}

CylinderStandardItem::CylinderStandardItem(const QString &text, tesseract_geometry::Cylinder::Ptr cylinder)
  : QStandardItem(QIcon(":/tesseract_widgets/png/cylinder.png"), text)
  , cylinder(std::move(cylinder))
{
  ctor();
}

CylinderStandardItem::CylinderStandardItem(const QIcon &icon, const QString &text, tesseract_geometry::Cylinder::Ptr cylinder)
  : QStandardItem(icon, text)
  , cylinder(std::move(cylinder))
{
  ctor();
}

int CylinderStandardItem::type() const
{
  return static_cast<int>(StandardItemType::CYLINDER);
}

void CylinderStandardItem::ctor()
{
  appendRow(createStandardItemFloat("radius", cylinder->getRadius()));
  appendRow(createStandardItemFloat("length", cylinder->getLength()));
}
}
