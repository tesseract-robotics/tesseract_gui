#include <tesseract_gui/widgets/scene_graph/octree_standard_item.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, OCTREE_ICON, (":/tesseract_gui/png/octree.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, NUMERIC_ICON, (":/tesseract_gui/png/numeric.png"));

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, CUBE_ICON, (":/tesseract_gui/png/cube.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, SPHERE_ICON, (":/tesseract_gui/png/sphere.png"));

namespace tesseract_gui
{
OctreeStandardItem::OctreeStandardItem(tesseract_geometry::Octree::Ptr octree)
  : QStandardItem(*OCTREE_ICON(), "Octree")
  , octree(std::move(octree))
{
  ctor();
}

OctreeStandardItem::OctreeStandardItem(const QString &text, tesseract_geometry::Octree::Ptr octree)
  : QStandardItem(*OCTREE_ICON(), text)
  , octree(std::move(octree))
{
  ctor();
}

OctreeStandardItem::OctreeStandardItem(const QIcon &icon, const QString &text, tesseract_geometry::Octree::Ptr octree)
  : QStandardItem(icon, text)
  , octree(std::move(octree))
{
  ctor();
}

int OctreeStandardItem::type() const
{
  return static_cast<int>(StandardItemType::OCTREE);
}

void OctreeStandardItem::ctor()
{
  {
    auto* name = new QStandardItem(*NUMERIC_ICON(), "pruned");
    auto* value = new QStandardItem(QString("%1").arg(((octree->getPruned()) ? "True" : "False")));
    appendRow({name, value});
  }

  {
    auto* name = new QStandardItem(*NUMERIC_ICON(), "subshape");

    QStandardItem* value;
    switch (octree->getSubType())
    {
      case tesseract_geometry::Octree::BOX:
      {
        value = new QStandardItem(*CUBE_ICON(), "BOX");
        break;
      }
      case tesseract_geometry::Octree::SPHERE_INSIDE:
      {
        value = new QStandardItem(*SPHERE_ICON(), "SPHERE INSIDE");
        break;
      }
      case tesseract_geometry::Octree::SPHERE_OUTSIDE:
      {
        value = new QStandardItem(*SPHERE_ICON(), "SPHERE OUTSIDE");
        break;
      }

    }
    appendRow({name, value});
  }

  {
    auto* name = new QStandardItem(*NUMERIC_ICON(), "shape count");
    auto* value = new QStandardItem(QString("%1").arg(octree->calcNumSubShapes()));
    appendRow({name, value});
  }
}
}

