#include <tesseract_widgets/collision/contact_results_widget.h>
#include <tesseract_widgets/collision/contact_results_model.h>
#include "ui_contact_results_widget.h"

namespace tesseract_gui
{
struct ContactResultsWidgetImpl
{
  tesseract_gui::ContactTestFn contact_test_fn;
  ContactResultsModel* model;
};

ContactResultsWidget::ContactResultsWidget(QWidget* parent)
  : QWidget(parent)
  , ui(std::make_unique<Ui::ContactResultsWidget>())
  , data_(std::make_unique<ContactResultsWidgetImpl>())
{
  ui->setupUi(this);

  ui->contact_request_group_box->hide();
  ui->compute_frame->hide();

  connect(ui->compute_push_button, SIGNAL(clicked()), this, SLOT(onComputeClicked()));
}

ContactResultsWidget::~ContactResultsWidget() = default;

void ContactResultsWidget::setModel(ContactResultsModel* model)
{
  ui->tree_view->setModel(model);
  data_->model = model;
}

void ContactResultsWidget::setContactTestFn(ContactTestFn contact_test_fn)
{
  data_->contact_test_fn = contact_test_fn;
  if (data_->contact_test_fn == nullptr)
  {
    ui->contact_request_group_box->hide();
    ui->compute_frame->hide();
  }
  else
  {
    ui->contact_request_group_box->show();
    ui->compute_frame->show();
  }
}

void ContactResultsWidget::onComputeClicked()
{
  if (data_->contact_test_fn == nullptr)
    return;

  tesseract_collision::ContactManagerConfig config(ui->contact_threshold->value());

  tesseract_collision::ContactRequest request;
  request.calculate_distance = ui->calculate_distance->isChecked();
  request.calculate_penetration = ui->calculate_penetration->isChecked();
  request.type = static_cast<tesseract_collision::ContactTestType>(ui->contact_test_type->currentIndex());
  tesseract_collision::ContactResultMap results = data_->contact_test_fn(config, request);

  data_->model->setContactResults("Computed", results);
  ui->tree_view->expandToDepth(1);
}

}  // namespace tesseract_gui
