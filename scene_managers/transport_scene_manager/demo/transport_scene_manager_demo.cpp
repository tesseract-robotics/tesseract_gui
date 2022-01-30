
#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#include <QApplication>
#include <QDebug>
#include <QtQuickWidgets/QtQuickWidgets>
TESSERACT_COMMON_IGNORE_WARNINGS_POP

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

    Q_INIT_RESOURCE(resources);
    Q_INIT_RESOURCE(minimal_scene);

    std::string path = std::string(TESSERACT_SUPPORT_DIR) + "/urdf/lbr_iiwa_14_r820.urdf";

    tesseract_common::SimpleResourceLocator locator(locateResource);
    auto scene_graph = tesseract_urdf::parseURDFFile(path, locator);

    QQuickWidget widget;
    widget.setResizeMode(QQuickWidget::SizeRootObjectToView);
    widget.setSource(QUrl(":/tesseract_gui/MinimalScene.qml"));
    widget.show();

    auto scene_manager = std::make_unique<tesseract_gui::TransportSceneManager>();

    return app.exec();
}
