
#include <tesseract_gui/widgets/kinematic_groups/joint_group_standard_item.h>
#include <tesseract_gui/widgets/common/standard_item_utils.h>
#include <tesseract_gui/common/standard_item_type.h>

namespace tesseract_gui
{

JointGroupStandardItem::JointGroupStandardItem(tesseract_srdf::JointGroup group)
  : QStandardItem(QIcon(":/tesseract_gui/png/robotic-arm.png"), "Joint Group")
  , group(std::move(group))
{
  ctor();
}

JointGroupStandardItem::JointGroupStandardItem(const QString &text, tesseract_srdf::JointGroup group)
  : QStandardItem(QIcon(":/tesseract_gui/png/robotic-arm.png"), text)
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
    appendRow(createStandardItemString("joint", joint));
}
}

