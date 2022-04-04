#ifndef TESSERACT_GUI_ENVIRONMENT_CONVERSIONS_H
#define TESSERACT_GUI_ENVIRONMENT_CONVERSIONS_H

#include <tesseract_gui/common/entity_container.h>
#include <tesseract_scene_graph/graph.h>
#include <tesseract_scene_graph/link.h>
#include <tesseract_geometry/geometry.h>
#include <ignition/rendering/Scene.hh>
#include <ignition/rendering/Visual.hh>
#include <ignition/rendering/Material.hh>

namespace tesseract_gui
{

bool isMeshWithColor(const std::string& file_path);

void loadSceneGraph(ignition::rendering::Scene& scene,
                                              EntityContainer& entity_container,
                                              const tesseract_scene_graph::SceneGraph& scene_graph);

ignition::rendering::VisualPtr loadLink(ignition::rendering::Scene& scene,
                                        EntityContainer& entity_container,
                                        const tesseract_scene_graph::Link& link);

ignition::rendering::VisualPtr loadLinkVisuals(ignition::rendering::Scene& scene,
                                               EntityContainer& entity_container,
                                               const tesseract_scene_graph::Link& link);

ignition::rendering::VisualPtr loadLinkCollisions(ignition::rendering::Scene& scene,
                                                  EntityContainer& entity_container,
                                                  const tesseract_scene_graph::Link& link);

ignition::rendering::VisualPtr loadLinkGeometry(ignition::rendering::Scene& scene,
                                                EntityContainer& entity_container,
                                                const tesseract_geometry::Geometry& geometry,
                                                const Eigen::Vector3d& scale,
                                                const Eigen::Isometry3d& local_pose,
                                                const tesseract_scene_graph::Material::ConstPtr& material);

ignition::rendering::MaterialPtr loadMaterial(ignition::rendering::Scene& scene,
                                              const tesseract_scene_graph::Material::ConstPtr& material);
//  ignition::rendering::LightPtr loadLight(ignition::rendering::Scene& scene, const ignition::msgs::Light &msg);
}

#endif // TESSERACT_GUI_ENVIRONMENT_CONVERSIONS_H
