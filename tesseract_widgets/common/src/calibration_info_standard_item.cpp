
#include <tesseract_widgets/common/calibration_info_standard_item.h>
#include <tesseract_widgets/common/transform_standard_item.h>
#include <tesseract_widgets/common/standard_item_type.h>

namespace tesseract_gui
{

CalibrationInfoStandardItem::CalibrationInfoStandardItem(tesseract_common::CalibrationInfo calibration_info)
  : QStandardItem(QIcon(":/tesseract_widgets/png/cube.png"), "Calibration Info")
  , calibration_info(std::move(calibration_info))
{
  ctor();
}

CalibrationInfoStandardItem::CalibrationInfoStandardItem(const QString &text, tesseract_common::CalibrationInfo calibration_info)
  : QStandardItem(QIcon(":/tesseract_widgets/png/cube.png"), text)
  , calibration_info(std::move(calibration_info))
{
  ctor();
}

CalibrationInfoStandardItem::CalibrationInfoStandardItem(const QIcon &icon, const QString &text, tesseract_common::CalibrationInfo calibration_info)
  : QStandardItem(icon, text)
  , calibration_info(std::move(calibration_info))
{
  ctor();
}

int CalibrationInfoStandardItem::type() const
{
  return static_cast<int>(StandardItemType::CALIBRATION_INFO);
}

void CalibrationInfoStandardItem::ctor()
{
  auto* joints_item = new QStandardItem(QIcon(":/tesseract_widgets/ignition/joint_vector.png"), "Joints");
  for (auto& joint : calibration_info.joints)
  {
    auto* item = new TransformStandardItem(QString::fromStdString(joint.first), joint.second);
    joints_item->appendRow(item);
  }
  joints_item->sortChildren(0);
  appendRow(joints_item);
}
}

