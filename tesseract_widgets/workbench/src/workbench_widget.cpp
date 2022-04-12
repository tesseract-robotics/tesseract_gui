#include <tesseract_widgets/workbench/workbench_widget.h>
#include <tesseract_widgets/environment/environment_widget.h>
#include <tesseract_widgets/joint_trajectory/joint_trajectory_widget.h>

#include "ui_workbench_widget.h"

#include <QVBoxLayout>

namespace tesseract_gui
{

struct WorkbenchWidgetImpl
{
  EnvironmentWidget* environment_widget;
  JointTrajectoryWidget* joint_trajectory_widget;
};

WorkbenchWidget::WorkbenchWidget(EnvironmentWidget* environment_widget, QWidget *parent)
  : QWidget(parent)
  , ui(std::make_unique<Ui::WorkbenchWidget>())
  , data_(std::make_unique<WorkbenchWidgetImpl>())
{
  ui->setupUi(this);
  data_->environment_widget = environment_widget;
  data_->joint_trajectory_widget = new JointTrajectoryWidget();

  { // Add environment widget
    auto* layout = new QVBoxLayout(); // NOLINT
    layout->setMargin(0);
    layout->addWidget(environment_widget);
    ui->environment_tab->setLayout(layout);
  }

  { // Add joint trajectory widget
    auto* layout = new QVBoxLayout(); // NOLINT
    layout->setMargin(0);
    layout->addWidget(data_->joint_trajectory_widget);
    ui->trajectory_tab->setLayout(layout);
  }
}

WorkbenchWidget::~WorkbenchWidget() = default;
}
