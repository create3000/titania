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

#include "NavigationInfo.h"

#include "../../Browser/Browser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../Layering/X3DLayerNode.h"

namespace titania {
namespace X3D {

NavigationInfo::NavigationInfo (X3DExecutionContext* const executionContext) :
	      X3DBasicNode (executionContext -> getBrowser (), executionContext), 
	   X3DBindableNode (),                                                    
	        avatarSize ({ 0.25, 1.6, 0.75 }),                                 // MFFloat  [in,out] avatarSize         [0.25 1.6 0.75]        [0,∞)
	         headlight (true),                                                // SFBool   [in,out] headlight          TRUE
	             speed (1),                                                   // SFFloat  [in,out] speed              1.0                    [0,∞)
	    transitionTime (1),                                                   // SFTime   [in,out] transitionTime     1.0                    [0, ∞)
	    transitionType ({ "LINEAR" }),                                        // MFString [in,out] transitionType     ["LINEAR"]             ["TELEPORT","LINEAR",
	              type ({ "EXAMINE", "ANY" }),                                // MFString [ ]      type               { "EXAMINE", "ANY" }
	   visibilityLimit (),                                                    // SFFloat  [ ]      visibilityLimit    0
	transitionComplete (),                                                    // SFBool   [ ]      transitionComplete                  
	  directionalLight (new DirectionalLight (executionContext))              
{
	setComponent ("Navigation");
	setTypeName ("NavigationInfo");

	appendField (inputOutput, "metadata",           metadata);
	appendField (inputOnly,   "set_bind",           set_bind);
	appendField (inputOutput, "avatarSize",         avatarSize);
	appendField (inputOutput, "headlight",          headlight);
	appendField (inputOutput, "speed",              speed);
	appendField (inputOutput, "transitionTime",     transitionTime);
	appendField (inputOutput, "transitionType",     transitionType);
	appendField (inputOutput, "type",               type);
	appendField (inputOutput, "visibilityLimit",    visibilityLimit);
	appendField (outputOnly,  "transitionComplete", transitionComplete);
	appendField (outputOnly,  "bindTime",           bindTime);
	appendField (outputOnly,  "isBound",            isBound);

	setChildren (directionalLight);
}

X3DBasicNode*
NavigationInfo::create (X3DExecutionContext* const executionContext) const
{
	return new NavigationInfo (executionContext);
}

void
NavigationInfo::initialize ()
{
	X3DBindableNode::initialize ();

	directionalLight -> setup ();

	getBrowser () -> addNavigationInfo (this);
}

void
NavigationInfo::bindToLayer (X3DLayerNode* const layer)
{
	layer -> navigationInfoStack .push (this);
}

void
NavigationInfo::removeFromLayer (X3DLayerNode* const layer)
{
	layer -> navigationInfoStack .pop (this);
}

void
NavigationInfo::enable ()
{
	if (headlight)
	{
		// Reset modelview matrix for headlight.
		glPushMatrix ();
		glLoadIdentity ();

		directionalLight -> enable ();

		glPopMatrix ();
	}
}

void
NavigationInfo::disable ()
{
	if (headlight)
		directionalLight -> disable ();
}

void
NavigationInfo::dispose ()
{
	getBrowser () -> removeNavigationInfo (this);

	X3DBindableNode::dispose ();
}

} // X3D
} // titania
