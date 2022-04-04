#ifndef TESSERACT_GUI_ENVIRONMENT_ADD_SCENE_GRAPH_COMMAND_STANDARD_ITEM_H
#define TESSERACT_GUI_ENVIRONMENT_ADD_SCENE_GRAPH_COMMAND_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_environment/commands/add_scene_graph_command.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class AddSceneGraphCommandStandardItem : public QStandardItem
{
public:
  AddSceneGraphCommandStandardItem(tesseract_environment::AddSceneGraphCommand::ConstPtr command);
  explicit AddSceneGraphCommandStandardItem(const QString &text, tesseract_environment::AddSceneGraphCommand::ConstPtr command);
  AddSceneGraphCommandStandardItem(const QIcon &icon, const QString &text, tesseract_environment::AddSceneGraphCommand::ConstPtr command);
  int type() const override;

  tesseract_environment::AddSceneGraphCommand::ConstPtr command;

private:
  void ctor();
};
}

#endif // TESSERACT_GUI_ENVIRONMENT_ADD_SCENE_GRAPH_COMMAND_STANDARD_ITEM_H
