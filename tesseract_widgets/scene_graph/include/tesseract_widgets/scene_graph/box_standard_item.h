#ifndef TESSERACT_WIDGETS_SCENE_GRAPH_BOX_STANDARD_ITEM_H
#define TESSERACT_WIDGETS_SCENE_GRAPH_BOX_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_geometry/impl/box.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class BoxStandardItem : public QStandardItem
{
public:
  BoxStandardItem(tesseract_geometry::Box::Ptr box);
  explicit BoxStandardItem(const QString &text, tesseract_geometry::Box::Ptr box);
  BoxStandardItem(const QIcon &icon, const QString &text, tesseract_geometry::Box::Ptr box);
  int type() const override;

  tesseract_geometry::Box::Ptr box;

private:
  void ctor();
};
}

#endif // TESSERACT_WIDGETS_SCENE_GRAPH_BOX_STANDARD_ITEM_H
