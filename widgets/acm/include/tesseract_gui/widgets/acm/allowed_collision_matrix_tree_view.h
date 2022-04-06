#ifndef TESSERACT_GUI_WIDGETS_ACM_ALLOWED_COLLISION_MATRIX_TREE_VIEW_H
#define TESSERACT_GUI_WIDGETS_ACM_ALLOWED_COLLISION_MATRIX_TREE_VIEW_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_common/allowed_collision_matrix.h>
#include <memory>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QTreeView>

namespace tesseract_gui
{
class AllowedCollisionMatrixTreeView : public QTreeView
{
  Q_OBJECT

public:
  explicit AllowedCollisionMatrixTreeView(QWidget *parent = nullptr);
  ~AllowedCollisionMatrixTreeView() override;

  void setModel(QAbstractItemModel *model) override;

public Q_SLOTS:
  void onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
  void mousePressEvent(QMouseEvent *event) override;

Q_SIGNALS:
  void entrySelected(tesseract_common::AllowedCollisionEntries selection);
  void selectedLinksChanged(const std::vector<std::string>& selected_links);
};

}

#endif // TESSERACT_GUI_WIDGETS_ACM_ALLOWED_COLLISION_MATRIX_TREE_VIEW_H
