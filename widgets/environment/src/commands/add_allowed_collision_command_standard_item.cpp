
#include <tesseract_gui/widgets/environment/commands/add_allowed_collision_command_standard_item.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, TEXT_ICON, (":/tesseract_gui/png/text.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, COMMAND_ICON, (":/tesseract_gui/png/merge.png"));

namespace tesseract_gui
{

AddAllowedCollisionCommandStandardItem::AddAllowedCollisionCommandStandardItem(tesseract_environment::AddAllowedCollisionCommand::ConstPtr command)
  : QStandardItem(*COMMAND_ICON(), "Add Allowed Collision")
  , command(std::move(command))
{
  ctor();
}

AddAllowedCollisionCommandStandardItem::AddAllowedCollisionCommandStandardItem(const QString &text, tesseract_environment::AddAllowedCollisionCommand::ConstPtr command)
  : QStandardItem(*COMMAND_ICON(), text)
  , command(std::move(command))
{
  ctor();
}

AddAllowedCollisionCommandStandardItem::AddAllowedCollisionCommandStandardItem(const QIcon &icon, const QString &text, tesseract_environment::AddAllowedCollisionCommand::ConstPtr command)
  : QStandardItem(icon, text)
  , command(std::move(command))
{
  ctor();
}

int AddAllowedCollisionCommandStandardItem::type() const
{
  return static_cast<int>(StandardItemType::ENV_COMMAND_ADD_ALLOWED_COLLISION);
}

void AddAllowedCollisionCommandStandardItem::ctor()
{
  auto* ln1_name = new QStandardItem(*TEXT_ICON, "link_name1");
  auto* ln1_value = new QStandardItem(QString("%1").arg(command->getLinkName1().c_str()));
  appendRow({ln1_name, ln1_value});

  auto* ln2_name = new QStandardItem(*TEXT_ICON, "link_name2");
  auto* ln2_value = new QStandardItem(QString("%1").arg(command->getLinkName2().c_str()));
  appendRow({ln2_name, ln2_value});

  auto* reason_name = new QStandardItem(*TEXT_ICON, "reason");
  auto* reason_value = new QStandardItem(QString("%1").arg(command->getReason().c_str()));
  appendRow({reason_name, reason_value});
}
}

