#ifndef TESSERACT_GUI_WIDGETS_COMMON_CONTACT_MANAGERS_PLUGIN_INFO_STANDARD_ITEM_H
#define TESSERACT_GUI_WIDGETS_COMMON_CONTACT_MANAGERS_PLUGIN_INFO_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_common/types.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class ContactManagersPluginInfoStandardItem : public QStandardItem
{
public:
  ContactManagersPluginInfoStandardItem(tesseract_common::ContactManagersPluginInfo plugin_info);
  explicit ContactManagersPluginInfoStandardItem(const QString &text, tesseract_common::ContactManagersPluginInfo plugin_info);
  ContactManagersPluginInfoStandardItem(const QIcon &icon, const QString &text, tesseract_common::ContactManagersPluginInfo plugin_info);
  int type() const override;

  tesseract_common::ContactManagersPluginInfo plugin_info;

private:
  void ctor();
};
}

#endif // TESSERACT_GUI_WIDGETS_COMMON_CONTACT_MANAGERS_PLUGIN_INFO_STANDARD_ITEM_H
