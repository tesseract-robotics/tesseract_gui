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
#ifndef TESSERACT_WIDGETS_COMMON_STANDARD_ITEM_UTILS_H
#define TESSERACT_WIDGETS_COMMON_STANDARD_ITEM_UTILS_H

#include <array>
#include <QStandardItem>
#include <QList>
#include <QDateTime>

namespace tesseract_gui
{
QList<QStandardItem*> createStandardItemFloat(const std::string& text, double data);
QList<QStandardItem*> createStandardItemFloat(const QIcon& icon, const std::string& text, double data);
QList<QStandardItem*> createStandardItemInt(const std::string& text, int data);
QList<QStandardItem*> createStandardItemInt(const QIcon& icon, const std::string& text, int data);
QList<QStandardItem*> createStandardItemString(const std::string& text, const std::string& data);
QList<QStandardItem*> createStandardItemString(const QIcon& icon, const std::string& text, const std::string& data);
QList<QStandardItem*> createStandardItemURL(const std::string& text, const std::string& data);
QList<QStandardItem*> createStandardItemBool(const std::string& text, bool data);
QList<QStandardItem*> createStandardItemBool(const QIcon& icon, const std::string& text, bool data);
QList<QStandardItem*> createStandardItemDateTime(const std::string& text, const QDateTime& data);
QList<QStandardItem*> createStandardItemDateTime(const QIcon& icon, const std::string& text, const QDateTime& data);
}  // namespace tesseract_gui

#endif  // TESSERACT_WIDGETS_COMMON_STANDARD_ITEM_UTILS_H
