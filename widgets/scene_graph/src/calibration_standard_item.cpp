#include <tesseract_gui/widgets/scene_graph/calibration_standard_item.h>
#include <tesseract_gui/widgets/common/standard_item_utils.h>
#include <tesseract_gui/common/standard_item_type.h>

namespace tesseract_gui
{
CalibrationStandardItem::CalibrationStandardItem(tesseract_scene_graph::JointCalibration::Ptr calibration)
  : QStandardItem(QIcon(":/tesseract_gui/png/calibration.png"), "Calibration")
  , calibration(std::move(calibration))
{
  ctor();
}

CalibrationStandardItem::CalibrationStandardItem(const QString &text, tesseract_scene_graph::JointCalibration::Ptr calibration)
  : QStandardItem(QIcon(":/tesseract_gui/png/calibration.png"), text)
  , calibration(std::move(calibration))
{
  ctor();
}

CalibrationStandardItem::CalibrationStandardItem(const QIcon &icon, const QString &text, tesseract_scene_graph::JointCalibration::Ptr calibration)
  : QStandardItem(icon, text)
  , calibration(std::move(calibration))
{
  ctor();
}

int CalibrationStandardItem::type() const
{
  return static_cast<int>(StandardItemType::CALIBRATION);
}

void CalibrationStandardItem::ctor()
{
  appendRow(createStandardItemFloat("reference_position", calibration->reference_position));
  appendRow(createStandardItemFloat("rising", calibration->rising));
  appendRow(createStandardItemFloat("falling", calibration->falling));
}
}
