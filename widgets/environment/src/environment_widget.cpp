#include <tesseract_gui/environment/environment_widget.h>
#include "ui_environment_widget.h"

#include <tesseract_gui/scene_graph/scene_graph_standard_item.h>
#include <tesseract_gui/scene_graph/scene_state_standard_item.h>
#include <tesseract_gui/kinematic_groups/kinematic_groups_model.h>
#include <tesseract_gui/kinematic_groups/group_tcps_model.h>
#include <tesseract_gui/kinematic_groups/group_joint_states_model.h>
#include <tesseract_gui/acm/allowed_collision_matrix_model.h>
#include <tesseract_gui/common/gui_events.h>

#include <QStandardItemModel>


namespace tesseract_gui
{
struct EnvironmentWidgetImpl
{
  tesseract_environment::Environment::UPtr environment;
  QStandardItemModel scene_model;
  QStandardItemModel state_model;
  KinematicGroupsModel group_model;
  GroupTCPsModel group_tcps_model;
  GroupJointStatesModel group_states_model;
  AllowedCollisionMatrixModel acm_model;
};

EnvironmentWidget::EnvironmentWidget(QWidget *parent)
  : QWidget(parent)
  , ui(std::make_unique<Ui::EnvironmentWidget>())
  , data_(std::make_unique<EnvironmentWidgetImpl>())
{
  ui->setupUi(this);
  ui->scene_tree_view->setModel(&data_->scene_model);
  ui->state_tree_view->setModel(&data_->state_model);
  ui->groups_tree_view->setModel(&data_->group_model);
  ui->group_tcps_tree_view->setModel(&data_->group_tcps_model);
  ui->group_states_tree_view->setModel(&data_->group_states_model);
  ui->acm_tree_view->setModel(&data_->acm_model);

  connect(ui->scene_tree_view, &QTreeView::collapsed, [this](){ui->scene_tree_view->resizeColumnToContents(0);});
  connect(ui->scene_tree_view, &QTreeView::expanded, [this](){ui->scene_tree_view->resizeColumnToContents(0);});
  connect(ui->state_tree_view, &QTreeView::collapsed, [this](){ui->state_tree_view->resizeColumnToContents(0);});
  connect(ui->state_tree_view, &QTreeView::expanded, [this](){ui->state_tree_view->resizeColumnToContents(0);});
  connect(ui->groups_tree_view, &QTreeView::collapsed, [this](){ui->groups_tree_view->resizeColumnToContents(0);});
  connect(ui->groups_tree_view, &QTreeView::expanded, [this](){ui->groups_tree_view->resizeColumnToContents(0);});
  connect(ui->group_tcps_tree_view, &QTreeView::collapsed, [this](){ui->group_tcps_tree_view->resizeColumnToContents(0);});
  connect(ui->group_tcps_tree_view, &QTreeView::expanded, [this](){ui->group_tcps_tree_view->resizeColumnToContents(0);});
  connect(ui->group_states_tree_view, &QTreeView::collapsed, [this](){ui->group_states_tree_view->resizeColumnToContents(0);});
  connect(ui->group_states_tree_view, &QTreeView::expanded, [this](){ui->group_states_tree_view->resizeColumnToContents(0);});
  connect(ui->acm_tree_view, &QTreeView::collapsed, [this](){ui->acm_tree_view->resizeColumnToContents(0);});
  connect(ui->acm_tree_view, &QTreeView::expanded, [this](){ui->acm_tree_view->resizeColumnToContents(0);});
}

EnvironmentWidget::~EnvironmentWidget() = default;

void EnvironmentWidget::setEnvironment(tesseract_environment::Environment::UPtr env)
{
  data_->environment = std::move(env);

  // Scene Graph
  data_->scene_model.clear();
  data_->scene_model.setColumnCount(2);
  data_->scene_model.setHorizontalHeaderLabels({"Name", "Values"});
  auto* scene_item = new tesseract_gui::SceneGraphStandardItem(data_->environment->getSceneGraph()->clone());
  data_->scene_model.appendRow(scene_item);

  // Scene State
  data_->state_model.clear();
  data_->state_model.setColumnCount(2);
  data_->state_model.setHorizontalHeaderLabels({"Name", "Values"});
  auto* state_item = new tesseract_gui::SceneStateStandardItem(data_->environment->getState());
  data_->state_model.appendRow(state_item);

  // Allowed Collision Matrix
  data_->acm_model.setAllowedCollisionMatrix(*data_->environment->getAllowedCollisionMatrix());

  // Kinematic Groups
  auto kin_info = data_->environment->getKinematicsInformation();
  data_->group_model.set(kin_info.chain_groups, kin_info.joint_groups, kin_info.link_groups);

  // Groups States
  data_->group_states_model.set(kin_info.group_states);
  // This hides the root element
  ui->group_states_tree_view->setRootIndex(data_->group_states_model.index(0,0));

  // Tool Center Points
  data_->group_tcps_model.set(kin_info.group_tcps);
  // This hides the root element
  ui->group_tcps_tree_view->setRootIndex(data_->group_tcps_model.index(0,0));

  // New data may have been added so resize first column
  ui->scene_tree_view->resizeColumnToContents(0);
  ui->state_tree_view->resizeColumnToContents(0);
  ui->groups_tree_view->resizeColumnToContents(0);
  ui->group_tcps_tree_view->resizeColumnToContents(0);
  ui->group_states_tree_view->resizeColumnToContents(0);
  ui->acm_tree_view->resizeColumnToContents(0);

  emit environmentSet(*data_->environment);
}

const tesseract_environment::Environment* EnvironmentWidget::getEnvironment() const
{
  return data_->environment.get();
}

bool EnvironmentWidget::applyCommands(const tesseract_environment::Commands& commands)
{
  if (data_->environment == nullptr)
    return false;

  if (data_->environment->applyCommands(commands))
  {
    emit environmentCommandsApplied(commands);
    return true;
  }

  return false;
}

bool EnvironmentWidget::applyCommand(const tesseract_environment::Command::ConstPtr& command)
{
  return applyCommands({command});
}

}
