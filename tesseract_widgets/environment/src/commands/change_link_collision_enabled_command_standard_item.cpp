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
#include <tesseract_widgets/environment/commands/change_link_collision_enabled_command_standard_item.h>
#include <tesseract_widgets/common/standard_item_utils.h>
#include <tesseract_widgets/common/standard_item_type.h>

namespace tesseract_gui
{

ChangeLinkCollisionEnabledCommandStandardItem::ChangeLinkCollisionEnabledCommandStandardItem(tesseract_environment::ChangeLinkCollisionEnabledCommand::ConstPtr command)
  : QStandardItem(QIcon(":/tesseract_widgets/png/merge.png"), "Change Link Collision Enabled")
  , command(std::move(command))
{
  ctor();
}

ChangeLinkCollisionEnabledCommandStandardItem::ChangeLinkCollisionEnabledCommandStandardItem(const QString &text, tesseract_environment::ChangeLinkCollisionEnabledCommand::ConstPtr command)
  : QStandardItem(QIcon(":/tesseract_widgets/png/merge.png"), text)
  , command(std::move(command))
{
  ctor();
}

ChangeLinkCollisionEnabledCommandStandardItem::ChangeLinkCollisionEnabledCommandStandardItem(const QIcon &icon, const QString &text, tesseract_environment::ChangeLinkCollisionEnabledCommand::ConstPtr command)
  : QStandardItem(icon, text)
  , command(std::move(command))
{
  ctor();
}

int ChangeLinkCollisionEnabledCommandStandardItem::type() const
{
  return static_cast<int>(StandardItemType::ENV_COMMAND_CHANGE_LINK_COLLISION_ENABLED);
}

void ChangeLinkCollisionEnabledCommandStandardItem::ctor()
{
  appendRow(createStandardItemString(QIcon(":/tesseract_widgets/ignition/link.png"), command->getLinkName(), (command->getEnabled()) ? "True" : "False"));
}
}
