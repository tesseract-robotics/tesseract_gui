#ifndef TESSERACT_WIDGETS_ENVIRONMENT_IGNITION_ENVIRONMENT_WIDGET_H
#define TESSERACT_WIDGETS_ENVIRONMENT_IGNITION_ENVIRONMENT_WIDGET_H

#include <tesseract_widgets/environment/environment_widget.h>
#include <tesseract_widgets/common/entity_manager.h>
#include <tesseract_widgets/common/entity_container.h>
#include <unordered_map>

namespace tesseract_gui
{
class IgnitionEnvironmentWidget : public EnvironmentWidget
{
  Q_OBJECT
public:
  explicit IgnitionEnvironmentWidget(std::string scene_name, EntityManager& entity_manager, QWidget* parent = nullptr);
  ~IgnitionEnvironmentWidget() override;

  tesseract_gui::EnvironmentWidget* clone() const override;

private Q_SLOTS:
  void onEnvironmentSet(const std::shared_ptr<tesseract_environment::Environment>& env);
  void onEnvironmentChanged(const tesseract_environment::Environment& env);
  void onEnvironmentCurrentStateChanged(const tesseract_environment::Environment& env);
  void onLinkVisibleChanged(const std::string& link_name, bool visible);
  void onLinkCollisionVisibleChanged(const std::string& link_name, bool visible);
  void onKinkVisualVisibleChanged(const std::string& link_name, bool visible);
  void onSelectedLinksChanged(const std::vector<std::string>& selected_links);

protected:
  std::string container_name_;
  std::string scene_name_;
  EntityContainer::Ptr entity_container_;
  long render_revision_{ 0 };
  std::chrono::system_clock::time_point render_state_timestamp_{ std::chrono::system_clock::now() };
  std::vector<std::string> render_link_names_;
  bool render_dirty_{ true };
  bool render_reset_{ true };
  bool render_state_dirty_{ true };
  std::unordered_map<std::string, bool> link_visible_changes_;
  std::unordered_map<std::string, bool> link_collision_visible_changes_;
  std::unordered_map<std::string, bool> link_visual_visible_changes_;
  std::vector<std::string> link_selection_changes_;
  std::vector<Entity> highlighted_entities_;

  // Documentation inherited
  bool eventFilter(QObject* _obj, QEvent* _event) override;
};

}  // namespace tesseract_gui

#endif  // TESSERACT_WIDGETS_ENVIRONMENT_IGNITION_ENVIRONMENT_WIDGET_H
