#ifndef TESSERACT_WIDGETS_ENVIRONMENT_COMMAND_STANDARD_ITEM_H
#define TESSERACT_WIDGETS_ENVIRONMENT_COMMAND_STANDARD_ITEM_H


#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_environment/commands/add_allowed_collision_command.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class CommandStandardItem : public QStandardItem
{
public:
  using CommandStandardItem::CommandStandardItem;
  virtual tesseract_environment::Command::ConstPtr getCommand() const = 0;

};
}

#endif // TESSERACT_WIDGETS_ENVIRONMENT_COMMAND_STANDARD_ITEM_H
