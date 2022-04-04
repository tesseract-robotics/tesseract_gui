#include <tesseract_gui/widgets/acm/add_allowed_collision_entry_dialog.h>
#include "ui_add_allowed_collision_entry_dialog.h"
#include <QRegExpValidator>
namespace tesseract_gui
{
AddAllowedCollisionEntryDialog::AddAllowedCollisionEntryDialog(QWidget *parent)
  : QDialog(parent)
  , ui_(std::make_unique<Ui::AddAllowedCollisionEntryDialog>())
{
  ui_->setupUi(this);
  ui_->linkName1LineEdit->setValidator(new QRegExpValidator(QRegExp("\\S*")));
  ui_->linkName2LineEdit->setValidator(new QRegExpValidator(QRegExp("\\S*")));
  connect(ui_->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
  connect(ui_->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

AddAllowedCollisionEntryDialog::~AddAllowedCollisionEntryDialog() = default;

QString AddAllowedCollisionEntryDialog::getLinkName1() const
{
  return ui_->linkName1LineEdit->text();
}

QString AddAllowedCollisionEntryDialog::getLinkName2() const
{
  return ui_->linkName2LineEdit->text();
}

QString AddAllowedCollisionEntryDialog::getReason() const
{
  return ui_->reasonLineEdit->text();
}

}
