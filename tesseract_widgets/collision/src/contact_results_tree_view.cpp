/**
 * @author Levi Armstrong <levi.armstrong@gmail.com>
 *
 * @copyright Copyright (C) 2022 Levi Armstrong <levi.armstrong@gmail.com>
 *
 * @par License
 * GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
 * @par
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 * @par
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * @par
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#include <tesseract_widgets/collision/contact_results_tree_view.h>
#include <tesseract_widgets/collision/contact_results_model.h>
#include <tesseract_widgets/collision/contact_result_standard_item.h>
#include <tesseract_widgets/common/standard_item_type.h>

namespace tesseract_gui
{
struct ContactResultsTreeViewImpl
{
  ContactResultsModel* model;

  // Store the selected item
  QStandardItem* selected_item;
};

ContactResultsTreeView::ContactResultsTreeView(QWidget* parent)
  : QTreeView(parent), data_(std::make_unique<ContactResultsTreeViewImpl>())
{
  connect(this, &QTreeView::collapsed, [this]() { resizeColumnToContents(0); });
  connect(this, &QTreeView::expanded, [this]() { resizeColumnToContents(0); });
}

ContactResultsTreeView::~ContactResultsTreeView() = default;

void ContactResultsTreeView::setModel(QAbstractItemModel* model)
{
  data_->model = qobject_cast<ContactResultsModel*>(model);
  QTreeView::setModel(model);

  connect(selectionModel(),
          SIGNAL(currentRowChanged(QModelIndex, QModelIndex)),
          this,
          SLOT(onCurrentRowChanged(QModelIndex, QModelIndex)));
}

void ContactResultsTreeView::onCurrentRowChanged(const QModelIndex& current, const QModelIndex& previous)
{
  data_->selected_item = data_->model->itemFromIndex(current);

  if (data_->selected_item != nullptr)
  {
    tesseract_collision::ContactResultVector contact_results = data_->model->getContactResults(current);
    Q_EMIT showContactResults(contact_results);
  }
}

}  // namespace tesseract_gui
