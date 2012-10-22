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

#include "Inline.h"

#include "../../Browser/Browser.h"
#include "../../Execution/X3DExecutionContext.h"
#include <iostream>
#include <iterator>

namespace titania {
namespace X3D {

Inline::Inline (X3DExecutionContext* const executionContext) :
	    X3DBasicNode (executionContext -> getBrowser (), executionContext),
	    X3DChildNode (),                                 
	X3DBoundedObject (),                                 
	    X3DUrlObject (),                                 
	            load (true),                             // SFBool [in,out] load  TRUE
	       loadState (NOT_STARTED_STATE),                
	           scene ()
{
	setComponent ("Networking");
	setTypeName ("Inline");

	appendField (inputOutput,    "metadata",   metadata);
	appendField (inputOutput,    "load",       load);
	appendField (inputOutput,    "url",        url);
	appendField (initializeOnly, "bboxSize",   bboxSize);
	appendField (initializeOnly, "bboxCenter", bboxCenter);
}

X3DBasicNode*
Inline::create (X3DExecutionContext* const executionContext) const
{
	return new Inline (executionContext);
}

void
Inline::initialize ()
{
	X3DChildNode::initialize ();
	X3DUrlObject::initialize ();

	load .addInterest (this, &Inline::set_load);
	url  .addInterest (this, &Inline::set_url);

	set_url ();
}

void
Inline::set_load ()
{
	if (load)
		requestImmediateLoad ();
	else
	{
		// remove viewpoints & backgrounds from ExecutionContext
		loadState = NOT_STARTED_STATE;
		scene     = getBrowser () -> createScene ();
	}
}

void
Inline::set_url ()
{
	loadState = NOT_STARTED_STATE;
	requestImmediateLoad ();
}

void
Inline::requestImmediateLoad ()
{
	if (loadState == COMPLETE_STATE or loadState == IN_PROGRESS_STATE)
		return;

	if (not load)
		return;

	loadState = IN_PROGRESS_STATE;

	try
	{
		scene = createX3DFromURL (url);

		for (const auto & node : scene -> getNavigationInfos ())
			getExecutionContext () -> addNavigationInfo (*node);

		for (const auto & node : scene -> getBackgrounds ())
			getExecutionContext () -> addBackground (*node);

		for (const auto & node : scene -> getViewpoints ())
			getExecutionContext () -> addViewpoint (*node);

		for (const auto & node : scene -> getFogs ())
			getExecutionContext () -> addFog (*node);

		scene -> configure ();

		loadState = COMPLETE_STATE;
	}
	catch (const X3DError & error)
	{
		loadState = FAILED_STATE;
		scene     = getBrowser () -> createScene ();

		std::clog << error .what () << std::endl;

		std::copy (URLError.cbegin (), URLError .cend (),
		           std::ostream_iterator <std::string> (std::clog, "\n"));
	}
}

Box3f
Inline::getBBox ()
{
	requestImmediateLoad ();

	return scene -> getBBox ();
}

void
Inline::intersect ()
{
	for (const auto & rootNode : scene -> getRootNodes ())
		rootNode -> select ();
}

void
Inline::draw ()
{
	for (const auto & rootNode : scene -> getRootNodes ())
		rootNode -> display ();
}

void
Inline::dispose ()
{
	// remove viewpoints & backgrounds from ExecutionContext
	scene .dispose ();

	X3DChildNode::dispose ();
}

} // X3D
} // titania
