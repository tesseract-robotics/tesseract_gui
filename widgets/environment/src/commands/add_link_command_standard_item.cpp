
#include <tesseract_gui/widgets/environment/commands/add_link_command_standard_item.h>
#include <tesseract_gui/widgets/scene_graph/link_standard_item.h>
#include <tesseract_gui/widgets/scene_graph/joint_standard_item.h>
#include <tesseract_gui/widgets/common/standard_item_utils.h>
#include <tesseract_gui/common/standard_item_type.h>

namespace tesseract_gui
{

AddLinkCommandStandardItem::AddLinkCommandStandardItem(tesseract_environment::AddLinkCommand::ConstPtr command)
  : QStandardItem(QIcon(":/tesseract_gui/png/merge.png"), "Add Link")
  , command(std::move(command))
{
  ctor();
}

AddLinkCommandStandardItem::AddLinkCommandStandardItem(const QString &text, tesseract_environment::AddLinkCommand::ConstPtr command)
  : QStandardItem(QIcon(":/tesseract_gui/png/merge.png"), text)
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
  appendRow(new LinkStandardItem(std::make_shared<tesseract_scene_graph::Link>(command->getLink()->clone())));

  if (command->getJoint() != nullptr)
    appendRow(new JointStandardItem(std::make_shared<tesseract_scene_graph::Joint>(command->getJoint()->clone())));
  else
    appendRow(createStandardItemString(QIcon(":/tesseract_gui/ignition/joint.png"), "Joint", "NULL"));

  appendRow(createStandardItemString("replace allowed", (command->replaceAllowed()) ? "True" : "False"));
}
}
