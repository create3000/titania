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

#include "X3DBaseInterface.h"

#include <Titania/X3D/Browser/X3DBrowser.h>
#include <Titania/X3D/Components/Core/MetadataSet.h>
#include <Titania/X3D/Components/Core/WorldInfo.h>
#include <Titania/X3D/Components/Core/WorldInfo.h>
#include <Titania/X3D/Components/Core/X3DPrototypeInstance.h>
#include <Titania/X3D/Components/Layering/X3DLayerNode.h>
#include <Titania/X3D/Execution/World.h>
#include <Titania/X3D/Execution/X3DExecutionContext.h>

#include "../Browser/X3DBrowserWindow.h"

#include <cassert>

namespace titania {
namespace puck {

const std::string X3DBaseInterface::typeName = "X3DBaseInterface";

///  Constructor.
X3DBaseInterface::X3DBaseInterface (X3DBrowserWindow* const browserWindow, X3D::Browser* const browser) :
	     sigc::trackable (),
	X3D::X3DParentObject (browser),
	       browserWindow (browserWindow)
{
	assert (browserWindow);
}

///  Dummy constructor.
X3DBaseInterface::X3DBaseInterface () :
	     sigc::trackable (),
	X3D::X3DParentObject (nullptr),
	       browserWindow (nullptr)
{
	assert (false);
}

///  Setups this object.
void
X3DBaseInterface::setup ()
{
	X3D::X3DParentObject::setup ();

	browserWindow -> getCurrentBrowser () .addInterest (&X3DBaseInterface::set_browser, this);
}

///  Return the master browser.
const X3D::BrowserPtr &
X3DBaseInterface::getMasterBrowser () const
{
	return browserWindow -> getMasterBrowser ();
}

///  Return the current browser.
const X3D::BrowserPtr &
X3DBaseInterface::getCurrentBrowser () const
{
	return browserWindow -> getCurrentBrowser ();
}

///  Return the current world.
const X3D::WorldPtr &
X3DBaseInterface::getCurrentWorld () const
{
	return getCurrentBrowser () -> getWorld ();
}

///  Return the current scene.
const X3D::X3DScenePtr &
X3DBaseInterface::getCurrentScene () const
{
	return browserWindow -> getCurrentScene ();
}

///  Sets the current execution context.
void
X3DBaseInterface::setCurrentContext (const X3D::X3DExecutionContextPtr & executionContext)
{
	return browserWindow -> setCurrentContext (executionContext);
}

///  Return the current execution context.
const X3D::X3DExecutionContextPtr &
X3DBaseInterface::getCurrentContext () const
{
	return browserWindow -> getCurrentContext ();
}

///  Return true if the current execution context is a prototype.
bool
X3DBaseInterface::inProtoDeclaration () const
{
	return getCurrentContext () -> isProtoDeclaration ();
}

///  Return true if the current execution context is a prototype instance.
bool
X3DBaseInterface::inPrototypeInstance () const
{
	static const X3D::NodeTypeSet protoInstance = { X3D::X3DConstants::X3DPrototypeInstance };

	return getCurrentContext () -> isType (protoInstance);
}

///  Return the WorldInfo node from the current scene. The node is created if needed.
X3D::X3DPtr <X3D::WorldInfo>
X3DBaseInterface::createCurrentWorldInfo ()
throw (X3D::Error <X3D::NOT_SUPPORTED>)
{
	return getCurrentWorldInfo (true);
}

///  Return the WorldInfo node from the current scene, otherwise it throws an exception.
X3D::X3DPtr <X3D::WorldInfo>
X3DBaseInterface::getCurrentWorldInfo () const
throw (X3D::Error <X3D::NOT_SUPPORTED>)
{
	return const_cast <X3DBaseInterface*> (this) -> getCurrentWorldInfo (false);
}

///  Return the WorldInfo node from the current scene. If @a create is true, the node is created if needed, otherwise it
///  throws an exception.
X3D::X3DPtr <X3D::WorldInfo>
X3DBaseInterface::getCurrentWorldInfo (const bool create)
throw (X3D::Error <X3D::NOT_SUPPORTED>)
{
	auto worldInfo = getCurrentScene () -> getWorldInfo ();

	if (not worldInfo)
	{
		if (not create)
			throw X3D::Error <X3D::NOT_SUPPORTED> ("X3DBaseInterface::getCurrentWorldInfo: not supported.");
	
		worldInfo = getCurrentScene () -> createNode <X3D::WorldInfo> ();
		worldInfo -> title () = getCurrentScene () -> getWorldURL () .basename (false);

		getCurrentScene () -> getRootNodes () .emplace_front (worldInfo);
		getCurrentScene () -> realize ();
	}

	return worldInfo;
}

///  If needed, this function creates a MetaDataSet from the active layer or if no active layer from the current WorldInfo node.
X3D::X3DPtr <X3D::MetadataSet>
X3DBaseInterface::createMetaData (const std::string & key)
{
	const auto & layerSet = getCurrentWorld () -> getLayerSet ();

	if (layerSet -> getActiveLayer () and layerSet -> getActiveLayer () not_eq layerSet -> getLayer0 ())
		return layerSet -> getActiveLayer () -> createMetaData <X3D::MetadataSet> (key);

	return createCurrentWorldInfo () -> createMetaData <X3D::MetadataSet> (key);
}

///  Returns a MetaDataSet from the active layer or if no active layer from the current WorldInfo node.
X3D::X3DPtr <X3D::MetadataSet>
X3DBaseInterface::getMetaData (const std::string & key) const
{
	const auto & layerSet = getCurrentWorld () -> getLayerSet ();

	if (layerSet -> getActiveLayer () and layerSet -> getActiveLayer () not_eq layerSet -> getLayer0 ())
		return layerSet -> getActiveLayer () -> getMetaData <X3D::MetadataSet> (key);
	
	return getCurrentWorldInfo () -> getMetaData <X3D::MetadataSet> (key);
}

///  Adds @a undoStep to the current browser.
void
X3DBaseInterface::addUndoStep (const X3D::UndoStepPtr & undoStep)
{
	browserWindow -> addUndoStep (undoStep);
}

///  Removes the last undo step from the current browser.
void
X3DBaseInterface::removeUndoStep ()
{
	browserWindow -> removeUndoStep ();
}

///  Returns the last undo step from the current browser.
const X3D::UndoStepPtr &
X3DBaseInterface::getUndoStep () const
{
	return browserWindow -> getUndoStep ();
}

///  Sets the current browser to the underlying X3DParentObject.
void
X3DBaseInterface::set_browser (const X3D::BrowserPtr & value)
{
	setBrowser (value);
}

///  Disposed this on_show_all_objects_activated.
void
X3DBaseInterface::dispose ()
{
	notify_callbacks ();

	X3D::X3DParentObject::dispose ();
}

/// Destructor
X3DBaseInterface::~X3DBaseInterface ()
{ }

} // puck
} // titania
