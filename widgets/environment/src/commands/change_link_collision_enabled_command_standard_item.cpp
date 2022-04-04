
#include <tesseract_gui/widgets/environment/commands/change_link_collision_enabled_command_standard_item.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, CUBE_ICON, (":/tesseract_gui/png/cube.png"));

namespace tesseract_gui
{

ChangeLinkCollisionEnabledCommandStandardItem::ChangeLinkCollisionEnabledCommandStandardItem(tesseract_environment::ChangeLinkCollisionEnabledCommand::ConstPtr command)
  : QStandardItem(*CUBE_ICON(), "Change Link Collision Enabled")
  , command(std::move(command))
{
  ctor();
}

ChangeLinkCollisionEnabledCommandStandardItem::ChangeLinkCollisionEnabledCommandStandardItem(const QString &text, tesseract_environment::ChangeLinkCollisionEnabledCommand::ConstPtr command)
  : QStandardItem(*CUBE_ICON(), text)
  , command(std::move(command))
{
  ctor();
}

ChangeLinkCollisionEnabledCommandStandardItem::ChangeLinkCollisionEnabledCommandStandardItem(const QIcon &icon, const QString &text, tesseract_environment::ChangeLinkCollisionEnabledCommand::ConstPtr command)
  : QStandardItem(icon, text)
  , command(std::move(command))
{
  ctor();
}

int ChangeLinkCollisionEnabledCommandStandardItem::type() const
{
  return static_cast<int>(StandardItemType::ENV_COMMAND_CHANGE_LINK_COLLISION_ENABLED);
}

void ChangeLinkCollisionEnabledCommandStandardItem::ctor()
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
