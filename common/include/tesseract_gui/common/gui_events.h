/*
 * Copyright (C) 2020 Open Source Robotics Foundation
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
#ifndef TESSERACT_GUI_COMMON_GUI_EVENTS_H
#define TESSERACT_GUI_COMMON_GUI_EVENTS_H
#include <QEvent>
#include <QMap>
#include <QString>

#include <set>
#include <string>
#include <utility>
#include <vector>
#include <memory>
#include <tesseract_gui/common/entity.h>

#include <ignition/common/KeyEvent.hh>
#include <ignition/common/MouseEvent.hh>
#include <ignition/math/Vector2.hh>
#include <ignition/math/Vector3.hh>
//#include <ignition/msgs/world_control.pb.h>

namespace tesseract_gui
{
/// \brief Namespace for all events. Refer to the EventManager class for
/// more information about events.
namespace events
{
  /// User defined events should start from QEvent::MaxUser and
  /// count down to avoid collision with ign-gazebo events

  /// \brief Event called in the render thread of a 3D scene after the user
  /// camera has rendered.
  /// It's safe to make rendering calls in this event's callback.
  class Render : public QEvent
  {
  public:
    Render(const std::string& scene_key);
    ~Render() override;

    /// \brief Unique type for this event.
    static const QEvent::Type kType = QEvent::Type(QEvent::MaxUser);
  private:

    /// \internal
    /// \brief Private data pointer
    class Implementation;
    std::unique_ptr<Implementation> dataPtr;
  };

  /// \brief The class for sending and receiving custom snap value events.
  /// This event is used in the Transform Control plugin tool when the
  /// user manually alters their snapping values.
  class SnapIntervals : public QEvent
  {
  public:
    /// \brief Constructor
    /// \param[in] _xyz XYZ snapping values.
    /// \param[in] _rpy RPY snapping values.
    /// \param[in] _scale Scale snapping values.
    SnapIntervals(const ignition::math::Vector3d &_xyz,
                  const ignition::math::Vector3d &_rpy,
                  const ignition::math::Vector3d &_scale,
                  const std::string& scene_key);

    ~SnapIntervals() override;

    /// \brief Get the XYZ snapping values.
    /// \return The XYZ snapping values.
    ignition::math::Vector3d Position() const;

    /// \brief Get the RPY snapping values.
    /// \return The RPY snapping values.
    ignition::math::Vector3d Rotation() const;

    /// \brief Get the scale snapping values.
    /// \return The scale snapping values.
    ignition::math::Vector3d Scale() const;

    /// \brief The QEvent representing a snap event occurrence.
    static const QEvent::Type kType = QEvent::Type(QEvent::MaxUser - 1);
  private:

    /// \internal
    /// \brief Private data pointer
    class Implementation;
    std::unique_ptr<Implementation> dataPtr;
  };

  /// \brief Event called to spawn a resource, given its description as a
  /// string.
  class SpawnFromDescription : public QEvent
  {
  public:
    /// \brief Constructor
    /// \param[in] _description The resource's description as a string, such
    /// as an SDF file.
    explicit SpawnFromDescription(const std::string &_description, const std::string& scene_key);
    ~SpawnFromDescription() override;

    /// \brief Get the string description of the resource.
    /// \return The resource string
    ///
    const std::string &Description() const;

    /// \brief Unique type for this event.
    static const QEvent::Type kType = QEvent::Type(QEvent::MaxUser - 2);
  private:

    /// \internal
    /// \brief Private data pointer
    class Implementation;
    std::unique_ptr<Implementation> dataPtr;
  };

  /// \brief Event called to spawn a resource, which takes the path
  /// to its file.
  class SpawnFromPath : public QEvent
  {
  public:
    /// \brief Constructor
    /// \param[in] _filePath The path to a file.
    explicit SpawnFromPath(const std::string &_filePath, const std::string& scene_key);
    ~SpawnFromPath() override;

    /// \brief Get the path of the file.
    /// \return The file path.
    const std::string &FilePath() const;

    /// \brief Unique type for this event.
    static const QEvent::Type kType = QEvent::Type(QEvent::MaxUser - 3);
  private:

    /// \internal
    /// \brief Private data pointer
    class Implementation;
    std::unique_ptr<Implementation> dataPtr;
  };

  /// \brief Event which is called to broadcast the 3D coordinates of a
  /// user's mouse hover within the scene.
  class HoverToScene : public QEvent
  {
  public:
    /// \brief Constructor
    /// \param[in] _point The point at which the mouse is hovering within
    /// the scene
    explicit HoverToScene(const ignition::math::Vector3d &_point, const std::string& scene_key);
    ~HoverToScene() override;

    /// \brief Get the point within the scene over which the user is
    /// hovering.
    /// \return The 3D point
    ignition::math::Vector3d Point() const;

    /// \brief Unique type for this event.
    static const QEvent::Type kType = QEvent::Type(QEvent::MaxUser - 4);
  private:

    /// \internal
    /// \brief Private data pointer
    class Implementation;
    std::unique_ptr<Implementation> dataPtr;
  };

  /// \brief Event which is called to broadcast the 3D coordinates of a
  /// user's releasing the left button within the scene.
  /// \sa LeftClickOnScene
  class LeftClickToScene : public QEvent
  {
  public:
    /// \brief Constructor
    /// \param[in] _point The point which the user has left clicked within
    /// the scene
    explicit LeftClickToScene(const ignition::math::Vector3d &_point, const std::string& scene_key);
    ~LeftClickToScene() override;

    /// \brief Get the point within the scene that the user clicked.
    /// \return The 3D point.
    ignition::math::Vector3d Point() const;

    /// \brief Unique type for this event.
    static const QEvent::Type kType = QEvent::Type(QEvent::MaxUser - 5);
  private:
    /// \internal
    /// \brief Private data pointer
    class Implementation;
    std::unique_ptr<Implementation> dataPtr;
  };

  /// \brief Event which is called to broadcast the 3D coordinates of a
  /// user's releasing the right button within the scene.
  /// \sa RightClickOnScene
  class RightClickToScene : public QEvent
  {
  public:
    /// \brief Constructor
    /// \param[in] _point The point which the user has right clicked
    /// within the scene
    explicit RightClickToScene(const ignition::math::Vector3d &_point, const std::string& scene_key);
    ~RightClickToScene() override;

    /// \brief Get the point within the scene that the user clicked.
    /// \return The 3D point.
    ignition::math::Vector3d Point() const;

    /// \brief Unique type for this event.
    static const QEvent::Type kType = QEvent::Type(QEvent::MaxUser - 6);
  private:

    /// \internal
    /// \brief Private data pointer
    class Implementation;
    std::unique_ptr<Implementation> dataPtr;
  };

  /// \brief Event which is called to enable or disable the dropdown menu.
  /// This is primarily used by plugins which also use the right click
  /// mouse event to cancel any actions currently in progress.
  class DropdownMenuEnabled : public QEvent
  {
  public:
    /// \brief Constructor
    /// \param[in] _menuEnabled The boolean indicating whether the dropdown
    /// menu should be enabled or disabled.
    explicit DropdownMenuEnabled(bool _menuEnabled, const std::string& scene_key);
    ~DropdownMenuEnabled() override;

    /// \brief Gets whether the menu is enabled or not for this event.
    /// \return True if enabling the menu, false if disabling the menu
    bool MenuEnabled() const;

    /// \brief Unique type for this event.
    static const QEvent::Type kType = QEvent::Type(QEvent::MaxUser - 7);
  private:

    /// \internal
    /// \brief Private data pointer
    class Implementation;
    std::unique_ptr<Implementation> dataPtr;
  };

  /// \brief Event which is called to broadcast the key release within
  /// the scene.
  class KeyReleaseOnScene : public QEvent
  {
  public:
    /// \brief Constructor
    /// \param[in] _key The key released event within the scene
    explicit KeyReleaseOnScene(const ignition::common::KeyEvent &_key, const std::string &scene_key);
    ~KeyReleaseOnScene() override;

    /// \brief Get the released key within the scene that the user released.
    /// \return The key code.
    public: ignition::common::KeyEvent Key() const;

    /// \brief Unique type for this event.
    static const QEvent::Type kType = QEvent::Type(QEvent::MaxUser - 8);
  private:

    /// \internal
    /// \brief Private data pointer
    class Implementation;
    std::unique_ptr<Implementation> dataPtr;
  };

  /// \brief Event which is called to broadcast the key press within
  /// the scene.
  class KeyPressOnScene : public QEvent
  {
  public:
    /// \brief Constructor
    /// \param[in] _key The pressed key within the scene
    explicit KeyPressOnScene(const ignition::common::KeyEvent &_key, const std::string &scene_key);
    ~KeyPressOnScene() override;

    /// \brief Get the key within the scene that the user pressed
    /// \return The key code.
    ignition::common::KeyEvent Key() const;

    /// \brief Unique type for this event.
    static const QEvent::Type kType = QEvent::Type(QEvent::MaxUser - 9);
  private:

    /// \internal
    /// \brief Private data pointer
    class Implementation;
    std::unique_ptr<Implementation> dataPtr;
  };

  /// \brief Event which is called to broadcast information about left
  /// mouse releases on the scene.
  /// For the 3D coordinates of that point on the scene, see
  /// `LeftClickToScene`.
  /// \sa LeftClickToScene
  class LeftClickOnScene : public QEvent
  {
  public:
    /// \brief Constructor
    /// \param[in] _mouse The left mouse event on the scene
    explicit LeftClickOnScene(const ignition::common::MouseEvent &_mouse, const std::string &scene_key);
    ~LeftClickOnScene() override;

    /// \brief Return the left mouse event
    const ignition::common::MouseEvent &Mouse() const;

    /// \brief Unique type for this event.
    static const QEvent::Type kType = QEvent::Type(QEvent::MaxUser - 10);
  private:

    /// \internal
    /// \brief Private data pointer
    class Implementation;
    std::unique_ptr<Implementation> dataPtr;
  };

  /// \brief Event which is called to broadcast information about right
  /// mouse releases on the scene.
  /// For the 3D coordinates of that point on the scene, see
  /// `RightClickToScene`.
  /// \sa RightClickToScene
  class RightClickOnScene : public QEvent
  {
  public:
    /// \brief Constructor
    /// \param[in] _mouse The right mouse event on the scene
    RightClickOnScene(const ignition::common::MouseEvent &_mouse, const std::string &scene_key);
    ~RightClickOnScene() override;

    /// \brief Return the right mouse event
    public: const ignition::common::MouseEvent &Mouse() const;

    /// \brief Unique type for this event.
    static const QEvent::Type kType = QEvent::Type(QEvent::MaxUser - 11);
  private:

    /// \internal
    /// \brief Private data pointer
    class Implementation;
    std::unique_ptr<Implementation> dataPtr;
  };

  /// \brief Event that block the Interactive View control when some of the
  /// other plugins require it. For example: When the transform control is
  /// active we should block the movements of the camera.
  class BlockOrbit : public QEvent
  {
  public:
    /// \brief Constructor
    /// \param[in] _block True to block otherwise False
    explicit BlockOrbit(const bool &_block, const std::string &scene_key);
    ~BlockOrbit() override;

    /// \brief Get the if the event should block the Interactive view
    /// controller
    /// \return True to block otherwise False.
    bool Block() const;

    /// \brief Unique type for this event.
    static const QEvent::Type kType = QEvent::Type(QEvent::MaxUser - 12);
  private:

    /// \internal
    /// \brief Private data pointer
    class Implementation;
    std::unique_ptr<Implementation> dataPtr;
  };

  /// \brief Event which is called to broadcast the 2D coordinates of a
  /// user's mouse hover within the scene.
  class HoverOnScene : public QEvent
  {
  public:
    /// \brief Constructor
    /// \param[in] _mouse The hover mouse event on the scene
    explicit HoverOnScene(const ignition::common::MouseEvent &_mouse, const std::string &scene_key);
    ~HoverOnScene() override;

    /// \brief Get the point within the scene over which the user is
    /// hovering.
    /// \return The 2D point
    ignition::common::MouseEvent Mouse() const;

    /// \brief Unique type for this event.
    static const QEvent::Type kType = QEvent::Type(QEvent::MaxUser - 13);
  private:

    /// \internal
    /// \brief Private data pointer
    class Implementation;
    std::unique_ptr<Implementation> dataPtr;
  };

  /// \brief Event called to clone a resource, given its name as a string.
  class SpawnCloneFromName : public QEvent
  {
  public:
    /// \brief Constructor
    /// \param[in] _name The name of the resource to clone
    explicit SpawnCloneFromName(const std::string &_name, const std::string &scene_key);
    ~SpawnCloneFromName() override;

    /// \brief Get the name of the resource to be cloned
    /// \return The name of the resource to be cloned
    const std::string &Name() const;

    /// \brief Unique type for this event.
    static const QEvent::Type kType = QEvent::Type(QEvent::MaxUser - 14);
  private:

    /// \internal
    /// \brief Private data pointer
    class Implementation;
    std::unique_ptr<Implementation> dataPtr;
  };

  /// \brief Event called to clone a resource, given its name as a string.
  class DropOnScene : public QEvent
  {
  public:
    /// \brief Constructor
    /// \param[in] _dropText Dropped string.
    /// \param[in] _dropMouse x and y  coordinate of mouse position.
    explicit DropOnScene(const std::string &_dropText, const ignition::math::Vector2i &_dropMouse, const std::string &scene_key);
    ~DropOnScene() override;

    /// \brief Get the text of the dropped thing on the scene
    /// \return The name of the dropped thing on the scene
    const std::string &DropText() const;

    /// \brief Get X and Y position
    /// \return Get X and Y position
    const ignition::math::Vector2i &Mouse() const;

    /// \brief Unique type for this event.
    static const QEvent::Type kType = QEvent::Type(QEvent::MaxUser - 15);
  private:

    /// \internal
    /// \brief Private data pointer
    class Implementation;
    std::unique_ptr<Implementation> dataPtr;
  };

  /// \brief Event which is called to broadcast information about mouse
  /// scrolls on the scene.
  class ScrollOnScene : public QEvent
  {
  public:
    /// \brief Constructor
    /// \param[in] _mouse The scroll mouse event on the scene
    explicit ScrollOnScene(const ignition::common::MouseEvent &_mouse, const std::string &scene_key);
    ~ScrollOnScene() override;

    /// \brief Return the scroll mouse event
    const ignition::common::MouseEvent &Mouse() const;

    /// \brief Unique type for this event.
    static const QEvent::Type kType = QEvent::Type(QEvent::MaxUser - 16);
  private:

    /// \internal
    /// \brief Private data pointer
    class Implementation;
    std::unique_ptr<Implementation> dataPtr;
  };

  /// \brief Event which is called to broadcast information about mouse
  /// drags on the scene.
  class DragOnScene : public QEvent
  {
  public:
    /// \brief Constructor
    /// \param[in] _mouse The drag mouse event on the scene
    explicit DragOnScene(const ignition::common::MouseEvent &_mouse, const std::string &scene_key);
    ~DragOnScene() override;

    /// \brief Get the point within the scene over which the user is
    /// dragging.
    /// \return The 2D point
    ignition::common::MouseEvent Mouse() const;

    /// \brief Unique type for this event.
    static const QEvent::Type kType = QEvent::Type(QEvent::MaxUser - 17);
  public:

    /// \internal
    /// \brief Private data pointer
    class Implementation;
    std::unique_ptr<Implementation> dataPtr;
  };

  /// \brief Event which is called to broadcast information about mouse
  /// presses on the scene, with right, left or middle buttons.
  class MousePressOnScene : public QEvent
  {
  public:
    /// \brief Constructor
    /// \param[in] _mouse The mouse event on the scene
    MousePressOnScene(const ignition::common::MouseEvent &_mouse, const std::string &scene_key);
    ~MousePressOnScene() override;

    /// \brief Return the button press mouse event
    const ignition::common::MouseEvent &Mouse() const;

    /// \brief Unique type for this event.
    static const QEvent::Type kType = QEvent::Type(QEvent::MaxUser - 18);
  private:
    /// \internal
    /// \brief Private data pointer
    class Implementation;
    std::unique_ptr<Implementation> dataPtr;
  };

//  /// \brief Event which is called to share WorldControl information.
//  class WorldControl : public QEvent
//  {
//  public:
//    /// \brief Constructor
//    /// \param[in] _worldControl The WorldControl information
//    explicit WorldControl(const ignition::msgs::WorldControl &_worldControl);

//    /// \brief Get the WorldControl information
//    /// \return The WorldControl information
//    const ignition::msgs::WorldControl &WorldControlInfo() const;

//    /// \brief Unique type for this event.
//    static const QEvent::Type kType = QEvent::Type(QEvent::MaxUser - 19);
//  private:

//    /// \internal
//    /// \brief Private data pointer
//    class Implementation;
//    std::unique_ptr<Implementation> dataPtr;
//  };

  /// \brief Event called in the render thread of a 3D scene, before the
  /// user camera is rendered.
  /// It's safe to make rendering calls in this event's callback.
  class PreRender : public QEvent
  {
  public:
    /// \brief Constructor
    PreRender(const std::string &scene_key);
    ~PreRender() override;

    /// \brief Unique type for this event.
    static const QEvent::Type kType = QEvent::Type(QEvent::MaxUser - 20);
  private:

    /// \internal
    /// \brief Private data pointer
    class Implementation;
    std::unique_ptr<Implementation> dataPtr;
  };

//  /// \brief Event that notifies when new entities have been selected.
//  class EntitiesSelected : public QEvent
//  {
//    /// \brief Constructor
//    /// \param[in] _entities All the selected entities
//    /// \param[in] _fromUser True if the event was directly generated by the
//    /// user, false in case it's been propagated through a different mechanism.
//    public: explicit EntitiesSelected(
//        const std::vector<Entity> &_entities,  // NOLINT
//        bool _fromUser = false)
//        : QEvent(kType), entities(_entities), fromUser(_fromUser)
//    {
//    }

//    /// \brief Get the data sent with the event.
//    /// \return The entities being selected.
//    public: std::vector<Entity> Data() const
//    {
//      return this->entities;
//    }

//    /// \brief Get whether the event was generated by the user.
//    /// \return True for the user.
//    public: bool FromUser() const
//    {
//      return this->fromUser;
//    }

//    /// \brief Unique type for this event.
//    static const QEvent::Type kType = QEvent::Type(QEvent::User + 1);

//    /// \brief The selected entities.
//    private: std::vector<Entity> entities;

//    /// \brief Whether the event was generated by the user,
//    private: bool fromUser{false};
//  };

//  /// \brief Event that notifies when all entities have been deselected.
//  class DeselectAllEntities : public QEvent
//  {
//    /// \brief Constructor
//    /// \param[in] _fromUser True if the event was directly generated by the
//    /// user, false in case it's been propagated through a different mechanism.
//    public: explicit DeselectAllEntities(bool _fromUser = false)
//        : QEvent(kType), fromUser(_fromUser)
//    {
//    }

//    /// \brief Get whether the event was generated by the user.
//    /// \return True for the user.
//    public: bool FromUser() const
//    {
//      return this->fromUser;
//    }

//    /// \brief Unique type for this event.
//    static const QEvent::Type kType = QEvent::Type(QEvent::User + 2);

//    /// \brief Whether the event was generated by the user,
//    private: bool fromUser{false};
//  };

//  /// \brief Event that contains entities newly created or removed from the
//  /// GUI, but that aren't present on the server yet.
//  /// \sa NewRemovedEntities
//  class GuiNewRemovedEntities : public QEvent
//  {
//    /// \brief Constructor
//    /// \param[in] _newEntities Set of newly created entities
//    /// \param[in] _removedEntities Set of recently removed entities
//    public: GuiNewRemovedEntities(const std::set<Entity> &_newEntities,
//                const std::set<Entity> &_removedEntities);

//    /// \brief Get the set of newly created entities
//    public: const std::set<Entity> &NewEntities() const;

//    /// \brief Get the set of recently removed entities
//    public: const std::set<Entity> &RemovedEntities() const;

//    /// \brief Unique type for this event.
//    static const QEvent::Type kType = QEvent::Type(QEvent::User + 3);

//    /// \internal
//    /// \brief Private data pointer
//    class Implementation;
//    std::unique_ptr<Implementation> dataPtr;
//  };

//  /// \brief Event that notifies when new entities have been created or removed
//  /// on the server. This is a duplication of what `GuiSystem`s would get from
//  /// `EachNew` / `EachRemoved` ECM calls.
//  /// \sa GuiNewRemovedEntities
//  class NewRemovedEntities : public QEvent
//  {
//    /// \brief Constructor
//    /// \param[in] _newEntities Set of newly created entities
//    /// \param[in] _removedEntities Set of recently removed entities
//    public: NewRemovedEntities(const std::set<Entity> &_newEntities,
//                const std::set<Entity> &_removedEntities);

//    /// \brief Get the set of newly created entities
//    public: const std::set<Entity> &NewEntities() const;

//    /// \brief Get the set of recently removed entities
//    public: const std::set<Entity> &RemovedEntities() const;

//    /// \brief Unique type for this event.
//    static const QEvent::Type kType = QEvent::Type(QEvent::User + 4);

//    /// \internal
//    /// \brief Private data pointer
//    class Implementation;
//    std::unique_ptr<Implementation> dataPtr;
//  };

//  /// \brief True if a transform control is currently active (translate /
//  /// rotate / scale). False if we're in selection mode.
//  class TransformControlModeActive : public QEvent
//  {
//    /// \brief Constructor
//    /// \param[in] _tranformModeActive is the transform control mode active
//    public: explicit TransformControlModeActive(const bool _tranformModeActive)
//        : QEvent(kType), tranformModeActive(_tranformModeActive)
//    {
//    }

//    /// \brief Unique type for this event.
//    static const QEvent::Type kType = QEvent::Type(QEvent::User + 6);

//    /// \brief Get the event's value.
//    public: bool TransformControlActive()
//    {
//      return this->tranformModeActive;
//    }

//    /// \brief True if a transform mode is active.
//    private: bool tranformModeActive;
//  };

//  /// \brief Event that notifies an entity is to be added to the model editor
//  class ModelEditorAddEntity : public QEvent
//  {
//    /// \brief Constructor
//    /// \param[in] _entity Entity added
//    /// \param[in] _type Entity type
//    /// \param[in] _parent Parent entity.
//    public: explicit ModelEditorAddEntity(QString _entity, QString _type, tesseract_gui::Entity _parent);

//    /// \brief Get the entity to add
//    public: QString Entity() const;

//    /// \brief Get the entity type
//    public: QString EntityType() const;


//    /// \brief Get the parent entity to add the entity to
//    public: tesseract_gui::Entity ParentEntity() const;

//    /// \brief Get the data map.
//    /// \return the QMap of string, string holding custom data.
//    public: QMap<QString, QString> &Data();

//    static const QEvent::Type kType = QEvent::Type(QEvent::User + 7);

//    /// \internal
//    /// \brief Private data pointer
//    class Implementation;
//    std::unique_ptr<Implementation> dataPtr;
//  };

}  // namespace events
}  // namespace tesseract_gui
#endif // TESSERACT_GUI_COMMON_GUI_EVENTS_H
