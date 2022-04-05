#include <tesseract_gui/widgets/scene_graph/inertial_standard_item.h>
#include <tesseract_gui/widgets/common/transform_standard_item.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, NUMERIC_ICON, (":/tesseract_gui/png/numeric.png"));

namespace tesseract_gui
{
InertialStandardItem::InertialStandardItem(tesseract_scene_graph::Inertial::Ptr inertial)
  : QStandardItem("Inertial")
  , inertial(std::move(inertial))
{
  ctor();
}

InertialStandardItem::InertialStandardItem(const QString &text, tesseract_scene_graph::Inertial::Ptr inertial)
  : QStandardItem(text)
  , inertial(std::move(inertial))
{
  ctor();
}
InertialStandardItem::InertialStandardItem(const QIcon &icon, const QString &text, tesseract_scene_graph::Inertial::Ptr inertial)
  : QStandardItem(icon, text)
  , inertial(std::move(inertial))
{
  ctor();
}

int InertialStandardItem::type() const
{
  return static_cast<int>(StandardItemType::INERTIAL);
}

void InertialStandardItem::ctor()
{
  auto* origin_item = new TransformStandardItem(inertial->origin);
  appendRow(origin_item);

  auto* mass_name = new QStandardItem(*NUMERIC_ICON(), "mass");
  auto* mass_value = new QStandardItem(QString("%1").arg(inertial->mass));
  appendRow({mass_name, mass_value});

  auto* ixx_name = new QStandardItem(*NUMERIC_ICON(), "ixx");
  auto* ixx_value = new QStandardItem(QString("%1").arg(inertial->ixx));
  appendRow({ixx_name, ixx_value});

  auto* ixy_name = new QStandardItem(*NUMERIC_ICON(), "ixy");
  auto* ixy_value = new QStandardItem(QString("%1").arg(inertial->ixy));
  appendRow({ixy_name, ixy_value});

  auto* ixz_name = new QStandardItem(*NUMERIC_ICON(), "ixz");
  auto* ixz_value = new QStandardItem(QString("%1").arg(inertial->ixz));
  appendRow({ixz_name, ixz_value});

  auto* iyy_name = new QStandardItem(*NUMERIC_ICON(), "iyy");
  auto* iyy_value = new QStandardItem(QString("%1").arg(inertial->iyy));
  appendRow({iyy_name, iyy_value});

  auto* iyz_name = new QStandardItem(*NUMERIC_ICON(), "iyz");
  auto* iyz_value = new QStandardItem(QString("%1").arg(inertial->iyz));
  appendRow({iyz_name, iyz_value});

  auto* izz_name = new QStandardItem(*NUMERIC_ICON(), "izz");
  auto* izz_value = new QStandardItem(QString("%1").arg(inertial->izz));
  appendRow({izz_name, izz_value});
}
}
