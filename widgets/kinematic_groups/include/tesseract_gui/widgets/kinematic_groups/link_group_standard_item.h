#ifndef TESSERACT_GUI_WIDGETS_KINEMATICS_GROUP_LINK_GROUP_STANDARD_ITEM_H
#define TESSERACT_GUI_WIDGETS_KINEMATICS_GROUP_LINK_GROUP_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_srdf/kinematics_information.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class LinkGroupStandardItem : public QStandardItem
{
public:
  LinkGroupStandardItem(tesseract_srdf::LinkGroup group);
  explicit LinkGroupStandardItem(const QString &text, tesseract_srdf::LinkGroup group);
  LinkGroupStandardItem(const QIcon &icon, const QString &text, tesseract_srdf::LinkGroup group);
  int type() const override;

  tesseract_srdf::LinkGroup group;

private:
  void ctor();
};
}
#endif // TESSERACT_GUI_WIDGETS_KINEMATICS_GROUP_LINK_GROUP_STANDARD_ITEM_H
