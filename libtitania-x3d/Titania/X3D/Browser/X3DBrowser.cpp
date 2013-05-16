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

#include "X3DBrowser.h"

#include "../Components/Navigation/X3DViewpointNode.h"
#include "../Bits/config.h"

#include <csignal>
#include <cstdlib>

namespace titania {
namespace X3D {

void
signal_handler (int sig)
{
	// print out all the frames to stderr
	std::clog << "Error: signal " << sig << ":" << std::endl;
	backtrace_fn (100);
	exit (1);
}

const std::string X3DBrowser::version ("0.2");

X3DBrowser::X3DBrowser () :
	  X3DBrowserContext (),                          
	       X3DUrlObject (),                          
	    supportedFields (this),                      
	     supportedNodes (this),                      
	supportedComponents (this),                      
	  supportedProfiles (this, supportedComponents), 
	        description (),                          // SFString  [in,out] description ""
	              scene (),                          // SFNode    [in,out] scene       NULL
	              world ()                           // SFNode    [in,out] world       NULL
{
	std::clog << "Constructing Browser:" << std::endl;

	// install our handler
	std::signal (SIGSEGV, signal_handler);

	setComponent ("Browser");
	setTypeName ("Browser");
	setName ("Titania");

	addField (outputOnly, "url", url ());

	setChildren (description, scene, world);

	X3D::X3DUrlObject::addURN ("about:splash", get_page ("about/splash.wrl"));

	std::clog << "\tDone constructing Browser." << std::endl;
}

void
X3DBrowser::initialize ()
{
	std::clog << "Initializing Browser ..." << std::endl;

	X3DBrowserContext::initialize ();
	X3DUrlObject::initialize ();

	// Initialize scene
	
	world = scene = createScene ();
	
	if (browserOptions -> splashScreen ())
		world = scene = createX3DFromURL ({ "about:splash" });
		
	// Process outstanding events
	
	getRouter () .processEvents ();
	
	// Update display
	
	bind ();
	update ();

	// Replace world service.

	scene .addInterest (this, &X3DBrowser::set_scene);
	world .addInterest (this, &X3DBrowser::set_world);
	
	// Welcome

	print (std::boolalpha,
	       '\n',
	       std::string (80, '*'), '\n',
	       std::string (80, '*'), '\n',

	       "Welcome to ", getName (), " X3D Browser ", getVersion (), ':', '\n',
	       "\tCompiled at ", __DATE__, " ", __TIME__, '\n',

	       renderingProperties, '\n',
	       javaScriptEngine, '\n',

	       std::string (80, '*'), '\n',
	       std::string (80, '*'), '\n',
	       '\n');

	std::clog
		<< "\tDone initializing Browser." << std::endl
		<< std::endl;
}

X3DBrowser*
X3DBrowser::getBrowser () const
{
	return const_cast <X3DBrowser*> (this);
}

const std::string &
X3DBrowser::getVersion () const
throw (Error <DISPOSED>)
{
	return version;
}

void
X3DBrowser::setDescription (const std::string & value)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	description = value;
}

const SFString &
X3DBrowser::getDescription ()
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	return description;
}

const basic::uri &
X3DBrowser::getWorldURL () const
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	return getExecutionContext () -> getWorldURL ();
}

const FieldTypesArray &
X3DBrowser::getSupportedFields () const
throw (Error <DISPOSED>)
{
	return supportedFields .get ();
}

const BasicNodeArray &
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

const X3DFieldDefinition*
X3DBrowser::getFieldType (const std::string & name) const
throw (Error <INVALID_NAME>)
{
	return supportedFields .get (name);
}

const X3DBaseNode*
X3DBrowser::getNode (const std::string & name) const
throw (Error <INVALID_NAME>)
{
	return supportedNodes .get (name);
}

const ComponentInfo*
X3DBrowser::getComponent (const std::string & name, const size_t level) const
throw (Error <NOT_SUPPORTED>)
{
	return supportedComponents .get (name, level);
}

const ProfileInfo*
X3DBrowser::getProfile (const std::string & name) const
throw (Error <NOT_SUPPORTED>)
{
	return supportedProfiles .get (name);
}

Scene*
X3DBrowser::getExecutionContext () const
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	return *scene;
}

