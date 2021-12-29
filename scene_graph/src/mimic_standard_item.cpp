#include <tesseract_gui/scene_graph/mimic_standard_item.h>
#include <tesseract_gui/scene_graph/types.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, NUMERIC_ICON, (":/tesseract_gui/png/numeric.png"));

namespace tesseract_gui
{
MimicStandardItem::MimicStandardItem(tesseract_scene_graph::JointMimic::Ptr mimic)
  : QStandardItem("Mimic")
  , mimic(std::move(mimic))
{
  ctor();
}

MimicStandardItem::MimicStandardItem(const QString &text, tesseract_scene_graph::JointMimic::Ptr mimic)
  : QStandardItem(text)
  , mimic(std::move(mimic))
{
  ctor();
}

MimicStandardItem::MimicStandardItem(const QIcon &icon, const QString &text, tesseract_scene_graph::JointMimic::Ptr mimic)
  : QStandardItem(icon, text)
  , mimic(std::move(mimic))
{
  ctor();
}

int MimicStandardItem::type() const
{
  return static_cast<int>(SceneGraphItemType::MIMIC);
}

void MimicStandardItem::ctor()
{
  {
    auto* item = new QStandardItem(*NUMERIC_ICON(), "offset");
    auto* value = new QStandardItem(QString("%1").arg(mimic->offset));
    appendRow({item, value});
  }

  {
    auto* item = new QStandardItem(*NUMERIC_ICON(), "multiplier");
    auto* value = new QStandardItem(QString("%1").arg(mimic->multiplier));
    appendRow({item, value});
  }

  {
    auto* item = new QStandardItem(*NUMERIC_ICON(), "joint_name");
    auto* value = new QStandardItem(QString::fromStdString(mimic->joint_name));
    appendRow({item, value});
  }
}
}
