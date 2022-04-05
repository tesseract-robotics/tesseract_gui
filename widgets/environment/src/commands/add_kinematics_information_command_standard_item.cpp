
#include <tesseract_gui/widgets/environment/commands/add_kinematics_information_command_standard_item.h>
#include <tesseract_gui/widgets/kinematic_groups/kinematics_info_standard_item.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, COMMAND_ICON, (":/tesseract_gui/png/merge.png"));

namespace tesseract_gui
{

AddKinematicsInformationCommandStandardItem::AddKinematicsInformationCommandStandardItem(tesseract_environment::AddKinematicsInformationCommand::ConstPtr command)
  : QStandardItem(*COMMAND_ICON(), "Add Kinematics Information")
  , command(std::move(command))
{
  ctor();
}

AddKinematicsInformationCommandStandardItem::AddKinematicsInformationCommandStandardItem(const QString &text, tesseract_environment::AddKinematicsInformationCommand::ConstPtr command)
  : QStandardItem(*COMMAND_ICON(), text)
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
