#include <tesseract_gui/common/gui_utils.h>
#include <ignition/rendering/RenderingIface.hh>
#include <ignition/rendering/RenderEngine.hh>
#include <ignition/common/Console.hh>

namespace tesseract_gui
{
QApplication* getApp()
{
  return qobject_cast<QApplication *>(qGuiApp);
}

ignition::rendering::ScenePtr sceneFromFirstRenderEngine(const std::string& scene_name)
{
  auto loadedEngNames = ignition::rendering::loadedEngines();
  if (loadedEngNames.empty())
  {
    igndbg << "No rendering engine is loaded yet" << std::endl;
    return nullptr;
  }

  auto engineName = loadedEngNames[0];
  if (loadedEngNames.size() > 1)
  {
    ignwarn << "More than one engine is available. "
      << "Using engine [" << engineName << "]" << std::endl;
  }

  auto* engine = ignition::rendering::engine(engineName);
  if (!engine)
  {
    ignerr << "Internal error: failed to load engine [" << engineName
      << "]." << std::endl;
    return nullptr;
  }

  if (engine->SceneCount() == 0)
  {
    igndbg << "No scene has been created yet" << std::endl;
    return nullptr;
  }

  auto scene = engine->SceneByName(scene_name);
  if (nullptr == scene)
  {
    ignerr << "Internal error: scene is null." << std::endl;
    return nullptr;
  }

  return scene;
}

ignition::rendering::ScenePtr sceneFromFirstRenderEngine(const std::string& engin_name, const std::string& scene_name)
{
  auto* engine = ignition::rendering::engine(engin_name);
  if (!engine)
  {
    ignerr << "Internal error: failed to load engine [" << engin_name << "]." << std::endl;
    return nullptr;
  }

  if (engine->SceneCount() == 0)
  {
    igndbg << "No scene has been created yet" << std::endl;
    return nullptr;
  }

  auto scene = engine->SceneByName(scene_name);
  if (nullptr == scene)
  {
    ignerr << "Internal error: scene is null." << std::endl;
    return nullptr;
  }

  return scene;
}
}
