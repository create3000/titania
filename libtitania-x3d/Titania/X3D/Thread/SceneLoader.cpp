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

#include "SceneLoader.h"

#include "../Browser/X3DBrowser.h"
#include "../Execution/X3DScene.h"

#include <sys/types.h>
#include <unistd.h>

namespace titania {
namespace X3D {

class FutureUrlErrorException:
	virtual public std::exception
{
public:

	FutureUrlErrorException (const MFString & urlError) :
		std::exception (),
	   urlError (urlError)
	{ }

	virtual
	char const*
	what () const
	noexcept (true) override
	{ return "FutureUrlErrorException"; }

	const MFString &
	getUrlError () const
	{ return urlError; }


private:

	MFString urlError;

};

// See http://www.web3d.org/files/specifications/19775-1/V3.3/Part01/components/networking.html#X3DUrlObject for
// how to handle the profile and component arguments/statements of inline nodes.

SceneLoader::SceneLoader (X3DExecutionContext* const executionContext, const MFString & url, const Callback & callback) :
	X3DFuture (),
	  browser (executionContext -> getBrowser ()),
	 callback (callback),
	   loader (nullptr, executionContext -> getWorldURL ()),
	    scene (),
	 urlError (),
	   future (getFuture (url /*, executionContext -> getProfile (), executionContext -> getComponents () */))
{
	getBrowser () -> prepareEvents () .addInterest (this, &SceneLoader::set_scene, true);
	getBrowser () -> addEvent ();
}

std::future <X3DScenePtr> 
SceneLoader::getFuture (const MFString & url)
{
	return std::async (std::launch::async, std::mem_fn (&SceneLoader::loadAsync), this, url);
}

void
SceneLoader::setExecutionContext (X3DExecutionContext* const executionContext)
{
	const auto browser = executionContext -> getBrowser ();

	if (getBrowser () -> prepareEvents () .hasInterest (this, &SceneLoader::set_scene))
	{
		getBrowser () -> prepareEvents () .removeInterest (this, &SceneLoader::set_scene);
		browser -> prepareEvents () .addInterest (this, &SceneLoader::set_scene, true);
	}

	this -> browser = browser;

	getBrowser () -> addEvent ();
}

bool
SceneLoader::ready ()
{
	if (not future .valid ())
		return true;

	const auto status = future .wait_for (std::chrono::milliseconds (0));

	return status == std::future_status::ready;
}

void
SceneLoader::wait ()
{
	if (isStopping ())
		return;

	if (future .valid ())
	{
		future .wait ();

		try
		{
			scene = future .get ();

			scene -> requestImmediateLoadOfExternProtos ();

			callback (std::move (scene));
		}
		catch (const FutureUrlErrorException & error)
		{
			urlError = error .getUrlError ();

			callback (nullptr);
		}
		catch (const std::exception &)
		{
		   // Interrupt
		}
	}

	dispose ();
}

X3DScenePtr
SceneLoader::loadAsync (const MFString & url)
{
	checkForInterrupt ();

	const auto mutex = getBrowser () -> getDownloadMutex ();

	checkForInterrupt ();

	std::lock_guard <std::mutex> lock (*mutex);

	checkForInterrupt ();

	auto scene = getBrowser () -> createScene ();

	checkForInterrupt ();

	try
	{
		loader .parseIntoScene (scene, url);
	}
	catch (const X3DError & error)
	{
		checkForInterrupt ();

		getBrowser () -> println (error .what ());

		throw FutureUrlErrorException ({ error .what () });
	}
		
	checkForInterrupt ();

	getBrowser () -> println ("Done loading scene '", loader .getWorldURL (), "'.");
		
	checkForInterrupt ();

	return scene;
}

void
SceneLoader::set_scene (const bool addEvent)
{
	if (isStopping ())
		return;

	if (addEvent)
		getBrowser () -> addEvent ();

	if (not future .valid ())
		return;

	const auto status = future .wait_for (std::chrono::milliseconds (0));

	if (status not_eq std::future_status::ready)
		return;

	// Remove set_scene connection.
	X3DFuture::dispose ();

	try
	{
		scene = future .get ();

		scene -> getExternProtosLoadCount () .addInterest (this, &SceneLoader::set_loadCount);

		scene -> requestAsyncLoadOfExternProtos ();
	}
	catch (const FutureUrlErrorException & error)
	{
		urlError = error .getUrlError ();

		callback (nullptr);
	}
	catch (const std::exception &)
	{
	   // Interrupt
	}
}

void
SceneLoader::set_loadCount (const int32_t loadCount)
{
	if (isStopping ())
		return;

	if (loadCount)
	   return;

	callback (std::move (scene));

	dispose ();
}

void
SceneLoader::dispose ()
{
	if (isStopping ())
		return;

	stop ();
	loader .stop ();

	scene .dispose ();

	X3DFuture::dispose ();

	// This is very important, otherwise not all nodes do dispose as they could be bound in the callback!
	callback = [ ] (X3DScenePtr &&) { };
}

SceneLoader::~SceneLoader ()
{
	dispose ();

	if (future .valid ())
		future .wait ();
}

} // X3D
} // titania
