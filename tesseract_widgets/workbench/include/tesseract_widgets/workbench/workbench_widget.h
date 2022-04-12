#ifndef WORKBENCH_WIDGET_H
#define WORKBENCH_WIDGET_H

#include <QWidget>
#include <memory>

namespace Ui {
class WorkbenchWidget;
}

namespace tesseract_gui
{
class EnvironmentWidget;
struct WorkbenchWidgetImpl;

class WorkbenchWidget : public QWidget
{
  Q_OBJECT

public:
  explicit WorkbenchWidget(EnvironmentWidget* environment_widget, QWidget *parent = nullptr);
  ~WorkbenchWidget();

//  EnvironmentWidget& getEnvironmentWidget();
//  const EnvironmentWidget& getEnvironmentWidget() const;

private:
  std::unique_ptr<Ui::WorkbenchWidget> ui;
  std::unique_ptr<WorkbenchWidgetImpl> data_;
};
}
#endif // WORKBENCH_WIDGET_H
