#include <tesseract_gui/scene_graph/material_standard_item.h>
#include <tesseract_gui/scene_graph/types.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, MATERIAL_ICON, (":/tesseract_gui/png/color.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, COLOR_ICON, (":/tesseract_gui/png/rgb.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, NUMERIC_ICON, (":/tesseract_gui/png/numeric.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, TEXT_ICON, (":/tesseract_gui/png/text.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, URL_ICON, (":/tesseract_gui/png/url.png"));

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
  return static_cast<int>(SceneGraphItemType::MATERIAL);
}

void MaterialStandardItem::ctor()
{
  auto* name_item = new QStandardItem(*TEXT_ICON(), "name");
  auto* name_value = new QStandardItem(QString::fromStdString(material->getName()));
  appendRow({name_item, name_value});

  auto* texture_item = new QStandardItem(*URL_ICON(), "texture");
  auto* texture_value = new QStandardItem(QString::fromStdString(material->texture_filename));
  appendRow({texture_item, texture_value});

  auto* color_item = new QStandardItem(*COLOR_ICON(), "color");
  auto* color_value = new QStandardItem(QString("{r: %1, g: %2, b: %3, a: %4}").arg(material->color(0)).arg(material->color(1)).arg(material->color(2)).arg(material->color(3)));
  color_item->setColumnCount(2);
  auto* r_name = new QStandardItem(*NUMERIC_ICON(), "r");
  auto* r_value = new QStandardItem(QString("%1").arg(material->color(0)));
  color_item->appendRow({r_name, r_value});

  auto* g_name = new QStandardItem(*NUMERIC_ICON(), "g");
  auto* g_value = new QStandardItem(QString("%1").arg(material->color(1)));
  color_item->appendRow({g_name, g_value});

  auto* b_name = new QStandardItem(*NUMERIC_ICON(), "b");
  auto* b_value = new QStandardItem(QString("%1").arg(material->color(2)));
  color_item->appendRow({b_name, b_value});

  auto* a_name = new QStandardItem(*NUMERIC_ICON(), "a");
  auto* a_value = new QStandardItem(QString("%1").arg(material->color(3)));
  color_item->appendRow({a_name, a_value});
  appendRow({color_item, color_value});
}
}
