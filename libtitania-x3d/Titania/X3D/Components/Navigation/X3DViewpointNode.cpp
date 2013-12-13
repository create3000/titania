/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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
#include "../Layering/X3DLayerNode.h"
#include "../Navigation/NavigationInfo.h"

namespace titania {
namespace X3D {

X3DViewpointNode::Fields::Fields () :
	orientation (new SFRotation ()),
	centerOfRotation (new SFVec3f ()),
	jump (new SFBool (true)),
	positionOffset (),
	orientationOffset (),
	scaleOffset (1, 1, 1),
	scaleOrientationOffset (),
	centerOfRotationOffset ()
{ }

X3DViewpointNode::X3DViewpointNode () :
	             X3DBindableNode (),
	          X3DViewpointObject (),
	                      fields (),
	                parentMatrix (),
	        transformationMatrix (1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 10, 1),
	 inverseTransformationMatrix (1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, -10, 1),
	                  timeSensor (new TimeSensor (getExecutionContext ())),
	               easeInEaseOut (new EaseInEaseOut (getExecutionContext ())),
	        positionInterpolator (new PositionInterpolator (getExecutionContext ())),
	     orientationInterpolator (new OrientationInterpolator (getExecutionContext ())),
	           scaleInterpolator (new PositionInterpolator (getExecutionContext ())),
	scaleOrientationInterpolator (new OrientationInterpolator (getExecutionContext ()))
{
	addNodeType (X3DConstants::X3DViewpointNode);

	addChildren (positionOffset (),
	             orientationOffset (),
	             scaleOffset (),
	             scaleOrientationOffset (),
	             centerOfRotationOffset (),
	             timeSensor,
	             easeInEaseOut,
	             positionInterpolator,
	             orientationInterpolator,
	             scaleInterpolator,
	             scaleOrientationInterpolator);
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

	isBound () .addInterest (this, &X3DViewpointNode::_set_bind);
}

Vector3f
X3DViewpointNode::getUserPosition () const
{
	return getPosition () + positionOffset ();
}

Rotation4f
X3DViewpointNode::getUserOrientation () const
{
	return orientation () * orientationOffset ();
}

Vector3f
X3DViewpointNode::getUserCenterOfRotation () const
{
	return centerOfRotation () + centerOfRotationOffset ();
}

void
X3DViewpointNode::getRelativeTransformation (X3DViewpointNode* fromViewpoint,
                                             Vector3f & relativePosition,
                                             Rotation4f & relativeOrientation,
                                             Vector3f & relativeScale,
                                             Rotation4f & relativeScaleOrientation) const
{
	Matrix4f differenceMatrix = getParentMatrix () * fromViewpoint -> getInverseTransformationMatrix ();

	differenceMatrix .translate (getPosition ());
	differenceMatrix .inverse ();

	// Get relative transformations from viewpoint.
	Rotation4f rotation;
	differenceMatrix .get (relativePosition, rotation, relativeScale, relativeScaleOrientation);

	relativeOrientation = ~orientation () * rotation;
}

void
X3DViewpointNode::setTransformationMatrix (const Matrix4f & value)
{
	try
	{
		transformationMatrix        = value;
		inverseTransformationMatrix = ~value;
	}
	catch (const std::domain_error &)
	{
		transformationMatrix        = Matrix4f ();
		inverseTransformationMatrix = Matrix4f ();
	}
}

void
X3DViewpointNode::bindToLayer (X3DLayerNode* const layer)
{
	std::clog << "Trying to bind X3DViewpoint '" << getName () << ":" << description () << "' to layer '" << layer -> getName () << ":" << layer << "': " << std::flush;

	layer -> getViewpointStack () -> push (this);

	std::clog << (layer -> getViewpointStack () -> top () == this ? "success." : "rejected.") << std::endl;
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
X3DViewpointNode::resetUserOffsets ()
{
	positionOffset ()         = Vector3f ();
	orientationOffset ()      = Rotation4f ();
	scaleOffset ()            = Vector3f (1, 1, 1);
	scaleOrientationOffset () = Rotation4f ();
	centerOfRotationOffset () = Vector3f ();
}

void
X3DViewpointNode::straighten ()
{ }

void
X3DViewpointNode::lookAt (Box3f bbox, float factor)
{
	try
	{
		std::clog
			<< "Look at using viewpoint: " << description () << "." << std::endl
			<< getTypeName () << " {" << std::endl
			<< "  position " << getUserPosition () << std::endl
			<< "  orientation " << getUserOrientation () << std::endl
			<< "  centerOfRotation " << getUserCenterOfRotation () << std::endl
			<< "}" << std::endl;

		bbox *= ~getParentMatrix ();

		for (const auto & layer : getLayers ())
			layer -> getNavigationInfo () -> transitionStart () = true;

		timeSensor -> cycleInterval () = 0.2;
		timeSensor -> stopTime ()      = getCurrentTime ();
		timeSensor -> startTime ()     = getCurrentTime ();
		timeSensor -> isActive () .addInterest (this, &X3DViewpointNode::set_isActive);
		
		easeInEaseOut -> easeInEaseOut () = { SFVec2f (0, 1), SFVec2f (1, 0) };
	
		auto translation = lerp <Vector3f> (positionOffset (), getLookAtPositionOffset (bbox), factor);
		auto direction   = getPosition () + translation - bbox .center ();
		auto rotation    = orientationOffset () * Rotation4f (getUserOrientation () * Vector3f (0, 0, 1), direction);

		positionInterpolator         -> keyValue () = { positionOffset (), translation };
		orientationInterpolator      -> keyValue () = { orientationOffset (), rotation };
		scaleInterpolator            -> keyValue () = { scaleOffset (), scaleOffset () };
		scaleOrientationInterpolator -> keyValue () = { scaleOrientationOffset (), scaleOrientationOffset () };

		positionInterpolator         -> value_changed () .addInterest (positionOffset ());
		orientationInterpolator      -> value_changed () .addInterest (orientationOffset ());
		scaleInterpolator            -> value_changed () .addInterest (scaleOffset ());
		scaleOrientationInterpolator -> value_changed () .addInterest (scaleOrientationOffset ());

		centerOfRotationOffset () = bbox .center () - centerOfRotation ();
		set_bind ()               = true;
	}
	catch (const std::domain_error &)
	{
		// Catch error from ~getParentMatrix ()
	}
}

void
X3DViewpointNode::transitionStart (X3DViewpointNode* fromViewpoint)
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
					easeInEaseOut -> easeInEaseOut () = { SFVec2f (0, 0), SFVec2f (0, 0) };
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

			Vector3f   startPosition         = relativePosition;
			Rotation4f startOrientation      = relativeOrientation;
			Vector3f   startScale            = relativeScale;
			Rotation4f startScaleOrientation = relativeScaleOrientation;

			Vector3f   endPosition         = positionOffset ();
			Rotation4f endOrientation      = orientationOffset ();
			Vector3f   endScale            = scaleOffset ();
			Rotation4f endScaleOrientation = scaleOrientationOffset ();

			positionOffset ()         = startPosition;
			orientationOffset ()      = startOrientation;
			scaleOffset ()            = startScale;
			scaleOrientationOffset () = startScaleOrientation;

			positionInterpolator         -> keyValue () = { startPosition, endPosition };
			orientationInterpolator      -> keyValue () = { startOrientation, endOrientation };
			scaleInterpolator            -> keyValue () = { startScale, endScale };
			scaleOrientationInterpolator -> keyValue () = { startScaleOrientation, endScaleOrientation };

			positionInterpolator         -> value_changed () .addInterest (positionOffset ());
			orientationInterpolator      -> value_changed () .addInterest (orientationOffset ());
			scaleInterpolator            -> value_changed () .addInterest (scaleOffset ());
			scaleOrientationInterpolator -> value_changed () .addInterest (scaleOrientationOffset ());
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
X3DViewpointNode::set_isActive (bool value)
{
	if (not value)
	{
		for (const auto & layer : getLayers ())
			layer -> getNavigationInfo () -> transitionComplete () = true;

		positionInterpolator         -> value_changed () .removeInterest (positionOffset ());
		orientationInterpolator      -> value_changed () .removeInterest (orientationOffset ());
		scaleInterpolator            -> value_changed () .removeInterest (scaleOffset ());
		scaleOrientationInterpolator -> value_changed () .removeInterest (scaleOrientationOffset ());

		positionOffset ()         = positionInterpolator         -> keyValue () .back ();
		orientationOffset ()      = orientationInterpolator      -> keyValue () .back ();
		scaleOffset ()            = scaleInterpolator            -> keyValue () .back ();
		scaleOrientationOffset () = scaleOrientationInterpolator -> keyValue () .back ();
	}
}

void
X3DViewpointNode::_set_bind ()
{
	if (set_bind ())
		;
	else
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

			setParentMatrix (ModelViewMatrix4f ());

			if (isBound ())
			{
				Matrix4f matrix;
				matrix .set (getUserPosition (), getUserOrientation (), scaleOffset (), scaleOrientationOffset ());

				setTransformationMatrix (matrix * ModelViewMatrix4f ());
			}
			break;
		}
		default:
			break;
	}
}

void
X3DViewpointNode::reshape ()
{
	NavigationInfo* navigationInfo = getCurrentLayer () -> getNavigationInfo ();

	reshape (navigationInfo -> getNearPlane (), navigationInfo -> getFarPlane ());
}

void
X3DViewpointNode::transform ()
{
	glLoadMatrixf (getInverseTransformationMatrix () .data ());
}

void
X3DViewpointNode::dispose ()
{
	timeSensor                   .dispose ();
	easeInEaseOut                .dispose ();
	positionInterpolator         .dispose ();
	orientationInterpolator      .dispose ();
	scaleInterpolator            .dispose ();
	scaleOrientationInterpolator .dispose ();

	X3DViewpointObject::dispose ();
	X3DBindableNode::dispose ();
}

} // X3D
} // titania
