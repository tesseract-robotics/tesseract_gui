#include <tesseract_gui/widgets/scene_graph/polygon_mesh_standard_item.h>
#include <tesseract_gui/widgets/common/standard_item_utils.h>
#include <tesseract_gui/common/standard_item_type.h>
#include <tesseract_geometry/impl/convex_mesh.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, POLYGON_MESH_ICON, (":/tesseract_gui/png/mesh.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, MESH_ICON, (":/tesseract_gui/png/surface.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, CONVEX_MESH_ICON, (":/tesseract_gui/png/mesh.png"));

namespace tesseract_gui
{
PolygonMeshStandardItem::PolygonMeshStandardItem(tesseract_geometry::PolygonMesh::Ptr mesh)
  : QStandardItem(*POLYGON_MESH_ICON(), "PolygonMesh")
  , mesh(std::move(mesh))
{
  ctor();
}

PolygonMeshStandardItem::PolygonMeshStandardItem(const QString &text, tesseract_geometry::PolygonMesh::Ptr mesh)
  : QStandardItem(*POLYGON_MESH_ICON(), text)
  , mesh(std::move(mesh))
{
  ctor();
}

PolygonMeshStandardItem::PolygonMeshStandardItem(const QIcon &icon, const QString &text, tesseract_geometry::PolygonMesh::Ptr mesh)
  : QStandardItem(icon, text)
  , mesh(std::move(mesh))
{
  ctor();
}

int PolygonMeshStandardItem::type() const
{
  return static_cast<int>(StandardItemType::POLYGON_MESH);
}

void PolygonMeshStandardItem::ctor()
{
  if (mesh->getType() == tesseract_geometry::GeometryType::CONVEX_MESH)
  {
    setIcon(*CONVEX_MESH_ICON());
    setText("Convex Mesh");

    std::string method {"Default"};
    auto convex_mesh = std::static_pointer_cast<tesseract_geometry::ConvexMesh>(mesh);
    if (convex_mesh->getCreationMethod() == tesseract_geometry::ConvexMesh::MESH)
      method = "Mesh";
    else if (convex_mesh->getCreationMethod() == tesseract_geometry::ConvexMesh::CONVERTED)
      method = "Converted";

    appendRow(createStandardItemString("creation method", method));
  }
  else if (mesh->getType() == tesseract_geometry::GeometryType::MESH)
  {
    setIcon(*MESH_ICON());
    setText("Mesh");
  }
  else if (mesh->getType() == tesseract_geometry::GeometryType::SDF_MESH)
  {
    setIcon(*MESH_ICON());
    setText("SDF Mesh");
  }

  std::string url;
  if (mesh->getResource() != nullptr)
    url = mesh->getResource()->getUrl();

  appendRow(createStandardItemURL("resource", url));
  appendRow(createStandardItemInt("vertex count", mesh->getVertexCount()));
  appendRow(createStandardItemInt("face count", mesh->getFaceCount()));
}
}

