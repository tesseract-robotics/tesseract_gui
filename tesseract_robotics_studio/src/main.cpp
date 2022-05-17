#include <tesseract_robotics_studio/tesseract_robotics_studio.h>
#include <tesseract_qt/common/theme_utils.h>
#include <QString>
#include <QFile>
#include <QApplication>
#include <QDebug>
#include <memory>

int main(int argc, char* argv[])
{
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
  QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#if QT_VERSION >= 0x050600
  QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
#endif
  QApplication::setAttribute(Qt::AA_ShareOpenGLContexts);

  QApplication app(argc, argv);
  QApplication::setApplicationName("Tesseract Robotics Studio");
  QApplication::setQuitOnLastWindowClosed(true);

  Q_INIT_RESOURCE(tesseract_qt_resources);
  Q_INIT_RESOURCE(qdarkstyle_dark);
  Q_INIT_RESOURCE(qdarkstyle_light);

  // setup stylesheet
  app.setStyleSheet(tesseract_gui::themes::getDarkTheme());

  tesseract_gui::TesseractRoboticsStudio widget;
  widget.show();

  return QApplication::exec();
}
