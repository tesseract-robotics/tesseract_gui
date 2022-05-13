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

#include <tesseract_widgets/common/icon_utils.h>
#include <tesseract_widgets/common/svg_util.h>
namespace tesseract_gui::icons
{
QIcon getAnchorIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/png/anchor.png");
  return icon;
}

QIcon getAxisIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/png/axis.png");
  return icon;
}

QIcon getCalibrationIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/png/calibration.png");
  return icon;
}

QIcon getCapsuleIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/png/capsule.png");
  return icon;
}

QIcon getColorIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/png/color.png");
  return icon;
}

QIcon getConeIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/png/cone.png");
  return icon;
}

QIcon getContinuousIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/png/continuous.png");
  return icon;
}

QIcon getCubeIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/png/cube.png");
  return icon;
}

QIcon getCylinderIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/png/cylinder.png");
  return icon;
}

QIcon getDynamicsIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/png/dynamics.png");
  return icon;
}

QIcon getTrajectoryIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/png/trajectory.png");
  return icon;
}

QIcon getLimitsIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/png/limits.png");
  return icon;
}

QIcon getCommandEntryIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/png/merge.png");
  return icon;
}

QIcon getConvexMeshIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/png/mesh.png");
  return icon;
}

QIcon getNumericIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/png/numeric.png");
  return icon;
}

QIcon getOctreeIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/png/octree.png");
  return icon;
}

QIcon getOrientationIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/png/orientation.png");
  return icon;
}

QIcon getOriginIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/png/origin.png");
  return icon;
}

QIcon getPlaneIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/png/plane.png");
  return icon;
}

QIcon getPluginIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/png/plugin.png");
  return icon;
}

QIcon getPositionIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/png/position.png");
  return icon;
}

QIcon getPrismaticIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/png/prismatic.png");
  return icon;
}

QIcon getSetIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/png/programming.png");
  return icon;
}

QIcon getRevoluteIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/png/revolute.png");
  return icon;
}

QIcon getRGBIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/png/rgb.png");
  return icon;
}

QIcon getRobotArmIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/png/robotic-arm.png");
  return icon;
}

QIcon getSafetyIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/png/safety.png");
  return icon;
}

QIcon getShapesIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/png/shapes.png");
  return icon;
}

QIcon getSphereIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/png/sphere.png");
  return icon;
}

QIcon getMeshIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/png/surface.png");
  return icon;
}

QIcon getTextIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/png/text.png");
  return icon;
}

QIcon getURLIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/png/url.png");
  return icon;
}

QIcon getLogIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/png/log.png");
  return icon;
}

QIcon getTrashIcon()
{
  static QIcon icon = QIcon(LoadSvg(":/tesseract_widgets/svg/trash.svg"));
  return icon;
}

QIcon getPlotIcon()
{
  static QIcon icon = QIcon(LoadSvg(":/tesseract_widgets/svg/plot_image.svg"));
  return icon;
}

QIcon getImportIcon()
{
  static QIcon icon = QIcon(LoadSvg(":/tesseract_widgets/svg/import.svg"));
  return icon;
}

QIcon getSaveIcon()
{
  static QIcon icon = QIcon(LoadSvg(":/tesseract_widgets/svg/save.svg"));
  return icon;
}

QIcon getModelIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/ignition/model.png");
  return icon;
}
QIcon getJointIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/ignition/joint.png");
  return icon;
}
QIcon getLinkIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/ignition/link.png");
  return icon;
}
QIcon getJointVectorIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/ignition/joint_vector.png");
  return icon;
}
QIcon getLinkVectorIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/ignition/link_vector.png");
  return icon;
}
QIcon getVisualIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/ignition/visual.png");
  return icon;
}
QIcon getCollisionIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/ignition/collision.png");
  return icon;
}
QIcon getVisualVectorIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/ignition/visual_vector.png");
  return icon;
}
QIcon getCollisionVectorIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/ignition/collision_vector.png");
  return icon;
}

QIcon getUnknownIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/png/unknown.png");
  return icon;
}
QIcon getJointFixedIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/ignition/joint_fixed.png");
  return icon;
}
QIcon getJointRevoluteIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/ignition/joint_revolute.png");
  return icon;
}
QIcon getJointContinuousIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/ignition/joint_continuous.png");
  return icon;
}
QIcon getJointPrismaticIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/ignition/joint_prismatic.png");
  return icon;
}
QIcon getJointUnknownIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/ignition/joint_unknown.png");
  return icon;
}

QIcon getShowAllLinksIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/ignition/show-link.png");
  return icon;
}

QIcon getHideAllLinksIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/ignition/hide-link.png");
  return icon;
}

QIcon getShowVisualAllLinksIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/ignition/show-visual.png");
  return icon;
}

QIcon getHideVisualAllLinksIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/ignition/hide-visual.png");
  return icon;
}

QIcon getShowCollisionAllLinksIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/ignition/show-collision.png");
  return icon;
}

QIcon getHideCollisionAllLinksIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/ignition/hide-collision.png");
  return icon;
}

QIcon getShowAxisAllLinksIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/png/show-origin.png");
  return icon;
}

QIcon getHideAxisAllLinksIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/png/hide-origin.png");
  return icon;
}

QIcon getSelectAllLinksIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/ignition/select-link.png");
  return icon;
}

QIcon getDeselectAllLinksIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/ignition/deselect-link.png");
  return icon;
}

QIcon getTesseractIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/png/tesseract_logo.png");
  return icon;
}

QIcon getRestoreIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/png/restore.png");
  return icon;
}

QIcon getDateTimeIcon()
{
  static QIcon icon = QIcon(":/tesseract_widgets/png/datetime.png");
  return icon;
}
}  // namespace tesseract_gui::icons
