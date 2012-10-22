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
#include "../InputOutput/uncompress.h"
#include <Titania/Chrono/SteadyTimer.h>
#include <Titania/Chrono/SystemTimer.h>

namespace titania {
namespace X3D {

const std::string X3DBrowser::version ("0.1");

X3DBrowser::X3DBrowser () :
	X3DExecutionContext (),                                        
	       X3DUrlObject (),                                        
	              clock (new chrono::system_timer <time_type> ()), 
	    supportedFields (this),                                    
	     supportedNodes (this),                                    
	supportedComponents (this),                                    
	  supportedProfiles (this, supportedComponents),               
	renderingProperties (new RenderingProperties (this)),          
	  browserProperties (new BrowserProperties (this)),
	     browserOptions (new BrowserOptions (this)),
	   javaScriptEngine (new JavaScriptEngine (this)),
	             router (),                                        
	       currentSpeed (0),                                       
	   currentFrameRate (0),                                       
	        description (""),                                      
	            changed (),                                        // SFTime   [out]    changed
	              world ()                                         // SFNode   [out]    world
{ }

void
X3DBrowser::initialize ()
{
	X3DExecutionContext::initialize ();
	X3DUrlObject::initialize ();

	renderingProperties -> setup ();
	browserProperties   -> setup ();
	browserOptions      -> setup ();
	javaScriptEngine    -> setup ();

	url .addInterest (this, &X3DBrowser::set_url);

	std::clog
		<< std::boolalpha
		<< std::endl
		<< std::string (80, '*') << std::endl
		<< std::string (80, '*') << std::endl

		<< "Welcome to " << getName () << " X3D Browser " << getVersion () << ':' << std::endl
		<< "\tCompiled at " << __DATE__ << " " << __TIME__ << std::endl;

	std::clog
		<< renderingProperties << std::endl
		<< javaScriptEngine << std::endl;

	std::clog
		<< std::string (80, '*') << std::endl
		<< std::string (80, '*') << std::endl
		<< std::endl;

	if (url .size ())
		set_url ();
}

X3DBrowser*
X3DBrowser::getBrowser () const
{
	return const_cast <X3DBrowser*> (this);
}

Router &
X3DBrowser::getRouter ()
{
	return router;
}

const std::string &
X3DBrowser::getVersion () const
throw (Error <DISPOSED>)
{
	return version;
}

time_type
X3DBrowser::getCurrentTime () const
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	return clock -> cycle ();
}

float
X3DBrowser::getCurrentSpeed () const
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	return currentSpeed;
}

float
X3DBrowser::getCurrentFrameRate () const
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	return currentFrameRate;
}

const FieldTypesArray &
X3DBrowser::getSupportedFieldTypes () const
throw (Error <DISPOSED>)
{
	return supportedFields .getFields ();
}

const BasicNodeArray &
X3DBrowser::getSupportedNodes () const
throw (Error <DISPOSED>)
{
	return supportedNodes .getNodes ();
}

const ComponentInfoArray &
X3DBrowser::getSupportedComponents () const
throw (Error <DISPOSED>)
{
	return supportedComponents .getComponents ();
}

const ProfileInfoArray &
X3DBrowser::getSupportedProfiles () const
throw (Error <DISPOSED>)
{
	return supportedProfiles .getProfiles ();
}

const X3DFieldDefinition*
X3DBrowser::getFieldType (const std::string & name) const
throw (Error <INVALID_NAME>)
{
	return supportedFields .getField (name);
}

const X3DBasicNode*
X3DBrowser::getNode (const std::string & name) const
throw (Error <INVALID_NAME>)
{
	return supportedNodes .getNode (name);
}

const ComponentInfo*
X3DBrowser::getComponent (const std::string & name, const size_t level) const
throw (Error <NOT_SUPPORTED>)
{
	return supportedComponents .getComponent (name, level);
}

const ProfileInfo*
X3DBrowser::getProfile (const std::string & name) const
throw (Error <NOT_SUPPORTED>)
{
	return supportedProfiles .getProfile (name);
}

Scene*
X3DBrowser::getExecutionContext () const
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	return *world -> scene;
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
X3DBrowser::replaceWorld (const SFNode <Scene> & scene)
throw (Error <INVALID_SCENE>)
{
	std::clog << "The browser is requested to replace the world:" << std::endl;

	router .processEvents ();

	world          = new World (this);
	world -> scene = scene;
	world -> setup ();

	std::clog << "Replacing world done." << std::endl;
}

