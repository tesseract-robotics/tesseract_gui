#include <tesseract_gui/widgets/scene_graph/limits_standard_item.h>
#include <tesseract_gui/widgets/common/standard_item_utils.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, LIMITS_ICON, (":/tesseract_gui/png/limits.png"));

namespace tesseract_gui
{
LimitsStandardItem::LimitsStandardItem(tesseract_scene_graph::JointLimits::Ptr limits)
  : QStandardItem(*LIMITS_ICON(), "Limits")
  , limits(std::move(limits))
{
  ctor();
}

LimitsStandardItem::LimitsStandardItem(const QString &text, tesseract_scene_graph::JointLimits::Ptr limits)
  : QStandardItem(*LIMITS_ICON(), text)
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
