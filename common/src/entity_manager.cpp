#include <tesseract_gui/common/entity_manager.h>
#include <tesseract_gui/common/entity_container.h>

namespace tesseract_gui
{

std::shared_ptr<EntityContainer> EntityManager::getEntityContainer(const std::string& container_name)
{
  std::unique_lock<std::shared_mutex> lock(mutex_);
  auto c_it = containers_.find(container_name);
  if (c_it == containers_.end())
  {
    auto container = std::make_shared<EntityContainer>(shared_from_this());
    containers_[container_name] = container;
    return container;
  }

  return c_it->second;
}

std::shared_ptr<const EntityContainer> EntityManager::getEntityContainer(const std::string& container_name) const
{
  std::shared_lock<std::shared_mutex> lock(mutex_);
  auto c_it = containers_.find(container_name);
  if (c_it == containers_.end())
    return nullptr;

  return c_it->second;
}

EntityID EntityManager::createEntityID()
{
  std::unique_lock<std::shared_mutex> lock(mutex_);
  return ++entity_counter_;
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
  entity_counter_ = 1000;
}
}  // namespace tesseract_visualization
