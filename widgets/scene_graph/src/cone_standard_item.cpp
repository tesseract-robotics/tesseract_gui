#include <tesseract_gui/widgets/scene_graph/cone_standard_item.h>
#include <tesseract_gui/widgets/common/standard_item_utils.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, CONE_ICON, (":/tesseract_gui/png/cone.png"));

namespace tesseract_gui
{
ConeStandardItem::ConeStandardItem(tesseract_geometry::Cone::Ptr cone)
  : QStandardItem(*CONE_ICON(), "Capsule")
  , cone(std::move(cone))
{
  ctor();
}

ConeStandardItem::ConeStandardItem(const QString &text, tesseract_geometry::Cone::Ptr cone)
  : QStandardItem(*CONE_ICON(), text)
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

