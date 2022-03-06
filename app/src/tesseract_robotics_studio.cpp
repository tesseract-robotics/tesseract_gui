#include <tesseract_gui/app/tesseract_robotics_studio.h>
#include <DockManager.h>
#include "ui_tesseract_robotics_studio.h"

#include <tesseract_gui/rendering/simple_render_widget.h>
#include <tesseract_gui/rendering/interactive_view_control.h>
#include <tesseract_gui/environment/environment_widget.h>

#include <tesseract_urdf/urdf_parser.h>
#include <tesseract_common/resource_locator.h>

#include <QSettings>
#include <QWidgetAction>
#include <QComboBox>
#include <QInputDialog>

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

struct SceneInfo
{
  SceneInfo(std::string scene_name, std::string scene_namespace)
    : scene_name(std::move(scene_name))
    , scene_namespace(std::move(scene_namespace))
    , key((scene_namespace.empty()) ? this->scene_name : this->scene_namespace + "::" + this->scene_name)
    , view_control(std::make_unique<tesseract_gui::InteractiveViewControl>(key))
  {
    qobject_cast<QApplication *>(qGuiApp)->installEventFilter(view_control.get());
  }

  const std::string scene_name;
  const std::string scene_namespace;
  const std::string key;
  SimpleRenderWidget* render_widget;
  std::unique_ptr<tesseract_gui::InteractiveViewControl> view_control;
};

struct TesseractRoboticsStudioPrivate
{
  TesseractRoboticsStudioPrivate(TesseractRoboticsStudio* app) : app(app)
  {
  }

  TesseractRoboticsStudio* app;
  ads::CDockManager* dock_manager {nullptr};
  QWidgetAction* perspective_list_action {nullptr};
  QComboBox* perspective_comboBox {nullptr};
  std::unordered_map<std::string, std::shared_ptr<SceneInfo>> scene_infos;

  /** @brief Saves the dock manager state and the main window geometry */
  void saveState();

  /** @brief Create a perspective */
  void createPerspective();

  /** @brief Save the list of perspectives */
  void savePerspectives();

  /** @brief Restores the dock manager state */
  void restoreState();

  /** @brief Restore the perspective list of the dock manager */
  void restorePerspectives();

};

void TesseractRoboticsStudioPrivate::saveState()
{
  QSettings Settings("TesseractRoboticsStudioSettings.ini", QSettings::IniFormat);
  Settings.setValue("TesseractRoboticsStudio/Geometry", app->saveGeometry());
  Settings.setValue("TesseractRoboticsStudio/State", app->saveState());
  Settings.setValue("TesseractRoboticsStudio/DockingState", dock_manager->saveState());
}

void TesseractRoboticsStudioPrivate::createPerspective()
{
  QString PerspectiveName = QInputDialog::getText(app, "Create Perspective", "Enter unique name:");
  if (PerspectiveName.isEmpty())
    return;

  dock_manager->addPerspective(PerspectiveName);
  QSignalBlocker Blocker(perspective_comboBox);
  perspective_comboBox->clear();
  perspective_comboBox->addItems(dock_manager->perspectiveNames());
  perspective_comboBox->setCurrentText(PerspectiveName);

  savePerspectives();
}

void TesseractRoboticsStudioPrivate::savePerspectives()
{
  QSettings Settings("TesseractRoboticsStudioSettings.ini", QSettings::IniFormat);
  dock_manager->savePerspectives(Settings);
}

void TesseractRoboticsStudioPrivate::restoreState()
{
  QSettings Settings("TesseractRoboticsStudioSettings.ini", QSettings::IniFormat);
  app->restoreGeometry(Settings.value("TesseractRoboticsStudio/Geometry").toByteArray());
  app->restoreState(Settings.value("TesseractRoboticsStudio/State").toByteArray());
  dock_manager->restoreState(Settings.value("TesseractRoboticsStudio/DockingState").toByteArray());
}

void TesseractRoboticsStudioPrivate::restorePerspectives()
{
  QSettings Settings("TesseractRoboticsStudioSettings.ini", QSettings::IniFormat);
  dock_manager->loadPerspectives(Settings);
  perspective_comboBox->clear();
  perspective_comboBox->addItems(dock_manager->perspectiveNames());
}

