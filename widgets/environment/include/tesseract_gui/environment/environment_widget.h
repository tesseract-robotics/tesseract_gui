#ifndef TESSERACT_GUI_ENVIRONMENT_ENVIRONMENT_WIDGET_H
#define TESSERACT_GUI_ENVIRONMENT_ENVIRONMENT_WIDGET_H

#include <QWidget>
#include <memory>
#include <tesseract_environment/environment.h>

namespace Ui {
class EnvironmentWidget;
}

namespace tesseract_gui
{
struct EnvironmentWidgetImpl;

class EnvironmentWidget : public QWidget
{
  Q_OBJECT

public:
  explicit EnvironmentWidget(QWidget *parent = nullptr);
  ~EnvironmentWidget();

  void addEnvironment(tesseract_environment::Environment::ConstPtr env);
  void removeEnvironment(const QString& env_name);

public Q_SLOTS:
  void onCurrentEnvironmentChanged(const QString& env_name);

private:
  std::unique_ptr<Ui::EnvironmentWidget> ui;
  std::unique_ptr<EnvironmentWidgetImpl> data_;

  // Documentation inherited
//  bool eventFilter(QObject *_obj, QEvent *_event) override;

};
}

#endif // TESSERACT_GUI_ENVIRONMENT_ENVIRONMENT_WIDGET_H
