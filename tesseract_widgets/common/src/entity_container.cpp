#include <tesseract_widgets/common/entity_container.h>
#include <tesseract_widgets/common/entity_manager.h>

namespace tesseract_gui
{

EntityContainer::EntityContainer(std::shared_ptr<EntityManager> manager, std::string name)
  : manager_(std::move(manager))
  , name_(std::move(name))
{
}

std::string EntityContainer::getName() const
{
  return name_;
}

Entity EntityContainer::addUntracked()
{
  std::unique_lock<std::shared_mutex> lock(mutex_);
  Entity entity = manager_->createEntity();
  untracked_entities_.push_back(entity);
  return entity;
}

EntityVector EntityContainer::getUntracked() const
{
  std::shared_lock<std::shared_mutex> lock(mutex_);
  return untracked_entities_;
}

Entity EntityContainer::addVisual(const std::string& name)
{
  std::unique_lock<std::shared_mutex> lock(mutex_);
  Entity entity = manager_->createEntity();
  visual_entity_map_[name] = entity;
  return entity;
}

Entity EntityContainer::getVisual(const std::string& name) const
{
  std::shared_lock<std::shared_mutex> lock(mutex_);
  auto it = visual_entity_map_.find(name);
  if (it == visual_entity_map_.end())
    throw std::runtime_error("Visual entity does not exist for name '" + name  + '.');

  return it->second;
}

bool EntityContainer::hasVisual(const std::string& name) const
{
  std::shared_lock<std::shared_mutex> lock(mutex_);
  return (visual_entity_map_.find(name) == visual_entity_map_.end());
}

EntityMap EntityContainer::getVisuals() const
{
  std::shared_lock<std::shared_mutex> lock(mutex_);
  return visual_entity_map_;
}

Entity EntityContainer::addSensor(const std::string& name)
{
  std::unique_lock<std::shared_mutex> lock(mutex_);
  Entity entity = manager_->createEntity();
  sensor_entity_map_[name] = entity;
  return entity;
}

Entity EntityContainer::getSensor(const std::string& name) const
{
  std::shared_lock<std::shared_mutex> lock(mutex_);
  auto it = sensor_entity_map_.find(name);
  if (it == sensor_entity_map_.end())
    throw std::runtime_error("Sensor entity does not exist for name '" + name  + '.');

  return it->second;
}

bool EntityContainer::hasSensor(const std::string& name) const
{
  std::shared_lock<std::shared_mutex> lock(mutex_);
  return (sensor_entity_map_.find(name) == sensor_entity_map_.end());
}

EntityMap EntityContainer::getSensors() const
{
  std::shared_lock<std::shared_mutex> lock(mutex_);
  return sensor_entity_map_;
}

bool EntityContainer::empty() const
{
  std::shared_lock<std::shared_mutex> lock(mutex_);
  return (visual_entity_map_.empty() && sensor_entity_map_.empty() && untracked_entities_.empty());
}

void EntityContainer::clear()
{
  std::unique_lock<std::shared_mutex> lock(mutex_);
  visual_entity_map_.clear();
  sensor_entity_map_.clear();
  untracked_entities_.clear();
}

}  // namespace tesseract_gui
