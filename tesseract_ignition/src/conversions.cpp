/*
 * Copyright (C) 2017 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <tesseract_ignition/conversions.h>
#include <tesseract_geometry/geometries.h>
#include <ignition/math/eigen3/Conversions.hh>
#include <ignition/common/MeshManager.hh>
#include <ignition/rendering/WireBox.hh>
#include <ignition/rendering/AxisVisual.hh>

namespace tesseract_gui
{
//////////////////////////////////////////////////
QColor convert(const ignition::math::Color& _color)
{
  return QColor(_color.R() * 255.0, _color.G() * 255.0, _color.B() * 255.0, _color.A() * 255.0);
}

//////////////////////////////////////////////////
ignition::math::Color convert(const QColor& _color)
{
  return ignition::math::Color(
      _color.red() / 255.0, _color.green() / 255.0, _color.blue() / 255.0, _color.alpha() / 255.0);
}

//////////////////////////////////////////////////
QPointF convert(const ignition::math::Vector2d& _pt) { return QPointF(_pt.X(), _pt.Y()); }

//////////////////////////////////////////////////
ignition::math::Vector2d convert(const QPointF& _pt) { return ignition::math::Vector2d(_pt.x(), _pt.y()); }

//////////////////////////////////////////////////
QVector3D convert(const ignition::math::Vector3d& _vec) { return QVector3D(_vec.X(), _vec.Y(), _vec.Z()); }

//////////////////////////////////////////////////
ignition::math::Vector3d convert(const QVector3D& _vec)
{
  return ignition::math::Vector3d(_vec.x(), _vec.y(), _vec.z());
}

//////////////////////////////////////////////////
ignition::common::MouseEvent convert(const QMouseEvent& _e)
{
  ignition::common::MouseEvent event;
  event.SetPos(_e.pos().x(), _e.pos().y());

  // Button
  if (_e.button() == Qt::LeftButton)
    event.SetButton(ignition::common::MouseEvent::LEFT);
  else if (_e.button() == Qt::RightButton)
    event.SetButton(ignition::common::MouseEvent::RIGHT);
  else if (_e.button() == Qt::MiddleButton)
    event.SetButton(ignition::common::MouseEvent::MIDDLE);

  // Buttons
  if (_e.buttons() & Qt::LeftButton)
    event.SetButtons(event.Buttons() | ignition::common::MouseEvent::LEFT);

  if (_e.buttons() & Qt::RightButton)
    event.SetButtons(event.Buttons() | ignition::common::MouseEvent::RIGHT);

  if (_e.buttons() & Qt::MiddleButton)
    event.SetButtons(event.Buttons() | ignition::common::MouseEvent::MIDDLE);

  // Type
  if (_e.type() == QEvent::MouseButtonPress)
    event.SetType(ignition::common::MouseEvent::PRESS);
  else if (_e.type() == QEvent::MouseButtonRelease)
    event.SetType(ignition::common::MouseEvent::RELEASE);
  else if (_e.type() == QEvent::MouseMove)
  {
    event.SetType(ignition::common::MouseEvent::MOVE);

    // Dragging
    if (_e.buttons() || _e.button())
      event.SetDragging(true);
  }

  // Modifiers
  if (_e.modifiers() & Qt::ShiftModifier)
    event.SetShift(true);

  if (_e.modifiers() & Qt::ControlModifier)
    event.SetControl(true);

  if (_e.modifiers() & Qt::AltModifier)
    event.SetAlt(true);

  return event;
}

//////////////////////////////////////////////////
ignition::common::MouseEvent convert(const QWheelEvent& _e)
{
  ignition::common::MouseEvent event;

  event.SetType(ignition::common::MouseEvent::SCROLL);
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
  event.SetPos(_e.x(), _e.y());
#else
  event.SetPos(_e.position().x(), _e.position().y());
#endif
  double scroll = (_e.angleDelta().y() > 0) ? -1.0 : 1.0;
  event.SetScroll(scroll, scroll);

  // Buttons
  if (_e.buttons() & Qt::LeftButton)
    event.SetButtons(event.Buttons() | ignition::common::MouseEvent::LEFT);

  if (_e.buttons() & Qt::RightButton)
    event.SetButtons(event.Buttons() | ignition::common::MouseEvent::RIGHT);

  if (_e.buttons() & Qt::MiddleButton)
    event.SetButtons(event.Buttons() | ignition::common::MouseEvent::MIDDLE);

  // Modifiers
  if (_e.modifiers() & Qt::ShiftModifier)
    event.SetShift(true);

  if (_e.modifiers() & Qt::ControlModifier)
    event.SetControl(true);

  if (_e.modifiers() & Qt::AltModifier)
    event.SetAlt(true);

  return event;
}

//////////////////////////////////////////////////
ignition::common::KeyEvent convert(const QKeyEvent& _e)
{
  ignition::common::KeyEvent event;
  event.SetKey(_e.key());
  event.SetText(_e.text().toStdString());

  if (_e.type() == QEvent::KeyPress)
  {
    event.SetType(ignition::common::KeyEvent::PRESS);
  }
  else if (_e.type() == QEvent::KeyRelease)
  {
    event.SetType(ignition::common::KeyEvent::RELEASE);
  }
  else
  {
    event.SetType(ignition::common::KeyEvent::NO_EVENT);
  }

  event.SetControl(_e.modifiers() & Qt::ControlModifier);
  event.SetShift(_e.modifiers() & Qt::ShiftModifier);
  event.SetAlt(_e.modifiers() & Qt::AltModifier);

  return event;
}

bool isMeshWithColor(const std::string& file_path)
{
  if (file_path.length() >= 4)
  {
    std::string last_four = file_path.substr(file_path.length() - 4);
    std::string last_four_lower;
    last_four_lower.resize(4);
    std::transform(last_four.begin(), last_four.end(), last_four_lower.begin(), ::tolower);
    return (last_four_lower == ".dae") || (last_four_lower == ".obj");
  }

  return false;
}

std::vector<std::string> loadSceneGraph(ignition::rendering::Scene& scene,
                                        tesseract_gui::EntityContainer& entity_container,
                                        const tesseract_scene_graph::SceneGraph& scene_graph,
                                        const std::string& prefix)
{
  std::vector<std::string> link_names;
  ignition::rendering::VisualPtr root = scene.RootVisual();
  if (prefix.empty())
  {
    for (const auto& link : scene_graph.getLinks())
    {
      root->AddChild(loadLink(scene, entity_container, *link));
      link_names.push_back(link->getName());
    }
  }
  else
  {
    for (const auto& link : scene_graph.getLinks())
    {
      auto clone_link = link->clone(prefix + link->getName());
      root->AddChild(loadLink(scene, entity_container, clone_link));
      link_names.push_back(clone_link.getName());
    }
  }
  return link_names;
}

ignition::rendering::VisualPtr loadLink(ignition::rendering::Scene& scene,
                                        tesseract_gui::EntityContainer& entity_container,
                                        const tesseract_scene_graph::Link& link)
{
  auto entity = entity_container.addTrackedEntity(tesseract_gui::EntityContainer::VISUAL_NS, link.getName());
  ignition::rendering::VisualPtr ign_link = scene.CreateVisual(entity.id, entity.unique_name);
  ign_link->AddChild(loadLinkVisuals(scene, entity_container, link));
  ign_link->AddChild(loadLinkCollisions(scene, entity_container, link));
  ign_link->AddChild(loadLinkAxis(scene, entity_container, link));
  if (!link.visual.empty() || !link.collision.empty())
    ign_link->AddChild(loadLinkWireBox(scene, entity_container, link, ign_link->LocalBoundingBox()));

  return ign_link;
}

ignition::rendering::VisualPtr loadLinkVisuals(ignition::rendering::Scene& scene,
                                               EntityContainer& entity_container,
                                               const tesseract_scene_graph::Link& link)
{
  std::string name = link.getName() + "::Visuals";
  auto entity = entity_container.addTrackedEntity(tesseract_gui::EntityContainer::VISUAL_NS, name);
  ignition::rendering::VisualPtr ign_link_visuals = scene.CreateVisual(entity.id, entity.unique_name);

  for (const auto& visual : link.visual)
    ign_link_visuals->AddChild(loadLinkGeometry(
        scene, entity_container, *visual->geometry, Eigen::Vector3d::Ones(), visual->origin, visual->material));

  return ign_link_visuals;
}

ignition::rendering::VisualPtr loadLinkCollisions(ignition::rendering::Scene& scene,
                                                  EntityContainer& entity_container,
                                                  const tesseract_scene_graph::Link& link)
{
  std::string name = link.getName() + "::Collisions";
  auto entity = entity_container.addTrackedEntity(tesseract_gui::EntityContainer::VISUAL_NS, name);
  ignition::rendering::VisualPtr ign_link_collisions = scene.CreateVisual(entity.id, entity.unique_name);

  for (const auto& visual : link.visual)
    ign_link_collisions->AddChild(loadLinkGeometry(
        scene, entity_container, *visual->geometry, Eigen::Vector3d::Ones(), visual->origin, visual->material));

  ign_link_collisions->SetVisible(false);
  return ign_link_collisions;
}

ignition::rendering::VisualPtr loadLinkWireBox(ignition::rendering::Scene& scene,
                                               EntityContainer& entity_container,
                                               const tesseract_scene_graph::Link& link,
                                               const ignition::math::v6::AxisAlignedBox& aabb)
{
  std::string name = link.getName() + "::WireBox";
  auto entity = entity_container.addTrackedEntity(tesseract_gui::EntityContainer::VISUAL_NS, name);

  auto white = scene.Material("highlight_material");
  if (!white)
  {
    white = scene.CreateMaterial("highlight_material");
    white->SetAmbient(1.0, 1.0, 1.0);
    white->SetDiffuse(1.0, 1.0, 1.0);
    white->SetSpecular(1.0, 1.0, 1.0);
    white->SetEmissive(1.0, 1.0, 1.0);
  }

  ignition::rendering::WireBoxPtr wire_box = scene.CreateWireBox();
  wire_box->SetBox(aabb);

  // Create visual and add wire box
  ignition::rendering::VisualPtr wire_box_vis = scene.CreateVisual(entity.id, entity.unique_name);
  wire_box_vis->SetInheritScale(false);
  wire_box_vis->AddGeometry(wire_box);
  wire_box_vis->SetMaterial(white, false);
  wire_box_vis->SetVisible(false);

  return wire_box_vis;
}

ignition::rendering::VisualPtr loadLinkAxis(ignition::rendering::Scene& scene,
                                            EntityContainer& entity_container,
                                            const tesseract_scene_graph::Link& link)
{
  std::string name = link.getName() + "::Axis";
  auto entity = entity_container.addTrackedEntity(tesseract_gui::EntityContainer::VISUAL_NS, name);

  // Set an alpha not equal to 1 breaks rendering so cannot use CreateAxisVisual
  // ignition::rendering::AxisVisualPtr axis =scene.CreateAxisVisual(lc, name);
  ignition::rendering::VisualPtr axis = scene.CreateVisual(entity.id, entity.unique_name);
  auto axis_red = scene.Material("tesseract_gui_axis_red_material");
  if (!axis_red)
  {
    axis_red = scene.CreateMaterial("tesseract_gui_axis_red_material");
    axis_red->SetAmbient(1.0, 0.0, 0.0);
    axis_red->SetDiffuse(1.0, 0.0, 0.0);
    axis_red->SetSpecular(1.0, 0.0, 0.0);
  }

  auto axis_green = scene.Material("tesseract_gui_axis_green_material");
  if (!axis_green)
  {
    axis_green = scene.CreateMaterial("tesseract_gui_axis_green_material");
    axis_green->SetAmbient(0.0, 1.0, 0.0);
    axis_green->SetDiffuse(0.0, 1.0, 0.0);
    axis_green->SetSpecular(0.0, 1.0, 0.0);
  }

  auto axis_blue = scene.Material("tesseract_gui_axis_blue_material");
  if (!axis_blue)
  {
    axis_blue = scene.CreateMaterial("tesseract_gui_axis_blue_material");
    axis_blue->SetAmbient(0.0, 0.0, 1.0);
    axis_blue->SetDiffuse(0.0, 0.0, 1.0);
    axis_blue->SetSpecular(0.0, 0.0, 1.0);
  }

  {
    auto gv_entity = entity_container.addUntrackedEntity(tesseract_gui::EntityContainer::VISUAL_NS);
    ignition::rendering::VisualPtr cylinder = scene.CreateVisual(gv_entity.id, gv_entity.unique_name);
    Eigen::Isometry3d pose = Eigen::Isometry3d::Identity();
    pose.translation() = Eigen::Vector3d(0, 0, 0.5);
    cylinder->SetLocalPose(ignition::math::eigen3::convert(pose));
    cylinder->AddGeometry(scene.CreateCylinder());
    cylinder->Scale(0.1, 0.1, 1.0);
    cylinder->SetMaterial(axis_blue);
    axis->AddChild(cylinder);
  }

  {
    auto gv_entity = entity_container.addUntrackedEntity(tesseract_gui::EntityContainer::VISUAL_NS);
    ignition::rendering::VisualPtr cylinder = scene.CreateVisual(gv_entity.id, gv_entity.unique_name);
    Eigen::Isometry3d pose = Eigen::Isometry3d::Identity();
    pose.rotate(Eigen::AngleAxisd(M_PI_2, Eigen::Vector3d::UnitY()));
    pose.translation() = Eigen::Vector3d(0.5, 0, 0);
    cylinder->SetLocalPose(ignition::math::eigen3::convert(pose));
    cylinder->AddGeometry(scene.CreateCylinder());
    cylinder->Scale(0.1, 0.1, 1.0);
    cylinder->SetMaterial(axis_red);
    axis->AddChild(cylinder);
  }

  {
    auto gv_entity = entity_container.addUntrackedEntity(tesseract_gui::EntityContainer::VISUAL_NS);
    ignition::rendering::VisualPtr cylinder = scene.CreateVisual(gv_entity.id, gv_entity.unique_name);
    Eigen::Isometry3d pose = Eigen::Isometry3d::Identity();
    pose.rotate(Eigen::AngleAxisd(-M_PI_2, Eigen::Vector3d::UnitX()));
    pose.translation() = Eigen::Vector3d(0, 0.5, 0);
    cylinder->SetLocalPose(ignition::math::eigen3::convert(pose));
    cylinder->AddGeometry(scene.CreateCylinder());
    cylinder->Scale(0.1, 0.1, 1.0);
    cylinder->SetMaterial(axis_green);
    axis->AddChild(cylinder);
  }

  axis->SetInheritScale(false);
  axis->Scale(0.1, 0.1, 0.1);
  axis->SetVisible(true);
  return axis;
}

ignition::rendering::VisualPtr loadLinkGeometry(ignition::rendering::Scene& scene,
                                                tesseract_gui::EntityContainer& entity_container,
                                                const tesseract_geometry::Geometry& geometry,
                                                const Eigen::Vector3d& scale,
                                                const Eigen::Isometry3d& local_pose,
                                                const tesseract_scene_graph::Material::ConstPtr& material)
{
  ignition::rendering::MaterialPtr ign_material = loadMaterial(scene, material);
  switch (geometry.getType())
  {
    case tesseract_geometry::GeometryType::BOX:
    {
      auto gv_entity = entity_container.addUntrackedEntity(tesseract_gui::EntityContainer::VISUAL_NS);
      ignition::rendering::VisualPtr box = scene.CreateVisual(gv_entity.id, gv_entity.unique_name);
      box->SetLocalPose(ignition::math::eigen3::convert(local_pose));
      box->AddGeometry(scene.CreateBox());

      const auto& shape = static_cast<const tesseract_geometry::Box&>(geometry);
      box->Scale(shape.getX() * scale.x(), shape.getY() * scale.y(), shape.getZ() * scale.z());
      box->SetMaterial(ign_material);
      return box;
    }
    case tesseract_geometry::GeometryType::SPHERE:
    {
      auto gv_entity = entity_container.addUntrackedEntity(tesseract_gui::EntityContainer::VISUAL_NS);
      ignition::rendering::VisualPtr sphere = scene.CreateVisual(gv_entity.id, gv_entity.unique_name);
      sphere->SetLocalPose(ignition::math::eigen3::convert(local_pose));
      sphere->AddGeometry(scene.CreateSphere());

      const auto& shape = static_cast<const tesseract_geometry::Sphere&>(geometry);
      sphere->Scale(shape.getRadius() * scale.x(), shape.getRadius() * scale.y(), shape.getRadius() * scale.z());
      sphere->SetMaterial(ign_material);
      return sphere;
    }
    case tesseract_geometry::GeometryType::CYLINDER:
    {
      auto gv_entity = entity_container.addUntrackedEntity(tesseract_gui::EntityContainer::VISUAL_NS);
      ignition::rendering::VisualPtr cylinder = scene.CreateVisual(gv_entity.id, gv_entity.unique_name);
      cylinder->SetLocalPose(ignition::math::eigen3::convert(local_pose));
      cylinder->AddGeometry(scene.CreateCylinder());

      const auto& shape = static_cast<const tesseract_geometry::Cylinder&>(geometry);
      cylinder->Scale(shape.getRadius() * scale.x(), shape.getRadius() * scale.y(), shape.getLength() * scale.z());
      cylinder->SetMaterial(ign_material);
      return cylinder;
    }
    case tesseract_geometry::GeometryType::CONE:
    {
      auto gv_entity = entity_container.addUntrackedEntity(tesseract_gui::EntityContainer::VISUAL_NS);
      ignition::rendering::VisualPtr cone = scene.CreateVisual(gv_entity.id, gv_entity.unique_name);
      cone->SetLocalPose(ignition::math::eigen3::convert(local_pose));
      cone->AddGeometry(scene.CreateCone());

      const auto& shape = static_cast<const tesseract_geometry::Cone&>(geometry);
      cone->Scale(shape.getRadius() * scale.x(), shape.getRadius() * scale.y(), shape.getLength() * scale.z());
      cone->SetMaterial(ign_material);
      return cone;
    }
    case tesseract_geometry::GeometryType::CAPSULE:
    {
      return nullptr;
    }
    case tesseract_geometry::GeometryType::MESH:
    {
      const auto& shape = static_cast<const tesseract_geometry::Mesh&>(geometry);
      auto resource = shape.getResource();
      if (resource)
      {
        auto gv_entity = entity_container.addUntrackedEntity(tesseract_gui::EntityContainer::VISUAL_NS);
        ignition::rendering::VisualPtr mesh = scene.CreateVisual(gv_entity.id, gv_entity.unique_name);
        mesh->SetLocalPose(ignition::math::eigen3::convert(local_pose));

        ignition::rendering::MeshDescriptor descriptor;
        descriptor.meshName = resource->getFilePath();
        ignition::common::MeshManager* mesh_manager = ignition::common::MeshManager::Instance();
        descriptor.mesh = mesh_manager->Load(descriptor.meshName);
        ignition::rendering::MeshPtr mesh_geom = scene.CreateMesh(descriptor);

        if (!isMeshWithColor(resource->getFilePath()))
          mesh_geom->SetMaterial(ign_material);

        mesh->AddGeometry(mesh_geom);
        return mesh;
      }

      assert(false);
      return nullptr;
    }
    case tesseract_geometry::GeometryType::CONVEX_MESH:
    {
      const auto& shape = static_cast<const tesseract_geometry::ConvexMesh&>(geometry);
      auto resource = shape.getResource();
      if (resource)
      {
        auto gv_entity = entity_container.addUntrackedEntity(tesseract_gui::EntityContainer::VISUAL_NS);
        ignition::rendering::VisualPtr mesh = scene.CreateVisual(gv_entity.id, gv_entity.unique_name);
        mesh->SetLocalPose(ignition::math::eigen3::convert(local_pose));

        ignition::rendering::MeshDescriptor descriptor;
        descriptor.meshName = resource->getFilePath();
        ignition::common::MeshManager* mesh_manager = ignition::common::MeshManager::Instance();
        descriptor.mesh = mesh_manager->Load(descriptor.meshName);
        ignition::rendering::MeshPtr mesh_geom = scene.CreateMesh(descriptor);

        if (!isMeshWithColor(resource->getFilePath()))
          mesh_geom->SetMaterial(ign_material);

        mesh->AddGeometry(mesh_geom);
        return mesh;
      }

      assert(false);
      return nullptr;
    }
    case tesseract_geometry::GeometryType::OCTREE:
    {
      const auto& shape = static_cast<const tesseract_geometry::Octree&>(geometry);

      // TODO: Need to implement
      assert(false);
      return nullptr;
    }
    default:
    {
      //      CONSOLE_BRIDGE_logError("This geometric shape type (%d) is not supported",
      //                              static_cast<int>(geometry->getType()));
      return nullptr;
    }
  }
}

ignition::rendering::MaterialPtr loadMaterial(ignition::rendering::Scene& scene,
                                              const tesseract_scene_graph::Material::ConstPtr& material)
{
  if (material == nullptr)
  {
    const Eigen::Vector4d& rgba = tesseract_scene_graph::DEFAULT_TESSERACT_MATERIAL->color;
    auto ign_material = scene.Material(tesseract_scene_graph::DEFAULT_TESSERACT_MATERIAL->getName());
    if (ign_material == nullptr)
    {
      ign_material = scene.CreateMaterial(tesseract_scene_graph::DEFAULT_TESSERACT_MATERIAL->getName());
      ign_material->SetAmbient(rgba(0), rgba(1), rgba(2), rgba(3));
      ign_material->SetDiffuse(rgba(0), rgba(1), rgba(2), rgba(3));
      ign_material->SetSpecular(rgba(0), rgba(1), rgba(2), rgba(3));
    }
    return ign_material;
  }

  if (material->texture_filename.empty())
  {
    const Eigen::Vector4d& rgba = material->color;
    auto ign_material = scene.Material(material->getName());
    if (ign_material == nullptr)
    {
      ign_material = scene.CreateMaterial(material->getName());
      ign_material->SetAmbient(rgba(0), rgba(1), rgba(2), rgba(3));
      ign_material->SetDiffuse(rgba(0), rgba(1), rgba(2), rgba(3));
      ign_material->SetSpecular(rgba(0), rgba(1), rgba(2), rgba(3));
    }
    return ign_material;
  }

  return nullptr;
}
}  // namespace tesseract_gui
