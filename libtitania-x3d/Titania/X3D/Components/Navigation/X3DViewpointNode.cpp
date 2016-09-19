/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstraße 31a, Leipzig, Germany 2011.
 *
 * All rights reserved. Holger Seelig <holger.seelig@yahoo.de>.
 *
 * THIS IS UNPUBLISHED SOURCE CODE OF create3000.
 *
 * The copyright notice above does not evidence any actual of intended
 * publication of such source code, and is an unpublished work by create3000.
 * This material contains CONFIDENTIAL INFORMATION that is the property of
 * create3000.
 *
 * No permission is granted to copy, distribute, or create derivative works from
 * the contents of this software, in whole or in part, without the prior written
 * permission of create3000.
 *
 * NON-MILITARY USE ONLY
 *
 * All create3000 software are effectively free software with a non-military use
 * restriction. It is free. Well commented source is provided. You may reuse the
 * source in any way you please with the exception anything that uses it must be
 * marked to indicate is contains 'non-military use only' components.
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright 1999, 2016 Holger Seelig <holger.seelig@yahoo.de>.
 *
 * This file is part of the Titania Project.
 *
 * Titania is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License version 3 only, as published by the
 * Free Software Foundation.
 *
 * Titania is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License version 3 for more
 * details (a copy is included in the LICENSE file that accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version 3
 * along with Titania.  If not, see <http://www.gnu.org/licenses/gpl.html> for a
 * copy of the GPLv3 License.
 *
 * For Silvio, Joy and Adi.
 *
 ******************************************************************************/

#include "X3DViewpointNode.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/BindableNodeList.h"
#include "../../Execution/BindableNodeStack.h"
#include "../../Execution/X3DScene.h"

#include "../Interpolation/EaseInEaseOut.h"
#include "../Interpolation/PositionInterpolator.h"
#include "../Interpolation/OrientationInterpolator.h"
#include "../Layering/X3DLayerNode.h"
#include "../Layering/X3DViewportNode.h"
#include "../Navigation/NavigationInfo.h"
#include "../Time/TimeSensor.h"

namespace titania {
namespace X3D {

static constexpr Vector3d yAxis (0, 1, 0);
static constexpr Vector3d zAxis (0, 0, 1);

X3DViewpointNode::Fields::Fields () :
	           orientation (new SFRotation ()),
	                  jump (new SFBool (true)),
	        positionOffset (),
	     orientationOffset (),
	           scaleOffset (1, 1, 1),
	scaleOrientationOffset (),
	centerOfRotationOffset (),
	      fieldOfViewScale (1)
{ }

X3DViewpointNode::X3DViewpointNode () :
	             X3DBindableNode (),
	          X3DViewpointObject (),
	                      fields (),
	        transformationMatrix (),
	           cameraSpaceMatrix (1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 10, 1),
	    inverseCameraSpaceMatrix (1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, -10, 1),
	                  timeSensor (new TimeSensor (getBrowser () -> getPrivateScene ())),
	               easeInEaseOut (new EaseInEaseOut (getBrowser () -> getPrivateScene ())),
	        positionInterpolator (new PositionInterpolator (getBrowser () -> getPrivateScene ())),
	     orientationInterpolator (new OrientationInterpolator (getBrowser () -> getPrivateScene ())),
	           scaleInterpolator (new PositionInterpolator (getBrowser () -> getPrivateScene ())),
	scaleOrientationInterpolator (new OrientationInterpolator (getBrowser () -> getPrivateScene ())),
	                lockToCamera ()
{
	addType (X3DConstants::X3DViewpointNode);

	addChildren (positionOffset (),
	             orientationOffset (),
	             scaleOffset (),
	             scaleOrientationOffset (),
	             centerOfRotationOffset (),
	             fieldOfViewScale (),
	             timeSensor,
	             easeInEaseOut,
	             positionInterpolator,
	             orientationInterpolator,
	             scaleInterpolator,
	             scaleOrientationInterpolator,
	             lockToCamera);
}

void
X3DViewpointNode::initialize ()
{
	X3DBindableNode::initialize ();
	X3DViewpointObject::initialize ();

	timeSensor -> stopTime () = 1;
	timeSensor -> setup ();

	easeInEaseOut -> key ()           = { 0, 1 };
	easeInEaseOut -> easeInEaseOut () = { SFVec2f (0, 0), SFVec2f (0, 0) };
	easeInEaseOut -> setup ();

	positionInterpolator         -> key () = { 0, 1 };
	orientationInterpolator      -> key () = { 0, 1 };
	scaleInterpolator            -> key () = { 0, 1 };
	scaleOrientationInterpolator -> key () = { 0, 1 };

	positionInterpolator         -> setup ();
	orientationInterpolator      -> setup ();
	scaleInterpolator            -> setup ();
	scaleOrientationInterpolator -> setup ();

	timeSensor -> fraction_changed () .addInterest (easeInEaseOut -> set_fraction ());

	easeInEaseOut -> modifiedFraction_changed () .addInterest (positionInterpolator         -> set_fraction ());
	easeInEaseOut -> modifiedFraction_changed () .addInterest (orientationInterpolator      -> set_fraction ());
	easeInEaseOut -> modifiedFraction_changed () .addInterest (scaleInterpolator            -> set_fraction ());
	easeInEaseOut -> modifiedFraction_changed () .addInterest (scaleOrientationInterpolator -> set_fraction ());

	positionInterpolator         -> value_changed () .addInterest (this, &X3DViewpointNode::set_positionOffset);
	orientationInterpolator      -> value_changed () .addInterest (orientationOffset ());
	scaleInterpolator            -> value_changed () .addInterest (this, &X3DViewpointNode::set_scaleOffset);
	scaleOrientationInterpolator -> value_changed () .addInterest (scaleOrientationOffset ());

	isBound () .addInterest (this, &X3DViewpointNode::set_bind_);
}

void
X3DViewpointNode::setExecutionContext (X3DExecutionContext* const executionContext)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	timeSensor                   -> setExecutionContext (executionContext -> getBrowser () -> getPrivateScene ());
	easeInEaseOut                -> setExecutionContext (executionContext -> getBrowser () -> getPrivateScene ());
	positionInterpolator         -> setExecutionContext (executionContext -> getBrowser () -> getPrivateScene ());
	orientationInterpolator      -> setExecutionContext (executionContext -> getBrowser () -> getPrivateScene ());
	scaleInterpolator            -> setExecutionContext (executionContext -> getBrowser () -> getPrivateScene ());
	scaleOrientationInterpolator -> setExecutionContext (executionContext -> getBrowser () -> getPrivateScene ());

	X3DBindableNode::setExecutionContext (executionContext);
}

void
X3DViewpointNode::setUserPosition (const Vector3d & userPosition)
{
	positionOffset () = userPosition - getPosition ();
}

Vector3d
X3DViewpointNode::getUserPosition () const
{
	return getPosition () + positionOffset ();
}

void
X3DViewpointNode::setUserOrientation (const Rotation4d & userOrientation)
{
	orientationOffset () = ~getOrientation () * userOrientation;
}

Rotation4d
X3DViewpointNode::getUserOrientation () const
{
	return getOrientation () * orientationOffset ();
}

Vector3d
X3DViewpointNode::getUserCenterOfRotation () const
{
	return getCenterOfRotation () + centerOfRotationOffset ();
}

void
X3DViewpointNode::setUserCenterOfRotation (const Vector3d & userCenterOfRotation)
{
	centerOfRotationOffset () = userCenterOfRotation - getCenterOfRotation ();
}

void
X3DViewpointNode::set_positionOffset ()
{
	positionOffset () = positionInterpolator -> value_changed () .getValue ();
}

void
X3DViewpointNode::set_scaleOffset ()
{
	scaleOffset () = scaleInterpolator -> value_changed () .getValue ();
}

void
X3DViewpointNode::getRelativeTransformation (X3DViewpointNode* const fromViewpoint,
                                             Vector3d & relativePosition,
                                             Rotation4d & relativeOrientation,
                                             Vector3d & relativeScale,
                                             Rotation4d & relativeScaleOrientation) const
{
	const Matrix4d differenceMatrix = inverse (getTransformationMatrix () * fromViewpoint -> getInverseCameraSpaceMatrix ());

	differenceMatrix .get (relativePosition, relativeOrientation, relativeScale, relativeScaleOrientation);

	relativePosition   -= getPosition ();
	relativeOrientation = ~getOrientation () * relativeOrientation; // mit gepuffereter location matrix
}

Matrix4d
X3DViewpointNode::getProjectionMatrix () const
{
	NavigationInfo* const navigationInfo = getCurrentLayer () -> getNavigationInfo ();

	return getProjectionMatrix (navigationInfo -> getNearValue (), navigationInfo -> getFarValue (this), getCurrentViewport () -> getRectangle ());
}

void
X3DViewpointNode::setCameraSpaceMatrix (const Matrix4d & value)
{
	try
	{
		if (value != cameraSpaceMatrix)
		{
			inverseCameraSpaceMatrix = inverse (value);
			cameraSpaceMatrix        = value;

			getBrowser () -> addEvent ();
		}
	}
	catch (const std::domain_error & error)
	{
	   __LOG__ << error .what () << std::endl;
	}
}

void
X3DViewpointNode::bindToLayer (X3DLayerNode* const layer)
{
	//std::clog << "Trying to bind X3DViewpoint '" << getName () << ":" << description () << "' to layer '" << layer -> getName () << ":" << layer << "': " << std::flush;

	layer -> getViewpointStack () -> push (this);

	//std::clog << (layer -> getViewpointStack () -> top () == this ? "success." : "rejected.") << std::endl;
}

void
X3DViewpointNode::unbindFromLayer (X3DLayerNode* const layer)
{
	layer -> getViewpointStack () -> pop (this);
}

void
X3DViewpointNode::removeFromLayer (X3DLayerNode* const layer)
{
	layer -> getViewpointStack () -> erase (this);
}

void
X3DViewpointNode::isLockedToCamera (const bool value)
{
	lockToCamera = value;

	if (lockToCamera)
		addInterest (this, &X3DViewpointNode::applyUserOffsets);

	else
		removeInterest (this, &X3DViewpointNode::applyUserOffsets);
}

void
X3DViewpointNode::applyUserOffsets ()
{
	if (timeSensor -> isActive ())
		return;

	if (positionOffset () not_eq Vector3d ())
	{
		setPosition (getUserPosition ());
		positionOffset () = Vector3d ();
	}

	if (orientationOffset () not_eq Rotation4d ())
	{
		setOrientation (getUserOrientation ());
		orientationOffset () = Rotation4d ();
	}

	if (centerOfRotationOffset () not_eq Vector3d ())
	{
		setCenterOfRotation (getUserCenterOfRotation ());
		centerOfRotationOffset () = Vector3d ();
	}
}

void
X3DViewpointNode::resetUserOffsets ()
{
	positionOffset ()         = Vector3d ();
	orientationOffset ()      = Rotation4d ();
	scaleOffset ()            = Vector3d (1, 1, 1);
	scaleOrientationOffset () = Rotation4d ();
	centerOfRotationOffset () = Vector3d ();
	fieldOfViewScale ()       = 1;
}

void
X3DViewpointNode::straighten (const bool horizon)
{
	for (const auto & layer : getLayers ())
		layer -> getNavigationInfo () -> transitionStart () = true;

	timeSensor -> cycleInterval () = 0.4;
	timeSensor -> stopTime ()      = getCurrentTime ();
	timeSensor -> startTime ()     = getCurrentTime ();
	timeSensor -> isActive () .addInterest (this, &X3DViewpointNode::set_isActive);

	easeInEaseOut -> easeInEaseOut () = { SFVec2f (0, 1), SFVec2f (1, 0) };

	const Rotation4d rotation = orientationOffset () * (horizon
	                                                    ? straightenHorizon (getUserOrientation ())
																		 : straightenView (getUserOrientation ()));

	positionInterpolator         -> keyValue () = { positionOffset () .getValue (),         positionOffset () .getValue () };
	orientationInterpolator      -> keyValue () = { orientationOffset () .getValue (),      rotation };
	scaleInterpolator            -> keyValue () = { scaleOffset () .getValue (),            scaleOffset () .getValue () };
	scaleOrientationInterpolator -> keyValue () = { scaleOrientationOffset () .getValue (), scaleOrientationOffset () .getValue () };

	const auto distanceToCenter = abs (getUserCenterOfRotation () - getUserPosition ());

	centerOfRotationOffset () = getUserPosition () + (Vector3d (0, 0, -1) * distanceToCenter) * (getOrientation () * rotation) - getCenterOfRotation ();

	set_bind () = true;
}

///  Returns a relative rotation that can right multiplied with @a orientation to get a straighten horizon.
Rotation4d
X3DViewpointNode::straightenHorizon (const Rotation4d & orientation) const
{
	// Taken from Billboard

	const Vector3d direction = zAxis * orientation;
	const Vector3d normal    = cross (direction, getUpVector ());
	const Vector3d vector    = cross (direction, yAxis * orientation);

	return Rotation4d (vector, normal);
}

///  This function straightens the view in a way that the viewpoint looks along the upVector plane.
///  Returns a relative rotation that can right multiplied with @a orientation to get a straighten view.
Rotation4d
X3DViewpointNode::straightenView (const Rotation4d & orientation) const
{
	return Rotation4d (yAxis * orientation, getUpVector ());
}

///  Flys to @a point in world coordinates to a distance of @a factor and centers the point within the browser surface.
///  If @a straighten is true the camera up vector is rotate into the plane defined by the local y-axis and the cameras
///  direction vector.
void
X3DViewpointNode::lookAt (Vector3d point, const double factor, const bool straighten)
{
	if (not getBrowser () -> getActiveLayer ())
		return;

	try
	{
		point = point * inverse (getTransformationMatrix ());

		const double minDistance = getBrowser () -> getActiveLayer () -> getNavigationInfo () -> getNearValue () * 2;

		lookAt (point, minDistance, factor, straighten);
	}
	catch (const std::domain_error &)
	{ }
}

///  Given a @a bbox in world coordinates the camera flys to the box to a distance of @a factor and centers the box
///  within the browser surface.  If @a straighten is true the camera up vector is rotate into the plane defined by the
///  local y-axis and the cameras direction vector.
void
X3DViewpointNode::lookAt (Box3d bbox, const double factor, const bool straighten)
{
	if (not getBrowser () -> getActiveLayer ())
		return;

	try
	{
		bbox *= inverse (getTransformationMatrix ());

		const double minDistance = getBrowser () -> getActiveLayer () -> getNavigationInfo () -> getNearValue () * 2;

		lookAt (bbox .center (), std::max (minDistance, getLookAtDistance (bbox)), factor, straighten);
	}
	catch (const std::domain_error &)
	{ }
}

void
X3DViewpointNode::lookAt (const Vector3d & point, const double distance, const double factor, const bool straighten)
{
   const auto offset = point + Vector3d (0, 0, distance) * getUserOrientation () - getPosition ();

	for (const auto & layer : getLayers ())
		layer -> getNavigationInfo () -> transitionStart () = true;

	timeSensor -> cycleInterval () = 0.2;
	timeSensor -> stopTime ()      = getCurrentTime ();
	timeSensor -> startTime ()     = getCurrentTime ();
	timeSensor -> isActive () .addInterest (this, &X3DViewpointNode::set_isActive);

	easeInEaseOut -> easeInEaseOut () = { SFVec2f (0, 1), SFVec2f (1, 0) };

	const auto translation = lerp <Vector3d> (positionOffset (), offset, factor);
	const auto direction   = getPosition () + translation - point;
	auto       rotation    = orientationOffset () * Rotation4d (zAxis * getUserOrientation (), direction);

	if (straighten)
		rotation = ~getOrientation () * straightenHorizon (getOrientation () * rotation);

	positionInterpolator         -> keyValue () = { positionOffset () .getValue (),         translation };
	orientationInterpolator      -> keyValue () = { orientationOffset () .getValue (),      rotation };
	scaleInterpolator            -> keyValue () = { scaleOffset () .getValue (),            scaleOffset () .getValue () };
	scaleOrientationInterpolator -> keyValue () = { scaleOrientationOffset () .getValue (), scaleOrientationOffset () .getValue () };

	centerOfRotationOffset () = point - getCenterOfRotation ();
	set_bind ()               = true;
}

///  Starts a transition from @a fromViewpoint to this viewpoints position and orientation.  The transitionType from the
///  currently bound NavigationInfo is used.  The user offsets are reseted if retainUserOffsets is false.  You can use:
///
///      viewpoint -> transitionStart (viewpoint);
///
///  to fly to the original position of the viewpoint.
void
X3DViewpointNode::transitionStart (X3DViewpointNode* const fromViewpoint)
{
	try
	{
		if (jump ())
		{
			if (not retainUserOffsets ())
				resetUserOffsets ();

			TransitionType transitionType = TransitionType::LINEAR;
			time_type      transitionTime = 1;

			for (const auto & layer : getLayers ())
			{
				layer -> getNavigationInfo () -> transitionStart () = true;

				transitionType = layer -> getNavigationInfo () -> getTransitionType ();
				transitionTime = layer -> getNavigationInfo () -> transitionTime ();
			}

			switch (transitionType)
			{
				case TransitionType::TELEPORT:
				{
					for (const auto & layer : getLayers ())
						layer -> getNavigationInfo () -> transitionComplete () = true;

					return;
				}
				case TransitionType::LINEAR:
				{
					easeInEaseOut -> easeInEaseOut () = { SFVec2f (0, 0), SFVec2f (0, 0) };
					break;
				}
				case TransitionType::ANIMATE:
				{
					easeInEaseOut -> easeInEaseOut () = { SFVec2f (0, 1), SFVec2f (1, 0) };
					break;
				}
			}

			timeSensor -> cycleInterval () = transitionTime;
			timeSensor -> stopTime ()      = getCurrentTime ();
			timeSensor -> startTime ()     = getCurrentTime ();
			timeSensor -> isActive () .addInterest (this, &X3DViewpointNode::set_isActive);

			Vector3d   relativePosition, relativeScale;
			Rotation4d relativeOrientation, relativeScaleOrientation;

			getRelativeTransformation (fromViewpoint, relativePosition, relativeOrientation, relativeScale, relativeScaleOrientation);

			const Vector3d   startPosition         = relativePosition;
			const Rotation4d startOrientation      = relativeOrientation;
			const Vector3d   startScale            = relativeScale;
			const Rotation4d startScaleOrientation = relativeScaleOrientation;

			const Vector3d   endPosition         = positionOffset ();
			const Rotation4d endOrientation      = orientationOffset ();
			const Vector3d   endScale            = scaleOffset ();
			const Rotation4d endScaleOrientation = scaleOrientationOffset ();

			positionOffset ()         = startPosition;
			orientationOffset ()      = startOrientation;
			scaleOffset ()            = startScale;
			scaleOrientationOffset () = startScaleOrientation;

			positionInterpolator         -> keyValue () = { startPosition, endPosition };
			orientationInterpolator      -> keyValue () = { startOrientation, endOrientation };
			scaleInterpolator            -> keyValue () = { startScale, endScale };
			scaleOrientationInterpolator -> keyValue () = { startScaleOrientation, endScaleOrientation };
		}
		else
		{
			Vector3d   relativePosition, relativeScale;
			Rotation4d relativeOrientation, relativeScaleOrientation;

			getRelativeTransformation (fromViewpoint, relativePosition, relativeOrientation, relativeScale, relativeScaleOrientation);

			positionOffset ()         = relativePosition;
			orientationOffset ()      = relativeOrientation;
			scaleOffset ()            = relativeScale;
			scaleOrientationOffset () = relativeScaleOrientation;
		}
	}
	catch (const std::domain_error &)
	{
		// Catch error from differenceMatrix .inverse ()
	}
}

void
X3DViewpointNode::transitionStop ()
{
	timeSensor -> stopTime () = getCurrentTime ();
	timeSensor -> isActive () .removeInterest (this, &X3DViewpointNode::set_isActive);
}

// Notify NavigationInfos when transitions are complete.
void
X3DViewpointNode::set_isActive (const bool value)
{
	if (not value && timeSensor -> fraction_changed () == 1.0f)
	{
		for (const auto & layer : getLayers ())
			layer -> getNavigationInfo () -> transitionComplete () = true;

		easeInEaseOut -> set_fraction () = 1;
	}
}

void
X3DViewpointNode::set_bind_ ()
{
	if (not isBound ())
		timeSensor -> stopTime () = getCurrentTime ();
}

void
X3DViewpointNode::traverse (const TraverseType type)
{
	switch (type)
	{
		case TraverseType::CAMERA:
		{
			getCurrentLayer () -> getViewpoints () -> push_back (this);

			setTransformationMatrix (getModelViewMatrix () .get ());

			if (isBound ())
			{
				Matrix4d matrix;
				matrix .set (getUserPosition (), getUserOrientation (), scaleOffset (), scaleOrientationOffset ());

				setCameraSpaceMatrix (matrix * getModelViewMatrix () .get ());
			}

			return;
		}
		default:
			return;
	}
}

void
X3DViewpointNode::dispose ()
{
	X3DViewpointObject::dispose ();
	X3DBindableNode::dispose ();
}

X3DViewpointNode::~X3DViewpointNode ()
{ }

} // X3D
} // titania
