#ifndef TESSERACT_WIDGETS_KINEMATICS_GROUP_JOINT_GROUP_STANDARD_ITEM_H
#define TESSERACT_WIDGETS_KINEMATICS_GROUP_JOINT_GROUP_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_srdf/kinematics_information.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class JointGroupStandardItem : public QStandardItem
{
public:
  JointGroupStandardItem(tesseract_srdf::JointGroup group);
  explicit JointGroupStandardItem(const QString &text, tesseract_srdf::JointGroup group);
  JointGroupStandardItem(const QIcon &icon, const QString &text, tesseract_srdf::JointGroup group);
  int type() const override;

  tesseract_srdf::JointGroup group;

private:
  void ctor();
};
}

#endif // TESSERACT_WIDGETS_KINEMATICS_GROUP_JOINT_GROUP_STANDARD_ITEM_H