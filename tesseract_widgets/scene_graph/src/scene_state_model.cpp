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
#include <tesseract_widgets/scene_graph/scene_state_model.h>
#include <tesseract_widgets/common/transform_standard_item.h>
#include <tesseract_widgets/common/standard_item_utils.h>
#include <tesseract_widgets/common/standard_item_type.h>
#include <tesseract_widgets/common/icon_utils.h>

#include <tesseract_scene_graph/scene_state.h>

namespace tesseract_gui
{
struct SceneStateModelPrivate
{
  std::vector<std::string> link_names;
  std::vector<std::string> joint_names;

  QStandardItem* values_item;
  QStandardItem* links_item;
  QStandardItem* joints_item;

  std::unordered_map<std::string, QStandardItem*> values;
  std::unordered_map<std::string, TransformStandardItem*> links;
  std::unordered_map<std::string, TransformStandardItem*> joints;

  void clear()
  {
    link_names.clear();
    joint_names.clear();
    values_item = nullptr;
    links_item = nullptr;
    joints_item = nullptr;
    values.clear();
    links.clear();
    joints.clear();
  }
};

SceneStateModel::SceneStateModel(QObject* parent)
  : QStandardItemModel(parent), data_(std::make_unique<SceneStateModelPrivate>())
{
  clear();
}

SceneStateModel::~SceneStateModel() = default;

SceneStateModel::SceneStateModel(const SceneStateModel& other) : QStandardItemModel(other.d_ptr->parent) {}

SceneStateModel& SceneStateModel::operator=(const SceneStateModel& other) { return *this; }

void SceneStateModel::setState(const tesseract_scene_graph::SceneState& scene_state)
{
  bool sort_required{ false };
  for (const auto& joint : scene_state.joints)
  {
    auto it = data_->values.find(joint.first);
    if (it != data_->values.end())
    {
      it->second->setData(joint.second, Qt::DisplayRole);
    }
    else
    {
      auto row = createStandardItemFloat(joint.first, joint.second);
      data_->values[joint.first] = row[1];
      data_->values_item->appendRow(row);
      sort_required = true;
    }
  }

  std::vector<std::string> link_names;
  link_names.reserve(scene_state.link_transforms.size());
  for (const auto& link : scene_state.link_transforms)
  {
    auto it = data_->links.find(link.first);
    if (it != data_->links.end())
    {
      it->second->setTransform(link.second);
    }
    else
    {
      auto* item = new TransformStandardItem(QString::fromStdString(link.first), link.second);
      data_->links_item->appendRow(item);
      data_->links[link.first] = item;
      data_->link_names.push_back(link.first);
      sort_required = true;
    }
    link_names.push_back(link.first);
  }

  std::vector<std::string> joint_names;
  joint_names.reserve(scene_state.joint_transforms.size());
  for (const auto& joint : scene_state.joint_transforms)
  {
    auto it = data_->joints.find(joint.first);
    if (it != data_->joints.end())
    {
      it->second->setTransform(joint.second);
    }
    else
    {
      auto* item = new TransformStandardItem(QString::fromStdString(joint.first), joint.second);
      data_->joints_item->appendRow(item);
      data_->joints[joint.first] = item;
      data_->joint_names.push_back(joint.first);
      sort_required = true;
    }
    joint_names.push_back(joint.first);
  }

  for (const auto& link_name : data_->link_names)
  {
    auto it = scene_state.link_transforms.find(link_name);
    if (it == scene_state.link_transforms.end())
    {
      QModelIndex idx = indexFromItem(data_->links[link_name]);
      removeRow(idx.row());
      data_->links.erase(link_name);
      sort_required = true;
    }
  }
  data_->link_names = link_names;

  for (const auto& joint_name : data_->joint_names)
  {
    auto it = scene_state.joint_transforms.find(joint_name);
    if (it == scene_state.joint_transforms.end())
    {
      QModelIndex joint_idx = indexFromItem(data_->joints[joint_name]);
      removeRow(joint_idx.row());

      QModelIndex value_idx = indexFromItem(data_->values[joint_name]);
      removeRow(value_idx.row());

      data_->joints.erase(joint_name);
      data_->values.erase(joint_name);
      sort_required = true;
    }
  }
  data_->link_names = joint_names;

  if (sort_required)
  {
    data_->values_item->sortChildren(0);
    data_->links_item->sortChildren(0);
    data_->joints_item->sortChildren(0);
  }
}

void SceneStateModel::clear()
{
  QStandardItemModel::clear();
  data_->clear();

  setColumnCount(2);
  setHorizontalHeaderLabels({ "Name", "Values" });

  data_->values_item = new QStandardItem(icons::getJointVectorIcon(), "Values");
  data_->links_item = new QStandardItem(icons::getLinkVectorIcon(), "Links");
  data_->joints_item = new QStandardItem(icons::getJointVectorIcon(), "Joints");

  appendRow(data_->values_item);
  appendRow(data_->links_item);
  appendRow(data_->joints_item);
}

}  // namespace tesseract_gui
