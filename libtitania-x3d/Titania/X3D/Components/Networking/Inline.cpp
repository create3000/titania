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

#include "Inline.h"

#include "../Grouping/Group.h"
#include "../../Browser/Picking/PickingHierarchyGuard.h"
#include "../../Browser/PointingDeviceSensor/HierarchyGuard.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DScene.h"
#include "../../InputOutput/FileLoader.h"
#include "../../Rendering/X3DRenderObject.h"
#include "../../Thread/SceneFuture.h"

namespace titania {
namespace X3D {

static constexpr bool X3D_PARALLEL = true;

const Component   Inline::component      = Component ("Networking", 3);
const std::string Inline::typeName       = "Inline";
const std::string Inline::containerField = "children";

Inline::Fields::Fields () :
	load (new SFBool (true))
{ }

Inline::Inline (X3DExecutionContext* const executionContext) :
	     X3DBaseNode (executionContext -> getBrowser (), executionContext),
	    X3DChildNode (),
	X3DBoundedObject (),
	    X3DUrlObject (),
	          fields (),
	          buffer (),
	           scene (executionContext -> getBrowser () -> createScene (false)),
	           group (new Group (executionContext)),
	          future ()
{
	addType (X3DConstants::Inline);

	addField (inputOutput,    "metadata",   metadata ());
	addField (inputOutput,    "load",       load ());
	addField (inputOutput,    "url",        url ());
	addField (initializeOnly, "bboxSize",   bboxSize ());
	addField (initializeOnly, "bboxCenter", bboxCenter ());

	addChildObjects (scene, group, buffer, future);
}

X3DBaseNode*
Inline::create (X3DExecutionContext* const executionContext) const
{
	return new Inline (executionContext);
}

void
Inline::initialize ()
{
	X3DChildNode::initialize ();
	X3DBoundedObject::initialize ();
	X3DUrlObject::initialize ();

	getExecutionContext () -> isLive () .addInterest (&Inline::set_live, this);
	isLive () .addInterest (&Inline::set_live, this);

	load () .addInterest (&Inline::set_load,   this);
	url ()  .addInterest (&Inline::set_url,    this);
	buffer  .addInterest (&Inline::set_buffer, this);

	setScene (X3DScenePtr (getBrowser () -> getPrivateScene ()));

	group -> isCameraObject ()   .addInterest (&Inline::setCameraObject,   static_cast <X3DChildNode*> (this));
	group -> isPickableObject () .addInterest (&Inline::setPickableObject, static_cast <X3DChildNode*> (this));

	group -> setPrivate (true);
	group -> setup ();

	set_url ();
}

void
Inline::setExecutionContext (X3DExecutionContext* const executionContext)
{
	getExecutionContext () -> isLive () .removeInterest (&Inline::set_live, this);

	if (scene == getExecutionContext () -> getBrowser () -> getPrivateScene ())
		scene = executionContext -> getBrowser () -> getPrivateScene ();
	else if (scene)
		scene -> setExecutionContext (executionContext);

	group -> setExecutionContext (executionContext);

	if (future)
		future -> setExecutionContext (executionContext);

	X3DUrlObject::setExecutionContext (executionContext);
	X3DChildNode::setExecutionContext (executionContext);

	if (isInitialized ())
	{
		getExecutionContext () -> isLive () .addInterest (&Inline::set_live, this);

		set_live ();
	}
}

Box3d
Inline::getBBox () const
{
	if (bboxSize () == Vector3f (-1, -1, -1))
		return group -> getBBox ();

	return Box3d (bboxSize () .getValue (), bboxCenter () .getValue ());
}

///  throws Error <INVALID_NAME>, Error <NODE_NOT_AVAILABLE>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
SFNode
Inline::getExportedNode (const std::string & exportedName) const
{
	if (scene)
		return scene -> getExportedNode (exportedName);

	throw Error <DISPOSED> ("Inline::getExportedNode: scene is disposed.");
}

///  throws Error <NODE_NOT_AVAILABLE>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
const ExportedNodeIndex &
Inline::getExportedNodes () const
{
	if (scene)
		return scene -> getExportedNodes ();

	throw Error <DISPOSED> ("Inline::getExportedNodes: scene is disposed.");
}

MFNode &
Inline::getRootNodes ()
{
	return scene -> getRootNodes ();
}

const MFNode &
Inline::getRootNodes () const
{
	return scene -> getRootNodes ();
}

void
Inline::requestImmediateLoad ()
{
	if (X3D_PARALLEL)
	{
		requestAsyncLoad ();
	}
	else
	{
		if (not getBrowser () -> getLoadUrlObjects ())
			return;
	
		if (checkLoadState () == COMPLETE_STATE or checkLoadState () == IN_PROGRESS_STATE)
			return;
	
		if (future)
		{
			future -> wait ();
			return;
		}
	
		setLoadState (IN_PROGRESS_STATE);
	
		FileLoader loader (getExecutionContext ());
	
		try
		{
			auto scene = loader .createX3DFromURL (url ());

			setLoadState (COMPLETE_STATE);
			setLoadedUrl (scene -> getWorldURL ());
			setScene (std::move (scene));
		}
		catch (const X3DError & error)
		{
			getBrowser () -> getConsole () -> error (error .what (), "\n");
	
			for (const auto & string : loader .getUrlError ())
				getBrowser () -> getConsole () -> error (string .raw (), "\n");

			setLoadState (FAILED_STATE);
			setLoadedUrl ("");
			setScene (X3DScenePtr (getBrowser () -> getPrivateScene ()));
		}
	}
}

void
Inline::requestAsyncLoad ()
{
	if (not getBrowser () -> getLoadUrlObjects ())
		return;

	if (checkLoadState () == COMPLETE_STATE or checkLoadState () == IN_PROGRESS_STATE)
		return;

	setLoadState (IN_PROGRESS_STATE);

	buffer .addEvent ();
}

void
Inline::requestUnload ()
{
	if (checkLoadState () == NOT_STARTED_STATE or checkLoadState () == FAILED_STATE)
		return;

	future .setValue (nullptr);

	setLoadState (NOT_STARTED_STATE);

	setScene (X3DScenePtr (getBrowser () -> getPrivateScene ()));
}

void
Inline::setSceneAsync (X3DScenePtr && value)
{
	if (value)
	{
		setLoadState (COMPLETE_STATE);
		setLoadedUrl (value -> getWorldURL ());
		setScene (std::move (value));
	}
	else
	{
		setLoadState (FAILED_STATE);
		setLoadedUrl ("");
		setScene (X3DScenePtr (getBrowser () -> getPrivateScene ()));
	}
}

void
Inline::setScene (X3DScenePtr && value)
{
	if (scene)
		scene -> getRootNodes () .removeInterest (group -> children ());

	// First initialize,

	if (checkLoadState () == COMPLETE_STATE)
	{
		value -> setExecutionContext (getExecutionContext ());
		value -> isLive () = getExecutionContext () -> isLive () and isLive ();
		value -> setPrivate (getExecutionContext () -> getPrivate ());
		value -> getRootNodes () .addInterest (group -> children ());

		if (isInitialized ())
			value -> setup ();

		else
			getExecutionContext () -> addUninitializedNode (value);
	}

	// then assign.

	scene = std::move (value);

	group -> children () = scene -> getRootNodes ();
}

void
Inline::set_live ()
{
	if (checkLoadState () == COMPLETE_STATE)
	{
		const bool value = getExecutionContext () -> isLive () and isLive ();
		
		if (value not_eq scene -> isLive ())
			scene -> isLive () = value;
	}
}

void
Inline::set_load ()
{
	if (load ())
	{
		setLoadState (NOT_STARTED_STATE);

		requestImmediateLoad ();
	}
	else
		requestUnload ();
}

void
Inline::set_url ()
{
	if (not load ())
		return;

	setLoadState (NOT_STARTED_STATE);

	requestAsyncLoad ();
}

void
Inline::set_buffer ()
{
	using namespace std::placeholders;

	future .setValue (new SceneFuture (getExecutionContext (),
	                                   url (),
	                                   false,
	                                   std::bind (&Inline::setSceneAsync, this, _1)));
}

void
Inline::traverse (const TraverseType type, X3DRenderObject* const renderObject)
{
	switch (type)
	{
		case TraverseType::POINTER:
		{
			HierarchyGuard guard (renderObject -> getBrowser (), this);

			group -> traverse (type, renderObject);

			break;
		}
		case TraverseType::PICKING:
		{
			PickingHierarchyGuard guard (renderObject -> getBrowser (), this);

			group -> traverse (type, renderObject);

			break;
		}
		default:
		{
			group -> traverse (type, renderObject);
			break;
		}
	}
}

void
Inline::dispose ()
{
	future .setValue (nullptr);

	X3DUrlObject::dispose ();
	X3DBoundedObject::dispose ();
	X3DChildNode::dispose ();
}

Inline::~Inline ()
{ }

} // X3D
} // titania
