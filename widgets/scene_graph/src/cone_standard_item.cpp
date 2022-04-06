#include <tesseract_gui/widgets/scene_graph/cone_standard_item.h>
#include <tesseract_gui/widgets/common/standard_item_utils.h>
#include <tesseract_gui/common/standard_item_type.h>

namespace tesseract_gui
{
ConeStandardItem::ConeStandardItem(tesseract_geometry::Cone::Ptr cone)
  : QStandardItem(QIcon(":/tesseract_gui/png/cone.png"), "Capsule")
  , cone(std::move(cone))
{
  ctor();
}

ConeStandardItem::ConeStandardItem(const QString &text, tesseract_geometry::Cone::Ptr cone)
  : QStandardItem(QIcon(":/tesseract_gui/png/cone.png"), text)
  , cone(std::move(cone))
{
  ctor();
}

ConeStandardItem::ConeStandardItem(const QIcon &icon, const QString &text, tesseract_geometry::Cone::Ptr cone)
  : QStandardItem(icon, text)
  , cone(std::move(cone))
{
  ctor();
}

int ConeStandardItem::type() const
{
  return static_cast<int>(StandardItemType::CONE);
}

void ConeStandardItem::ctor()
{
  appendRow(createStandardItemFloat("radius", cone->getRadius()));
  appendRow(createStandardItemFloat("length", cone->getLength()));
}
}

