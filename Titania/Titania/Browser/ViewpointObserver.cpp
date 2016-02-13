/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstraﬂe 31a, Leipzig, Germany 2011.
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
 * Copyright 1999, 2012 Holger Seelig <holger.seelig@yahoo.de>.
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

#include "../Browser/X3DBrowserWindow.h"

#include <Titania/X3D/Browser/Navigation/X3DViewer.h>
#include <Titania/X3D/Components/Layering/X3DLayerNode.h>

namespace titania {
namespace puck {

ViewpointObserver::ViewpointObserver (X3DBrowserWindow* const browserWindow) :
	      X3DBaseInterface (browserWindow, browserWindow -> getCurrentBrowser ()),
	       X3DEditorObject (),
	            viewerNode (),
	              undoStep (),
	        positionOffset (),
	     orientationOffset (),
	centerOfRotationOffset (),
	      fieldOfViewScale ()
{
	setup ();
}

void
ViewpointObserver::setBrowser (const X3D::BrowserPtr & current, const X3D::BrowserPtr & value)
{
	current -> getActiveViewpointEvent () .removeInterest (this, &ViewpointObserver::set_offsets);
	current -> getViewer ()               .removeInterest (this, &ViewpointObserver::set_viewer);

	value -> getActiveViewpointEvent () .addInterest (this, &ViewpointObserver::set_offsets);
	value -> getViewer ()               .addInterest (this, &ViewpointObserver::set_viewer);

	set_viewer (value -> getViewer ());
	set_offsets ();
}

void
ViewpointObserver::set_viewer (const X3D::X3DPtr <X3D::X3DViewer> & value)
{
	if (viewerNode)
	{
		viewerNode -> isActive ()   .removeInterest (this, &ViewpointObserver::set_viewer_active);
		viewerNode -> scrollTime () .removeInterest (this, &ViewpointObserver::set_viewer_scrollTime);
	}

	viewerNode = value;

	if (viewerNode)
	{
		viewerNode -> isActive ()   .addInterest (this, &ViewpointObserver::set_viewer_active);
		viewerNode -> scrollTime () .addInterest (this, &ViewpointObserver::set_viewer_scrollTime);
	}
}

void
ViewpointObserver::set_viewer_active (const bool value)
{
	try
	{
		const X3D::X3DPtr <X3D::X3DViewpointNode> activeViewpoint = getCurrentBrowser () -> getActiveLayer () -> getViewpoint ();
	
		if (value)
		{
			resetUndoGroup ("positionOffset", undoStep);
			beginUndoGroup ("positionOffset", undoStep);
			addUndoFunction (activeViewpoint, activeViewpoint -> positionOffset (),         undoStep);
			addUndoFunction (activeViewpoint, activeViewpoint -> orientationOffset (),      undoStep);
			addUndoFunction (activeViewpoint, activeViewpoint -> centerOfRotationOffset (), undoStep);
			addUndoFunction (activeViewpoint, activeViewpoint -> fieldOfViewScale (),       undoStep);
			endUndoGroup ("positionOffset", undoStep);
		}
		else
		{
			beginRedoGroup ("positionOffset", undoStep);
			addRedoFunction (activeViewpoint, activeViewpoint -> positionOffset (),         undoStep);
			addRedoFunction (activeViewpoint, activeViewpoint -> orientationOffset (),      undoStep);
			addRedoFunction (activeViewpoint, activeViewpoint -> centerOfRotationOffset (), undoStep);
			addRedoFunction (activeViewpoint, activeViewpoint -> fieldOfViewScale (),       undoStep);
			endRedoGroup ("positionOffset", undoStep);
	
			set_offsets ();
		}
	}
	catch (const X3D::X3DError &)
	{ }
}

void
ViewpointObserver::set_viewer_scrollTime ()
{
	try
	{
		const X3D::X3DPtr <X3D::X3DViewpointNode> activeViewpoint = getCurrentBrowser () -> getActiveLayer () -> getViewpoint ();
	
		if (getUndoStep () != undoStep or not undoStep)
		{
			const X3D::Vector3d   positionAux         = activeViewpoint -> positionOffset ();
			const X3D::Rotation4d orientationAux      = activeViewpoint -> orientationOffset ();
			const X3D::Vector3d   centerOfRotationAux = activeViewpoint -> centerOfRotationOffset ();
			const auto            fieldOfViewAux      = activeViewpoint -> fieldOfViewScale ();
	
			activeViewpoint -> positionOffset ()         = positionOffset;
			activeViewpoint -> orientationOffset ()      = orientationOffset;
			activeViewpoint -> centerOfRotationOffset () = centerOfRotationOffset;
			activeViewpoint -> fieldOfViewScale ()       = fieldOfViewScale;
	
			beginUndoGroup ("positionOffset", undoStep);
			addUndoFunction (activeViewpoint, activeViewpoint -> positionOffset (),         undoStep);
			addUndoFunction (activeViewpoint, activeViewpoint -> orientationOffset (),      undoStep);
			addUndoFunction (activeViewpoint, activeViewpoint -> centerOfRotationOffset (), undoStep);
			addUndoFunction (activeViewpoint, activeViewpoint -> fieldOfViewScale (),       undoStep);
			endUndoGroup ("positionOffset", undoStep);
			
			activeViewpoint -> positionOffset ()         = positionAux;
			activeViewpoint -> orientationOffset ()      = orientationAux;
			activeViewpoint -> centerOfRotationOffset () = centerOfRotationAux;
			activeViewpoint -> fieldOfViewScale ()       = fieldOfViewAux;
		}
	
		beginRedoGroup ("positionOffset", undoStep);
		addRedoFunction (activeViewpoint, activeViewpoint -> positionOffset (),         undoStep);
		addRedoFunction (activeViewpoint, activeViewpoint -> orientationOffset (),      undoStep);
		addRedoFunction (activeViewpoint, activeViewpoint -> centerOfRotationOffset (), undoStep);
		addRedoFunction (activeViewpoint, activeViewpoint -> fieldOfViewScale (),       undoStep);
		endRedoGroup ("positionOffset", undoStep);
	}
	catch (const X3D::X3DError &)
	{ }
}

void
ViewpointObserver::set_offsets ()
{
	try
	{
		const X3D::X3DPtr <X3D::X3DViewpointNode> activeViewpoint = getCurrentBrowser () -> getActiveLayer () -> getViewpoint ();
	
		positionOffset         = activeViewpoint -> positionOffset ();
		orientationOffset      = activeViewpoint -> orientationOffset ();
		centerOfRotationOffset = activeViewpoint -> centerOfRotationOffset ();
		fieldOfViewScale       = activeViewpoint -> fieldOfViewScale ();

		resetUndoGroup ("positionOffset", undoStep);
	}
	catch (const X3D::X3DError &)
	{ }
}

} // puck
} // titania
