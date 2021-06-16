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

#include "X3DSelector.h"

#include "../../Browser/PointingDeviceSensor/PointingDevice.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Components/Layering/X3DLayerNode.h"
#include "../../Rendering/FrameBuffer.h"
#include "../../Rendering/OpenGL.h"

#include <Titania/Math/Geometry/Camera.h>
#include <Titania/Math/Mesh/Tessellator.h>

namespace titania {
namespace X3D {

X3DSelector::X3DSelector () :
	X3DExamineViewer (),
	       touchable (false),
	          points ()
{
	addType (X3DConstants::X3DSelector);
}

void
X3DSelector::initialize ()
{
	X3DExamineViewer::initialize ();

	touchable = getExecutionContext () -> getBrowser () -> getPointingDevice () -> getEnabled ();

	//getBrowser () -> etPointingDevice () -> setEnabled (false);
}

bool
X3DSelector::on_1button1_press_event (GdkEventButton* event)
{
	getBrowser () -> addEvent ();
	getBrowser () -> displayed () .addInterest (&X3DSelector::display, this);

	clear ();
	addPoint (event -> x, event -> y);
	return false;
}

bool
X3DSelector::on_1button1_release_event (GdkEventButton* event)
{
	try
	{
		if (points .empty ())
			return false;

		ContextLock lock (getBrowser ());

		const auto & viewport = getBrowser () -> getViewport ();

		getBrowser () -> addEvent ();
		getBrowser () -> displayed () .removeInterest (&X3DSelector::display, this);

		// Hide tools.

		getBrowser () -> getDisplayTools () .push (false);

		// Depth buffer

		getBrowser () -> getDepthBuffer () .reset (new FrameBuffer (getBrowser (), viewport [2], viewport [3], 0, true));
		getBrowser () -> getDepthBuffer () -> setup ();
		getBrowser () -> getDepthBuffer () -> bind ();

		getBrowser () -> getActiveLayer () -> traverse (TraverseType::DEPTH, nullptr);

		getBrowser () -> getDepthBuffer () -> readDepth ();
		getBrowser () -> getDepthBuffer () -> unbind ();

		// Selection buffer

		getBrowser () -> getSelectionBuffer () .reset (new FrameBuffer (getBrowser (), viewport [2], viewport [3], 0));
		getBrowser () -> getSelectionBuffer () -> setup ();
		getBrowser () -> getSelectionBuffer () -> bind ();

		draw ();

		getBrowser () -> getSelectionBuffer () -> readPixels ();
		getBrowser () -> getSelectionBuffer () -> unbind ();

		// Touch

		const auto selectionType = getBrowser () -> getSelectionType ();

		getBrowser () -> setSelectionType (SelectionType::LASSO);
		getBrowser () -> touch (points [0] .x (), points [0] .y ());
		getBrowser () -> setSelectionType (selectionType);

		// Show tools.

		getBrowser () -> getDisplayTools () .pop ();

		// Reset

		getBrowser () -> getSelectionBuffer () .reset ();
		getBrowser () -> getDepthBuffer () .reset ();
		return false;
	}
	catch (const X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
		return false;
	}
}

bool
X3DSelector::on_motion1_notify_event (GdkEventMotion* event)
{
	getBrowser () -> addEvent ();

	addPoint (event -> x, event -> y);
	return false;
}

void
X3DSelector::addPoint (const double x, const double y)
{
	const auto & viewport = getBrowser () -> getViewport ();
	const double height   = viewport [3];

	points .emplace_back (x, height - y, 0);
}

void
X3DSelector::clear ()
{
	points .clear ();
}

void
X3DSelector::display ()
{
	PolygonModeLock polygonMode (GL_FILL);

	// Configure HUD

	const auto f = make_rgba <double> (210, 225, 252, 43);
	const auto s = make_rgba <double> (210, 225, 252, 255);

	const auto & viewport = getBrowser () -> getViewport ();
	const auto & width    = viewport [2];
	const auto & height   = viewport [3];

	const auto projection = camera <double>::ortho (0, width, 0, height, -1, 1);

	glMatrixMode (GL_PROJECTION);
	glLoadMatrixd (projection .front () .data ());
	glMatrixMode (GL_MODELVIEW);

	// Display Lasso.
	// Draw a black and a white line.

	glDisable (GL_DEPTH_TEST);
	glLoadIdentity ();

	glEnable (GL_BLEND);
	glDisable (GL_CULL_FACE);
	glColor4f (f .r (), f .g (), f .b (), f .a ());
	polygon ();
	glDisable (GL_BLEND);

	glEnableClientState (GL_VERTEX_ARRAY);
	glVertexPointer (3, GL_DOUBLE, 0, points .data ());

	glLineWidth (2);
	glColor4f (s .r (), s .g (), s .b (), s .a ());
	glDrawArrays (GL_LINE_LOOP, 0, points .size ());

	glDisableClientState (GL_VERTEX_ARRAY);
	glEnable (GL_DEPTH_TEST);
}

void
X3DSelector::draw ()
{
	PolygonModeLock polygonMode (GL_FILL);

	// Configure HUD

	const auto & viewport = getBrowser () -> getViewport ();
	const auto & width    = viewport [2];
	const auto & height   = viewport [3];

	const auto projection = camera <double>::ortho (0, width, 0, height, -1, 1);

	glClearColor (0, 0, 0, 0);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode (GL_PROJECTION);
	glLoadMatrixd (projection .front () .data ());
	glMatrixMode (GL_MODELVIEW);

	// Display Lasso.
	// Draw a black and a white line.

	glDisable (GL_DEPTH_TEST);
	glLoadIdentity ();

	glEnable (GL_BLEND);
	glDisable (GL_CULL_FACE);
	glColor4f (1, 1, 1, 1);
	polygon ();
	glDisable (GL_BLEND);
}

void
X3DSelector::polygon ()
{
	math::tessellator <double, int32_t> tessellator;

	tessellator .begin_polygon ();
	tessellator .begin_contour ();

	for (const auto & point : points)
		tessellator .add_vertex (point, 0);

	tessellator .end_contour ();
	tessellator .end_polygon ();

	for (const auto & polygon : tessellator .polygons ())
	{
		glBegin (polygon .type ());

		for (const auto & vertex : polygon)
			glVertex3dv (vertex .point () .data ());

		glEnd ();
	}
}

void
X3DSelector::dispose ()
{
	//getBrowser () -> etPointingDevice () -> setEnabled (touchable);

	X3DExamineViewer::dispose ();
}

X3DSelector::~X3DSelector ()
{ }

} // X3D
} // titania
