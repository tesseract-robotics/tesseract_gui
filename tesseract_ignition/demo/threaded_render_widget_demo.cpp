
#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#include <QApplication>
#include <QDebug>
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <tesseract_ignition/render_widget.h>
#include <tesseract_ignition/interactive_view_control.h>

int main(int argc, char ** argv)
{
    QApplication app(argc, argv);

    Q_INIT_RESOURCE(tesseract_widgets_resources);

    std::string scene_name = "scene";

    tesseract_gui::RenderWidget widget;
    widget.show();

    tesseract_gui::InteractiveViewControl view_control(scene_name);
    app.installEventFilter(&view_control);


    return app.exec();
}
