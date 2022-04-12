#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#include <QApplication>
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <tesseract_widgets/workbench/workbench_widget.h>
#include <tesseract_widgets/environment/environment_widget.h>
#include <tesseract_support/tesseract_support_resource_locator.h>

int main(int argc, char ** argv)
{
    QApplication app(argc, argv);

    Q_INIT_RESOURCE(tesseract_widgets_resources);

    auto locator = std::make_shared<tesseract_common::TesseractSupportResourceLocator>();
    tesseract_common::fs::path urdf_path = std::string(TESSERACT_SUPPORT_DIR) + "/urdf/lbr_iiwa_14_r820.urdf";
    tesseract_common::fs::path srdf_path = std::string(TESSERACT_SUPPORT_DIR) + "/urdf/lbr_iiwa_14_r820.srdf";

    auto env = std::make_unique<tesseract_environment::Environment>();
    env->init(urdf_path, srdf_path, locator);

    auto* env_widget = new tesseract_gui::EnvironmentWidget();
    env_widget->setEnvironment(std::move(env));

    tesseract_gui::WorkbenchWidget widget(env_widget);
    widget.show();

    return app.exec();
}
