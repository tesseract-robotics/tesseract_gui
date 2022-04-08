#ifndef TESSERACT_WIDGETS_SCENE_GRAPH_CONE_STANDARD_ITEM_H
#define TESSERACT_WIDGETS_SCENE_GRAPH_CONE_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_geometry/impl/cone.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class ConeStandardItem : public QStandardItem
{
public:
  ConeStandardItem(tesseract_geometry::Cone::Ptr cone);
  explicit ConeStandardItem(const QString &text, tesseract_geometry::Cone::Ptr cone);
  ConeStandardItem(const QIcon &icon, const QString &text, tesseract_geometry::Cone::Ptr cone);
  int type() const override;

  tesseract_geometry::Cone::Ptr cone;

private:
  void ctor();
};
}


#endif // TESSERACT_WIDGETS_SCENE_GRAPH_CONE_STANDARD_ITEM_H
