/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstraﬂe 31a, Leipzig, Germany 2011.
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
 * Copyright 1999, 2012 Holger Seelig <holger.seelig@yahoo.de>.
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
#include "../Layering/X3DLayerNode.h"
#include "../Navigation/NavigationInfo.h"

#include "../Interpolation/EaseInEaseOut.h"
#include "../Interpolation/PositionInterpolator.h"
#include "../Interpolation/OrientationInterpolator.h"
#include "../Time/TimeSensor.h"

namespace titania {
namespace X3D {

static constexpr Vector3f yAxis (0, 1, 0);
static constexpr Vector3f zAxis (0, 0, 1);

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
	                parentMatrix (),
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

	positionInterpolator         -> value_changed () .addInterest (positionOffset ());
	orientationInterpolator      -> value_changed () .addInterest (orientationOffset ());
	scaleInterpolator            -> value_changed () .addInterest (scaleOffset ());
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
X3DViewpointNode::setUserPosition (const Vector3f & userPosition)
{
__LOG__ << std::endl;
	positionOffset () = userPosition - getPosition ();
}

Vector3f
X3DViewpointNode::getUserPosition () const
{
	return getPosition () + positionOffset ();
}

void
X3DViewpointNode::setUserOrientation (const Rotation4f & userOrientation)
{
__LOG__ << std::endl;
	orientationOffset () = ~getOrientation () * userOrientation;
}

Rotation4f
X3DViewpointNode::getUserOrientation () const
{
	return getOrientation () * orientationOffset ();
}

Vector3f
X3DViewpointNode::getUserCenterOfRotation () const
{
	return getCenterOfRotation () + centerOfRotationOffset ();
}

void
X3DViewpointNode::setUserCenterOfRotation (const Vector3f & userCenterOfRotation)
{
__LOG__ << std::endl;
	centerOfRotationOffset () = userCenterOfRotation - getCenterOfRotation ();
}

void
X3DViewpointNode::getRelativeTransformation (X3DViewpointNode* const fromViewpoint,
                                             Vector3f & relativePosition,
                                             Rotation4f & relativeOrientation,
                                             Vector3f & relativeScale,
                                             Rotation4f & relativeScaleOrientation) const
{
	const Matrix4f differenceMatrix = ~(getParentMatrix () * fromViewpoint -> getInverseCameraSpaceMatrix ());

	differenceMatrix .get (relativePosition, relativeOrientation, relativeScale, relativeScaleOrientation);

	relativePosition   -= getPosition ();
	relativeOrientation = ~getOrientation () * relativeOrientation; // mit gepuffereter location matrix
}

void
X3DViewpointNode::setTransformationMatrix (Matrix4f value)
{
	try
	{
		cameraSpaceMatrix        = value;
		inverseCameraSpaceMatrix = ~value;
	}
	catch (const std::domain_error &)
	{ }
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

	if (positionOffset () not_eq Vector3f ())
	{
		setPosition (getUserPosition ());
		positionOffset () = Vector3f ();
	}

	if (orientationOffset () not_eq Rotation4f ())
	{
		setOrientation (getUserOrientation ());
		orientationOffset () = Rotation4f ();
	}

	if (centerOfRotationOffset () not_eq Vector3f ())
	{
		setCenterOfRotation (getUserCenterOfRotation ());
		centerOfRotationOffset () = Vector3f ();
	}
}

void
X3DViewpointNode::resetUserOffsets ()
{
	positionOffset ()         = Vector3f ();
	orientationOffset ()      = Rotation4f ();
	scaleOffset ()            = Vector3f (1, 1, 1);
	scaleOrientationOffset () = Rotation4f ();
	centerOfRotationOffset () = Vector3f ();
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

	const Rotation4f rotation = orientationOffset () * (horizon
	                                                    ? straightenHorizon (getUserOrientation ())
																		 : straightenView (getUserOrientation ()));

	positionInterpolator         -> keyValue () = { positionOffset (), positionOffset () };
	orientationInterpolator      -> keyValue () = { orientationOffset (), rotation };
	scaleInterpolator            -> keyValue () = { scaleOffset (), scaleOffset () };
	scaleOrientationInterpolator -> keyValue () = { scaleOrientationOffset (), scaleOrientationOffset () };

	const auto distanceToCenter = abs (getUserCenterOfRotation () - getUserPosition ());

	centerOfRotationOffset () = getUserPosition () + (Vector3f (0, 0, -1) * distanceToCenter) * (getOrientation () * rotation) - getCenterOfRotation ();

	set_bind () = true;
}

///  Returns a relative rotation that can right multiplied with @a orientation to get a straighten horizon.
Rotation4f
X3DViewpointNode::straightenHorizon (const Rotation4f & orientation) const
{
	// Taken from Billboard

	Vector3f direction = zAxis * orientation;
	Vector3f normal    = cross (direction, getUpVector ());
	Vector3f vector    = cross (direction, yAxis * orientation);

	return Rotation4f (vector, normal);
}

///  This function straightens the view in a way that the viewpoint looks along the upVector plane.
///  Returns a relative rotation that can right multiplied with @a orientation to get a straighten view.
Rotation4f
X3DViewpointNode::straightenView (const Rotation4f & orientation) const
{
	return Rotation4f (yAxis * orientation, getUpVector ());
}

///  Flys to @a point in world coordinates to a distance of @a factor and centers the point within the browser surface.
///  If @a straighten is true the camera up vector is rotate into the plane defined by the local y-axis and the cameras
///  direction vector.
void
X3DViewpointNode::lookAt (Vector3f point, const float factor, const bool straighten)
{
	if (not getBrowser () -> getActiveLayer ())
		return;

	try
	{
		point = point * ~getParentMatrix ();

		const float minDistance = getBrowser () -> getActiveLayer () -> getNavigationInfo () -> getNearPlane () * 2;

		lookAt (point, minDistance, factor, straighten);
	}
	catch (const std::domain_error &)
	{ }
}

///  Given a @a bbox in world coordinates the camera flys to the box to a distance of @a factor and centers the box
///  within the browser surface.  If @a straighten is true the camera up vector is rotate into the plane defined by the
///  local y-axis and the cameras direction vector.
void
X3DViewpointNode::lookAt (Box3f bbox, const float factor, const bool straighten)
{
	if (not getBrowser () -> getActiveLayer ())
		return;

	try
	{
		bbox *= ~getParentMatrix ();

		const float minDistance = getBrowser () -> getActiveLayer () -> getNavigationInfo () -> getNearPlane () * 2;

		lookAt (bbox .center (), std::max (minDistance, getLookAtDistance (bbox)), factor, straighten);
	}
	catch (const std::domain_error &)
	{ }
}

void
X3DViewpointNode::lookAt (const Vector3f & point, const float distance, const float factor, const bool straighten)
{
   const auto offset = point + Vector3f (0, 0, distance) * getUserOrientation () - getPosition ();

	for (const auto & layer : getLayers ())
		layer -> getNavigationInfo () -> transitionStart () = true;

	timeSensor -> cycleInterval () = 0.2;
	timeSensor -> stopTime ()      = getCurrentTime ();
	timeSensor -> startTime ()     = getCurrentTime ();
	timeSensor -> isActive () .addInterest (this, &X3DViewpointNode::set_isActive);

	easeInEaseOut -> easeInEaseOut () = { SFVec2f (0, 1), SFVec2f (1, 0) };

	const auto translation = lerp <Vector3f> (positionOffset (), offset, factor);
	const auto direction   = getPosition () + translation - point;
	auto       rotation    = orientationOffset () * Rotation4f (zAxis * getUserOrientation (), direction);

	if (straighten)
		rotation = ~getOrientation () * straightenHorizon (getOrientation () * rotation);

	positionInterpolator         -> keyValue () = { positionOffset (), translation };
	orientationInterpolator      -> keyValue () = { orientationOffset (), rotation };
	scaleInterpolator            -> keyValue () = { scaleOffset (), scaleOffset () };
	scaleOrientationInterpolator -> keyValue () = { scaleOrientationOffset (), scaleOrientationOffset () };

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

			Vector3f   relativePosition, relativeScale;
			Rotation4f relativeOrientation, relativeScaleOrientation;

			getRelativeTransformation (fromViewpoint, relativePosition, relativeOrientation, relativeScale, relativeScaleOrientation);

			const Vector3f   startPosition         = relativePosition;
			const Rotation4f startOrientation      = relativeOrientation;
			const Vector3f   startScale            = relativeScale;
			const Rotation4f startScaleOrientation = relativeScaleOrientation;

			const Vector3f   endPosition         = positionOffset ();
			const Rotation4f endOrientation      = orientationOffset ();
			const Vector3f   endScale            = scaleOffset ();
			const Rotation4f endScaleOrientation = scaleOrientationOffset ();

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
			Vector3f   relativePosition, relativeScale;
			Rotation4f relativeOrientation, relativeScaleOrientation;

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

			setParentMatrix (getModelViewMatrix () .get ());

			if (isBound ())
			{
				Matrix4f matrix;
				matrix .set (getUserPosition (), getUserOrientation (), scaleOffset (), scaleOrientationOffset ());

				setTransformationMatrix (matrix * getModelViewMatrix () .get ());
			}

			break;
		}
		default:
			break;
	}
}

///  Reshape viewpoint that it suits for X3DBackground.
///  Overloaded in GeoViewpoint.
void
X3DViewpointNode::background (const double zNear, const double zFar)
{
	reshape (zNear, zFar);
}

void
X3DViewpointNode::reshape ()
{
	NavigationInfo* const navigationInfo = getCurrentLayer () -> getNavigationInfo ();

	reshape (navigationInfo -> getNearPlane (), navigationInfo -> getFarPlane (this));
}

void
X3DViewpointNode::reshape (const double zNear, const double zFar)
{
	glMatrixMode (GL_PROJECTION);

	glLoadMatrixd (getProjectionMatrix (zNear, zFar, Viewport4i ()) .data ());

	glMatrixMode (GL_MODELVIEW);
}

void
X3DViewpointNode::transform ()
{
	getModelViewMatrix () .set (getInverseCameraSpaceMatrix ());
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
