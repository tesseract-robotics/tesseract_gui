
#include <tesseract_widgets/environment/commands/add_kinematics_information_command_standard_item.h>
#include <tesseract_widgets/kinematic_groups/kinematics_info_standard_item.h>
#include <tesseract_widgets/common/standard_item_type.h>

namespace tesseract_gui
{

AddKinematicsInformationCommandStandardItem::AddKinematicsInformationCommandStandardItem(tesseract_environment::AddKinematicsInformationCommand::ConstPtr command)
  : QStandardItem(QIcon(":/tesseract_widgets/png/merge.png"), "Add Kinematics Information")
  , command(std::move(command))
{
  ctor();
}

AddKinematicsInformationCommandStandardItem::AddKinematicsInformationCommandStandardItem(const QString &text, tesseract_environment::AddKinematicsInformationCommand::ConstPtr command)
  : QStandardItem(QIcon(":/tesseract_widgets/png/merge.png"), text)
  , command(std::move(command))
{
  ctor();
}

AddKinematicsInformationCommandStandardItem::AddKinematicsInformationCommandStandardItem(const QIcon &icon, const QString &text, tesseract_environment::AddKinematicsInformationCommand::ConstPtr command)
  : QStandardItem(icon, text)
  , command(std::move(command))
{
  ctor();
}

int AddKinematicsInformationCommandStandardItem::type() const
{
  return static_cast<int>(StandardItemType::ENV_COMMAND_ADD_KINEMATICS_INFORMATION);
}

void AddKinematicsInformationCommandStandardItem::ctor()
{
  auto* kin_info = new KinematicsInfoStandardItem(command->getKinematicsInformation());
  appendRow(kin_info);
}
}
