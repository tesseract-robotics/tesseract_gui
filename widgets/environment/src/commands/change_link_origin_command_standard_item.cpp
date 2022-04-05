
#include <tesseract_gui/widgets/environment/commands/change_link_origin_command_standard_item.h>
#include <tesseract_gui/widgets/common/transform_standard_item.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, COMMAND_ICON, (":/tesseract_gui/png/merge.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, LINK_ICON, (":/tesseract_gui/ignition/link.png"));

namespace tesseract_gui
{

ChangeLinkOriginCommandStandardItem::ChangeLinkOriginCommandStandardItem(tesseract_environment::ChangeLinkOriginCommand::ConstPtr command)
  : QStandardItem(*COMMAND_ICON(), "Change Link Origin")
  , command(std::move(command))
{
  ctor();
}

ChangeLinkOriginCommandStandardItem::ChangeLinkOriginCommandStandardItem(const QString &text, tesseract_environment::ChangeLinkOriginCommand::ConstPtr command)
  : QStandardItem(*COMMAND_ICON(), text)
  , command(std::move(command))
{
  ctor();
}

ChangeLinkOriginCommandStandardItem::ChangeLinkOriginCommandStandardItem(const QIcon &icon, const QString &text, tesseract_environment::ChangeLinkOriginCommand::ConstPtr command)
  : QStandardItem(icon, text)
  , command(std::move(command))
{
  ctor();
}

int ChangeLinkOriginCommandStandardItem::type() const
{
  return static_cast<int>(StandardItemType::ENV_COMMAND_CHANGE_LINK_ORIGIN);
}

void ChangeLinkOriginCommandStandardItem::ctor()
{
  auto* item = new TransformStandardItem(*LINK_ICON(), command->getLinkName().c_str(), command->getOrigin());
  appendRow({item, new QStandardItem()});
}
}
