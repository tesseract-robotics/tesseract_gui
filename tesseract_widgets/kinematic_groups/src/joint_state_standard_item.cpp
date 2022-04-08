#include <tesseract_widgets/kinematic_groups/joint_state_standard_item.h>
#include <tesseract_widgets/common/standard_item_utils.h>
#include <tesseract_widgets/common/standard_item_type.h>

namespace tesseract_gui
{
JointStateStandardItem::JointStateStandardItem(QString name, const std::unordered_map<std::string, double>& state)
  : QStandardItem(QIcon(":/tesseract_widgets/png/cube.png"), "Joint State")
{
  ctor(state);
}

JointStateStandardItem::JointStateStandardItem(const QString &text, QString name, const std::unordered_map<std::string, double>& state)
  : QStandardItem(QIcon(":/tesseract_widgets/png/cube.png"), text)
{
  ctor(state);
}

JointStateStandardItem::JointStateStandardItem(const QIcon &icon, const QString &text, QString name, const std::unordered_map<std::string, double>& state)
  : QStandardItem(icon, text)
{
  ctor(state);
}

int JointStateStandardItem::type() const
{
  return static_cast<int>(StandardItemType::JOINT_STATE);
}

void JointStateStandardItem::ctor(const std::unordered_map<std::string, double>& state)
{
  for (const auto& s : state)
    appendRow(createStandardItemFloat(s.first, s.second));

  sortChildren(0);
}
}

