#ifndef TESSERACT_GUI_SIMPLE_RENDER_WIDGET_H
#define TESSERACT_GUI_SIMPLE_RENDER_WIDGET_H

#include <ignition/common/KeyEvent.hh>
#include <ignition/common/MouseEvent.hh>
#include <ignition/math/Color.hh>
#include <ignition/math/Pose3.hh>
#include <ignition/math/Vector2.hh>
#include <ignition/utils/ImplPtr.hh>
#include <ignition/rendering/Image.hh>

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

namespace tesseract_gui
{

class SimpleRendererImpl;

/// \brief Ign-rendering renderer.
/// All ign-rendering calls should be performed inside this class as it makes
/// sure that opengl calls in the underlying render engine do not interfere
/// with QtQuick's opengl render operations. The main Render function will
/// render to an offscreen texture and notify via signal and slots when it's
/// ready to be displayed.
class SimpleRenderer
{
public:
  ///  \brief Constructor
  SimpleRenderer();

  /// \param[in] _renderSync RenderSync to safely
  /// synchronize Qt and worker thread (this)
  ignition::rendering::Image Render();

  void Resize(int width, int height);

  /// \brief Initialize the render engine
  void Initialize();

  /// \brief Destroy camera associated with this renderer
  void Destroy();

  /// \brief New mouse event triggered
  /// \param[in] _e New mouse event
  void NewMouseEvent(const ignition::common::MouseEvent &_e);

  /// \brief New hover event triggered.
  /// \param[in] _hoverPos Mouse hover screen position
  void NewHoverEvent(const ignition::math::Vector2i &_hoverPos);

  /// \brief New hover event triggered.
  /// \param[in] _dropText Text dropped on the scene
  /// \param[in] _dropPos Mouse drop screen position
  void NewDropEvent(const std::string &_dropText, const ignition::math::Vector2i &_dropPos);

  /// \brief Handle key press event for snapping
  /// \param[in] _e The key event to process.
  void HandleKeyPress(const ignition::common::KeyEvent &_e);

  /// \brief Handle key release event for snapping
  /// \param[in] _e The key event to process.
  void HandleKeyRelease(const ignition::common::KeyEvent &_e);

  /// Values is constantly constantly cycled/swapped/changed
  /// from a worker thread
  /// Don't read this directly
  GLuint textureId;

  /// \brief Render engine to use
  std::string engineName = "ogre";

  /// \brief Unique scene name
  std::string sceneName = "scene";

  /// \brief Initial Camera pose
  ignition::math::Pose3d cameraPose = ignition::math::Pose3d(0, 0, 2, 0, 0.4, 0);

  /// \brief Default camera near clipping plane distance
  double cameraNearClip = 0.01;

  /// \brief Default camera far clipping plane distance
  double cameraFarClip = 1000.0;

  /// \brief Scene background color
  ignition::math::Color backgroundColor = ignition::math::Color::Black;

  /// \brief Ambient color
  ignition::math::Color ambientLight = ignition::math::Color(0.3f, 0.3f, 0.3f, 1.0f);

  /// \brief True if engine has been initialized;
  bool initialized = false;

  /// \brief Render texture size
  QSize textureSize = QSize(1024, 1024);

  /// \brief Flag to indicate texture size has changed.
  bool textureDirty = false;

  /// \brief Scene service. If not empty, a request will be made to get the
  /// scene information using this service and the renderer will populate the
  /// scene based on the response data
  std::string sceneService;

  /// \brief Scene pose topic. If not empty, a node will subcribe to this
  /// topic to get pose updates of objects in the scene
  std::string poseTopic;

  /// \brief Ign-transport deletion topic name
  std::string deletionTopic;

  /// \brief Ign-transport scene topic name
  /// New scene messages will be published to this topic when an entities are
  /// added
  std::string sceneTopic;

  /// \brief True if sky is enabled;
  bool skyEnable = false;

  /// \brief True if grid is enabled;
  bool gridEnable = true;

private:
  /// \brief Handle mouse event for view control
  void HandleMouseEvent();

  /// \brief Broadcasts the currently hovered 3d scene location.
  void BroadcastHoverPos();

  /// \brief Broadcasts drag events.
  void BroadcastDrag();

  /// \brief Broadcasts a left click (release) within the scene
  void BroadcastLeftClick();

  /// \brief Broadcasts a right click (release) within the scene
  void BroadcastRightClick();

  /// \brief Broadcasts a mouse press within the scene
  void BroadcastMousePress();

  /// \brief Broadcasts a scroll event within the scene.
  void BroadcastScroll();

  /// \brief Broadcasts a key release event within the scene
  void BroadcastKeyRelease();

  /// \brief Broadcasts a drop event within the scene
  void BroadcastDrop();

  /// \brief Broadcasts a key press event within the scene
  void BroadcastKeyPress();

