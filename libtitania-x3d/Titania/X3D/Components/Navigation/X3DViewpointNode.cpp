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
 ******************************************************************************/

#include "X3DViewpointNode.h"

#include "../../Browser/Browser.h"
#include "../Layering/X3DLayerNode.h"
#include "../Navigation/NavigationInfo.h"

namespace titania {
namespace X3D {

X3DViewpointNode::X3DViewpointNode (bool displayed) :
	            X3DBindableNode (),                                                 
	                description (),                                                 // SFString   [in,out] description       ""
	                orientation (),                                                 // SFRotation [in,out] orientation       0 0 1 0        [-1,1],(-∞,∞)
	           centerOfRotation (),                                                 // SFVec3f    [in,out] centerOfRotation  0 0 0          (-∞,∞)
	                       jump (true),                                             // SFBool     [in,out] jump              TRUE
	          retainUserOffsets (),                                                 // SFBool     [ ]      retainUserOffsets
	             positionOffset (),                                                 
	          orientationOffset (),                                                 
	     centerOfRotationOffset (),                                                 
	            modelViewMatrix (),                                                 
	       transformationMatrix (1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 10, 1),  
	inverseTransformationMatrix (1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, -10, 1), 
	           differenceMatrix (),                                                 
	                 timeSensor (),                                                    
	       positionInterpolator (),                                                     
	                  displayed (displayed)                                         
{
	addNodeType (X3DConstants::X3DViewpointNode);

	positionOffset         .addParent (this);
	orientationOffset      .addParent (this);
	centerOfRotationOffset .addParent (this);
}

void
X3DViewpointNode::initialize ()
{
	X3DBindableNode::initialize ();

	timeSensor                  = new TimeSensor (getExecutionContext ());
	timeSensor -> stopTime      = 1;
	timeSensor -> cycleInterval = 0.2;
	timeSensor -> setup ();

	positionInterpolator        = new PositionInterpolator (getExecutionContext ());
	positionInterpolator -> key = { 0, 1 };
	positionInterpolator -> setup ();

	timeSensor           -> fraction_changed .addInterest (positionInterpolator -> set_fraction);
	timeSensor           -> isActive         .addInterest (this, &X3DViewpointNode::set_active);
	positionInterpolator -> value_changed    .addInterest (positionOffset);

	isBound  .addInterest (this, &X3DViewpointNode::_set_bind);

	if (displayed)
		getScene () -> addViewpoint (this);
}

Vector3f
X3DViewpointNode::getUserPosition () const
{
	return getPosition () + positionOffset;
}

Rotation4f
X3DViewpointNode::getUserOrientation () const
{
	return orientation * orientationOffset;
}

Vector3f
X3DViewpointNode::getUserCenterOfRotation () const
{
	return centerOfRotation + centerOfRotationOffset;
}

void
X3DViewpointNode::setTransformationMatrix (const Matrix4f & value)
{
	transformationMatrix        = value;
	inverseTransformationMatrix = ~value;
}

void
X3DViewpointNode::bindToLayer (X3DLayerNode* const layer)
{
	std::clog << "Trying to bind X3DViewpoint '" << description << "': " << std::flush;

	layer -> viewpointStack .push (this);

	std::clog << (layer -> viewpointStack .top () == this ? "success." : "rejected.") << std::endl;
}

void
X3DViewpointNode::removeFromLayer (X3DLayerNode* const layer)
{
	layer -> viewpointStack .pop (this);
}

void
X3DViewpointNode::lookAt (Box3f bbox)
{
	std::clog << "Look at using viewpoint: " << description << "." << std::endl;

	bbox *= ~getModelViewMatrix ();

	timeSensor -> startTime          = 1;
	positionInterpolator -> keyValue = { this -> positionOffset, lookAtPositionOffset (bbox) };

	centerOfRotation       = bbox .center ();
	centerOfRotationOffset = Vector3f ();
	set_bind               = true;

	std::clog << getTypeName () << " {" << std::endl;
	std::clog << "  position " << getUserPosition () << std::endl;
	std::clog << "  orientation " << getUserOrientation () << std::endl;
	std::clog << "  centerOfRotation " << getUserCenterOfRotation () << std::endl;
	std::clog << "}" << std::endl;
}

// Notify NavigationInfos when transitions are complete.
void
X3DViewpointNode::set_active (const bool & value)
{
	if (not value)
	{
		for (const auto & layer : getLayers ())
			layer -> navigationInfoStack .top () -> transitionComplete = getCurrentTime ();
	}
}

void
X3DViewpointNode::_set_bind ()
{
	if (isBound)
	{
		if (jump)
		{
			if (not retainUserOffsets)
			{
				// Reinitialize user offsets.
				positionOffset         = Vector3f ();
				orientationOffset      = Rotation4f ();
				centerOfRotationOffset = Vector3f ();
			}
		}
		else
		{
			// Apply relative transformations from previous viewpoint.
			Vector3f   t;
			Rotation4f r;
			getDifferenceMatrix () .get (t, r);

			positionOffset    = t;
			orientationOffset = r;
		}
	}
}

void
X3DViewpointNode::display ()
{
	setModelViewMatrix (ModelViewMatrix4f ());

	Matrix4f transformationMatrix = ModelViewMatrix4f ();

	if (isBound)
	{
		if (jump)
		{
			transformationMatrix .translate (getUserPosition ());
			transformationMatrix .rotate (getUserOrientation ());

			setTransformationMatrix (transformationMatrix);
		}
		else
		{
			transformationMatrix .translate (getUserPosition ());
			transformationMatrix .rotate (getUserOrientation ());

			setTransformationMatrix (transformationMatrix);
		}
	}
	else
	{
		if (not jump)
		{
			transformationMatrix .translate (getPosition ());
			transformationMatrix .rotate (orientation);

			setDifferenceMatrix (getCurrentViewpoint () -> getTransformationMatrix () * ~transformationMatrix);
		}
	}
}

void
X3DViewpointNode::reshape ()
{
	NavigationInfo* navigationInfo = getCurrentLayer () -> getNavigationInfo ();

	reshape (navigationInfo -> getZNear (), navigationInfo -> getZFar ());

	glLoadMatrixf (getInverseTransformationMatrix () .data ());
}

void
X3DViewpointNode::dispose ()
{
	timeSensor           .dispose ();
	positionInterpolator .dispose ();
	             
	getScene () -> removeViewpoint (this);

	X3DBindableNode::dispose ();
}

} // X3D
} // titania
