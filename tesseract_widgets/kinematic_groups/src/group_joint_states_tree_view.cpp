#include <tesseract_widgets/kinematic_groups/group_joint_states_tree_view.h>
#include <tesseract_widgets/kinematic_groups/group_joint_states_model.h>
#include <tesseract_widgets/common/standard_item_type.h>

namespace tesseract_gui
{
struct GroupJointStatesTreeViewImpl
{
  GroupJointStatesModel* model;

  // Store the selected item
  QStandardItem* selected_item;
};

GroupJointStatesTreeView::GroupJointStatesTreeView(QWidget* parent)
  : QTreeView(parent), data_(std::make_unique<GroupJointStatesTreeViewImpl>())
{
  connect(this, &QTreeView::collapsed, [this]() { resizeColumnToContents(0); });
  connect(this, &QTreeView::expanded, [this]() { resizeColumnToContents(0); });
}

GroupJointStatesTreeView::~GroupJointStatesTreeView() = default;

void GroupJointStatesTreeView::setModel(QAbstractItemModel* model)
{
  data_->model = qobject_cast<GroupJointStatesModel*>(model);
  QTreeView::setModel(model);

  connect(selectionModel(),
          SIGNAL(currentRowChanged(QModelIndex, QModelIndex)),
          this,
          SLOT(onCurrentRowChanged(QModelIndex, QModelIndex)));
}

void GroupJointStatesTreeView::onCurrentRowChanged(const QModelIndex& current, const QModelIndex& previous)
{
  data_->selected_item = data_->model->itemFromIndex(current);

  if (data_->selected_item->type() != static_cast<int>(StandardItemType::NAMESPACE))
  {
    const tesseract_srdf::GroupsJointState& state = data_->model->getGroupsJointState(current);
    Q_EMIT showGroupsJointState(state);
  }
}

}  // namespace tesseract_gui
