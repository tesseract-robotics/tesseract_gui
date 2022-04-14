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

#include <tesseract_widgets/environment/environment_widget_config.h>
#include <tesseract_widgets/environment/environment_commands_model.h>
#include <tesseract_widgets/scene_graph/scene_graph_standard_item.h>
#include <tesseract_widgets/scene_graph/scene_state_standard_item.h>
#include <tesseract_widgets/kinematic_groups/kinematic_groups_model.h>
#include <tesseract_widgets/kinematic_groups/group_tcps_model.h>
#include <tesseract_widgets/kinematic_groups/group_joint_states_model.h>
#include <tesseract_widgets/acm/allowed_collision_matrix_model.h>

#include <tesseract_environment/environment.h>

#include <qobjectdefs.h>

namespace tesseract_gui
{

struct EnvironmentWidgetConfigImpl
{
  std::size_t hash;
  tesseract_environment::Environment::Ptr environment;
  QStandardItemModel scene_model;
  QStandardItemModel scene_state_model;
  KinematicGroupsModel group_model;
  GroupTCPsModel group_tcps_model;
  GroupJointStatesModel group_states_model;
  AllowedCollisionMatrixModel acm_model;
  EnvironmentCommandsModel commands_model;
};

EnvironmentWidgetConfig::EnvironmentWidgetConfig()
  : data_(std::make_unique<EnvironmentWidgetConfigImpl>())
{
  data_->hash = std::hash<EnvironmentWidgetConfig*>{}(this);
}

EnvironmentWidgetConfig::~EnvironmentWidgetConfig()
{
  // disconnect the callback
  if (data_->environment != nullptr)
    data_->environment->removeEventCallback(data_->hash);
};

void EnvironmentWidgetConfig::setEnvironment(tesseract_environment::Environment::Ptr env)
{
  // disconnect the callback
  if (data_->environment != nullptr)
    data_->environment->removeEventCallback(data_->hash);

  data_->environment = std::move(env);
  data_->environment->addEventCallback(data_->hash, [this](const tesseract_environment::Event& event){this->tesseractEventFilter(event);});

  clear();

  onUpdateModels();

  emit environmentSet(*data_->environment);
}

const tesseract_environment::Environment& EnvironmentWidgetConfig::environment() const
{
  return *(data_->environment);
}

tesseract_environment::Environment& EnvironmentWidgetConfig::environment()
{
  return *(data_->environment);
}

tesseract_environment::Environment::ConstPtr EnvironmentWidgetConfig::getEnvironment() const
{
  return data_->environment;
}

tesseract_environment::Environment::Ptr EnvironmentWidgetConfig::getEnvironment()
{
  return data_->environment;
}

QStandardItemModel& EnvironmentWidgetConfig::getSceneGraphModel() {return data_->scene_model;}
QStandardItemModel& EnvironmentWidgetConfig::getSceneStateModel() {return data_->scene_state_model;}
KinematicGroupsModel& EnvironmentWidgetConfig::getKinematicGroupsModel() {return data_->group_model;}
GroupTCPsModel& EnvironmentWidgetConfig::getGroupTCPsModel() {return data_->group_tcps_model;}
GroupJointStatesModel& EnvironmentWidgetConfig::getGroupJointStatesModel() {return data_->group_states_model;}
AllowedCollisionMatrixModel& EnvironmentWidgetConfig::getAllowedCollisionMatrixModel() {return data_->acm_model;}
EnvironmentCommandsModel& EnvironmentWidgetConfig::getEnvironmentCommandsModel() {return data_->commands_model;}

void EnvironmentWidgetConfig::clear()
{
  data_->scene_model.clear();
  data_->scene_state_model.clear();
  data_->acm_model.clear();
  data_->group_model.clear();
  data_->group_states_model.clear();
  data_->group_tcps_model.clear();
  data_->commands_model.clear();
}

void EnvironmentWidgetConfig::onUpdateModels()
{
  onUpdateSceneGraphModel();
  onUpdateCurrentStateModel();
  onUpdateAllowedCollisionMatrixModel();
  onUpdateKinematicsInformationModels();
  onUpdateCommandHistoryModel();

  emit modelsUpdated();
}

void EnvironmentWidgetConfig::onUpdateSceneGraphModel()
{
  if (!data_->environment->isInitialized())
    return;

  data_->scene_model.clear();
  data_->scene_model.setColumnCount(2);
  data_->scene_model.setHorizontalHeaderLabels({"Name", "Values"});
  auto* scene_item = new tesseract_gui::SceneGraphStandardItem(data_->environment->getSceneGraph()->clone());
  data_->scene_model.appendRow(scene_item);
}
void EnvironmentWidgetConfig::onUpdateCurrentStateModel()
{
  if (!data_->environment->isInitialized())
    return;

  data_->scene_state_model.clear();
  data_->scene_state_model.setColumnCount(2);
  data_->scene_state_model.setHorizontalHeaderLabels({"Name", "Values"});
  auto* state_item = new tesseract_gui::SceneStateStandardItem(data_->environment->getState());
  data_->scene_state_model.appendRow(state_item);
}
void EnvironmentWidgetConfig::onUpdateAllowedCollisionMatrixModel()
{
  if (!data_->environment->isInitialized())
    return;

  data_->acm_model.setAllowedCollisionMatrix(*data_->environment->getAllowedCollisionMatrix());
}

void EnvironmentWidgetConfig::onUpdateKinematicsInformationModels()
{
  if (!data_->environment->isInitialized())
    return;

  // Kinematic Groups
  auto kin_info = data_->environment->getKinematicsInformation();
  data_->group_model.set(kin_info.chain_groups, kin_info.joint_groups, kin_info.link_groups);

  // Groups States
  data_->group_states_model.set(kin_info.group_states);

  // Tool Center Points
  data_->group_tcps_model.set(kin_info.group_tcps);
}

void EnvironmentWidgetConfig::onUpdateCommandHistoryModel()
{
  if (!data_->environment->isInitialized())
    return;

  data_->commands_model.set(data_->environment->getCommandHistory());
}

void EnvironmentWidgetConfig::tesseractEventFilter(const tesseract_environment::Event& event)
{
  switch (event.type)
  {
    case tesseract_environment::Events::COMMAND_APPLIED:
    {
      onUpdateModels();
      emit environmentChanged(*data_->environment);
      break;
    }
    case tesseract_environment::Events::SCENE_STATE_CHANGED:
    {
      onUpdateCurrentStateModel();
      emit environmentCurrentStateChanged(*data_->environment);
      break;
    }
  }
}

EnvironmentWidgetConfig::UPtr EnvironmentWidgetConfig::clone() const
{
  auto c = std::make_unique<EnvironmentWidgetConfig>();
  c->setEnvironment(data_->environment->clone());
  return c;
}

}
