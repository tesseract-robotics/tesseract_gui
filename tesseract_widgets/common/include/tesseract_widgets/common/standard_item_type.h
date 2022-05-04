/**
 * @author Levi Armstrong <levi.armstrong@gmail.com>
 *
 * @copyright Copyright (C) 2022 Levi Armstrong <levi.armstrong@gmail.com>
 *
 * @par License
 * GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
 * @par
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 * @par
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * @par
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#ifndef TESSERACT_GUI_COMMON_STANDARD_ITEMS_TYPES_H
#define TESSERACT_GUI_COMMON_STANDARD_ITEMS_TYPES_H

#include <QStandardItem>

namespace tesseract_gui
{
// clang-format off
enum class StandardItemType : int
{
  // Common Types
  PLUGIN_INFO                  = QStandardItem::UserType + 1,
  PLUGIN_INFO_CONTAINER        = QStandardItem::UserType + 2,
  KINEMATICS_PLUGIN_INFO       = QStandardItem::UserType + 3,
  CONTACT_MANAGERS_PLUGIN_INFO = QStandardItem::UserType + 4,
  CALIBRATION_INFO             = QStandardItem::UserType + 5,
  COLLISION_MARGIN_DATA        = QStandardItem::UserType + 6,
  PAIRS_COLLISION_MARGIN_DATA  = QStandardItem::UserType + 7,
  TRANSFORM                    = QStandardItem::UserType + 8,
  TRANSFORM_POSITION           = QStandardItem::UserType + 9,
  TRANSFORM_QUATERNION         = QStandardItem::UserType + 10,
  NAMESPACE                    = QStandardItem::UserType + 11,
  VECTOR_DOUBLE                = QStandardItem::UserType + 12,
  VECTOR_STRING                = QStandardItem::UserType + 13,
  JOINT_STATE                  = QStandardItem::UserType + 14,
  MANIPULATOR_INFO             = QStandardItem::UserType + 15,
  COMMON_TYPES_END             = QStandardItem::UserType + 16,

  // Joint Trajectory Set Types
  JOINT_TRAJECTORY_SET            = COMMON_TYPES_END + 1,
  JOINT_TRAJECTORY_SET_TRAJECTORY = COMMON_TYPES_END + 2,
  JOINT_TRAJECTORY_SET_STATE      = COMMON_TYPES_END + 3,
  JOINT_TRAJECTORY_SET_END        = COMMON_TYPES_END + 4,

  // Scene Graph Types
  MATERIAL                = JOINT_TRAJECTORY_SET_END + 1,
  INERTIAL                = JOINT_TRAJECTORY_SET_END + 2,
  ORIGIN                  = JOINT_TRAJECTORY_SET_END + 3,
  GEOMETRY                = JOINT_TRAJECTORY_SET_END + 4,
  VISUAL                  = JOINT_TRAJECTORY_SET_END + 5,
  COLLISION               = JOINT_TRAJECTORY_SET_END + 6,
  LINK                    = JOINT_TRAJECTORY_SET_END + 7,
  DYNAMICS                = JOINT_TRAJECTORY_SET_END + 8,
  LIMITS                  = JOINT_TRAJECTORY_SET_END + 9,
  SAFETY                  = JOINT_TRAJECTORY_SET_END + 10,
  CALIBRATION             = JOINT_TRAJECTORY_SET_END + 11,
  MIMIC                   = JOINT_TRAJECTORY_SET_END + 12,
  JOINT                   = JOINT_TRAJECTORY_SET_END + 13,
  SCENE_GRAPH             = JOINT_TRAJECTORY_SET_END + 15,
  BOX                     = JOINT_TRAJECTORY_SET_END + 16,
  CAPSULE                 = JOINT_TRAJECTORY_SET_END + 17,
  CONE                    = JOINT_TRAJECTORY_SET_END + 18,
  CYLINDER                = JOINT_TRAJECTORY_SET_END + 19,
  PLANE                   = JOINT_TRAJECTORY_SET_END + 20,
  SPHERE                  = JOINT_TRAJECTORY_SET_END + 21,
  POLYGON_MESH            = JOINT_TRAJECTORY_SET_END + 22,
  OCTREE                  = JOINT_TRAJECTORY_SET_END + 23,
  VISUALS                 = JOINT_TRAJECTORY_SET_END + 24,
  COLLISIONS              = JOINT_TRAJECTORY_SET_END + 25,
  SCENE_GRAPH_TYPES_END   = JOINT_TRAJECTORY_SET_END + 26,

  // SRDF Types
  OPW_PARAMS              = SCENE_GRAPH_TYPES_END + 1,
  GROUP_TCPS              = SCENE_GRAPH_TYPES_END + 2,
  TCP                     = SCENE_GRAPH_TYPES_END + 3,
  CHAIN_GROUP             = SCENE_GRAPH_TYPES_END + 4,
  JOINT_GROUP             = SCENE_GRAPH_TYPES_END + 5,
  LINK_GROUP              = SCENE_GRAPH_TYPES_END + 6,
  GROUP_JOINT_STATES      = SCENE_GRAPH_TYPES_END + 7,
  GROUP_JOINT_STATE       = SCENE_GRAPH_TYPES_END + 8,
  KINEMATICS_INFO         = SCENE_GRAPH_TYPES_END + 9,
  SRDF_TYPES_END          = SCENE_GRAPH_TYPES_END + 10,

  // Environment Command Types
  ENV_COMMAND_UNINITIALIZED                         = SCENE_GRAPH_TYPES_END + 1,
  ENV_COMMAND_ADD_LINK                              = SCENE_GRAPH_TYPES_END + 2,
  ENV_COMMAND_MOVE_LINK                             = SCENE_GRAPH_TYPES_END + 3,
  ENV_COMMAND_MOVE_JOINT                            = SCENE_GRAPH_TYPES_END + 4,
  ENV_COMMAND_REMOVE_LINK                           = SCENE_GRAPH_TYPES_END + 5,
  ENV_COMMAND_REMOVE_JOINT                          = SCENE_GRAPH_TYPES_END + 6,
  ENV_COMMAND_CHANGE_LINK_ORIGIN                    = SCENE_GRAPH_TYPES_END + 7,
  ENV_COMMAND_CHANGE_JOINT_ORIGIN                   = SCENE_GRAPH_TYPES_END + 8,
  ENV_COMMAND_CHANGE_LINK_COLLISION_ENABLED         = SCENE_GRAPH_TYPES_END + 9,
  ENV_COMMAND_CHANGE_LINK_VISIBILITY                = SCENE_GRAPH_TYPES_END + 10,
  ENV_COMMAND_ADD_ALLOWED_COLLISION                 = SCENE_GRAPH_TYPES_END + 11,
  ENV_COMMAND_REMOVE_ALLOWED_COLLISION              = SCENE_GRAPH_TYPES_END + 12,
  ENV_COMMAND_REMOVE_ALLOWED_COLLISION_LINK         = SCENE_GRAPH_TYPES_END + 13,
  ENV_COMMAND_ADD_SCENE_GRAPH                       = SCENE_GRAPH_TYPES_END + 14,
  ENV_COMMAND_CHANGE_JOINT_POSITION_LIMITS          = SCENE_GRAPH_TYPES_END + 15,
  ENV_COMMAND_CHANGE_JOINT_VELOCITY_LIMITS          = SCENE_GRAPH_TYPES_END + 16,
  ENV_COMMAND_CHANGE_JOINT_ACCELERATION_LIMITS      = SCENE_GRAPH_TYPES_END + 17,
  ENV_COMMAND_ADD_KINEMATICS_INFORMATION            = SCENE_GRAPH_TYPES_END + 18,
  ENV_COMMAND_REPLACE_JOINT                         = SCENE_GRAPH_TYPES_END + 19,
  ENV_COMMAND_CHANGE_COLLISION_MARGINS              = SCENE_GRAPH_TYPES_END + 20,
  ENV_COMMAND_ADD_CONTACT_MANAGERS_PLUGIN_INFO      = SCENE_GRAPH_TYPES_END + 21,
  ENV_COMMAND_SET_ACTIVE_DISCRETE_CONTACT_MANAGER   = SCENE_GRAPH_TYPES_END + 22,
  ENV_COMMAND_SET_ACTIVE_CONTINUOUS_CONTACT_MANAGER = SCENE_GRAPH_TYPES_END + 23,
  ENV_COMMAND_COMMANDS                              = SCENE_GRAPH_TYPES_END + 24,
  ENV_COMMAND_TYPES_END                             = SCENE_GRAPH_TYPES_END + 25,

  // Collision Types
  COLLISION_CONTACT_RESULT                          = ENV_COMMAND_TYPES_END + 1,
  COLLISION_CONTACT_REQUEST                         = ENV_COMMAND_TYPES_END + 2,
  COLLISION_CONTACT_DETAILS                         = ENV_COMMAND_TYPES_END + 3,
  COLLISION_COLLISION_MARGIN_DATA                   = ENV_COMMAND_TYPES_END + 4,
  COLLISION_TYPES_END                               = ENV_COMMAND_TYPES_END + 5,

  // COMMAND_LANGUAGE Types
  CL_CARTESIAN_WAYPOINT                             = COLLISION_TYPES_END + 1,
  CL_JOINT_WAYPOINT                                 = COLLISION_TYPES_END + 2,
  CL_STATE_WAYPOINT                                 = COLLISION_TYPES_END + 3,
  CL_NULL_WAYPOINT                                  = COLLISION_TYPES_END + 4,
  CL_MOVE_INSTRUCTION                               = COLLISION_TYPES_END + 5,
  CL_SET_ANALOG_INSTRUCTION                         = COLLISION_TYPES_END + 6,
  CL_SET_TOOL_INSTRUCTION                           = COLLISION_TYPES_END + 7,
  CL_TIMER_INSTRUCTION                              = COLLISION_TYPES_END + 8,
  CL_WAIT_INSTRUCTION                               = COLLISION_TYPES_END + 9,
  CL_COMPOSITE_INSTRUCTION                          = COLLISION_TYPES_END + 10,
  CL_VECTOR_INSTRUCTION                             = COLLISION_TYPES_END + 11,
  CL_NULL_INSTRUCTION                               = COLLISION_TYPES_END + 12,
  CL_INSTRUCTION                                    = COLLISION_TYPES_END + 13,
  CL_PLAN_INSTRUCTION                               = COLLISION_TYPES_END + 14,
  CL_TYPES_END                                      = COLLISION_TYPES_END + 15,
};
// clang-format on
}  // namespace tesseract_gui

#endif  // TESSERACT_GUI_COMMON_STANDARD_ITEMS_TYPES_H
