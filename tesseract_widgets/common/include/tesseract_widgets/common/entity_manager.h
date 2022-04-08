#ifndef TESSERACT_GUI_COMMON_ENTITY_MANAGER_H
#define TESSERACT_GUI_COMMON_ENTITY_MANAGER_H

#include <tesseract_widgets/common/entity.h>
#include <memory>
#include <shared_mutex>

namespace tesseract_gui
{

class EntityContainer;

class EntityManager : public std::enable_shared_from_this<EntityManager>
{
public:
  using Ptr = std::shared_ptr<EntityManager>;
  using ConstPtr = std::shared_ptr<const EntityManager>;

  /**
   * @brief Get entity container. If it does not exist one is created.
   * @param name The name of the container
   * @return The entity container
   */
  std::shared_ptr<EntityContainer> getEntityContainer(const std::string& name);
  std::shared_ptr<const EntityContainer> getEntityContainer(const std::string& name) const;

  /**
   * @brief Create new entity ID
   * @return A new entity ID
   */
  EntityID createEntityID();

  /** @brief Check if empty */
  bool empty() const;

  /** @brief Clear internal data */
  void clear();

private:
  mutable std::shared_mutex mutex_;
  std::unordered_map<std::string, std::shared_ptr<EntityContainer>> containers_;
  EntityID entity_counter_{ 1000 }; /**< Start entity counter to avoid clashing with gazebo */
};
}
#endif // TESSERACT_GUI_COMMON_ENTITY_MANAGER_H
