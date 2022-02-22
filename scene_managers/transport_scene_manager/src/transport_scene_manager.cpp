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

#include <algorithm>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include <ignition/common/Console.hh>
#include <ignition/common/MeshManager.hh>
#include <ignition/math/Pose3.hh>
#include <ignition/math/Vector3.hh>

// TODO(louise) Remove these pragmas once ign-rendering and ign-msgs
// are disabling the warnings
#ifdef _MSC_VER
#pragma warning(push, 0)
#endif
#include <ignition/msgs.hh>

#include <ignition/rendering/RenderEngine.hh>
#include <ignition/rendering/RenderingIface.hh>
#include <ignition/rendering/Scene.hh>

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#include <ignition/transport/Node.hh>

#include <ignition/gui/Application.hh>
#include <ignition/gui/Conversions.hh>
#include <ignition/gui/GuiEvents.hh>
#include <ignition/gui/MainWindow.hh>

#include <tesseract_gui/common/gui_utils.h>
#include <tesseract_gui/transport_scene_manager/transport_conversions.h>
#include <tesseract_gui/transport_scene_manager/transport_scene_manager.h>

/// \brief Private data class for TransportSceneManager
class tesseract_gui::TransportSceneManagerPrivate
{
  /// \brief Make the scene service request and populate the scene
  public: void Request();

  /// \brief Update the scene based on pose msgs received
  public: void OnRender();

  /// \brief Initialize transport, subscribing to the necessary topics.
  /// To be called after a valid scene has been found.
  public: void InitializeTransport();

  /// \brief Callback function for the pose topic
  /// \param[in] _msg Pose vector msg
  public: void OnPoseVMsg(const ignition::msgs::Pose_V &_msg);

  /// \brief Load the scene from a scene msg
  /// \param[in] _msg Scene msg
  public: void LoadScene(const ignition::msgs::Scene &_msg);

  /// \brief Callback function for the request topic
  /// \param[in] _msg Deletion message
  public: void OnDeletionMsg(const ignition::msgs::UInt32_V &_msg);

  /// \brief Load the scene from a scene msg
  /// \param[in] _msg Scene msg
  public: void OnSceneSrvMsg(const ignition::msgs::Scene &_msg, const bool result);

  /// \brief Called when there's an entity is added to the scene
  /// \param[in] _msg Scene msg
  public: void OnSceneMsg(const ignition::msgs::Scene &_msg);

  /// \brief Load the model from a model msg
  /// \param[in] _msg Model msg
  /// \return Model visual created from the msg
  public: ignition::rendering::VisualPtr LoadModel(const ignition::msgs::Model &_msg);

  /// \brief Load a link from a link msg
  /// \param[in] _msg Link msg
  /// \return Link visual created from the msg
  public: ignition::rendering::VisualPtr LoadLink(const ignition::msgs::Link &_msg);

  /// \brief Load a visual from a visual msg
  /// \param[in] _msg Visual msg
  /// \return Visual visual created from the msg
  public: ignition::rendering::VisualPtr LoadVisual(const ignition::msgs::Visual &_msg);

  /// \brief Load a light from a light msg
  /// \param[in] _msg Light msg
  /// \return Light object created from the msg
  public: ignition::rendering::LightPtr LoadLight(const ignition::msgs::Light &_msg);

  /// \brief Delete an entity
  /// \param[in] _entity Entity to delete
  public: void DeleteEntity(const unsigned int _entity);

  //// \brief Ign-transport scene service name
  public: std::string service;

  //// \brief Ign-transport pose topic name
  public: std::string poseTopic;

  //// \brief Ign-transport deletion topic name
  public: std::string deletionTopic;

  //// \brief Ign-transport scene topic name
  public: std::string sceneTopic;

  //// \brief Pointer to the rendering scene
  public: ignition::rendering::ScenePtr scene{nullptr};

  //// \brief Mutex to protect the msgs
  public: std::mutex msgMutex;

