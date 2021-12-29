#include <tesseract_gui/scene_graph/plane_standard_item.h>
#include <tesseract_gui/scene_graph/types.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, PLANE_ICON, (":/tesseract_gui/png/plane.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, NUMERIC_ICON, (":/tesseract_gui/png/numeric.png"));

namespace tesseract_gui
{
PlaneStandardItem::PlaneStandardItem(tesseract_geometry::Plane::Ptr plane)
  : QStandardItem(*PLANE_ICON(), "Plane")
  , plane(std::move(plane))
{
  ctor();
}

PlaneStandardItem::PlaneStandardItem(const QString &text, tesseract_geometry::Plane::Ptr plane)
  : QStandardItem(*PLANE_ICON(), text)
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
  return static_cast<int>(SceneGraphItemType::PLANE);
}

void PlaneStandardItem::ctor()
{
  {
    auto* name = new QStandardItem(*NUMERIC_ICON(), "a");
    auto* value = new QStandardItem(QString("%1").arg(plane->getA()));
    appendRow({name, value});
  }

  {
    auto* name = new QStandardItem(*NUMERIC_ICON(), "b");
    auto* value = new QStandardItem(QString("%1").arg(plane->getB()));
    appendRow({name, value});
  }

  {
    auto* name = new QStandardItem(*NUMERIC_ICON(), "c");
    auto* value = new QStandardItem(QString("%1").arg(plane->getC()));
    appendRow({name, value});
  }

  {
    auto* name = new QStandardItem(*NUMERIC_ICON(), "d");
    auto* value = new QStandardItem(QString("%1").arg(plane->getD()));
    appendRow({name, value});
  }
}
}

