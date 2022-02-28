#ifndef TESSERACT_GUI_KINEMATIC_GROUP_OPW_PARAMS_STANDARD_ITEM_H
#define TESSERACT_GUI_KINEMATIC_GROUP_OPW_PARAMS_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_kinematics/opw/opw_inv_kin.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class OPWParamsStandardItem : public QStandardItem
{
public:
  OPWParamsStandardItem(opw_kinematics::Parameters<double> params);
  explicit OPWParamsStandardItem(const QString &text, opw_kinematics::Parameters<double> params);
  OPWParamsStandardItem(const QIcon &icon, const QString &text, opw_kinematics::Parameters<double> params);
  int type() const override;

  opw_kinematics::Parameters<double> params;

private:
  void ctor();
};
}

#endif // TESSERACT_GUI_KINEMATIC_GROUP_OPW_PARAMS_STANDARD_ITEM_H
