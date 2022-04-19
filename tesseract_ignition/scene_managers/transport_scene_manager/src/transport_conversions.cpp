#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#include <ignition/msgs/geometry.pb.h>
#include <ignition/msgs/material.pb.h>
#include <ignition/msgs/inertial.pb.h>
#include <ignition/msgs/collision.pb.h>
#include <ignition/msgs/visual.pb.h>
#include <ignition/msgs/link.pb.h>
#include <ignition/msgs/Utility.hh>
#include <ignition/rendering/Capsule.hh>
#include <ignition/math/eigen3/Conversions.hh>
#include <ignition/common/Console.hh>
#include <ignition/common/MeshManager.hh>
#include <console_bridge/console.h>
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <tesseract_gui/transport_scene_manager/transport_conversions.h>
#include <tesseract_geometry/geometries.h>

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

ignition::msgs::Material toMsg(const Eigen::Vector4d& rgba)
{
  ignition::msgs::Material shape_material_msg;
  shape_material_msg.mutable_ambient()->set_r(static_cast<float>(rgba(0)));
  shape_material_msg.mutable_ambient()->set_g(static_cast<float>(rgba(1)));
  shape_material_msg.mutable_ambient()->set_b(static_cast<float>(rgba(2)));
  shape_material_msg.mutable_ambient()->set_a(static_cast<float>(rgba(3)));

  shape_material_msg.mutable_diffuse()->set_r(static_cast<float>(rgba(0)));
  shape_material_msg.mutable_diffuse()->set_g(static_cast<float>(rgba(1)));
  shape_material_msg.mutable_diffuse()->set_b(static_cast<float>(rgba(2)));
  shape_material_msg.mutable_diffuse()->set_a(static_cast<float>(rgba(3)));

  shape_material_msg.mutable_specular()->set_r(static_cast<float>(rgba(0)));
  shape_material_msg.mutable_specular()->set_g(static_cast<float>(rgba(1)));
  shape_material_msg.mutable_specular()->set_b(static_cast<float>(rgba(2)));
  shape_material_msg.mutable_specular()->set_a(static_cast<float>(rgba(3)));

  return shape_material_msg;
}

