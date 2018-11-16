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

#include "X3DNetworkingContext.h"

#include "../X3DBrowser.h"
#include "../Notification.h"
#include "../../Components/Networking/LoadSensor.h"
#include "../../Execution/Scene.h"
#include "../../Thread/X3DFuture.h"

#include <Titania/String/sprintf.h>

namespace titania {
namespace X3D {

static constexpr size_t DOWNLOAD_THREADS_MAX = 8;

const std::string X3DNetworkingContext::providerUrl = "http://titania.create3000.de";

X3DNetworkingContext::X3DNetworkingContext () :
	       X3DBaseNode (),
	         userAgent (),
	      privateScene (new Scene (getBrowser ())),
	        loadSensor (new LoadSensor (getExecutionContext ())),
	downloadMutexIndex (0),
	   downloadMutexes ({ std::make_shared <std::mutex> () }),
	     downloadMutex (),
	    loadingObjects (),
	      loadingTotal (),
	         loadCount (),
	    loadUrlObjects (true),
	      notifyOnLoad (false),
	      monitorFiles (false)
{
	addChildObjects (privateScene,
	                 loadSensor,
	                 loadingTotal,
	                 loadCount,
	                 loadUrlObjects,
	                 notifyOnLoad,
	                 monitorFiles);
}

void
X3DNetworkingContext::initialize ()
{
	userAgent = getBrowser () -> getName () + "/" + getBrowser () -> getVersion () + " (X3D Browser; +" + providerUrl + ")";

	privateScene -> setPrivate (true);
	privateScene -> setup ();
	loadSensor   -> setup ();

	downloadMutexes .resize (DOWNLOAD_THREADS_MAX);

	for (auto & mutex : std::pair (downloadMutexes .begin () + 1, downloadMutexes .end ()))
		mutex .reset (new std::mutex ());
}

///  throws Error <DISPOSED>
const std::shared_ptr <std::mutex> &
X3DNetworkingContext::getDownloadMutex ()
{
	if (downloadMutexes .empty ())
		throw Error <DISPOSED> ("X3DNetworkingContext::getDownloadMutex");

	std::lock_guard <std::mutex> lock (downloadMutex);

	// First try to find a free mutex.

	for (const auto & mutex : downloadMutexes)
	{
		if (mutex -> try_lock ())
		{
			mutex -> unlock ();
			return mutex;
		}
	}

	// Rotate mutexes.

	downloadMutexIndex = (downloadMutexIndex + 1) % downloadMutexes .size ();

	return downloadMutexes [downloadMutexIndex];
}

void
X3DNetworkingContext::addLoadCount (const void* const object)
{
	try
	{
		loadingObjects .emplace (object);

		loadingTotal = loadingTotal + 1;
		loadCount    = loadingObjects .size ();

		set_loadCount ();
	}
	catch (const X3DError &)
	{ }
}

void
X3DNetworkingContext::removeLoadCount (const void* const object)
{
	try
	{
		if (not loadingObjects .erase (object))
			return;
 
		loadCount = loadingObjects .size ();

		set_loadCount ();
	}
	catch (const X3DError &)
	{ }
}

void
X3DNetworkingContext::set_loadCount ()
{
	if (notifyOnLoad)
	{
		switch (loadCount)
		{
			case 0:
				getBrowser () -> getNotification () -> string () = _("Loading done");
				break;
			case 1:
				getBrowser () -> getNotification () -> string () = basic::sprintf (_ ("Loading %d file"), loadCount .getValue ());
				break;
			default:   
				getBrowser () -> getNotification () -> string () = basic::sprintf (_ ("Loading %d files"), loadCount .getValue ());
				break;
		}
	}

	//setCursor ("DEFAULT");
}

void
X3DNetworkingContext::resetLoadCount ()
{
	loadingTotal = 0;
	loadCount    = 0;

	loadingObjects .clear ();			   
}

void
X3DNetworkingContext::dispose ()
{
	downloadMutexes .clear ();
}

X3DNetworkingContext::~X3DNetworkingContext ()
{ }

} // X3D
} // titania
