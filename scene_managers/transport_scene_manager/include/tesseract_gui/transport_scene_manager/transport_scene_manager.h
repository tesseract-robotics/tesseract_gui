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
#ifndef TESSERACT_GUI_TRANSPORT_SCENE_MANAGER_H
#define TESSERACT_GUI_TRANSPORT_SCENE_MANAGER_H

#include <memory>
#include <QObject>
#include <QtEvents>

namespace tesseract_gui
{
  class TransportSceneManagerPrivate;

  /// \brief Provides an Ignition Transport interface to tesseract_gui::MinimalScene`.
  ///
  /// ## Configuration
  ///
  /// * \<service\> : Name of service where this system will request a scene message.
  /// * \<pose_topic\> : Name of topic to subscribe to receive pose updates.
  /// * \<deletion_topic\> : Name of topic to request entity deletions.
  /// * \<scene_topic\> : Name of topic to receive scene updates.
  class TransportSceneManager : public QObject
  {
    Q_OBJECT

    /// \brief Constructor
    public: TransportSceneManager();

    /// \brief Destructor
    public: virtual ~TransportSceneManager();

  public: TransportSceneManager(const std::string& scene_topic,
                                const std::string& pose_topic,
                                const std::string& deletion_topic,
                                const std::string& service);

//    // Documentation inherited
//    public: virtual void LoadConfig(const tinyxml2::XMLElement *_pluginElem)
//        override;

    // Documentation inherited
    private: bool eventFilter(QObject *_obj, QEvent *_event) override;

    /// \internal
    /// \brief Pointer to private data.
    private: std::unique_ptr<TransportSceneManagerPrivate> dataPtr;
  };
}

#endif // TESSERACT_GUI_TRANSPORT_SCENE_MANAGER_H
