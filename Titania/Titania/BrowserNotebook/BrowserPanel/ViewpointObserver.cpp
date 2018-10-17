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

#include "ViewpointObserver.h"

#include "../../Browser/X3DBrowserWindow.h"

#include <Titania/X3D/Browser/Navigation/X3DViewer.h>
#include <Titania/X3D/Components/Layering/X3DLayerNode.h>
#include <Titania/X3D/Components/Navigation/NavigationInfo.h>
#include <Titania/X3D/Components/Navigation/X3DViewpointNode.h>

namespace titania {
namespace puck {

ViewpointObserver::ViewpointObserver (X3DBrowserWindow* const browserWindow, const X3D::BrowserPtr & browser_) :
	      X3DBaseInterface (browserWindow, browserWindow -> getCurrentBrowser ()),
	       X3DEditorObject (),
	               browser (browser_),
	            viewerNode (),
	        navigationInfo (),
	        positionOffset (),
	     orientationOffset (),
	centerOfRotationOffset (),
	      fieldOfViewScale (),
	           undoHistory (new X3D::UndoHistory ()),
	    transitionUndoStep (),
	          moveUndoStep (),
	        scrollUndoStep ()
{
	addChildObjects (browser, viewerNode, navigationInfo);

	browser -> getViewer ()               .addInterest (&ViewpointObserver::set_viewer,         this);
	browser -> getActiveNavigationInfo () .addInterest (&ViewpointObserver::set_navigationInfo, this);
	browser -> getActiveViewpoint ()      .addInterest (&ViewpointObserver::set_offsets,        this);

	set_viewer (browser -> getViewer ());

	setup ();
}

void
ViewpointObserver::set_viewer (const X3D::X3DPtr <X3D::X3DViewer> & value)
{
	if (viewerNode)
	{
		viewerNode -> isActive ()   .removeInterest (&ViewpointObserver::set_active,     this);
		viewerNode -> scrollTime () .removeInterest (&ViewpointObserver::set_scrollTime, this);
	}

	viewerNode = value;

	if (viewerNode)
	{
		viewerNode -> isActive ()   .addInterest (&ViewpointObserver::set_active,     this);
		viewerNode -> scrollTime () .addInterest (&ViewpointObserver::set_scrollTime, this);
	}

	set_offsets ();
}

void
ViewpointObserver::set_navigationInfo ()
{
	if (navigationInfo)
	{
		navigationInfo -> transitionStart ()    .removeInterest (&ViewpointObserver::set_transitionStart,    this);
		navigationInfo -> transitionComplete () .removeInterest (&ViewpointObserver::set_transitionComplete, this);
	}

	navigationInfo = browser -> getActiveNavigationInfo ();

	if (navigationInfo)
	{
		navigationInfo -> transitionStart ()    .addInterest (&ViewpointObserver::set_transitionStart,    this);
		navigationInfo -> transitionComplete () .addInterest (&ViewpointObserver::set_transitionComplete, this);
	}
}

void
ViewpointObserver::set_transitionStart (const bool value)
{
	const X3D::X3DPtr <X3D::X3DViewpointNode> viewpoint (browser -> getActiveLayer () -> getViewpoint ());

	transitionUndoStep  = std::make_shared <X3D::UndoStep> ("Viewpoint Transition");
	transitionUndoStep -> addObjects (viewpoint);
	transitionUndoStep -> addUndoFunction (&X3D::SFVec3d::setValue,    std::ref (viewpoint -> positionOffset ()),         viewpoint -> positionOffset ());
	transitionUndoStep -> addUndoFunction (&X3D::SFRotation::setValue, std::ref (viewpoint -> orientationOffset ()),      viewpoint -> orientationOffset ());
	transitionUndoStep -> addUndoFunction (&X3D::SFVec3d::setValue,    std::ref (viewpoint -> centerOfRotationOffset ()), viewpoint -> centerOfRotationOffset ());
	transitionUndoStep -> addUndoFunction (&X3D::SFDouble::setValue,   std::ref (viewpoint -> fieldOfViewScale ()),       viewpoint -> fieldOfViewScale ());
}

void
ViewpointObserver::set_transitionComplete (const bool value)
{
	if (not transitionUndoStep)
		return;

	const X3D::X3DPtr <X3D::X3DViewpointNode> viewpoint (browser -> getActiveLayer () -> getViewpoint ());

	transitionUndoStep -> addObjects (viewpoint);
	transitionUndoStep -> addRedoFunction (&X3D::SFVec3d::setValue,    std::ref (viewpoint -> positionOffset ()),         viewpoint -> positionOffset ());
	transitionUndoStep -> addRedoFunction (&X3D::SFRotation::setValue, std::ref (viewpoint -> orientationOffset ()),      viewpoint -> orientationOffset ());
	transitionUndoStep -> addRedoFunction (&X3D::SFVec3d::setValue,    std::ref (viewpoint -> centerOfRotationOffset ()), viewpoint -> centerOfRotationOffset ());
	transitionUndoStep -> addRedoFunction (&X3D::SFDouble::setValue,   std::ref (viewpoint -> fieldOfViewScale ()),       viewpoint -> fieldOfViewScale ());

	addUndoStep (transitionUndoStep);
}

void
ViewpointObserver::set_active (const bool value)
{
	try
	{
		const X3D::X3DPtr <X3D::X3DViewpointNode> viewpoint (browser -> getActiveLayer () -> getViewpoint ());

		if (value)
		{
			resetUndoGroup ("positionOffset", moveUndoStep);
			beginUndoGroup ("positionOffset", moveUndoStep);
			addUndoFunction (viewpoint, viewpoint -> positionOffset (),         moveUndoStep);
			addUndoFunction (viewpoint, viewpoint -> orientationOffset (),      moveUndoStep);
			addUndoFunction (viewpoint, viewpoint -> centerOfRotationOffset (), moveUndoStep);
			addUndoFunction (viewpoint, viewpoint -> fieldOfViewScale (),       moveUndoStep);
			endUndoGroup ("positionOffset", moveUndoStep);
		}
		else if (moveUndoStep)
		{
			beginRedoGroup ("positionOffset", moveUndoStep);
			addRedoFunction (viewpoint, viewpoint -> positionOffset (),         moveUndoStep);
			addRedoFunction (viewpoint, viewpoint -> orientationOffset (),      moveUndoStep);
			addRedoFunction (viewpoint, viewpoint -> centerOfRotationOffset (), moveUndoStep);
			addRedoFunction (viewpoint, viewpoint -> fieldOfViewScale (),       moveUndoStep);
			endRedoGroup ("positionOffset", moveUndoStep);
	
			set_offsets ();
		}
	}
	catch (const X3D::X3DError &)
	{ }
}

void
ViewpointObserver::set_scrollTime ()
{
	try
	{
		const X3D::X3DPtr <X3D::X3DViewpointNode> viewpoint (browser -> getActiveLayer () -> getViewpoint ());

		if (getUndoStep () not_eq scrollUndoStep or not scrollUndoStep)
		{
			const X3D::Vector3d   positionAux         = viewpoint -> positionOffset ();
			const X3D::Rotation4d orientationAux      = viewpoint -> orientationOffset ();
			const X3D::Vector3d   centerOfRotationAux = viewpoint -> centerOfRotationOffset ();
			const auto            fieldOfViewAux      = viewpoint -> fieldOfViewScale ();
	
			viewpoint -> positionOffset ()         = positionOffset;
			viewpoint -> orientationOffset ()      = orientationOffset;
			viewpoint -> centerOfRotationOffset () = centerOfRotationOffset;
			viewpoint -> fieldOfViewScale ()       = fieldOfViewScale;
	
			beginUndoGroup ("positionOffset", scrollUndoStep);
			addUndoFunction (viewpoint, viewpoint -> positionOffset (),         scrollUndoStep);
			addUndoFunction (viewpoint, viewpoint -> orientationOffset (),      scrollUndoStep);
			addUndoFunction (viewpoint, viewpoint -> centerOfRotationOffset (), scrollUndoStep);
			addUndoFunction (viewpoint, viewpoint -> fieldOfViewScale (),       scrollUndoStep);
			endUndoGroup ("positionOffset", scrollUndoStep);
			
			viewpoint -> positionOffset ()         = positionAux;
			viewpoint -> orientationOffset ()      = orientationAux;
			viewpoint -> centerOfRotationOffset () = centerOfRotationAux;
			viewpoint -> fieldOfViewScale ()       = fieldOfViewAux;
		}
	
		beginRedoGroup ("positionOffset", scrollUndoStep);
		addRedoFunction (viewpoint, viewpoint -> positionOffset (),         scrollUndoStep);
		addRedoFunction (viewpoint, viewpoint -> orientationOffset (),      scrollUndoStep);
		addRedoFunction (viewpoint, viewpoint -> centerOfRotationOffset (), scrollUndoStep);
		addRedoFunction (viewpoint, viewpoint -> fieldOfViewScale (),       scrollUndoStep);
		endRedoGroup ("positionOffset", scrollUndoStep);
	}
	catch (const X3D::X3DError &)
	{ }
}

void
ViewpointObserver::set_offsets ()
{
	try
	{
		const X3D::X3DPtr <X3D::X3DViewpointNode> viewpoint (browser -> getActiveLayer () -> getViewpoint ());
	
		positionOffset         = viewpoint -> positionOffset ();
		orientationOffset      = viewpoint -> orientationOffset ();
		centerOfRotationOffset = viewpoint -> centerOfRotationOffset ();
		fieldOfViewScale       = viewpoint -> fieldOfViewScale ();

		resetUndoGroup ("positionOffset", moveUndoStep);
		resetUndoGroup ("positionOffset", scrollUndoStep);
	}
	catch (const X3D::X3DError &)
	{ }
}

ViewpointObserver::~ViewpointObserver ()
{
	dispose ();
}

} // puck
} // titania
