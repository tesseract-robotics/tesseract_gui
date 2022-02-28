#include <tesseract_gui/scene_graph/link_standard_item.h>
#include <tesseract_gui/scene_graph/inertial_standard_item.h>
#include <tesseract_gui/scene_graph/visual_standard_item.h>
#include <tesseract_gui/scene_graph/collision_standard_item.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, LINK_ICON, (":/tesseract_gui/ignition/link.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, TEXT_ICON, (":/tesseract_gui/png/text.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, VISUAL_VECTOR_ICON, (":/tesseract_gui/ignition/visual_vector.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, COLLISION_VECTOR_ICON, (":/tesseract_gui/ignition/collision_vector.png"));

namespace tesseract_gui
{
LinkStandardItem::LinkStandardItem(tesseract_scene_graph::Link::Ptr link)
  : QStandardItem(*LINK_ICON(), "Link")
  , link(std::move(link))
{
  ctor();
}

LinkStandardItem::LinkStandardItem(const QString &text, tesseract_scene_graph::Link::Ptr link)
  : QStandardItem(*LINK_ICON(), text)
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
  auto* name_item = new QStandardItem(*TEXT_ICON(), "name");
  auto* name_value = new QStandardItem(QString::fromStdString(link->getName()));
  appendRow({name_item, name_value});

  if (link->inertial != nullptr)
  {
    auto* inertial_item = new InertialStandardItem(link->inertial);
    appendRow(inertial_item);
  }

  if (!link->visual.empty())
  {
    auto* visuals_item = new QStandardItem(*VISUAL_VECTOR_ICON(), "Visual");
    for (std::size_t i = 0; i < link->visual.size(); ++i)
    {
      auto* visual_item = new VisualStandardItem(QString("[%1]").arg(i), link->visual.at(i));
      visuals_item->appendRow(visual_item);
    }
    appendRow(visuals_item);
  }

  if (!link->collision.empty())
  {
    auto* collisions_item = new QStandardItem(*COLLISION_VECTOR_ICON(), "Collision");
    for (std::size_t i = 0; i < link->collision.size(); ++i)
    {
      auto* collision_item = new CollisionStandardItem(QString("[%1]").arg(i), link->collision.at(i));
      collisions_item->appendRow(collision_item);
    }
    appendRow(collisions_item);
  }
}
}
