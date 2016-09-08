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

#include "RigidBody.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const ComponentType RigidBody::component      = ComponentType::RIGID_BODY_PHYSICS;
const std::string   RigidBody::typeName       = "RigidBody";
const std::string   RigidBody::containerField = "bodies";

RigidBody::Fields::Fields () :
	angularDampingFactor (new SFFloat (0.001)),
	angularVelocity (new SFVec3f ()),
	autoDamp (new SFBool ()),
	autoDisable (new SFBool ()),
	centerOfMass (new SFVec3f ()),
	disableAngularSpeed (new SFFloat ()),
	disableLinearSpeed (new SFFloat ()),
	disableTime (new SFFloat ()),
	enabled (new SFBool (true)),
	finiteRotationAxis (new SFVec3f ()),
	fixed (new SFBool ()),
	forces (new MFVec3f ()),
	geometry (new MFNode ()),
	inertia (new SFMatrix3f ())
{ }

RigidBody::RigidBody (X3DExecutionContext* const executionContext) :
	X3DBaseNode (executionContext -> getBrowser (), executionContext),
	    X3DNode (),
	     fields ()
{
	addType (X3DConstants::RigidBody);

	addField (inputOutput, "metadata",             metadata ());
	addField (inputOutput, "angularDampingFactor", angularDampingFactor ());
	addField (inputOutput, "angularVelocity",      angularVelocity ());
	addField (inputOutput, "autoDamp",             autoDamp ());
	addField (inputOutput, "autoDisable",          autoDisable ());
	addField (inputOutput, "centerOfMass",         centerOfMass ());
	addField (inputOutput, "disableAngularSpeed",  disableAngularSpeed ());
	addField (inputOutput, "disableLinearSpeed",   disableLinearSpeed ());
	addField (inputOutput, "disableTime",          disableTime ());
	addField (inputOutput, "enabled",              enabled ());
	addField (inputOutput, "finiteRotationAxis",   finiteRotationAxis ());
	addField (inputOutput, "fixed",                fixed ());
	addField (inputOutput, "forces",               forces ());
	addField (inputOutput, "geometry",             geometry ());
	addField (inputOutput, "inertia",              inertia ());
}

X3DBaseNode*
RigidBody::create (X3DExecutionContext* const executionContext) const
{
	return new RigidBody (executionContext);
}

} // X3D
} // titania
