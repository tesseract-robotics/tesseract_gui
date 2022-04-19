#ifndef TESSERACT_GUI_TRANSPORT_CONVERSIONS_H
#define TESSERACT_GUI_TRANSPORT_CONVERSIONS_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#include <ignition/rendering/Visual.hh>
#include <ignition/rendering/Geometry.hh>
#include <ignition/rendering/Material.hh>
#include <ignition/rendering/Light.hh>
#include <ignition/rendering/Scene.hh>

#include <ignition/msgs/scene.pb.h>
#include <ignition/msgs/model.pb.h>
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <tesseract_gui/common/entity_manager.h>
#include <tesseract_scene_graph/graph.h>
#include <tesseract_common/types.h>

namespace tesseract_gui
{
bool isMeshWithColor(const std::string& file_path);

ignition::msgs::Material toMsg(const Eigen::Vector4d& rgba);

ignition::msgs::Scene toMsg(EntityManager& entity_manager,
                            const tesseract_scene_graph::SceneGraph& scene_graph,
                            const tesseract_common::TransformMap& link_transforms);

////////////////////////////////////////////////////////

ignition::rendering::VisualPtr loadModel(ignition::rendering::Scene& scene, const ignition::msgs::Model& msg);
ignition::rendering::VisualPtr loadLink(ignition::rendering::Scene& scene, const ignition::msgs::Link& msg);
ignition::rendering::VisualPtr loadVisual(ignition::rendering::Scene& scene, const ignition::msgs::Visual& msg);
ignition::rendering::GeometryPtr loadGeometry(ignition::rendering::Scene& scene,
                                              const ignition::msgs::Geometry& msg,
                                              ignition::math::Vector3d& _scale,
                                              ignition::math::Pose3d& _localPose);
ignition::rendering::MaterialPtr loadMaterial(ignition::rendering::Scene& scene, const ignition::msgs::Material& msg);
ignition::rendering::LightPtr loadLight(ignition::rendering::Scene& scene, const ignition::msgs::Light& msg);
}  // namespace tesseract_gui
#endif  // TESSERACT_GUI_TRANSPORT_CONVERSIONS_H
