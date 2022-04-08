#ifndef TESSERACT_WIDGETS_JOINT_STATE_SLIDER_WIDGET_H
#define TESSERACT_WIDGETS_JOINT_STATE_SLIDER_WIDGET_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_scene_graph/joint.h>
#include <memory>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QWidget>

class QGridLayout;

namespace Ui {
class JointStateSliderWidget;
}

namespace tesseract_gui
{
class JointStateSliderWidget : public QWidget
{
  Q_OBJECT

public:
  explicit JointStateSliderWidget(QWidget *parent = nullptr);
  ~JointStateSliderWidget();

  void setJoints(const std::vector<tesseract_scene_graph::Joint::ConstPtr>& joints);
  std::unordered_map<std::string, double> getJointState() const;

Q_SIGNALS:
  void jointValueChanged(QString name, double value);
  void jointStateChanged(std::unordered_map<std::string, double> state);

private:
  std::unique_ptr<Ui::JointStateSliderWidget> ui_;
  QGridLayout* layout_;
  std::unordered_map<std::string, double> state_;

};
}

#endif // TESSERACT_WIDGETS_JOINT_STATE_SLIDER_WIDGET_H
