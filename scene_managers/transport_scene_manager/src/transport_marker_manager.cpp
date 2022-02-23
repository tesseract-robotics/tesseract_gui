/*
 * Copyright (C) 2021 Open Source Robotics Foundation
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
#include <list>
#include <map>
#include <string>

#include <ignition/common/Console.hh>
#include <ignition/common/Profiler.hh>
#include <ignition/common/StringUtils.hh>

#include <ignition/math/Rand.hh>

#ifdef _MSC_VER
#pragma warning(push, 0)
#endif
#include <ignition/msgs.hh>
#ifdef _MSC_VER
#pragma warning(pop)
#endif

#include <ignition/rendering/Marker.hh>
#include <ignition/rendering/RenderingIface.hh>
#include <ignition/rendering/Scene.hh>

#include <ignition/transport/Node.hh>

#include <ignition/gui/Application.hh>
#include <ignition/gui/GuiEvents.hh>
#include <ignition/gui/Helpers.hh>
#include <ignition/gui/MainWindow.hh>

#include <tesseract_gui/transport_scene_manager/transport_marker_manager.h>

/// \brief Private data class for TransportMarkerManager
class tesseract_gui::TransportMarkerManagerPrivate
{
public:
  /// \brief Update markers based on msgs received
  void OnRender();

  /// \brief Initialize services and subscriptions
  void Initialize();

  /// \brief Processes a marker message.
  /// \param[in] _msg The message data.
  /// \return True if the marker was processed successfully.
  bool ProcessMarkerMsg(const ignition::msgs::Marker &_msg);

  /// \brief Services callback that returns a list of markers.
  /// \param[out] _rep Service reply
  /// \return True on success.
  bool OnList(ignition::msgs::Marker_V &_rep);

  /// \brief Callback that receives marker messages.
  /// \param[in] _req The marker message.
  void OnMarkerMsg(const ignition::msgs::Marker &_req);

  /// \brief Callback that receives multiple marker messages.
  /// \param[in] _req The vector of marker messages
  /// \param[in] _res Response data
  /// \return True if the request is received
  bool OnMarkerMsgArray(const ignition::msgs::Marker_V &_req,
              ignition::msgs::Boolean &_res);

  /// \brief Subscriber callback when new world statistics are received
  void OnWorldStatsMsg(const ignition::msgs::WorldStatistics &_msg);

  /// \brief Sets Visual from marker message.
  /// \param[in] _msg The message data.
  /// \param[out] _visualPtr The visual pointer to set.
  void SetVisual(const ignition::msgs::Marker &_msg,
                         const ignition::rendering::VisualPtr &_visualPtr);

  /// \brief Sets Marker from marker message.
  /// \param[in] _msg The message data.
  /// \param[out] _markerPtr The message pointer to set.
  void SetMarker(const ignition::msgs::Marker &_msg,
                         const ignition::rendering::MarkerPtr &_markerPtr);

  /// \brief Converts an ignition msg material to ignition rendering
  //         material.
  //  \param[in] _msg The message data.
  //  \return Converted rendering material, if any.
  ignition::rendering::MaterialPtr MsgToMaterial(
    const ignition::msgs::Marker &_msg);

  /// \brief Converts an ignition msg render type to ignition rendering
  /// \param[in] _msg The message data
  /// \return Converted rendering type, if any.
  ignition::rendering::MarkerType MsgToType(
                    const ignition::msgs::Marker &_msg);

  //// \brief Pointer to the rendering scene
  ignition::rendering::ScenePtr scene{nullptr};

  /// \brief Mutex to protect message list.
  std::mutex mutex;

  /// \brief List of marker message to process.
  std::list<ignition::msgs::Marker> markerMsgs;

  /// \brief Map of visuals
  std::map<std::string,
      std::map<uint64_t, ignition::rendering::VisualPtr>> visuals;

  /// \brief Ignition node
  ignition::transport::Node node;

  /// \brief Topic name for the marker service
  std::string topicName = "/marker";

  /// \brief Sim time according to world stats message
  std::chrono::steady_clock::duration simTime;

  /// \brief Previous sim time received
  std::chrono::steady_clock::duration lastSimTime;

  /// \brief The last marker message received
  ignition::msgs::Marker msg;

  /// \brief True to print console warnings if the user tries to perform an
  /// action with an inexistent marker.
  bool warnOnActionFailure{true};
};

using namespace tesseract_gui;

/////////////////////////////////////////////////
void TransportMarkerManagerPrivate::Initialize()
{
  if (!this->scene)
  {
    ignerr << "Scene pointer is invalid" << std::endl;
    return;
  }

  if (this->topicName.empty())
  {
    ignerr << "Unable to advertise marker service. Topic name empty."
           << std::endl;
    return;
  }

  // Advertise the list service
  if (!this->node.Advertise(this->topicName + "/list",
      &TransportMarkerManagerPrivate::OnList, this))
  {
    ignerr << "Unable to advertise to the " << this->topicName
           << "/list service.\n";
  }

  igndbg << "Advertise " << this->topicName << "/list service.\n";

  // Advertise to the marker service
  if (!this->node.Advertise(this->topicName,
        &TransportMarkerManagerPrivate::OnMarkerMsg, this))
  {
    ignerr << "Unable to advertise to the " << this->topicName
           << " service.\n";
  }

  igndbg << "Advertise " << this->topicName << "/list.\n";

  // Advertise to the marker_array service
  if (!this->node.Advertise(this->topicName + "_array",
        &TransportMarkerManagerPrivate::OnMarkerMsgArray, this))
  {
    ignerr << "Unable to advertise to the " << this->topicName
           << "_array service.\n";
  }

  igndbg << "Advertise " << this->topicName << "_array.\n";
}

/////////////////////////////////////////////////
void TransportMarkerManagerPrivate::OnRender()
{
  if (!this->scene)
  {
    this->scene = ignition::rendering::sceneFromFirstRenderEngine();
    if (nullptr == this->scene)
      return;

    this->Initialize();
  }

  std::lock_guard<std::mutex> lock(this->mutex);
  // Process the marker messages.
  for (auto markerIter = this->markerMsgs.begin();
       markerIter != this->markerMsgs.end();)
  {
    this->ProcessMarkerMsg(*markerIter);
    this->markerMsgs.erase(markerIter++);
  }

  // Erase any markers that have a lifetime.
  for (auto mit = this->visuals.begin();
       mit != this->visuals.end();)
  {
    for (auto it = mit->second.cbegin();
         it != mit->second.cend(); ++it)
    {
      if (it->second->GeometryCount() == 0U)
        continue;

      ignition::rendering::MarkerPtr markerPtr =
            std::dynamic_pointer_cast<ignition::rendering::Marker>
            (it->second->GeometryByIndex(0U));
      if (markerPtr != nullptr)
      {
        if (markerPtr->Lifetime().count() != 0 &&
            (markerPtr->Lifetime() <= this->simTime ||
            this->simTime < this->lastSimTime))
        {
          this->scene->DestroyVisual(it->second);
          it = mit->second.erase(it);
          break;
        }
      }
    }

    // Erase a namespace if it's empty
    if (mit->second.empty())
      mit = this->visuals.erase(mit);
    else
      ++mit;
  }
  this->lastSimTime = this->simTime;
}

/////////////////////////////////////////////////
bool TransportMarkerManagerPrivate::OnList(ignition::msgs::Marker_V &_rep)
{
  std::lock_guard<std::mutex> lock(this->mutex);
  _rep.clear_marker();

  // Create the list of visuals
  for (const auto& mIter : this->visuals)
  {
    for (const auto& iter : mIter.second)
    {
      ignition::msgs::Marker *markerMsg = _rep.add_marker();
      markerMsg->set_ns(mIter.first);
      markerMsg->set_id(iter.first);
    }
  }

  return true;
}

/////////////////////////////////////////////////
void TransportMarkerManagerPrivate::OnMarkerMsg(const ignition::msgs::Marker &_req)
{
  std::lock_guard<std::mutex> lock(this->mutex);
  this->markerMsgs.push_back(_req);
}

/////////////////////////////////////////////////
bool TransportMarkerManagerPrivate::OnMarkerMsgArray(
    const ignition::msgs::Marker_V&_req, ignition::msgs::Boolean &_res)
{
  std::lock_guard<std::mutex> lock(this->mutex);
  std::copy(_req.marker().begin(), _req.marker().end(),
            std::back_inserter(this->markerMsgs));
  _res.set_data(true);
  return true;
}

//////////////////////////////////////////////////
bool TransportMarkerManagerPrivate::ProcessMarkerMsg(const ignition::msgs::Marker &_msg)
{
  // Get the namespace, if it exists. Otherwise, use the global namespace
  std::string ns;
  if (!_msg.ns().empty()) {
    ns = _msg.ns();
  }

  // Get the namespace that the marker belongs to
  auto nsIter = this->visuals.find(ns);

  // If an id is given
  size_t id{0};
  if (_msg.id() != 0)
  {
    id = _msg.id();
  }
  // Otherwise generate unique id
  else
  {
    id = ignition::math::Rand::IntUniform(0, ignition::math::MAX_I32);

    // Make sure it's unique if namespace is given
    if (nsIter != this->visuals.end())
    {
      while (nsIter->second.find(id) != nsIter->second.end())
        id = ignition::math::Rand::IntUniform(ignition::math::MIN_UI32,
                                              ignition::math::MAX_UI32);
    }
  }

  // Get visual for this namespace and id
  std::map<uint64_t, ignition::rendering::VisualPtr>::iterator visualIter;
  if (nsIter != this->visuals.end())
    visualIter = nsIter->second.find(id);

  // Add/modify a marker
  if (_msg.action() == ignition::msgs::Marker::ADD_MODIFY)
  {
    // Modify an existing marker, identified by namespace and id
    if (nsIter != this->visuals.end() &&
        visualIter != nsIter->second.end())
    {
      if (visualIter->second->GeometryCount() > 0U)
      {
        // TODO(anyone): Update so that multiple markers can
        //               be attached to one visual
        ignition::rendering::MarkerPtr markerPtr =
              std::dynamic_pointer_cast<ignition::rendering::Marker>
              (visualIter->second->GeometryByIndex(0));

        visualIter->second->RemoveGeometryByIndex(0);

        // Set the visual values from the Marker Message
        this->SetVisual(_msg, visualIter->second);

        // Set the marker values from the Marker Message
        this->SetMarker(_msg, markerPtr);

        visualIter->second->AddGeometry(markerPtr);
      }
    }
    // Otherwise create a new marker
    else
    {
      // Create the name for the marker
      std::string name = "__IGN_MARKER_VISUAL_" + ns + "_" +
                         std::to_string(id);

      // Create the new marker
      ignition::rendering::VisualPtr visualPtr = this->scene->CreateVisual(name);

      // Create and load the marker
      ignition::rendering::MarkerPtr markerPtr = this->scene->CreateMarker();

      // Set the visual values from the Marker Message
      this->SetVisual(_msg, visualPtr);

      // Set the marker values from the Marker Message
      this->SetMarker(_msg, markerPtr);

      // Add populated marker to the visual
      visualPtr->AddGeometry(markerPtr);

      // Add visual to root visual
      if (!visualPtr->HasParent())
      {
        this->scene->RootVisual()->AddChild(visualPtr);
      }

      // Store the visual
      this->visuals[ns][id] = visualPtr;
    }
  }
  // Remove a single marker
  else if (_msg.action() == ignition::msgs::Marker::DELETE_MARKER)
  {
    // Remove the marker if it can be found.
    if (nsIter != this->visuals.end() &&
        visualIter != nsIter->second.end())
    {
      this->scene->DestroyVisual(visualIter->second);
      this->visuals[ns].erase(visualIter);

      // Remove namespace if empty
      if (this->visuals[ns].empty())
        this->visuals.erase(nsIter);
    }
    else
    {
      if (this->warnOnActionFailure)
      {
        ignwarn << "Unable to delete marker with id[" << id << "] "
                << "in namespace[" << ns << "]" << std::endl;
      }
      return false;
    }
  }
  // Remove all markers, or all markers in a namespace
  else if (_msg.action() == ignition::msgs::Marker::DELETE_ALL)
  {
    // If given namespace doesn't exist
    if (!ns.empty() && nsIter == this->visuals.end())
    {
      if (this->warnOnActionFailure)
      {
        ignwarn << "Unable to delete all markers in namespace[" << ns
                << "], namespace can't be found." << std::endl;
      }
      return false;
    }

    // Remove all markers in the specified namespace
    if (nsIter != this->visuals.end())
    {
      for (const auto& it : nsIter->second)
      {
        this->scene->DestroyVisual(it.second);
      }
      nsIter->second.clear();
      this->visuals.erase(nsIter);
    }
    // Remove all markers in all namespaces.
    else
    {
      for (nsIter = this->visuals.begin();
           nsIter != this->visuals.end(); ++nsIter)
      {
        for (const auto& it : nsIter->second)
        {
          this->scene->DestroyVisual(it.second);
        }
      }
      this->visuals.clear();
    }
  }
  else
  {
    ignerr << "Unknown marker action[" << _msg.action() << "]\n";
    return false;
  }

  return true;
}

/////////////////////////////////////////////////
void TransportMarkerManagerPrivate::SetVisual(const ignition::msgs::Marker &_msg,
                           const ignition::rendering::VisualPtr &_visualPtr)
{
  // Set Visual Scale
  // The scale for points is used as the size of each point, so skip it here.
  if (_msg.has_scale() && _msg.type() != ignition::msgs::Marker::POINTS)
  {
    _visualPtr->SetLocalScale(_msg.scale().x(),
                              _msg.scale().y(),
                              _msg.scale().z());
  }

  // Set Visual Pose
  if (_msg.has_pose())
  {
    ignition::math::Pose3d pose(_msg.pose().position().x(),
                      _msg.pose().position().y(),
                      _msg.pose().position().z(),
                      _msg.pose().orientation().w(),
                      _msg.pose().orientation().x(),
                      _msg.pose().orientation().y(),
                      _msg.pose().orientation().z());
    pose.Correct();
    _visualPtr->SetLocalPose(pose);
  }

  // Set Visual Parent
  if (!_msg.parent().empty())
  {
    if (_visualPtr->HasParent())
    {
      _visualPtr->Parent()->RemoveChild(_visualPtr);
    }

    ignition::rendering::VisualPtr parent = this->scene->VisualByName(_msg.parent());

    if (parent)
    {
      parent->AddChild(_visualPtr);
    }
    else
    {
      ignerr << "No visual with the name[" << _msg.parent() << "]\n";
    }
  }

  // todo(anyone) Update Marker Visibility
}

/////////////////////////////////////////////////
void TransportMarkerManagerPrivate::SetMarker(const ignition::msgs::Marker &_msg,
                           const ignition::rendering::MarkerPtr &_markerPtr)
{
  _markerPtr->SetLayer(_msg.layer());

  // Set Marker Lifetime
  std::chrono::steady_clock::duration lifetime =
    std::chrono::seconds(_msg.lifetime().sec()) +
    std::chrono::nanoseconds(_msg.lifetime().nsec());

  if (lifetime.count() != 0)
  {
    _markerPtr->SetLifetime(lifetime + this->simTime);
  }
  else
  {
    _markerPtr->SetLifetime(std::chrono::seconds(0));
  }
  // Set Marker Render Type
  ignition::rendering::MarkerType markerType = MsgToType(_msg);
  _markerPtr->SetType(markerType);

  // Set Marker Material
  if (_msg.has_material())
  {
    ignition::rendering::MaterialPtr materialPtr = MsgToMaterial(_msg);
    _markerPtr->SetMaterial(materialPtr, true /* clone */);

    // clean up material after clone
    this->scene->DestroyMaterial(materialPtr);
  }

  // Assume the presence of points means we clear old ones
  if (!_msg.point().empty())
  {
    _markerPtr->ClearPoints();
  }

  ignition::math::Color color(
      _msg.material().diffuse().r(),
      _msg.material().diffuse().g(),
      _msg.material().diffuse().b(),
      _msg.material().diffuse().a());

  // Set Marker Points
  for (int i = 0; i < _msg.point().size(); ++i)
  {
    ignition::math::Vector3d vector(
        _msg.point(i).x(),
        _msg.point(i).y(),
        _msg.point(i).z());

    _markerPtr->AddPoint(vector, color);
  }
  if (_msg.has_scale())
  {
    _markerPtr->SetSize(_msg.scale().x());
  }
}

