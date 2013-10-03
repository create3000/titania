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

#include "Inline.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/Scene.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../InputOutput/Loader.h"

#include <atomic>
#include <future>

#include <Titania/Backtrace.h>

namespace titania {
namespace X3D {

static constexpr bool X3D_PARALLEL = true;

class Inline::Future :
	public X3DInput
{
public:

	Future (Inline* const inlineNode) :
		inlineNode (inlineNode),
		   running (true),
		    future (getFuture ())
	{
		inlineNode -> getBrowser () -> prepareEvents () .addInterest (this, &Future::prepareEvents);
		inlineNode -> getBrowser () -> addEvent ();
	}

	void
	wait ()
	{
		if (future .valid ())
		{
			future .wait ();
			prepareEvents ();
		}
	}

	virtual
	~Future ()
	{
		running = false;

		if (future .valid ())
			future .wait ();
	}

private:

	std::future <X3DSFNode <Scene>> 
	getFuture ()
	{
		return std::async (std::launch::async, std::mem_fn (&Future::loadAsync), this, inlineNode -> url ());
	}

	X3DSFNode <Scene>
	loadAsync (const MFString & url)
	{
		std::lock_guard <std::mutex> lock (inlineNode -> getBrowser () -> getThread ());

		X3DSFNode <Scene> scene = inlineNode -> getBrowser () -> createScene ();

		if (running)
			Loader (inlineNode -> getExecutionContext ()) .parseIntoScene (scene, url);

		return scene;
	}

	void
	prepareEvents ()
	{
		inlineNode -> getBrowser () -> addEvent ();

		if (future .valid ())
		{
			auto status = future .wait_for (std::chrono::milliseconds (0));

			if (status == std::future_status::ready)
			{
				inlineNode -> getBrowser () -> prepareEvents () .removeInterest (this, &Future::prepareEvents);

				try
				{
					X3DSFNode <Scene> scene = future .get ();

					inlineNode -> setScene (scene);
					inlineNode -> setLoadState (COMPLETE_STATE);
				}
				catch (const X3DError & error)
				{
					inlineNode -> setScene (inlineNode -> getBrowser () -> createScene ());
					inlineNode -> setLoadState (FAILED_STATE);

					inlineNode -> getBrowser () -> println (error .what ());

					for (const auto & string : loader .getUrlError ())
						inlineNode -> getBrowser () -> println (string .str ());
				}
			}
		}
	}

	Inline* const                    inlineNode;
	std::atomic <bool>               running;
	std::future <X3DSFNode <Scene>>  future;

};

const std::string Inline::componentName  = "Networking";
const std::string Inline::typeName       = "Inline";
const std::string Inline::containerField = "children";

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
	           group (new Group (executionContext)),
	          future (),
	     initialized (false)
{
	addField (inputOutput,    "metadata",   metadata ());
	addField (inputOutput,    "load",       load ());
	addField (inputOutput,    "url",        url ());
	addField (initializeOnly, "bboxSize",   bboxSize ());
	addField (initializeOnly, "bboxCenter", bboxCenter ());

	addChildren (scene, group);
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

	initialized = true;

	load () .addInterest (this, &Inline::set_load);
	url ()  .addInterest (this, &Inline::set_url);

	if (X3D_PARALLEL)
	{
		if (checkLoadState () == NOT_STARTED_STATE)
			setScene (getBrowser () -> createScene ());

		if (load ())
			requestAsyncLoad ();
	}
	else
	{
		if (load ())
			requestImmediateLoad ();

		else
			setScene (getBrowser () -> createScene ());
	}

	group -> setInternal (true);
	group -> setup ();
}

void
Inline::setScene (const X3DSFNode <Scene> & value)
{
	if (scene)
		scene -> getRootNodes () .removeInterest (group -> children ());

	scene = value;

	if (initialized)
		scene -> setup ();

	else
		getExecutionContext () -> addUninitializedNode (scene);

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

const SFNode &
Inline::getExportedNode (const std::string & exportedName) const
throw (Error <INVALID_NAME>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	if (load ())
		const_cast <Inline*> (this) -> requestImmediateLoad ();

	if (checkLoadState () == COMPLETE_STATE)
		return scene -> getExportedNode (exportedName);

	throw Error <INVALID_NAME> ("Imported node error: Inline node '" + getName () + "' is not loaded.");
}

void
Inline::requestAsyncLoad ()
{
	if (checkLoadState () == COMPLETE_STATE or checkLoadState () == IN_PROGRESS_STATE)
		return;

	setLoadState (IN_PROGRESS_STATE);

	future .reset (new Future (this));
}

void
Inline::requestImmediateLoad ()
{
	if (X3D_PARALLEL)
	{
		if (checkLoadState () == IN_PROGRESS_STATE)
		{
			future -> wait ();
			return;
		}
	}

	if (checkLoadState () == COMPLETE_STATE or checkLoadState () == IN_PROGRESS_STATE)
		return;

	setLoadState (IN_PROGRESS_STATE);

	Loader loader (getExecutionContext ());

	try
	{
		setScene (loader .createX3DFromURL (url ()));

		setLoadState (COMPLETE_STATE);
	}
	catch (const X3DError & error)
	{
		setLoadState (FAILED_STATE);

		for (const auto & string : loader .getUrlError ())
			getBrowser () -> println (string .str ());

		getBrowser () -> println (error .what ());
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
Inline::set_load ()
{
	if (load ())
	{
		setLoadState (NOT_STARTED_STATE);

		if (X3D_PARALLEL)
			requestAsyncLoad ();

		else
			requestImmediateLoad ();
	}
	else
		requestUnload ();
}

void
Inline::set_url ()
{
	if (load ())
	{
		setLoadState (NOT_STARTED_STATE);

		if (X3D_PARALLEL)
			requestAsyncLoad ();

		else
			requestImmediateLoad ();
	}
}

void
Inline::traverse (const TraverseType type)
{
	group -> traverse (type);
}

void
Inline::dispose ()
{
	future .reset ();

	scene .dispose ();
	group .dispose ();

	X3DUrlObject::dispose ();
	X3DBoundedObject::dispose ();
	X3DChildNode::dispose ();
}

} // X3D
} // titania
