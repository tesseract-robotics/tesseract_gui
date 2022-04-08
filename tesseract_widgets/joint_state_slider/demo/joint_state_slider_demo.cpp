
#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#include <QApplication>
#include <QDebug>
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <tesseract_widgets/joint_state_slider/joint_state_slider_widget.h>

#include <tesseract_urdf/urdf_parser.h>
#include <tesseract_support/tesseract_support_resource_locator.h>

int main(int argc, char ** argv)
{
  QApplication app(argc, argv);

  // Load Scene Graph
  std::string path = std::string(TESSERACT_SUPPORT_DIR) + "/urdf/lbr_iiwa_14_r820.urdf";
  tesseract_common::TesseractSupportResourceLocator locator;
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
