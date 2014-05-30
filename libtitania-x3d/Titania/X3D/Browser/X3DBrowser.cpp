// /* -*- Mode: C++; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*- */
// /*************************************************************************
//  *
//  * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
//  *
//  * Copyright 1999, 2012 Holger Seelig <holger.seelig@yahoo.de>.
//  *
//  * Titania - a multi-platform office productivity suite
//  *
//  * This file is part of the Titania Project.
//  *
//  * Titania is free software: you can redistribute it and/or modify
//  * it under the terms of the GNU Lesser General Public License version 3
//  * only, as published by the Free Software Foundation.
//  *
//  * Titania is distributed in the hope that it will be useful,
//  * but WITHOUT ANY WARRANTY; without even the implied warranty of
//  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  * GNU Lesser General Public License version 3 for more details
//  * (a copy is included in the LICENSE file that accompanied this code).
//  *
//  * You should have received a copy of the GNU Lesser General Public License
//  * version 3 along with Titania.  If not, see
//  * <http://www.gnu.org/licenses/lgpl.html>
//  * for a copy of the LGPLv3 License.
//  *
//  ************************************************************************/

#include "../../X3D.h"

#include "X3DBrowser.h"

#include "../Bits/config.h"
#include "../Browser/BrowserOptions.h"
#include "../Browser/BrowserProperties.h"
#include "../Browser/Notification.h"
#include "../Browser/RenderingProperties.h"
#include "../Execution/Scene.h"
#include "../Execution/World.h"
#include "../InputOutput/Loader.h"

#include <Titania/Backtrace.h>

namespace titania {
namespace X3D {

const std::string X3DBrowser::version = "0.7.0";

const std::string X3DBrowser::componentName  = "Browser";
const std::string X3DBrowser::typeName       = "Browser";
const std::string X3DBrowser::containerField = "browser";

X3DBrowser::X3DBrowser () :
	        X3DBaseNode (),
	  X3DBrowserContext (),
	        description (),
	       currentSpeed (0),
	   currentFrameRate (0),
	    supportedFields (),
	     supportedNodes (this),
	supportedComponents (true),
	  supportedProfiles (supportedComponents),
	     browserOptions (new BrowserOptions      (this)),
	  browserProperties (new BrowserProperties   (this)),
	renderingProperties (new RenderingProperties (this)),
	   executionContext (createScene ()),
	           urlError (),
	         inShutdown ()
{
	std::clog << "Constructing Browser:" << std::endl;

	enable_backtrace ();

	setName ("Titania");
	addType (X3DConstants::X3DBrowser);

	addChildren (description,
	             browserOptions,
	             browserProperties,
	             renderingProperties,
	             executionContext,
	             urlError);

	std::clog << "\tDone constructing Browser." << std::endl;
}

void
X3DBrowser::initialize ()
{
	std::clog << "Initializing Browser ..." << std::endl;

	X3DBaseNode::initialize ();
	X3DBrowserContext::initialize ();

	browserOptions      -> setup ();
	browserProperties   -> setup ();
	renderingProperties -> setup ();

	// Add necessary routes.

	prepareEvents () .addInterest (this, &X3DBrowser::set_prepareEvents);

	executionContext .addInterest (this, &X3DBrowser::set_executionContext);

	// Show splash screen or proceed with empty scene.

	if (glXGetCurrentContext ())
	{
		try
		{
			if (browserOptions -> splashScreen ())
				executionContext .set (Loader (getScene ()) .createX3DFromURL ({ get_page ("about/splash.x3dv") .str () }));
		}
		catch (const X3DError & error)
		{
			std::clog << error .what () << std::endl;
		}

		replaceWorld (executionContext);

		update ();
	}
	else
		replaceWorld (executionContext);

	// Welcome

	print (std::boolalpha,
	       '\n',
	       std::string (80, '*'), '\n',
	       std::string (80, '*'), '\n',

	       "Welcome to ", getName (), " X3D Browser ", getVersion (), ':', '\n',
	       "\tCompiled at ", __DATE__, " ", __TIME__, '\n',

	       renderingProperties, '\n',
	       getJavaScriptEngine (), '\n',

	       "\tId: ", this, '\n',

	       std::string (80, '*'), '\n',
	       std::string (80, '*'), '\n',
	       '\n');

	initialized () = getCurrentTime ();
}

void
X3DBrowser::set_prepareEvents ()
{
	if (getActiveLayer ())
		currentSpeed .setPosition (getActiveLayer () -> getViewpoint () -> getTransformationMatrix () .origin (), currentFrameRate);

	else
		currentSpeed .setPosition (Vector3f (), 0);

	currentFrameRate = 1 / getClock () -> interval ();
}

X3DBrowser*
X3DBrowser::getBrowser () const
{
	return const_cast <X3DBrowser*> (this);
}

void
X3DBrowser::setDescription (const std::string & value)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	description = value;

