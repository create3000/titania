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

#include "SingleAxisHingeJoint.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

SingleAxisHingeJoint::Fields::Fields () :
	anchorPoint (new SFVec3f ()),
	axis (new SFVec3f ()),
	maxAngle (new SFFloat ()),
	minAngle (new SFFloat ()),
	stopBounce (new SFFloat ()),
	stopErrorCorrection (new SFFloat (0.8)),
	angle (new SFFloat ()),
	angleRate (new SFFloat ()),
	body1AnchorPoint (new SFVec3f ()),
	body2AnchorPoint (new SFVec3f ())
{ }

SingleAxisHingeJoint::SingleAxisHingeJoint (X3DExecutionContext* const executionContext) :
	      X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DRigidJointNode (),
	           fields ()
{
	setComponent ("RigidBodyPhysics");
	setTypeName ("SingleAxisHingeJoint");

	addField (inputOutput, "metadata",            metadata ());
	addField (inputOutput, "body1",               body1 ());
	addField (inputOutput, "body2",               body2 ());
	addField (inputOutput, "forceOutput",         forceOutput ());
	addField (inputOutput, "anchorPoint",         anchorPoint ());
	addField (inputOutput, "axis",                axis ());
	addField (inputOutput, "maxAngle",            maxAngle ());
	addField (inputOutput, "minAngle",            minAngle ());
	addField (inputOutput, "stopBounce",          stopBounce ());
	addField (inputOutput, "stopErrorCorrection", stopErrorCorrection ());
	addField (outputOnly,  "angle",               angle ());
	addField (outputOnly,  "angleRate",           angleRate ());
	addField (outputOnly,  "body1AnchorPoint",    body1AnchorPoint ());
	addField (outputOnly,  "body2AnchorPoint",    body2AnchorPoint ());
}

X3DBaseNode*
SingleAxisHingeJoint::create (X3DExecutionContext* const executionContext) const
{
	return new SingleAxisHingeJoint (executionContext);
}

} // X3D
} // titania
