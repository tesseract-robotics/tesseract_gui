#include <tesseract_widgets/scene_graph/material_standard_item.h>
#include <tesseract_widgets/common/standard_item_utils.h>
#include <tesseract_widgets/common/rgba_standard_item.h>
#include <tesseract_widgets/common/standard_item_type.h>

namespace tesseract_gui
{
MaterialStandardItem::MaterialStandardItem(tesseract_scene_graph::Material::Ptr material)
  : QStandardItem(QIcon(":/tesseract_widgets/png/color.png"), "Material")
  , material(std::move(material))
{
  ctor();
}

MaterialStandardItem::MaterialStandardItem(const QString &text, tesseract_scene_graph::Material::Ptr material)
  : QStandardItem(QIcon(":/tesseract_widgets/png/color.png"), text)
  , material(std::move(material))
{
  ctor();
}

MaterialStandardItem::MaterialStandardItem(const QIcon &icon, const QString &text, tesseract_scene_graph::Material::Ptr material)
  : QStandardItem(icon, text)
  , material(std::move(material))
{
  ctor();
}

int MaterialStandardItem::type() const
{
  return static_cast<int>(StandardItemType::MATERIAL);
}

void MaterialStandardItem::ctor()
{
  appendRow(createStandardItemString("name", material->getName()));
  appendRow(createStandardItemURL("texture", material->texture_filename));
  appendRow(new RGBAStandardItem(material->color));
}
}