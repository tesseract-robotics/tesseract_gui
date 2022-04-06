#include <tesseract_gui/widgets/common/standard_item_utils.h>

namespace tesseract_gui
{
QList<QStandardItem*> createStandardItemFloat(const std::string& text, double data)
{
  return createStandardItemFloat(QIcon(":/tesseract_gui/png/numeric.png"), text, data);
}

QList<QStandardItem*> createStandardItemFloat(const QIcon &icon, const std::string& text, double data)
{
  auto* name = new QStandardItem(icon, QString::fromStdString(text));
  auto* value = new QStandardItem(); // NOLINT
  value->setData(data, Qt::DisplayRole);
  return {name, value};
}

QList<QStandardItem*> createStandardItemInt(const std::string& text, int data)
{
  return createStandardItemInt(QIcon(":/tesseract_gui/png/numeric.png"), text, data);
}

QList<QStandardItem*> createStandardItemInt(const QIcon &icon, const std::string& text, int data)
{
  auto* name = new QStandardItem(icon, QString::fromStdString(text));
  auto* value = new QStandardItem(); // NOLINT
  value->setData(data, Qt::DisplayRole);
  return {name, value};
}

QList<QStandardItem*> createStandardItemString(const std::string& text, const std::string& data)
{
  return createStandardItemString(QIcon(":/tesseract_gui/png/text.png"), text, data);
}

QList<QStandardItem*> createStandardItemString(const QIcon &icon, const std::string& text, const std::string& data)
{
  auto* name = new QStandardItem(icon, QString::fromStdString(text));
  auto* value = new QStandardItem(); // NOLINT
  value->setData(QString::fromStdString(data), Qt::DisplayRole);
  return {name, value};
}

QList<QStandardItem*> createStandardItemURL(const std::string& text, const std::string& data)
{
  auto* name = new QStandardItem(QIcon(":/tesseract_gui/png/url.png"), QString::fromStdString(text));
  auto* value = new QStandardItem(); // NOLINT
  value->setData(QString::fromStdString(data), Qt::DisplayRole);
  return {name, value};
}
}
