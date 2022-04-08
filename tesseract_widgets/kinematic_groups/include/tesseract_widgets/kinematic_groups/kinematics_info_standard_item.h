#ifndef TESSERACT_WIDGETS_KINEMATICS_GROUP_KINEMATICS_INFO_STANDARD_ITEM_H
#define TESSERACT_WIDGETS_KINEMATICS_GROUP_KINEMATICS_INFO_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_srdf/kinematics_information.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class KinematicsInfoStandardItem : public QStandardItem
{
public:
  KinematicsInfoStandardItem(tesseract_srdf::KinematicsInformation kinematics_info);
  explicit KinematicsInfoStandardItem(const QString &text, tesseract_srdf::KinematicsInformation kinematics_info);
  KinematicsInfoStandardItem(const QIcon &icon, const QString &text, tesseract_srdf::KinematicsInformation kinematics_info);
  int type() const override;

  tesseract_srdf::KinematicsInformation kinematics_info;

private:
  void ctor();
};
}


#endif // TESSERACT_WIDGETS_KINEMATICS_GROUP_KINEMATICS_INFO_STANDARD_ITEM_H
