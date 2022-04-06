
#include <tesseract_gui/widgets/environment/commands/add_scene_graph_command_standard_item.h>
#include <tesseract_gui/widgets/scene_graph/scene_graph_standard_item.h>
#include <tesseract_gui/widgets/scene_graph/joint_standard_item.h>
#include <tesseract_gui/widgets/common/standard_item_utils.h>
#include <tesseract_gui/common/standard_item_type.h>

namespace tesseract_gui
{

AddSceneGraphCommandStandardItem::AddSceneGraphCommandStandardItem(tesseract_environment::AddSceneGraphCommand::ConstPtr command)
  : QStandardItem(QIcon(":/tesseract_gui/png/merge.png"), "Add Scene Graph")
  , command(std::move(command))
{
  ctor();
}

AddSceneGraphCommandStandardItem::AddSceneGraphCommandStandardItem(const QString &text, tesseract_environment::AddSceneGraphCommand::ConstPtr command)
  : QStandardItem(QIcon(":/tesseract_gui/png/merge.png"), text)
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
  appendRow(createStandardItemString("prefix", command->getPrefix()));

  if (command->getJoint() != nullptr)
    appendRow(new JointStandardItem(std::make_shared<tesseract_scene_graph::Joint>(command->getJoint()->clone())));
  else
    appendRow(createStandardItemString(QIcon(":/tesseract_gui/ignition/joint.png"), "Joint", "NULL"));

  appendRow(new SceneGraphStandardItem(command->getSceneGraph()->clone()));
}
}
