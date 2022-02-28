#include <tesseract_gui/environment/environment_widget.h>
#include "ui_environment_widget.h"

#include <tesseract_gui/scene_graph/scene_graph_standard_item.h>
#include <tesseract_gui/acm/allowed_collision_matrix_model.h>
#include <unordered_map>
#include <QStandardItemModel>
#include <QStringListModel>

namespace tesseract_gui
{
struct EnvironmentWidgetImpl
{
  QMap<QString, tesseract_environment::Environment::ConstPtr> environments;
  QStringListModel names_model;
  QStandardItemModel scene_model;
  AllowedCollisionMatrixModel acm_model;
};

EnvironmentWidget::EnvironmentWidget(QWidget *parent)
  : QWidget(parent)
  , ui(std::make_unique<Ui::EnvironmentWidget>())
  , data_(std::make_unique<EnvironmentWidgetImpl>())
{
  ui->setupUi(this);
  ui->env_combo_box->setModel(&data_->names_model);
  ui->scene_tree_view->setModel(&data_->scene_model);
  ui->acm_tree_view->setModel(&data_->acm_model);

  connect(ui->env_combo_box, SIGNAL(currentTextChanged(QString)), this, SLOT(onCurrentEnvironmentChanged(QString)));
}

EnvironmentWidget::~EnvironmentWidget() = default;

void EnvironmentWidget::addEnvironment(tesseract_environment::Environment::ConstPtr env)
{
  QString name = QString::fromStdString(env->getName());
  data_->environments[name] = std::move(env);

  ui->env_combo_box->blockSignals(true);
  QStringList list = data_->names_model.stringList();
  QString current = (list.empty()) ? name : ui->env_combo_box->currentText();
  list.append(name);
  data_->names_model.setStringList(list);
  ui->env_combo_box->setCurrentText(current);
  ui->env_combo_box->blockSignals(false);
  if (list.size() == 1)
    onCurrentEnvironmentChanged(current);
}

void EnvironmentWidget::removeEnvironment(const QString& env_name)
{
  QString current = ui->env_combo_box->currentText();
  auto it =  data_->environments.find(env_name);
  if (it != data_->environments.end())
  {
    data_->environments.erase(it);
    QStringList list = data_->names_model.stringList();
    list.removeAll(env_name);
    data_->names_model.setStringList(list);
    if (current == env_name)
      ui->env_combo_box->setCurrentIndex(0);
    else
      ui->env_combo_box->setCurrentText(current);
  }
}

void EnvironmentWidget::onCurrentEnvironmentChanged(const QString& env_name)
{
  auto it = data_->environments.find(env_name);
  if (it != data_->environments.end())
  {
    data_->scene_model.clear();
    auto* item = new tesseract_gui::SceneGraphStandardItem(it.value()->getSceneGraph()->clone());
    data_->scene_model.appendRow(item);

    data_->acm_model.setAllowedCollisionMatrix(*it.value()->getAllowedCollisionMatrix());
  }
}

}
