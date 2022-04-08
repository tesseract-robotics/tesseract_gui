#include <tesseract_robotics_studio/tesseract_robotics_studio.h>
#include <QString>
#include <QFile>
#include <QApplication>
#include <QDebug>
#include <memory>

int main(int argc, char *argv[])
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

  Q_INIT_RESOURCE(tesseract_widgets_resources);
  Q_INIT_RESOURCE(qdarkstyle_dark);
  Q_INIT_RESOURCE(qdarkstyle_light);

  { //open qss file
//    QFile file(":/tesseract_widgets/themes/VisualScript/VisualScript.qss");
//    QFile file(":/tesseract_widgets/themes/Combinear/Combinear.qss");
    QFile file(":/qdarkstyle/dark/qdarkstyle_dark.qss");
//    QFile file(":/qdarkstyle/light/qdarkstyle_light.qss");
    file.open(QFile::ReadOnly);

    QString styleSheet { QLatin1String(file.readAll()) };

    //setup stylesheet
    app.setStyleSheet(styleSheet);
  }

//  QFile StyleSheetFile(":/adsdemo/app.css");
//  StyleSheetFile.open(QIODevice::ReadOnly);
//  QTextStream StyleSheetStream(&StyleSheetFile);
//  app.setStyleSheet(StyleSheetStream.readAll());
//  StyleSheetFile.close();

  tesseract_gui::TesseractRoboticsStudio widget;
  widget.show();

  return app.exec();
}