  /// \brief Map of entity id to pose
  public: std::map<unsigned int, ignition::math::Pose3d> poses;

  /// \brief Map of entity id to initial local poses
  /// This is currently used to handle the normal vector in plane visuals. In
  /// general, this can be used to store any local transforms between the
  /// parent Visual and geometry.
  public: std::map<unsigned int, ignition::math::Pose3d> localPoses;

  /// \brief Map of visual id to visual pointers.
  public: std::map<unsigned int, ignition::rendering::VisualPtr::weak_type> visuals;

  /// \brief Map of light id to light pointers.
  public: std::map<unsigned int, ignition::rendering::LightPtr::weak_type> lights;

  /// Entities to be deleted
  public: std::vector<unsigned int> toDeleteEntities;

  /// \brief Keeps the a list of unprocessed scene messages
  public: std::vector<ignition::msgs::Scene> sceneMsgs;

  /// \brief Transport node for making service request and subscribing to
  /// pose topic
  public: ignition::transport::Node node;
};

using namespace tesseract_gui;

/////////////////////////////////////////////////
TransportSceneManager::TransportSceneManager()
  : QObject(), dataPtr(new TransportSceneManagerPrivate)
{
}

/////////////////////////////////////////////////
TransportSceneManager::~TransportSceneManager()
{
}

TransportSceneManager::TransportSceneManager(const std::string& scene_topic,
                                             const std::string& pose_topic,
                                             const std::string& deletion_topic,
                                             const std::string& service)
{
  if (!scene_topic.empty())
    this->dataPtr->sceneTopic = scene_topic;

  if (!pose_topic.empty())
    this->dataPtr->poseTopic = pose_topic;

  if (!deletion_topic.empty())
    this->dataPtr->deletionTopic = deletion_topic;

  if (!service.empty())
    this->dataPtr->service = service;
}

///////////////////////////////////////////////////
//void TransportSceneManager::LoadConfig(const tinyxml2::XMLElement *_pluginElem)
//{
//  if (this->title.empty())
//    this->title = "Transport Scene Manager";

//  // Custom parameters
//  if (_pluginElem)
//  {
//    auto elem = _pluginElem->FirstChildElement("service");
//    if (nullptr != elem && nullptr != elem->GetText())
//    {
//      this->dataPtr->service = elem->GetText();
//    }

//    elem = _pluginElem->FirstChildElement("pose_topic");
//    if (nullptr != elem && nullptr != elem->GetText())
//    {
//      this->dataPtr->poseTopic = elem->GetText();
//    }

//    elem = _pluginElem->FirstChildElement("deletion_topic");
//    if (nullptr != elem && nullptr != elem->GetText())
//    {
//      this->dataPtr->deletionTopic = elem->GetText();
//    }

//    elem = _pluginElem->FirstChildElement("scene_topic");
//    if (nullptr != elem && nullptr != elem->GetText())
//    {
//      this->dataPtr->sceneTopic = elem->GetText();
//    }
//  }

//  App()->findChild<MainWindow *>()->installEventFilter(this);
//}

/////////////////////////////////////////////////
void TransportSceneManagerPrivate::InitializeTransport()
{
  this->Request();

  if (!this->node.Subscribe(this->poseTopic,
      &TransportSceneManagerPrivate::OnPoseVMsg, this))
  {
    ignerr << "Error subscribing to pose topic: " << this->poseTopic
      << std::endl;
  }
  else
  {
    ignmsg << "Listening to pose messages on [" << this->poseTopic << "]"
           << std::endl;
  }

  if (!this->node.Subscribe(this->deletionTopic,
      &TransportSceneManagerPrivate::OnDeletionMsg, this))
  {
    ignerr << "Error subscribing to deletion topic: " << this->deletionTopic
      << std::endl;
  }
  else
  {
    ignmsg << "Listening to deletion messages on [" << this->deletionTopic
           << "]" << std::endl;
  }

  if (!this->node.Subscribe(this->sceneTopic,
      &TransportSceneManagerPrivate::OnSceneMsg, this))
  {
    ignerr << "Error subscribing to scene topic: " << this->sceneTopic
           << std::endl;
  }
  else
  {
    ignmsg << "Listening to scene messages on [" << this->sceneTopic << "]"
           << std::endl;
  }

  ignmsg << "Transport initialized." << std::endl;
}

