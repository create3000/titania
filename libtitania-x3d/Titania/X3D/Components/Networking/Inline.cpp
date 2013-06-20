/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#include "Inline.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/Scene.h"
#include "../../Execution/X3DExecutionContext.h"
#include <iostream>
#include <iterator>

namespace titania {
namespace X3D {

Inline::Fields::Fields () :
	load (new SFBool (true))
{ }

Inline::Inline (X3DExecutionContext* const executionContext) :
	     X3DBaseNode (executionContext -> getBrowser (), executionContext), 
	    X3DChildNode (),                                                    
	X3DBoundedObject (),                                                    
	    X3DUrlObject (),
	          fields (),
	           scene (),                                                    
	           group (new Group (executionContext))                                                     
{
	setComponent ("Networking");
	setTypeName ("Inline");

	addField (inputOutput,    "metadata",   metadata ());
	addField (inputOutput,    "load",       load ());
	addField (inputOutput,    "url",        url ());
	addField (initializeOnly, "bboxSize",   bboxSize ());
	addField (initializeOnly, "bboxCenter", bboxCenter ());

	setChildren (scene, group);
}

X3DBaseNode*
Inline::create (X3DExecutionContext* const executionContext) const
{
	return new Inline (executionContext);
}

void
Inline::initialize ()
{
	X3DChildNode::initialize ();
	X3DBoundedObject::initialize ();
	X3DUrlObject::initialize ();

	load () .addInterest (this, &Inline::set_load);
	url ()  .addInterest (this, &Inline::set_url);

	if (load ())
		requestLoad ();
	
	else
		setScene (getBrowser () -> createScene ());
		
	group -> setup ();
}

void
Inline::setScene (const SFNode <Scene> & value)
{
	if (scene)
		scene -> getRootNodes () .removeInterest (group -> children ());

	scene = value;
	scene -> getRootNodes () .addInterest (group -> children ());
	
	group -> children () = scene -> getRootNodes ();
}

Box3f
Inline::getBBox ()
{
	if (bboxSize () == Vector3f (-1, -1, -1))
		return group -> getBBox ();

	return Box3f (bboxSize (), bboxCenter ());
}

const SFNode <X3DBaseNode> &
Inline::getExportedNode (const std::string & exportedName) const
throw (Error <INVALID_NAME>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	if (checkLoadState () == COMPLETE_STATE)
		return scene -> getExportedNode (exportedName);

	else
		throw Error <INVALID_NAME> ("Imported node error: Inline node '" + getName () + "' is not loaded.");
}

void
Inline::set_load ()
{
	if (load ())
		requestLoad ();

	else
		requestUnload ();
}

void
Inline::set_url ()
{
	if (load ())
		requestLoad ();
}

void
Inline::requestLoad ()
{
	requestImmediateLoad ();
}

void
Inline::requestImmediateLoad ()
{
	if (checkLoadState () == COMPLETE_STATE or checkLoadState () == IN_PROGRESS_STATE)
		return;

	setLoadState (IN_PROGRESS_STATE);

	try
	{
		setScene (createX3DFromURL (url ()));

		setLoadState (COMPLETE_STATE);
	}
	catch (const X3DError & error)
	{
		setLoadState (FAILED_STATE);

		std::clog << error .what () << std::endl;

		std::copy (urlError () .cbegin (), urlError () .cend (),
		           std::ostream_iterator <String> (std::clog, "\n"));
	}
}

void
Inline::requestUnload ()
{
	if (checkLoadState () == NOT_STARTED_STATE or checkLoadState () == FAILED_STATE)
		return;

	setScene (getBrowser () -> createScene ());

	setLoadState (NOT_STARTED_STATE);
}

void
Inline::traverse (TraverseType type)
{
	group -> traverse (type);
}

void
Inline::dispose ()
{
	scene .dispose ();
	group .dispose ();

	X3DUrlObject::dispose ();
	X3DBoundedObject::dispose ();
	X3DChildNode::dispose ();
}

} // X3D
} // titania
