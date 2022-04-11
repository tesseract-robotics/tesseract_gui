#include <tesseract_widgets/common/entity_manager.h>
#include <tesseract_widgets/common/entity_container.h>

namespace tesseract_gui
{
int EntityManager::entity_counter_ = 1000; // NOLINT

std::shared_ptr<EntityContainer> EntityManager::getEntityContainer(const std::string& name)
{
  std::unique_lock<std::shared_mutex> lock(mutex_);
  auto c_it = containers_.find(name);
  if (c_it == containers_.end())
  {
    auto container = std::make_shared<EntityContainer>(shared_from_this(), name);
    containers_[name] = container;
    containers_const_[name] = container;
    return container;
  }

  return c_it->second;
}

std::shared_ptr<const EntityContainer> EntityManager::getEntityContainer(const std::string& name) const
{
  std::shared_lock<std::shared_mutex> lock(mutex_);
  auto c_it = containers_.find(name);
  if (c_it == containers_.end())
    return nullptr;

  return c_it->second;
}

bool EntityManager::hasEntityContainer(const std::string& name) const
{
  std::shared_lock<std::shared_mutex> lock(mutex_);
  return (containers_.find(name) != containers_.end());
}

void EntityManager::removeEntityContainer(const std::string& name)
{
  std::unique_lock<std::shared_mutex> lock(mutex_);
  containers_.erase(name);
  containers_const_.erase(name);
}

std::unordered_map<std::string, std::shared_ptr<EntityContainer>> EntityManager::getEntityContainers()
{
  std::shared_lock<std::shared_mutex> lock(mutex_);
  return containers_;
}

std::unordered_map<std::string, std::shared_ptr<const EntityContainer>> EntityManager::getEntityContainers() const
{
  std::shared_lock<std::shared_mutex> lock(mutex_);
  return containers_const_;
}

Entity EntityManager::createEntity()
{
  std::unique_lock<std::shared_mutex> lock(mutex_);
  Entity entity;
  entity.id = ++entity_counter_;
  entity.unique_name = "TesseractEntity::" + std::to_string(entity.id);
  return entity;
}

bool EntityManager::empty() const
{
  std::shared_lock<std::shared_mutex> lock(mutex_);
  for (const auto& c : containers_)
  {
    if (!c.second->empty())
      return false;
  }

  return true;
}

void EntityManager::clear()
{
  std::unique_lock<std::shared_mutex> lock(mutex_);
  for (auto& c : containers_)
    c.second->clear();

  containers_.clear();
}
}  // namespace tesseract_visualization
