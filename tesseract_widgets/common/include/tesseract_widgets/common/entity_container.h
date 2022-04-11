#ifndef TESSERACT_GUI_COMMON_ENTITY_CONTAINER_H
#define TESSERACT_GUI_COMMON_ENTITY_CONTAINER_H

#include <tesseract_widgets/common/entity.h>
#include <memory>
#include <vector>
#include <shared_mutex>

namespace tesseract_gui
{
class EntityManager;

class EntityContainer
{
public:
  using Ptr = std::shared_ptr<EntityContainer>;
  using ConstPtr = std::shared_ptr<const EntityContainer>;

  EntityContainer(std::shared_ptr<EntityManager> manager, std::string name);

  /**
   * @brief Get the name of the container
   * @brief This should be unique
   * @return The container name
   */
  std::string getName() const;

  /**
   * @brief Add untracked entity
   * @return The Entity
   */
  Entity addUntracked();

  /**
   * @brief Get a vector of untracked entities
   * @return A vector of entities.
   */
  EntityVector getUntracked() const;

  /**
   * @brief Add visual name to manager and return id for visual
   * @param name Name given to the visual
   * @return The Entity
   */
  Entity addVisual(const std::string& name);

  /**
   * @brief Given the visual name return the Entity
   * @param name Name of the visual
   * @throws If entity does not exits for the provided name
   * @return The Entity of the visual
   */
  Entity getVisual(const std::string& name) const;

  /**
   * @brief Given the visual name return if an entity exist for the provided name
   * @param name Name of the visual
   * @return True if an entity exits
   */
  bool hasVisual(const std::string& name) const;

  /**
   * @brief Get all visuals being managed
   * @return A map of names to entity's
   */
  EntityMap getVisuals() const;

  /**
   * @brief Add sensor name to manager and return id for sensor
   * @param name Name given to the sensor
   * @return The Entity
   */
  Entity addSensor(const std::string& name);

  /**
   * @brief Given the sensor name return if an entity exist for the provided name
   * @param name Name of the sensor
   * @return True if an entity exits
   */
  bool hasSensor(const std::string& name) const;

  /**
   * @brief Given the sensor name return the entity
   * @param name Name of the sensor
   * @return The entity of the visual
   */
  Entity getSensor(const std::string& name) const;

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
  std::string name_;
  EntityMap visual_entity_map_;          /**< Stores a map of entity id to name for each visual */
  EntityMap sensor_entity_map_;          /**< Stores a map of entity id to name for each sensor */
  EntityVector untracked_entities_;      /**< Stored a vector of untracked entities */


  mutable std::shared_mutex mutex_;
};
}
#endif // TESSERACT_GUI_COMMON_ENTITY_CONTAINER_H
