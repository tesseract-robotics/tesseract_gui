#ifndef TESSERACT_WIDGETS_KINEMATICS_GROUP_CHAIN_GROUP_STANDARD_ITEM_H
#define TESSERACT_WIDGETS_KINEMATICS_GROUP_CHAIN_GROUP_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_srdf/kinematics_information.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class ChainGroupStandardItem : public QStandardItem
{
public:
  ChainGroupStandardItem(tesseract_srdf::ChainGroup group);
  explicit ChainGroupStandardItem(const QString &text, tesseract_srdf::ChainGroup group);
  ChainGroupStandardItem(const QIcon &icon, const QString &text, tesseract_srdf::ChainGroup group);
  int type() const override;

  tesseract_srdf::ChainGroup group;

private:
  void ctor();
};
}

#endif // TESSERACT_WIDGETS_KINEMATICS_GROUP_CHAIN_GROUP_STANDARD_ITEM_H
