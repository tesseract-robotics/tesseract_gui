#include <tesseract_gui/scene_graph/polygon_mesh_standard_item.h>
#include <tesseract_geometry/impl/convex_mesh.h>
#include <tesseract_gui/scene_graph/types.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, POLYGON_MESH_ICON, (":/tesseract_gui/png/mesh.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, MESH_ICON, (":/tesseract_gui/png/surface.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, CONVEX_MESH_ICON, (":/tesseract_gui/png/mesh.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, NUMERIC_ICON, (":/tesseract_gui/png/numeric.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, URL_ICON, (":/tesseract_gui/png/url.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, TEXT_ICON, (":/tesseract_gui/png/text.png"));

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
  return static_cast<int>(SceneGraphItemType::POLYGON_MESH);
}

void PolygonMeshStandardItem::ctor()
{
  if (mesh->getType() == tesseract_geometry::GeometryType::CONVEX_MESH)
  {
    setIcon(*CONVEX_MESH_ICON());
    setText("Convex Mesh");
    auto* name = new QStandardItem(*TEXT_ICON(), "creation method");

    std::string method {"Default"};
    auto convex_mesh = std::static_pointer_cast<tesseract_geometry::ConvexMesh>(mesh);
    if (convex_mesh->getCreationMethod() == tesseract_geometry::ConvexMesh::MESH)
      method = "Mesh";
    else if (convex_mesh->getCreationMethod() == tesseract_geometry::ConvexMesh::CONVERTED)
      method = "Converted";

    auto* value = new QStandardItem(QString::fromStdString(method));
    appendRow({name, value});
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


  {
    auto* name = new QStandardItem(*URL_ICON(), "resource");
    std::string url;
    if (mesh->getResource() != nullptr)
      url = mesh->getResource()->getUrl();

    auto* value = new QStandardItem(QString::fromStdString(url));
    appendRow({name, value});
  }

  {
    auto* name = new QStandardItem(*NUMERIC_ICON(), "vertex count");
    auto* value = new QStandardItem(QString("%1").arg(mesh->getVertexCount()));
    appendRow({name, value});
  }
  {
    auto* name = new QStandardItem(*NUMERIC_ICON(), "face count");
    auto* value = new QStandardItem(QString("%1").arg(mesh->getFaceCount()));
    appendRow({name, value});
  }
}
}

