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
#include <tesseract_widgets/scene_graph/link_standard_item.h>
#include <tesseract_widgets/scene_graph/inertial_standard_item.h>
#include <tesseract_widgets/scene_graph/visual_standard_item.h>
#include <tesseract_widgets/scene_graph/collision_standard_item.h>
#include <tesseract_widgets/common/standard_item_utils.h>
#include <tesseract_widgets/common/standard_item_type.h>

namespace tesseract_gui
{
LinkStandardItem::LinkStandardItem(tesseract_scene_graph::Link::Ptr link)
  : QStandardItem(QIcon(":/tesseract_widgets/ignition/link.png"), "Link")
  , link(std::move(link))
{
  ctor();
}

LinkStandardItem::LinkStandardItem(const QString &text, tesseract_scene_graph::Link::Ptr link)
  : QStandardItem(QIcon(":/tesseract_widgets/ignition/link.png"), text)
  , link(std::move(link))
{
  ctor();
}

LinkStandardItem::LinkStandardItem(const QIcon &icon, const QString &text, tesseract_scene_graph::Link::Ptr link)
  : QStandardItem(icon, text)
  , link(std::move(link))
{
  ctor();
}

int LinkStandardItem::type() const
{
  return static_cast<int>(StandardItemType::LINK);
}

void LinkStandardItem::ctor()
{
  appendRow(createStandardItemString("name", link->getName()));

  if (link->inertial != nullptr)
    appendRow(new InertialStandardItem(link->inertial));

  if (!link->visual.empty())
  {
    auto* visuals_item = new QStandardItem(QIcon(":/tesseract_widgets/ignition/visual_vector.png"), "Visual");
    for (std::size_t i = 0; i < link->visual.size(); ++i)
      visuals_item->appendRow(new VisualStandardItem(QString("[%1]").arg(i), link->visual.at(i)));

    appendRow(visuals_item);
  }

  if (!link->collision.empty())
  {
    auto* collisions_item = new QStandardItem(QIcon(":/tesseract_widgets/ignition/collision_vector.png"), "Collision");
    for (std::size_t i = 0; i < link->collision.size(); ++i)
      collisions_item->appendRow(new CollisionStandardItem(QString("[%1]").arg(i), link->collision.at(i)));

    appendRow(collisions_item);
  }
}
}
