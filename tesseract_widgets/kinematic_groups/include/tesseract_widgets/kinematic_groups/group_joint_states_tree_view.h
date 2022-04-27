#ifndef TESSERACT_WIDGETS_KINEMATIC_GROUP_GROUP_JOINT_STATES_TREE_VIEW_H
#define TESSERACT_WIDGETS_KINEMATIC_GROUP_GROUP_JOINT_STATES_TREE_VIEW_H

#include <QTreeView>
#include <memory>

namespace tesseract_gui
{
struct GroupJointStatesTreeViewImpl;
class GroupJointStatesTreeView : public QTreeView
{
  Q_OBJECT
public:
  explicit GroupJointStatesTreeView(QWidget* parent = nullptr);
  ~GroupJointStatesTreeView();

  void setModel(QAbstractItemModel* model) override;

Q_SIGNALS:
  void showGroupsJointState(const std::unordered_map<std::string, double>& groups_joint_state);

public Q_SLOTS:
  void onCurrentRowChanged(const QModelIndex& current, const QModelIndex& previous);

private:
  std::unique_ptr<GroupJointStatesTreeViewImpl> data_;
};
}  // namespace tesseract_gui

#endif  // TESSERACT_WIDGETS_KINEMATIC_GROUP_GROUP_JOINT_STATES_TREE_VIEW_H