const basic::uri &
X3DBrowser::getWorldURL () const
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	return getExecutionContext () -> getWorldURL ();
}

void
X3DBrowser::set_url ()
{
	try
	{
		requestImmediateLoad ();
	}
	catch (...)
	{ }
}

void
X3DBrowser::requestImmediateLoad ()
{
	loadURL (url);
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

NavigationInfo*
X3DBrowser::getActiveNavigationInfo () const
throw (Error <DISPOSED>)
{
	return getExecutionContext () -> getActiveLayer () -> getActiveNavigationInfo ();
}

void
X3DBrowser::changeViewpoint (const std::string & name)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	try
	{
		const SFNode <X3DViewpointNode> viewpoint = getExecutionContext () -> getNamedNode (name);

		if (viewpoint)
			viewpoint -> set_bind = true;

		else
			throw Error <INVALID_NAME> ("Warning: Node named '" + name + "' is not a viewpoint node.");
	}
	catch (const Error <INVALID_NAME> & error)
	{
		std::clog << error .what ();
	}
}

X3DViewpointNode*
X3DBrowser::getActiveViewpoint ()
throw (Error <DISPOSED>)
{
	return getExecutionContext () -> getActiveLayer () -> getActiveViewpoint ();
}

void
X3DBrowser::print (const X3DObject & object) const
throw (Error <DISPOSED>)
{
	std::cout << CompactStyle << object << std::flush;
}

void
X3DBrowser::println (const X3DObject & object) const
throw (Error <DISPOSED>)
{
	std::cout << CompactStyle << object << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////  Visibility-, Proxmity-, ... Sensor
void
X3DBrowser::addSensor (X3DSensorNode* sensor)
{
	std::clog << "\tAdding sensor " << sensor -> getTypeName () << " to world: " << sensors .size () << " are registered until now." << std::endl;
	sensors .insert (sensor);
	std::clog << "\tNow are " << sensors .size () << " registered." << std::endl;
}

void
X3DBrowser::removeSensor (X3DSensorNode* sensor)
{
	std::clog << "\tRemoving sensor " << sensor -> getTypeName () << " from world: " << sensors .size () << " are registered until now." << std::endl;
	sensors .erase (sensor);
	std::clog << "\tNow are " << sensors .size () << " registered." << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void
X3DBrowser::notify (X3DBasicNode* const node)
{
	assert (node);

	router .notify (node);

	ObjectSet sourceFields;

	changed .processEvents (sourceFields);
}

void
X3DBrowser::update ()
{
	display ();
}

void
X3DBrowser::display ()
{
	getGarbageCollector () .dispose ();

	renderingProperties -> prepare ();

	clock -> advance ();

	currentFrameRate = 1 / clock -> interval ();

	// Calculate current speed every 1/100 second.
	if (getCurrentTime () - priorTime > 0.01)
	{
		Vector3f position = getActiveViewpoint () -> getMatrix () .translation ();

		//std::clog << (position - priorPosition) << ", " << priorPosition << ", " << position << std::endl;

		currentSpeed = abs (Vector3d (position - priorPosition) / priorTime);

		priorPosition = position;
		priorTime     = getCurrentTime ();
	}

	SensorNodeSet sensorsToActivate = sensors;

	for (const auto & sensor : sensorsToActivate)
		sensor -> activate ();

	router .processEvents ();

	world -> display ();
	
	renderingProperties -> display ();

	processInterests ();

	GLenum errorNum = glGetError ();

	if (errorNum not_eq GL_NO_ERROR)
		std::clog << "OpenGL Error at " << SFDouble (getCurrentTime ()) << ": " << gluErrorString (errorNum) << std::endl;
}

void
X3DBrowser::dispose ()
{
	std::clog << "Browser::dispose ..." << std::endl;

	supportedFields     .dispose ();
	supportedNodes      .dispose ();
	supportedComponents .dispose ();
	supportedProfiles   .dispose ();

	getGarbageCollector () .dispose ();

	std::clog << "Browser::dispose done ..." << std::endl;
}

//

X3DBrowser::~X3DBrowser ()
{ }

} // X3D
} // titania
