#include <tesseract_gui/scene_graph/calibration_standard_item.h>
#include <tesseract_gui/scene_graph/types.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, CALIBRATION_ICON, (":/tesseract_gui/png/calibration.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, NUMERIC_ICON, (":/tesseract_gui/png/numeric.png"));

namespace tesseract_gui
{
CalibrationStandardItem::CalibrationStandardItem(tesseract_scene_graph::JointCalibration::Ptr calibration)
  : QStandardItem(*CALIBRATION_ICON(), "Calibration")
  , calibration(std::move(calibration))
{
  ctor();
}

CalibrationStandardItem::CalibrationStandardItem(const QString &text, tesseract_scene_graph::JointCalibration::Ptr calibration)
  : QStandardItem(*CALIBRATION_ICON(), text)
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
  return static_cast<int>(SceneGraphItemType::CALIBRATION);
}

void CalibrationStandardItem::ctor()
{
  {
    auto* item = new QStandardItem(*NUMERIC_ICON(), "reference_position");
    auto* value = new QStandardItem(QString("%1").arg(calibration->reference_position));
    appendRow({item, value});
  }

  {
    auto* item = new QStandardItem(*NUMERIC_ICON(), "rising");
    auto* value = new QStandardItem(QString("%1").arg(calibration->rising));
    appendRow({item, value});
  }

  {
    auto* item = new QStandardItem(*NUMERIC_ICON(), "falling");
    auto* value = new QStandardItem(QString("%1").arg(calibration->falling));
    appendRow({item, value});
  }
}
}
