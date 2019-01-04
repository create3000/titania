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

#include "UniversalJoint.h"

#include "../../Execution/X3DExecutionContext.h"
#include "../RigidBodyPhysics/RigidBody.h"
#include "../RigidBodyPhysics/RigidBodyCollection.h"

namespace titania {
namespace X3D {

const std::string UniversalJoint::componentName  = "RigidBodyPhysics";
const std::string UniversalJoint::typeName       = "UniversalJoint";
const std::string UniversalJoint::containerField = "joints";

UniversalJoint::Fields::Fields () :
	         anchorPoint (new SFVec3f ()),
	               axis1 (new SFVec3f ()),
	               axis2 (new SFVec3f ()),
	         stop1Bounce (new SFFloat ()),
	         stop2Bounce (new SFFloat ()),
	stop1ErrorCorrection (new SFFloat (0.8)),
	stop2ErrorCorrection (new SFFloat (0.8)),
	    body1AnchorPoint (new SFVec3f ()),
	    body2AnchorPoint (new SFVec3f ()),
	           body1Axis (new SFVec3f ()),
	           body2Axis (new SFVec3f ())
{ }

UniversalJoint::UniversalJoint (X3DExecutionContext* const executionContext) :
	      X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DRigidJointNode (),
	           fields (),
	          outputs (),
	            joint ()
{
	addType (X3DConstants::UniversalJoint);

	addField (inputOutput, "metadata",             metadata ());
	addField (inputOutput, "forceOutput",          forceOutput ());
	addField (inputOutput, "anchorPoint",          anchorPoint ());
	addField (inputOutput, "axis1",                axis1 ());
	addField (inputOutput, "axis2",                axis2 ());
	addField (inputOutput, "stop1Bounce",          stop1Bounce ());
	addField (inputOutput, "stop2Bounce",          stop2Bounce ());
	addField (inputOutput, "stop1ErrorCorrection", stop1ErrorCorrection ());
	addField (inputOutput, "stop2ErrorCorrection", stop2ErrorCorrection ());
	addField (outputOnly,  "body1AnchorPoint",     body1AnchorPoint ());
	addField (outputOnly,  "body2AnchorPoint",     body2AnchorPoint ());
	addField (outputOnly,  "body1Axis",            body1Axis ());
	addField (outputOnly,  "body2Axis",            body2Axis ());
	addField (inputOutput, "body1",                body1 ());
	addField (inputOutput, "body2",                body2 ());

	// Units

	anchorPoint ()      .setUnit (UnitCategory::LENGTH);
	body1AnchorPoint () .setUnit (UnitCategory::LENGTH);
	body2AnchorPoint () .setUnit (UnitCategory::LENGTH);
}

X3DBaseNode*
UniversalJoint::create (X3DExecutionContext* const executionContext) const
{
	return new UniversalJoint (executionContext);
}

void
UniversalJoint::initialize ()
{
	X3DRigidJointNode::initialize ();

	anchorPoint () .addInterest (&UniversalJoint::set_joint,       this);
	axis1 ()       .addInterest (&UniversalJoint::set_joint,       this);
	axis2 ()       .addInterest (&UniversalJoint::set_joint,       this);
}

void
UniversalJoint::addJoint ()
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

	auto localAnchorPoint1 = anchorPoint () .getValue ();
	auto localAnchorPoint2 = anchorPoint () .getValue ();
	auto localAxis1        = axis1 () .getValue ();
	auto localAxis2        = axis2 () .getValue ();

	localAnchorPoint1 = localAnchorPoint1 * getInitialInverseMatrix1 ();
	localAnchorPoint2 = localAnchorPoint2 * getInitialInverseMatrix2 ();
	localAxis1        = normalize (getInitialInverseMatrix1 () .mult_dir_matrix (localAxis1));
	localAxis2        = normalize (getInitialInverseMatrix2 () .mult_dir_matrix (localAxis2));

	joint .reset (new btUniversalConstraint (*getBody1 () -> getRigidBody (),
	                                         *getBody2 () -> getRigidBody (),
	                                         btVector3 (anchorPoint () .getX (), anchorPoint () .getY (), anchorPoint () .getZ ()),
	                                         btVector3 (axis1 () .getX (), axis1 () .getY (), axis1 () .getZ ()),
	                                         btVector3 (axis2 () .getX (), axis2 () .getY (), axis2 () .getZ ())));

	getCollection () -> getDynamicsWorld () -> addConstraint (joint .get (), true);

	if (outputs [size_t (OutputType::body1AnchorPoint)])
		body1AnchorPoint () = localAnchorPoint1;

	if (outputs [size_t (OutputType::body2AnchorPoint)])
		body2AnchorPoint () = localAnchorPoint2;

	if (outputs [size_t (OutputType::body1Axis)])
		body1Axis () = localAxis1;

	if (outputs [size_t (OutputType::body2Axis)])
		body2Axis () = localAxis2;
}

void
UniversalJoint::removeJoint ()
{
	if (not joint)
		return;

	if (getCollection ())
		getCollection () -> getDynamicsWorld () -> removeConstraint (joint .get ());

	joint .reset ();
}

void
UniversalJoint::set_forceOutput ()
{
	const std::map <std::string, OutputType> outputTypes = {
		std::pair ("body1AnchorPoint", OutputType::body1AnchorPoint),
		std::pair ("body2AnchorPoint", OutputType::body2AnchorPoint),
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
UniversalJoint::update1 ()
{
	// Editing support.

	if (getExecutionContext () -> isLive ())
		return;

	initialize1 ();
	set_joint ();
}

void
UniversalJoint::update2 ()
{
	// Editing support.

	if (getExecutionContext () -> isLive ())
		return;

	initialize2 ();
	set_joint ();
}

} // X3D
} // titania
