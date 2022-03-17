#ifndef TESSERACT_GUI_COMMON_ENTITY_CONTAINER_H
#define TESSERACT_GUI_COMMON_ENTITY_CONTAINER_H

#include <tesseract_gui/common/entity.h>
#include <memory>
#include <vector>
#include <shared_mutex>

namespace tesseract_gui
{
class EntityManager;

class LinkEntityContainer
{
public:
  using Ptr = std::shared_ptr<LinkEntityContainer>;
  using ConstPtr = std::shared_ptr<const LinkEntityContainer>;

  LinkEntityContainer(std::shared_ptr<EntityManager> manager);

  /**
   * @brief Get Id associated with link container
   * @return The link entity Id
   */
  EntityID getId() const;

  /**
   * @brief Add visual to manager and return id for visual
   * @return The Entity ID
   */
  EntityID addVisual();

  /**
   * @brief Get all visuals being managed
   * @return A vector of entity id's
   */
  std::vector<EntityID> getVisuals() const;

  /**
   * @brief Add collision to manager and return id for visual
   * @return The Entity ID
   */
  EntityID addCollision();

  /**
   * @brief Get all collisions being managed
   * @return A vector of entity id's
   */
  std::vector<EntityID> getCollisions() const;


private:
  std::shared_ptr<EntityManager> manager_;
  mutable std::shared_mutex mutex_;
  EntityID id_;
  std::vector<EntityID> visual_entities_;
  std::vector<EntityID> collision_entities_;
};

using LinkEntityContainerMap = std::unordered_map<std::string, LinkEntityContainer::Ptr>;

class EntityContainer
{
public:
  using Ptr = std::shared_ptr<EntityContainer>;
  using ConstPtr = std::shared_ptr<const EntityContainer>;

  EntityContainer(std::shared_ptr<EntityManager> manager);

  /**
   * @brief Add link name to manager and return id for link
   * @param name Name given to the link
   * @return The Entity ID
   */
  LinkEntityContainer::Ptr addLink(const std::string& name);

  /**
   * @brief Given the link name return the ID
   * @param name Name of the link
   * @return The ID of the link (if < 1000 it was not found)
   */
  LinkEntityContainer::Ptr getLink(const std::string& name) const;

  /**
   * @brief Get all links being managed
   * @return A map of names to entity id's
   */
  LinkEntityContainerMap getLinks() const;

  /**
   * @brief Add visual name to manager and return id for visual
   * @param name Name given to the visual
   * @return The Entity ID
   */
  EntityID addVisual(const std::string& name);

  /**
   * @brief Given the visual name return the ID
   * @param name Name of the visual
   * @return The ID of the visual (if < 1000 it was not found)
   */
  EntityID getVisual(const std::string& name) const;

  /**
   * @brief Get all visuals being managed
   * @return A map of names to entity id's
   */
  EntityMap getVisuals() const;

  /**
   * @brief Add sensor name to manager and return id for sensor
   * @param name Name given to the sensor
   * @return The Entity ID
   */
  EntityID addSensor(const std::string& name);

  /**
   * @brief Given the sensor name return the ID
   * @param name Name of the sensor
   * @return The ID of the visual (if < 1000 it was not found)
   */
  EntityID getSensor(const std::string& name) const;

  /**
   * @brief Get all sensors being managed
   * @return A map of names to entity id's
   */
  EntityMap getSensors() const;

  /** @brief Check if empty */
  bool empty() const;

  /** @brief Clear internal data */
  void clear();

private:
  std::shared_ptr<EntityManager> manager_;
  mutable std::shared_mutex mutex_;
  LinkEntityContainerMap link_map_;  /**< Stores entity id for each link */
  EntityMap visual_id_map_;          /**< Stores entity id for each visual */
  EntityMap sensor_id_map_;          /**< Stores entity id for each sensor */
};
}
#endif // TESSERACT_GUI_COMMON_ENTITY_CONTAINER_H
