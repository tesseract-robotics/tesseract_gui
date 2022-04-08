#include <tesseract_widgets/environment/environment_commands_model.h>
#include <tesseract_widgets/common/standard_item_type.h>

namespace tesseract_gui
{

EnvironmentCommandsModel::EnvironmentCommandsModel(QObject *parent)
  : QStandardItemModel(parent)
{
  clear();
}
EnvironmentCommandsModel::EnvironmentCommandsModel(const EnvironmentCommandsModel &other)
: QStandardItemModel(other.d_ptr->parent)
{
}

EnvironmentCommandsModel &EnvironmentCommandsModel::operator=(const EnvironmentCommandsModel &/*other*/)
{
  return *this;
}

void EnvironmentCommandsModel::clear()
{
  QStandardItemModel::clear();
  setColumnCount(2);
  setHorizontalHeaderLabels({"Name", "Values"});
  appendRow(new EnvironmentCommandsStandardItem());
}

void EnvironmentCommandsModel::set(const tesseract_environment::Commands& commands)
{
  QStandardItemModel::clear();
  setColumnCount(2);
  setHorizontalHeaderLabels({"Name", "Values"});
  appendRow(new EnvironmentCommandsStandardItem(commands));
}

void EnvironmentCommandsModel::appendCommand(const tesseract_environment::Command::ConstPtr& command)
{
  getRoot()->appendCommand(command);
}

const tesseract_environment::Commands& EnvironmentCommandsModel::getCommands() const
{
  return getRoot()->getCommands();
}

EnvironmentCommandsStandardItem* EnvironmentCommandsModel::getRoot()
{
  return dynamic_cast<EnvironmentCommandsStandardItem*>(item(0));
}

const EnvironmentCommandsStandardItem* EnvironmentCommandsModel::getRoot() const
{
  return dynamic_cast<const EnvironmentCommandsStandardItem*>(item(0));
}

}
