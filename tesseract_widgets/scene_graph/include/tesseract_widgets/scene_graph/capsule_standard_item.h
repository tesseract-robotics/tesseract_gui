#ifndef TESSERACT_WIDGETS_SCENE_GRAPH_CAPSULE_STANDARD_ITEM_H
#define TESSERACT_WIDGETS_SCENE_GRAPH_CAPSULE_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_geometry/impl/capsule.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class CapsuleStandardItem : public QStandardItem
{
public:
  CapsuleStandardItem(tesseract_geometry::Capsule::Ptr capsule);
  explicit CapsuleStandardItem(const QString &text, tesseract_geometry::Capsule::Ptr capsule);
  CapsuleStandardItem(const QIcon &icon, const QString &text, tesseract_geometry::Capsule::Ptr capsule);
  int type() const override;

  tesseract_geometry::Capsule::Ptr capsule;

private:
  void ctor();
};
}

#endif // TESSERACT_WIDGETS_SCENE_GRAPH_CAPSULE_STANDARD_ITEM_H
