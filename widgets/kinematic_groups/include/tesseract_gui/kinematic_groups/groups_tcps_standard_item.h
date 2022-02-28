#ifndef TESSERACT_GUI_KINEMATIC_GROUP_GROUPS_TCPS_STANDARD_ITEM_H
#define TESSERACT_GUI_KINEMATIC_GROUP_GROUPS_TCPS_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_srdf/kinematics_information.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <tesseract_gui/scene_graph/origin_standard_item.h>

#include <QStandardItem>

namespace tesseract_gui
{
class GroupsTCPsStandardItem : public QStandardItem
{
public:
  explicit GroupsTCPsStandardItem(const QString &text, const tesseract_srdf::GroupsTCPs& groups_tcps);
  explicit GroupsTCPsStandardItem(const QIcon &icon, const QString &text, const tesseract_srdf::GroupsTCPs& groups_tcps);
  int type() const override;

  tesseract_srdf::GroupsTCPs groups_tcps;

private:
  void ctor();
};
}
#endif // TESSERACT_GUI_KINEMATIC_GROUP_GROUPS_TCPS_STANDARD_ITEM_H
