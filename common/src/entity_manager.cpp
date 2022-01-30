#include <tesseract_gui/common/entity_manager.h>

namespace tesseract_gui
{
EntityID EntityManager::addModel(const std::string& name)
{
  model_id_map_[name] = ++entity_counter_;
  return model_id_map_[name];
}

EntityID EntityManager::getModel(const std::string& name) const
{
  auto it = model_id_map_.find(name);
  if (it == model_id_map_.end())
    return NULL_ENTITY_ID;

  return it->second;
}

const EntityMap& EntityManager::getModels() const { return model_id_map_; }

EntityID EntityManager::addLink(const std::string& name)
{
  link_id_map_[name] = ++entity_counter_;
  return link_id_map_[name];
}

EntityID EntityManager::getLink(const std::string& name) const
{
  auto it = link_id_map_.find(name);
  if (it == link_id_map_.end())
    return NULL_ENTITY_ID;

  return it->second;
}

const EntityMap& EntityManager::getLinks() const { return link_id_map_; }

EntityID EntityManager::addVisual(const std::string& name)
{
  visual_id_map_[name] = ++entity_counter_;
  return visual_id_map_[name];
}

EntityID EntityManager::getVisual(const std::string& name) const
{
  auto it = visual_id_map_.find(name);
  if (it == visual_id_map_.end())
    return NULL_ENTITY_ID;

  return it->second;
}

const EntityMap& EntityManager::getVisuals() const { return visual_id_map_; }

EntityID EntityManager::addSensor(const std::string& name)
{
  sensor_id_map_[name] = ++entity_counter_;
  return sensor_id_map_[name];
}

EntityID EntityManager::getSensor(const std::string& name) const
{
  auto it = sensor_id_map_.find(name);
  if (it == sensor_id_map_.end())
    return NULL_ENTITY_ID;

  return it->second;
}

const EntityMap& EntityManager::getSensors() const { return sensor_id_map_; }

bool EntityManager::empty() const { return (entity_counter_ == 1000); }

void EntityManager::clear()
{
  model_id_map_.clear();
  link_id_map_.clear();
  visual_id_map_.clear();
  sensor_id_map_.clear();
  entity_counter_ = 1000;
}
}  // namespace tesseract_visualization
