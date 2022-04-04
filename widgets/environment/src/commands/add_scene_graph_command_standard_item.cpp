
#include <tesseract_gui/widgets/environment/commands/add_scene_graph_command_standard_item.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, CUBE_ICON, (":/tesseract_gui/png/cube.png"));

namespace tesseract_gui
{

AddSceneGraphCommandStandardItem::AddSceneGraphCommandStandardItem(tesseract_environment::AddSceneGraphCommand::ConstPtr command)
  : QStandardItem(*CUBE_ICON(), "Add Scene Graph")
  , command(std::move(command))
{
  ctor();
}

AddSceneGraphCommandStandardItem::AddSceneGraphCommandStandardItem(const QString &text, tesseract_environment::AddSceneGraphCommand::ConstPtr command)
  : QStandardItem(*CUBE_ICON(), text)
  , command(std::move(command))
{
  ctor();
}

AddSceneGraphCommandStandardItem::AddSceneGraphCommandStandardItem(const QIcon &icon, const QString &text, tesseract_environment::AddSceneGraphCommand::ConstPtr command)
  : QStandardItem(icon, text)
  , command(std::move(command))
{
  ctor();
}

int AddSceneGraphCommandStandardItem::type() const
{
  return static_cast<int>(StandardItemType::ENV_COMMAND_ADD_SCENE_GRAPH);
}

void AddSceneGraphCommandStandardItem::ctor()
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
