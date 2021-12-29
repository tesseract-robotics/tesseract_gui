#include <tesseract_gui/scene_graph/cone_standard_item.h>
#include <tesseract_gui/scene_graph/types.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, CONE_ICON, (":/tesseract_gui/png/cone.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, NUMERIC_ICON, (":/tesseract_gui/png/numeric.png"));

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
  return static_cast<int>(SceneGraphItemType::CONE);
}

void ConeStandardItem::ctor()
{
  {
    auto* name = new QStandardItem(*NUMERIC_ICON(), "radius");
    auto* value = new QStandardItem(QString("%1").arg(cone->getRadius()));
    appendRow({name, value});
  }

  {
    auto* name = new QStandardItem(*NUMERIC_ICON(), "length");
    auto* value = new QStandardItem(QString("%1").arg(cone->getLength()));
    appendRow({name, value});
  }
}
}

