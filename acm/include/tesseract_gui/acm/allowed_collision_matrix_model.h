#ifndef TESSERACT_GUI_ALLOWED_COLLISION_MATRIX_MODEL_H
#define TESSERACT_GUI_ALLOWED_COLLISION_MATRIX_MODEL_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_common/allowed_collision_matrix.h>
#include <QStandardItemModel>
#include <QMetaType>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

namespace tesseract_gui
{
class AllowedCollisionMatrixModel : public QStandardItemModel
{
    Q_OBJECT
public:
  enum AllowedCollisionMatrixRoles {
      Link1Role = Qt::UserRole + 1,
      Link2Role = Qt::UserRole + 2,
      ReasonRole = Qt::UserRole + 3
  };

  AllowedCollisionMatrixModel(QObject *parent = nullptr);
  AllowedCollisionMatrixModel(const AllowedCollisionMatrixModel &other);
  AllowedCollisionMatrixModel &operator=(const AllowedCollisionMatrixModel &other);
  ~AllowedCollisionMatrixModel() override = default;

  Q_INVOKABLE void setAllowedCollisionMatrix(const tesseract_common::AllowedCollisionMatrix& acm);
  Q_INVOKABLE void add(const QString& link1_name, const QString& link2_name, const QString& reason);
  Q_INVOKABLE void clear();

  QHash<int, QByteArray> roleNames() const override;

  bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;


  tesseract_common::AllowedCollisionMatrix getAllowedCollisionMatrix() const;

public Q_SLOTS:
  void onEntrySelected(int row);

Q_SIGNALS:

  void entryRemoved(QString link1_name, QString link2_name);
  void entryAdded(QString link1_name, QString link2_name, QString reason);
  void entrySelected(QString link1_name, QString link2_name, QString reason);

private:
  tesseract_common::AllowedCollisionMatrix acm_;
};
}
Q_DECLARE_METATYPE(tesseract_gui::AllowedCollisionMatrixModel)

#endif // TESSERACT_GUI_ALLOWED_COLLISION_MATRIX_MODEL_H
