
#include <tesseract_gui/widgets/environment/commands/add_scene_graph_command_standard_item.h>
#include <tesseract_gui/widgets/scene_graph/scene_graph_standard_item.h>
#include <tesseract_gui/widgets/scene_graph/joint_standard_item.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, COMMAND_ICON, (":/tesseract_gui/png/merge.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, JOINT_ICON, (":/tesseract_gui/ignition/joint.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, TEXT_ICON, (":/tesseract_gui/png/text.png"));

namespace tesseract_gui
{

AddSceneGraphCommandStandardItem::AddSceneGraphCommandStandardItem(tesseract_environment::AddSceneGraphCommand::ConstPtr command)
  : QStandardItem(*COMMAND_ICON(), "Add Scene Graph")
  , command(std::move(command))
{
  ctor();
}

AddSceneGraphCommandStandardItem::AddSceneGraphCommandStandardItem(const QString &text, tesseract_environment::AddSceneGraphCommand::ConstPtr command)
  : QStandardItem(*COMMAND_ICON(), text)
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
  auto* prefix_name = new QStandardItem(*TEXT_ICON(), "prefix");
  auto* prefix_value = new QStandardItem(command->getPrefix().c_str());
  appendRow({prefix_name, prefix_value});

  if (command->getJoint() != nullptr)
  {
    auto* joint = new JointStandardItem(std::make_shared<tesseract_scene_graph::Joint>(command->getJoint()->clone()));
    appendRow(joint);
  }
  else
  {
    auto* x_name = new QStandardItem(*JOINT_ICON(), "Joint");
    auto* x_value = new QStandardItem("NULL");
    appendRow({x_name, x_value});
  }

  auto* scene_graph = new SceneGraphStandardItem(command->getSceneGraph()->clone());
  appendRow(scene_graph);
}
}
