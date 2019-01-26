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

#include "DoubleAxisHingeJoint.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../RigidBodyPhysics/RigidBody.h"
#include "../RigidBodyPhysics/RigidBodyCollection.h"

namespace titania {
namespace X3D {

const Component   DoubleAxisHingeJoint::component      = Component ("RigidBodyPhysics", 2);
const std::string DoubleAxisHingeJoint::typeName       = "DoubleAxisHingeJoint";
const std::string DoubleAxisHingeJoint::containerField = "joints";

DoubleAxisHingeJoint::Fields::Fields () :
	              anchorPoint (new SFVec3f ()),
	                    axis1 (new SFVec3f ()),
	                    axis2 (new SFVec3f ()),
	                minAngle1 (new SFFloat (-3.14159)),
	                maxAngle1 (new SFFloat (3.14159)),
	  desiredAngularVelocity1 (new SFFloat ()),
	  desiredAngularVelocity2 (new SFFloat ()),
	               maxTorque1 (new SFFloat ()),
	               maxTorque2 (new SFFloat ()),
	              stopBounce1 (new SFFloat ()),
	    stopConstantForceMix1 (new SFFloat (0.001)),
	     stopErrorCorrection1 (new SFFloat (0.8)),
	          suspensionForce (new SFFloat ()),
	suspensionErrorCorrection (new SFFloat (0.8)),
	         body1AnchorPoint (new SFVec3f ()),
	         body2AnchorPoint (new SFVec3f ()),
	                body1Axis (new SFVec3f ()),
	                body2Axis (new SFVec3f ()),
	              hinge1Angle (new SFFloat ()),
	              hinge2Angle (new SFFloat ()),
	          hinge1AngleRate (new SFFloat ()),
	          hinge2AngleRate (new SFFloat ())
{ }

DoubleAxisHingeJoint::DoubleAxisHingeJoint (X3DExecutionContext* const executionContext) :
	      X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DRigidJointNode (),
	           fields (),
	          outputs (),
	            joint (),
	localAnchorPoint1 (),
	localAnchorPoint2 (),
	       localAxis1 (),
	       localAxis2 ()
{
	addType (X3DConstants::DoubleAxisHingeJoint);

	addField (inputOutput, "metadata",                  metadata ());
	addField (inputOutput, "forceOutput",               forceOutput ());
	addField (inputOutput, "anchorPoint",               anchorPoint ());
	addField (inputOutput, "axis1",                     axis1 ());
	addField (inputOutput, "axis2",                     axis2 ());
	addField (inputOutput, "minAngle1",                 minAngle1 ());
	addField (inputOutput, "maxAngle1",                 maxAngle1 ());
	addField (inputOutput, "desiredAngularVelocity1",   desiredAngularVelocity1 ());
	addField (inputOutput, "desiredAngularVelocity2",   desiredAngularVelocity2 ());
	addField (inputOutput, "maxTorque1",                maxTorque1 ());
	addField (inputOutput, "maxTorque2",                maxTorque2 ());
	addField (inputOutput, "stopBounce1",               stopBounce1 ());
	addField (inputOutput, "stopConstantForceMix1",     stopConstantForceMix1 ());
	addField (inputOutput, "stopErrorCorrection1",      stopErrorCorrection1 ());
	addField (inputOutput, "suspensionForce",           suspensionForce ());
	addField (inputOutput, "suspensionErrorCorrection", suspensionErrorCorrection ());
	addField (outputOnly,  "body1AnchorPoint",          body1AnchorPoint ());
	addField (outputOnly,  "body2AnchorPoint",          body2AnchorPoint ());
	addField (outputOnly,  "body1Axis",                 body1Axis ());
	addField (outputOnly,  "body2Axis",                 body2Axis ());
	addField (outputOnly,  "hinge1Angle",               hinge1Angle ());
	addField (outputOnly,  "hinge2Angle",               hinge2Angle ());
	addField (outputOnly,  "hinge1AngleRate",           hinge1AngleRate ());
	addField (outputOnly,  "hinge2AngleRate",           hinge2AngleRate ());
	addField (inputOutput, "body1",                     body1 ());
	addField (inputOutput, "body2",                     body2 ());

	// Units

	anchorPoint ()             .setUnit (UnitCategory::LENGTH);
	minAngle1 ()               .setUnit (UnitCategory::ANGLE);
	maxAngle1 ()               .setUnit (UnitCategory::ANGLE);
	desiredAngularVelocity1 () .setUnit (UnitCategory::ANGULAR_RATE);
	desiredAngularVelocity2 () .setUnit (UnitCategory::ANGULAR_RATE);
	stopConstantForceMix1 ()   .setUnit (UnitCategory::FORCE);
	suspensionForce ()         .setUnit (UnitCategory::FORCE);
}

X3DBaseNode*
DoubleAxisHingeJoint::create (X3DExecutionContext* const executionContext) const
{
	return new DoubleAxisHingeJoint (executionContext);
}

void
DoubleAxisHingeJoint::initialize ()
{
	X3DRigidJointNode::initialize ();

	anchorPoint () .addInterest (&DoubleAxisHingeJoint::set_joint,       this);
	axis1 ()       .addInterest (&DoubleAxisHingeJoint::set_joint,       this);
	axis2 ()       .addInterest (&DoubleAxisHingeJoint::set_joint,       this);
}

void
DoubleAxisHingeJoint::set_forceOutput ()
{
	const std::map <std::string, OutputType> outputTypes = {
		std::pair ("body1AnchorPoint", OutputType::body1AnchorPoint),
		std::pair ("body2AnchorPoint", OutputType::body2AnchorPoint),
		std::pair ("body1Axis",        OutputType::body1Axis),
		std::pair ("body2Axis",        OutputType::body2Axis),
		std::pair ("hinge1Angle",      OutputType::hinge1Angle),
		std::pair ("hinge2Angle",      OutputType::hinge2Angle),
		std::pair ("hinge1AngleRate",  OutputType::hinge1AngleRate),
		std::pair ("hinge2AngleRate",  OutputType::hinge2AngleRate),
	};

	std::fill (outputs .begin (), outputs .end (), false);

	for (const auto & value : basic::make_const_range (forceOutput ()))
	{
		try
		{
			if (value == "ALL")
			{
				std::fill (outputs .begin (), outputs .end (), true);
			}
			else
			{
				outputs [size_t (outputTypes .at (value))] = true;
			}
		}
		catch (const std::out_of_range & error)
		{ }
	}
}

void
DoubleAxisHingeJoint::addJoint ()
{
	if (not getCollection ())
		return;

	if (not getBody1 ())
		return;

	if (not getBody2 ())
		return;

   if (getBody1 () -> getCollection () not_eq getCollection ())
		return;

   if (getBody2 () -> getCollection () not_eq getCollection ())
		return;

	localAnchorPoint1 = anchorPoint () .getValue () * getInitialInverseMatrix1 ();
	localAnchorPoint2 = anchorPoint () .getValue () * getInitialInverseMatrix2 ();
	localAxis1        = normalize (getInitialInverseMatrix1 () .mult_dir_matrix (axis1 () .getValue ()));
	localAxis2        = normalize (getInitialInverseMatrix2 () .mult_dir_matrix (axis2 () .getValue ()));

	joint .reset (new btHingeConstraint (*getBody1 () -> getRigidBody (),
	                                     *getBody2 () -> getRigidBody (),
	                                     btVector3 (localAnchorPoint1 .x (), localAnchorPoint1 .y (), localAnchorPoint1 .z ()),
	                                     btVector3 (localAnchorPoint2 .x (), localAnchorPoint2 .y (), localAnchorPoint2 .z ()),
	                                     btVector3 (localAxis1 .x (), localAxis1 .y (), localAxis1 .z ()),
	                                     btVector3 (localAxis2 .x (), localAxis2 .y (), localAxis2 .z ()),
	                                     false));

	getCollection () -> getDynamicsWorld () -> addConstraint (joint .get (), true);
}

void
DoubleAxisHingeJoint::removeJoint ()
{
	if (not joint)
		return;

	if (getCollection ())
		getCollection () -> getDynamicsWorld () -> removeConstraint (joint .get ());

	joint .reset ();
}

void
DoubleAxisHingeJoint::update1 ()
{
	// Editing support.

	if (not getExecutionContext () -> isLive ())
	{
		initialize1 ();
		set_joint ();
	}

	// When the two bodies are initially placed in the scene, their initial positions define the resting coordinate
	// frames for the two bodies on that joint. Output values from those joints are then relative to this initial position.

	if (outputs [size_t (OutputType::body1AnchorPoint)])
		body1AnchorPoint () = localAnchorPoint1 * getBody1 () -> getMatrix () * getInitialInverseMatrix1 ();

	if (outputs [size_t (OutputType::body1Axis)])
		body1Axis () = normalize (getInitialInverseMatrix1 () .mult_dir_matrix (getBody1 () -> getMatrix () .mult_dir_matrix (localAxis1)));

	if (outputs [size_t (OutputType::hinge1Angle)])
	{
		const auto lastAngle  = hinge1Angle () .getValue ();
		const auto difference = getInitialInverseMatrix1 () * getBody1 () -> getMatrix ();

		Vector3f   translation;
		Rotation4f rotation;

		difference .get (translation, rotation);

		hinge1Angle () = rotation .angle ();

		if (outputs [size_t (OutputType::hinge1AngleRate)])
			hinge1AngleRate () = (hinge1Angle () .getValue () - lastAngle) * getBrowser () -> getCurrentFrameRate ();
	}
}

void
DoubleAxisHingeJoint::update2 ()
{
	// Editing support.

	if (not getExecutionContext () -> isLive ())
	{
		initialize2 ();
		set_joint ();
	}

	// When the two bodies are initially placed in the scene, their initial positions define the resting coordinate
	// frames for the two bodies on that joint. Output values from those joints are then relative to this initial position.

	if (outputs [size_t (OutputType::body2AnchorPoint)])
		body2AnchorPoint () = localAnchorPoint2 * getBody2 () -> getMatrix () * getInitialInverseMatrix2 ();

	if (outputs [size_t (OutputType::body2Axis)])
		body2Axis () = normalize (getInitialInverseMatrix2 () .mult_dir_matrix (getBody2 () -> getMatrix () .mult_dir_matrix (localAxis2)));

	if (outputs [size_t (OutputType::hinge2Angle)])
	{
		const auto lastAngle  = hinge2Angle () .getValue ();
		const auto difference = getInitialInverseMatrix2 () * getBody2 () -> getMatrix ();

		Vector3f   translation;
		Rotation4f rotation;

		difference .get (translation, rotation);

		hinge2Angle () = rotation .angle ();

		if (outputs [size_t (OutputType::hinge2AngleRate)])
			hinge2AngleRate () = (hinge2Angle () .getValue () - lastAngle) * getBrowser () -> getCurrentFrameRate ();
	}
}

} // X3D
} // titania
