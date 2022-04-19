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
#include <tesseract_widgets/common/icon_utils.h>

namespace tesseract_gui
{
TransformStandardItem::TransformStandardItem(const Eigen::Isometry3d& transform)
  : QStandardItem(icons::getOriginIcon(), "Transform")
{
  ctor(transform);
}

TransformStandardItem::TransformStandardItem(const QString& text, const Eigen::Isometry3d& transform)
  : QStandardItem(icons::getOriginIcon(), text)
{
  ctor(transform);
}

TransformStandardItem::TransformStandardItem(const QIcon& icon, const QString& text, const Eigen::Isometry3d& transform)
  : QStandardItem(icon, text)
{
  ctor(transform);
}

int TransformStandardItem::type() const { return static_cast<int>(StandardItemType::TRANSFORM); }

void TransformStandardItem::setTransform(const Eigen::Isometry3d& transform)
{
  position_->child(0, 1)->setData(transform.translation().x(), Qt::DisplayRole);
  position_->child(1, 1)->setData(transform.translation().y(), Qt::DisplayRole);
  position_->child(2, 1)->setData(transform.translation().z(), Qt::DisplayRole);

  Eigen::Quaterniond q(transform.rotation());
  orientation_->child(0, 1)->setData(q.x(), Qt::DisplayRole);
  orientation_->child(1, 1)->setData(q.y(), Qt::DisplayRole);
  orientation_->child(2, 1)->setData(q.z(), Qt::DisplayRole);
  orientation_->child(3, 1)->setData(q.w(), Qt::DisplayRole);
}

void TransformStandardItem::ctor(const Eigen::Isometry3d& transform)
{
  position_ = new QStandardItem(icons::getPositionIcon(), "position");
  position_->setColumnCount(2);

  position_->appendRow(createStandardItemFloat("x", transform.translation().x()));
  position_->appendRow(createStandardItemFloat("y", transform.translation().y()));
  position_->appendRow(createStandardItemFloat("z", transform.translation().z()));

  appendRow(position_);

  orientation_ = new QStandardItem(icons::getOrientationIcon(), "orientation");
  orientation_->setColumnCount(2);

  Eigen::Quaterniond q(transform.rotation());

  orientation_->appendRow(createStandardItemFloat("x", q.x()));
  orientation_->appendRow(createStandardItemFloat("y", q.y()));
  orientation_->appendRow(createStandardItemFloat("z", q.z()));
  orientation_->appendRow(createStandardItemFloat("w", q.w()));

  appendRow(orientation_);
}
}  // namespace tesseract_gui