/////////////////////////////////////////////////
ignition::rendering::MaterialPtr TransportMarkerManagerPrivate::MsgToMaterial(const ignition::msgs::Marker &_msg)
{
  ignition::rendering::MaterialPtr material = this->scene->CreateMaterial();

  material->SetAmbient(
      _msg.material().ambient().r(),
      _msg.material().ambient().g(),
      _msg.material().ambient().b(),
      _msg.material().ambient().a());

  material->SetDiffuse(
      _msg.material().diffuse().r(),
      _msg.material().diffuse().g(),
      _msg.material().diffuse().b(),
      _msg.material().diffuse().a());

  material->SetSpecular(
      _msg.material().specular().r(),
      _msg.material().specular().g(),
      _msg.material().specular().b(),
      _msg.material().specular().a());

  material->SetEmissive(
      _msg.material().emissive().r(),
      _msg.material().emissive().g(),
      _msg.material().emissive().b(),
      _msg.material().emissive().a());

  material->SetLightingEnabled(_msg.material().lighting());

  return material;
}

/////////////////////////////////////////////////
ignition::rendering::MarkerType TransportMarkerManagerPrivate::MsgToType(
                          const ignition::msgs::Marker &_msg)
{
  ignition::msgs::Marker_Type marker = this->msg.type();
  if (marker != _msg.type() && _msg.type() != ignition::msgs::Marker::NONE)
  {
    marker = _msg.type();
    this->msg.set_type(_msg.type());
  }
  switch (marker)
  {
    case ignition::msgs::Marker::BOX:
      return ignition::rendering::MarkerType::MT_BOX;
    case ignition::msgs::Marker::CAPSULE:
      return ignition::rendering::MarkerType::MT_CAPSULE;
    case ignition::msgs::Marker::CYLINDER:
      return ignition::rendering::MarkerType::MT_CYLINDER;
    case ignition::msgs::Marker::LINE_STRIP:
      return ignition::rendering::MarkerType::MT_LINE_STRIP;
    case ignition::msgs::Marker::LINE_LIST:
      return ignition::rendering::MarkerType::MT_LINE_LIST;
    case ignition::msgs::Marker::POINTS:
      return ignition::rendering::MarkerType::MT_POINTS;
    case ignition::msgs::Marker::SPHERE:
      return ignition::rendering::MarkerType::MT_SPHERE;
    case ignition::msgs::Marker::TEXT:
      return ignition::rendering::MarkerType::MT_TEXT;
    case ignition::msgs::Marker::TRIANGLE_FAN:
      return ignition::rendering::MarkerType::MT_TRIANGLE_FAN;
    case ignition::msgs::Marker::TRIANGLE_LIST:
      return ignition::rendering::MarkerType::MT_TRIANGLE_LIST;
    case ignition::msgs::Marker::TRIANGLE_STRIP:
      return ignition::rendering::MarkerType::MT_TRIANGLE_STRIP;
    default:
      ignerr << "Unable to create marker of type[" << _msg.type() << "]\n";
      break;
  }
  return ignition::rendering::MarkerType::MT_NONE;
}

