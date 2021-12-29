#include <tesseract_gui/scene_graph/limits_standard_item.h>
#include <tesseract_gui/scene_graph/types.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, LIMITS_ICON, (":/tesseract_gui/png/limits.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, NUMERIC_ICON, (":/tesseract_gui/png/numeric.png"));

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
  return static_cast<int>(SceneGraphItemType::LIMITS);
}

void LimitsStandardItem::ctor()
{
  {
    auto* item = new QStandardItem(*NUMERIC_ICON(), "lower");
    auto* value = new QStandardItem(QString("%1").arg(limits->lower));
    appendRow({item, value});
  }

  {
    auto* item = new QStandardItem(*NUMERIC_ICON(), "upper");
    auto* value = new QStandardItem(QString("%1").arg(limits->upper));
    appendRow({item, value});
  }

  {
    auto* item = new QStandardItem(*NUMERIC_ICON(), "effort");
    auto* value = new QStandardItem(QString("%1").arg(limits->effort));
    appendRow({item, value});
  }

  {
    auto* item = new QStandardItem(*NUMERIC_ICON(), "velocity");
    auto* value = new QStandardItem(QString("%1").arg(limits->velocity));
    appendRow({item, value});
  }

  {
    auto* item = new QStandardItem(*NUMERIC_ICON(), "acceleration");
    auto* value = new QStandardItem(QString("%1").arg(limits->acceleration));
    appendRow({item, value});
  }
}
}
