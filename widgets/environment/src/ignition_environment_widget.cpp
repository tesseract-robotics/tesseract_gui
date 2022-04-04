#include <tesseract_gui/widgets/environment/ignition_environment_widget.h>
#include <tesseract_gui/widgets/environment/conversions.h>
#include <tesseract_gui/common/entity_container.h>
#include <tesseract_gui/common/gui_events.h>
#include <tesseract_gui/common/gui_utils.h>
#include <tesseract_environment/commands.h>

#include <ignition/math/eigen3/Conversions.hh>

#include <QUuid>

namespace tesseract_gui
{
IgnitionEnvironmentWidget::IgnitionEnvironmentWidget(std::string scene_name,
                                                     EntityManager& entity_manager,
                                                     QWidget *parent)
: EnvironmentWidget(parent)
, container_name_(QUuid::createUuid().toString().toStdString())
, scene_name_(std::move(scene_name))
, entity_container_(entity_manager.getEntityContainer(container_name_))
{
}

IgnitionEnvironmentWidget::~IgnitionEnvironmentWidget() = default;

void IgnitionEnvironmentWidget::onEnvironmentSet(const tesseract_environment::Environment& env)
{
  render_revision_ = 0;
  render_dirty_ = true;
  render_reset_ = true;
  render_state_dirty_ = true;
}

void IgnitionEnvironmentWidget::onEnvironmentCommandsApplied(const tesseract_environment::Commands& commands)
{
  render_dirty_ = true;
  render_state_dirty_ = true;
}

void IgnitionEnvironmentWidget::onLinkVisibleChanged(const std::string& link_name, bool visible)
{
  link_visible_changes_[link_name] = visible;
  render_dirty_ = true;
}

void IgnitionEnvironmentWidget::onLinkCollisionVisibleChanged(const std::string& link_name, bool visible)
{
  link_collision_visible_changes_[link_name] = visible;
  render_dirty_ = true;
}

void IgnitionEnvironmentWidget::onKinkVisualVisibleChanged(const std::string& link_name, bool visible)
{
  link_visual_visible_changes_[link_name] = visible;
  render_dirty_ = true;
}

bool IgnitionEnvironmentWidget::eventFilter(QObject *_obj, QEvent *_event)
{
  if (_event->type() == events::Render::kType)
  {
    assert(dynamic_cast<events::Render*>(_event) != nullptr);
    if (static_cast<events::Render*>(_event)->getSceneName() == scene_name_ && render_dirty_)
    {
      ignition::rendering::ScenePtr scene = sceneFromFirstRenderEngine(scene_name_);
      if (scene != nullptr)
      {
        if (render_reset_) // Remove all
        {
          if (!entity_container_->empty())
          {
            for (const auto& id : entity_container_->getVisuals())
              scene->DestroyNodeById(id.second);

            for (const auto& id : entity_container_->getSensors())
              scene->DestroyNodeById(id.second);

            entity_container_->clear();
          }
          render_reset_ = false;
        }

        if (render_dirty_)
        {
          auto revision = getEnvironment()->getRevision();
          if (revision > render_revision_)
          {
            auto commands = getEnvironment()->getCommandHistory();
            for (std::size_t i = render_revision_; i < commands.size(); ++i)
            {
              const tesseract_environment::Command::ConstPtr& command = commands.at(i);
              switch (command->getType())
              {
                case tesseract_environment::CommandType::ADD_LINK:
                {
                  auto cmd = std::static_pointer_cast<const tesseract_environment::AddLinkCommand>(command);
  //                success &= applyAddCommand(cmd);
                  break;
                }
                case tesseract_environment::CommandType::REMOVE_LINK:
                {
                  auto cmd = std::static_pointer_cast<const tesseract_environment::RemoveLinkCommand>(command);
  //                success &= applyRemoveLinkCommand(cmd);
                  break;
                }
                case tesseract_environment::CommandType::REMOVE_JOINT:
                {
                  auto cmd = std::static_pointer_cast<const tesseract_environment::RemoveJointCommand>(command);
  //                success &= applyRemoveJointCommand(cmd);
                  break;
                }
                case tesseract_environment::CommandType::CHANGE_LINK_ORIGIN:
                {
                  auto cmd = std::static_pointer_cast<const tesseract_environment::ChangeLinkOriginCommand>(command);
  //                success &= applyChangeLinkOriginCommand(cmd);
                  break;
                }
                case tesseract_environment::CommandType::CHANGE_LINK_VISIBILITY:
                {
                  auto cmd = std::static_pointer_cast<const tesseract_environment::ChangeLinkVisibilityCommand>(command);
  //                success &= applyChangeLinkVisibilityCommand(cmd);
                  break;
                }
                case tesseract_environment::CommandType::ADD_SCENE_GRAPH:
                {
                  auto cmd = std::static_pointer_cast<const tesseract_environment::AddSceneGraphCommand>(command);
                  loadSceneGraph(*scene, *entity_container_, *cmd->getSceneGraph());
                  break;
                }
                case tesseract_environment::CommandType::MOVE_LINK:
                case tesseract_environment::CommandType::MOVE_JOINT:
                case tesseract_environment::CommandType::REPLACE_JOINT:
                case tesseract_environment::CommandType::CHANGE_JOINT_ORIGIN:
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
          }

          for (const auto& l : link_visible_changes_)
          {
            auto lc = entity_container_->getVisual(l.first);
            auto visual_node = scene->VisualById(lc);
            if (visual_node != nullptr)
              visual_node->SetVisible(l.second);
          }

          for (const auto& l : link_visual_visible_changes_)
          {
            std::string visual_key = l.first + "::Visuals";
            auto lc = entity_container_->getVisual(visual_key);
            auto visual_node = scene->VisualById(lc);
            if (visual_node != nullptr)
              visual_node->SetVisible(l.second);
          }

          for (const auto& l : link_collision_visible_changes_)
          {
            std::string visual_key = l.first + "::Collisions";
            auto lc = entity_container_->getVisual(visual_key);
            auto visual_node = scene->VisualById(lc);
            if (visual_node != nullptr)
              visual_node->SetVisible(l.second);
          }
        }
        render_dirty_ = false;

        if (render_state_dirty_)
        {
          tesseract_scene_graph::SceneState state = getEnvironment()->getState();
          for (const auto& pair : state.link_transforms)
          {
            EntityID id = entity_container_->getVisual(pair.first);
            scene->VisualById(id)->SetWorldPose(ignition::math::eigen3::convert(pair.second));
          }
        }
        render_state_dirty_ = false;
      }
    }
  }

  // Standard event processing
  return QObject::eventFilter(_obj, _event);
}
}
