/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*- */
/*******************************************************************************
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

#include "HAnimJoint.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

HAnimJoint::HAnimJoint (X3DExecutionContext* const executionContext) :
	    X3DBasicNode (executionContext -> getBrowser (), executionContext),
	 X3DGroupingNode (),                                 
	          center (),                                 // SFVec3f    [in,out] center            0 0 0          (-∞,∞)
	      displacers (),                                 // MFNode     [in,out] displacers        [ ]            [HAnimDisplacer]
	limitOrientation (),                                 // SFRotation [in,out] limitOrientation  0 0 1 0        (-∞,∞)|[-1,1]
	          llimit (),                                 // MFFloat    [in,out] llimit            [ ]            (-∞,∞)
	            name (),                                 // SFString   [in,out] name              ""
	        rotation (),                                 // SFRotation [in,out] rotation          0 0 1 0        (-∞,∞)|[-1,1]
	           scale (1, 1, 1),                          // SFVec3f    [in,out] scale             1 1 1          (0,∞)
	scaleOrientation (),                                 // SFRotation [in,out] scaleOrientation  0 0 1 0        (-∞,∞)|[-1,1]
	  skinCoordIndex (),                                 // MFInt32    [in,out] skinCoordIndex    [ ]
	 skinCoordWeight (),                                 // MFFloat    [in,out] skinCoordWeight   [ ]
	       stiffness ({ 0, 0, 0 }),                      // MFFloat    [in,out] stiffness         [0 0 0]        [0,1]
	     translation (),                                 // SFVec3f    [in,out] translation       0 0 0          (-∞,∞)
	          ulimit ()                                  // MFFloat    [in,out] ulimit            [ ]            (-∞,∞)
{
	setComponent ("H-Anim");
	setTypeName ("HAnimJoint");

	appendField (inputOutput,    "metadata",         metadata);
	appendField (initializeOnly, "bboxSize",         bboxSize);
	appendField (initializeOnly, "bboxCenter",       bboxCenter);
	appendField (inputOnly,      "addChildren",      addChildren);
	appendField (inputOnly,      "removeChildren",   removeChildren);
	appendField (inputOutput,    "children",         children);
	appendField (inputOutput,    "center",           center);
	appendField (inputOutput,    "displacers",       displacers);
	appendField (inputOutput,    "limitOrientation", limitOrientation);
	appendField (inputOutput,    "llimit",           llimit);
	appendField (inputOutput,    "name",             name);
	appendField (inputOutput,    "rotation",         rotation);
	appendField (inputOutput,    "scale",            scale);
	appendField (inputOutput,    "scaleOrientation", scaleOrientation);
	appendField (inputOutput,    "skinCoordIndex",   skinCoordIndex);
	appendField (inputOutput,    "skinCoordWeight",  skinCoordWeight);
	appendField (inputOutput,    "stiffness",        stiffness);
	appendField (inputOutput,    "translation",      translation);
	appendField (inputOutput,    "ulimit",           ulimit);
}

X3DBasicNode*
HAnimJoint::create (X3DExecutionContext* const executionContext) const
{
	return new HAnimJoint (executionContext);
}

} // X3D
} // titania
