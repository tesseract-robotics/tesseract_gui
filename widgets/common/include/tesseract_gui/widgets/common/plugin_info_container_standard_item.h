#ifndef TESSERACT_GUI_WIDGETS_COMMON_PLUGIN_INFO_CONTAINER_STANDARD_ITEM_H
#define TESSERACT_GUI_WIDGETS_COMMON_PLUGIN_INFO_CONTAINER_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_common/types.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class PluginInfoContainerStandardItem : public QStandardItem
{
public:
  PluginInfoContainerStandardItem(tesseract_common::PluginInfoContainer plugin_info_container);
  explicit PluginInfoContainerStandardItem(const QString &text, tesseract_common::PluginInfoContainer plugin_info_container);
  PluginInfoContainerStandardItem(const QIcon &icon, const QString &text, tesseract_common::PluginInfoContainer plugin_info_container);
  int type() const override;

  tesseract_common::PluginInfoContainer plugin_info_container;

private:
  void ctor();
};
}

#endif // TESSERACT_GUI_WIDGETS_COMMON_PLUGIN_INFO_CONTAINER_STANDARD_ITEM_H
