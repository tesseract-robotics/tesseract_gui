#ifndef MINIMAL_EXAMPLE_H
#define MINIMAL_EXAMPLE_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
#include <QOffscreenSurface>
#include <QtDebug>
#include <QOpenGLFunctions_4_5_Core>
#include <QPainter>

#include <ignition/rendering/Camera.hh>
#include <ignition/rendering/RayQuery.hh>
#include <ignition/rendering/RenderEngine.hh>
#include <ignition/rendering/RenderingIface.hh>
#include <ignition/rendering/Scene.hh>
#include <ignition/rendering/Grid.hh>
#include <ignition/common/Console.hh>
#include <ignition/common/Image.hh>

inline GLenum glCheckError_(const char *file, int line)
{
    GLenum errorCode;
    while ((errorCode = QOpenGLContext::currentContext()->functions()->glGetError()) != GL_NO_ERROR)
    {
        const char *error;
        switch (errorCode)
        {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        qDebug() << error << " | " << file << " (" << line << ")";
    }
    return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)

inline void APIENTRY glDebugOutput(GLenum source,
                            GLenum type,
                            GLuint id,
                            GLenum severity,
                            GLsizei,
                            const GLchar *message,
                            const void *)
{
    // ignore non-significant error/warning codes
    // if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    qDebug() << "---------------";
    qDebug() << "Debug message (" << id << "):" <<  message;

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             qDebug() << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   qDebug() << "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: qDebug() << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     qDebug() << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     qDebug() << "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           qDebug() << "Source: Other"; break;
    }

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               qDebug() << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: qDebug() << "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  qDebug() << "Type: Undefined Behaviour"; break;
        case GL_DEBUG_TYPE_PORTABILITY:         qDebug() << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         qDebug() << "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              qDebug() << "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          qDebug() << "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           qDebug() << "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               qDebug() << "Type: Other"; break;
    }

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         qDebug() << "Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       qDebug() << "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          qDebug() << "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: qDebug() << "Severity: notification"; break;
    }
}

class IssueRenderWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
  Q_OBJECT
public:

  IssueRenderWidget()
  {
    connect(this, &QOpenGLWidget::aboutToCompose, this, &IssueRenderWidget::onAboutToCompose);

  }
  void initialize()
  {
    std::map<std::string, std::string> params;
    params["useCurrentGLContext"] = "1";
    params["winID"] = std::to_string(this->winId());
  //  params["winID"] = std::to_string(
  //    ignition::gui::App()->findChild<ignition::gui::MainWindow *>()->
  //      QuickWindow()-winId());
    auto* engine = ignition::rendering::engine(engine_name_, params);
    if (engine == nullptr)
    {
      ignerr << "Engine [" << this->engine_name_ << "] is not supported" << std::endl;
      return;
    }

    // Scene
    scene_ = engine->SceneByName(scene_name_);
    if (!scene_)
    {
      igndbg << "Create scene [" << scene_name_ << "]" << std::endl;
      scene_ = engine->CreateScene(scene_name_);
      scene_->SetAmbientLight(ambient_light_);
      scene_->SetBackgroundColor(background_color_);
    }

    scene_->SetSkyEnabled(true);

    ignition::rendering::VisualPtr visual = scene_->VisualByName("tesseract_grid");
    if (visual == nullptr)
    {
      ignition::rendering::VisualPtr root = scene_->RootVisual();

      // create gray material
      ignition::rendering::MaterialPtr gray = scene_->CreateMaterial();
      gray->SetAmbient(0.7, 0.7, 0.7);
      gray->SetDiffuse(0.7, 0.7, 0.7);
      gray->SetSpecular(0.7, 0.7, 0.7);

      // create grid visual
      unsigned id = 1000; //static_cast<unsigned>(this->dataPtr->entity_manager.addVisual("tesseract_grid"));
      ignition::rendering::VisualPtr visual = scene_->CreateVisual(id, "tesseract_grid");
      ignition::rendering::GridPtr gridGeom = scene_->CreateGrid();
      if (!gridGeom)
      {
        ignwarn << "Failed to create grid for scene ["
          << scene_->Name() << "] on engine ["
            << scene_->Engine()->Name() << "]"
              << std::endl;
        return;
      }
      gridGeom->SetCellCount(20);
      gridGeom->SetCellLength(1);
      gridGeom->SetVerticalCellCount(0);
      visual->AddGeometry(gridGeom);
      visual->SetLocalPosition(0, 0, 0.015);
      visual->SetMaterial(gray);
      root->AddChild(visual);
    }
    else
    {
      visual->SetVisible(true);
    }

    auto root = scene_->RootVisual();

    // Camera
    camera_ = scene_->CreateCamera();
    camera_->SetUserData("user-camera", true);
    root->AddChild(camera_);
    camera_->SetLocalPose(camera_pose_);
    camera_->SetNearClipPlane(camera_near_clip_);
    camera_->SetFarClipPlane(camera_far_clip_);
    camera_->SetImageWidth(texture_size_.width());
    camera_->SetImageHeight(texture_size_.height());
    camera_->SetAntiAliasing(8);
    camera_->SetHFOV(M_PI * 0.5);

    /** @todo LEVI Need figure out haw to get the camera by name. */
    // create directional light
    ignition::rendering::DirectionalLightPtr light0 = scene_->CreateDirectionalLight();
    light0->SetDirection(1, 0, 0);
    light0->SetDiffuseColor(0.8, 0.8, 0.8);
    light0->SetSpecularColor(0.5, 0.5, 0.5);
    camera_->AddChild(light0);

    initialized_ = true;

  }


  void resizeGL(int w, int h)
  {
    glViewport( 0, 0, w, h );
    // Resizing camera causes issues, but not sure why
    texture_size_ = QSize(w, h);
    texture_dirty_ = true;

    ignerr << "Resized" << std::endl;
  }

  void initializeGL()
  {
    ignerr << "initializeGL" << std::endl;
    initializeOpenGLFunctions();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    auto* f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_5_Core>();
    f->glDebugMessageCallback(glDebugOutput, nullptr);

    // Initialize renderer
    if (!initialized_)
      initialize();

    // check if engine has been successfully initialized
    if (!initialized_)
    {
      ignerr << "Unable to initialize renderer" << std::endl;
    }
  }

  void paintGL()
  {
    ignerr << "paintGL" << std::endl;
    if (!initialized_)
      return;

    if (texture_dirty_)
    {
      camera_->SetImageWidth(texture_size_.width());
      camera_->SetImageHeight(texture_size_.height());
      camera_->SetAspectRatio(static_cast<double>(texture_size_.width()) / static_cast<double>(texture_size_.height()));
      // setting the size should cause the render texture to be rebuilt
      camera_->PreRender();
      texture_dirty_ = false;
    }

    GLuint texture_id = camera_->RenderTextureGLId();
    image_ = camera_->CreateImage();
    camera_->Capture(image_);
//    {
//      ignition::common::Image tmp;
//      tmp.SetFromData(data, image.Width(), image.Height(), ignition::common::Image::RGB_INT8);
//      tmp.SavePNG("/tmp/ign_image.png");
//    }

//    ignition::rendering::Image image(image_);
    auto* data = image_.Data<unsigned char>();
    makeCurrent();
//    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    auto imgw = image_.Width();
//    auto imgh = image_.Height();
//    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    glEnable(GL_TEXTURE_RECTANGLE);
//    glViewport (0, 0, (GLsizei) imgw, (GLsizei) imgh);
//    glGenTextures (1, &texture_);
//    glBindTexture (GL_TEXTURE_RECTANGLE, texture_);
//    glTexImage2D (GL_TEXTURE_RECTANGLE, 0, GL_RGB, (GLint)imgw, (GLint)imgh, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
//    glBegin (GL_QUADS);
//    // Bottom left
//    glTexCoord2d (imgw, imgh);
//    glVertex2f(-1, -1);

//    // Top left
//    glTexCoord2d (0, imgh);
//    glVertex2f (1, -1);

//    // Top right
//    glTexCoord2d (0, 0);
//    glVertex2f (1, 1);

//    // Bottom right
//    glTexCoord2d (imgw, 0);
//    glVertex2f (-1, 1);
//    glEnd();
//    glDisable(GL_TEXTURE_RECTANGLE);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
    glDrawPixels(image_.Width(), image_.Height(), GL_RGB, GL_UNSIGNED_BYTE, data);
    doneCurrent();
  }
