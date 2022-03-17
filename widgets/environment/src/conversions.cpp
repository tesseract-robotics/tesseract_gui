#include <tesseract_gui/environment/conversions.h>
#include <tesseract_geometry/geometries.h>
#include <ignition/math/eigen3/Conversions.hh>
#include <ignition/common/MeshManager.hh>

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

void loadSceneGraph(ignition::rendering::Scene& scene,
                    tesseract_gui::EntityContainer &entity_container,
                    const tesseract_scene_graph::SceneGraph& scene_graph)
{
  ignition::rendering::VisualPtr root = scene.RootVisual();
  for (const auto& link : scene_graph.getLinks())
    root->AddChild(loadLink(scene, entity_container, *link));
}

ignition::rendering::VisualPtr loadLink(ignition::rendering::Scene& scene,
                                        tesseract_gui::EntityContainer &entity_container,
                                        const tesseract_scene_graph::Link& link)
{
  auto lc = entity_container.addLink(link.getName());
  ignition::rendering::VisualPtr ign_link = scene.CreateVisual(lc->getId(), link.getName());

  for (const auto& visual : link.visual)
    ign_link->AddChild(loadLinkGeometry(scene, *lc, *visual->geometry, Eigen::Vector3d::Ones(), visual->origin, visual->material));

  for (const auto& collision : link.collision)
    ign_link->AddChild(loadLinkGeometry(scene, *lc, *collision->geometry, Eigen::Vector3d::Ones(), collision->origin, nullptr));

  return ign_link;
}

ignition::rendering::VisualPtr loadLinkGeometry(ignition::rendering::Scene& scene,
                                            tesseract_gui::LinkEntityContainer &entity_container,
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
      auto gv_id = static_cast<unsigned>(entity_container.addVisual());
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
      auto gv_id = static_cast<unsigned>(entity_container.addVisual());
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
      auto gv_id = static_cast<unsigned>(entity_container.addVisual());
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
      auto gv_id = static_cast<unsigned>(entity_container.addVisual());
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
        auto gv_id = static_cast<unsigned>(entity_container.addVisual());
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
        auto gv_id = static_cast<unsigned>(entity_container.addVisual());
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
