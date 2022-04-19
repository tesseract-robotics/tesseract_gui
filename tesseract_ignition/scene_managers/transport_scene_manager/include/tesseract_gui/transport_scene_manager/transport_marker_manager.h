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

#ifndef TESSERACT_GUI_TRANSPORT_MARKER_MANAGER_H
#define TESSERACT_GUI_TRANSPORT_MARKER_MANAGER_H
#include <memory>
#include <QObject>
#include <QtEvents>

namespace tesseract_gui
{
class TransportMarkerManagerPrivate;

/// \brief This plugin will be in charge of handling the markers in the
/// scene. It will allow to add, modify or remove markers.
///
/// ## Parameters
///
/// * `<topic_name>`: Options. Name of topic for marker service. Defaults
/// to `/marker`.
/// * `<warn_on_action_failure>`: True to display warnings if the user
/// attempts to perform an invalid action. Defaults to true.
class TransportMarkerManager : public QObject
{
  Q_OBJECT
public:
  /// \brief Constructor
  TransportMarkerManager(const std::string& scene_name);

  /// \brief Destructor
  virtual ~TransportMarkerManager();

  //    // Documentation inherited
  //    virtual void LoadConfig(const tinyxml2::XMLElement *_pluginElem)
  //        override;
private:
  // Documentation inherited
  bool eventFilter(QObject* _obj, QEvent* _event) override;

  /// \internal
  /// \brief Pointer to private data.
  std::unique_ptr<TransportMarkerManagerPrivate> dataPtr;
};
}  // namespace tesseract_gui

#endif  // TESSERACT_GUI_TRANSPORT_MARKER_MANAGER_H
