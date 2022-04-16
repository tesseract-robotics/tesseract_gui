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
#include <tesseract_widgets/scene_graph/scene_graph_standard_item.h>
#include <tesseract_widgets/scene_graph/link_standard_item.h>
#include <tesseract_widgets/scene_graph/joint_standard_item.h>
#include <tesseract_widgets/common/standard_item_utils.h>
#include <tesseract_widgets/common/standard_item_type.h>
#include <tesseract_widgets/common/icon_utils.h>

namespace tesseract_gui
{
SceneGraphStandardItem::SceneGraphStandardItem(tesseract_scene_graph::SceneGraph::UPtr scene_graph)
  : QStandardItem(icons::getModelIcon(), "SceneGraph")
  , scene_graph(std::move(scene_graph))
{
  ctor();
}

SceneGraphStandardItem::SceneGraphStandardItem(const QString &text, tesseract_scene_graph::SceneGraph::UPtr scene_graph)
  : QStandardItem(icons::getModelIcon(), text)
  , scene_graph(std::move(scene_graph))
{
  ctor();
}

SceneGraphStandardItem::SceneGraphStandardItem(const QIcon &icon, const QString &text, tesseract_scene_graph::SceneGraph::UPtr scene_graph)
  : QStandardItem(icon, text)
  , scene_graph(std::move(scene_graph))
{
  ctor();
}

int SceneGraphStandardItem::type() const
{
  return static_cast<int>(StandardItemType::SCENE_GRAPH);
}

void SceneGraphStandardItem::ctor()
{
  appendRow(createStandardItemString("name", scene_graph->getName()));
  appendRow(createStandardItemString("root_link", scene_graph->getRoot()));

  auto* links_item = new QStandardItem(icons::getLinkVectorIcon(), "Links");
  for (const auto& link : scene_graph->getLinks())
  {
    auto* item = new LinkStandardItem(QString::fromStdString(link->getName()), std::make_shared<tesseract_scene_graph::Link>(link->clone()));
    links_item->appendRow(item);
  }
  appendRow(links_item);

  auto* joints_item = new QStandardItem(icons::getJointVectorIcon(), "Joints");
  for (const auto& joint : scene_graph->getJoints())
  {
    auto* item = new JointStandardItem(QString::fromStdString(joint->getName()), std::make_shared<tesseract_scene_graph::Joint>(joint->clone()));
    joints_item->appendRow(item);
  }
  appendRow(joints_item);
}
}
