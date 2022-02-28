#include <tesseract_gui/app/tesseract_robotics_studio.h>
#include <Qads/DockManager.h>
#include "ui_tesseract_robotics_studio.h"

#include <tesseract_gui/rendering/simple_render_widget.h>
#include <tesseract_gui/scene_graph/scene_graph_standard_item.h>
#include <QTreeView>
#include <QVBoxLayout>
#include <tesseract_urdf/urdf_parser.h>
#include <tesseract_common/resource_locator.h>

namespace tesseract_gui
{

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

struct TesseractRoboticsStudioPrivate
{
  TesseractRoboticsStudioPrivate(TesseractRoboticsStudio* app) : app(app) {}

  TesseractRoboticsStudio* app;
  ads::CDockManager* dock_manager {nullptr};
};

TesseractRoboticsStudio::TesseractRoboticsStudio(QWidget *parent)
  : QMainWindow(parent)
  , ui(std::make_unique<Ui::TesseractRoboticsStudio>())
  , d_(std::make_unique<TesseractRoboticsStudioPrivate>(this))
{
  ui->setupUi(this);
  setWindowTitle(QApplication::applicationName());

  d_->dock_manager = new ads::CDockManager(this);


  {
    SimpleRenderWidget* w = new SimpleRenderWidget();
    ads::CDockWidget* dock_widget = new ads::CDockWidget("Scene");
    dock_widget->setWidget(w);
    dock_widget->setFeature(ads::CDockWidget::DockWidgetDeleteOnClose, true);
    dock_widget->setFeature(ads::CDockWidget::DockWidgetFocusable, true);
    d_->dock_manager->setCentralWidget(dock_widget);
  }

  {
    std::string path = std::string(TESSERACT_SUPPORT_DIR) + "/urdf/lbr_iiwa_14_r820.urdf";

    tesseract_common::SimpleResourceLocator locator(locateResource);
    auto scene_graph = tesseract_urdf::parseURDFFile(path, locator);

    auto* model = new QStandardItemModel();
    model->setColumnCount(2);
    model->setHorizontalHeaderLabels({"Name", "Values"});

    auto* item = new tesseract_gui::SceneGraphStandardItem(std::move(scene_graph));
    model->appendRow(item);

    auto* w = new QTreeView();
    w->setModel(model);

    ads::CDockWidget* dock_widget = new ads::CDockWidget("Scene Graph");
    dock_widget->setWidget(w);
    dock_widget->setFeature(ads::CDockWidget::DockWidgetDeleteOnClose, true);
    dock_widget->setFeature(ads::CDockWidget::DockWidgetFocusable, true);
    d_->dock_manager->addDockWidgetTab(ads::RightDockWidgetArea, dock_widget);
  }
//  d_->dock_manager->addDockWidgetTab(ads::CenterDockWidgetArea, dock_widget);
}

TesseractRoboticsStudio::~TesseractRoboticsStudio() = default;

void TesseractRoboticsStudio::saveState()
{

}

void TesseractRoboticsStudio::savePerspectives()
{

}

void TesseractRoboticsStudio::restoreState()
{

}

void TesseractRoboticsStudio::restorePerspectives()
{

}

}
