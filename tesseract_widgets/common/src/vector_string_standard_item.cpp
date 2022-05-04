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
#include <tesseract_widgets/common/vector_string_standard_item.h>
#include <tesseract_widgets/common/standard_item_utils.h>
#include <tesseract_widgets/common/standard_item_type.h>
#include <tesseract_widgets/common/icon_utils.h>

namespace tesseract_gui
{
VectorStringStandardItem::VectorStringStandardItem(const std::vector<std::string>& vector_string)
  : QStandardItem(icons::getJointTrajectorySetIcon(), "Vector String")
{
  ctor(vector_string);
}

VectorStringStandardItem::VectorStringStandardItem(const QString& text, const std::vector<std::string>& vector_string)
  : QStandardItem(icons::getJointTrajectorySetIcon(), text)
{
  ctor(vector_string);
}

VectorStringStandardItem::VectorStringStandardItem(const QIcon& icon,
                                                   const QString& text,
                                                   const std::vector<std::string>& vector_string)
  : QStandardItem(icon, text)
{
  ctor(vector_string);
}

int VectorStringStandardItem::type() const { return static_cast<int>(StandardItemType::VECTOR_STRING); }

void VectorStringStandardItem::ctor(const std::vector<std::string>& vector_string)
{
  for (std::size_t i = 0; i < vector_string.size(); ++i)
    appendRow(createStandardItemString(QString("[%1]").arg(i).toStdString(), vector_string[i]));
}
}  // namespace tesseract_gui