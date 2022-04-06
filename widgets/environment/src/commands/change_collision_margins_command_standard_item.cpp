
#include <tesseract_gui/widgets/environment/commands/change_collision_margins_command_standard_item.h>
#include <tesseract_gui/widgets/common/collision_margin_data_standard_item.h>
#include <tesseract_gui/widgets/common/standard_item_utils.h>
#include <tesseract_gui/common/standard_item_type.h>

namespace tesseract_gui
{

ChangeCollisionMarginsCommandStandardItem::ChangeCollisionMarginsCommandStandardItem(tesseract_environment::ChangeCollisionMarginsCommand::ConstPtr command)
  : QStandardItem(QIcon(":/tesseract_gui/png/merge.png"), "Change Collision Margins")
  , command(std::move(command))
{
  ctor();
}

ChangeCollisionMarginsCommandStandardItem::ChangeCollisionMarginsCommandStandardItem(const QString &text, tesseract_environment::ChangeCollisionMarginsCommand::ConstPtr command)
  : QStandardItem(QIcon(":/tesseract_gui/png/merge.png"), text)
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
  switch (command->getCollisionMarginOverrideType())
  {
    case tesseract_common::CollisionMarginOverrideType::NONE:
    {
      appendRow(createStandardItemString("Override type", "NONE"));
      break;
    }
    case tesseract_common::CollisionMarginOverrideType::REPLACE:
    {
      appendRow(createStandardItemString("Override type", "REPLACE"));
      break;
    }
    case tesseract_common::CollisionMarginOverrideType::MODIFY:
    {
      appendRow(createStandardItemString("Override type", "MODIFY"));
      break;
    }
    case tesseract_common::CollisionMarginOverrideType::OVERRIDE_DEFAULT_MARGIN:
    {
      appendRow(createStandardItemString("Override type", "OVERRIDE_DEFAULT_MARGIN"));
      break;
    }
    case tesseract_common::CollisionMarginOverrideType::OVERRIDE_PAIR_MARGIN:
    {
      appendRow(createStandardItemString("Override type", "OVERRIDE_PAIR_MARGIN"));
      break;
    }
    case tesseract_common::CollisionMarginOverrideType::MODIFY_PAIR_MARGIN:
    {
      appendRow(createStandardItemString("Override type", "MODIFY_PAIR_MARGIN"));
      break;
    }
  }

  auto* cmd_item = new CollisionMarginDataStandardItem(command->getCollisionMarginData());
  appendRow({cmd_item, new QStandardItem()});
}
}
