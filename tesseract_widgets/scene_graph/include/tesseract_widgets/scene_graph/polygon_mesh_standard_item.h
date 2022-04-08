#ifndef TESSERACT_WIDGETS_SCENE_GRAPH_POLYGON_MESH_STANDARD_ITEM_H
#define TESSERACT_WIDGETS_SCENE_GRAPH_POLYGON_MESH_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_geometry/impl/polygon_mesh.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class PolygonMeshStandardItem : public QStandardItem
{
public:
  PolygonMeshStandardItem(tesseract_geometry::PolygonMesh::Ptr mesh);
  explicit PolygonMeshStandardItem(const QString &text, tesseract_geometry::PolygonMesh::Ptr mesh);
  PolygonMeshStandardItem(const QIcon &icon, const QString &text, tesseract_geometry::PolygonMesh::Ptr mesh);
  int type() const override;

  tesseract_geometry::PolygonMesh::Ptr mesh;

private:
  void ctor();
};
}

#endif // TESSERACT_WIDGETS_SCENE_GRAPH_POLYGON_MESH_STANDARD_ITEM_H
