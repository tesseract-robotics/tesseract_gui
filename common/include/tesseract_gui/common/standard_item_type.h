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
  SRDF                    = QStandardItem::UserType + 24,
  OPW_PARAMS              = SRDF + 1,
  GROUP_TCPS              = SRDF + 2,
  TCP                     = SRDF + 3,
  CHAIN_GROUP             = SRDF + 4,
  JOINT_GROUP             = SRDF + 5,
  LINK_GROUP              = SRDF + 6,
  GROUP_JOINT_STATES      = SRDF + 7,
  JOINT_STATE             = SRDF + 8,

  // Scene State
  SCENE_STATE                 = SRDF + 9,
//  SCENE_STATE_JOINT_VALUE     = SCENE_STATE + 1,
//  SCENE_STATE_LINK_TRANSFORM  = SCENE_STATE + 2,
//  SCENE_STATE_JOINT_TRANSFORM = SCENE_STATE + 3
};
}

#endif // TESSERACT_GUI_COMMON_STANDARD_ITEMS_TYPES_H
