#include <tesseract_widgets/scene_graph/link_standard_item.h>
#include <tesseract_widgets/scene_graph/inertial_standard_item.h>
#include <tesseract_widgets/scene_graph/visual_standard_item.h>
#include <tesseract_widgets/scene_graph/collision_standard_item.h>
#include <tesseract_widgets/common/standard_item_utils.h>
#include <tesseract_widgets/common/standard_item_type.h>

namespace tesseract_gui
{
LinkStandardItem::LinkStandardItem(tesseract_scene_graph::Link::Ptr link)
  : QStandardItem(QIcon(":/tesseract_widgets/ignition/link.png"), "Link")
  , link(std::move(link))
{
  ctor();
}

LinkStandardItem::LinkStandardItem(const QString &text, tesseract_scene_graph::Link::Ptr link)
  : QStandardItem(QIcon(":/tesseract_widgets/ignition/link.png"), text)
  , link(std::move(link))
{
  ctor();
}

LinkStandardItem::LinkStandardItem(const QIcon &icon, const QString &text, tesseract_scene_graph::Link::Ptr link)
  : QStandardItem(icon, text)
  , link(std::move(link))
{
  ctor();
}

int LinkStandardItem::type() const
{
  return static_cast<int>(StandardItemType::LINK);
}

void LinkStandardItem::ctor()
{
  appendRow(createStandardItemString("name", link->getName()));

  if (link->inertial != nullptr)
    appendRow(new InertialStandardItem(link->inertial));

  if (!link->visual.empty())
  {
    auto* visuals_item = new QStandardItem(QIcon(":/tesseract_widgets/ignition/visual_vector.png"), "Visual");
    for (std::size_t i = 0; i < link->visual.size(); ++i)
      visuals_item->appendRow(new VisualStandardItem(QString("[%1]").arg(i), link->visual.at(i)));

    appendRow(visuals_item);
  }

  if (!link->collision.empty())
  {
    auto* collisions_item = new QStandardItem(QIcon(":/tesseract_widgets/ignition/collision_vector.png"), "Collision");
    for (std::size_t i = 0; i < link->collision.size(); ++i)
      collisions_item->appendRow(new CollisionStandardItem(QString("[%1]").arg(i), link->collision.at(i)));

    appendRow(collisions_item);
  }
}
}
