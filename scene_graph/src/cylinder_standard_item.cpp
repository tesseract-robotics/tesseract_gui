#include <tesseract_gui/scene_graph/cylinder_standard_item.h>
#include <tesseract_gui/scene_graph/types.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, CYLINDER_ICON, (":/tesseract_gui/png/cylinder.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, NUMERIC_ICON, (":/tesseract_gui/png/numeric.png"));

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
  return static_cast<int>(SceneGraphItemType::CYLINDER);
}

void CylinderStandardItem::ctor()
{
  {
    auto* name = new QStandardItem(*NUMERIC_ICON(), "radius");
    auto* value = new QStandardItem(QString("%1").arg(cylinder->getRadius()));
    appendRow({name, value});
  }

  {
    auto* name = new QStandardItem(*NUMERIC_ICON(), "length");
    auto* value = new QStandardItem(QString("%1").arg(cylinder->getLength()));
    appendRow({name, value});
  }
}
}

