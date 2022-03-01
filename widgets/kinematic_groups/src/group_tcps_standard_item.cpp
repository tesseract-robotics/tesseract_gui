#include <tesseract_gui/kinematic_groups/group_tcps_standard_item.h>
#include <tesseract_gui/kinematic_groups/tcp_standard_item.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, ROBOT_ICON, (":/tesseract_gui/png/robotic-arm.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, TEXT_ICON, (":/tesseract_gui/png/text.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, NUMERIC_ICON, (":/tesseract_gui/png/numeric.png"));

namespace tesseract_gui
{
GroupTCPsStandardItem::GroupTCPsStandardItem()
: QStandardItem("Group TCPs")
{
  ctor();
}

GroupTCPsStandardItem::GroupTCPsStandardItem(tesseract_srdf::GroupTCPs group_tcps)
  : QStandardItem("Group TCPs")
  , group_tcps_(std::move(group_tcps))
{
  ctor();
}

GroupTCPsStandardItem::GroupTCPsStandardItem(const QString &text, tesseract_srdf::GroupTCPs group_tcps)
  : QStandardItem(text)
  , group_tcps_(std::move(group_tcps))
{
  ctor();
}

GroupTCPsStandardItem::GroupTCPsStandardItem(const QIcon &icon, const QString &text, tesseract_srdf::GroupTCPs group_tcps)
  : QStandardItem(icon, text)
  , group_tcps_(std::move(group_tcps))
{
  ctor();
}

int GroupTCPsStandardItem::type() const
{
  return static_cast<int>(StandardItemType::GROUP_TCPS);
}

void GroupTCPsStandardItem::addGroupTCPItem(const QString& group_name, const QString& tcp_name, const Eigen::Isometry3d& tcp)
{
  QStandardItem* group_item {nullptr};
  auto it = group_items_.find(group_name.toStdString());
  if (it != group_items_.end())
  {
    group_item = it->second;
  }
  else
  {
    group_item = new QStandardItem(*ROBOT_ICON(), group_name);
    group_items_[group_name.toStdString()] = group_item;
    appendRow({group_item, new QStandardItem()}); // Must have two columns so QTreeView::setRootIndex will have two columns
  }

  group_item->appendRow(new TCPStandardItem(tcp_name, tcp));
}

void GroupTCPsStandardItem::addGroupTCP(const QString& group_name, const QString& tcp_name, const Eigen::Isometry3d& tcp)
{
  removeGroupTCP(group_name, tcp_name);
  addGroupTCPItem(group_name, tcp_name, tcp);

  group_tcps_[group_name.toStdString()][tcp_name.toStdString()] = tcp;
}

void GroupTCPsStandardItem::removeGroupTCP(const QString &group_name, const QString &tcp_name)
{
  auto group_it = group_tcps_.find(group_name.toStdString());
  if(group_it != group_tcps_.end())
  {
    QStandardItem* item = group_items_.at(group_name.toStdString());
    for (int i = item->rowCount(); i > 0; i--)
    {
      QStandardItem* child_item = item->child(i-1);
      if (child_item->text() == tcp_name)
        item->removeRow(i - 1);
    }

    auto state_it = group_it->second.find(tcp_name.toStdString());
    if(state_it != group_it->second.end())
      group_it->second.erase(state_it);

    if (group_it->second.empty())
    {
      group_tcps_.erase(group_it);
      removeRow(item->index().row());
    }
  }
}

const tesseract_srdf::GroupTCPs& GroupTCPsStandardItem::getGroupTCPs() const
{
  return group_tcps_;
}

void GroupTCPsStandardItem::ctor()
{
  for(auto& group : group_tcps_)
  {
    for (auto& tcp : group.second)
      addGroupTCPItem(QString::fromStdString(group.first), QString::fromStdString(tcp.first), tcp.second);
  }
}
}

