#include <tesseract_gui/acm/allowed_collision_matrix_model.h>

namespace tesseract_gui
{

AllowedCollisionMatrixModel::AllowedCollisionMatrixModel(QObject *parent)
  : QStandardItemModel(parent)
{
}

AllowedCollisionMatrixModel::AllowedCollisionMatrixModel(const AllowedCollisionMatrixModel &other)
  : QStandardItemModel(other.d_ptr->parent)
{
  this->acm_ = other.acm_;
}

AllowedCollisionMatrixModel &AllowedCollisionMatrixModel::operator=(const AllowedCollisionMatrixModel &other)
{
  this->acm_ = other.acm_;
  return *this;
}

QHash<int, QByteArray> AllowedCollisionMatrixModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Link1Role] = "link1";
    roles[Link2Role] = "link2";
    roles[ReasonRole] = "reason";
    return roles;
}

void AllowedCollisionMatrixModel::setAllowedCollisionMatrix(const tesseract_common::AllowedCollisionMatrix& acm)
{
  QStandardItemModel::clear();
  acm_ = acm;
  QStandardItem *parent_item = this->invisibleRootItem();
  for (const auto& ac : acm.getAllAllowedCollisions())
  {
    auto item = new QStandardItem();
    item->setData(QString::fromStdString(ac.first.first), AllowedCollisionMatrixRoles::Link1Role);
    item->setData(QString::fromStdString(ac.first.second), AllowedCollisionMatrixRoles::Link2Role);
    item->setData(QString::fromStdString(ac.second), AllowedCollisionMatrixRoles::ReasonRole);
    parent_item->appendRow(item);
  }
  sort(0);
}

void AllowedCollisionMatrixModel::add(const QString& link1_name, const QString& link2_name, const QString& reason)
{
  acm_.addAllowedCollision(link1_name.toStdString(), link2_name.toStdString(), reason.toStdString());
  QStandardItem *parent_item = this->invisibleRootItem();
  auto item = new QStandardItem();
  item->setData(link1_name, AllowedCollisionMatrixRoles::Link1Role);
  item->setData(link2_name, AllowedCollisionMatrixRoles::Link2Role);
  item->setData(reason, AllowedCollisionMatrixRoles::ReasonRole);
  parent_item->appendRow(item);
  sort(0);

  // If the count does not match then it was a replace so rebuild model.
  if (parent_item->rowCount() != static_cast<int>(acm_.getAllAllowedCollisions().size()))
    setAllowedCollisionMatrix(acm_);

  emit entryAdded(link1_name, link2_name, reason);
}

bool AllowedCollisionMatrixModel::removeRows(int row, int count, const QModelIndex &parent)
{
  if (row >= 0)
  {
    QStandardItem *row_item = item(row);
    QString link1_name = row_item->data(AllowedCollisionMatrixRoles::Link1Role).toString();
    QString link2_name = row_item->data(AllowedCollisionMatrixRoles::Link2Role).toString();
    acm_.removeAllowedCollision(link1_name.toStdString(), link2_name.toStdString());

    bool success = QStandardItemModel::removeRows(row, count, parent);

    if (success)
    {
     emit entryRemoved(link1_name, link2_name);
    }

    return success;
  }
  return false;
}

void AllowedCollisionMatrixModel::clear()
{
  QStandardItemModel::clear();
  acm_.clearAllowedCollisions();
}

void AllowedCollisionMatrixModel::onEntrySelected(int row)
{
  QStandardItem *row_item = item(row);
  QString link1_name = row_item->data(AllowedCollisionMatrixRoles::Link1Role).toString();
  QString link2_name = row_item->data(AllowedCollisionMatrixRoles::Link2Role).toString();
  QString reason = row_item->data(AllowedCollisionMatrixRoles::ReasonRole).toString();
  emit entrySelected(link1_name, link2_name, reason);
}

tesseract_common::AllowedCollisionMatrix AllowedCollisionMatrixModel::getAllowedCollisionMatrix() const
{
  return acm_;
}

}
