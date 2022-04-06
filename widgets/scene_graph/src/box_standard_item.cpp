#include <tesseract_gui/widgets/scene_graph/box_standard_item.h>
#include <tesseract_gui/widgets/common/standard_item_utils.h>
#include <tesseract_gui/common/standard_item_type.h>

namespace tesseract_gui
{
BoxStandardItem::BoxStandardItem(tesseract_geometry::Box::Ptr box)
  : QStandardItem(QIcon(":/tesseract_gui/png/cube.png"), "Box")
  , box(std::move(box))
{
  ctor();
}

BoxStandardItem::BoxStandardItem(const QString &text, tesseract_geometry::Box::Ptr box)
  : QStandardItem(QIcon(":/tesseract_gui/png/cube.png"), text)
  , box(std::move(box))
{
  ctor();
}

BoxStandardItem::BoxStandardItem(const QIcon &icon, const QString &text, tesseract_geometry::Box::Ptr box)
  : QStandardItem(icon, text)
  , box(std::move(box))
{
  ctor();
}

int BoxStandardItem::type() const
{
  return static_cast<int>(StandardItemType::BOX);
}

void BoxStandardItem::ctor()
{
  appendRow(createStandardItemFloat("x", box->getX()));
  appendRow(createStandardItemFloat("y", box->getY()));
  appendRow(createStandardItemFloat("z", box->getZ()));
}
}

