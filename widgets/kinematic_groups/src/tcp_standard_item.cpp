#include <tesseract_gui/widgets/kinematic_groups/tcp_standard_item.h>
#include <tesseract_gui/widgets/scene_graph/origin_standard_item.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, ORIGIN_ICON, (":/tesseract_gui/png/origin.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, POSITION_ICON, (":/tesseract_gui/png/position.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, ORIENTATION_ICON, (":/tesseract_gui/png/orientation.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, NUMERIC_ICON, (":/tesseract_gui/png/numeric.png"));

namespace tesseract_gui
{
TCPStandardItem::TCPStandardItem(const Eigen::Isometry3d& tcp)
  : QStandardItem(*ORIGIN_ICON(), "TCP")
{
  ctor(tcp);
}

TCPStandardItem::TCPStandardItem(const QString &text, const Eigen::Isometry3d& tcp)
  : QStandardItem(*ORIGIN_ICON(), text)
{
  ctor(tcp);
}

TCPStandardItem::TCPStandardItem(const QIcon &icon, const QString &text, const Eigen::Isometry3d& tcp)
  : QStandardItem(icon, text)
{
  ctor(tcp);
}

int TCPStandardItem::type() const
{
  return static_cast<int>(StandardItemType::TCP);
}

void TCPStandardItem::ctor(const Eigen::Isometry3d& tcp)
{
  {
    auto* position_item = new QStandardItem(*POSITION_ICON(), "position");
    position_item->setColumnCount(2);

    auto* x_name = new QStandardItem(*NUMERIC_ICON(), "x");
    auto* x_value = new QStandardItem(QString("%1").arg(tcp.translation().x()));
    position_item->appendRow({x_name, x_value});

    auto* y_name = new QStandardItem(*NUMERIC_ICON(), "y");
    auto* y_value = new QStandardItem(QString("%1").arg(tcp.translation().y()));
    position_item->appendRow({y_name, y_value});

    auto* z_name = new QStandardItem(*NUMERIC_ICON(), "z");
    auto* z_value = new QStandardItem(QString("%1").arg(tcp.translation().z()));
    position_item->appendRow({z_name, z_value});

    appendRow(position_item);
  }

  {
    auto* orientation_item = new QStandardItem(*ORIENTATION_ICON(), "orientation");
    orientation_item->setColumnCount(2);

    Eigen::Quaterniond q(tcp.rotation());

    auto* x_name = new QStandardItem(*NUMERIC_ICON(), "x");
    auto* x_value = new QStandardItem(QString("%1").arg(q.x()));
    orientation_item->appendRow({x_name, x_value});

    auto* y_name = new QStandardItem(*NUMERIC_ICON(), "y");
    auto* y_value = new QStandardItem(QString("%1").arg(q.y()));
    orientation_item->appendRow({y_name, y_value});

    auto* z_name = new QStandardItem(*NUMERIC_ICON(), "z");
    auto* z_value = new QStandardItem(QString("%1").arg(q.z()));
    orientation_item->appendRow({z_name, z_value});

    auto* w_name = new QStandardItem(*NUMERIC_ICON(), "w");
    auto* w_value = new QStandardItem(QString("%1").arg(q.w()));
    orientation_item->appendRow({w_name, w_value});

    appendRow(orientation_item);
  }
}
}

