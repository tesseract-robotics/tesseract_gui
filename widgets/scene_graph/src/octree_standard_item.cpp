#include <tesseract_gui/widgets/scene_graph/octree_standard_item.h>
#include <tesseract_gui/widgets/common/standard_item_utils.h>
#include <tesseract_gui/common/standard_item_type.h>

namespace tesseract_gui
{
OctreeStandardItem::OctreeStandardItem(tesseract_geometry::Octree::Ptr octree)
  : QStandardItem(QIcon(":/tesseract_gui/png/octree.png"), "Octree")
  , octree(std::move(octree))
{
  ctor();
}

OctreeStandardItem::OctreeStandardItem(const QString &text, tesseract_geometry::Octree::Ptr octree)
  : QStandardItem(QIcon(":/tesseract_gui/png/octree.png"), text)
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
      appendRow(createStandardItemString(QIcon(":/tesseract_gui/png/cube.png"), "subshape", "BOX"));
      break;
    }
    case tesseract_geometry::Octree::SPHERE_INSIDE:
    {
      appendRow(createStandardItemString(QIcon(":/tesseract_gui/png/sphere.png"), "subshape", "SPHERE INSIDE"));
      break;
    }
    case tesseract_geometry::Octree::SPHERE_OUTSIDE:
    {
      appendRow(createStandardItemString(QIcon(":/tesseract_gui/png/sphere.png"), "subshape", "SPHERE OUTSIDE"));
      break;
    }
  }

  appendRow(createStandardItemFloat("shape count", octree->calcNumSubShapes()));
}
}

