#include <tesseract_gui/widgets/common/transform_standard_item.h>
#include <tesseract_gui/widgets/common/standard_item_utils.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, ORIGIN_ICON, (":/tesseract_gui/png/origin.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, POSITION_ICON, (":/tesseract_gui/png/position.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, ORIENTATION_ICON, (":/tesseract_gui/png/orientation.png"));

namespace tesseract_gui
{
TransformStandardItem::TransformStandardItem(const Eigen::Isometry3d& transform)
  : QStandardItem(*ORIGIN_ICON(), "Transform")
  , transform(transform)
{
  ctor();
}

TransformStandardItem::TransformStandardItem(const QString &text, const Eigen::Isometry3d& transform)
  : QStandardItem(*ORIGIN_ICON(), text)
  , transform(transform)
{
  ctor();
}

TransformStandardItem::TransformStandardItem(const QIcon &icon, const QString &text, const Eigen::Isometry3d& transform)
  : QStandardItem(icon, text)
  , transform(transform)
{
  ctor();
}

int TransformStandardItem::type() const
{
  return static_cast<int>(StandardItemType::TRANSFORM);
}

void TransformStandardItem::ctor()
{
  {
    auto* position_item = new QStandardItem(*POSITION_ICON(), "position");
    position_item->setColumnCount(2);

    position_item->appendRow(createStandardItemFloat("x", transform.translation().x()));
    position_item->appendRow(createStandardItemFloat("y", transform.translation().y()));
    position_item->appendRow(createStandardItemFloat("z", transform.translation().z()));

    appendRow(position_item);
  }

  {
    auto* orientation_item = new QStandardItem(*ORIENTATION_ICON(), "orientation");
    orientation_item->setColumnCount(2);

    Eigen::Quaterniond q(transform.rotation());

    orientation_item->appendRow(createStandardItemFloat( "x", q.x()));
    orientation_item->appendRow(createStandardItemFloat( "y", q.y()));
    orientation_item->appendRow(createStandardItemFloat( "z", q.z()));
    orientation_item->appendRow(createStandardItemFloat( "w", q.w()));

    appendRow(orientation_item);
  }
}
}
