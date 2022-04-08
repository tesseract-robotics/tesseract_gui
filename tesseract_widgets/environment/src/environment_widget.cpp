#include <tesseract_widgets/environment/environment_widget.h>
#include "ui_environment_widget.h"

#include <tesseract_widgets/environment/environment_commands_model.h>
#include <tesseract_widgets/scene_graph/scene_graph_standard_item.h>
#include <tesseract_widgets/scene_graph/scene_state_standard_item.h>
#include <tesseract_widgets/kinematic_groups/kinematic_groups_model.h>
#include <tesseract_widgets/kinematic_groups/group_tcps_model.h>
#include <tesseract_widgets/kinematic_groups/group_joint_states_model.h>
#include <tesseract_widgets/acm/allowed_collision_matrix_model.h>

#include <QStandardItemModel>

namespace tesseract_gui
{
struct EnvironmentWidgetImpl
{
  tesseract_environment::Environment::Ptr environment;
  long widget_revision{0};
  std::chrono::system_clock::time_point widget_state_timestamp{std::chrono::system_clock::now()};
  QStandardItemModel scene_model;
  QStandardItemModel state_model;
  KinematicGroupsModel group_model;
  GroupTCPsModel group_tcps_model;
  GroupJointStatesModel group_states_model;
  AllowedCollisionMatrixModel acm_model;
  EnvironmentCommandsModel commands_model;
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
  ui->cmd_history_tree_view->setModel(&data_->commands_model);

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
  connect(ui->cmd_history_tree_view, &QTreeView::collapsed, [this](){ui->cmd_history_tree_view->resizeColumnToContents(0);});
  connect(ui->cmd_history_tree_view, &QTreeView::expanded, [this](){ui->cmd_history_tree_view->resizeColumnToContents(0);});

//  connect(ui->acm_tree_view, SIGNAL(entrySelected()), this, SIGNAL(entrySelected()));
  connect(ui->acm_tree_view, SIGNAL(selectedLinksChanged(std::vector<std::string>)), this, SIGNAL(selectedLinksChanged(std::vector<std::string>)));
}

EnvironmentWidget::~EnvironmentWidget() = default;

void EnvironmentWidget::setEnvironment(tesseract_environment::Environment::Ptr env)
{
  data_->environment = std::move(env);
  data_->environment->addEventCallback([this](const tesseract_environment::Event& event){this->tesseractEventFilter(event);});

  updateModels();

  emit environmentSet(*data_->environment);
}

const tesseract_environment::Environment& EnvironmentWidget::environment() const
{
  return *(data_->environment);
}

tesseract_environment::Environment& EnvironmentWidget::environment()
{
  return *(data_->environment);
}

tesseract_environment::Environment::ConstPtr EnvironmentWidget::getEnvironment() const
{
  return data_->environment;
}

tesseract_environment::Environment::Ptr EnvironmentWidget::getEnvironment()
{
  return data_->environment;
}

void EnvironmentWidget::updateModels()
{
  auto lock = data_->environment->lockRead();

  // Store the revision
  data_->widget_revision = data_->environment->getRevision();
  data_->widget_state_timestamp = data_->environment->getCurrentStateTimestamp();

  // Scene Graph
  updateSceneGraphModel();

  // Scene State
  updateCurrentStateModel();

  // Allowed Collision Matrix
  updateAllowedCollisionMatrixModel();

  // Kinematic Information
  updateKinematicsInformationModels();

  // Command History
  updateCommandHistoryModel();
}

void EnvironmentWidget::updateSceneGraphModel()
{
  data_->scene_model.clear();
  data_->scene_model.setColumnCount(2);
  data_->scene_model.setHorizontalHeaderLabels({"Name", "Values"});
  auto* scene_item = new tesseract_gui::SceneGraphStandardItem(data_->environment->getSceneGraph()->clone());
  data_->scene_model.appendRow(scene_item);

  // New data may have been added so resize first column
  ui->scene_tree_view->resizeColumnToContents(0);
}
void EnvironmentWidget::updateCurrentStateModel()
{
  data_->state_model.clear();
  data_->state_model.setColumnCount(2);
  data_->state_model.setHorizontalHeaderLabels({"Name", "Values"});
  auto* state_item = new tesseract_gui::SceneStateStandardItem(data_->environment->getState());
  data_->state_model.appendRow(state_item);

  // New data may have been added so resize first column
  ui->state_tree_view->resizeColumnToContents(0);
}
void EnvironmentWidget::updateAllowedCollisionMatrixModel()
{
  data_->acm_model.setAllowedCollisionMatrix(*data_->environment->getAllowedCollisionMatrix());

  // New data may have been added so resize first column
  ui->acm_tree_view->resizeColumnToContents(0);
}

void EnvironmentWidget::updateKinematicsInformationModels()
{
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
  ui->groups_tree_view->resizeColumnToContents(0);
  ui->group_tcps_tree_view->resizeColumnToContents(0);
  ui->group_states_tree_view->resizeColumnToContents(0);
}

void EnvironmentWidget::updateCommandHistoryModel()
{
  data_->commands_model.set(data_->environment->getCommandHistory());
  // This hides the root element
  ui->cmd_history_tree_view->setRootIndex(data_->commands_model.index(0,0));

  // New data may have been added so resize first column
  ui->cmd_history_tree_view->resizeColumnToContents(0);
}

void EnvironmentWidget::tesseractEventFilter(const tesseract_environment::Event& event)
{
  switch (event.type)
  {
    case tesseract_environment::Events::COMMAND_APPLIED:
    {
      updateModels();
      emit environmentChanged(*data_->environment);
      break;
    }
    case tesseract_environment::Events::SCENE_STATE_CHANGED:
    {
      updateCurrentStateModel();
      emit environmentCurrentStateChanged(*data_->environment);
      break;
    }
  }
}
}
