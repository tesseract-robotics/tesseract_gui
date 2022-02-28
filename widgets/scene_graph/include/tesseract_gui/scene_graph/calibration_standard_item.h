#ifndef TESSERACT_GUI_SCENE_GRAPH_CALIBRATION_STANDARD_ITEM_H
#define TESSERACT_GUI_SCENE_GRAPH_CALIBRATION_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_scene_graph/link.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class CalibrationStandardItem : public QStandardItem
{
public:
  CalibrationStandardItem(tesseract_scene_graph::JointCalibration::Ptr calibration);
  explicit CalibrationStandardItem(const QString &text, tesseract_scene_graph::JointCalibration::Ptr calibration);
  CalibrationStandardItem(const QIcon &icon, const QString &text, tesseract_scene_graph::JointCalibration::Ptr calibration);
  int type() const override;

  tesseract_scene_graph::JointCalibration::Ptr calibration;

private:
  void ctor();
};
}

#endif // TESSERACT_GUI_SCENE_GRAPH_CALIBRATION_STANDARD_ITEM_H
