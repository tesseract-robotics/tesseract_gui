#include <tesseract_widgets/scene_graph/polygon_mesh_standard_item.h>
#include <tesseract_widgets/common/standard_item_utils.h>
#include <tesseract_widgets/common/standard_item_type.h>
#include <tesseract_geometry/impl/convex_mesh.h>

namespace tesseract_gui
{
PolygonMeshStandardItem::PolygonMeshStandardItem(tesseract_geometry::PolygonMesh::Ptr mesh)
  : QStandardItem(QIcon(":/tesseract_widgets/png/mesh.png"), "PolygonMesh")
  , mesh(std::move(mesh))
{
  ctor();
}

PolygonMeshStandardItem::PolygonMeshStandardItem(const QString &text, tesseract_geometry::PolygonMesh::Ptr mesh)
  : QStandardItem(QIcon(":/tesseract_widgets/png/mesh.png"), text)
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
    setIcon(QIcon(":/tesseract_widgets/png/mesh.png"));
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
    setIcon(QIcon(":/tesseract_widgets/png/surface.png"));
    setText("Mesh");
  }
  else if (mesh->getType() == tesseract_geometry::GeometryType::SDF_MESH)
  {
    setIcon(QIcon(":/tesseract_widgets/png/surface.png"));
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

