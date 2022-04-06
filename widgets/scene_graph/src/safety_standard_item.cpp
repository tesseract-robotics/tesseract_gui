#include <tesseract_gui/widgets/scene_graph/safety_standard_item.h>
#include <tesseract_gui/widgets/common/standard_item_utils.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, SAFETY_ICON, (":/tesseract_gui/png/safety.png"));

namespace tesseract_gui
{
SafetyStandardItem::SafetyStandardItem(tesseract_scene_graph::JointSafety::Ptr safety)
  : QStandardItem(*SAFETY_ICON(), "Safety")
  , safety(std::move(safety))
{
  ctor();
}

SafetyStandardItem::SafetyStandardItem(const QString &text, tesseract_scene_graph::JointSafety::Ptr safety)
  : QStandardItem(*SAFETY_ICON(), text)
  , safety(std::move(safety))
{
  ctor();
}

SafetyStandardItem::SafetyStandardItem(const QIcon &icon, const QString &text, tesseract_scene_graph::JointSafety::Ptr safety)
  : QStandardItem(icon, text)
  , safety(std::move(safety))
{
  ctor();
}

int SafetyStandardItem::type() const
{
  return static_cast<int>(StandardItemType::SAFETY);
}

void SafetyStandardItem::ctor()
{
  appendRow(createStandardItemFloat("soft_upper_limit", safety->soft_upper_limit));
  appendRow(createStandardItemFloat("soft_lower_limit", safety->soft_lower_limit));
  appendRow(createStandardItemFloat("k_position", safety->k_position));
  appendRow(createStandardItemFloat("k_velocity", safety->k_velocity));
}
}
