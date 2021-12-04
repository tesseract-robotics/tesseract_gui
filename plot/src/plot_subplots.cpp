#include <tesseract_gui/plot/plot_subplots.h>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QFrame>

namespace tesseract_gui
{

PlotSubplots::PlotSubplots(PlotDataMapRef& datamap, int rows, int cols, QWidget* parent)
  : QWidget(parent)
{
  if (rows < 1)
    throw std::runtime_error("PlotSubplots, rows must be greater than zero!");

  if (cols < 1)
    throw std::runtime_error("PlotSubplots, cols must be greater than zero!");

  QGridLayout *grid_layout = new QGridLayout(this);
  grid_layout->setHorizontalSpacing(cols);
  grid_layout->setVerticalSpacing(rows);
  grid_layout->setSpacing(0);

  subplots_.resize(rows);
  for (std::size_t r = 0; r < rows; ++r)
  {
    std::vector<std::unique_ptr<PlotWidget>>& row = subplots_[r];
    row.resize(cols);
    for  (std::size_t c = 0; c < cols; ++c)
    {
      QFrame* frame = new QFrame(this);
      frame->setFrameStyle(QFrame::Box);
      QGridLayout *layout = new QGridLayout(frame);

      auto plot_widget = std::make_unique<PlotWidget>(datamap);
      layout->addWidget(plot_widget->widget());
      grid_layout->addWidget(frame);
      row[c] = std::move(plot_widget);
    }
  }
}

PlotWidget* PlotSubplots::getSubplot(int row, int col)
{
  return subplots_.at(row).at(col).get();
}

}
