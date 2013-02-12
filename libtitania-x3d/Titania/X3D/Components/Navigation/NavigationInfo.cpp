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
 * For Silvio, Joy and Adi.
 *
 ******************************************************************************/

#include "NavigationInfo.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../Layering/X3DLayerNode.h"

#include <limits>

namespace titania {
namespace X3D {

NavigationInfo::NavigationInfo (X3DExecutionContext* const executionContext, bool displayed) :
	       X3DBaseNode (executionContext -> getBrowser (), executionContext), 
	   X3DBindableNode (displayed),                                           
	        avatarSize ({ 0.25, 1.6, 0.75 }),                                 // MFFloat  [in,out] avatarSize         [0.25 1.6 0.75]        [0,∞)
	         headlight (true),                                                // SFBool   [in,out] headlight          TRUE
	             speed (1),                                                   // SFFloat  [in,out] speed              1.0                    [0,∞)
	    transitionTime (1),                                                   // SFTime   [in,out] transitionTime     1.0                    [0, ∞)
	    transitionType ({ "LINEAR" }),                                        // MFString [in,out] transitionType     ["LINEAR"]             ["TELEPORT","LINEAR",
	              type ({ "EXAMINE", "ANY" }),                                // MFString [in,out] type               { "EXAMINE", "ANY" }
	   visibilityLimit (),                                                    // SFFloat  [in,out] visibilityLimit    0
	transitionComplete (),                                                    // SFBool   [out]    transitionComplete
	  directionalLight (new DirectionalLight (executionContext)),
	             light ()              
{
	setComponent ("Navigation");
	setTypeName ("NavigationInfo");

	addField (inputOutput, "metadata",           metadata);
	addField (inputOnly,   "set_bind",           set_bind);
	addField (inputOutput, "avatarSize",         avatarSize);
	addField (inputOutput, "headlight",          headlight);
	addField (inputOutput, "speed",              speed);
	addField (inputOutput, "transitionTime",     transitionTime);
	addField (inputOutput, "transitionType",     transitionType);
	addField (inputOutput, "type",               type);
	addField (inputOutput, "visibilityLimit",    visibilityLimit);
	addField (outputOnly,  "transitionComplete", transitionComplete);
	addField (outputOnly,  "bindTime",           bindTime);
	addField (outputOnly,  "isBound",            isBound);

	setChildren (directionalLight);
}

X3DBaseNode*
NavigationInfo::create (X3DExecutionContext* const executionContext) const
{
	return new NavigationInfo (executionContext, true);
}

void
NavigationInfo::initialize ()
{
	X3DBindableNode::initialize ();

	directionalLight -> setup ();
	
	headlight .addInterest (this, &NavigationInfo::set_headlight);
	
	set_headlight ();
}

void
NavigationInfo::set_headlight ()
{
	if (headlight)
		light .reset (new LightContainer (Matrix4f (), directionalLight));
		
	else
		light .reset ();
}

float
NavigationInfo::getZNear () const
{
	float zNear = avatarSize .size () ? avatarSize .front () : 0.25;

	if (zNear == 0)
		zNear = std::numeric_limits <float>::epsilon () * 100;

	else
		zNear *= 0.5f;

	return zNear;
}

float
NavigationInfo::getZFar () const
{
	return visibilityLimit ? visibilityLimit : 100000;
}

void
NavigationInfo::addToLayer (X3DLayerNode* const layer)
{
	layer -> getNavigationInfos () .push_back (this);
}

void
NavigationInfo::removeFromLayer (X3DLayerNode* const layer)
{
	layer -> getNavigationInfos () .erase (this);
}

void
NavigationInfo::bindToLayer (X3DLayerNode* const layer)
{
	layer -> getNavigationInfoStack () .push (this);
}

void
NavigationInfo::unbindFromLayer (X3DLayerNode* const layer)
{
	layer -> getNavigationInfoStack () .pop (this);
}

void
NavigationInfo::enable ()
{
	if (headlight)
		light -> enable ();
}

void
NavigationInfo::disable ()
{
	if (headlight)
		light -> disable ();
}

void
NavigationInfo::dispose ()
{
	directionalLight .dispose ();

	X3DBindableNode::dispose ();
}

} // X3D
} // titania
