#ifndef TESSERACT_GUI_PLOT_PLOT_SUBPLOTS_H
#define TESSERACT_GUI_PLOT_PLOT_SUBPLOTS_H

#include <QWidget>
#include <memory>
#include <tesseract_gui/plot/plot_database.h>
#include <tesseract_gui/plot/plot_widget.h>

namespace tesseract_gui
{
class PlotSubplots : public QWidget
{
  Q_OBJECT

public:
  PlotSubplots(PlotDataMapRef& datamap, int rows, int cols, QWidget* parent = nullptr);

  PlotWidget* getSubplot(int row, int col);

private:
  std::vector<std::vector<std::unique_ptr<PlotWidget>>> subplots_;
};
}
#endif // TESSERACT_GUI_PLOT_PLOT_SUBPLOTS_H