TesseractRoboticsStudio::TesseractRoboticsStudio(QWidget *parent)
  : QMainWindow(parent)
  , ui(std::make_unique<Ui::TesseractRoboticsStudio>())
  , d_(std::make_unique<TesseractRoboticsStudioPrivate>(this))
{
  ui->setupUi(this);
  setWindowTitle(QApplication::applicationName());

  d_->dock_manager = new ads::CDockManager(this);
  d_->dock_manager->setStyleSheet("");

  // Setup actions
  ui->actionSave_State->setIcon(QIcon(":/tesseract_gui/png/save.png"));
  ui->actionRestore_State->setIcon(QIcon(":/tesseract_gui/png/restore.png"));
  ui->actionCreate_Perspective->setIcon(QIcon(":/tesseract_gui/png/layout.png"));
  connect(ui->actionSave_State, &QAction::triggered, [this](){d_->saveState();});
  connect(ui->actionRestore_State, &QAction::triggered, [this](){d_->restoreState();});
  connect(ui->actionCreate_Perspective, &QAction::triggered, [this](){d_->createPerspective();});
  d_->perspective_list_action = new QWidgetAction(this);
  d_->perspective_comboBox = new QComboBox(this);
  d_->perspective_comboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
  d_->perspective_comboBox->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
  d_->perspective_list_action->setDefaultWidget(d_->perspective_comboBox);
  ui->toolBar->addAction(d_->perspective_list_action);
  connect(d_->perspective_comboBox, SIGNAL(activated(const QString&)), d_->dock_manager, SLOT(openPerspective(const QString&)));

  {
    auto locator = std::make_shared<tesseract_common::SimpleResourceLocator>(locateResource);
    tesseract_common::fs::path urdf_path = std::string(TESSERACT_SUPPORT_DIR) + "/urdf/lbr_iiwa_14_r820.urdf";
    tesseract_common::fs::path srdf_path = std::string(TESSERACT_SUPPORT_DIR) + "/urdf/lbr_iiwa_14_r820.srdf";

    auto env = std::make_shared<tesseract_environment::Environment>();
    env->init(urdf_path, srdf_path, locator);

    if (createScene(env->getName()))
    {
      auto* widget = new tesseract_gui::EnvironmentWidget();
      widget->addEnvironment(env);

      ads::CDockWidget* dock_widget = new ads::CDockWidget("Environment");
      dock_widget->setWidget(widget);
      dock_widget->setFeature(ads::CDockWidget::DockWidgetDeleteOnClose, true);
      dock_widget->setFeature(ads::CDockWidget::DockWidgetFocusable, true);
      d_->dock_manager->addDockWidgetTab(ads::LeftDockWidgetArea, dock_widget);
    }
  }

//  d_->restoreState(); If this is enabled widgets do not show up if name change
  d_->restorePerspectives();
}

TesseractRoboticsStudio::~TesseractRoboticsStudio() = default;

bool TesseractRoboticsStudio::createScene(const std::string& scene_name, const std::string& scene_namespace) const
{
  auto scene_info = std::make_shared<SceneInfo>(scene_name, scene_namespace);
  auto it = d_->scene_infos.find(scene_info->key);
  if (it != d_->scene_infos.end())
    return false;

  scene_info->render_widget = new SimpleRenderWidget(scene_info->key);
  ads::CDockWidget* dock_widget = new ads::CDockWidget(QString::fromStdString(scene_info->key));
  dock_widget->setWidget(scene_info->render_widget);
  dock_widget->setFeature(ads::CDockWidget::DockWidgetDeleteOnClose, true);
  dock_widget->setFeature(ads::CDockWidget::DockWidgetFocusable, true);
  d_->dock_manager->addDockWidgetTab(ads::CenterDockWidgetArea, dock_widget);
  d_->scene_infos[scene_info->key] = scene_info;
  return true;
}

const std::unordered_map<std::string, std::shared_ptr<SceneInfo>>& TesseractRoboticsStudio::getSceneInfos() const
{
  return d_->scene_infos;
}


}
