#ifndef TESSERACT_GUI_WIDGETS_COMMON_STANDARD_ITEM_UTILS_H
#define TESSERACT_GUI_WIDGETS_COMMON_STANDARD_ITEM_UTILS_H

#include <array>
#include <QStandardItem>
#include <QList>

namespace tesseract_gui
{
  QList<QStandardItem*> createStandardItemFloat(const std::string& text, double data);
  QList<QStandardItem*> createStandardItemInt(const std::string& text, int data);
  QList<QStandardItem*> createStandardItemString(const std::string& text, const std::string& data);
  QList<QStandardItem*> createStandardItemString(const QIcon &icon, const std::string& text, const std::string& data);
  QList<QStandardItem*> createStandardItemURL(const std::string& text, const std::string& data);
}

#endif // TESSERACT_GUI_WIDGETS_COMMON_STANDARD_ITEM_UTILS_H
