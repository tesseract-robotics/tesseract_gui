#ifndef TESSERACT_GUI_APP_TESSERACT_ROBOTICS_STUDIO_H
#define TESSERACT_GUI_APP_TESSERACT_ROBOTICS_STUDIO_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <memory>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QMainWindow>

namespace Ui {
class TesseractRoboticsStudio;
}

namespace tesseract_gui
{
struct TesseractRoboticsStudioPrivate;
struct SceneInfo;

class TesseractRoboticsStudio : public QMainWindow
{
  Q_OBJECT

public:
  explicit TesseractRoboticsStudio(QWidget *parent = nullptr);
  ~TesseractRoboticsStudio();

  bool createScene(const std::string& scene_name, const std::string& scene_namespace = "") const;
  const std::unordered_map<std::string, std::shared_ptr<SceneInfo>>& getSceneInfos() const;

private:
  std::unique_ptr<Ui::TesseractRoboticsStudio> ui;
  std::unique_ptr<TesseractRoboticsStudioPrivate> d_;

  friend struct TesseractRoboticsStudioPrivate;
};
}

#endif // TESSERACT_GUI_APP_TESSERACT_ROBOTICS_STUDIO_H
