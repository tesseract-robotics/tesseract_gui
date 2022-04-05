#ifndef TESSERACT_GUI_WIDGETS_COMMON_CALIBRATION_INFO_STANDARD_ITEM_H
#define TESSERACT_GUI_WIDGETS_COMMON_CALIBRATION_INFO_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_common/types.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class CalibrationInfoStandardItem : public QStandardItem
{
public:
  CalibrationInfoStandardItem(tesseract_common::CalibrationInfo calibration_info);
  explicit CalibrationInfoStandardItem(const QString &text, tesseract_common::CalibrationInfo calibration_info);
  CalibrationInfoStandardItem(const QIcon &icon, const QString &text, tesseract_common::CalibrationInfo calibration_info);
  int type() const override;

  tesseract_common::CalibrationInfo calibration_info;

private:
  void ctor();
};
}

#endif // TESSERACT_GUI_WIDGETS_COMMON_CALIBRATION_INFO_STANDARD_ITEM_H
