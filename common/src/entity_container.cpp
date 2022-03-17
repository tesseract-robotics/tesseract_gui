#include <tesseract_gui/common/entity_container.h>
#include <tesseract_gui/common/entity_manager.h>

namespace tesseract_gui
{

LinkEntityContainer::LinkEntityContainer(std::shared_ptr<EntityManager> manager)
  : manager_(std::move(manager))
  , id_(manager_->createEntityId())
{
}

EntityID LinkEntityContainer::getId() const
{
  std::shared_lock<std::shared_mutex> lock(mutex_);
  return id_;
}

/**
 * @brief Add visual to manager and return id for visual
 * @return The Entity ID
 */
EntityID LinkEntityContainer::addVisual()
{
  std::unique_lock<std::shared_mutex> lock(mutex_);
  EntityID id = manager_->createEntityId();
  visual_entities_.push_back(id);
  return id;
}

/**
 * @brief Get all visuals being managed
 * @return A vector of entity id's
 */
std::vector<EntityID> LinkEntityContainer::getVisuals() const
{
  std::shared_lock<std::shared_mutex> lock(mutex_);
  return visual_entities_;
}

/**
 * @brief Add collision to manager and return id for visual
 * @return The Entity ID
 */
EntityID LinkEntityContainer::addCollision()
{
  std::unique_lock<std::shared_mutex> lock(mutex_);
  EntityID id = manager_->createEntityId();
  collision_entities_.push_back(id);
  return id;
}

/**
 * @brief Get all collisions being managed
 * @return A vector of entity id's
 */
std::vector<EntityID> LinkEntityContainer::getCollisions() const
{
  std::shared_lock<std::shared_mutex> lock(mutex_);
  return collision_entities_;
}

EntityContainer::EntityContainer(std::shared_ptr<EntityManager> manager)
  : manager_(std::move(manager))
{
}

LinkEntityContainer::Ptr EntityContainer::addLink(const std::string& name)
{
  std::unique_lock<std::shared_mutex> lock(mutex_);
  auto lc = std::make_shared<LinkEntityContainer>(manager_);
  link_map_[name] = lc;
  return lc;
}

LinkEntityContainer::Ptr EntityContainer::getLink(const std::string& name) const
{
  std::shared_lock<std::shared_mutex> lock(mutex_);
  auto it = link_map_.find(name);
  if (it == link_map_.end())
    return nullptr;

  return it->second;
}

LinkEntityContainerMap EntityContainer::getLinks() const
{
  std::shared_lock<std::shared_mutex> lock(mutex_);
  return link_map_;
}

EntityID EntityContainer::addVisual(const std::string& name)
{
  std::unique_lock<std::shared_mutex> lock(mutex_);
  EntityID id = manager_->createEntityId();
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
  EntityID id = manager_->createEntityId();
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
  return (link_map_.empty() && visual_id_map_.empty() && sensor_id_map_.empty());
}

void EntityContainer::clear()
{
  std::unique_lock<std::shared_mutex> lock(mutex_);
  link_map_.clear();
  visual_id_map_.clear();
  sensor_id_map_.clear();
}
}  // namespace tesseract_gui
