#ifndef TESSERACT_GUI_APP_TESSERACT_ROBOTICS_STUDIO_H
#define TESSERACT_GUI_APP_TESSERACT_ROBOTICS_STUDIO_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <memory>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QMainWindow>

#include <tesseract_gui/rendering/simple_render_widget.h>
#include <tesseract_gui/rendering/interactive_view_control.h>
#include <tesseract_gui/common/entity_manager.h>

namespace Ui {
class TesseractRoboticsStudio;
}

namespace tesseract_gui
{
struct TesseractRoboticsStudioPrivate;

struct SceneInfo
{
  using Ptr = std::shared_ptr<SceneInfo>;
  using ConstPtr = std::shared_ptr<const SceneInfo>;

  SceneInfo(std::string scene_name);

  std::string scene_name;
  EntityManager::Ptr entity_manager;
  SimpleRenderWidget* render_widget;
  std::unique_ptr<tesseract_gui::InteractiveViewControl> view_control;
};

class TesseractRoboticsStudio : public QMainWindow
{
  Q_OBJECT

public:
  explicit TesseractRoboticsStudio(QWidget *parent = nullptr);
  ~TesseractRoboticsStudio();

  SceneInfo::Ptr createScene(const std::string& scene_name) const;
  const std::unordered_map<std::string, SceneInfo::Ptr>& getSceneInfos() const;

private:
  std::unique_ptr<Ui::TesseractRoboticsStudio> ui;
  std::unique_ptr<TesseractRoboticsStudioPrivate> d_;

  friend struct TesseractRoboticsStudioPrivate;
};
}

#endif // TESSERACT_GUI_APP_TESSERACT_ROBOTICS_STUDIO_H
