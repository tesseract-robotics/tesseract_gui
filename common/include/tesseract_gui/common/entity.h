#ifndef TESSERACT_GUI_COMMON_ENTITY_H
#define TESSERACT_GUI_COMMON_ENTITY_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#include <string>
#include <unordered_map>
TESSERACT_COMMON_IGNORE_WARNINGS_POP

namespace tesseract_gui
{
  using EntityID = int;
  using EntityMap = std::unordered_map<std::string, EntityID>;

  static const int NULL_ENTITY_ID = -1;
  static const EntityMap NULL_ENTITY_MAP;
}

#endif // TESSERACT_GUI_COMMON_ENTITY_H
