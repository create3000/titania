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

#include "RigidBodyCollection.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

RigidBodyCollection::RigidBodyCollection (X3DExecutionContext* const executionContext) :
	            X3DBaseNode (executionContext -> getBrowser (), executionContext), 
	           X3DChildNode (),                                                    
	           set_contacts (),                                                    // MFNode  [in]     set_contacts                             [ ]
	            autoDisable (),                                                    // SFBool  [in,out] autoDisable              FALSE
	                 bodies (),                                                    // MFNode  [in,out] bodies                   [ ]             [RigidBody]
	       constantForceMix (0.0001),                                              // SFFloat [in,out] constantForceMix         0.0001          [0,∞)
	contactSurfaceThickness (),                                                    // SFFloat [in,out] contactSurfaceThickness  0               [0,∞)
	    disableAngularSpeed (),                                                    // SFFloat [in,out] disableAngularSpeed      0               [0,∞)
	     disableLinearSpeed (),                                                    // SFFloat [in,out] disableLinearSpeed       0               [0,∞)
	            disableTime (),                                                    // SFFloat [in,out] disableTime              0               [0,∞)
	                enabled (true),                                                // SFBool  [in,out] enabled                  TRUE
	        errorCorrection (0.8),                                                 // SFFloat [in,out] errorCorrection          0.8             [0,1]
	                gravity (0, -9.8, 0),                                          // SFVec3f [in,out] gravity                  0 -9.8 0
	             iterations (10),                                                  // SFInt32 [in,out] iterations               10              [0,∞)
	                 joints (),                                                    // MFNode  [in,out] joints                   [ ]             [X3DRigidJointNode]
	     maxCorrectionSpeed (-1),                                                  // SFFloat [in,out] maxCorrectionSpeed       -1              [0,∞) or -1
	         preferAccuracy (),                                                    // SFBool  [in,out] preferAccuracy           FALSE
	               collider ()                                                     // SFNode  [ ]      collider                 NULL            [CollisionCollection]
{
	setComponent ("RigidBodyPhysics");
	setTypeName ("RigidBodyCollection");

	addField (inputOutput,    "metadata",                metadata);
	addField (inputOnly,      "set_contacts",            set_contacts);
	addField (inputOutput,    "autoDisable",             autoDisable);
	addField (inputOutput,    "bodies",                  bodies);
	addField (inputOutput,    "constantForceMix",        constantForceMix);
	addField (inputOutput,    "contactSurfaceThickness", contactSurfaceThickness);
	addField (inputOutput,    "disableAngularSpeed",     disableAngularSpeed);
	addField (inputOutput,    "disableLinearSpeed",      disableLinearSpeed);
	addField (inputOutput,    "disableTime",             disableTime);
	addField (inputOutput,    "enabled",                 enabled);
	addField (inputOutput,    "errorCorrection",         errorCorrection);
	addField (inputOutput,    "gravity",                 gravity);
	addField (inputOutput,    "iterations",              iterations);
	addField (inputOutput,    "joints",                  joints);
	addField (inputOutput,    "maxCorrectionSpeed",      maxCorrectionSpeed);
	addField (inputOutput,    "preferAccuracy",          preferAccuracy);
	addField (initializeOnly, "collider",                collider);
}

X3DBaseNode*
RigidBodyCollection::create (X3DExecutionContext* const executionContext) const
{
	return new RigidBodyCollection (executionContext);
}

} // X3D
} // titania
