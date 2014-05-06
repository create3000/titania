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

#include "../Browser/Console.h"
#include "../Browser/X3DBrowserContext.h"
#include "../Configuration/SupportedComponents.h"
#include "../Configuration/SupportedFields.h"
#include "../Configuration/SupportedNodes.h"
#include "../Configuration/SupportedProfiles.h"
#include "../Types/Speed.h"

#include <Titania/Stream/InputFileStream.h>

namespace titania {
namespace X3D {

class X3DBrowser :
	virtual public X3DBaseNode, public X3DBrowserContext
{
public:

	///  @name Common members

	virtual
	const std::string &
	getComponentName () const final override
	{ return componentName; }

	virtual
	const std::string &
	getTypeName () const
	throw (Error <DISPOSED>) final override
	{ return typeName; }

	virtual
	const std::string &
	getContainerField () const final override
	{ return containerField; }

	///  @name Member access

	virtual
	X3DBrowser*
	getBrowser () const;

	const std::string &
	getVersion () const
	throw (Error <DISPOSED>)
	{ return version; }

	void
	setDescription (const std::string & value)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	const SFString &
	getDescription ()
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ return description; }

	double
	getCurrentSpeed () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ return currentSpeed; }

	double
	getCurrentFrameRate () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ return currentFrameRate; }

	virtual
	const basic::uri &
	getWorldURL () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final override;

	const X3DFieldDefinition*
	getSupportedField (const std::string &) const
	throw (Error <INVALID_NAME>);

	const SupporteFieldArray &
	getSupportedFields () const
	throw (Error <DISPOSED>);

	const X3DBaseNode*
	getSupportedNode (const std::string &) const
	throw (Error <INVALID_NAME>);

	const BasicNodeArray &
	getSupportedNodes () const
	throw (Error <DISPOSED>);

	const ComponentInfoArray &
	getSupportedComponents () const
	throw (Error <DISPOSED>);

	const ProfileInfoArray &
	getSupportedProfiles () const
	throw (Error <DISPOSED>);

	ComponentInfoPtr
	getComponent (const std::string &, const size_t) const
	throw (Error <NOT_SUPPORTED>);

	const ProfileInfoPtr &
	getProfile (const std::string &) const
	throw (Error <NOT_SUPPORTED>);

	const ScenePtr &
	getExecutionContext () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ return scene; }

	///  @name X3D Creation Handling

	ScenePtr
	createScene () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	ScenePtr
	createScene (const ProfileInfoPtr &, const ComponentInfoArray &) const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	void
	replaceWorld (const ScenePtr &)
	throw (Error <INVALID_SCENE>,
	       Error <INVALID_OPERATION_TIMING>);

	ScenePtr
	importDocument (/*const XML DOMNode &*/)
	throw (Error <INVALID_DOCUMENT>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>,
	       Error <NOT_SUPPORTED>);

	void
	loadURL (const MFString &)
	throw (Error <INVALID_URL>,
	       Error <URL_UNAVAILABLE>,
	       Error <INVALID_OPERATION_TIMING>);

	void
	loadURL (const MFString &, const MFString &)
	throw (Error <INVALID_URL>,
	       Error <URL_UNAVAILABLE>,
	       Error <INVALID_OPERATION_TIMING>);

	ScenePtr
	createX3DFromString (const std::string &)
	throw (Error <INVALID_X3D>,
	       Error <NOT_SUPPORTED>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	ScenePtr
	createX3DFromStream (basic::ifilestream &)
	throw (Error <INVALID_X3D>,
	       Error <NOT_SUPPORTED>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	ScenePtr
	createX3DFromStream (const basic::uri &, basic::ifilestream &)
	throw (Error <INVALID_X3D>,
	       Error <NOT_SUPPORTED>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	ScenePtr
	createX3DFromURL (const MFString &)
	throw (Error <INVALID_URL>,
	       Error <URL_UNAVAILABLE>,
	       Error <INVALID_OPERATION_TIMING>);

	/// @name Update service

	void
	beginUpdate ()
	throw (Error <DISPOSED>);

	void
	endUpdate ()
	throw (Error <DISPOSED>);

	/// @name Browser properties service

	const RenderingPropertiesPtr &
	getRenderingProperties () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ return renderingProperties; }

	const BrowserPropertiesPtr &
	getBrowserProperties () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ return browserProperties; }

	//	bool
	//	setBrowserOption (SAIString, SAIObject)
	//	throw (Error <INVALID_OPERATION_TIMING>,
	//	       Error <DISPOSED>);

	const BrowserOptionsPtr &
	getBrowserOptions () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ return browserOptions; }

	/// @name Change viewpoint service

	void
	firstViewpoint ()
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	void
	previousViewpoint ()
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	void
	nextViewpoint ()
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	void
	lastViewpoint ()
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	void
	changeViewpoint (const std::string &)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	///  @name print

	template <typename ... Args>
	void
	print (Args && ... args)
	throw (Error <DISPOSED>);

	template <typename ... Args>
	void
	println (Args && ... args)
	throw (Error <DISPOSED>)
	{ print (args ..., '\n'); }

	///  @name Error handling

	const MFString &
	getUrlError () const
	{ return urlError; }

	///  @name Destruction

	virtual
	void
	dispose () override;


protected:

	///  @name Construction

	X3DBrowser ();

	virtual
	void
	initialize () override;

	///  @name Destruction

	virtual
	~X3DBrowser ();


private:

	///  @name Member access

	virtual
	const WorldPtr &
	getWorld () const final override
	{ return world; }

	///  @name Operations

	void
	bindViewpoint (X3DViewpointNode* const);

	template <typename First, typename ... Args>
	void
	print (std::ostringstream & stream, First && first, Args && ... args);

	void
	print (std::ostringstream &)
	{ }

	///  @name Event handlers

	void
	set_prepareEvents ();

	void
	set_scene ();

	///  @name Static members

	static const std::string version;
	static const std::string componentName;
	static const std::string typeName;
	static const std::string containerField;

	///  @name Members

	SFString       description;
	Speed <double> currentSpeed;
	double         currentFrameRate;

	SupportedFields     supportedFields;
	SupportedNodes      supportedNodes;
	SupportedComponents supportedComponents;
	SupportedProfiles   supportedProfiles;

	BrowserOptionsPtr      browserOptions;
	BrowserPropertiesPtr   browserProperties;
	RenderingPropertiesPtr renderingProperties;

	ScenePtr scene;
	WorldPtr world;
	WorldPtr root;
	MFString urlError;
	size_t   inShutdown;

};

template <typename ... Args>
inline
void
X3DBrowser::print (Args && ... args)
throw (Error <DISPOSED>)
{
	std::ostringstream ostream;

	ostream .imbue (std::locale::classic ());

	print (ostream, args ...);

	getConsole () -> set_string () .emplace_back (ostream .str ());

	std::clog << ostream .str ();
}

template <typename First, typename ... Args>
inline
void
X3DBrowser::print (std::ostringstream & ostream, First && first, Args && ... args)
{
	ostream << first;

	print (ostream, args ...);
}

} // X3D
} // titania

#endif
