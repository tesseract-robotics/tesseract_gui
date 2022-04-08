
#include <tesseract_widgets/environment/commands/change_link_origin_command_standard_item.h>
#include <tesseract_widgets/common/transform_standard_item.h>
#include <tesseract_widgets/common/standard_item_utils.h>
#include <tesseract_widgets/common/standard_item_type.h>

namespace tesseract_gui
{

ChangeLinkOriginCommandStandardItem::ChangeLinkOriginCommandStandardItem(tesseract_environment::ChangeLinkOriginCommand::ConstPtr command)
  : QStandardItem(QIcon(":/tesseract_widgets/png/merge.png"), "Change Link Origin")
  , command(std::move(command))
{
  ctor();
}

ChangeLinkOriginCommandStandardItem::ChangeLinkOriginCommandStandardItem(const QString &text, tesseract_environment::ChangeLinkOriginCommand::ConstPtr command)
  : QStandardItem(QIcon(":/tesseract_widgets/png/merge.png"), text)
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
  auto* item = new TransformStandardItem(QIcon(":/tesseract_widgets/ignition/link.png"), command->getLinkName().c_str(), command->getOrigin());
  appendRow({item, new QStandardItem()});
}
}
