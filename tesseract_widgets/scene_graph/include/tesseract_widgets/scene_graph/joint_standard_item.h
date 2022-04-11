#ifndef TESSERACT_WIDGETS_SCENE_GRAPH_JOINT_STANDARD_ITEM_H
#define TESSERACT_WIDGETS_SCENE_GRAPH_JOINT_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_scene_graph/joint.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{

class JointStandardItem : public QStandardItem
{
public:
  JointStandardItem(tesseract_scene_graph::Joint::Ptr joint);
  explicit JointStandardItem(const QString &text, tesseract_scene_graph::Joint::Ptr joint);
  JointStandardItem(const QIcon &icon, const QString &text, tesseract_scene_graph::Joint::Ptr joint);
  int type() const override;

  tesseract_scene_graph::Joint::Ptr joint;

private:
  void ctor();
};

}

#endif // TESSERACT_WIDGETS_SCENE_GRAPH_JOINT_STANDARD_ITEM_H