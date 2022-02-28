
#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#include <QApplication>
#include <QHBoxLayout>
#include <QObject>
#include <QLatin1String>
#include <QDebug>
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <tesseract_gui/plot/plot_widget.h>

int main(int argc, char ** argv)
{
    QApplication app(argc, argv);
    { //open qss file
      QFile file(":/tesseract_gui/themes/Takezo/Takezo.qss");
      file.open(QFile::ReadOnly);

      QString styleSheet { QLatin1String(file.readAll()) };

      //setup stylesheet
      app.setStyleSheet(styleSheet);
    }

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

    auto widget =  std::make_unique<tesseract_gui::PlotWidget>(plot_data_map);
    widget->addCurve("cosine");
    widget->addCurve("sine");
    widget->enableTracker(true);
    widget->replot();

    QObject::connect(widget.get(), &tesseract_gui::PlotWidget::trackerMoved, [&](QPointF pos){
      widget->setTrackerPosition(pos.x());
      widget->replot();
    });

    layout->addWidget(widget->widget(), 0, 0);

    window.show();

    return app.exec();
}
