#include <tesseract_gui/scene_graph/collision_standard_item.h>
#include <tesseract_gui/scene_graph/origin_standard_item.h>
#include <tesseract_gui/scene_graph/box_standard_item.h>
#include <tesseract_gui/scene_graph/capsule_standard_item.h>
#include <tesseract_gui/scene_graph/cone_standard_item.h>
#include <tesseract_gui/scene_graph/cylinder_standard_item.h>
#include <tesseract_gui/scene_graph/plane_standard_item.h>
#include <tesseract_gui/scene_graph/sphere_standard_item.h>
#include <tesseract_gui/scene_graph/polygon_mesh_standard_item.h>
#include <tesseract_gui/scene_graph/octree_standard_item.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, COLLISION_ICON, (":/tesseract_gui/ignition/collision.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, TEXT_ICON, (":/tesseract_gui/png/text.png"));

namespace tesseract_gui
{
CollisionStandardItem::CollisionStandardItem(tesseract_scene_graph::Collision::Ptr collision)
  : QStandardItem(*COLLISION_ICON(), "Collision")
  , collision(std::move(collision))
{
  ctor();
}

CollisionStandardItem::CollisionStandardItem(const QString &text, tesseract_scene_graph::Collision::Ptr collision)
  : QStandardItem(*COLLISION_ICON(),text)
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
  auto* name_item = new QStandardItem(*TEXT_ICON(), "name");
  auto* name_value = new QStandardItem(QString::fromStdString(collision->name));
  appendRow({name_item, name_value});

  auto* origin_item = new OriginStandardItem(collision->origin);
  appendRow(origin_item);

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
