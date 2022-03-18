#include <tesseract_gui/common/entity_container.h>
#include <tesseract_gui/common/entity_manager.h>

namespace tesseract_gui
{

EntityContainer::EntityContainer(std::shared_ptr<EntityManager> manager)
  : manager_(std::move(manager))
{
}

EntityID EntityContainer::createEntityID()
{
  return manager_->createEntityID();
}

EntityID EntityContainer::addVisual(const std::string& name)
{
  std::unique_lock<std::shared_mutex> lock(mutex_);
  EntityID id = manager_->createEntityID();
  visual_id_map_[name] = id;
  return id;
}

EntityID EntityContainer::getVisual(const std::string& name) const
{
  std::shared_lock<std::shared_mutex> lock(mutex_);
  auto it = visual_id_map_.find(name);
  if (it == visual_id_map_.end())
    return NULL_ENTITY_ID;

  return it->second;
}

EntityMap EntityContainer::getVisuals() const
{
  std::shared_lock<std::shared_mutex> lock(mutex_);
  return visual_id_map_;
}

EntityID EntityContainer::addSensor(const std::string& name)
{
  std::unique_lock<std::shared_mutex> lock(mutex_);
  EntityID id = manager_->createEntityID();
  sensor_id_map_[name] = id;
  return id;
}

EntityID EntityContainer::getSensor(const std::string& name) const
{
  std::shared_lock<std::shared_mutex> lock(mutex_);
  auto it = sensor_id_map_.find(name);
  if (it == sensor_id_map_.end())
    return NULL_ENTITY_ID;

  return it->second;
}

EntityMap EntityContainer::getSensors() const
{
  std::shared_lock<std::shared_mutex> lock(mutex_);
  return sensor_id_map_;
}

bool EntityContainer::empty() const
{
  std::shared_lock<std::shared_mutex> lock(mutex_);
  return (visual_id_map_.empty() && sensor_id_map_.empty());
}

void EntityContainer::clear()
{
  std::unique_lock<std::shared_mutex> lock(mutex_);
  visual_id_map_.clear();
  sensor_id_map_.clear();
}
}  // namespace tesseract_gui
