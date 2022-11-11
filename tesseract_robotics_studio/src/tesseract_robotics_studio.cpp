/**
 * @author Levi Armstrong <levi.armstrong@gmail.com>
 *
 * @copyright Copyright (C) 2022 Levi Armstrong <levi.armstrong@gmail.com>
 *
 * @par License
 * GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
 * @par
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 * @par
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * @par
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#include <tesseract_robotics_studio/tesseract_robotics_studio.h>
#include <DockManager.h>
#include "ui_tesseract_robotics_studio.h"

#include <tesseract_ignition/ignition_environment_widget.h>

#include <tesseract_urdf/urdf_parser.h>
#include <tesseract_support/tesseract_support_resource_locator.h>

#include <tesseract_qt/environment/environment_widget_config.h>

#include <QSettings>
#include <QWidgetAction>
#include <QComboBox>
#include <QInputDialog>

namespace tesseract_gui
{
SceneInfo::SceneInfo(std::string scene_name)
  : scene_name(std::move(scene_name))
  , entity_manager(std::make_shared<EntityManager>())
{
}

struct TesseractRoboticsStudioPrivate
{
  TesseractRoboticsStudioPrivate(TesseractRoboticsStudio* app) : app(app) {}

  TesseractRoboticsStudio* app;
  ads::CDockManager* dock_manager{ nullptr };
  QWidgetAction* perspective_list_action{ nullptr };
  QComboBox* perspective_comboBox{ nullptr };
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

TesseractRoboticsStudio::TesseractRoboticsStudio(QWidget* parent)
  : QMainWindow(parent)
  , ui(std::make_unique<Ui::TesseractRoboticsStudio>())
  , d_(std::make_unique<TesseractRoboticsStudioPrivate>(this))
{
  ui->setupUi(this);
  setWindowTitle(QApplication::applicationName());

  d_->dock_manager = new ads::CDockManager(this);
  d_->dock_manager->setStyleSheet("");

  // Setup actions
  ui->actionSave_State->setIcon(QIcon(":/tesseract_qt/png/save.png"));
  ui->actionRestore_State->setIcon(QIcon(":/tesseract_qt/png/restore.png"));
  ui->actionCreate_Perspective->setIcon(QIcon(":/tesseract_qt/png/layout.png"));
  connect(ui->actionSave_State, &QAction::triggered, [this]() { d_->saveState(); });
  connect(ui->actionRestore_State, &QAction::triggered, [this]() { d_->restoreState(); });
  connect(ui->actionCreate_Perspective, &QAction::triggered, [this]() { d_->createPerspective(); });
  d_->perspective_list_action = new QWidgetAction(this);
  d_->perspective_comboBox = new QComboBox(this);
  d_->perspective_comboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
  d_->perspective_comboBox->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
  d_->perspective_list_action->setDefaultWidget(d_->perspective_comboBox);
  ui->toolBar->addAction(d_->perspective_list_action);
  connect(d_->perspective_comboBox,
          SIGNAL(activated(const QString&)),
          d_->dock_manager,
          SLOT(openPerspective(const QString&)));

  {
    auto locator = std::make_shared<tesseract_common::TesseractSupportResourceLocator>();
    tesseract_common::fs::path urdf_path = std::string(TESSERACT_SUPPORT_DIR) + "/urdf/lbr_iiwa_14_r820.urdf";
    tesseract_common::fs::path srdf_path = std::string(TESSERACT_SUPPORT_DIR) + "/urdf/lbr_iiwa_14_r820.srdf";

    auto env = std::make_unique<tesseract_environment::Environment>();
    env->init(urdf_path, srdf_path, locator);

    std::string scene_name = env->getName();
    SceneInfo::Ptr scene_info = createScene(scene_name);
    if (scene_info != nullptr)
    {
      auto config = std::make_shared<tesseract_gui::EnvironmentWidgetConfig>();
      config->setEnvironment(std::move(env));

      auto* widget = new tesseract_gui::IgnitionEnvironmentWidget(scene_info->scene_name, *scene_info->entity_manager);
      widget->setConfiguration(std::move(config));
      qobject_cast<QApplication*>(qGuiApp)->installEventFilter(widget);
      connect(widget, SIGNAL(triggerRender()), scene_info->render_widget, SLOT(update()));

      auto* dock_widget = new ads::CDockWidget("Environment");
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

SceneInfo::Ptr TesseractRoboticsStudio::createScene(const std::string& scene_name) const
{
  auto it = d_->scene_infos.find(scene_name);
  if (it != d_->scene_infos.end())
    return nullptr;

  auto scene_info = std::make_shared<SceneInfo>(scene_name);
  scene_info->render_widget = new SimpleRenderWidget(scene_info->scene_name);
  ads::CDockWidget* dock_widget = new ads::CDockWidget(QString::fromStdString(scene_info->scene_name));
  dock_widget->setWidget(scene_info->render_widget);
  dock_widget->setFeature(ads::CDockWidget::DockWidgetDeleteOnClose, true);
  dock_widget->setFeature(ads::CDockWidget::DockWidgetFocusable, true);
  d_->dock_manager->addDockWidgetTab(ads::CenterDockWidgetArea, dock_widget);
  d_->scene_infos[scene_info->scene_name] = scene_info;
  return scene_info;
}

const std::unordered_map<std::string, SceneInfo::Ptr>& TesseractRoboticsStudio::getSceneInfos() const
{
  return d_->scene_infos;
}

}  // namespace tesseract_gui
