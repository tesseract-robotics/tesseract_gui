#include <tesseract_gui/widgets/kinematic_groups/joint_state_standard_item.h>
#include <tesseract_gui/widgets/common/standard_item_utils.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, CUBE_ICON, (":/tesseract_gui/png/cube.png"));

namespace tesseract_gui
{
JointStateStandardItem::JointStateStandardItem(QString name, const std::unordered_map<std::string, double>& state)
  : QStandardItem(*CUBE_ICON(), "Joint State")
{
  ctor(state);
}

JointStateStandardItem::JointStateStandardItem(const QString &text, QString name, const std::unordered_map<std::string, double>& state)
  : QStandardItem(*CUBE_ICON(), text)
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

