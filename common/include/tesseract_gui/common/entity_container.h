#ifndef TESSERACT_GUI_COMMON_ENTITY_CONTAINER_H
#define TESSERACT_GUI_COMMON_ENTITY_CONTAINER_H

#include <tesseract_gui/common/entity.h>
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
   * @return The container name
   */
  std::string getName() const;

  /**
   * @brief Create unique entity ID
   * @return New Entity ID
   */
  EntityID createEntityID();

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
  std::string name_;
  EntityMap visual_id_map_;          /**< Stores entity id for each visual */
  EntityMap sensor_id_map_;          /**< Stores entity id for each sensor */
  mutable std::shared_mutex mutex_;

  std::string getKey(const std::string& name) const;
};
}
#endif // TESSERACT_GUI_COMMON_ENTITY_CONTAINER_H
