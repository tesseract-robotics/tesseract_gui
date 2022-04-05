#ifndef TESSERACT_GUI_WIDGETS_ENVIRONMENT_ENVIRONMENT_COMMANDS_MODEL_H
#define TESSERACT_GUI_WIDGETS_ENVIRONMENT_ENVIRONMENT_COMMANDS_MODEL_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_srdf/kinematics_information.h>
#include <tesseract_environment/command.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItemModel>
#include <tesseract_gui/widgets/environment/environment_commands_standard_item.h>

namespace tesseract_gui
{

class EnvironmentCommandsModel : public QStandardItemModel
{
  Q_OBJECT

public:

  explicit EnvironmentCommandsModel(QObject *parent = nullptr);
  EnvironmentCommandsModel(const EnvironmentCommandsModel &other);
  EnvironmentCommandsModel &operator=(const EnvironmentCommandsModel &other);

  void set(const tesseract_environment::Commands& commands);
  void appendCommand(const tesseract_environment::Command::ConstPtr& command);
  void clear();

  const tesseract_environment::Commands& getCommands() const;

private:
  EnvironmentCommandsStandardItem* getRoot();
  const EnvironmentCommandsStandardItem* getRoot() const;

};

}

#endif // TESSERACT_GUI_WIDGETS_ENVIRONMENT_ENVIRONMENT_COMMANDS_MODEL_H
