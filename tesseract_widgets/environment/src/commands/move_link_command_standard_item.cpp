
#include <tesseract_widgets/environment/commands/move_link_command_standard_item.h>
#include <tesseract_widgets/scene_graph/joint_standard_item.h>
#include <tesseract_widgets/common/standard_item_utils.h>
#include <tesseract_widgets/common/standard_item_type.h>

namespace tesseract_gui
{

MoveLinkCommandStandardItem::MoveLinkCommandStandardItem(tesseract_environment::MoveLinkCommand::ConstPtr command)
  : QStandardItem(QIcon(":/tesseract_widgets/png/merge.png"), "Move Link")
  , command(std::move(command))
{
  ctor();
}

MoveLinkCommandStandardItem::MoveLinkCommandStandardItem(const QString &text, tesseract_environment::MoveLinkCommand::ConstPtr command)
  : QStandardItem(QIcon(":/tesseract_widgets/png/merge.png"), text)
  , command(std::move(command))
{
  ctor();
}

MoveLinkCommandStandardItem::MoveLinkCommandStandardItem(const QIcon &icon, const QString &text, tesseract_environment::MoveLinkCommand::ConstPtr command)
  : QStandardItem(icon, text)
  , command(std::move(command))
{
  ctor();
}

int MoveLinkCommandStandardItem::type() const
{
  return static_cast<int>(StandardItemType::ENV_COMMAND_MOVE_LINK);
}

void MoveLinkCommandStandardItem::ctor()
{
  auto* item = new JointStandardItem(std::make_shared<tesseract_scene_graph::Joint>(command->getJoint()->clone()));
  appendRow({item, new QStandardItem()});
}
}