ignition::msgs::Scene toMsg(EntityManager& entity_manager,
                            const tesseract_scene_graph::SceneGraph& scene_graph,
                            const tesseract_common::TransformMap& link_transforms)
{
  ignition::msgs::Scene scene_msg;
  scene_msg.set_name("scene");
  ignition::msgs::Model* model = scene_msg.add_model();
  model->set_name(scene_graph.getName());
  model->set_id(static_cast<unsigned>(entity_manager.addModel(scene_graph.getName())));
  for (const auto& link : scene_graph.getLinks())
  {
    ignition::msgs::Link* link_msg = model->add_link();
    link_msg->set_name(link->getName());
    link_msg->set_id(static_cast<unsigned>(entity_manager.addLink(link->getName())));
    link_msg->mutable_pose()->CopyFrom(
        ignition::msgs::Convert(ignition::math::eigen3::convert(link_transforms.at(link->getName()))));

    int cnt = 0;
    for (const auto& vs : link->visual)
    {
      std::string gv_name = link->getName() + std::to_string(++cnt);
      switch (vs->geometry->getType())
      {
        case tesseract_geometry::GeometryType::BOX:
        {
          ignition::msgs::Visual* gv_msg = link_msg->add_visual();
          gv_msg->set_id(static_cast<unsigned>(entity_manager.addVisual(gv_name)));
          gv_msg->set_name(gv_name);
          gv_msg->mutable_pose()->CopyFrom(ignition::msgs::Convert(ignition::math::eigen3::convert(vs->origin)));

          ignition::msgs::Geometry geometry_msg;
          geometry_msg.set_type(ignition::msgs::Geometry::Type::Geometry_Type_BOX);

          auto shape = std::static_pointer_cast<const tesseract_geometry::Box>(vs->geometry);
          ignition::msgs::BoxGeom shape_geometry_msg;
          shape_geometry_msg.mutable_size()->CopyFrom(
              ignition::msgs::Convert(ignition::math::Vector3d(shape->getX(), shape->getY(), shape->getZ())));
          geometry_msg.mutable_box()->CopyFrom(shape_geometry_msg);
          gv_msg->mutable_geometry()->CopyFrom(geometry_msg);

          if (vs->material != nullptr && vs->material->getName() != "default_tesseract_material" &&
              vs->material->texture_filename.empty())
          {
            gv_msg->mutable_material()->CopyFrom(toMsg(vs->material->color));
          }

          gv_msg->set_parent_name(link->getName());
          break;
        }
        case tesseract_geometry::GeometryType::SPHERE:
        {
          ignition::msgs::Visual* gv_msg = link_msg->add_visual();
          gv_msg->set_id(static_cast<unsigned>(entity_manager.addVisual(gv_name)));
          gv_msg->set_name(gv_name);
          gv_msg->mutable_pose()->CopyFrom(ignition::msgs::Convert(ignition::math::eigen3::convert(vs->origin)));

          ignition::msgs::Geometry geometry_msg;
          geometry_msg.set_type(ignition::msgs::Geometry::Type::Geometry_Type_SPHERE);

          auto shape = std::static_pointer_cast<const tesseract_geometry::Sphere>(vs->geometry);
          ignition::msgs::SphereGeom shape_geometry_msg;
          shape_geometry_msg.set_radius(shape->getRadius());
          geometry_msg.mutable_sphere()->CopyFrom(shape_geometry_msg);
          gv_msg->mutable_geometry()->CopyFrom(geometry_msg);

          if (vs->material != nullptr && vs->material->getName() != "default_tesseract_material" &&
              vs->material->texture_filename.empty())
          {
            gv_msg->mutable_material()->CopyFrom(toMsg(vs->material->color));
          }

          gv_msg->set_parent_name(link->getName());
          break;
        }
        case tesseract_geometry::GeometryType::CYLINDER:
        {
          ignition::msgs::Visual* gv_msg = link_msg->add_visual();
          gv_msg->set_id(static_cast<unsigned>(entity_manager.addVisual(gv_name)));
          gv_msg->set_name(gv_name);
          gv_msg->mutable_pose()->CopyFrom(ignition::msgs::Convert(ignition::math::eigen3::convert(vs->origin)));

          ignition::msgs::Geometry geometry_msg;
          geometry_msg.set_type(ignition::msgs::Geometry::Type::Geometry_Type_CYLINDER);

          auto shape = std::static_pointer_cast<const tesseract_geometry::Cylinder>(vs->geometry);
          ignition::msgs::CylinderGeom shape_geometry_msg;
          shape_geometry_msg.set_radius(shape->getRadius());
          shape_geometry_msg.set_length(shape->getLength());
          geometry_msg.mutable_cylinder()->CopyFrom(shape_geometry_msg);
          gv_msg->mutable_geometry()->CopyFrom(geometry_msg);

          if (vs->material != nullptr && vs->material->getName() != "default_tesseract_material" &&
              vs->material->texture_filename.empty())
          {
            gv_msg->mutable_material()->CopyFrom(toMsg(vs->material->color));
          }

          gv_msg->set_parent_name(link->getName());
          break;
        }
          //      case tesseract_geometry::GeometryType::CONE:
          //      {
          //          ignition::msgs::Visual* gv_msg = link_msg->add_visual();
          //          gv_msg->set_id(static_cast<unsigned>(entity_manager.addVisual(gv_name)));
          //          gv_msg->set_name(gv_name);
          //          gv_msg->mutable_pose()->CopyFrom(ignition::msgs::Convert(ignition::math::eigen3::convert(vs->origin)));

          //          ignition::msgs::Geometry geometry_msg;
          //          geometry_msg.set_type(ignition::msgs::Geometry::Type::Geometry_Type_CONE);

          //          auto shape = std::static_pointer_cast<const tesseract_geometry::Cone>(vs->geometry);
          //          ignition::msgs::ConeGeom shape_geometry_msg;
          //          shape_geometry_msg.set_radius(shape->getRadius());
          //          shape_geometry_msg.set_length(shape->getLength());
          //          geometry_msg.mutable_sphere()->CopyFrom(shape_geometry_msg);
          //          gv_msg->mutable_geometry()->CopyFrom(geometry_msg);
          //
          //          if (vs->material != nullptr && vs->material->getName() != "default_tesseract_material" &&
          //          vs->material->texture_filename.empty())
          //          {
          //            gv_msg->mutable_material()->CopyFrom(convert(vs->material->color));
          //          }
          //
          //          gv_msg->set_parent_name(link->getName());
          //          break;
          //      }

          //        case tesseract_geometry::GeometryType::CAPSULE:
          //        {
          //          ignition::msgs::Visual* gv_msg = link_msg->add_visual();
          //          gv_msg->set_id(static_cast<unsigned>(entity_manager.addVisual(gv_name)));
          //          gv_msg->set_name(gv_name);
          //          gv_msg->mutable_pose()->CopyFrom(ignition::msgs::Convert(ignition::math::eigen3::convert(vs->origin)));

          //          ignition::msgs::Geometry geometry_msg;
          //          geometry_msg.set_type(ignition::msgs::Geometry::Type::Geometry_Type_CAPSULE);

          //          auto shape = std::static_pointer_cast<const tesseract_geometry::Capsule>(vs->geometry);
          //          ignition::msgs::CapsuleGeom shape_geometry_msg;
          //          shape_geometry_msg.set_radius(shape->getRadius());
          //          shape_geometry_msg.set_length(shape->getLength());
          //          geometry_msg.mutable_sphere()->CopyFrom(shape_geometry_msg);
          //          gv_msg->mutable_geometry()->CopyFrom(geometry_msg);
          //
          //          if (vs->material != nullptr && vs->material->getName() != "default_tesseract_material" &&
          //          vs->material->texture_filename.empty())
          //          {
          //            gv_msg->mutable_material()->CopyFrom(convert(vs->material->color));
          //          }
          //
          //          gv_msg->set_parent_name(link->getName());
          //          break;
          //        }
        case tesseract_geometry::GeometryType::MESH:
        {
          ignition::msgs::Visual* gv_msg = link_msg->add_visual();
          gv_msg->set_id(static_cast<unsigned>(entity_manager.addVisual(gv_name)));
          gv_msg->set_name(gv_name);
          gv_msg->mutable_pose()->CopyFrom(ignition::msgs::Convert(ignition::math::eigen3::convert(vs->origin)));

          ignition::msgs::Geometry geometry_msg;
          geometry_msg.set_type(ignition::msgs::Geometry::Type::Geometry_Type_MESH);

          auto shape = std::static_pointer_cast<const tesseract_geometry::Mesh>(vs->geometry);
          auto resource = shape->getResource();
          if (resource)
          {
            ignition::msgs::MeshGeom shape_geometry_msg;
            shape_geometry_msg.set_filename(resource->getFilePath());
            shape_geometry_msg.mutable_scale()->CopyFrom(
                ignition::msgs::Convert(ignition::math::eigen3::convert(shape->getScale())));
            geometry_msg.mutable_mesh()->CopyFrom(shape_geometry_msg);
            gv_msg->mutable_geometry()->CopyFrom(geometry_msg);

            if (!isMeshWithColor(resource->getFilePath()) && vs->material != nullptr &&
                vs->material->getName() != "default_tesseract_material" && vs->material->texture_filename.empty())
            {
              gv_msg->mutable_material()->CopyFrom(toMsg(vs->material->color));
            }

            gv_msg->set_parent_name(link->getName());
          }
          else
          {
            assert(false);
          }

          break;
        }
        case tesseract_geometry::GeometryType::CONVEX_MESH:
        {
          ignition::msgs::Visual* gv_msg = link_msg->add_visual();
          gv_msg->set_id(static_cast<unsigned>(entity_manager.addVisual(gv_name)));
          gv_msg->set_name(gv_name);
          gv_msg->mutable_pose()->CopyFrom(ignition::msgs::Convert(ignition::math::eigen3::convert(vs->origin)));

          ignition::msgs::Geometry geometry_msg;
          geometry_msg.set_type(ignition::msgs::Geometry::Type::Geometry_Type_MESH);

          auto shape = std::static_pointer_cast<const tesseract_geometry::ConvexMesh>(vs->geometry);
          auto resource = shape->getResource();
          if (resource)
          {
            ignition::msgs::MeshGeom shape_geometry_msg;
            shape_geometry_msg.set_filename(resource->getFilePath());
            shape_geometry_msg.mutable_scale()->CopyFrom(
                ignition::msgs::Convert(ignition::math::eigen3::convert(shape->getScale())));
            geometry_msg.mutable_mesh()->CopyFrom(shape_geometry_msg);
            gv_msg->mutable_geometry()->CopyFrom(geometry_msg);

            if (!isMeshWithColor(resource->getFilePath()) && vs->material != nullptr &&
                vs->material->getName() != "default_tesseract_material" && vs->material->texture_filename.empty())
            {
              gv_msg->mutable_material()->CopyFrom(toMsg(vs->material->color));
            }

            gv_msg->set_parent_name(link->getName());
          }
          else
          {
            assert(false);
          }

          break;
        }
          //        case tesseract_geometry::GeometryType::OCTREE:
          //        {
          //          auto shape = std::static_pointer_cast<const tesseract_geometry::Octree>(vs->geometry);

          //          // TODO: Need to implement
          //          assert(false);
          //          break;
          //        }
        default:
        {
          throw std::runtime_error("This geometric shape type " +
                                   std::to_string(static_cast<int>(vs->geometry->getType())) + " is not supported");
          break;
        }
      }
    }
  }
  return scene_msg;
}

