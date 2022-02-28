#include <tesseract_gui/kinematic_groups/groups_tcps_standard_item.h>
#include <tesseract_gui/scene_graph/origin_standard_item.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, ROBOT_ICON, (":/tesseract_gui/png/robotic-arm.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, TEXT_ICON, (":/tesseract_gui/png/text.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, NUMERIC_ICON, (":/tesseract_gui/png/numeric.png"));

namespace tesseract_gui
{

GroupsTCPsStandardItem::GroupsTCPsStandardItem(const QString &text, const tesseract_srdf::GroupsTCPs& groups_tcps)
  : QStandardItem(*ROBOT_ICON(), text)
  , groups_tcps(groups_tcps)
{
  ctor();
}

GroupsTCPsStandardItem::GroupsTCPsStandardItem(const QIcon &icon, const QString &text, const tesseract_srdf::GroupsTCPs& groups_tcps)
  : QStandardItem(icon, text)
  , groups_tcps(groups_tcps)
{
  ctor();
}

int GroupsTCPsStandardItem::type() const
{
  return static_cast<int>(StandardItemType::TCP);
}

void GroupsTCPsStandardItem::ctor()
{
  for (auto& tcp : groups_tcps)
    appendRow(new OriginStandardItem(QString::fromStdString(tcp.first), tcp.second));
}
}

