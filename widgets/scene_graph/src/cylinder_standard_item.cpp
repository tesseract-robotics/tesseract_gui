#include <tesseract_gui/widgets/scene_graph/cylinder_standard_item.h>
#include <tesseract_gui/widgets/common/standard_item_utils.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, CYLINDER_ICON, (":/tesseract_gui/png/cylinder.png"));

namespace tesseract_gui
{
CylinderStandardItem::CylinderStandardItem(tesseract_geometry::Cylinder::Ptr cylinder)
  : QStandardItem(*CYLINDER_ICON(), "Cylinder")
  , cylinder(std::move(cylinder))
{
  ctor();
}

CylinderStandardItem::CylinderStandardItem(const QString &text, tesseract_geometry::Cylinder::Ptr cylinder)
  : QStandardItem(*CYLINDER_ICON(), text)
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

