#include <tesseract_gui/scene_graph/box_standard_item.h>
#include <tesseract_gui/scene_graph/types.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, CUBE_ICON, (":/tesseract_gui/png/cube.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, NUMERIC_ICON, (":/tesseract_gui/png/numeric.png"));

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
  return static_cast<int>(SceneGraphItemType::BOX);
}

void BoxStandardItem::ctor()
{
  auto* x_name = new QStandardItem(*NUMERIC_ICON(), "x");
  auto* x_value = new QStandardItem(QString("%1").arg(box->getX()));
  appendRow({x_name, x_value});

  auto* y_name = new QStandardItem(*NUMERIC_ICON(), "y");
  auto* y_value = new QStandardItem(QString("%1").arg(box->getY()));
  appendRow({y_name, y_value});

  auto* z_name = new QStandardItem(*NUMERIC_ICON(), "z");
  auto* z_value = new QStandardItem(QString("%1").arg(box->getZ()));
  appendRow({z_name, z_value});
}
}