/////////////////////////////////////////////////
void TransportMarkerManagerPrivate::OnWorldStatsMsg(
  const ignition::msgs::WorldStatistics &_msg)
{
  std::lock_guard<std::mutex> lock(this->mutex);
  std::chrono::steady_clock::duration timePoint;
  if (_msg.has_sim_time())
  {
    timePoint = ignition::math::secNsecToDuration(
        _msg.sim_time().sec(),
        _msg.sim_time().nsec());
    this->simTime = timePoint;
  }
  else if (_msg.has_real_time())
  {
    timePoint = ignition::math::secNsecToDuration(
        _msg.real_time().sec(),
        _msg.real_time().nsec());
    this->simTime = timePoint;
  }
}

/////////////////////////////////////////////////
TransportMarkerManager::TransportMarkerManager()
  : dataPtr(new TransportMarkerManagerPrivate)
{
}

/////////////////////////////////////////////////
TransportMarkerManager::~TransportMarkerManager() = default;

/////////////////////////////////////////////////
//void TransportMarkerManager::LoadConfig(const tinyxml2::XMLElement * _pluginElem)
//{
//  if (this->title.empty())
//    this->title = "Marker Manager";

//  // Custom parameters
//  if (_pluginElem)
//  {
//    auto elem = _pluginElem->FirstChildElement("topic_name");
//    if (nullptr != elem && nullptr != elem->GetText())
//    {
//      std::string topic = transport::TopicUtils::AsValidTopic(elem->GetText());
//      if (!topic.empty())
//      {
//        this->dataPtr->topicName = topic;
//      }
//      else
//      {
//        ignerr << "the provided topic is no allowed. Using default ["
//               << this->dataPtr->topicName << "]"<<  std::endl;
//      }
//    }

