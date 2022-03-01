#include <tesseract_gui/kinematic_groups/group_tcps_model.h>

#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, ROBOT_ICON, (":/tesseract_gui/png/robotic-arm.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, TEXT_ICON, (":/tesseract_gui/png/text.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, NUMERIC_ICON, (":/tesseract_gui/png/numeric.png"));

namespace tesseract_gui
{

GroupTCPsModel::GroupTCPsModel(QObject *parent)
  : QStandardItemModel(parent)
{
  clear();
}
GroupTCPsModel::GroupTCPsModel(const GroupTCPsModel &other)
: QStandardItemModel(other.d_ptr->parent)
{
}

GroupTCPsModel &GroupTCPsModel::operator=(const GroupTCPsModel &/*other*/)
{
  return *this;
}

void GroupTCPsModel::clear()
{
  QStandardItemModel::clear();
  setColumnCount(2);
  setHorizontalHeaderLabels({"Name", "Values"});
  appendRow(new GroupTCPsStandardItem());
}

void GroupTCPsModel::set(const tesseract_srdf::GroupTCPs& group_tcps)
{
  QStandardItemModel::clear();
  setColumnCount(2);
  setHorizontalHeaderLabels({"Name", "Values"});
  appendRow(new GroupTCPsStandardItem(group_tcps));
}

void GroupTCPsModel::addGroupTCP(const QString &group_name, const QString &tcp_name, const Eigen::Isometry3d& tcp)
{
  getRoot()->addGroupTCP(group_name, tcp_name, tcp);
}

void GroupTCPsModel::removeGroupTCP(const QString &group_name, const QString &tcp_name)
{
  getRoot()->removeGroupTCP(group_name, tcp_name);
}

const tesseract_srdf::GroupTCPs& GroupTCPsModel::getGroupTCPs() const
{
  return getRoot()->getGroupTCPs();
}

GroupTCPsStandardItem* GroupTCPsModel::getRoot()
{
  return dynamic_cast<GroupTCPsStandardItem*>(item(0));
}

const GroupTCPsStandardItem* GroupTCPsModel::getRoot() const
{
  return dynamic_cast<const GroupTCPsStandardItem*>(item(0));
}

}
