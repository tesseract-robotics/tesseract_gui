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
  void setEnvironment(tesseract_environment::Environment::UPtr env);

  /**
   * @brief The the environment
   * @return The environment
   */
  const tesseract_environment::Environment* getEnvironment() const;

  /**
   * @brief Applies the commands to the environment
   * @param commands Commands to be applied to the environment
   * @return true if successful. If returned false, then only a partial set of commands have been applied. Call
   * getCommandHistory to check. Some commands are not checked for success
   */
  bool applyCommands(const tesseract_environment::Commands& commands);

  /**
   * @brief Apply command to the environment
   * @param command Command to be applied to the environment
   * @return true if successful. If returned false, then the command have not been applied.
   * Some type of Command are not checked for success
   */
  bool applyCommand(const tesseract_environment::Command::ConstPtr& command);


Q_SIGNALS:
  void environmentSet(const tesseract_environment::Environment& env);
  void environmentCommandsApplied(const tesseract_environment::Commands& commands);
  void linkVisibleChanged(const std::string& link_name, bool visible);
  void linkCollisionVisibleChanged(const std::string& link_name, bool visible);
  void linkVisualVisibleChanged(const std::string& link_name, bool visible);

protected:
  std::unique_ptr<Ui::EnvironmentWidget> ui;
  std::unique_ptr<EnvironmentWidgetImpl> data_;

  void updatedModels();
};
}

#endif // TESSERACT_GUI_ENVIRONMENT_ENVIRONMENT_WIDGET_H
