#include <tesseract_gui/widgets/scene_graph/joint_standard_item.h>
#include <tesseract_gui/widgets/scene_graph/dynamics_standard_item.h>
#include <tesseract_gui/widgets/scene_graph/limits_standard_item.h>
#include <tesseract_gui/widgets/scene_graph/safety_standard_item.h>
#include <tesseract_gui/widgets/scene_graph/calibration_standard_item.h>
#include <tesseract_gui/widgets/scene_graph/mimic_standard_item.h>
#include <tesseract_gui/widgets/common/transform_standard_item.h>
#include <tesseract_gui/widgets/common/standard_item_utils.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, JOINT_ICON, (":/tesseract_gui/ignition/joint.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, AXIS_ICON, (":/tesseract_gui/png/axis.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, FIXED_ICON, (":/tesseract_gui/png/anchor.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, REVOLUTE_ICON, (":/tesseract_gui/png/revolute.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, CONTINUOUS_ICON, (":/tesseract_gui/png/continuous.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, PRISMATIC_ICON, (":/tesseract_gui/png/prismatic.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, UNKNOWN_ICON, (":/tesseract_gui/png/unknown.png"));

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, FIXED_JOINT_ICON, (":/tesseract_gui/ignition/joint_fixed.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, REVOLUTE_JOINT_ICON, (":/tesseract_gui/ignition/joint_revolute.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, CONTINUOUS_JOINT_ICON, (":/tesseract_gui/ignition/joint_continuous.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, PRISMATIC_JOINT_ICON, (":/tesseract_gui/ignition/joint_prismatic.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, UNKNOWN_JOINT_ICON, (":/tesseract_gui/ignition/joint_unknown.png"));

namespace tesseract_gui
{
JointStandardItem::JointStandardItem(tesseract_scene_graph::Joint::Ptr joint)
  : QStandardItem(*JOINT_ICON(), "Joint")
  , joint(std::move(joint))
{
  ctor();
}

JointStandardItem::JointStandardItem(const QString &text, tesseract_scene_graph::Joint::Ptr joint)
  : QStandardItem(*JOINT_ICON(), text)
  , joint(std::move(joint))
{
  ctor();
}

JointStandardItem::JointStandardItem(const QIcon &icon, const QString &text, tesseract_scene_graph::Joint::Ptr joint)
  : QStandardItem(icon, text)
  , joint(std::move(joint))
{
  ctor();
}

int JointStandardItem::type() const
{
  return static_cast<int>(StandardItemType::JOINT);
}

void JointStandardItem::ctor()
{
  appendRow(createStandardItemString("name", joint->getName()));

  {
    QStandardItem* item;
    if (joint->type == tesseract_scene_graph::JointType::FIXED)
    {
      item = new QStandardItem(*FIXED_ICON(), "type");
      setIcon(*FIXED_JOINT_ICON());
    }
    else if (joint->type == tesseract_scene_graph::JointType::REVOLUTE)
    {
      item = new QStandardItem(*REVOLUTE_ICON(), "type");
      setIcon(*REVOLUTE_JOINT_ICON());
    }
    else if (joint->type == tesseract_scene_graph::JointType::CONTINUOUS)
    {
      item = new QStandardItem(*CONTINUOUS_ICON(), "type");
      setIcon(*CONTINUOUS_JOINT_ICON());
    }
    else if (joint->type == tesseract_scene_graph::JointType::PRISMATIC)
    {
      item = new QStandardItem(*PRISMATIC_ICON(), "type");
      setIcon(*PRISMATIC_JOINT_ICON());
    }
    else
    {
      item = new QStandardItem(*UNKNOWN_ICON(), "type");
      setIcon(*UNKNOWN_JOINT_ICON());
    }

    std::stringstream ss;
    ss << joint->type;
    auto* value = new QStandardItem(QString::fromStdString(ss.str()));
    appendRow({item, value});
  }

  if (joint->type == tesseract_scene_graph::JointType::REVOLUTE ||
      joint->type == tesseract_scene_graph::JointType::PRISMATIC ||
      joint->type == tesseract_scene_graph::JointType::PLANAR)
  {
    auto* item = new QStandardItem(*AXIS_ICON(), "axis");
    item->setColumnCount(2);

    item->appendRow(createStandardItemFloat("x", joint->axis.x()));
    item->appendRow(createStandardItemFloat("y", joint->axis.y()));
    item->appendRow(createStandardItemFloat("z", joint->axis.z()));

    appendRow(item);
  }

  appendRow(createStandardItemString("child_link_name", joint->child_link_name));
  appendRow(createStandardItemString("parent_link_name", joint->parent_link_name));
  appendRow(new TransformStandardItem(joint->parent_to_joint_origin_transform));

  if (joint->dynamics != nullptr)
    appendRow(new DynamicsStandardItem(joint->dynamics));

  if (joint->limits != nullptr)
    appendRow(new LimitsStandardItem(joint->limits));

  if (joint->safety != nullptr)
    appendRow(new SafetyStandardItem(joint->safety));

  if (joint->calibration != nullptr)
    appendRow(new CalibrationStandardItem(joint->calibration));

  if (joint->mimic != nullptr)
    appendRow(new MimicStandardItem(joint->mimic));

}
}
