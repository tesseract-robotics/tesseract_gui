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
#include <tesseract_widgets/scene_graph/capsule_standard_item.h>
#include <tesseract_widgets/common/standard_item_utils.h>
#include <tesseract_widgets/common/standard_item_type.h>
#include <tesseract_widgets/common/icon_utils.h>

namespace tesseract_gui
{
CapsuleStandardItem::CapsuleStandardItem(tesseract_geometry::Capsule::Ptr capsule)
  : QStandardItem(icons::getCapsuleIcon(), "Capsule")
  , capsule(std::move(capsule))
{
  ctor();
}

CapsuleStandardItem::CapsuleStandardItem(const QString &text, tesseract_geometry::Capsule::Ptr capsule)
  : QStandardItem(icons::getCapsuleIcon(), text)
  , capsule(std::move(capsule))
{
  ctor();
}

CapsuleStandardItem::CapsuleStandardItem(const QIcon &icon, const QString &text, tesseract_geometry::Capsule::Ptr capsule)
  : QStandardItem(icon, text)
  , capsule(std::move(capsule))
{
  ctor();
}

int CapsuleStandardItem::type() const
{
  return static_cast<int>(StandardItemType::CAPSULE);
}

void CapsuleStandardItem::ctor()
{
  appendRow(createStandardItemFloat("radius", capsule->getRadius()));
  appendRow(createStandardItemFloat("length", capsule->getLength()));
}
}

