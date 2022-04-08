#ifndef TESSERACT_WIDGETS_PLOT_PLOT_SUBPLOTS_H
#define TESSERACT_WIDGETS_PLOT_PLOT_SUBPLOTS_H

#include <QWidget>
#include <memory>
#include <tesseract_widgets/plot/plot_database.h>
#include <tesseract_widgets/plot/plot_widget.h>

namespace tesseract_gui
{
class PlotSubplots : public QWidget
{
  Q_OBJECT

public:
  PlotSubplots(PlotDataMapRef& datamap, int rows, int cols, QWidget* parent = nullptr);

  PlotWidget* getSubplot(int row, int col);
  int rows() const;
  int cols() const;

public Q_SLOTS:
  void replot();

private:
  int rows_{0};
  int cols_{0};
  std::vector<std::vector<std::unique_ptr<PlotWidget>>> subplots_;
};
}
#endif // TESSERACT_WIDGETS_PLOT_PLOT_SUBPLOTS_H