	if (not value .empty ())
		getNotification () -> string () = value;
}

const X3DFieldDefinition*
X3DBrowser::getSupportedField (const std::string & name) const
throw (Error <INVALID_NAME>)
{
	return supportedFields .get (name);
}

const SupporteFieldArray &
X3DBrowser::getSupportedFields () const
throw (Error <DISPOSED>)
{
	return supportedFields .get ();
}

const X3DBaseNode*
X3DBrowser::getSupportedNode (const std::string & name) const
throw (Error <INVALID_NAME>)
{
	return supportedNodes .get (name);
}

const BaseNodeArray &
X3DBrowser::getSupportedNodes () const
throw (Error <DISPOSED>)
{
	return supportedNodes .get ();
}

const ComponentInfoArray &
X3DBrowser::getSupportedComponents () const
throw (Error <DISPOSED>)
{
	return supportedComponents .get ();
}

const ProfileInfoArray &
X3DBrowser::getSupportedProfiles () const
throw (Error <DISPOSED>)
{
	return supportedProfiles .get ();
}

ComponentInfoPtr
X3DBrowser::getComponent (const std::string & name, const size_t level) const
throw (Error <NOT_SUPPORTED>)
{
	return supportedComponents .get (name, level);
}

const ProfileInfoPtr &
X3DBrowser::getProfile (const std::string & name) const
throw (Error <NOT_SUPPORTED>)
{
	return supportedProfiles .get (name);
}

ScenePtr
X3DBrowser::createScene () const
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	const ScenePtr scene = new Scene (const_cast <X3DBrowser*> (this));

	scene -> isLive () = false;

	return scene;
}

ScenePtr
X3DBrowser::createScene (const ProfileInfoPtr & profile, const ComponentInfoArray & components) const
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	const ScenePtr scene = createScene ();

	scene -> setProfile (profile);

	for (const auto & component : components)
		scene -> addComponent (component);

	return scene;
}

void
X3DBrowser::replaceWorld (const ScenePtr & value)
throw (Error <INVALID_SCENE>,
       Error <INVALID_OPERATION_TIMING>)
{
	replaceWorld (X3DExecutionContextPtr (value));
}

