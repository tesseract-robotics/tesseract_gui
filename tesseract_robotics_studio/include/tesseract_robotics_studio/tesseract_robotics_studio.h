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
#ifndef TESSERACT_GUI_APP_TESSERACT_ROBOTICS_STUDIO_H
#define TESSERACT_GUI_APP_TESSERACT_ROBOTICS_STUDIO_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <memory>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QMainWindow>

#include <tesseract_ignition/simple_render_widget.h>
#include <tesseract_qt/common/entity_manager.h>

namespace Ui
{
class TesseractRoboticsStudio;
}

namespace tesseract_gui
{
struct TesseractRoboticsStudioPrivate;

struct SceneInfo
{
  using Ptr = std::shared_ptr<SceneInfo>;
  using ConstPtr = std::shared_ptr<const SceneInfo>;

  SceneInfo(std::string scene_name);

  std::string scene_name;
  EntityManager::Ptr entity_manager;
  SimpleRenderWidget* render_widget;
};

class TesseractRoboticsStudio : public QMainWindow
{
  Q_OBJECT

public:
  explicit TesseractRoboticsStudio(QWidget* parent = nullptr);
  ~TesseractRoboticsStudio();

  SceneInfo::Ptr createScene(const std::string& scene_name) const;
  const std::unordered_map<std::string, SceneInfo::Ptr>& getSceneInfos() const;

private:
  std::unique_ptr<Ui::TesseractRoboticsStudio> ui;
  std::unique_ptr<TesseractRoboticsStudioPrivate> d_;

  friend struct TesseractRoboticsStudioPrivate;
};
}  // namespace tesseract_gui

#endif  // TESSERACT_GUI_APP_TESSERACT_ROBOTICS_STUDIO_H
