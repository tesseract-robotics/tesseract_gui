#include <tesseract_widgets/acm/allowed_collision_matrix_tree_view.h>
#include <tesseract_widgets/acm/allowed_collision_matrix_model.h>
#include <QMouseEvent>

namespace tesseract_gui
{
  AllowedCollisionMatrixTreeView::AllowedCollisionMatrixTreeView(QWidget *parent)
    : QTreeView(parent) { }

  AllowedCollisionMatrixTreeView::~AllowedCollisionMatrixTreeView() = default;

  void AllowedCollisionMatrixTreeView::setModel(QAbstractItemModel *model)
  {
    assert(qobject_cast<tesseract_gui::AllowedCollisionMatrixModel*>(model) != nullptr);
    QTreeView::setModel(model);
    connect(selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(onSelectionChanged(QItemSelection,QItemSelection)));
  }

  void AllowedCollisionMatrixTreeView::mousePressEvent(QMouseEvent *event)
  {
    // This enables deselecting items when clicked again or clicking black area of treeview
    QModelIndex item = indexAt(event->pos());
    bool selected = selectionModel()->isSelected(indexAt(event->pos()));
    QTreeView::mousePressEvent(event);
    if ((item.row() == -1 && item.column() == -1) || selected)
    {
      clearSelection();
      const QModelIndex index;
      selectionModel()->setCurrentIndex(index, QItemSelectionModel::Select);
    }
  }

  void AllowedCollisionMatrixTreeView::onSelectionChanged(const QItemSelection &/*selected*/, const QItemSelection &/*deselected*/)
  {
    auto* model = qobject_cast<tesseract_gui::AllowedCollisionMatrixModel*>(this->model());
    QModelIndexList indices = selectionModel()->selectedRows();

    tesseract_common::AllowedCollisionEntries selected_entries;
    std::set<std::string> selected_links_set;
    for (const auto& i : indices)
    {
      selected_entries[tesseract_common::makeOrderedLinkPair(model->item(i.row(), 0)->text().toStdString(), model->item(i.row(), 1)->text().toStdString())] = model->item(i.row(), 2)->text().toStdString();
      selected_links_set.insert(model->item(i.row(), 0)->text().toStdString());
      selected_links_set.insert(model->item(i.row(), 1)->text().toStdString());
    }

    std::vector<std::string> selected_links(selected_links_set.begin(), selected_links_set.end());
    emit entrySelected(selected_entries);
    emit selectedLinksChanged(selected_links);
  }
}
