#include <tesseract_gui/widgets/common/rgba_standard_item.h>
#include <tesseract_gui/widgets/common/standard_item_utils.h>
#include <tesseract_gui/common/standard_item_type.h>

namespace tesseract_gui
{
RGBAStandardItem::RGBAStandardItem(const Eigen::Vector4d& rgba)
  : QStandardItem(QIcon(":/tesseract_gui/png/rgb.png"), "Color")
  , rgba(rgba)
{
  ctor();
}

RGBAStandardItem::RGBAStandardItem(const QString &text, const Eigen::Vector4d& rgba)
  : QStandardItem(QIcon(":/tesseract_gui/png/rgb.png"), text)
  , rgba(rgba)
{
  ctor();
}

RGBAStandardItem::RGBAStandardItem(const QIcon &icon, const QString &text, const Eigen::Vector4d& rgba)
  : QStandardItem(icon, text)
  , rgba(rgba)
{
  ctor();
}

int RGBAStandardItem::type() const
{
  return static_cast<int>(StandardItemType::TRANSFORM);
}

void RGBAStandardItem::ctor()
{
  appendRow(createStandardItemFloat("r", rgba(0)));
  appendRow(createStandardItemFloat("g", rgba(1)));
  appendRow(createStandardItemFloat("b", rgba(2)));
  appendRow(createStandardItemFloat("a", rgba(3)));
}
}
