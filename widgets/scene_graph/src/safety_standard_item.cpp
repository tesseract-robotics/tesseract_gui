#include <tesseract_gui/scene_graph/safety_standard_item.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, SAFETY_ICON, (":/tesseract_gui/png/safety.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, NUMERIC_ICON, (":/tesseract_gui/png/numeric.png"));

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
  {
    auto* item = new QStandardItem(*NUMERIC_ICON(), "soft_upper_limit");
    auto* value = new QStandardItem(QString("%1").arg(safety->soft_upper_limit));
    appendRow({item, value});
  }

  {
    auto* item = new QStandardItem(*NUMERIC_ICON(), "soft_lower_limit");
    auto* value = new QStandardItem(QString("%1").arg(safety->soft_lower_limit));
    appendRow({item, value});
  }

  {
    auto* item = new QStandardItem(*NUMERIC_ICON(), "k_position");
    auto* value = new QStandardItem(QString("%1").arg(safety->k_position));
    appendRow({item, value});
  }

  {
    auto* item = new QStandardItem(*NUMERIC_ICON(), "k_velocity");
    auto* value = new QStandardItem(QString("%1").arg(safety->k_velocity));
    appendRow({item, value});
  }
}
}
