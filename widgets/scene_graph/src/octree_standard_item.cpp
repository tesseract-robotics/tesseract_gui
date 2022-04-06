#include <tesseract_gui/widgets/scene_graph/octree_standard_item.h>
#include <tesseract_gui/widgets/common/standard_item_utils.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, OCTREE_ICON, (":/tesseract_gui/png/octree.png"));
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
  appendRow(createStandardItemString("pruned", (octree->getPruned()) ? "True" : "False"));

   switch (octree->getSubType())
  {
    case tesseract_geometry::Octree::BOX:
    {
      appendRow(createStandardItemString(*CUBE_ICON(), "subshape", "BOX"));
      break;
    }
    case tesseract_geometry::Octree::SPHERE_INSIDE:
    {
      appendRow(createStandardItemString(*SPHERE_ICON(), "subshape", "SPHERE INSIDE"));
      break;
    }
    case tesseract_geometry::Octree::SPHERE_OUTSIDE:
    {
      appendRow(createStandardItemString(*SPHERE_ICON(), "subshape", "SPHERE OUTSIDE"));
      break;
    }
  }

  appendRow(createStandardItemFloat("shape count", octree->calcNumSubShapes()));
}
}

