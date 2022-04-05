#include <tesseract_gui/widgets/kinematic_groups/joint_state_standard_item.h>
#include <tesseract_gui/widgets/common/origin_standard_item.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, CUBE_ICON, (":/tesseract_gui/png/cube.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, TEXT_ICON, (":/tesseract_gui/png/text.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, NUMERIC_ICON, (":/tesseract_gui/png/numeric.png"));

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
  {
    auto* p_name = new QStandardItem(*NUMERIC_ICON(), QString::fromStdString(s.first));
    auto* p_value = new QStandardItem(QString("%1").arg(s.second));
    appendRow({p_name, p_value});
  }
  sortChildren(0);
}
}

