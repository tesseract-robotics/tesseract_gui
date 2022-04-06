#include <tesseract_gui/widgets/scene_graph/mimic_standard_item.h>
#include <tesseract_gui/widgets/common/standard_item_utils.h>
#include <tesseract_gui/common/standard_item_type.h>

namespace tesseract_gui
{
MimicStandardItem::MimicStandardItem(tesseract_scene_graph::JointMimic::Ptr mimic)
  : QStandardItem("Mimic")
  , mimic(std::move(mimic))
{
  ctor();
}

MimicStandardItem::MimicStandardItem(const QString &text, tesseract_scene_graph::JointMimic::Ptr mimic)
  : QStandardItem(text)
  , mimic(std::move(mimic))
{
  ctor();
}

MimicStandardItem::MimicStandardItem(const QIcon &icon, const QString &text, tesseract_scene_graph::JointMimic::Ptr mimic)
  : QStandardItem(icon, text)
  , mimic(std::move(mimic))
{
  ctor();
}

int MimicStandardItem::type() const
{
  return static_cast<int>(StandardItemType::MIMIC);
}

void MimicStandardItem::ctor()
{
  appendRow(createStandardItemFloat("offset", mimic->offset));
  appendRow(createStandardItemFloat("multiplier", mimic->multiplier));
  appendRow(createStandardItemString("joint_name", mimic->joint_name));
}
}
