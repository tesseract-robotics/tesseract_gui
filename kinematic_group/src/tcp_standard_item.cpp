#include <tesseract_gui/kinematic_group/tcp_standard_item.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, CUBE_ICON, (":/tesseract_gui/png/cube.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, TEXT_ICON, (":/tesseract_gui/png/text.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, NUMERIC_ICON, (":/tesseract_gui/png/numeric.png"));

namespace tesseract_gui
{
TCPStandardItem::TCPStandardItem(QString name, const Eigen::Isometry3d& tcp)
  : QStandardItem(*CUBE_ICON(), "TCP")
  , name(std::move(name))
  , tcp(tcp)
{
  ctor();
}

TCPStandardItem::TCPStandardItem(const QString &text, QString name, const Eigen::Isometry3d& tcp)
  : QStandardItem(*CUBE_ICON(), text)
  , name(std::move(name))
  , tcp(tcp)
{
  ctor();
}

TCPStandardItem::TCPStandardItem(const QIcon &icon, const QString &text, QString name, const Eigen::Isometry3d& tcp)
  : QStandardItem(icon, text)
  , name(std::move(name))
  , tcp(tcp)
{
  ctor();
}

int TCPStandardItem::type() const
{
  return static_cast<int>(StandardItemType::TCP);
}

void TCPStandardItem::ctor()
{
  {
    auto* n = new QStandardItem(*TEXT_ICON(), "name");
    auto* value = new QStandardItem(name);
    appendRow({n, value});
  }

  appendRow(new OriginStandardItem("Pose", tcp));
}
}

