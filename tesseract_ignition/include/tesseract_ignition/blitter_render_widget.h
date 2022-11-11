#ifndef BLITTER_RENDER_WIDGET_H
#define BLITTER_RENDER_WIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLTextureBlitter>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QImage>

namespace tesseract_gui
{
class BlitterRenderWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
  Q_OBJECT
public:
  explicit BlitterRenderWidget(QWidget* _parent = nullptr) : QOpenGLWidget(_parent)
  {
    image_ = QImage("/home/larmstrong/Pictures/initial_image.png");
  }

protected:
  // Documentation inherited
  void initializeGL() override
  {
    initializeOpenGLFunctions();
    blitter_.create();
  }

  // Documentation inherited
  void resizeGL(int w, int h) override { glViewport(0, 0, w, h); }

  // Documentation inherited
  void paintGL() override
  {
    makeCurrent();
    QOpenGLTexture texture(image_);
    texture.setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    texture.setMagnificationFilter(QOpenGLTexture::Linear);
    glBindTexture(GL_TEXTURE_2D, texture.textureId());
    blitter_.bind(GL_TEXTURE_2D);
    QMatrix4x4 target;
    target.setToIdentity();
    blitter_.blit(texture.textureId(), target, QOpenGLTextureBlitter::OriginTopLeft);
    blitter_.release();
    texture.release();
    texture.destroy();
    doneCurrent();
  }

  QOpenGLTextureBlitter blitter_;
  QImage image_;
};
}  // namespace tesseract_gui
#endif  // BLITTER_RENDER_WIDGET_H
