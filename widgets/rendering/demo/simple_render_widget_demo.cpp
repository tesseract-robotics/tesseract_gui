
#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#include <QApplication>
#include <QDebug>
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <tesseract_gui/rendering/simple_render_widget.h>
#include <tesseract_gui/rendering/interactive_view_control.h>

int main(int argc, char ** argv)
{
    QApplication app(argc, argv);

    Q_INIT_RESOURCE(tesseract_gui_resources);

    tesseract_gui::SimpleRenderWidget widget;
    widget.show();

    tesseract_gui::InteractiveViewControl view_control;
    app.installEventFilter(&view_control);

    return app.exec();
}
