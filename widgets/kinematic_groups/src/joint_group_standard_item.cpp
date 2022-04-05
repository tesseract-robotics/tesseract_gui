
#include <tesseract_gui/widgets/kinematic_groups/joint_group_standard_item.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, ROBOT_ICON, (":/tesseract_gui/png/robotic-arm.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, TEXT_ICON, (":/tesseract_gui/png/text.png"));

namespace tesseract_gui
{

JointGroupStandardItem::JointGroupStandardItem(tesseract_srdf::JointGroup group)
  : QStandardItem(*ROBOT_ICON(), "Joint Group")
  , group(std::move(group))
{
  ctor();
}

JointGroupStandardItem::JointGroupStandardItem(const QString &text, tesseract_srdf::JointGroup group)
  : QStandardItem(*ROBOT_ICON(), text)
  , group(std::move(group))
{
  ctor();
}

JointGroupStandardItem::JointGroupStandardItem(const QIcon &icon, const QString &text, tesseract_srdf::JointGroup group)
  : QStandardItem(icon, text)
  , group(std::move(group))
{
  ctor();
}

int JointGroupStandardItem::type() const
{
  return static_cast<int>(StandardItemType::JOINT_GROUP);
}

void JointGroupStandardItem::ctor()
{
  for (const auto& joint : group)
  {
    auto* name = new QStandardItem(*TEXT_ICON(), "joint");
    auto* value = new QStandardItem(QString::fromStdString(joint));
    appendRow({name, value});
  }
}
}

