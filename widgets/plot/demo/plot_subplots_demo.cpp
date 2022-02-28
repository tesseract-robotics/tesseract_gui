
#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#include <QApplication>
#include <QObject>
#include <QDebug>
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <tesseract_gui/plot/plot_subplots.h>

int main(int argc, char ** argv)
{
    QApplication app(argc, argv);

    tesseract_gui::PlotDataMapRef plot_data_map;
    tesseract_gui::PlotData& cosine_data = plot_data_map.getOrCreateNumeric("cosine");
    tesseract_gui::PlotData& sine_data = plot_data_map.getOrCreateNumeric("sine");
    for (int i = 0; i < int((2 * M_PI)/0.01); ++i)
    {
      cosine_data.pushBack(tesseract_gui::PlotDataXY::Point(i * 0.01, std::cos(i*0.01)));
      sine_data.pushBack(tesseract_gui::PlotDataXY::Point(i * 0.01, std::sin(i*0.01)));
    }

    tesseract_gui::PlotSubplots subplots(plot_data_map, 2, 1);
    subplots.resize(320, 240);
    subplots.setWindowTitle(QApplication::translate("subplots", "Subplots"));

    { // Subplot [0, 0]
      tesseract_gui::PlotWidget* widget = subplots.getSubplot(0, 0);
      widget->addCurve("cosine");
      widget->setTitle("Cosine");
      widget->setXAxisTitle("Time (seconds)");
      widget->setYAxisTitle("Amplitude");
      widget->enableTracker(true);
      widget->zoomOut(true);
      widget->replot();

      QObject::connect(widget, &tesseract_gui::PlotWidget::trackerMoved, [widget](QPointF pos){
        widget->setTrackerPosition(pos.x());
        widget->replot();
      });
    }

    { // Subplot [1, 0]
      tesseract_gui::PlotWidget* widget = subplots.getSubplot(1, 0);
      widget->addCurve("sine");
      widget->setTitle("Sine");
      widget->setXAxisTitle("Time (seconds)");
      widget->setYAxisTitle("Amplitude");
      widget->enableTracker(true);
      widget->zoomOut(true);
      widget->replot();

      QObject::connect(widget, &tesseract_gui::PlotWidget::trackerMoved, [widget](QPointF pos){
        widget->setTrackerPosition(pos.x());
        widget->replot();
      });
    }

    subplots.show();

    return app.exec();
}
