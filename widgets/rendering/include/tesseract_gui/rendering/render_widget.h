#ifndef TESSERACT_GUI_RENDER_WIDGET_H
#define TESSERACT_GUI_RENDER_WIDGET_H

#include <ignition/common/KeyEvent.hh>
#include <ignition/common/MouseEvent.hh>
#include <ignition/math/Color.hh>
#include <ignition/math/Pose3.hh>
#include <ignition/math/Vector2.hh>
#include <ignition/utils/ImplPtr.hh>

#include <QObject>
#include <QOffscreenSurface>
#include <QOpenGLContext>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QMouseEvent>
//#include <QSGNode>
//#include <QSGSimpleTextureNode>
//#include <QSGTexture>
#include <QMutex>
#include <QScopedPointer>
#include <QThread>

namespace tesseract_gui
{

class TesseractRenderSync;

/// \brief Ign-rendering renderer.
/// All ign-rendering calls should be performed inside this class as it makes
/// sure that opengl calls in the underlying render engine do not interfere
/// with QtQuick's opengl render operations. The main Render function will
/// render to an offscreen texture and notify via signal and slots when it's
/// ready to be displayed.
class TesseractRenderer
{
public:
  ///  \brief Constructor
  TesseractRenderer();

  /// \param[in] _renderSync RenderSync to safely
  /// synchronize Qt and worker thread (this)
  void Render(TesseractRenderSync *_renderSync);

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
  IGN_UTILS_UNIQUE_IMPL_PTR(dataPtr)
};

/// \brief Rendering thread
class RenderThread : public QThread
{
  Q_OBJECT
public:
  /// \brief Constructor
  RenderThread();

  /// \brief Offscreen surface to render to
  QOffscreenSurface *surface = nullptr;

  /// \brief OpenGL context to be passed to the render engine
  QOpenGLContext *context = nullptr;

  /// \brief Ign-rendering renderer
  TesseractRenderer renderer;

  /// \brief Render when safe
  /// \param[in] _renderSync RenderSync to safely
  /// synchronize Qt and worker thread (this)
  public slots: void RenderNext(TesseractRenderSync *_renderSync);

  /// \brief Shutdown the thread and the render engine
  public slots: void ShutDown();

  /// \brief Slot called to update render texture size
  public slots: void SizeChanged();

  /// \brief Signal to indicate that a frame has been rendered and ready
  /// to be displayed
  /// \param[in] _id GLuid of the opengl texture
  /// \param[in] _size Size of the texture

signals:
    void TextureReady(uint _id, const QSize &_size);
    void ContextWanted();


};

class RenderWidget;

/// \brief Texture node for displaying the render texture from ign-renderer
class TextureNode : public QObject
{
  Q_OBJECT

  /// \brief Constructor
  /// \param[in] _window Window to display the texture
  /// \param[in] _renderSync RenderSync to safely
  /// synchronize Qt (this) and worker thread
  public: explicit TextureNode(RenderWidget *_window, TesseractRenderSync &_renderSync);

  /// \brief Destructor
  public: ~TextureNode() override;

  /// \brief This function gets called on the FBO rendering thread and will
  ///  store the texture id and size and schedule an update on the window.
  /// \param[in] _id OpenGL render texture Id
  /// \param[in] _size Texture size
  public slots: void NewTexture(uint _id, const QSize &_size);

  /// \brief Before the scene graph starts to render, we update to the
  /// pending texture
  public slots: void PrepareNode();

  /// \param[in] _renderSync RenderSync to send to the worker thread
  signals: void TextureInUse(TesseractRenderSync *_renderSync);

  /// \brief Signal emitted when a new texture is ready to trigger window
  /// update
  signals: void PendingNewTexture();

  /// \brief OpenGL texture id
  public: uint id = 0;

  /// \brief Texture size
  public: QSize size = QSize(0, 0);

  /// \brief Mutex to protect the texture variables
  public: QMutex mutex;

  /// \brief See RenderSync
  public: TesseractRenderSync &renderSync;

  /// \brief Qt's scene graph texture
  public: QOpenGLTexture *texture = nullptr;

  /// \brief Qt quick window
  public: RenderWidget *window = nullptr;
};

/// \brief A QWidget that manages the render window
class RenderWidget : public QOpenGLWidget
{
  Q_OBJECT
public:
  /// \brief Constructor
  /// \param[in] _parent Parent item
  explicit RenderWidget(QWidget* _parent = nullptr);

  ~RenderWidget();

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

  /// \brief Slot called when thread is ready to be started
  public Q_SLOTS: void Ready();

  /// \brief Handle key press event for snapping
  /// \param[in] _e The key event to process.
  void HandleKeyPress(const ignition::common::KeyEvent &_e);

  /// \brief Handle key release event for snapping
  /// \param[in] _e The key event to process.
  void HandleKeyRelease(const ignition::common::KeyEvent &_e);

signals:
    void renderRequested();

public slots:
    void grabContext();

private slots:
    void onAboutToCompose();
    void onFrameSwapped();
    void onAboutToResize();
    void onResized();
    void onTextureReady();

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

  /// \brief Overrides the paint event to render the render engine
  /// camera view
  /// \param[in] _oldNode The node passed in previous updatePaintNode
  /// function. It represents the visual representation of the item.
  /// \param[in] _data The node transformation data.
  /// \return Updated node.
//  QSGNode *updatePaintNode(QSGNode *_oldNode, QQuickItem::UpdatePaintNodeData *_data) override;

//  void paintGL() override;

  void paintEvent(QPaintEvent *) override { }

  /// \internal
  /// \brief Pointer to private data.
  IGN_UTILS_UNIQUE_IMPL_PTR(dataPtr)
};

}

#endif // TESSERACT_GUI_RENDER_WIDGET_H
