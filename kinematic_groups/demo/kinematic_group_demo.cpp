
#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#include <QApplication>
#include <QStandardItemModel>
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <tesseract_gui/kinematic_groups/kinematic_groups_editor_widget.h>

int main(int argc, char ** argv)
{
    QApplication app(argc, argv);

    Q_INIT_RESOURCE(resources);

    QStringList joint_names {"joint_1", "joint_2","joint_3","joint_4","joint_5","joint_6"};
    QStringList link_names {"link_1", "link_2","link_3","link_4","link_5","link_6"};

    tesseract_gui::KinematicGroupsModel model;
    tesseract_gui::KinematicGroupsEditorWidget widget(joint_names, link_names);
    widget.setModel(&model);
    widget.show();

    return app.exec();
}
