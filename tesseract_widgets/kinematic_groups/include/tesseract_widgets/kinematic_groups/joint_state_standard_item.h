#ifndef TESSERACT_WIDGETS_KINEMATIC_GROUP_JOINT_STATE_STANDARD_ITEM_H
#define TESSERACT_WIDGETS_KINEMATIC_GROUP_JOINT_STATE_STANDARD_ITEM_H

#include <tesseract_srdf/kinematics_information.h>

#include <QStandardItem>

namespace tesseract_gui
{
class JointStateStandardItem : public QStandardItem
{
public:
  explicit JointStateStandardItem(QString name, const std::unordered_map<std::string, double>& state);
  explicit JointStateStandardItem(const QString &text, QString name, const std::unordered_map<std::string, double>& state);
  explicit JointStateStandardItem(const QIcon &icon, const QString &text, QString name, const std::unordered_map<std::string, double>& state);
  int type() const override;

private:
  void ctor(const std::unordered_map<std::string, double>& state);
};
}

#endif // TESSERACT_WIDGETS_KINEMATIC_GROUP_JOINT_STATE_STANDARD_ITEM_H
