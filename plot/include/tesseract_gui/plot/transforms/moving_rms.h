#ifndef MOVING_RMS_H
#define MOVING_RMS_H

#include <QSpinBox>
#include <QWidget>
#include "PlotJuggler/transform_function.h"
#include "PlotJuggler/ring_span.hpp"

namespace Ui
{
class MovingRMS;
}

class MovingRMS : public PJ::TransformFunction_SISO
{
  Q_OBJECT

public:
  explicit MovingRMS();

  ~MovingRMS() override;

  void reset() override;

  const char* name() const override
  {
    return "Moving Root Mean Squared";
  }

  QWidget* optionsWidget() override;

  bool xmlSaveState(QDomDocument& doc, QDomElement& parent_element) const override;

  bool xmlLoadState(const QDomElement& parent_element) override;

private:
  Ui::MovingRMS* ui;

  QWidget* _widget;
  std::vector<PJ::PlotData::Point> _buffer;
  nonstd::ring_span_lite::ring_span<PJ::PlotData::Point> _ring_view;

  std::optional<PJ::PlotData::Point> calculateNextPoint(size_t index) override;
};

#endif  // MOVING_RMS_H