ignition::rendering::VisualPtr loadModel(ignition::rendering::Scene& scene, const ignition::msgs::Model& msg)
{
  ignition::rendering::VisualPtr modelVis;
  if (!msg.name().empty() && !scene.HasVisualName(msg.name()))
  {
    modelVis = scene.CreateVisual(msg.name());
  }
  else
  {
    modelVis = scene.CreateVisual();
  }

  if (msg.has_pose())
    modelVis->SetLocalPose(ignition::msgs::Convert(msg.pose()));

  // load links
  for (int i = 0; i < msg.link_size(); ++i)
  {
    ignition::rendering::VisualPtr linkVis = loadLink(scene, msg.link(i));
    if (linkVis)
      modelVis->AddChild(linkVis);
    else
      ignerr << "Failed to load link: " << msg.link(i).name() << std::endl;
  }

  // load nested models
  for (int i = 0; i < msg.model_size(); ++i)
  {
    ignition::rendering::VisualPtr nestedModelVis = loadModel(scene, msg.model(i));
    if (nestedModelVis)
      modelVis->AddChild(nestedModelVis);
    else
      ignerr << "Failed to load nested model: " << msg.model(i).name() << std::endl;
  }

  return modelVis;
}
ignition::rendering::VisualPtr loadLink(ignition::rendering::Scene& scene, const ignition::msgs::Link& msg)
{
  ignition::rendering::VisualPtr linkVis;
  if (!msg.name().empty() && !scene.HasVisualName(msg.name()))
  {
    linkVis = scene.CreateVisual(msg.name());
  }
  else
  {
    linkVis = scene.CreateVisual();
  }

  if (msg.has_pose())
    linkVis->SetLocalPose(ignition::msgs::Convert(msg.pose()));

  // load visuals
  for (int i = 0; i < msg.visual_size(); ++i)
  {
    ignition::rendering::VisualPtr visualVis = loadVisual(scene, msg.visual(i));
    if (visualVis)
      linkVis->AddChild(visualVis);
    else
      ignerr << "Failed to load visual: " << msg.visual(i).name() << std::endl;
  }

  // load lights
  for (int i = 0; i < msg.light_size(); ++i)
  {
    ignition::rendering::LightPtr light = loadLight(scene, msg.light(i));
    if (light)
      linkVis->AddChild(light);
    else
      ignerr << "Failed to load light: " << msg.light(i).name() << std::endl;
  }

  return linkVis;
}

