
#include <tesseract_gui/widgets/environment/commands/add_link_command_standard_item.h>
#include <tesseract_gui/widgets/scene_graph/link_standard_item.h>
#include <tesseract_gui/widgets/scene_graph/joint_standard_item.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, COMMAND_ICON, (":/tesseract_gui/png/merge.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, JOINT_ICON, (":/tesseract_gui/ignition/joint.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, TEXT_ICON, (":/tesseract_gui/png/text.png"));

namespace tesseract_gui
{

AddLinkCommandStandardItem::AddLinkCommandStandardItem(tesseract_environment::AddLinkCommand::ConstPtr command)
  : QStandardItem(*COMMAND_ICON(), "Add Link")
  , command(std::move(command))
{
  ctor();
}

AddLinkCommandStandardItem::AddLinkCommandStandardItem(const QString &text, tesseract_environment::AddLinkCommand::ConstPtr command)
  : QStandardItem(*COMMAND_ICON(), text)
  , command(std::move(command))
{
  ctor();
}

AddLinkCommandStandardItem::AddLinkCommandStandardItem(const QIcon &icon, const QString &text, tesseract_environment::AddLinkCommand::ConstPtr command)
  : QStandardItem(icon, text)
  , command(std::move(command))
{
  ctor();
}

int AddLinkCommandStandardItem::type() const
{
  return static_cast<int>(StandardItemType::ENV_COMMAND_ADD_LINK);
}

void AddLinkCommandStandardItem::ctor()
{
  auto* link = new LinkStandardItem(std::make_shared<tesseract_scene_graph::Link>(command->getLink()->clone()));
  appendRow(link);

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

  auto* x_name = new QStandardItem(*TEXT_ICON(), "replace allowed");
  auto* x_value = new QStandardItem((command->replaceAllowed()) ? "true" : "false");
  appendRow({x_name, x_value});
}
}
