
#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#include <QApplication>
#include <QStandardItemModel>
#include <QStringListModel>
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <tesseract_gui/kinematic_groups/groups_joint_states_editor_widget.h>
#include <tesseract_gui/kinematic_groups/group_joint_states_model.h>

#include <tesseract_urdf/urdf_parser.h>
#include <tesseract_common/resource_locator.h>

tesseract_scene_graph::SceneGraph::UPtr sg;

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

std::vector<tesseract_scene_graph::Joint::ConstPtr> getGroupJoints(QString group_name)
{
  std::vector<tesseract_scene_graph::Joint::ConstPtr> joints;
  if (group_name == "group1")
  {
    joints.push_back(sg->getJoint("joint_a1"));
    joints.push_back(sg->getJoint("joint_a2"));
    joints.push_back(sg->getJoint("joint_a3"));
    joints.push_back(sg->getJoint("joint_a4"));
    joints.push_back(sg->getJoint("joint_a5"));
    joints.push_back(sg->getJoint("joint_a6"));
    joints.push_back(sg->getJoint("joint_a7"));
  }
  else if (group_name == "group2")
  {
    joints.push_back(sg->getJoint("joint_a1"));
    joints.push_back(sg->getJoint("joint_a4"));
    joints.push_back(sg->getJoint("joint_a7"));
  }
  return joints;
}

int main(int argc, char ** argv)
{
    QApplication app(argc, argv);

    Q_INIT_RESOURCE(tesseract_gui_resources);

    // Load Scene Graph
    std::string path = std::string(TESSERACT_SUPPORT_DIR) + "/urdf/lbr_iiwa_14_r820.urdf";
    tesseract_common::SimpleResourceLocator locator(locateResource);
    sg = tesseract_urdf::parseURDFFile(path, locator);

    tesseract_gui::GroupJointStatesModel model;
    QStringList list {"group1", "group2"};
    QStringListModel group_names;
    group_names.setStringList(list);
    tesseract_gui::GroupJointsRetriever func = getGroupJoints;
    tesseract_gui::GroupsJointStatesEditorWidget widget(&group_names, func);
    widget.setModel(&model);
    widget.show();

    return app.exec();
}