ignition::rendering::VisualPtr loadVisual(ignition::rendering::Scene& scene, const ignition::msgs::Visual& msg)
{
  if (!msg.has_geometry())
    return ignition::rendering::VisualPtr();

  ignition::rendering::VisualPtr visualVis;
  if (!msg.name().empty() && !scene.HasVisualName(msg.name()))
  {
    visualVis = scene.CreateVisual(msg.name());
  }
  else
  {
    visualVis = scene.CreateVisual();
  }

  ignition::math::Vector3d scale = ignition::math::Vector3d::One;
  ignition::math::Pose3d localPose;
  ignition::rendering::GeometryPtr geom = loadGeometry(scene, msg.geometry(), scale, localPose);

  if (msg.has_pose())
    visualVis->SetLocalPose(ignition::msgs::Convert(msg.pose()) * localPose);
  else
    visualVis->SetLocalPose(localPose);

  if (geom)
  {
    visualVis->AddGeometry(geom);
    visualVis->SetLocalScale(scale);

    // set material
    ignition::rendering::MaterialPtr material{ nullptr };
    if (msg.has_material())
    {
      material = loadMaterial(scene, msg.material());
    }
    // Don't set a default material for meshes because they
    // may have their own
    // TODO(anyone) support overriding mesh material
    else if (!msg.geometry().has_mesh())
    {
      // create default material
      material = scene.Material("ign-grey");
      if (!material)
      {
        material = scene.CreateMaterial("ign-grey");
        material->SetAmbient(0.3, 0.3, 0.3);
        material->SetDiffuse(0.7, 0.7, 0.7);
        material->SetSpecular(1.0, 1.0, 1.0);
        material->SetRoughness(0.2f);
        material->SetMetalness(1.0f);
      }
    }
    else
    {
      // meshes created by mesh loader may have their own materials
      // update/override their properties based on input sdf element values
      auto mesh = std::dynamic_pointer_cast<ignition::rendering::Mesh>(geom);
      for (unsigned int i = 0; i < mesh->SubMeshCount(); ++i)
      {
        auto submesh = mesh->SubMeshByIndex(i);
        auto submeshMat = submesh->Material();
        if (submeshMat)
        {
          double productAlpha = (1.0 - msg.transparency()) * (1.0 - submeshMat->Transparency());
          submeshMat->SetTransparency(1 - productAlpha);
          submeshMat->SetCastShadows(msg.cast_shadows());
        }
      }
    }

    if (material)
    {
      // set transparency
      material->SetTransparency(msg.transparency());

      // cast shadows
      material->SetCastShadows(msg.cast_shadows());

      geom->SetMaterial(material);
      // todo(anyone) SetMaterial function clones the input material.
      // but does not take ownership of it so we need to destroy it here.
      // This is not ideal. We should let ign-rendering handle the lifetime
      // of this material
      scene.DestroyMaterial(material);
    }
  }
  else
  {
    ignerr << "Failed to load geometry for visual: " << msg.name() << std::endl;
  }

  return visualVis;
}

