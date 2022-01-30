
#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#include <QApplication>
#include <QDebug>
#include <QtQuickWidgets/QtQuickWidgets>
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <tesseract_gui/rendering/minimal_scene.h>

int main(int argc, char ** argv)
{
    QApplication app(argc, argv);

    Q_INIT_RESOURCE(resources);
    Q_INIT_RESOURCE(minimal_scene);

    qmlRegisterType<tesseract_gui::RenderWindowItem>("RenderWindow", 1, 0, "RenderWindow");

    QQuickWidget widget;
//    widget.setResizeMode(QQuickWidget::SizeRootObjectToView);
//    QUrl url(":/tesseract_gui/MinimalScene.qml");
    QUrl url = QUrl::fromLocalFile("/home/levi/catkin_ws/tesseract_gui_ws/devel/include/tesseract_gui/rendering/MinimalScene.qml");
    widget.setSource(url);
    widget.show();

    QList<QQmlError> errors = widget.errors();
    for (const auto& error : errors)
      qCritical() << error.toString();
    return app.exec();
}