  /// \brief Retrieve the first point on a surface in the 3D scene hit by a
  /// ray cast from the given 2D screen coordinates.
  /// \param[in] _screenPos 2D coordinates on the screen, in pixels.
  /// \return 3D coordinates of a point in the 3D scene.
  ignition::math::Vector3d ScreenToScene(const ignition::math::Vector2i &_screenPos) const;

  /// \internal
  /// \brief Pointer to private data.
  std::unique_ptr<SimpleRendererImpl> dataPtr;
};

class SimpleRenderWidgetImpl;

class SimpleRenderWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
  Q_OBJECT
public:
  /// \brief Constructor
  /// \param[in] _parent Parent item
  explicit SimpleRenderWidget(QWidget* _parent = nullptr);

  ~SimpleRenderWidget();

  /// \brief Set background color of render window
  /// \param[in] _color Color of render window background
  void SetBackgroundColor(const ignition::math::Color &_color);

  /// \brief Set ambient light of render window
  /// \param[in] _ambient Color of ambient light
  void SetAmbientLight(const ignition::math::Color &_ambient);

  /// \brief Set engine name used to create the render window
  /// \param[in] _name Name of render engine
  void SetEngineName(const std::string &_name);

  /// \brief Set name of scene created inside the render window
  /// \param[in] _name Name of scene
  void SetSceneName(const std::string &_name);

  /// \brief Set the initial pose the render window camera
  /// \param[in] _pose Initial camera pose
  void SetCameraPose(const ignition::math::Pose3d &_pose);

  /// \brief Set the render window camera's near clipping plane distance
  /// \param[in] _near Near clipping plane distance
  void SetCameraNearClip(double _near);

  /// \brief Set the render window camera's far clipping plane distance
  /// \param[in] _far Far clipping plane distance
  void SetCameraFarClip(double _far);

  /// \brief Set scene service to use in this render window
  /// A service call will be made using ign-transport to get scene
  /// data using this service
  /// \param[in] _service Scene service name
  void SetSceneService(const std::string &_service);

  /// \brief Set pose topic to use for updating objects in the scene
  /// The renderer will subscribe to this topic to get pose messages of
  /// visuals in the scene
  /// \param[in] _topic Pose topic
  void SetPoseTopic(const std::string &_topic);

  /// \brief Set deletion topic to use for deleting objects from the scene
  /// The renderer will subscribe to this topic to get notified when entities
  /// in the scene get deleted
  /// \param[in] _topic Deletion topic
  void SetDeletionTopic(const std::string &_topic);

  /// \brief Set the scene topic to use for updating objects in the scene
  /// The renderer will subscribe to this topic to get updates scene messages
  /// \param[in] _topic Scene topic
  void SetSceneTopic(const std::string &_topic);

  /// \brief Called when the mouse hovers to a new position.
  /// \param[in] _hoverPos 2D coordinates of the hovered mouse position on
  /// the render window.
  void OnHovered(int _mouseX, int _mouseY);
//    public: void OnHovered(const ignition::math::Vector2i &_hoverPos);

  /// \brief Callback when receives a drop event.
  /// \param[in] _drop Dropped string.
  /// \param[in] _dropPos x coordinate of mouse position.
  void OnDropped(const QString &_drop, int _mouseX, int _mouseY);

//    public: void OnDropped(const QString &_drop,
//        const ignition::math::Vector2i &_dropPos);

  /// \brief Set if sky is enabled
  /// \param[in] _sky True to enable the sky, false otherwise.
  void SetSkyEnabled(const bool &_sky);

  /// \brief Show grid view in the scene
  void SetGridEnabled(bool _grid);

  /// \brief Handle key press event for snapping
  /// \param[in] _e The key event to process.
  void HandleKeyPress(const ignition::common::KeyEvent &_e);

  /// \brief Handle key release event for snapping
  /// \param[in] _e The key event to process.
  void HandleKeyRelease(const ignition::common::KeyEvent &_e);

protected:
  // Documentation inherited
  void mousePressEvent(QMouseEvent *_e) override;

  // Documentation inherited
  void mouseReleaseEvent(QMouseEvent *_e) override;

  // Documentation inherited
  void mouseMoveEvent(QMouseEvent *_e) override;

  // Documentation inherited
  void keyPressEvent(QKeyEvent *_e) override;

  // Documentation inherited
  void keyReleaseEvent(QKeyEvent *_e) override;

  // Documentation inherited
  void wheelEvent(QWheelEvent *_e) override;

  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;
  bool event(QEvent *e) override;


  /// \internal
  /// \brief Pointer to private data.
  std::unique_ptr<SimpleRenderWidgetImpl> dataPtr;
};
}
#endif // TESSERACT_GUI_SIMPLE_RENDER_WIDGET_H
