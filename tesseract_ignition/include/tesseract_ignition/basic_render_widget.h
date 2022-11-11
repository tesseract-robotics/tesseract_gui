#ifndef BASIC_RENDER_WIDGET_H
#define BASIC_RENDER_WIDGET_H

#include <QWidget>
#include <QOpenGLWidget>

namespace tesseract_gui
{
class BasicRenderWidget : public QOpenGLWidget
{
  Q_OBJECT
public:
  explicit BasicRenderWidget(QWidget* _parent = nullptr);

protected:
  void paintEvent(QPaintEvent* event) override;
};
}  // namespace tesseract_gui
#endif  // BASIC_RENDER_WIDGET_H