/////////////////////////////////////////////////
bool TransportSceneManager::eventFilter(QObject *_obj, QEvent *_event)
{
  if (_event->type() == ignition::gui::events::Render::kType)
  {
    this->dataPtr->OnRender();
  }

  // Standard event processing
  return QObject::eventFilter(_obj, _event);
}

/////////////////////////////////////////////////
void TransportSceneManagerPrivate::Request()
{
  // wait for the service to be advertised
  std::vector<ignition::transport::ServicePublisher> publishers;
  const std::chrono::duration<double> sleepDuration{1.0};
  const std::size_t tries = 30;
  for (std::size_t i = 0; i < tries; ++i)
  {
    this->node.ServiceInfo(this->service, publishers);
    if (publishers.size() > 0)
      break;
    std::this_thread::sleep_for(sleepDuration);
    igndbg << "Waiting for service " << this->service << "\n";
  }

  if (publishers.empty() || !this->node.Request(this->service,
      &TransportSceneManagerPrivate::OnSceneSrvMsg, this))
  {
    ignerr << "Error making service request to " << this->service << std::endl;
  }
}

/////////////////////////////////////////////////
void TransportSceneManagerPrivate::OnPoseVMsg(const ignition::msgs::Pose_V &_msg)
{
  std::lock_guard<std::mutex> lock(this->msgMutex);
  for (int i = 0; i < _msg.pose_size(); ++i)
  {
    ignition::math::Pose3d pose = ignition::msgs::Convert(_msg.pose(i));

    // apply additional local poses if available
    const auto it = this->localPoses.find(_msg.pose(i).id());
    if (it != this->localPoses.end())
    {
      pose = pose * it->second;
    }

    this->poses[_msg.pose(i).id()] = pose;
  }
}

/////////////////////////////////////////////////
void TransportSceneManagerPrivate::OnDeletionMsg(const ignition::msgs::UInt32_V &_msg)
{
  std::lock_guard<std::mutex> lock(this->msgMutex);
  std::copy(_msg.data().begin(), _msg.data().end(),
            std::back_inserter(this->toDeleteEntities));
}

/////////////////////////////////////////////////
void TransportSceneManagerPrivate::OnRender()
{
  if (nullptr == this->scene)
  {
    this->scene = ignition::rendering::sceneFromFirstRenderEngine();
    if (nullptr == this->scene)
      return;

    this->InitializeTransport();
  }

  std::lock_guard<std::mutex> lock(this->msgMutex);

  for (const auto &msg : this->sceneMsgs)
  {
    this->LoadScene(msg);
  }
  this->sceneMsgs.clear();

  for (const auto &entity : this->toDeleteEntities)
  {
    this->DeleteEntity(entity);
  }
  this->toDeleteEntities.clear();

  for (auto pIt = this->poses.begin(); pIt != this->poses.end();)
  {
    auto vIt = this->visuals.find(pIt->first);
    if (vIt != this->visuals.end())
    {
      auto visual = vIt->second.lock();
      if (visual)
      {
        visual->SetLocalPose(pIt->second);
      }
      else
      {
        this->visuals.erase(vIt);
      }
      this->poses.erase(pIt++);
    }
    else
    {
      auto lIt = this->lights.find(pIt->first);
      if (lIt != this->lights.end())
      {
        auto light = lIt->second.lock();
        if (light)
        {
          light->SetLocalPose(pIt->second);
        }
        else
        {
          this->lights.erase(lIt);
        }
        this->poses.erase(pIt++);
      }
      else
      {
        ++pIt;
      }
    }
  }

  // Note we are clearing the pose msgs here but later on we may need to
  // consider the case where pose msgs arrive before scene/visual msgs
  this->poses.clear();
}