SFNode <Scene>
X3DBrowser::createScene () const
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	SFNode <Scene> scene = new Scene (const_cast <X3DBrowser*> (this));
	scene -> setup ();
	return scene;
}

SFNode <Scene>
X3DBrowser::createScene (const ProfileInfo* profile, const ComponentInfoArray & components) const
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	SFNode <Scene> scene = createScene ();
	scene -> setProfile (profile);
	scene -> addComponents (components);
	return scene;
}

void
X3DBrowser::replaceWorld (const SFNode <Scene> & value)
throw (Error <INVALID_SCENE>)
{
	// Replace world.

	std::clog << "The browser is requested to replace the world:" << std::endl;
	print ("*** The browser is requested to replace the world\n");

	if (not value)
		throw Error <INVALID_SCENE> ("Scene is NULL.");

	clock -> advance ();

	if (scene)
		shutdown .processInterests ();

	scene = value;
	
	std::clog << "Replacing world done." << std::endl;
}

void
X3DBrowser::set_scene ()
{
	if (scene not_eq world)
		world = scene;
}

void
X3DBrowser::set_world ()
{
	bind ();

	// Generate initialized event immediately upon receiving this service.

	initialized = getCurrentTime ();
}

void
X3DBrowser::bind ()
{
	for (auto & layer : scene -> getLayerSet () -> getLayers ())
	{
		if (layer -> getNavigationInfos () .size ())
			layer -> getNavigationInfos () [0] -> set_bind () = true;

		if (layer -> getBackgrounds () .size ())
			layer -> getBackgrounds () [0] -> set_bind () = true;

		if (layer -> getFogs () .size ())
			layer -> getFogs () [0] -> set_bind () = true;

		// Bind first viewpoint in viewpoint stack.

		if (layer -> getViewpoints () .size ())
			layer -> getViewpoints () [0] -> set_bind () = true;
	}

	// Bind viewpoint from URL.

	if (scene -> getWorldURL () .fragment () .length ())
		changeViewpoint (scene -> getWorldURL () .fragment ());
}

void
X3DBrowser::loadURL (const MFString & url)
throw (Error <INVALID_URL>,
       Error <URL_UNAVAILABLE>)
{
	loadURL (url, { });
}

void
X3DBrowser::loadURL (const MFString & url, const MFString & parameter)
throw (Error <INVALID_URL>,
       Error <URL_UNAVAILABLE>)
{
	// where parameter is "target=nameOfFrame"

	SFNode <Scene> scene = getExecutionContext ();

	replaceWorld (createScene ());

	try
	{
		parseIntoScene (getExecutionContext (), url);
		clock -> advance ();
	}
	catch (const Error <INVALID_URL> & error)
	{
		replaceWorld (scene);
		throw error;
	}
	catch (const Error <URL_UNAVAILABLE> & error)
	{
		replaceWorld (scene);
		throw error;
	}
}

SFNode <Scene>
X3DBrowser::importDocument (/*const XML DOMNode &*/)
throw (Error <INVALID_DOCUMENT>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>,
       Error <NOT_SUPPORTED>)
{
	throw Error <NOT_SUPPORTED> ("");
	//return createScene ();
}

void
X3DBrowser::beginUpdate ()
throw (Error <DISPOSED>)
{
	std::clog << "Browser begin update." << std::endl;

	// add parents to fields
}

void
X3DBrowser::endUpdate ()
throw (Error <DISPOSED>)
{
	std::clog << "Browser end update." << std::endl;

	// remove parents from
}

const SFNode <RenderingProperties> &
X3DBrowser::getRenderingProperties () const
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	return renderingProperties;
}

const SFNode <BrowserProperties> &
X3DBrowser::getBrowserProperties () const
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	return browserProperties;
}

const SFNode <BrowserOptions> &
X3DBrowser::getBrowserOptions () const
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	return browserOptions;
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
X3DBrowser::dispose ()
{
	__LOG__ << (void*) this << std::endl;

	supportedFields     .dispose ();
	supportedNodes      .dispose ();
	supportedComponents .dispose ();
	supportedProfiles   .dispose ();

	scene .dispose ();

	X3DBrowserContext::dispose ();

	__LOG__ << std::endl;
}

//

X3DBrowser::~X3DBrowser ()
{ }

} // X3D
} // titania
