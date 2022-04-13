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
#include <tesseract_widgets/common/transform_standard_item.h>
#include <tesseract_widgets/common/standard_item_utils.h>
#include <tesseract_widgets/common/standard_item_type.h>

namespace tesseract_gui
{
TransformStandardItem::TransformStandardItem(const Eigen::Isometry3d& transform)
  : QStandardItem(QIcon(":/tesseract_widgets/png/origin.png"), "Transform")
  , transform(transform)
{
  ctor();
}

TransformStandardItem::TransformStandardItem(const QString &text, const Eigen::Isometry3d& transform)
  : QStandardItem(QIcon(":/tesseract_widgets/png/origin.png"), text)
  , transform(transform)
{
  ctor();
}

TransformStandardItem::TransformStandardItem(const QIcon &icon, const QString &text, const Eigen::Isometry3d& transform)
  : QStandardItem(icon, text)
  , transform(transform)
{
  ctor();
}

int TransformStandardItem::type() const
{
  return static_cast<int>(StandardItemType::TRANSFORM);
}

void TransformStandardItem::ctor()
{

  auto* position_item = new QStandardItem(QIcon(":/tesseract_widgets/png/position.png"), "position");
  position_item->setColumnCount(2);

  position_item->appendRow(createStandardItemFloat("x", transform.translation().x()));
  position_item->appendRow(createStandardItemFloat("y", transform.translation().y()));
  position_item->appendRow(createStandardItemFloat("z", transform.translation().z()));

  appendRow(position_item);

  auto* orientation_item = new QStandardItem(QIcon(":/tesseract_widgets/png/orientation.png"), "orientation");
  orientation_item->setColumnCount(2);

  Eigen::Quaterniond q(transform.rotation());

  orientation_item->appendRow(createStandardItemFloat( "x", q.x()));
  orientation_item->appendRow(createStandardItemFloat( "y", q.y()));
  orientation_item->appendRow(createStandardItemFloat( "z", q.z()));
  orientation_item->appendRow(createStandardItemFloat( "w", q.w()));

  appendRow(orientation_item);

}
}
