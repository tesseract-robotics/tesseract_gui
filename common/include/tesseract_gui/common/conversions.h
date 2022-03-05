#ifndef TESSERACT_GUI_COMMON_CONVERSIONS_H
#define TESSERACT_GUI_COMMON_CONVERSIONS_H

#include <ignition/common/KeyEvent.hh>
#include <ignition/common/MouseEvent.hh>
#include <ignition/common/Time.hh>
#include <ignition/math/Color.hh>
#include <ignition/math/Vector2.hh>
#include <ignition/math/Vector3.hh>

#include <QtCore>
#include <QColor>
#include <QVector3D>
#include <QInputEvent>

namespace tesseract_gui
{
/// \brief Return the equivalent Qt color
/// \param[in] _color Ignition color to convert
/// \return Qt color value
QColor convert(const ignition::math::Color &_color);

/// \brief Return the equivalent Ignition color
/// \param[in] _color Qt color to convert
/// \return Ignition color value
ignition::math::Color convert(const QColor &_color);

/// \brief Return the equivalent QPointF.
/// \param[in] _pt Ignition vector to convert.
/// \return QPointF.
QPointF convert(const ignition::math::Vector2d &_pt);

/// \brief Return the equivalent Ignition vector.
/// \param[in] _pt QPointF to convert
/// \return Ignition Vector2d.
ignition::math::Vector2d convert(const QPointF &_pt);

/// \brief Return the equivalent Qt vector 3d.
/// \param[in] _vec Ignition vector 3d to convert.
/// \return Qt vector 3d value.
QVector3D convert(const ignition::math::Vector3d &_vec);

/// \brief Return the equivalent Ignition vector 3d.
/// \param[in] _vec Qt vector 3d to convert.
/// \return Ignition vector 3d value
ignition::math::Vector3d convert(const QVector3D &_vec);

/// \brief Return the equivalent Ignition mouse event.
///
/// Note that there isn't a 1-1 mapping between these types, so fields such
/// as common::MouseEvent::PressPos need to be set afterwards.
/// \param[in] _e Qt mouse event
/// \return Ignition mouse event
ignition::common::MouseEvent convert(const QMouseEvent &_e);

/// \brief Return the equivalent Ignition mouse event.
///
/// Note that there isn't a 1-1 mapping between these types.
/// \param[in] _e Qt wheel event
/// \return Ignition mouse event
ignition::common::MouseEvent convert(const QWheelEvent &_e);

/// \brief Return the equivalent ignition key event.
///
/// \param[in] _e Qt key event
/// \return Ignition key event
ignition::common::KeyEvent convert(const QKeyEvent &_e);
}

#endif // TESSERACT_GUI_COMMON_CONVERSIONS_H
