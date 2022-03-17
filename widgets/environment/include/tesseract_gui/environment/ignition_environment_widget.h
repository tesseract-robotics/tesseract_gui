#ifndef TESSERACT_GUI_ENVIRONMENT_IGNITION_ENVIRONMENT_WIDGET_H
#define TESSERACT_GUI_ENVIRONMENT_IGNITION_ENVIRONMENT_WIDGET_H

#include <tesseract_gui/environment/environment_widget.h>
#include <tesseract_gui/common/entity_manager.h>
#include <tesseract_gui/common/entity_container.h>
#include <unordered_map>

namespace tesseract_gui
{
class IgnitionEnvironmentWidget : public EnvironmentWidget
{
  Q_OBJECT
public:
  explicit IgnitionEnvironmentWidget(std::string scene_name, EntityManager &entity_manager, QWidget *parent = nullptr);
  ~IgnitionEnvironmentWidget() override;

private Q_SLOTS:
  void onEnvironmentSet(const tesseract_environment::Environment& env);
  void onEnvironmentCommandsApplied(const tesseract_environment::Commands& commands);
  void onLinkVisibleChanged(const std::string& link_name, bool visible);
  void onLinkCollisionVisibleChanged(const std::string& link_name, bool visible);
  void onKinkVisualVisibleChanged(const std::string& link_name, bool visible);

protected:
  std::string container_name_;
  std::string scene_name_;
  EntityContainer::Ptr entity_container_;
  int render_revision_{0};
  bool render_dirty_{true};
  bool render_reset_{true};
  bool render_state_dirty_{true};
  std::unordered_map<std::string, bool> link_visible_changes_;
  std::unordered_map<std::string, bool> link_collision_visible_changes_;
  std::unordered_map<std::string, bool> link_visual_visible_changes_;

  // Documentation inherited
  bool eventFilter(QObject *_obj, QEvent *_event) override;
};

}

#endif // TESSERACT_GUI_ENVIRONMENT_IGNITION_ENVIRONMENT_WIDGET_H
