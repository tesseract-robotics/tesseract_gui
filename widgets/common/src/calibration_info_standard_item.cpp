
#include <tesseract_gui/widgets/common/calibration_info_standard_item.h>
#include <tesseract_gui/widgets/common/transform_standard_item.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, CUBE_ICON, (":/tesseract_gui/png/cube.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, JOINT_VECTOR_ICON, (":/tesseract_gui/ignition/joint_vector.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, ORIGIN_ICON, (":/tesseract_gui/png/origin.png"));

namespace tesseract_gui
{

CalibrationInfoStandardItem::CalibrationInfoStandardItem(tesseract_common::CalibrationInfo calibration_info)
  : QStandardItem(*CUBE_ICON(), "Calibration Info")
  , calibration_info(std::move(calibration_info))
{
  ctor();
}

CalibrationInfoStandardItem::CalibrationInfoStandardItem(const QString &text, tesseract_common::CalibrationInfo calibration_info)
  : QStandardItem(*CUBE_ICON(), text)
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
  auto* joints_item = new QStandardItem(*JOINT_VECTOR_ICON(), "Joints");
  for (auto& joint : calibration_info.joints)
  {
    auto* item = new TransformStandardItem(*ORIGIN_ICON(), QString::fromStdString(joint.first), joint.second);
    joints_item->appendRow(item);
  }
  joints_item->sortChildren(0);
  appendRow(joints_item);
}
}

