
#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#include <QApplication>
#include <QHBoxLayout>
#include <QObject>
#include <QDebug>
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <tesseract_widgets/plot/plot_docker.h>

int main(int argc, char ** argv)
{
    QApplication app(argc, argv);
    QWidget window;
    window.resize(320, 240);
    window.setWindowTitle(QApplication::translate("childwidget", "Child widget"));

    QHBoxLayout *layout = new QHBoxLayout(&window);

    tesseract_gui::PlotDataMapRef plot_data_map;
    tesseract_gui::PlotData& cosine_data = plot_data_map.getOrCreateNumeric("cosine");
    tesseract_gui::PlotData& sine_data = plot_data_map.getOrCreateNumeric("sine");
    for (int i = 0; i < int((2 * M_PI)/0.01); ++i)
    {
      cosine_data.pushBack(tesseract_gui::PlotDataXY::Point(i * 0.01, std::cos(i*0.01)));
      sine_data.pushBack(tesseract_gui::PlotDataXY::Point(i * 0.01, std::sin(i*0.01)));
    }

    auto plot_docker = new tesseract_gui::PlotDocker("test", plot_data_map);
    tesseract_gui::PlotWidget* widget = plot_docker->plotAt(0);
    widget->addCurve("cosine");
    widget->addCurve("sine");
    widget->enableTracker(true);
    widget->replot();

    QObject::connect(widget, &tesseract_gui::PlotWidget::trackerMoved, [widget](QPointF pos){
      widget->setTrackerPosition(pos.x());
      widget->replot();
    });

    layout->addWidget(plot_docker, 0, 0);

    window.show();

    return app.exec();
}