void
X3DBrowser::replaceWorld (const X3DExecutionContextPtr & value)
throw (Error <INVALID_SCENE>,
       Error <INVALID_OPERATION_TIMING>)
{
	if (makeCurrent ())
	{
		// Process shutdown.

		if (initialized ()) // Don't do this if browser is not initialized.
		{
			if (not inShutdown)
			{
				++ inShutdown;

				//getClock () -> advance ();

				shutdown () .processInterests ();

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
			processEvents ();

			// Replace world.

			setDescription ("");
			browserOptions -> assign (X3D::getBrowser () -> getBrowserOptions ());

			executionContext = value ? value : X3DExecutionContextPtr (createScene ());
			
			isLive () .addInterest (executionContext -> isLive ());
			executionContext -> isLive () = isLive ();
			executionContext -> setup ();

			setWorld (new World (executionContext));
			getWorld () -> setup ();
		}
		else
			executionContext = value;

		if (initialized ())
			initialized () = getCurrentTime ();

		print ("*** The browser is requested to replace the world with '", executionContext -> getWorldURL (), "'.\n");
	}
	else
		throw Error <INVALID_OPERATION_TIMING> ("Invalid operation timing.");
}

void
X3DBrowser::set_executionContext ()
{
	std::clog << "Replacing world done." << std::endl;
}

ScenePtr
X3DBrowser::importDocument (/*const XML DOMNode &*/)
throw (Error <INVALID_DOCUMENT>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>,
       Error <NOT_SUPPORTED>)
{
	throw Error <NOT_SUPPORTED> ("XML DOM import is not supported.");
	//return createScene ();
}

void
X3DBrowser::loadURL (const MFString & url)
throw (Error <INVALID_URL>,
       Error <URL_UNAVAILABLE>,
       Error <INVALID_OPERATION_TIMING>)
{
	loadURL (url, { });
}

void
X3DBrowser::loadURL (const MFString & url, const MFString & parameter)
throw (Error <INVALID_URL>,
       Error <URL_UNAVAILABLE>,
       Error <INVALID_OPERATION_TIMING>)
{
	if (makeCurrent ())
	{
		// where parameter is "target=nameOfFrame"

		Loader loader (this);

		try
		{
			const ScenePtr scene = createScene ();

			loader .parseIntoScene (scene, url);

			replaceWorld (scene);

			getClock () -> advance ();

			return;
		}
		catch (const X3DError &)
		{
			urlError = loader .getUrlError ();

			for (const auto & string : urlError)
				getBrowser () -> println (string .str ());

			throw;
		}
	}

	throw Error <INVALID_OPERATION_TIMING> ("Invalid operation timing.");
}

ScenePtr
X3DBrowser::createX3DFromString (const std::string & string)
throw (Error <INVALID_X3D>,
       Error <NOT_SUPPORTED>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	if (makeCurrent ())
		return Loader (this) .createX3DFromString (string);

	throw Error <INVALID_OPERATION_TIMING> ("Invalid operation timing.");
}

ScenePtr
X3DBrowser::createX3DFromStream (basic::ifilestream & istream)
throw (Error <INVALID_X3D>,
       Error <NOT_SUPPORTED>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	if (makeCurrent ())
		return Loader (this) .createX3DFromStream (istream);

	throw Error <INVALID_OPERATION_TIMING> ("Invalid operation timing.");
}

ScenePtr
X3DBrowser::createX3DFromStream (const basic::uri & worldURL, basic::ifilestream & istream)
throw (Error <INVALID_X3D>,
       Error <NOT_SUPPORTED>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	if (makeCurrent ())
		return Loader (this) .createX3DFromStream (worldURL, istream);

	throw Error <INVALID_OPERATION_TIMING> ("Invalid operation timing.");
}

ScenePtr
X3DBrowser::createX3DFromURL (const MFString & url)
throw (Error <INVALID_URL>,
       Error <URL_UNAVAILABLE>,
       Error <INVALID_OPERATION_TIMING>)
{
	if (makeCurrent ())
	{
		try
		{
			Loader loader (this);

			const ScenePtr scene = loader .createX3DFromURL (url);

			return scene;
		}
		catch (const X3DError & error)
		{
			urlError = MFString ({ error .what () });

			throw;
		}
	}

	throw Error <INVALID_OPERATION_TIMING> ("Invalid operation timing.");
}

void
X3DBrowser::firstViewpoint ()
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	if (getActiveLayer ())
	{
		const auto viewpoints = getActiveLayer () -> getUserViewpoints ();

		if (not viewpoints .empty ())
			bindViewpoint (viewpoints .front ());
	}
}

void
X3DBrowser::previousViewpoint ()
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	if (getActiveLayer ())
	{
		const auto viewpoints = getActiveLayer () -> getUserViewpoints ();

		if (not viewpoints .empty ())
		{
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
}

void
X3DBrowser::nextViewpoint ()
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	if (getActiveLayer ())
	{
		const auto viewpoints = getActiveLayer () -> getUserViewpoints ();

		if (not viewpoints .empty ())
		{
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
}

void
X3DBrowser::lastViewpoint ()
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	if (getActiveLayer ())
	{
		const auto viewpoints = getActiveLayer () -> getUserViewpoints ();

		if (not viewpoints .empty ())
			bindViewpoint (viewpoints .back ());
	}
}

void
X3DBrowser::changeViewpoint (const std::string & name)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	try
	{
		getExecutionContext () -> changeViewpoint (name);
	}
	catch (const Error <INVALID_NAME> & error)
	{
		std::clog << error .what ();
	}
}

void
X3DBrowser::bindViewpoint (X3DViewpointNode* const viewpoint)
{
	if (viewpoint -> isBound ())
		viewpoint -> transitionStart (viewpoint);

	else
		viewpoint -> set_bind () = true;

	getNotification () -> string () = viewpoint -> description ();
}

// Destruction

void
X3DBrowser::dispose ()
{
	__LOG__ << this << std::endl;

	lock ();

	supportedFields .dispose ();
	supportedNodes  .dispose ();

	X3DBrowserContext::dispose ();
	X3DBaseNode::dispose ();

	unlock ();

	__LOG__ << std::endl;
}

//

X3DBrowser::~X3DBrowser ()
{
	__LOG__ << std::endl;
}

} // X3D
} // titania