ignition::rendering::GeometryPtr loadGeometry(ignition::rendering::Scene& scene,
                                              const ignition::msgs::Geometry& msg,
                                              ignition::math::Vector3d& _scale,
                                              ignition::math::Pose3d& _localPose)
{
  ignition::math::Vector3d scale = ignition::math::Vector3d::One;
  ignition::math::Pose3d localPose = ignition::math::Pose3d::Zero;
  ignition::rendering::GeometryPtr geom{ nullptr };
  if (msg.has_box())
  {
    geom = scene.CreateBox();
    if (msg.box().has_size())
      scale = ignition::msgs::Convert(msg.box().size());
  }
  else if (msg.has_cylinder())
  {
    geom = scene.CreateCylinder();
    scale.X() = msg.cylinder().radius() * 2;
    scale.Y() = scale.X();
    scale.Z() = msg.cylinder().length();
  }
  else if (msg.has_capsule())
  {
    auto capsule = scene.CreateCapsule();
    capsule->SetRadius(msg.capsule().radius());
    capsule->SetLength(msg.capsule().length());
    geom = capsule;

    scale.X() = msg.capsule().radius() * 2;
    scale.Y() = scale.X();
    scale.Z() = msg.capsule().length() + scale.X();
  }
  else if (msg.has_ellipsoid())
  {
    geom = scene.CreateSphere();
    scale.X() = msg.ellipsoid().radii().x() * 2;
    scale.Y() = msg.ellipsoid().radii().y() * 2;
    scale.Z() = msg.ellipsoid().radii().z() * 2;
  }
  else if (msg.has_plane())
  {
    geom = scene.CreatePlane();

    if (msg.plane().has_size())
    {
      scale.X() = msg.plane().size().x();
      scale.Y() = msg.plane().size().y();
    }

    if (msg.plane().has_normal())
    {
      // Create a rotation for the plane mesh to account for the normal vector.
      // The rotation is the angle between the +z(0,0,1) vector and the
      // normal, which are both expressed in the local (Visual) frame.
      ignition::math::Vector3d normal = ignition::msgs::Convert(msg.plane().normal());
      localPose.Rot().From2Axes(ignition::math::Vector3d::UnitZ, normal.Normalized());
    }
  }
  else if (msg.has_sphere())
  {
    geom = scene.CreateSphere();
    scale.X() = msg.sphere().radius() * 2;
    scale.Y() = scale.X();
    scale.Z() = scale.X();
  }
  else if (msg.has_mesh())
  {
    if (msg.mesh().filename().empty())
    {
      ignerr << "Mesh geometry missing filename" << std::endl;
      return geom;
    }
    ignition::rendering::MeshDescriptor descriptor;

    // Assume absolute path to mesh file
    descriptor.meshName = msg.mesh().filename();

    ignition::common::MeshManager* meshManager = ignition::common::MeshManager::Instance();
    descriptor.mesh = meshManager->Load(descriptor.meshName);
    geom = scene.CreateMesh(descriptor);

    scale = ignition::msgs::Convert(msg.mesh().scale());
  }
  else
  {
    ignerr << "Unsupported geometry type" << std::endl;
  }
  _scale = scale;
  _localPose = localPose;
  return geom;
}
ignition::rendering::MaterialPtr loadMaterial(ignition::rendering::Scene& scene,
                                              tesseract_gui::EntityContainer& entity_container,
                                              const ignition::msgs::Material& msg)
{
  ignition::rendering::MaterialPtr material = scene.CreateMaterial();
  if (msg.has_ambient())
  {
    material->SetAmbient(ignition::msgs::Convert(msg.ambient()));
  }
  if (msg.has_diffuse())
  {
    material->SetDiffuse(ignition::msgs::Convert(msg.diffuse()));
  }
  if (msg.has_specular())
  {
    material->SetDiffuse(ignition::msgs::Convert(msg.specular()));
  }
  if (msg.has_emissive())
  {
    material->SetEmissive(ignition::msgs::Convert(msg.emissive()));
  }

  return material;
}
ignition::rendering::LightPtr loadLight(ignition::rendering::Scene& scene, const ignition::msgs::Light& msg)
{
  ignition::rendering::LightPtr light;

  switch (msg.type())
  {
    case ignition::msgs::Light_LightType_POINT:
      light = scene.CreatePointLight();
      break;
    case ignition::msgs::Light_LightType_SPOT:
    {
      light = scene.CreateSpotLight();
      ignition::rendering::SpotLightPtr spotLight = std::dynamic_pointer_cast<ignition::rendering::SpotLight>(light);
      spotLight->SetInnerAngle(msg.spot_inner_angle());
      spotLight->SetOuterAngle(msg.spot_outer_angle());
      spotLight->SetFalloff(msg.spot_falloff());
      break;
    }
    case ignition::msgs::Light_LightType_DIRECTIONAL:
    {
      light = scene.CreateDirectionalLight();
      ignition::rendering::DirectionalLightPtr dirLight =
          std::dynamic_pointer_cast<ignition::rendering::DirectionalLight>(light);

      if (msg.has_direction())
        dirLight->SetDirection(ignition::msgs::Convert(msg.direction()));
      break;
    }
    default:
      ignerr << "Light type not supported" << std::endl;
      return light;
  }

  if (msg.has_pose())
    light->SetLocalPose(ignition::msgs::Convert(msg.pose()));

  if (msg.has_diffuse())
    light->SetDiffuseColor(ignition::msgs::Convert(msg.diffuse()));

  if (msg.has_specular())
    light->SetSpecularColor(ignition::msgs::Convert(msg.specular()));

  light->SetAttenuationConstant(msg.attenuation_constant());
  light->SetAttenuationLinear(msg.attenuation_linear());
  light->SetAttenuationQuadratic(msg.attenuation_quadratic());
  light->SetAttenuationRange(msg.range());

  light->SetCastShadows(msg.cast_shadows());

  return light;
}
}  // namespace tesseract_gui
