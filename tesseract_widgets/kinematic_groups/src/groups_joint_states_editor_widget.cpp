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
#include <tesseract_widgets/kinematic_groups/groups_joint_states_editor_widget.h>
#include <tesseract_widgets/common/standard_item_type.h>
#include "ui_groups_joint_states_editor_widget.h"

#include <QtQuickWidgets/QQuickWidget>
namespace tesseract_gui
{
GroupsJointStatesEditorWidget::GroupsJointStatesEditorWidget(QStringListModel* group_names_model,
                                                             GroupJointsRetriever group_joints_retriever,
                                                             QWidget* parent)
  : QWidget(parent)
  , ui_(std::make_unique<Ui::GroupsJointStatesEditorWidget>())
  , group_joints_retriever_(group_joints_retriever)
{
  if (group_names_model == nullptr)
    throw std::runtime_error("GroupsJointStatesEditorWidget, group name model cannot be a nullptr!");

  if (group_joints_retriever == nullptr)
    throw std::runtime_error("GroupsJointStatesEditorWidget, group joints retriever cannot be a nullptr!");

  ui_->setupUi(this);
  ui_->groupNamesComboBox->setModel(group_names_model);

  connect(ui_->groupNamesComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(onGroupNameChanged()));
  connect(ui_->addPushButton, SIGNAL(clicked()), this, SLOT(onAddJointState()));
  connect(ui_->removePushButton, SIGNAL(clicked()), this, SLOT(onRemoveJointState()));
  connect(ui_->jointSliderWidget,
          &tesseract_gui::JointStateSliderWidget::jointStateChanged,
          this,
          &tesseract_gui::GroupsJointStatesEditorWidget::jointStateChanged);

  onGroupNameChanged();
}

GroupsJointStatesEditorWidget::~GroupsJointStatesEditorWidget() = default;

void GroupsJointStatesEditorWidget::setModel(tesseract_gui::GroupJointStatesModel* model)
{
  model_ = model;
  ui_->treeView->setModel(model_);
}

void GroupsJointStatesEditorWidget::onGroupNameChanged()
{
  ui_->jointSliderWidget->setJoints(group_joints_retriever_(ui_->groupNamesComboBox->currentText()));
}

void GroupsJointStatesEditorWidget::onAddJointState()
{
  QString group_name = ui_->groupNamesComboBox->currentText();
  QString state_name = ui_->jointStateNameLineEdit->text();
  if (state_name.isEmpty())
    return;

  model_->addGroupJointState(group_name, state_name, ui_->jointSliderWidget->getJointState());
  ui_->jointStateNameLineEdit->clear();
}

void GroupsJointStatesEditorWidget::onRemoveJointState()
{
  QModelIndexList selection = ui_->treeView->selectionModel()->selectedIndexes();
  int row_cnt = selection.count();
  for (int i = row_cnt; i > 0; i--)
  {
    QStandardItem* item = model_->itemFromIndex(selection.at(i - 1));
    if (item->type() == static_cast<int>(StandardItemType::JOINT_STATE))
      model_->removeGroupJointState(item->parent()->text(), item->text());
  }
}

}  // namespace tesseract_gui