//    if ((elem = _pluginElem->FirstChildElement("warn_on_action_failure")))
//    {
//      if (elem->QueryBoolText(&this->dataPtr->warnOnActionFailure) !=
//          tinyxml2::XML_SUCCESS)
//      {
//        ignerr << "Faild to parse <warn_on_action_failure> value: "
//               << elem->GetText() << std::endl;
//      }
//    }

//    // World name from window, to construct default topics and services
//    std::string worldName = "example";
//    auto worldNames = gui::worldNames();
//    if (!worldNames.empty())
//      worldName = worldNames[0].toStdString();

//    // Subscribe to world stats
//    std::string statsTopic;
//    auto statsTopicElem = _pluginElem->FirstChildElement("stats_topic");
//    if (nullptr != statsTopicElem && nullptr != statsTopicElem->GetText())
//      statsTopic = statsTopicElem->GetText();

//    // Service specified with different world name
//    auto parts = common::Split(statsTopic, '/');
//    if (!worldName.empty() &&
//        parts.size() == 4 &&
//        parts[0] == "" &&
//        parts[1] == "world" &&
//        parts[2] != worldName &&
//        parts[3] == "stats")
//    {
//      ignwarn << "Ignoring topic [" << statsTopic
//              << "], world name different from [" << worldName
//              << "]. Fix or remove your <stats_topic> tag." << std::endl;

