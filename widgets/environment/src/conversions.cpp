#include <tesseract_gui/widgets/environment/conversions.h>
#include <tesseract_geometry/geometries.h>
#include <ignition/math/eigen3/Conversions.hh>
#include <ignition/common/MeshManager.hh>
#include <ignition/rendering/WireBox.hh>
#include <ignition/rendering/AxisVisual.hh>

namespace tesseract_gui
{

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
                                        tesseract_gui::EntityContainer &entity_container,
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
                                        tesseract_gui::EntityContainer &entity_container,
                                        const tesseract_scene_graph::Link& link)
{
  auto lc = entity_container.addVisual(link.getName());
  ignition::rendering::VisualPtr ign_link = scene.CreateVisual(lc, link.getName());
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
  auto lc = entity_container.addVisual(name);
  ignition::rendering::VisualPtr ign_link_visuals = scene.CreateVisual(lc, name);

  for (const auto& visual : link.visual)
    ign_link_visuals->AddChild(loadLinkGeometry(scene, entity_container, *visual->geometry, Eigen::Vector3d::Ones(), visual->origin, visual->material));

  return ign_link_visuals;
}

ignition::rendering::VisualPtr loadLinkCollisions(ignition::rendering::Scene& scene,
                                                  EntityContainer& entity_container,
                                                  const tesseract_scene_graph::Link& link)
{
  std::string name = link.getName() + "::Collisions";
  auto lc = entity_container.addVisual(name);
  ignition::rendering::VisualPtr ign_link_collisions = scene.CreateVisual(lc, name);

  for (const auto& visual : link.visual)
    ign_link_collisions->AddChild(loadLinkGeometry(scene, entity_container, *visual->geometry, Eigen::Vector3d::Ones(), visual->origin, visual->material));

  ign_link_collisions->SetVisible(false);
  return ign_link_collisions;
}

ignition::rendering::VisualPtr loadLinkWireBox(ignition::rendering::Scene& scene,
                                               EntityContainer& entity_container,
                                               const tesseract_scene_graph::Link &link,
                                               const ignition::math::v6::AxisAlignedBox &aabb)
{
  std::string name = link.getName() + "::WireBox";
  auto lc = entity_container.addVisual(name);

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
  ignition::rendering::VisualPtr wire_box_vis = scene.CreateVisual(lc, name);
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
  auto lc = entity_container.addVisual(name);

  // Set an alpha not equal to 1 breaks rendering so cannot use CreateAxisVisual
  // ignition::rendering::AxisVisualPtr axis =scene.CreateAxisVisual(lc, name);
  ignition::rendering::VisualPtr axis = scene.CreateVisual(lc, name);
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
    auto gv_id = static_cast<unsigned>(entity_container.createEntityID());
    ignition::rendering::VisualPtr cylinder = scene.CreateVisual(gv_id);
    Eigen::Isometry3d pose = Eigen::Isometry3d::Identity();
    pose.translation() = Eigen::Vector3d(0,0,0.5);
    cylinder->SetLocalPose(ignition::math::eigen3::convert(pose));
    cylinder->AddGeometry(scene.CreateCylinder());
    cylinder->Scale(0.1, 0.1, 1.0);
    cylinder->SetMaterial(axis_blue);
    axis->AddChild(cylinder);
  }

  {
    auto gv_id = static_cast<unsigned>(entity_container.createEntityID());
    ignition::rendering::VisualPtr cylinder = scene.CreateVisual(gv_id);
    Eigen::Isometry3d pose = Eigen::Isometry3d::Identity();
    pose.rotate(Eigen::AngleAxisd(M_PI_2, Eigen::Vector3d::UnitY()));
    pose.translation() = Eigen::Vector3d(0.5,0,0);
    cylinder->SetLocalPose(ignition::math::eigen3::convert(pose));
    cylinder->AddGeometry(scene.CreateCylinder());
    cylinder->Scale(0.1, 0.1, 1.0);
    cylinder->SetMaterial(axis_red);
    axis->AddChild(cylinder);
  }

  {
    auto gv_id = static_cast<unsigned>(entity_container.createEntityID());
    ignition::rendering::VisualPtr cylinder = scene.CreateVisual(gv_id);
    Eigen::Isometry3d pose = Eigen::Isometry3d::Identity();
    pose.rotate(Eigen::AngleAxisd(-M_PI_2, Eigen::Vector3d::UnitX()));
    pose.translation() = Eigen::Vector3d(0,0.5,0);
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
                                            tesseract_gui::EntityContainer &entity_container,
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
      auto gv_id = static_cast<unsigned>(entity_container.createEntityID());
      ignition::rendering::VisualPtr box = scene.CreateVisual(gv_id);
      box->SetLocalPose(ignition::math::eigen3::convert(local_pose));
      box->AddGeometry(scene.CreateBox());

      const auto& shape = static_cast<const tesseract_geometry::Box&>(geometry);
      box->Scale(shape.getX() * scale.x(), shape.getY() * scale.y(), shape.getZ() * scale.z());
      box->SetMaterial(ign_material);
      return box;
    }
    case tesseract_geometry::GeometryType::SPHERE:
    {
      auto gv_id = static_cast<unsigned>(entity_container.createEntityID());
      ignition::rendering::VisualPtr sphere = scene.CreateVisual(gv_id);
      sphere->SetLocalPose(ignition::math::eigen3::convert(local_pose));
      sphere->AddGeometry(scene.CreateSphere());

      const auto& shape = static_cast<const tesseract_geometry::Sphere&>(geometry);
      sphere->Scale(shape.getRadius() * scale.x(), shape.getRadius() * scale.y(), shape.getRadius() * scale.z());
      sphere->SetMaterial(ign_material);
      return sphere;
    }
    case tesseract_geometry::GeometryType::CYLINDER:
    {
      auto gv_id = static_cast<unsigned>(entity_container.createEntityID());
      ignition::rendering::VisualPtr cylinder = scene.CreateVisual(gv_id);
      cylinder->SetLocalPose(ignition::math::eigen3::convert(local_pose));
      cylinder->AddGeometry(scene.CreateCylinder());

      const auto& shape = static_cast<const tesseract_geometry::Cylinder&>(geometry);
      cylinder->Scale(shape.getRadius() * scale.x(), shape.getRadius() * scale.y(), shape.getLength() * scale.z());
      cylinder->SetMaterial(ign_material);
      return cylinder;
    }
    case tesseract_geometry::GeometryType::CONE:
    {
      auto gv_id = static_cast<unsigned>(entity_container.createEntityID());
      ignition::rendering::VisualPtr cone = scene.CreateVisual(gv_id);
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
        auto gv_id = static_cast<unsigned>(entity_container.createEntityID());
        ignition::rendering::VisualPtr mesh = scene.CreateVisual(gv_id);
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
        auto gv_id = static_cast<unsigned>(entity_container.createEntityID());
        ignition::rendering::VisualPtr mesh = scene.CreateVisual(gv_id);
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

}
