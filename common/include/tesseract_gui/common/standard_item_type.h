#ifndef TESSERACT_GUI_COMMON_STANDARD_ITEMS_TYPES_H
#define TESSERACT_GUI_COMMON_STANDARD_ITEMS_TYPES_H

#include <QStandardItem>

namespace tesseract_gui
{
enum class StandardItemType : int
{
  // Scene Graph Types
  MATERIAL                = QStandardItem::UserType + 1,
  INERTIAL                = QStandardItem::UserType + 2,
  ORIGIN                  = QStandardItem::UserType + 3,
  GEOMETRY                = QStandardItem::UserType + 4,
  VISUAL                  = QStandardItem::UserType + 5,
  COLLISION               = QStandardItem::UserType + 6,
  LINK                    = QStandardItem::UserType + 7,
  DYNAMICS                = QStandardItem::UserType + 8,
  LIMITS                  = QStandardItem::UserType + 9,
  SAFETY                  = QStandardItem::UserType + 10,
  CALIBRATION             = QStandardItem::UserType + 11,
  MIMIC                   = QStandardItem::UserType + 12,
  JOINT                   = QStandardItem::UserType + 13,
  SCENE_GRAPH             = QStandardItem::UserType + 15,
  BOX                     = QStandardItem::UserType + 16,
  CAPSULE                 = QStandardItem::UserType + 17,
  CONE                    = QStandardItem::UserType + 18,
  CYLINDER                = QStandardItem::UserType + 19,
  PLANE                   = QStandardItem::UserType + 20,
  SPHERE                  = QStandardItem::UserType + 21,
  POLYGON_MESH            = QStandardItem::UserType + 22,
  OCTREE                  = QStandardItem::UserType + 23,

  // SRDF Types
  OPW_PARAMS              = QStandardItem::UserType + 24,
  TCP                     = QStandardItem::UserType + 25,
  CHAIN_GROUP             = QStandardItem::UserType + 26,
  JOINT_GROUP             = QStandardItem::UserType + 27,
  LINK_GROUP              = QStandardItem::UserType + 28,
  GROUPS_JOINT_STATE      = QStandardItem::UserType + 29
};
}

#endif // TESSERACT_GUI_COMMON_STANDARD_ITEMS_TYPES_H
