
#include <tesseract_gui/widgets/environment/commands/change_collision_margins_command_standard_item.h>
#include <tesseract_gui/widgets/common/collision_margin_data_standard_item.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, COMMAND_ICON, (":/tesseract_gui/png/merge.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, TEXT_ICON, (":/tesseract_gui/png/text.png"));

namespace tesseract_gui
{

ChangeCollisionMarginsCommandStandardItem::ChangeCollisionMarginsCommandStandardItem(tesseract_environment::ChangeCollisionMarginsCommand::ConstPtr command)
  : QStandardItem(*COMMAND_ICON(), "Change Collision Margins")
  , command(std::move(command))
{
  ctor();
}

ChangeCollisionMarginsCommandStandardItem::ChangeCollisionMarginsCommandStandardItem(const QString &text, tesseract_environment::ChangeCollisionMarginsCommand::ConstPtr command)
  : QStandardItem(*COMMAND_ICON(), text)
  , command(std::move(command))
{
  ctor();
}

ChangeCollisionMarginsCommandStandardItem::ChangeCollisionMarginsCommandStandardItem(const QIcon &icon, const QString &text, tesseract_environment::ChangeCollisionMarginsCommand::ConstPtr command)
  : QStandardItem(icon, text)
  , command(std::move(command))
{
  ctor();
}

int ChangeCollisionMarginsCommandStandardItem::type() const
{
  return static_cast<int>(StandardItemType::ENV_COMMAND_CHANGE_COLLISION_MARGINS);
}

void ChangeCollisionMarginsCommandStandardItem::ctor()
{
  auto* type_name = new QStandardItem(*TEXT_ICON(), "Override type");
  QStandardItem* type_value;
  switch (command->getCollisionMarginOverrideType())
  {
    case tesseract_common::CollisionMarginOverrideType::NONE:
    {
      type_value = new QStandardItem("NONE");
      break;
    }
    case tesseract_common::CollisionMarginOverrideType::REPLACE:
    {
      type_value = new QStandardItem("REPLACE");
      break;
    }
    case tesseract_common::CollisionMarginOverrideType::MODIFY:
    {
      type_value = new QStandardItem("MODIFY");
      break;
    }
    case tesseract_common::CollisionMarginOverrideType::OVERRIDE_DEFAULT_MARGIN:
    {
      type_value = new QStandardItem("OVERRIDE_DEFAULT_MARGIN");
      break;
    }
    case tesseract_common::CollisionMarginOverrideType::OVERRIDE_PAIR_MARGIN:
    {
      type_value = new QStandardItem("OVERRIDE_PAIR_MARGIN");
      break;
    }
    case tesseract_common::CollisionMarginOverrideType::MODIFY_PAIR_MARGIN:
    {
      type_value = new QStandardItem("MODIFY_PAIR_MARGIN");
      break;
    }
  }
  appendRow({type_name, type_value});

  auto* cmd_item = new CollisionMarginDataStandardItem(command->getCollisionMarginData());
  appendRow({cmd_item, new QStandardItem()});
}
}
