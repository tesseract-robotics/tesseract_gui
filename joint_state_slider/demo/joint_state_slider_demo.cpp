
#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#include <QApplication>
#include <QDebug>
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <tesseract_gui/joint_state_slider/joint_state_slider_widget.h>

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

int main(int argc, char ** argv)
{
  QApplication app(argc, argv);

  // Load Scene Graph
  std::string path = std::string(TESSERACT_SUPPORT_DIR) + "/urdf/lbr_iiwa_14_r820.urdf";
  tesseract_common::SimpleResourceLocator locator(locateResource);
  tesseract_scene_graph::SceneGraph::UPtr sg = tesseract_urdf::parseURDFFile(path, locator);
  std::vector<tesseract_scene_graph::Joint::ConstPtr> joints;
  for (const auto& joint : sg->getJoints())
  {
    if (joint->type == tesseract_scene_graph::JointType::REVOLUTE ||
        joint->type == tesseract_scene_graph::JointType::CONTINUOUS ||
        joint->type == tesseract_scene_graph::JointType::PRISMATIC)
      joints.push_back(joint);
  }
  tesseract_gui::JointStateSliderWidget widget;
  widget.setJoints(joints);
  widget.show();

  return app.exec();
}
