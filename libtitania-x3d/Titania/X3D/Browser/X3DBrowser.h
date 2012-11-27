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

#ifndef __TITANIA_X3D_BROWSER_X3DBROWSER_H__
#define __TITANIA_X3D_BROWSER_X3DBROWSER_H__

#include "../Browser/X3DBrowserContext.h"
#include "../Browser/Properties/BrowserOptions.h"
#include "../Browser/Properties/BrowserProperties.h"
#include "../Browser/Properties/RenderingProperties.h"

#include "../Configuration/SupportedComponents.h"
#include "../Configuration/SupportedFields.h"
#include "../Configuration/SupportedNodes.h"
#include "../Configuration/SupportedProfiles.h"

#include "../JavaScript/JavaScriptEngine.h"
#include "../Routing/Router.h"

#include "../Components/Navigation/Viewpoint.h"
#include "../Components/Networking/X3DUrlObject.h"

#include "../Execution/Scene.h"
#include "../Execution/World.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "../Browser/HitArray.h"

#include <Titania/Chrono/ClockBase.h>
#include <memory>

namespace titania {
namespace X3D {

typedef chrono::clock_base <time_type> X3DClock;
typedef std::stack <GLenum>            LightStack;

enum EventType
{
	INITIALIZED_EVENT,
	SHUTDOWN_EVENT

};

class X3DBrowser :
	public X3DBrowserContext, public X3DUrlObject
{
public:

	virtual
	X3DBrowser*
	getBrowser () const;

	Router &
	getRouter ();

	const std::string &
	getVersion () const
	throw (Error <DISPOSED>);

	virtual
	time_type
	getCurrentTime () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	float
	getCurrentSpeed () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	float
	getCurrentFrameRate () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	void
	setDescription (const std::string & value)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	const std::string &
	getDescription ()
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	virtual
	const FieldTypesArray &
	getSupportedFields () const
	throw (Error <DISPOSED>);

	virtual
	const BasicNodeArray &
	getSupportedNodes () const
	throw (Error <DISPOSED>);

	virtual
	const ComponentInfoArray &
	getSupportedComponents () const
	throw (Error <DISPOSED>);

	const ProfileInfoArray &
	getSupportedProfiles () const
	throw (Error <DISPOSED>);

	virtual
	const X3DFieldDefinition*
	getFieldType (const std::string &) const
	throw (Error <INVALID_NAME>);

	virtual
	const X3DBasicNode*
	getNode (const std::string &) const
	throw (Error <INVALID_NAME>);

	virtual
	const ComponentInfo*
	getComponent (const std::string &, const size_t) const
	throw (Error <NOT_SUPPORTED>);

	const ProfileInfo*
	getProfile (const std::string &) const
	throw (Error <NOT_SUPPORTED>);

	virtual
	Scene*
	getExecutionContext () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	SFNode <Scene>
	createScene () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	SFNode <Scene>
	createScene (const ProfileInfo*, const ComponentInfoArray &) const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	void
	replaceWorld (const SFNode <Scene> &)
	throw (Error <INVALID_SCENE>);

	void
	loadURL (const MFString &)
	throw (Error <INVALID_URL>,
	       Error <URL_UNAVAILABLE>,
	       Error <INVALID_X3D>);

	void
	loadURL (const MFString &, const MFString &)
	throw (Error <INVALID_URL>,
	       Error <URL_UNAVAILABLE>,
	       Error <INVALID_X3D>);

	SFNode <Scene>
	importDocument (/*const XML DOMNode &*/)
	throw (Error <INVALID_DOCUMENT>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>,
	       Error <NOT_SUPPORTED>);

	void
	beginUpdate ()
	throw (Error <DISPOSED>);

	void
	endUpdate ()
	throw (Error <DISPOSED>);

	const SFNode <RenderingProperties> &
	getRenderingProperties () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	const SFNode <BrowserProperties> &
	getBrowserProperties () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	const SFNode <BrowserOptions> &
	getBrowserOptions () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	NavigationInfo*
	getActiveNavigationInfo () const
	throw (Error <DISPOSED>);

	void
	changeViewpoint (const std::string &)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	X3DViewpointNode*
	getActiveViewpoint ()
	throw (Error <DISPOSED>);

	//	void
	//	changeViewpoint (SAIAction)
	//	throw (Error <INVALID_OPERATION_TIMING>,
	//	       Error <DISPOSED>);
	//
	//	void
	//	changeViewpoint (SAIAction, int32_t SAILayerID)
	//	throw (Error <INVALID_OPERATION_TIMING>,
	//	       Error <DISPOSED>);

	void
	print (const X3DObject &) const
	throw (Error <DISPOSED>);

	void
	println (const X3DObject &) const
	throw (Error <DISPOSED>);

	//	bool
	//	setBrowserOption (SAIString, SAIObject)
	//	throw (Error <INVALID_OPERATION_TIMING>,
	//	       Error <DISPOSED>);

	virtual
	void
	notify (X3DBasicNode* const);

	virtual
	void
	intersect ();

	void
	prepare ();

	virtual
	void
	display ();

	void
	finish ();

	virtual
	void
	dispose ();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual
	void
	pushRenderer (X3DRenderer*) = 0;

	virtual
	void
	popRenderer () = 0;

	virtual
	X3DRenderer*
	getCurrentRenderer () = 0;

	virtual
	const X3DRenderer*
	getCurrentRenderer () const = 0;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	LightStack &
	getLights ();

	////  pushPointingDeviceSensorNode
	virtual
	void
	pushSensitiveNode (X3DBasicNode* node) = 0;

	virtual
	void
	popSensitiveNode () = 0;

	virtual
	const std::vector <X3DBasicNode*> &
	getSensitiveNodes () const = 0;

	virtual
	bool
	isSensitive () const = 0;

	virtual
	void
	pick (const size_t, const size_t) = 0;

	virtual
	Line3f
	getHitRay () const = 0;

	virtual
	void
	addHit (Hit*) = 0;

	virtual
	const HitArray &
	getHits () const = 0;

	////

protected:

	X3DBrowser ();

	virtual
	void
	initialize ();

	virtual
	~X3DBrowser ();


private:

	using X3DUrlObject::url;

	typedef std::set <X3DSensorNode*> SensorNodeSet;

	static const std::string version;

	Router                     router;
	std::shared_ptr <X3DClock> clock;

	SupportedFields             supportedFields;
	SupportedNodes              supportedNodes;
	SupportedComponents         supportedComponents;
	SupportedProfiles           supportedProfiles;

	SFNode <RenderingProperties> renderingProperties;
	SFNode <BrowserProperties>   browserProperties;
	SFNode <BrowserOptions>      browserOptions;
	SFNode <JavaScriptEngine>    javaScriptEngine;

	float currentSpeed;
	float currentFrameRate;

	std::string description;


public:

	SFTime initialized;
	SFTime exposed;
	SFTime displayed;
	SFTime finished;
	SFTime shutdown;
	SFTime changed;


private:

	SFNode <World> world;
	SFNode <Scene> scene;

	void
	set_scene ();

	Vector3f priorPosition;

	LightStack lightStack;

};

} // X3D
} // titania

#endif
