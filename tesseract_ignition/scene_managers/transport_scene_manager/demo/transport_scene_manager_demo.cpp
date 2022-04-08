
#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#include <QApplication>
#include <QDebug>
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <tesseract_gui/rendering/simple_render_widget.h>
#include <tesseract_gui/rendering/interactive_view_control.h>

#include <tesseract_common/resource_locator.h>
#include <tesseract_urdf/urdf_parser.h>

#include <tesseract_gui/transport_scene_manager/transport_scene_manager.h>

std::string locateResource(const std::string& url)
{
  std::string mod_url = url;
  if (url.find("package://tesseract_support") == 0)
  {
    mod_url.erase(0, strlen("package://tesseract_support"));
    size_t pos = mod_url.find('/');
    if (pos == std::string::npos)
    {
      return std::string();
    }

    std::string package = mod_url.substr(0, pos);
    mod_url.erase(0, pos);
    std::string package_path = std::string(TESSERACT_SUPPORT_DIR);

    if (package_path.empty())
    {
      return std::string();
    }

    mod_url = package_path + mod_url;
  }

  return mod_url;
}

int main(int argc, char ** argv)
{
    QApplication app(argc, argv);

    Q_INIT_RESOURCE(tesseract_gui_resources);

    std::string path = std::string(TESSERACT_SUPPORT_DIR) + "/urdf/lbr_iiwa_14_r820.urdf";

    tesseract_common::SimpleResourceLocator locator(locateResource);
    auto scene_graph = tesseract_urdf::parseURDFFile(path, locator);

    tesseract_gui::SimpleRenderWidget widget(scene_graph->getName());
    widget.show();

    tesseract_gui::InteractiveViewControl view_control(scene_graph->getName());
    app.installEventFilter(&view_control);

    tesseract_gui::TransportSceneManager scene_manager(scene_graph->getName());
    app.installEventFilter(&scene_manager);

    return app.exec();
}
