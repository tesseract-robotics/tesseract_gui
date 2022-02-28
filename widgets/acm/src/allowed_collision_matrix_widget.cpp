#include <tesseract_gui/acm/allowed_collision_matrix_widget.h>
#include <tesseract_gui/acm/add_allowed_collision_entry_dialog.h>
#include "ui_allowed_collision_matrix_widget.h"
#include <QDialog>

namespace tesseract_gui
{
AllowedCollisionMatrixWidget::AllowedCollisionMatrixWidget(QWidget *parent)
  : QWidget(parent)
  , ui_(std::make_unique<Ui::AllowedCollisionMatrixWidget>())
{
  ui_->setupUi(this);
  connect(ui_->generatePushButton, SIGNAL(clicked()), this, SLOT(onGenerateButtonClicked()));
  connect(ui_->removePushButton, SIGNAL(clicked()), this, SLOT(onRemoveButtonClicked()));
  connect(ui_->addPushButton, SIGNAL(clicked()), this, SLOT(onAddButtonClicked()));
}

AllowedCollisionMatrixWidget::~AllowedCollisionMatrixWidget() = default;

void AllowedCollisionMatrixWidget::setModel(tesseract_gui::AllowedCollisionMatrixModel* model)
{
  ui_->acmTableView->setModel(model);
  connect(ui_->acmTableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(onSelectionChanged(QItemSelection,QItemSelection)));
}

void AllowedCollisionMatrixWidget::onRemoveButtonClicked()
{
  QItemSelectionModel *selection = ui_->acmTableView->selectionModel();
  QModelIndexList indices = selection->selectedRows();
  int row_cnt = indices.count();
  for( int i = row_cnt; i > 0; i--)
    ui_->acmTableView->model()->removeRow(indices.at(i-1).row(),  QModelIndex());
}

void AllowedCollisionMatrixWidget::onAddButtonClicked()
{
  AddAllowedCollisionEntryDialog dialog;
  if(dialog.exec())
  {
    auto* model = qobject_cast<tesseract_gui::AllowedCollisionMatrixModel*>(ui_->acmTableView->model());
    model->add(dialog.getLinkName1(), dialog.getLinkName2(), dialog.getReason());
  }
}

void AllowedCollisionMatrixWidget::onGenerateButtonClicked()
{
  emit generateClicked(ui_->resolutionSlider->value());
}

void AllowedCollisionMatrixWidget::onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
  auto* model = qobject_cast<tesseract_gui::AllowedCollisionMatrixModel*>(ui_->acmTableView->model());
  QModelIndexList indices = ui_->acmTableView->selectionModel()->selectedRows();

  tesseract_common::AllowedCollisionEntries selection;
  for (const auto& i : indices)
    selection[tesseract_common::makeOrderedLinkPair(model->item(i.row(), 0)->text().toStdString(), model->item(i.row(), 1)->text().toStdString())] = model->item(i.row(), 2)->text().toStdString();

  emit entrySelected(selection);
}
}
