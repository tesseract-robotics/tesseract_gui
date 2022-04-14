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
#ifndef TESSERACT_WIDGETS_ENVIRONMENT_ENVIRONMENT_WIDGET_H
#define TESSERACT_WIDGETS_ENVIRONMENT_ENVIRONMENT_WIDGET_H

#include <QWidget>
#include <memory>
#include <tesseract_environment/environment.h>

namespace Ui {
class EnvironmentWidget;
}

namespace tesseract_gui
{
class EnvironmentWidgetConfig;

class EnvironmentWidget : public QWidget
{
  Q_OBJECT

public:
  explicit EnvironmentWidget(QWidget *parent = nullptr);
  ~EnvironmentWidget() override;

  /**
   * @brief Configure the environment widget
   * @param config The environment widgets configuration
   */
  void setConfiguration(std::shared_ptr<EnvironmentWidgetConfig> config);

  /**
   * @brief The the environment
   * @return The environment
   */
  const tesseract_environment::Environment& environment() const;
  tesseract_environment::Environment& environment();

  /**
   * @brief The the environment
   * @return The environment
   */
  tesseract_environment::Environment::ConstPtr getEnvironment() const;
  tesseract_environment::Environment::Ptr getEnvironment();

Q_SIGNALS:
  void configurationSet(EnvironmentWidgetConfig& config);
  void environmentSet(const tesseract_environment::Environment& env);
  void environmentChanged(const tesseract_environment::Environment& env);
  void environmentCurrentStateChanged(const tesseract_environment::Environment& env);
  void linkVisibleChanged(const std::string& link_name, bool visible);
  void linkCollisionVisibleChanged(const std::string& link_name, bool visible);
  void linkVisualVisibleChanged(const std::string& link_name, bool visible);
  void selectedLinksChanged(const std::vector<std::string>& selected_links);
  void triggerRender();

public Q_SLOTS:
  virtual void onModelsUpdated();
  virtual void onRender();

protected:
  std::unique_ptr<Ui::EnvironmentWidget> ui;
  std::shared_ptr<EnvironmentWidgetConfig> config_;

  void updateModels();

};
}

#endif // TESSERACT_WIDGETS_ENVIRONMENT_ENVIRONMENT_WIDGET_H
