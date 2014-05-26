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

#include "RigidBodyCollection.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const std::string RigidBodyCollection::componentName  = "RigidBodyPhysics";
const std::string RigidBodyCollection::typeName       = "RigidBodyCollection";
const std::string RigidBodyCollection::containerField = "children";

RigidBodyCollection::Fields::Fields () :
	set_contacts (new MFNode ()),
	autoDisable (new SFBool ()),
	bodies (new MFNode ()),
	constantForceMix (new SFFloat (0.0001)),
	contactSurfaceThickness (new SFFloat ()),
	disableAngularSpeed (new SFFloat ()),
	disableLinearSpeed (new SFFloat ()),
	disableTime (new SFFloat ()),
	enabled (new SFBool (true)),
	errorCorrection (new SFFloat (0.8)),
	gravity (new SFVec3f (0, -9.8, 0)),
	iterations (new SFInt32 (10)),
	joints (new MFNode ()),
	maxCorrectionSpeed (new SFFloat (-1)),
	preferAccuracy (new SFBool ()),
	collider (new SFNode ())
{ }

RigidBodyCollection::RigidBodyCollection (X3DExecutionContext* const executionContext) :
	 X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DChildNode (),
	      fields ()
{
	addType (X3DConstants::RigidBodyCollection);

	addField (inputOutput,    "metadata",                metadata ());
	addField (inputOnly,      "set_contacts",            set_contacts ());
	addField (inputOutput,    "autoDisable",             autoDisable ());
	addField (inputOutput,    "bodies",                  bodies ());
	addField (inputOutput,    "constantForceMix",        constantForceMix ());
	addField (inputOutput,    "contactSurfaceThickness", contactSurfaceThickness ());
	addField (inputOutput,    "disableAngularSpeed",     disableAngularSpeed ());
	addField (inputOutput,    "disableLinearSpeed",      disableLinearSpeed ());
	addField (inputOutput,    "disableTime",             disableTime ());
	addField (inputOutput,    "enabled",                 enabled ());
	addField (inputOutput,    "errorCorrection",         errorCorrection ());
	addField (inputOutput,    "gravity",                 gravity ());
	addField (inputOutput,    "iterations",              iterations ());
	addField (inputOutput,    "joints",                  joints ());
	addField (inputOutput,    "maxCorrectionSpeed",      maxCorrectionSpeed ());
	addField (inputOutput,    "preferAccuracy",          preferAccuracy ());
	addField (initializeOnly, "collider",                collider ());
}

X3DBaseNode*
RigidBodyCollection::create (X3DExecutionContext* const executionContext) const
{
	return new RigidBodyCollection (executionContext);
}

} // X3D
} // titania
