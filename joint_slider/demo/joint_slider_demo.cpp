
#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#include <QtGui/QtGui>
#include <QtQml/QQmlContext>
#include <QtQuick/QQuickItem>
#include <QtQuick/QQuickView>
#include <QDebug>
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <tesseract_gui/joint_slider/joint_list_model.h>

#include <tesseract_urdf/urdf_parser.h>
#include <tesseract_common/resource_locator.h>

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

void onJointValueChanged(QString name, double value)
{
  qDebug() << "Joint Value Changed: (" << name << ", " << value << ")";
}

int main(int argc, char ** argv)
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    // Load Scene Graph
    std::string path = std::string(TESSERACT_SUPPORT_DIR) + "/urdf/lbr_iiwa_14_r820.urdf";
    tesseract_common::SimpleResourceLocator locator(locateResource);
    tesseract_scene_graph::SceneGraph::UPtr sg = tesseract_urdf::parseURDFFile(path, locator);

    tesseract_gui::JointListModel model;
    for (const auto& j : sg->getJoints())
      model.add(j);

    QObject::connect(&model, &tesseract_gui::JointListModel::jointValueChanged, &onJointValueChanged);

    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.rootContext()->setContextProperty("jointSliderModel", &model);
    view.setSource(QUrl("qrc:/JointSlider/JointSlider.qml"));
    view.show();

    return app.exec();
}
