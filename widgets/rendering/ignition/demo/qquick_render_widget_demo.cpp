
#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#include <QApplication>
#include <QDebug>
#include <QtQuickWidgets/QtQuickWidgets>
#include <QOpenGLContext>
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <tesseract_gui/widgets/rendering/ignition/minimal_scene.h>
#include <tesseract_gui/widgets/rendering/ignition/interactive_view_control.h>

int main(int argc, char ** argv)
{
    QApplication app(argc, argv);

    Q_INIT_RESOURCE(tesseract_gui_resources);
    Q_INIT_RESOURCE(minimal_scene);

    qmlRegisterType<tesseract_gui::RenderWindowItem>("RenderWindow", 1, 0, "RenderWindow");

    QUrl url("/home/levi/catkin_ws/tesseract_gui_ws/devel/include/tesseract_gui/rendering/MinimalScene.qml");

//    QQuickWidget widget;
//    widget.setResizeMode(QQuickWidget::SizeRootObjectToView);
////    QUrl url(":/tesseract_gui/MinimalScene.qml");
//    widget.setSource(url);
//    widget.show();

    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(url);
    view.show();

    tesseract_gui::InteractiveViewControl view_control("scene");
    app.installEventFilter(&view_control);

//    QList<QQmlError> errors = view.errors();
//    for (const auto& error : errors)
//      qCritical() << error.toString();
    return app.exec();
}
