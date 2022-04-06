#include <tesseract_gui/widgets/common/standard_item_utils.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, NUMERIC_ICON, (":/tesseract_gui/png/numeric.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, TEXT_ICON, (":/tesseract_gui/png/text.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, URL_ICON, (":/tesseract_gui/png/url.png"));

namespace tesseract_gui
{
QList<QStandardItem*> createStandardItemFloat(const std::string& text, double data)
{
  auto* name = new QStandardItem(*NUMERIC_ICON(), QString::fromStdString(text));
  auto* value = new QStandardItem();
  value->setData(data, Qt::DisplayRole);
  return {name, value};
}

QList<QStandardItem*> createStandardItemInt(const std::string& text, int data)
{
  auto* name = new QStandardItem(*NUMERIC_ICON(), QString::fromStdString(text));
  auto* value = new QStandardItem();
  value->setData(data, Qt::DisplayRole);
  return {name, value};
}

QList<QStandardItem*> createStandardItemString(const std::string& text, const std::string& data)
{
  return createStandardItemString(*TEXT_ICON(), text, data);
}

QList<QStandardItem*> createStandardItemString(const QIcon &icon, const std::string& text, const std::string& data)
{
  auto* name = new QStandardItem(icon, QString::fromStdString(text));
  auto* value = new QStandardItem();
  value->setData(QString::fromStdString(data), Qt::DisplayRole);
  return {name, value};
}

QList<QStandardItem*> createStandardItemURL(const std::string& text, const std::string& data)
{
  auto* name = new QStandardItem(*URL_ICON(), QString::fromStdString(text));
  auto* value = new QStandardItem();
  value->setData(QString::fromStdString(data), Qt::DisplayRole);
  return {name, value};
}
}
