#include <tesseract_gui/widgets/scene_graph/material_standard_item.h>
#include <tesseract_gui/widgets/common/standard_item_utils.h>
#include <tesseract_gui/widgets/common/rgba_standard_item.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, MATERIAL_ICON, (":/tesseract_gui/png/color.png"));

namespace tesseract_gui
{
MaterialStandardItem::MaterialStandardItem(tesseract_scene_graph::Material::Ptr material)
  : QStandardItem(*MATERIAL_ICON(), "Material")
  , material(std::move(material))
{
  ctor();
}

MaterialStandardItem::MaterialStandardItem(const QString &text, tesseract_scene_graph::Material::Ptr material)
  : QStandardItem(*MATERIAL_ICON(), text)
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
