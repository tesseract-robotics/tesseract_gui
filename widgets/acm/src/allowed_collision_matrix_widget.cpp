#include <tesseract_gui/widgets/acm/allowed_collision_matrix_widget.h>
#include <tesseract_gui/widgets/acm/add_allowed_collision_entry_dialog.h>
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
  connect(ui_->acmTreeView, SIGNAL(entrySelected(tesseract_common::AllowedCollisionEntries)), this, SIGNAL(entrySelected(tesseract_common::AllowedCollisionEntries)));
  connect(ui_->acmTreeView, SIGNAL(selectedLinksChanged(std::vector<std::string>)), this, SIGNAL(selectedLinksChanged(std::vector<std::string>)));
}

AllowedCollisionMatrixWidget::~AllowedCollisionMatrixWidget() = default;

void AllowedCollisionMatrixWidget::setModel(tesseract_gui::AllowedCollisionMatrixModel* model)
{
  ui_->acmTreeView->setModel(model);
}

void AllowedCollisionMatrixWidget::onRemoveButtonClicked()
{
  QItemSelectionModel *selection = ui_->acmTreeView->selectionModel();
  QModelIndexList indices = selection->selectedRows();
  int row_cnt = indices.count();
  for( int i = row_cnt; i > 0; i--)
    ui_->acmTreeView->model()->removeRow(indices.at(i-1).row(),  QModelIndex());
}

void AllowedCollisionMatrixWidget::onAddButtonClicked()
{
  AddAllowedCollisionEntryDialog dialog;
  if(dialog.exec())
  {
    auto* model = qobject_cast<tesseract_gui::AllowedCollisionMatrixModel*>(ui_->acmTreeView->model());
    model->add(dialog.getLinkName1(), dialog.getLinkName2(), dialog.getReason());
  }
}

void AllowedCollisionMatrixWidget::onGenerateButtonClicked()
{
  emit generateClicked(ui_->resolutionSlider->value());
}

}
