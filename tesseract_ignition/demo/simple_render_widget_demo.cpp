
#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#include <QApplication>
#include <QDebug>
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <tesseract_ignition/simple_render_widget.h>
#include <tesseract_ignition/interactive_view_control.h>

int main(int argc, char ** argv)
{
    QApplication app(argc, argv);

    QSurfaceFormat format;
    format.setMajorVersion(3);
    format.setMinorVersion(2);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setOption(QSurfaceFormat::DebugContext);

    Q_INIT_RESOURCE(tesseract_widgets_resources);

    std::string scene_name = "scene";
    tesseract_gui::SimpleRenderWidget widget(scene_name);
    widget.show();

    tesseract_gui::InteractiveViewControl view_control(scene_name);
    app.installEventFilter(&view_control);

    return app.exec();
}
