#include <tesseract_ignition/ignition_environment_widget.h>
#include <tesseract_ignition/conversions.h>
#include <tesseract_ignition/gui_events.h>
#include <tesseract_ignition/gui_utils.h>
#include <tesseract_qt/common/entity_container.h>
#include <tesseract_environment/commands.h>

#include <ignition/math/eigen3/Conversions.hh>

#include <QUuid>

namespace tesseract_gui
{
IgnitionEnvironmentWidget::IgnitionEnvironmentWidget(std::string scene_name,
                                                     EntityManager& entity_manager,
                                                     QWidget* parent)
  : EnvironmentWidget(parent)
  , container_name_(QUuid::createUuid().toString().toStdString())
  , scene_name_(std::move(scene_name))
  , entity_container_(entity_manager.getEntityContainer(container_name_))
{
  connect(this,
          SIGNAL(selectedLinksChanged(std::vector<std::string>)),
          this,
          SLOT(onSelectedLinksChanged(std::vector<std::string>)));
  connect(this,
          SIGNAL(environmentSet(std::shared_ptr<tesseract_environment::Environment>)),
          this,
          SLOT(onEnvironmentSet(std::shared_ptr<tesseract_environment::Environment>)));
  connect(this,
          SIGNAL(environmentChanged(tesseract_environment::Environment)),
          this,
          SLOT(onEnvironmentChanged(tesseract_environment::Environment)));
  connect(this,
          SIGNAL(environmentCurrentStateChanged(tesseract_environment::Environment)),
          this,
          SLOT(onEnvironmentCurrentStateChanged(tesseract_environment::Environment)));
}

IgnitionEnvironmentWidget::~IgnitionEnvironmentWidget()
{
  ignition::rendering::ScenePtr scene = sceneFromFirstRenderEngine(scene_name_);
  if (scene != nullptr)
  {
    for (const auto& ns : entity_container_->getTrackedEntities())
    {
      for (const auto& entity : ns.second)
        scene->DestroyNodeById(entity.second.id);
    }

    for (const auto& ns : entity_container_->getUntrackedEntities())
    {
      for (const auto& entity : ns.second)
        scene->DestroyNodeById(entity.id);
    }

    entity_container_->clear();
  }
}

tesseract_gui::EnvironmentWidget* IgnitionEnvironmentWidget::clone() const
{
  return new IgnitionEnvironmentWidget(scene_name_, entity_container_->getEntityManager());
}

void IgnitionEnvironmentWidget::onEnvironmentSet(const std::shared_ptr<tesseract_environment::Environment>& /*env*/)
{
  render_revision_ = 0;
  render_dirty_ = true;
  render_reset_ = true;
  render_state_dirty_ = true;
  emit triggerRender();
}

void IgnitionEnvironmentWidget::onEnvironmentChanged(const tesseract_environment::Environment& /*env*/)
{
  render_dirty_ = true;
  render_state_dirty_ = true;
  emit triggerRender();
}

void IgnitionEnvironmentWidget::onEnvironmentCurrentStateChanged(const tesseract_environment::Environment& /*env*/)
{
  render_state_dirty_ = true;
  emit triggerRender();
}

void IgnitionEnvironmentWidget::onLinkVisibleChanged(const std::string& link_name, bool visible)
{
  link_visible_changes_[link_name] = visible;
  render_dirty_ = true;
  emit triggerRender();
}

void IgnitionEnvironmentWidget::onLinkCollisionVisibleChanged(const std::string& link_name, bool visible)
{
  link_collision_visible_changes_[link_name] = visible;
  render_dirty_ = true;
  emit triggerRender();
}

void IgnitionEnvironmentWidget::onKinkVisualVisibleChanged(const std::string& link_name, bool visible)
{
  link_visual_visible_changes_[link_name] = visible;
  render_dirty_ = true;
  emit triggerRender();
}

void IgnitionEnvironmentWidget::onSelectedLinksChanged(const std::vector<std::string>& selected_links)
{
  link_selection_changes_ = selected_links;
  render_dirty_ = true;
  emit triggerRender();
}

bool IgnitionEnvironmentWidget::eventFilter(QObject* _obj, QEvent* _event)
{
  if (_event->type() == events::PreRender::kType)
  {
    assert(dynamic_cast<events::PreRender*>(_event) != nullptr);
    if (static_cast<events::PreRender*>(_event)->getSceneName() == scene_name_ && render_dirty_)
    {
      ignition::rendering::ScenePtr scene = sceneFromFirstRenderEngine(scene_name_);
      if (scene != nullptr)
      {
        if (render_reset_)  // Remove all
        {
          if (!entity_container_->empty())
          {
            for (const auto& ns : entity_container_->getTrackedEntities())
            {
              for (const auto& entity : ns.second)
                scene->DestroyNodeById(entity.second.id);
            }

            for (const auto& ns : entity_container_->getUntrackedEntities())
            {
              for (const auto& entity : ns.second)
                scene->DestroyNodeById(entity.id);
            }

            entity_container_->clear();
            render_link_names_.clear();
            render_revision_ = 0;
            render_state_timestamp_ = environment().getCurrentStateTimestamp();
          }
          render_reset_ = false;
        }

        {  // Check environment
          auto lock = environment().lockRead();
          auto revision = environment().getRevision();
          auto state_timestamp = environment().getCurrentStateTimestamp();
          if (render_dirty_ || revision > render_revision_)
          {
            if (revision > render_revision_)
            {
              tesseract_environment::Commands commands = environment().getCommandHistory();

              bool links_removed{ false };
              for (std::size_t i = render_revision_; i < commands.size(); ++i)
              {
                const tesseract_environment::Command::ConstPtr& command = commands.at(i);
                switch (command->getType())
                {
                  case tesseract_environment::CommandType::ADD_SCENE_GRAPH:
                  {
                    auto cmd = std::static_pointer_cast<const tesseract_environment::AddSceneGraphCommand>(command);
                    auto link_names =
                        loadSceneGraph(*scene, *entity_container_, *cmd->getSceneGraph(), cmd->getPrefix());
                    render_link_names_.insert(render_link_names_.end(), link_names.begin(), link_names.end());
                    break;
                  }
                  case tesseract_environment::CommandType::ADD_LINK:
                  {
                    auto cmd = std::static_pointer_cast<const tesseract_environment::AddLinkCommand>(command);
                    ignition::rendering::VisualPtr root = scene->RootVisual();
                    root->AddChild(loadLink(*scene, *entity_container_, *cmd->getLink()));
                    render_link_names_.push_back(cmd->getLink()->getName());
                    break;
                  }
                  case tesseract_environment::CommandType::CHANGE_LINK_VISIBILITY:
                  {
                    auto cmd =
                        std::static_pointer_cast<const tesseract_environment::ChangeLinkVisibilityCommand>(command);
                    auto entity = entity_container_->getTrackedEntity(tesseract_gui::EntityContainer::VISUAL_NS,
                                                                      cmd->getLinkName());
                    auto visual_node = scene->VisualById(entity.id);
                    if (visual_node != nullptr)
                      visual_node->SetVisible(cmd->getEnabled());
                    break;
                  }
                  case tesseract_environment::CommandType::REMOVE_LINK:
                  case tesseract_environment::CommandType::REMOVE_JOINT:
                  {
                    links_removed = true;
                    break;
                  }
                  case tesseract_environment::CommandType::MOVE_LINK:
                  case tesseract_environment::CommandType::MOVE_JOINT:
                  case tesseract_environment::CommandType::REPLACE_JOINT:
                  case tesseract_environment::CommandType::CHANGE_JOINT_ORIGIN:
                  case tesseract_environment::CommandType::CHANGE_LINK_ORIGIN:
                  {
                    render_state_dirty_ = true;
                    break;
                  }
                  case tesseract_environment::CommandType::CHANGE_LINK_COLLISION_ENABLED:
                  case tesseract_environment::CommandType::ADD_ALLOWED_COLLISION:
                  case tesseract_environment::CommandType::REMOVE_ALLOWED_COLLISION:
                  case tesseract_environment::CommandType::REMOVE_ALLOWED_COLLISION_LINK:
                  case tesseract_environment::CommandType::CHANGE_JOINT_POSITION_LIMITS:
                  case tesseract_environment::CommandType::CHANGE_JOINT_VELOCITY_LIMITS:
                  case tesseract_environment::CommandType::CHANGE_JOINT_ACCELERATION_LIMITS:
                  case tesseract_environment::CommandType::ADD_KINEMATICS_INFORMATION:
                  case tesseract_environment::CommandType::CHANGE_COLLISION_MARGINS:
                  case tesseract_environment::CommandType::ADD_CONTACT_MANAGERS_PLUGIN_INFO:
                  case tesseract_environment::CommandType::SET_ACTIVE_CONTINUOUS_CONTACT_MANAGER:
                  case tesseract_environment::CommandType::SET_ACTIVE_DISCRETE_CONTACT_MANAGER:
                  {
                    break;
                  }
                  // LCOV_EXCL_START
                  default:
                  {
                    CONSOLE_BRIDGE_logError("IgnitionEnvironmentWidget, Unhandled environment command");
                    //                success &= false;
                  }
                    // LCOV_EXCL_STOP
                }
              }

              if (links_removed)
              {
                std::vector<std::string> link_names = environment().getLinkNames();
                std::vector<std::string> diff;

                std::sort(link_names.begin(), link_names.end());
                std::sort(render_link_names_.begin(), render_link_names_.end());

                std::set_difference(render_link_names_.begin(),
                                    render_link_names_.end(),
                                    link_names.begin(),
                                    link_names.end(),
                                    std::inserter(diff, diff.begin()));

                for (const auto& removed_link : diff)
                {
                  auto entity =
                      entity_container_->getTrackedEntity(tesseract_gui::EntityContainer::VISUAL_NS, removed_link);
                  scene->DestroyNodeById(entity.id);
                }
              }
              render_revision_ = revision;
            }
          }

          if (render_state_dirty_ || state_timestamp > render_state_timestamp_)
          {
            tesseract_scene_graph::SceneState state = environment().getState();
            for (const auto& pair : state.link_transforms)
            {
              Entity entity =
                  entity_container_->getTrackedEntity(tesseract_gui::EntityContainer::VISUAL_NS, pair.first);
              scene->VisualById(entity.id)->SetWorldPose(ignition::math::eigen3::convert(pair.second));
            }
            render_state_timestamp_ = state_timestamp;
          }
        }

        if (render_dirty_)
        {
          for (const auto& l : link_visible_changes_)
          {
            auto entity = entity_container_->getTrackedEntity(tesseract_gui::EntityContainer::VISUAL_NS, l.first);
            auto visual_node = scene->VisualById(entity.id);
            if (visual_node != nullptr)
              visual_node->SetVisible(l.second);
          }

          for (const auto& l : link_visual_visible_changes_)
          {
            std::string visual_key = l.first + "::Visuals";
            auto entity = entity_container_->getTrackedEntity(tesseract_gui::EntityContainer::VISUAL_NS, visual_key);
            auto visual_node = scene->VisualById(entity.id);
            if (visual_node != nullptr)
              visual_node->SetVisible(l.second);
          }

          for (const auto& l : link_collision_visible_changes_)
          {
            std::string visual_key = l.first + "::Collisions";
            auto entity = entity_container_->getTrackedEntity(tesseract_gui::EntityContainer::VISUAL_NS, visual_key);
            auto visual_node = scene->VisualById(entity.id);
            if (visual_node != nullptr)
              visual_node->SetVisible(l.second);
          }

          for (const auto& entity : highlighted_entities_)
          {
            auto visual_node = scene->VisualById(entity.id);
            if (visual_node != nullptr)
              visual_node->SetVisible(false);
          }
          highlighted_entities_.clear();

          for (const auto& l : link_selection_changes_)
          {
            std::string visual_key = l + "::WireBox";
            auto entity = entity_container_->getTrackedEntity(tesseract_gui::EntityContainer::VISUAL_NS, visual_key);
            auto visual_node = scene->VisualById(entity.id);
            if (visual_node != nullptr)
            {
              visual_node->SetVisible(true);
              highlighted_entities_.push_back(entity);
            }
          }
        }
        render_dirty_ = false;
      }
    }
  }

  // Standard event processing
  return QObject::eventFilter(_obj, _event);
}

}  // namespace tesseract_gui
