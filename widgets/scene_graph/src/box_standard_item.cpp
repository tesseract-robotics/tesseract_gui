#include <tesseract_gui/widgets/scene_graph/box_standard_item.h>
#include <tesseract_gui/widgets/common/standard_item_utils.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, CUBE_ICON, (":/tesseract_gui/png/cube.png"));


namespace tesseract_gui
{
BoxStandardItem::BoxStandardItem(tesseract_geometry::Box::Ptr box)
  : QStandardItem(*CUBE_ICON(), "Box")
  , box(std::move(box))
{
  ctor();
}

BoxStandardItem::BoxStandardItem(const QString &text, tesseract_geometry::Box::Ptr box)
  : QStandardItem(*CUBE_ICON(), text)
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

