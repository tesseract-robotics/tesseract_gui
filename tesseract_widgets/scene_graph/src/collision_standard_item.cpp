#include <tesseract_widgets/scene_graph/collision_standard_item.h>
#include <tesseract_widgets/scene_graph/box_standard_item.h>
#include <tesseract_widgets/scene_graph/capsule_standard_item.h>
#include <tesseract_widgets/scene_graph/cone_standard_item.h>
#include <tesseract_widgets/scene_graph/cylinder_standard_item.h>
#include <tesseract_widgets/scene_graph/plane_standard_item.h>
#include <tesseract_widgets/scene_graph/sphere_standard_item.h>
#include <tesseract_widgets/scene_graph/polygon_mesh_standard_item.h>
#include <tesseract_widgets/scene_graph/octree_standard_item.h>
#include <tesseract_widgets/common/transform_standard_item.h>
#include <tesseract_widgets/common/standard_item_utils.h>
#include <tesseract_widgets/common/standard_item_type.h>

namespace tesseract_gui
{
CollisionStandardItem::CollisionStandardItem(tesseract_scene_graph::Collision::Ptr collision)
  : QStandardItem(QIcon(":/tesseract_widgets/ignition/collision.png"), "Collision")
  , collision(std::move(collision))
{
  ctor();
}

CollisionStandardItem::CollisionStandardItem(const QString &text, tesseract_scene_graph::Collision::Ptr collision)
  : QStandardItem(QIcon(":/tesseract_widgets/ignition/collision.png"),text)
  , collision(std::move(collision))
{
  ctor();
}
CollisionStandardItem::CollisionStandardItem(const QIcon &icon, const QString &text, tesseract_scene_graph::Collision::Ptr collision)
  : QStandardItem(icon, text)
  , collision(std::move(collision))
{
  ctor();
}

int CollisionStandardItem::type() const
{
  return static_cast<int>(StandardItemType::COLLISION);
}

void CollisionStandardItem::ctor()
{
  appendRow(createStandardItemString("name", collision->name));
  appendRow(new TransformStandardItem(collision->origin));

  QStandardItem* geometry_item;
  switch (collision->geometry->getType())
  {
    case tesseract_geometry::GeometryType::BOX:
    {
      geometry_item = new BoxStandardItem(std::static_pointer_cast<tesseract_geometry::Box>(collision->geometry));
      break;
    }
    case tesseract_geometry::GeometryType::CAPSULE:
    {
      geometry_item = new CapsuleStandardItem(std::static_pointer_cast<tesseract_geometry::Capsule>(collision->geometry));
      break;
    }
    case tesseract_geometry::GeometryType::CONE:
    {
      geometry_item = new ConeStandardItem(std::static_pointer_cast<tesseract_geometry::Cone>(collision->geometry));
      break;
    }
    case tesseract_geometry::GeometryType::CYLINDER:
    {
      geometry_item = new CylinderStandardItem(std::static_pointer_cast<tesseract_geometry::Cylinder>(collision->geometry));
      break;
    }
    case tesseract_geometry::GeometryType::PLANE:
    {
      geometry_item = new PlaneStandardItem(std::static_pointer_cast<tesseract_geometry::Plane>(collision->geometry));
      break;
    }
    case tesseract_geometry::GeometryType::SPHERE:
    {
      geometry_item = new SphereStandardItem(std::static_pointer_cast<tesseract_geometry::Sphere>(collision->geometry));
      break;
    }
    case tesseract_geometry::GeometryType::OCTREE:
    {
      geometry_item = new OctreeStandardItem(std::static_pointer_cast<tesseract_geometry::Octree>(collision->geometry));
      break;
    }
    case tesseract_geometry::GeometryType::CONVEX_MESH:
    case tesseract_geometry::GeometryType::MESH:
    case tesseract_geometry::GeometryType::SDF_MESH:
    {
      geometry_item = new PolygonMeshStandardItem(std::static_pointer_cast<tesseract_geometry::PolygonMesh>(collision->geometry));
      break;
    }
  }
  setIcon(geometry_item->icon());
  appendRow(geometry_item);
}
}
