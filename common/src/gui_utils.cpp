#include <tesseract_gui/common/gui_utils.h>

namespace tesseract_gui
{
QApplication* getApp()
{
  return qobject_cast<QApplication *>(qGuiApp);
}
}
