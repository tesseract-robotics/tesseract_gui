#ifndef TESSERACT_GUI_ALLOWED_COLLISION_MATRIX_WIDGET_H
#define TESSERACT_GUI_ALLOWED_COLLISION_MATRIX_WIDGET_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_gui/acm/allowed_collision_matrix_model.h>
#include <tesseract_common/allowed_collision_matrix.h>
#include <memory>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QWidget>
#include <QItemSelection>

namespace Ui {
class AllowedCollisionMatrixWidget;
}

namespace tesseract_gui
{
class AllowedCollisionMatrixWidget : public QWidget
{
  Q_OBJECT

public:
  explicit AllowedCollisionMatrixWidget(QWidget *parent = nullptr);
  ~AllowedCollisionMatrixWidget();

  void setModel(tesseract_gui::AllowedCollisionMatrixModel* model);

public Q_SLOTS:
  void onRemoveButtonClicked();
  void onAddButtonClicked();
  void onGenerateButtonClicked();
  void onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

Q_SIGNALS:
  void generateClicked(int resolution);
  void entrySelected(tesseract_common::AllowedCollisionEntries selection);

private:
  std::unique_ptr<Ui::AllowedCollisionMatrixWidget> ui_;
};

}

#endif // TESSERACT_GUI_ALLOWED_COLLISION_MATRIX_WIDGET_H
