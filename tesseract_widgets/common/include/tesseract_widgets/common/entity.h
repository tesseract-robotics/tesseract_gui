#ifndef TESSERACT_GUI_COMMON_ENTITY_H
#define TESSERACT_GUI_COMMON_ENTITY_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#include <string>
#include <vector>
#include <unordered_map>
TESSERACT_COMMON_IGNORE_WARNINGS_POP

namespace tesseract_gui
{
  struct Entity
  {
    int id{-1};
    std::string unique_name;
  };

  using EntityMap = std::unordered_map<std::string, Entity>;
  using EntityVector = std::vector<Entity>;
}

#endif // TESSERACT_GUI_COMMON_ENTITY_H
