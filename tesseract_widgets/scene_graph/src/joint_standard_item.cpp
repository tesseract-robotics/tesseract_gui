#include <tesseract_widgets/scene_graph/joint_standard_item.h>
#include <tesseract_widgets/scene_graph/dynamics_standard_item.h>
#include <tesseract_widgets/scene_graph/limits_standard_item.h>
#include <tesseract_widgets/scene_graph/safety_standard_item.h>
#include <tesseract_widgets/scene_graph/calibration_standard_item.h>
#include <tesseract_widgets/scene_graph/mimic_standard_item.h>
#include <tesseract_widgets/common/transform_standard_item.h>
#include <tesseract_widgets/common/standard_item_utils.h>
#include <tesseract_widgets/common/standard_item_type.h>

namespace tesseract_gui
{
JointStandardItem::JointStandardItem(tesseract_scene_graph::Joint::Ptr joint)
  : QStandardItem(QIcon(":/tesseract_widgets/ignition/joint.png"), "Joint")
  , joint(std::move(joint))
{
  ctor();
}

JointStandardItem::JointStandardItem(const QString &text, tesseract_scene_graph::Joint::Ptr joint)
  : QStandardItem(QIcon(":/tesseract_widgets/ignition/joint.png"), text)
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
      item = new QStandardItem(QIcon(":/tesseract_widgets/png/anchor.png"), "type");
      setIcon(QIcon(":/tesseract_widgets/ignition/joint_fixed.png"));
    }
    else if (joint->type == tesseract_scene_graph::JointType::REVOLUTE)
    {
      item = new QStandardItem(QIcon(":/tesseract_widgets/png/revolute.png"), "type");
      setIcon(QIcon(":/tesseract_widgets/ignition/joint_revolute.png"));
    }
    else if (joint->type == tesseract_scene_graph::JointType::CONTINUOUS)
    {
      item = new QStandardItem(QIcon(":/tesseract_widgets/png/continuous.png"), "type");
      setIcon(QIcon(":/tesseract_widgets/ignition/joint_continuous.png"));
    }
    else if (joint->type == tesseract_scene_graph::JointType::PRISMATIC)
    {
      item = new QStandardItem(QIcon(":/tesseract_widgets/png/prismatic.png"), "type");
      setIcon(QIcon(":/tesseract_widgets/ignition/joint_prismatic.png"));
    }
    else
    {
      item = new QStandardItem(QIcon(":/tesseract_widgets/png/unknown.png"), "type");
      setIcon(QIcon(":/tesseract_widgets/ignition/joint_unknown.png"));
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
    auto* item = new QStandardItem(QIcon(":/tesseract_widgets/png/axis.png"), "axis");
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