private slots:
    void onAboutToCompose()
    {
//      ignerr << "onAboutToCompose" << std::endl;
//      makeCurrent();
//      if (texture_dirty_)
//      {
//        camera_->SetImageWidth(texture_size_.width());
//        camera_->SetImageHeight(texture_size_.height());
//        camera_->SetAspectRatio(static_cast<double>(texture_size_.width()) / static_cast<double>(texture_size_.height()));
//        // setting the size should cause the render texture to be rebuilt
//        camera_->PreRender();
//        texture_dirty_ = false;
//      }

//      image_ = camera_->CreateImage();
//      camera_->Capture(image_);
//      doneCurrent();
    }

//  void paintEvent(QPaintEvent*)
//  {
//    if (texture_dirty_)
//    {
//      camera_->SetImageWidth(texture_size_.width());
//      camera_->SetImageHeight(texture_size_.height());
//      camera_->SetAspectRatio(static_cast<double>(texture_size_.width()) / static_cast<double>(texture_size_.height()));
//      // setting the size should cause the render texture to be rebuilt
//      camera_->PreRender();
//      texture_dirty_ = false;
//    }

//    ignition::rendering::Image image = camera_->CreateImage();
//    camera_->Capture(image);

//    ignition::rendering::PixelFormat f = image.Format();
//    ignerr << "Format: " << f << std::endl;
//    auto* data = image.Data<unsigned char>();

//    {
//      ignerr << "Image, w: " << image.Width() << " h: " << image.Height() << std::endl;
//      ignition::common::Image tmp;
//      tmp.SetFromData(data, image.Width(), image.Height(), ignition::common::Image::RGB_INT8);
//      tmp.SavePNG("/tmp/ign_image.png");
//    }

//    QPainter painter(this);
//    QImage  qimage("/tmp/ign_image.png");

//    painter.drawImage(this->rect(),qimage);
//  }

protected:
  bool initialized_{false};
  std::string engine_name_{"ogre"};
  std::string scene_name_{"IssueScene"};
  ignition::rendering::ScenePtr scene_{nullptr};
  ignition::rendering::CameraPtr camera_{nullptr};
  ignition::math::Color background_color_{ignition::math::Color::Black};
  ignition::math::Color ambient_light_{ignition::math::Color(0.3f, 0.3f, 0.3f, 1.0f)};
  ignition::math::Pose3d camera_pose_ {ignition::math::Pose3d(0, 0, 2, 0, 0.4, 0)};
  double camera_near_clip_ {0.01};
  double camera_far_clip_ {1000.0};
  QSize texture_size_ {QSize(1920, 1200)};
  bool texture_dirty_{true};
  GLuint texture_{0};
  ignition::rendering::Image image_;
};


#endif // MINIMAL_EXAMPLE_H
