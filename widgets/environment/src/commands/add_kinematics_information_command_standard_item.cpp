
#include <tesseract_gui/widgets/environment/commands/add_kinematics_information_command_standard_item.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, CUBE_ICON, (":/tesseract_gui/png/cube.png"));

namespace tesseract_gui
{

AddKinematicsInformationCommandStandardItem::AddKinematicsInformationCommandStandardItem(tesseract_environment::AddKinematicsInformationCommand::ConstPtr command)
  : QStandardItem(*CUBE_ICON(), "Add Kinematics Information")
  , command(std::move(command))
{
  ctor();
}

AddKinematicsInformationCommandStandardItem::AddKinematicsInformationCommandStandardItem(const QString &text, tesseract_environment::AddKinematicsInformationCommand::ConstPtr command)
  : QStandardItem(*CUBE_ICON(), text)
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
//  auto* x_name = new QStandardItem(*NUMERIC_ICON(), "x");
//  auto* x_value = new QStandardItem(QString("%1").arg(box->getX()));
//  appendRow({x_name, x_value});

//  auto* y_name = new QStandardItem(*NUMERIC_ICON(), "y");
//  auto* y_value = new QStandardItem(QString("%1").arg(box->getY()));
//  appendRow({y_name, y_value});

//  auto* z_name = new QStandardItem(*NUMERIC_ICON(), "z");
//  auto* z_value = new QStandardItem(QString("%1").arg(box->getZ()));
//  appendRow({z_name, z_value});
}
}
