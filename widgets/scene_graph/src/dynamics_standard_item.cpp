#include <tesseract_gui/widgets/scene_graph/dynamics_standard_item.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, DYNAMICS_ICON, (":/tesseract_gui/png/dynamics.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, NUMERIC_ICON, (":/tesseract_gui/png/numeric.png"));

namespace tesseract_gui
{
DynamicsStandardItem::DynamicsStandardItem(tesseract_scene_graph::JointDynamics::Ptr dynamics)
  : QStandardItem(*DYNAMICS_ICON(), "Dynamics")
  , dynamics(std::move(dynamics))
{
  ctor();
}

DynamicsStandardItem::DynamicsStandardItem(const QString &text, tesseract_scene_graph::JointDynamics::Ptr dynamics)
  : QStandardItem(*DYNAMICS_ICON(), text)
  , dynamics(std::move(dynamics))
{
  ctor();
}
DynamicsStandardItem::DynamicsStandardItem(const QIcon &icon, const QString &text, tesseract_scene_graph::JointDynamics::Ptr dynamics)
  : QStandardItem(icon, text)
  , dynamics(std::move(dynamics))
{
  ctor();
}

int DynamicsStandardItem::type() const
{
  return static_cast<int>(StandardItemType::DYNAMICS);
}

void DynamicsStandardItem::ctor()
{
  {
    auto* item = new QStandardItem(*NUMERIC_ICON(), "damping");
    auto* value = new QStandardItem(QString("%1").arg(dynamics->damping));
    appendRow({item, value});
  }

  {
    auto* item = new QStandardItem(*NUMERIC_ICON(), "friction");
    auto* value = new QStandardItem(QString("%1").arg(dynamics->friction));
    appendRow({item, value});
  }
}
}
