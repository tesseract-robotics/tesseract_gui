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
  ~EnvironmentWidget() override;

  /**
   * @brief Set environment
   * @param env The environment
   * @param scene_name
   */
  void setEnvironment(tesseract_environment::Environment::Ptr env);

  /**
   * @brief The the environment
   * @return The environment
   */
  const tesseract_environment::Environment& environment() const;
  tesseract_environment::Environment& environment();

  /**
   * @brief The the environment
   * @return The environment
   */
  tesseract_environment::Environment::ConstPtr getEnvironment() const;
  tesseract_environment::Environment::Ptr getEnvironment();

Q_SIGNALS:
  void environmentSet(const tesseract_environment::Environment& env);
  void environmentChanged(const tesseract_environment::Environment& env);
  void environmentCurrentStateChanged(const tesseract_environment::Environment& env);
  void linkVisibleChanged(const std::string& link_name, bool visible);
  void linkCollisionVisibleChanged(const std::string& link_name, bool visible);
  void linkVisualVisibleChanged(const std::string& link_name, bool visible);
  void selectedLinksChanged(const std::vector<std::string>& selected_links);
  void triggerRender();

protected:
  std::unique_ptr<Ui::EnvironmentWidget> ui;
  std::unique_ptr<EnvironmentWidgetImpl> data_;

  void updateModels();
  void updateSceneGraphModel();
  void updateCurrentStateModel();
  void updateAllowedCollisionMatrixModel();
  void updateKinematicsInformationModels();
  void updateCommandHistoryModel();

  void tesseractEventFilter(const tesseract_environment::Event& event);

};
}

#endif // TESSERACT_GUI_ENVIRONMENT_ENVIRONMENT_WIDGET_H
