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
#include <tesseract_widgets/scene_graph/scene_state_standard_item.h>
#include <tesseract_widgets/common/transform_standard_item.h>
#include <tesseract_widgets/common/standard_item_utils.h>
#include <tesseract_widgets/common/standard_item_type.h>

namespace tesseract_gui
{
SceneStateStandardItem::SceneStateStandardItem(tesseract_scene_graph::SceneState scene_state)
  : QStandardItem(QIcon(":/tesseract_widgets/ignition/model.png"), "Scene State")
  , scene_state(std::move(scene_state))
{
  ctor();
}

SceneStateStandardItem::SceneStateStandardItem(const QString &text, tesseract_scene_graph::SceneState scene_state)
  : QStandardItem(QIcon(":/tesseract_widgets/ignition/model.png"), text)
  , scene_state(std::move(scene_state))
{
  ctor();
}

SceneStateStandardItem::SceneStateStandardItem(const QIcon &icon, const QString &text, tesseract_scene_graph::SceneState scene_state)
  : QStandardItem(icon, text)
  , scene_state(std::move(scene_state))
{
  ctor();
}

int SceneStateStandardItem::type() const
{
  return static_cast<int>(StandardItemType::SCENE_STATE);
}

void SceneStateStandardItem::ctor()
{
  auto* values_item = new QStandardItem(QIcon(":/tesseract_widgets/ignition/joint_vector.png"), "Values");
  for (auto& joint : scene_state.joints)
    values_item->appendRow(createStandardItemFloat(joint.first, joint.second));

  values_item->sortChildren(0);
  appendRow(values_item);

  auto* links_item = new QStandardItem(QIcon(":/tesseract_widgets/ignition/link_vector.png"), "Links");
  for (auto& link : scene_state.link_transforms)
  {
    auto* item = new TransformStandardItem(QString::fromStdString(link.first), link.second);
    links_item->appendRow(item);
  }
  links_item->sortChildren(0);
  appendRow(links_item);

  auto* joints_item = new QStandardItem(QIcon(":/tesseract_widgets/ignition/joint_vector.png"), "Joints");
  for (auto& joint : scene_state.joint_transforms)
  {
    auto* item = new TransformStandardItem(QString::fromStdString(joint.first), joint.second);
    joints_item->appendRow(item);
  }
  joints_item->sortChildren(0);
  appendRow(joints_item);
}
}
