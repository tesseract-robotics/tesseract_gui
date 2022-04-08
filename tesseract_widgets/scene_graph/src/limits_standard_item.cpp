#include <tesseract_widgets/scene_graph/limits_standard_item.h>
#include <tesseract_widgets/common/standard_item_utils.h>
#include <tesseract_widgets/common/standard_item_type.h>

namespace tesseract_gui
{
LimitsStandardItem::LimitsStandardItem(tesseract_scene_graph::JointLimits::Ptr limits)
  : QStandardItem(QIcon(":/tesseract_widgets/png/limits.png"), "Limits")
  , limits(std::move(limits))
{
  ctor();
}

LimitsStandardItem::LimitsStandardItem(const QString &text, tesseract_scene_graph::JointLimits::Ptr limits)
  : QStandardItem(QIcon(":/tesseract_widgets/png/limits.png"), text)
  , limits(std::move(limits))
{
  ctor();
}

LimitsStandardItem::LimitsStandardItem(const QIcon &icon, const QString &text, tesseract_scene_graph::JointLimits::Ptr limits)
  : QStandardItem(icon, text)
  , limits(std::move(limits))
{
  ctor();
}

int LimitsStandardItem::type() const
{
  return static_cast<int>(StandardItemType::LIMITS);
}

void LimitsStandardItem::ctor()
{
  appendRow(createStandardItemFloat("lower", limits->lower));
  appendRow(createStandardItemFloat("upper", limits->upper));
  appendRow(createStandardItemFloat("effort", limits->effort));
  appendRow(createStandardItemFloat("velocity", limits->velocity));
  appendRow(createStandardItemFloat("acceleration", limits->acceleration));
}
}
