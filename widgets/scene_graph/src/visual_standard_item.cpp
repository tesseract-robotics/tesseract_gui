#include <tesseract_gui/widgets/scene_graph/visual_standard_item.h>
#include <tesseract_gui/widgets/scene_graph/material_standard_item.h>
#include <tesseract_gui/widgets/scene_graph/box_standard_item.h>
#include <tesseract_gui/widgets/scene_graph/capsule_standard_item.h>
#include <tesseract_gui/widgets/scene_graph/cone_standard_item.h>
#include <tesseract_gui/widgets/scene_graph/cylinder_standard_item.h>
#include <tesseract_gui/widgets/scene_graph/plane_standard_item.h>
#include <tesseract_gui/widgets/scene_graph/sphere_standard_item.h>
#include <tesseract_gui/widgets/scene_graph/polygon_mesh_standard_item.h>
#include <tesseract_gui/widgets/scene_graph/octree_standard_item.h>
#include <tesseract_gui/widgets/common/origin_standard_item.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, VISUAL_ICON, (":/tesseract_gui/ignition/visual.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, TEXT_ICON, (":/tesseract_gui/png/text.png"));

namespace tesseract_gui
{
VisualStandardItem::VisualStandardItem(tesseract_scene_graph::Visual::Ptr visual)
  : QStandardItem(*VISUAL_ICON(), "Visual")
  , visual(std::move(visual))
{
  ctor();
}

VisualStandardItem::VisualStandardItem(const QString &text, tesseract_scene_graph::Visual::Ptr visual)
  : QStandardItem(*VISUAL_ICON(), text)
  , visual(std::move(visual))
{
  ctor();
}

VisualStandardItem::VisualStandardItem(const QIcon &icon, const QString &text, tesseract_scene_graph::Visual::Ptr visual)
  : QStandardItem(icon, text)
  , visual(std::move(visual))
{
  ctor();
}

int VisualStandardItem::type() const
{
  return static_cast<int>(StandardItemType::VISUAL);
}

void VisualStandardItem::ctor()
{
  auto* name_item = new QStandardItem(*TEXT_ICON(), "name");
  auto* name_value = new QStandardItem(QString::fromStdString(visual->name));
  appendRow({name_item, name_value});

  auto* origin_item = new OriginStandardItem(visual->origin);
  appendRow(origin_item);

  auto* material_item = new MaterialStandardItem(visual->material);
  appendRow(material_item);

  QStandardItem* geometry_item;
  switch (visual->geometry->getType())
  {
    case tesseract_geometry::GeometryType::BOX:
    {
      geometry_item = new BoxStandardItem(std::static_pointer_cast<tesseract_geometry::Box>(visual->geometry));
      break;
    }
    case tesseract_geometry::GeometryType::CAPSULE:
    {
      geometry_item = new CapsuleStandardItem(std::static_pointer_cast<tesseract_geometry::Capsule>(visual->geometry));
      break;
    }
    case tesseract_geometry::GeometryType::CONE:
    {
      geometry_item = new ConeStandardItem(std::static_pointer_cast<tesseract_geometry::Cone>(visual->geometry));
      break;
    }
    case tesseract_geometry::GeometryType::CYLINDER:
    {
      geometry_item = new CylinderStandardItem(std::static_pointer_cast<tesseract_geometry::Cylinder>(visual->geometry));
      break;
    }
    case tesseract_geometry::GeometryType::PLANE:
    {
      geometry_item = new PlaneStandardItem(std::static_pointer_cast<tesseract_geometry::Plane>(visual->geometry));
      break;
    }
    case tesseract_geometry::GeometryType::SPHERE:
    {
      geometry_item = new SphereStandardItem(std::static_pointer_cast<tesseract_geometry::Sphere>(visual->geometry));
      break;
    }
    case tesseract_geometry::GeometryType::OCTREE:
    {
      geometry_item = new OctreeStandardItem(std::static_pointer_cast<tesseract_geometry::Octree>(visual->geometry));
      break;
    }
    case tesseract_geometry::GeometryType::CONVEX_MESH:
    case tesseract_geometry::GeometryType::MESH:
    case tesseract_geometry::GeometryType::SDF_MESH:
    {
      geometry_item = new PolygonMeshStandardItem(std::static_pointer_cast<tesseract_geometry::PolygonMesh>(visual->geometry));
      break;
    }
  }
  setIcon(geometry_item->icon());
  appendRow(geometry_item);
}
}
