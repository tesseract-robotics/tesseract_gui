#ifndef TESSERACT_GUI_COMMON_ENTITY_H
#define TESSERACT_GUI_COMMON_ENTITY_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#include <string>
#include <unordered_map>
TESSERACT_COMMON_IGNORE_WARNINGS_POP

namespace tesseract_gui
{
  static const int NULL_ENTITY_ID = -1;
  using EntityID = int;
  using EntityMap = std::unordered_map<std::string, EntityID>;
}

#endif // TESSERACT_GUI_COMMON_ENTITY_H
