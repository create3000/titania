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

#include "SceneFuture.h"

#include "../Browser/X3DBrowser.h"
#include "../Execution/X3DScene.h"

#include <sys/types.h>
#include <unistd.h>

namespace titania {
namespace X3D {

// See http://www.web3d.org/files/specifications/19775-1/V3.3/Part01/components/networking.html#X3DUrlObject for
// how to handle the profile and component arguments/statements of inline nodes.

const std::string SceneFuture::componentName  = "Titania";
const std::string SceneFuture::typeName       = "SceneFuture";
const std::string SceneFuture::containerField = "future";

SceneFuture::SceneFuture (X3DExecutionContext* const executionContext, const MFString & url, const bool externproto, const SceneFutureCallback & callback) :
	X3DBaseNode (executionContext -> getBrowser (), executionContext),
	  X3DFuture (),
	externproto (externproto),
	   callback (callback),
	     loader (executionContext, executionContext -> getWorldURL ()),
	      scene (),
	   urlError (),
	     future (getFuture (url /*, executionContext -> getProfile (), executionContext -> getComponents () */))
{
	addChildObjects (scene);

	getBrowser () -> prepareEvents () .addInterest (&SceneFuture::set_scene, this);
	getBrowser () -> addEvent ();
	getBrowser () -> addLoadCount (this);
}

X3DBaseNode*
SceneFuture::create (X3DExecutionContext* const executionContext) const
{
	throw Error <NOT_SUPPORTED> ("SceneFuture::create");
}

std::future <X3DScenePtr>
SceneFuture::getFuture (const MFString & url)
{
	return std::async (std::launch::async, std::mem_fn (&SceneFuture::loadAsync), this, url);
}

void
SceneFuture::setExecutionContext (X3DExecutionContext* const executionContext)
{
	const bool prepareEvents = getBrowser () -> prepareEvents () .hasInterest (&SceneFuture::set_scene, this);

	getBrowser () -> prepareEvents () .removeInterest (&SceneFuture::set_scene, this);

	X3DFuture::setExecutionContext (executionContext);

	if (prepareEvents)
	{
		getBrowser () -> prepareEvents () .addInterest (&SceneFuture::set_scene, this);
		getBrowser () -> addEvent ();
	}
}

void
SceneFuture::wait ()
{
	try
	{
		checkForInterrupt ();

		if (not future .valid ())
		{
			stop ();
			return;
		}

		future .wait ();

		scene = future .get ();

		scene -> requestImmediateLoadOfExternProtos ();

		callback (std::move (scene));

		stop ();
	}
	catch (const InterruptThreadException &)
	{
		// Interrupt
	}
	catch (const std::exception & error)
	{
		urlError = { error .what () };

		callback (nullptr);

		stop ();
	}
}

X3DScenePtr
SceneFuture::loadAsync (const MFString & url)
{
	try
	{
		checkForInterrupt ();

		const auto mutex = getBrowser () -> getDownloadMutex ();

		checkForInterrupt ();

		std::lock_guard <std::mutex> lock (*mutex);

		if (externproto)
		{
			for (const auto & URL : url)
			{
				try
				{
					checkForInterrupt ();
		
					const auto scene = getBrowser () -> createScene (false);
		
					checkForInterrupt ();
		
					loader .parseIntoScene (scene, { URL });
		
					if (externproto)
					{
						const auto protoName = basic::uri (URL) .fragment ();
	
						if (protoName .empty ())
						{
							if (scene -> getProtoDeclarations () .empty ())
							{
								getBrowser () -> getConsole () -> error ("No PROTO found\n");
								continue;
							}
						}
						else
							scene -> getProtoDeclaration (protoName);
					}
	
					checkForInterrupt ();
			
					getBrowser () -> getConsole () -> log ("Done loading scene '", loader .getWorldURL (), "'.\n");
			
					checkForInterrupt ();
			
					return scene;
				}
				catch (const Error <INVALID_NAME> & error)
				{
					getBrowser () -> getConsole () -> error (error .what (), "\n");
					continue;
				}
				catch (const X3DError & error)
				{
					continue;
				}
			}
	
			throw Error <URL_UNAVAILABLE> ("Couldn't load any URL of " + url .toString () + "\n");
		}
		else
		{
			const auto scene = getBrowser () -> createScene (false);
	
			checkForInterrupt ();
	
			loader .parseIntoScene (scene, url);
	
			checkForInterrupt ();
	
			getBrowser () -> getConsole () -> log ("Done loading scene '", loader .getWorldURL (), "'.\n");
	
			checkForInterrupt ();
	
			return scene;
		}
	}
	catch (const InterruptThreadException &)
	{
		throw;
	}
	catch (const std::exception & error)
	{
		checkForInterrupt ();
		throw;
	}
}

void
SceneFuture::set_scene ()
{
	try
	{
		checkForInterrupt ();

		getBrowser () -> addEvent ();

		if (not future .valid ())
			return;

		const auto status = future .wait_for (std::chrono::milliseconds (0));

		if (status not_eq std::future_status::ready)
			return;

		getBrowser () -> prepareEvents () .removeInterest (&SceneFuture::set_scene, this);

		scene = future .get ();

		scene -> getExternProtosLoadCount () .addInterest (&SceneFuture::set_loadCount, this);

		scene -> requestAsyncLoadOfExternProtos ();
	}
	catch (const InterruptThreadException &)
	{
		// Interrupt
	}
	catch (const std::exception & error)
	{
		urlError = { error .what () };

		callback (nullptr);

		stop ();
	}
}

void
SceneFuture::set_loadCount (const int32_t loadCount)
{
	try
	{
		checkForInterrupt ();

		if (loadCount)
			return;

		scene -> getExternProtosLoadCount () .removeInterest (&SceneFuture::set_loadCount, this);

		callback (std::move (scene));

		stop ();
	}
	catch (const InterruptThreadException &)
	{
		// Interrupt
	}
}

void
SceneFuture::stop ()
{
	getBrowser () -> removeLoadCount (this);

	X3DFuture::stop ();

	loader .stop ();

	// This is very important, otherwise not all nodes do dispose as they could be bound in the callback!
	callback = [ ] (X3DScenePtr &&) { };
}

void
SceneFuture::dispose ()
{
	stop ();

	X3DFuture::dispose ();
}

SceneFuture::~SceneFuture ()
{
	if (future .valid ())
		future .wait ();
}

} // X3D
} // titania
