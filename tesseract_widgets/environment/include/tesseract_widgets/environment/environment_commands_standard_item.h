#ifndef TESSERACT_WIDGETS_ENVIRONMENT_ENVIRONMENT_COMMANDS_STANDARD_ITEM_H
#define TESSERACT_WIDGETS_ENVIRONMENT_ENVIRONMENT_COMMANDS_STANDARD_ITEM_H


#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_environment/command.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class EnvironmentCommandsStandardItem : public QStandardItem
{
public:
  EnvironmentCommandsStandardItem();
  explicit EnvironmentCommandsStandardItem(tesseract_environment::Commands commands);
  explicit EnvironmentCommandsStandardItem(const QString &text, tesseract_environment::Commands commands);
  explicit EnvironmentCommandsStandardItem(const QIcon &icon, const QString &text, tesseract_environment::Commands commands);
  int type() const override;

  void appendCommand(const tesseract_environment::Command::ConstPtr& command);
  const tesseract_environment::Commands& getCommands() const;

private:
  tesseract_environment::Commands commands_;

  void addCommand(const QString& text, const tesseract_environment::Command::ConstPtr& command);
  void ctor();
};
}


#endif // TESSERACT_WIDGETS_ENVIRONMENT_ENVIRONMENT_COMMANDS_STANDARD_ITEM_H
