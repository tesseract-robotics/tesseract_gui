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
#ifndef TESSERACT_WIDGETS_COLLISION_CONTACT_RESULTS_WIDGET_H
#define TESSERACT_WIDGETS_COLLISION_CONTACT_RESULTS_WIDGET_H

#include <QWidget>
#include <memory>
#include <functional>
#include <tesseract_collision/core/types.h>

namespace Ui
{
class ContactResultsWidget;
}

namespace tesseract_gui
{
using ContactTestFn =
    std::function<tesseract_collision::ContactResultMap(const tesseract_collision::ContactManagerConfig& config,
                                                        const tesseract_collision::ContactRequest& request)>;

class ContactResultsModel;
struct ContactResultsWidgetImpl;

class ContactResultsWidget : public QWidget
{
  Q_OBJECT

public:
  explicit ContactResultsWidget(QWidget* parent = nullptr);
  ~ContactResultsWidget();

  void setModel(ContactResultsModel* model);
  void setContactTestFn(ContactTestFn contact_test_fn);

protected Q_SLOTS:
  void onComputeClicked();

private:
  std::unique_ptr<Ui::ContactResultsWidget> ui;
  std::unique_ptr<ContactResultsWidgetImpl> data_;
};
}  // namespace tesseract_gui

#endif  // TESSERACT_WIDGETS_COLLISION_CONTACT_RESULTS_WIDGET_H
