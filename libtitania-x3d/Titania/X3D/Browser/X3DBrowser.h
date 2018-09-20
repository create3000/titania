// /* -*- Mode: C++; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*- */
// /*************************************************************************
//  *
//  * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
//  *
//  * Copyright 1999, 2016 Holger Seelig <holger.seelig@yahoo.de>.
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
#include "../Configuration/ComponentInfoArray.h"
#include "../Configuration/ProfileInfoArray.h"
#include "../Configuration/SupportedFieldsArray.h"
#include "../Configuration/SupportedNodesArray.h"
#include "../Types/Speed.h"

#include <Titania/Stream/InputFileStream.h>

#include <gtkmm/cssprovider.h>

namespace titania {
namespace X3D {

class X3DBrowser :
	virtual public X3DBaseNode, public X3DBrowserContext
{
public:

	///  @name Common members

	virtual
	ComponentType
	getComponent () const
	throw (Error <DISPOSED>) final override
	{ return component; }

	virtual
	const std::string &
	getTypeName () const
	throw (Error <DISPOSED>) final override
	{ return typeName; }

	virtual
	const std::string &
	getContainerField () const
	throw (Error <DISPOSED>) final override
	{ return containerField; }

	///  @name Member access

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

	const X3DFieldDefinition*
	getSupportedField (const std::string &) const
	throw (Error <NOT_SUPPORTED>,
	       Error <DISPOSED>);

	const SupportedFieldsArray &
	getSupportedFields () const
	throw (Error <DISPOSED>);

	const X3DBaseNode*
	getSupportedNode (const std::string &) const
	throw (Error <NOT_SUPPORTED>,
	       Error <DISPOSED>);

	const SupportedNodesArray &
	getSupportedNodes () const
	throw (Error <DISPOSED>);

	ComponentInfoPtr
	getComponent (const std::string &, const size_t) const
	throw (Error <NOT_SUPPORTED>,
	       Error <DISPOSED>);

	const ComponentInfoArray &
	getSupportedComponents () const
	throw (Error <DISPOSED>);

	const ProfileInfoPtr &
	getProfile (const std::string &) const
	throw (Error <NOT_SUPPORTED>,
	       Error <DISPOSED>);

	const ProfileInfoArray &
	getSupportedProfiles () const
	throw (Error <DISPOSED>);

	///  @name Execution context handling

	virtual
	const basic::uri &
	getWorldURL () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final override
	{ return getExecutionContext () -> getWorldURL (); }

	const X3DExecutionContextPtr &
	getExecutionContext () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ return executionContext; }

	X3DScenePtr
	createScene (const bool setup = true) const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	X3DScenePtr
	createScene (const ProfileInfoPtr &, const ComponentInfoArray &, const bool setup = true) const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	void
	replaceWorld (const X3DExecutionContextPtr & executionContext)
	throw (Error <INVALID_SCENE>,
	       Error <INVALID_OPERATION_TIMING>);

	X3DScenePtr
	importDocument (/*const XML DOMNode &*/)
	throw (Error <INVALID_DOCUMENT>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>,
	       Error <NOT_SUPPORTED>);

	void
	loadURL (const MFString & url, const MFString & parameter)
	throw (Error <INVALID_URL>,
	       Error <URL_UNAVAILABLE>,
	       Error <INVALID_OPERATION_TIMING>);

	X3DScenePtr
	createX3DFromString (const std::string & x3dSyntax)
	throw (Error <INVALID_X3D>,
	       Error <NOT_SUPPORTED>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	X3DScenePtr
	createX3DFromStream (basic::ifilestream & istream)
	throw (Error <INVALID_X3D>,
	       Error <NOT_SUPPORTED>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	X3DScenePtr
	createX3DFromStream (const basic::uri & worldURL, basic::ifilestream & istream)
	throw (Error <INVALID_X3D>,
	       Error <NOT_SUPPORTED>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	X3DScenePtr
	createX3DFromURL (const MFString & url)
	throw (Error <INVALID_URL>,
	       Error <URL_UNAVAILABLE>,
	       Error <INVALID_OPERATION_TIMING>);

	SceneFuturePtr
	createX3DFromURL (const MFString & url, const SceneFutureCallback & callback)
	throw (Error <INVALID_URL>,
	       Error <URL_UNAVAILABLE>,
	       Error <INVALID_OPERATION_TIMING>);

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

	/// @name Begin/End update service

	virtual
	void
	beginUpdate ()
	throw (Error <DISPOSED>) final override;

	///  @name print

	template <typename ... Args>
	void
	print (Args && ... args)
	throw (Error <DISPOSED>)
	{ getConsole () -> print (std::forward <Args> (args) ...); }

	template <typename ... Args>
	void
	println (Args && ... args)
	throw (Error <DISPOSED>)
	{ getConsole () -> print (std::forward <Args> (args) ..., '\n'); }

	///  @name Error handling

	virtual
	const SFEnum <LoadState> &
	checkLoadState () const
	{ return loadState; }

	const MFString &
	getUrlError () const
	{ return urlError; }

	///  @name Destruction

	virtual
	void
	dispose () override;

	virtual
	~X3DBrowser () override;


protected:

	///  @name Construction

	X3DBrowser (const X3DBrowserPtr & sharedBrowser, const MFString &, const MFString &);

	virtual
	void
	initialize () override;


private:

	///  @name Member access

	void
	setLoadState (const LoadState value)
	{ loadState = value; }

	///  @name Operations

	void
	bindViewpoint (X3DViewpointNode* const);

	///  @name Event handlers

	void
	set_loaded (const bool);

	void
	set_prepareEvents ();

	void
	set_loadCount ();

	void
	bind ();

	void
	set_executionContext ();

	void
	set_scene_async (X3DScenePtr &&);

	void
	set_scene (const X3DScenePtr &);

	///  @name Static members

	static const std::string version;
	static const ComponentType component;
	static const std::string   typeName;
	static const std::string   containerField;

	///  @name Members

	Glib::RefPtr <Gtk::CssProvider> cssProvider;

	MFString       url;
	MFString       parameter;
	SFString       description;
	Speed <double> currentSpeed;
	double         currentFrameRate;

	std::shared_ptr <SupportedFields>     supportedFields;
	std::shared_ptr <SupportedNodes>      supportedNodes;
	std::shared_ptr <SupportedComponents> supportedComponents;
	std::shared_ptr <SupportedProfiles>   supportedProfiles;

	X3DExecutionContextPtr executionContext;
	SFEnum <LoadState>     loadState;
	MFString               urlError;
	size_t                 inShutdown;

	X3DPtr <SceneFuture> future;

};

} // X3D
} // titania

#endif
