#ifndef TESSERACT_WIDGETS_COMMON_PLUGIN_INFO_STANDARD_ITEM_H
#define TESSERACT_WIDGETS_COMMON_PLUGIN_INFO_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_common/types.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class PluginInfoStandardItem : public QStandardItem
{
public:
  PluginInfoStandardItem(tesseract_common::PluginInfo plugin_info);
  explicit PluginInfoStandardItem(const QString &text, tesseract_common::PluginInfo plugin_info);
  PluginInfoStandardItem(const QIcon &icon, const QString &text, tesseract_common::PluginInfo plugin_info);
  int type() const override;

  tesseract_common::PluginInfo plugin_info;

private:
  void ctor();
};
}

#endif // TESSERACT_WIDGETS_COMMON_PLUGIN_INFO_STANDARD_ITEM_H
