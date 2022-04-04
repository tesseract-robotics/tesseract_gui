
#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#include <QApplication>
#include <QStandardItemModel>
#include <QTreeView>
#include <QDebug>
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <tesseract_gui/widgets/scene_graph/scene_graph_standard_item.h>
#include <tesseract_urdf/urdf_parser.h>
#include <tesseract_support/tesseract_support_resource_locator.h>

int main(int argc, char ** argv)
{
    QApplication app(argc, argv);

    Q_INIT_RESOURCE(tesseract_gui_resources);

    std::string path = std::string(TESSERACT_SUPPORT_DIR) + "/urdf/lbr_iiwa_14_r820.urdf";

    tesseract_common::TesseractSupportResourceLocator locator;
    auto scene_graph = tesseract_urdf::parseURDFFile(path, locator);

    auto* model = new QStandardItemModel();
    model->setColumnCount(2);
    model->setHorizontalHeaderLabels({"Name", "Values"});

    auto* item = new tesseract_gui::SceneGraphStandardItem(std::move(scene_graph));
    model->appendRow(item);

    QTreeView widget;
    widget.setModel(model);
    widget.show();

    return app.exec();
}
