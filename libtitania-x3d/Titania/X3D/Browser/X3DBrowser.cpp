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

#include "X3DBrowser.h"

#include "../Browser/Core/BrowserOptions.h"
#include "../Browser/Notification.h"
#include "../Components/Layering/X3DLayerNode.h"
#include "../Components/Navigation/X3DViewpointNode.h"
#include "../Components/Networking/LoadSensor.h"
#include "../Configuration/SupportedComponents.h"
#include "../Configuration/SupportedFields.h"
#include "../Configuration/SupportedNodes.h"
#include "../Configuration/SupportedProfiles.h"
#include "../Execution/Scene.h"
#include "../Execution/World.h"
#include "../InputOutput/FileLoader.h"
#include "../Thread/SceneFuture.h"

#include <Titania/Backtrace.h>

namespace titania {
namespace X3D {

const std::string X3DBrowser::version = "4.6.3"; // Don't forget to call 'make version'.

const Component   X3DBrowser::component      = Component ("Titania", 1);
const std::string X3DBrowser::typeName       = "Browser";
const std::string X3DBrowser::containerField = "browser";

X3DBrowser::X3DBrowser (const X3DBrowserPtr & sharedBrowser, const MFString & url, const MFString & parameter) :
	        X3DBaseNode (),
	  X3DBrowserContext (sharedBrowser),
	        cssProvider (Gtk::CssProvider::create ()),
	                url (url),
	          parameter (parameter),
	        description (),
	       currentSpeed (0),
	   currentFrameRate (60),
	    supportedFields (sharedBrowser ? sharedBrowser -> supportedFields     : std::make_shared <SupportedFields> ()),
	     supportedNodes (sharedBrowser ? sharedBrowser -> supportedNodes      : std::make_shared <SupportedNodes> (this)),
	supportedComponents (sharedBrowser ? sharedBrowser -> supportedComponents : std::make_shared <SupportedComponents> ()),
	  supportedProfiles (sharedBrowser ? sharedBrowser -> supportedProfiles   : std::make_shared <SupportedProfiles> (supportedComponents)),
	   executionContext (createScene (false)),
	          loadState (NOT_STARTED_STATE),
	           urlError (),
	         inShutdown (0),
	             future ()
{
	// __LOG__ << "Constructing browser " << this << "." << std::endl;

	install_backtrace ();

	setName ("Titania");
	addType (X3DConstants::X3DBrowser);

	addChildObjects (getRootNodes (),
	                 description,
	                 executionContext,
	                 loadState,
	                 urlError,
	                 future);

	try
	{
		std::string string;

		string += ".titania-private-invisible {\n";
		string += "  opacity: 0;\n";
		string += "}\n";

		cssProvider -> load_from_data (string);

		Gtk::StyleContext::add_provider_for_screen (Gdk::Screen::get_default (), cssProvider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
	}
	catch (const Glib::Error & error)
	{
	   __LOG__ << error .what () << std::endl;
	}

	get_style_context () -> add_class ("titania-private-invisible");
}

void
X3DBrowser::initialize ()
{
	// __LOG__ << "Initializing browser " << this << "." << std::endl;

	X3DBaseNode::initialize ();
	X3DBrowserContext::initialize ();

	// Add necessary routes.

	prepareEvents () .addInterest (&X3DBrowser::set_prepareEvents, this);

	replaceWorld (executionContext);

	// Load initial url or start with empty scene.

	if (url .empty ())
	{
	   initialized () = true;
	}
	else
	{
	   initialized () .set (true);

	   loadURL (url, parameter);
	}

	// Print welcome message.

	getConsole () -> log (std::boolalpha,
	                      '\n',
	                      std::string (80, '*'), '\n',
	                      std::string (80, '*'), '\n',

	                      "Welcome to ", getName (), " X3D Browser ", getVersion (), ':', '\n',
	                      "\tCompiled at ", __DATE__, " ", __TIME__, '\n',

	                      getRenderingProperties (), '\n',

			                "\t\tMax vertex uniform vectors: ", getMaxVertexUniformVectors (), "\n",
			                "\t\tMax fragment uniform vectors: ", getMaxFragmentUniformVectors (), "\n",
			                "\t\tMax vertex attribs: ", getMaxVertexAttributes (), "\n",

	                      getJavaScriptEngine (), '\n',

	                      #ifdef TITANIA_DEBUG
	                      "\tId: ", this, '\n',
	                      #endif

	                      std::string (80, '*'), '\n',
	                      std::string (80, '*'), '\n',
	                      '\n',
	                      '\n');
}

void
X3DBrowser::set_prepareEvents ()
{
	if (getActiveLayer ())
		currentSpeed .setPosition (getActiveLayer () -> getViewpoint () -> getCameraSpaceMatrix () .origin (), currentFrameRate);
	else
		currentSpeed .setPosition (Vector3f (), 0);

	if (getClock () -> interval ())
		currentFrameRate = 1 / getClock () -> interval ();
	else
		currentFrameRate = 60;
}

///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
void
X3DBrowser::setDescription (const std::string & value)
{
	description = value;

	if (not value .empty ())
		getNotification () -> string () = value;
}

///  throws Error <NOT_SUPPORTED>, Error <DISPOSED>
const X3DFieldDefinition*
X3DBrowser::getSupportedField (const std::string & typeName) const
{
	if (not supportedFields)
		throw Error <DISPOSED> ("X3DBrowser::getSupportedField: Browser is already disposed.");

	return supportedFields -> getField (typeName);
}

///  throws Error <DISPOSED>
const SupportedFieldsArray &
X3DBrowser::getSupportedFields () const
{
	if (not supportedFields)
		throw Error <DISPOSED> ("X3DBrowser::getSupportedFields: Browser is already disposed.");

	return supportedFields -> getFields ();
}

///  throws Error <NOT_SUPPORTED>, Error <DISPOSED>
const X3DBaseNode*
X3DBrowser::getSupportedNode (const std::string & typeName) const
{
	if (not supportedNodes)
		throw Error <DISPOSED> ("X3DBrowser::getSupportedNode: Browser is already disposed.");

	return supportedNodes -> getNode (typeName);
}

///  throws Error <DISPOSED>
const SupportedNodesArray &
X3DBrowser::getSupportedNodes () const
{
	if (not supportedNodes)
		throw Error <DISPOSED> ("X3DBrowser::getSupportedNodes: Browser is already disposed.");

	return supportedNodes -> getNodes ();
}

///  throws Error <NOT_SUPPORTED>, Error <DISPOSED>
ComponentInfoPtr
X3DBrowser::getComponent (const std::string & name, const size_t level) const
{
	if (not supportedComponents)
		throw Error <DISPOSED> ("X3DBrowser::getComponent: Browser is already disposed.");

	return supportedComponents -> get (name, level);
}

///  throws Error <DISPOSED>
const ComponentInfoArray &
X3DBrowser::getSupportedComponents () const
{
	if (not supportedComponents)
		throw Error <DISPOSED> ("X3DBrowser::getSupportedComponents: Browser is already disposed.");

	return supportedComponents -> get ();
}

///  throws Error <DISPOSED>
const ProfileInfoArray &
X3DBrowser::getSupportedProfiles () const
{
	if (not supportedProfiles)
		throw Error <DISPOSED> ("X3DBrowser::getSupportedProfiles: Browser is already disposed.");

	return supportedProfiles -> get ();
}

///  throws Error <NOT_SUPPORTED>, Error <DISPOSED>
ProfileInfoPtr
X3DBrowser::getProfile (const std::string & name) const
{
	if (not supportedProfiles)
		throw Error <DISPOSED> ("X3DBrowser::getProfile: Browser is already disposed.");

	return supportedProfiles -> get (name);
}

///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
X3DScenePtr
X3DBrowser::createScene (const bool setup) const
{
	const X3DScenePtr scene (new Scene (const_cast <X3DBrowser*> (this)));

	scene -> isLive () = false;

	if (setup)
		scene -> setup ();

	return scene;
}

///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
X3DScenePtr
X3DBrowser::createScene (const ProfileInfoPtr & profile, const ComponentInfoArray & components, const bool setup) const
{
	const X3DScenePtr scene = createScene (setup);

	scene -> setProfile (profile);
	scene -> setComponents (components);

	return scene;
}

///  throws Error <INVALID_SCENE>, Error <INVALID_OPERATION_TIMING>
void
X3DBrowser::replaceWorld (const X3DExecutionContextPtr & value)
{
	ContextLock lock (this);

	if (future)
	{
		removeLoadCount (future .getValue ());
		future -> stop ();
	}

	finished () .removeInterest (&X3DBrowser::set_scene, this);

	// Process shutdown.

	if (initialized ()) // Don't do this if browser is not initialized.
	{
		if (not inShutdown)
		{
			++ inShutdown;

			//getClock () -> advance ();

			shutdowned () .processInterests ();

			// Cancel replaceWorld if another replaceWorld was called in shutdown.

			if (inShutdown > 1)
			{
				inShutdown = 0;
				return;
			}

			inShutdown = 0;
		}
		else
			++ inShutdown;
	}

	// Process as normal.

	if (not initialized () or value not_eq executionContext)
	{
		// Remove world.

		isLive () .removeInterest (executionContext -> isLive ());
		executionContext -> isLive () = false;

		// Replace world.

		executionContext = value ? value : createScene (false);

		resetLoadCount ();
		prepareEvents () .removeInterest (&X3DBrowser::set_initialized, this);

		setDescription ("");
		const X3D::BrowserOptionsPtr browserOptions (new X3D::BrowserOptions (this));
		getBrowserOptions () -> assign (browserOptions, true);

		getConsole () -> log ("\n*** The browser is requested to replace the world with '", executionContext -> getWorldURL (), "'.\n\n");
		isLive () .addInterest (executionContext -> isLive ());
		executionContext -> isLive () = isLive ();
		executionContext -> setup ();
		executionContext -> realize ();

		setWorld (new World (executionContext));
		getWorld () -> setup ();
		getWorld () -> bind ();

		if (initialized ())
		{
			get_style_context () -> add_class ("titania-private-invisible"); // getBrowserOptions () -> SplashScreen ()
			getLoadCount () .addInterest (&X3DBrowser::set_loadCount, this);
			set_loadCount ();
		}
	}
}

void
X3DBrowser::set_loadCount ()
{
	if (getLoadCount ())
		return;

	getLoadCount () .removeInterest (&X3DBrowser::set_loadCount, this);
	prepareEvents () .addInterest (&X3DBrowser::set_initialized, this);
	addEvent ();
}

void
X3DBrowser::set_initialized ()
{
	prepareEvents () .removeInterest (&X3DBrowser::set_initialized, this);

	get_style_context () -> remove_class ("titania-private-invisible");
	initialized () = true;

	#ifdef TITANIA_DEBUG
	std::clog << "Replacing world done." << std::endl;
	#endif
}

///  throws Error <INVALID_DOCUMENT>, Error <INVALID_OPERATION_TIMING>, Error <NOT_SUPPORTED>, Error <DISPOSED>
X3DScenePtr
X3DBrowser::importDocument (/*const XML DOMNode &*/)
{
	throw Error <NOT_SUPPORTED> ("XML DOM import is not supported.");
	//return createScene ();
}

///  throws Error <INVALID_URL>, Error <URL_UNAVAILABLE>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
void
X3DBrowser::loadURL (const MFString & url, const MFString & parameter)
{
//	for (const auto & value : parameter)
//	{
//		auto pair = std::vector <std::string> ();
//
//		basic::split (std::back_inserter (pair), value .str (), "=");
//
//		if (pair .size () == 2)
//		{
//			if (pair [0] == "target" and pair [1] not_eq "_self")
//				return;
//		}
//	}

	using namespace std::placeholders;

	get_style_context () -> add_class ("titania-private-invisible"); // getBrowserOptions () -> SplashScreen ()

	prepareEvents () .removeInterest (&X3DBrowser::set_initialized, this);
	finished () .removeInterest (&X3DBrowser::set_scene, this);

	setLoadState (IN_PROGRESS_STATE);

	removeLoadCount (future .getValue ());

	future .setValue (new SceneFuture (this,
	                                   url,
	                                   false,
	                                   std::bind (&X3DBrowser::set_scene_async, this, _1)));

	addLoadCount (future .getValue ());
}

void
X3DBrowser::set_scene_async (X3DScenePtr && scene)
{
	// This function is called from the future. Ensure here that the future is not accidentally deleted when calling replaceWorld.
	// Use finished to get better currentTime on start up of world.

	removeLoadCount (future .getValue ());

	if (scene)
	{
		finished () .addInterest (&X3DBrowser::set_scene, this, std::move (scene));

		addEvent ();
	}
	else
	{
		urlError = future -> getUrlError ();
		setLoadState (FAILED_STATE);
	}
}

void
X3DBrowser::set_scene (const X3DScenePtr & scene)
{
	finished () .removeInterest (&X3DBrowser::set_scene, this);

	replaceWorld (scene);
	setLoadState (COMPLETE_STATE);
}

///  throws Error <INVALID_X3D>, Error <NOT_SUPPORTED>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
X3DScenePtr
X3DBrowser::createX3DFromString (const std::string & string)
{
	ContextLock lock (this);

	FileLoader loader (this);

	return loader .createX3DFromString (string);
}

///  throws Error <INVALID_X3D>, Error <NOT_SUPPORTED>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
X3DScenePtr
X3DBrowser::createX3DFromStream (basic::ifilestream & istream)
{
	ContextLock lock (this);

	FileLoader loader (this);

	return loader .createX3DFromStream (istream);
}

///  throws Error <INVALID_X3D>, Error <NOT_SUPPORTED>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
X3DScenePtr
X3DBrowser::createX3DFromStream (const basic::uri & worldURL, basic::ifilestream & istream)
{
	ContextLock lock (this);

	FileLoader loader (this);

	return loader .createX3DFromStream (worldURL, istream);
}

///  throws Error <INVALID_URL>, Error <URL_UNAVAILABLE>, Error <INVALID_OPERATION_TIMING>
X3DScenePtr
X3DBrowser::createX3DFromURL (const MFString & url)
{
	ContextLock lock (this);

	FileLoader loader (this);

	return loader .createX3DFromURL (url);
}

///  throws Error <INVALID_URL>, Error <URL_UNAVAILABLE>, Error <INVALID_OPERATION_TIMING>
SceneFuturePtr
X3DBrowser::createX3DFromURL (const MFString & url, const SceneFutureCallback & callback)
{
	using namespace std::placeholders;

	return MakePtr <SceneFuture> (getExecutionContext (), url, false, [callback] (X3DScenePtr && scene) { scene -> setup (); callback (std::move (scene)); });
}

///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
void
X3DBrowser::firstViewpoint ()
{
	if (getActiveLayer ())
	{
		const auto viewpoints = getActiveLayer () -> getUserViewpoints ();

		if (not viewpoints .empty ())
			bindViewpoint (viewpoints .front ());
	}
}

///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
void
X3DBrowser::previousViewpoint ()
{
	if (getActiveLayer ())
	{
		const auto viewpoints = getActiveLayer () -> getUserViewpoints ();

		if (viewpoints .empty ())
			return;

		size_t index = 0;

		for (const auto & viewpoint : viewpoints)
		{
			if (viewpoint -> isBound ())
				break;

			++ index;
		}

		if (index < viewpoints .size ())
		{
			if (index == 0)
				bindViewpoint (viewpoints .back ());

			else
				bindViewpoint (viewpoints [index - 1]);
		}
		else
			bindViewpoint (viewpoints .back ());
	}
}

///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
void
X3DBrowser::nextViewpoint ()
{
	if (getActiveLayer ())
	{
		const auto viewpoints = getActiveLayer () -> getUserViewpoints ();

		if (viewpoints .empty ())
			return;

		size_t index = 0;

		for (const auto & viewpoint : viewpoints)
		{
			if (viewpoint -> isBound ())
				break;

			++ index;
		}

		if (index < viewpoints .size ())
		{
			if (index == viewpoints .size () - 1)
				bindViewpoint (viewpoints .front ());

			else
				bindViewpoint (viewpoints [index + 1]);
		}
		else
			bindViewpoint (viewpoints .front ());
	}
}

///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
void
X3DBrowser::lastViewpoint ()
{
	if (getActiveLayer ())
	{
		const auto viewpoints = getActiveLayer () -> getUserViewpoints ();

		if (not viewpoints .empty ())
			bindViewpoint (viewpoints .back ());
	}
}

///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
void
X3DBrowser::changeViewpoint (const std::string & name)
{
	try
	{
		getExecutionContext () -> changeViewpoint (name);
	}
	catch (const Error <INVALID_NAME> & error)
	{
		#ifdef TITANIA_DEBUG
		std::clog << error .what ();
		#endif
	}
}

void
X3DBrowser::bindViewpoint (X3DViewpointNode* const viewpoint)
{
	viewpoint -> setAnimate (true);

	if (viewpoint -> isBound ())
		viewpoint -> transitionStart (viewpoint);

	else
		viewpoint -> set_bind () = true;

	getNotification () -> string () = viewpoint -> description ();
}

///  throws Error <DISPOSED>
void
X3DBrowser::beginUpdate ()
{
	getClock () -> advance ();

	X3DBaseNode::beginUpdate ();
}

// Destruction

void
X3DBrowser::dispose ()
{
	__LOG__ << this << std::endl;

	X3DBrowserContext::dispose ();
	X3DBaseNode::dispose ();

	future .setValue (nullptr);

	supportedFields     .reset ();
	supportedNodes      .reset ();
	supportedComponents .reset ();
	supportedProfiles   .reset ();

	removeChildObjects (getRootNodes ());

	__LOG__ << std::endl;
}

//

X3DBrowser::~X3DBrowser ()
{
	__LOG__ << this << std::endl;
}

} // X3D
} // titania
