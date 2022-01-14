
#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#include <QApplication>
#include <QDebug>
#include <QtQuickWidgets/QtQuickWidgets>
TESSERACT_COMMON_IGNORE_WARNINGS_POP

int main(int argc, char ** argv)
{
    QApplication app(argc, argv);

    Q_INIT_RESOURCE(resources);
    Q_INIT_RESOURCE(minimal_scene);


    QQuickWidget widget;
    widget.setResizeMode(QQuickWidget::SizeRootObjectToView);
    widget.setSource(QUrl(":/tesseract_gui/minimal_scene.qml"));
    widget.show();

    return app.exec();
}
