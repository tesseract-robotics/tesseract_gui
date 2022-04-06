#include <tesseract_gui/widgets/scene_graph/capsule_standard_item.h>
#include <tesseract_gui/widgets/common/standard_item_utils.h>
#include <tesseract_gui/common/standard_item_type.h>

namespace tesseract_gui
{
CapsuleStandardItem::CapsuleStandardItem(tesseract_geometry::Capsule::Ptr capsule)
  : QStandardItem(QIcon(":/tesseract_gui/png/capsule.png"), "Capsule")
  , capsule(std::move(capsule))
{
  ctor();
}

CapsuleStandardItem::CapsuleStandardItem(const QString &text, tesseract_geometry::Capsule::Ptr capsule)
  : QStandardItem(QIcon(":/tesseract_gui/png/capsule.png"), text)
  , capsule(std::move(capsule))
{
  ctor();
}

CapsuleStandardItem::CapsuleStandardItem(const QIcon &icon, const QString &text, tesseract_geometry::Capsule::Ptr capsule)
  : QStandardItem(icon, text)
  , capsule(std::move(capsule))
{
  ctor();
}

int CapsuleStandardItem::type() const
{
  return static_cast<int>(StandardItemType::CAPSULE);
}

void CapsuleStandardItem::ctor()
{
  appendRow(createStandardItemFloat("radius", capsule->getRadius()));
  appendRow(createStandardItemFloat("length", capsule->getLength()));
}
}

