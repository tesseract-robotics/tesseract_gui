/**
 * @author Levi Armstrong <levi.armstrong@gmail.com>
 *
 * @copyright Copyright (C) 2022 Levi Armstrong <levi.armstrong@gmail.com>
 *
 * @par License
 * GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
 * @par
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 * @par
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * @par
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#include <tesseract_widgets/environment/environment_widget.h>
#include "ui_environment_widget.h"

#include <tesseract_widgets/environment/environment_widget_config.h>
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
  EnvironmentWidgetConfig config;
};

EnvironmentWidget::EnvironmentWidget(QWidget *parent)
  : QWidget(parent)
  , ui(std::make_unique<Ui::EnvironmentWidget>())
  , config_(std::make_shared<EnvironmentWidgetConfig>())
{
  ui->setupUi(this);

  ui->tab_widget->setCurrentIndex(0);

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

void EnvironmentWidget::setConfiguration(std::shared_ptr<EnvironmentWidgetConfig> config)
{
  if (config != nullptr)
  {
    disconnect(config_.get(), SIGNAL(modelsUpdated()), this , SLOT(onModelsUpdated()));
    disconnect(config_.get(), SIGNAL(environmentSet(tesseract_environment::Environment)), this , SIGNAL(environmentSet(tesseract_environment::Environment)));
    disconnect(config_.get(), SIGNAL(environmentChanged(tesseract_environment::Environment)), this , SIGNAL(environmentChanged(tesseract_environment::Environment)));
    disconnect(config_.get(), SIGNAL(environmentCurrentStateChanged(tesseract_environment::Environment)), this , SIGNAL(environmentCurrentStateChanged(tesseract_environment::Environment)));
  }

  config_ = std::move(config);
  ui->scene_tree_view->setModel(&config_->getSceneGraphModel());
  ui->state_tree_view->setModel(&config_->getSceneStateModel());
  ui->groups_tree_view->setModel(&config_->getKinematicGroupsModel());
  ui->group_tcps_tree_view->setModel(&config_->getGroupTCPsModel());
  ui->group_states_tree_view->setModel(&config_->getGroupJointStatesModel());
  ui->acm_tree_view->setModel(&config_->getAllowedCollisionMatrixModel());
  ui->cmd_history_tree_view->setModel(&config_->getEnvironmentCommandsModel());

  onModelsUpdated();

  connect(config_.get(), SIGNAL(modelsUpdated()), this , SLOT(onModelsUpdated()));
  connect(config_.get(), SIGNAL(environmentSet(tesseract_environment::Environment)), this , SIGNAL(environmentSet(tesseract_environment::Environment)));
  connect(config_.get(), SIGNAL(environmentChanged(tesseract_environment::Environment)), this , SIGNAL(environmentChanged(tesseract_environment::Environment)));
  connect(config_.get(), SIGNAL(environmentCurrentStateChanged(tesseract_environment::Environment)), this , SIGNAL(environmentCurrentStateChanged(tesseract_environment::Environment)));

  emit configurationSet(*config_);
  emit environmentSet(config_->environment());
}

const tesseract_environment::Environment& EnvironmentWidget::environment() const
{
  return config_->environment();
}

tesseract_environment::Environment& EnvironmentWidget::environment()
{
  return config_->environment();
}

tesseract_environment::Environment::ConstPtr EnvironmentWidget::getEnvironment() const
{
  return config_->getEnvironment();
}

tesseract_environment::Environment::Ptr EnvironmentWidget::getEnvironment()
{
  return config_->getEnvironment();
}

void EnvironmentWidget::onModelsUpdated()
{
  if (!config_->environment().isInitialized())
    return;

  // This hides the root element
  ui->group_states_tree_view->setRootIndex(config_->getGroupJointStatesModel().index(0,0));

  // This hides the root element
  ui->group_tcps_tree_view->setRootIndex(config_->getGroupTCPsModel().index(0,0));

  // This hides the root element
  ui->cmd_history_tree_view->setRootIndex(config_->getEnvironmentCommandsModel().index(0,0));

  // New data may have been added so resize first column
  ui->scene_tree_view->resizeColumnToContents(0);
  ui->state_tree_view->resizeColumnToContents(0);
  ui->acm_tree_view->resizeColumnToContents(0);
  ui->groups_tree_view->resizeColumnToContents(0);
  ui->group_tcps_tree_view->resizeColumnToContents(0);
  ui->group_states_tree_view->resizeColumnToContents(0);
  ui->cmd_history_tree_view->resizeColumnToContents(0);
}

void EnvironmentWidget::onRender()
{

}
}
