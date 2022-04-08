#ifndef TESSERACT_IGNITION_GUI_UTILS_H
#define TESSERACT_IGNITION_GUI_UTILS_H

#include <QApplication>
#include <ignition/rendering/Scene.hh>

namespace tesseract_gui
{
  QApplication* getApp();

  ignition::rendering::ScenePtr sceneFromFirstRenderEngine(const std::string& scene_name);

  ignition::rendering::ScenePtr sceneFromRenderEngine(const std::string& engine_name, const std::string& scene_name);

}
#endif // TESSERACT_IGNITION_GUI_UTILS_H