//      statsTopic = "/world/" + worldName + "/stats";
//    }

//    if (statsTopic.empty() && !worldName.empty())
//    {
//      statsTopic = "/world/" + worldName + "/stats";
//    }

//    statsTopic = transport::TopicUtils::AsValidTopic(statsTopic);
//    if (!statsTopic.empty())
//    {
//      // Subscribe to world_stats
//      if (!this->dataPtr->node.Subscribe(statsTopic,
//          &MarkerManagerPrivate::OnWorldStatsMsg, this->dataPtr.get()))
//      {
//        ignerr << "Failed to subscribe to [" << statsTopic << "]" << std::endl;
//      }
//      else
//      {
//        ignmsg << "Listening to stats on [" << statsTopic << "]" << std::endl;
//      }
//    }
//    else
//    {
//      ignerr << "Failed to create valid topic for world [" << worldName << "]"
//             << std::endl;
//    }
//  }

//  App()->findChild<MainWindow *>()->installEventFilter(this);
//}

/////////////////////////////////////////////////
bool TransportMarkerManager::eventFilter(QObject *_obj, QEvent *_event)
{
  if (_event->type() == ignition::gui::events::Render::kType)
  {
    this->dataPtr->OnRender();
  }
  // Standard event processing
  return QObject::eventFilter(_obj, _event);
}