/////////////////////////////////////////////////
void TransportSceneManagerPrivate::OnSceneMsg(const ignition::msgs::Scene &_msg)
{
  std::lock_guard<std::mutex> lock(this->msgMutex);
  this->sceneMsgs.push_back(_msg);
}

/////////////////////////////////////////////////
void TransportSceneManagerPrivate::OnSceneSrvMsg(const ignition::msgs::Scene &_msg,
    const bool result)
{
  if (!result)
  {
    ignerr << "Error making service request to " << this->service
           << std::endl;
    return;
  }

  {
    std::lock_guard<std::mutex> lock(this->msgMutex);
    this->sceneMsgs.push_back(_msg);
  }
}

/////////////////////////////////////////////////
void TransportSceneManagerPrivate::LoadScene(const ignition::msgs::Scene &_msg)
{
  ignition::rendering::VisualPtr rootVis = this->scene->RootVisual();

  // load models
  for (int i = 0; i < _msg.model_size(); ++i)
  {
    // Only add if it's not already loaded
    if (this->visuals.find(_msg.model(i).id()) == this->visuals.end())
    {
      ignition::rendering::VisualPtr modelVis = this->LoadModel(_msg.model(i));
      if (modelVis)
        rootVis->AddChild(modelVis);
      else
        ignerr << "Failed to load model: " << _msg.model(i).name() << std::endl;
    }
  }

  // load lights
  for (int i = 0; i < _msg.light_size(); ++i)
  {
    if (this->lights.find(_msg.light(i).id()) == this->lights.end())
    {
      ignition::rendering::LightPtr light = this->LoadLight(_msg.light(i));
      if (light)
        rootVis->AddChild(light);
      else
        ignerr << "Failed to load light: " << _msg.light(i).name() << std::endl;
    }
  }
}

/////////////////////////////////////////////////
ignition::rendering::VisualPtr TransportSceneManagerPrivate::LoadModel(
    const ignition::msgs::Model &_msg)
{
  ignition::rendering::VisualPtr modelVis = loadModel(*(this->scene), _msg);
  this->visuals[_msg.id()] = modelVis;
  return modelVis;
}

/////////////////////////////////////////////////
ignition::rendering::VisualPtr TransportSceneManagerPrivate::LoadLink(const ignition::msgs::Link &_msg)
{
  ignition::rendering::VisualPtr linkVis = loadLink(*(this->scene), _msg);
  this->visuals[_msg.id()] = linkVis;
  return linkVis;
}

/////////////////////////////////////////////////
ignition::rendering::VisualPtr TransportSceneManagerPrivate::LoadVisual(const ignition::msgs::Visual &_msg)
{
  ignition::rendering::VisualPtr visualVis = loadVisual(*(this->scene), _msg);
  this->visuals[_msg.id()] = visualVis;
  this->localPoses[_msg.id()] = visualVis->LocalPose();
  return visualVis;
}

/////////////////////////////////////////////////
ignition::rendering::LightPtr TransportSceneManagerPrivate::LoadLight(const ignition::msgs::Light &_msg)
{
  ignition::rendering::LightPtr light = loadLight(*(this->scene), _msg);
  this->lights[_msg.id()] = light;
  return light;
}

/////////////////////////////////////////////////
void TransportSceneManagerPrivate::DeleteEntity(const unsigned int _entity)
{
  if (this->visuals.find(_entity) != this->visuals.end())
  {
    auto visual = this->visuals[_entity].lock();
    if (visual)
    {
      this->scene->DestroyVisual(visual, true);
    }
    this->visuals.erase(_entity);
  }
  else if (this->lights.find(_entity) != this->lights.end())
  {
    auto light = this->lights[_entity].lock();
    if (light)
    {
      this->scene->DestroyLight(light, true);
    }
    this->lights.erase(_entity);
  }
}
