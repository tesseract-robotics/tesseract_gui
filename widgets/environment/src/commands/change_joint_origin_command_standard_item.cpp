
#include <tesseract_gui/widgets/environment/commands/change_joint_origin_command_standard_item.h>
#include <tesseract_gui/widgets/common/transform_standard_item.h>
#include <tesseract_gui/common/standard_item_type.h>

namespace tesseract_gui
{

ChangeJointOriginCommandStandardItem::ChangeJointOriginCommandStandardItem(tesseract_environment::ChangeJointOriginCommand::ConstPtr command)
  : QStandardItem(QIcon(":/tesseract_gui/png/merge.png"), "Change Joint Origin")
  , command(std::move(command))
{
  ctor();
}

ChangeJointOriginCommandStandardItem::ChangeJointOriginCommandStandardItem(const QString &text, tesseract_environment::ChangeJointOriginCommand::ConstPtr command)
  : QStandardItem(QIcon(":/tesseract_gui/png/merge.png"), text)
  , command(std::move(command))
{
  ctor();
}

ChangeJointOriginCommandStandardItem::ChangeJointOriginCommandStandardItem(const QIcon &icon, const QString &text, tesseract_environment::ChangeJointOriginCommand::ConstPtr command)
  : QStandardItem(icon, text)
  , command(std::move(command))
{
  ctor();
}

int ChangeJointOriginCommandStandardItem::type() const
{
  return static_cast<int>(StandardItemType::ENV_COMMAND_CHANGE_JOINT_ORIGIN);
}

void ChangeJointOriginCommandStandardItem::ctor()
{
  auto* item = new TransformStandardItem(QIcon(":/tesseract_gui/ignition/joint.png"), command->getJointName().c_str(), command->getOrigin());
  appendRow({item, new QStandardItem()});
}
}
