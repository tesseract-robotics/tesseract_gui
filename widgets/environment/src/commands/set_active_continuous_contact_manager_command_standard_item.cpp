
#include <tesseract_gui/widgets/environment/commands/set_active_continuous_contact_manager_command_standard_item.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, CUBE_ICON, (":/tesseract_gui/png/cube.png"));

namespace tesseract_gui
{

SetActiveContinuousContactManagerCommandStandardItem::SetActiveContinuousContactManagerCommandStandardItem(tesseract_environment::SetActiveContinuousContactManagerCommand::ConstPtr command)
  : QStandardItem(*CUBE_ICON(), "Set Active Continuous Contact Manager")
  , command(std::move(command))
{
  ctor();
}

SetActiveContinuousContactManagerCommandStandardItem::SetActiveContinuousContactManagerCommandStandardItem(const QString &text, tesseract_environment::SetActiveContinuousContactManagerCommand::ConstPtr command)
  : QStandardItem(*CUBE_ICON(), text)
  , command(std::move(command))
{
  ctor();
}

SetActiveContinuousContactManagerCommandStandardItem::SetActiveContinuousContactManagerCommandStandardItem(const QIcon &icon, const QString &text, tesseract_environment::SetActiveContinuousContactManagerCommand::ConstPtr command)
  : QStandardItem(icon, text)
  , command(std::move(command))
{
  ctor();
}

int SetActiveContinuousContactManagerCommandStandardItem::type() const
{
  return static_cast<int>(StandardItemType::ENV_COMMAND_SET_ACTIVE_CONTINUOUS_CONTACT_MANAGER);
}

void SetActiveContinuousContactManagerCommandStandardItem::ctor()
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
