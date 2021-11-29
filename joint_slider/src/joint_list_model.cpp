
#include <tesseract_gui/joint_slider/joint_list_model.h>

namespace tesseract_gui
{

JointListModel::JointListModel(QObject *parent)
  : QStandardItemModel(parent)
{
  connect(this, &JointListModel::dataChanged, this, &JointListModel::onDataChanged);
}

JointListModel::JointListModel(const JointListModel &other)
  : QStandardItemModel(other.d_ptr->parent)
{
  this->joints_ = other.joints_;
}

JointListModel &JointListModel::operator=(const JointListModel &other)
{
  this->joints_ = other.joints_;
  return *this;
}

QHash<int, QByteArray> JointListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[MinRole] = "min";
    roles[MaxRole] = "max";
    roles[ValueRole] = "value";
    return roles;
}

void JointListModel::add(tesseract_scene_graph::Joint::ConstPtr joint)
{
  if (joint->type == tesseract_scene_graph::JointType::REVOLUTE || joint->type == tesseract_scene_graph::JointType::PRISMATIC)
  {
    joints_.push_back(joint);
    QStandardItem *parent_item = this->invisibleRootItem();
    QString joint_name = QString::fromStdString(joint->getName());
    auto item = new QStandardItem(joint_name);
    item->setData(joint_name, JointRoles::NameRole);
    item->setData(QString::number(joint->limits->lower), JointRoles::MinRole);
    item->setData(QString::number(joint->limits->upper), JointRoles::MaxRole);
    item->setData(QString::number(0), JointRoles::ValueRole);
    parent_item->appendRow(item);
  }
}

void JointListModel::clear()
{
  QStandardItemModel::clear();
  joints_.clear();
}

void JointListModel::onDataChanged(const QModelIndex &topLeft, const QModelIndex &/*bottomRight*/, const QVector<int> &roles)
{
  QStandardItem* item = this->itemFromIndex(topLeft);
  QString name = item->data(JointRoles::NameRole).toString();
  double min = item->data(JointRoles::MinRole).toDouble();
  double max = item->data(JointRoles::MaxRole).toDouble();
  double value = item->data(JointRoles::ValueRole).toDouble();

  for (const auto& role : roles)
  {
    switch (role)
    {
      case JointRoles::ValueRole:
      {
        emit jointValueChanged(name, value);
        break;
      }
      case JointRoles::MinRole:
      {
        emit jointMinChanged(name, min);
        break;
      }
      case JointRoles::MaxRole:
      {
        emit jointMaxChanged(name, max);
        break;
      }
    }
  }
}
}