#ifndef TESSERACT_WIDGETS_SCENE_GRAPH_MATERIAL_STANDARD_ITEM_H
#define TESSERACT_WIDGETS_SCENE_GRAPH_MATERIAL_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_scene_graph/link.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class MaterialStandardItem : public QStandardItem
{
public:
  MaterialStandardItem(tesseract_scene_graph::Material::Ptr material);
  explicit MaterialStandardItem(const QString &text, tesseract_scene_graph::Material::Ptr material);
  MaterialStandardItem(const QIcon &icon, const QString &text, tesseract_scene_graph::Material::Ptr material);
  int type() const override;

  tesseract_scene_graph::Material::Ptr material;

private:
  void ctor();
};
}

#endif // TESSERACT_WIDGETS_SCENE_GRAPH_MATERIAL_STANDARD_ITEM_H
