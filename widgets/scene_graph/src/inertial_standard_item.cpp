#include <tesseract_gui/widgets/scene_graph/inertial_standard_item.h>
#include <tesseract_gui/widgets/common/transform_standard_item.h>
#include <tesseract_gui/widgets/common/standard_item_utils.h>
#include <tesseract_gui/common/standard_item_type.h>

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

  appendRow(createStandardItemFloat("mass", inertial->mass));
  appendRow(createStandardItemFloat("ixx", inertial->ixx));
  appendRow(createStandardItemFloat("ixy", inertial->ixy));
  appendRow(createStandardItemFloat("ixz", inertial->ixz));
  appendRow(createStandardItemFloat("iyy", inertial->iyy));
  appendRow(createStandardItemFloat("iyz", inertial->iyz));
  appendRow(createStandardItemFloat("izz", inertial->izz));
}
}
