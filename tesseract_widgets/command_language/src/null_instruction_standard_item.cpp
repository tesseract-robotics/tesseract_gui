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
#include <tesseract_widgets/command_language/null_instruction_standard_item.h>
#include <tesseract_widgets/common/standard_item_type.h>
#include <tesseract_widgets/common/standard_item_utils.h>
#include <tesseract_widgets/common/icon_utils.h>

#include <tesseract_command_language/null_instruction.h>

namespace tesseract_gui
{
NullInstructionStandardItem::NullInstructionStandardItem(const tesseract_planning::NullInstruction& ni)
  : QStandardItem(icons::getUnknownIcon(), "Null Instruction")
{
  ctor(ni);
}

NullInstructionStandardItem::NullInstructionStandardItem(const QString& text,
                                                         const tesseract_planning::NullInstruction& ni)
  : QStandardItem(icons::getUnknownIcon(), text)
{
  ctor(ni);
}

NullInstructionStandardItem::NullInstructionStandardItem(const QIcon& icon,
                                                         const QString& text,
                                                         const tesseract_planning::NullInstruction& ni)
  : QStandardItem(icon, text)
{
  ctor(ni);
}

int NullInstructionStandardItem::type() const { return static_cast<int>(StandardItemType::CL_NULL_INSTRUCTION); }

void NullInstructionStandardItem::ctor(const tesseract_planning::NullInstruction& ni)
{
  appendRow(createStandardItemString("description", ni.getDescription()));
}
}  // namespace tesseract_gui