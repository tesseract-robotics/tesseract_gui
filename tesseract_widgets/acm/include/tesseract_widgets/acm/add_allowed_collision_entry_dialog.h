#ifndef TESSERACT_WIDGETS_ACM_ADD_ALLOWED_COLLISION_ENTRY_DIALOG_H
#define TESSERACT_WIDGETS_ACM_ADD_ALLOWED_COLLISION_ENTRY_DIALOG_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <memory>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QDialog>

namespace Ui {
class AddAllowedCollisionEntryDialog;
}

namespace tesseract_gui
{
class AddAllowedCollisionEntryDialog : public QDialog
{
  Q_OBJECT

public:
  explicit AddAllowedCollisionEntryDialog(QWidget *parent = nullptr);
  ~AddAllowedCollisionEntryDialog();

  QString getLinkName1() const;
  QString getLinkName2() const;
  QString getReason() const;

private:
  std::unique_ptr<Ui::AddAllowedCollisionEntryDialog> ui_;
};
}

#endif // TESSERACT_WIDGETS_ACM_ADD_ALLOWED_COLLISION_ENTRY_DIALOG_H
