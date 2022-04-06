#include <tesseract_gui/widgets/scene_graph/dynamics_standard_item.h>
#include <tesseract_gui/widgets/common/standard_item_utils.h>
#include <tesseract_gui/common/standard_item_type.h>

namespace tesseract_gui
{
DynamicsStandardItem::DynamicsStandardItem(tesseract_scene_graph::JointDynamics::Ptr dynamics)
  : QStandardItem(QIcon(":/tesseract_gui/png/dynamics.png"), "Dynamics")
  , dynamics(std::move(dynamics))
{
  ctor();
}

DynamicsStandardItem::DynamicsStandardItem(const QString &text, tesseract_scene_graph::JointDynamics::Ptr dynamics)
  : QStandardItem(QIcon(":/tesseract_gui/png/dynamics.png"), text)
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
  appendRow(createStandardItemFloat("damping", dynamics->damping));
  appendRow(createStandardItemFloat("friction", dynamics->friction));
}
}
