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

#include "../Browser.h"

#include "LassoSelection.h"

#include "../../Components/Layering/X3DLayerNode.h"

#include <Titania/Math/Geometry/Camera.h>

namespace titania {
namespace X3D {

LassoSelection::LassoSelection (Browser* const browser) :
	X3DSelector (browser),
	  sensitive (browser -> isSensitive ()),
	     button (0),
	     points ()
{ }

void
LassoSelection::initialize ()
{
	__LOG__ << std::endl;

	X3DSelector::initialize ();

	getBrowser () -> signal_button_press_event ()   .connect (sigc::mem_fun (*this, &LassoSelection::on_button_press_event),   false);
	getBrowser () -> signal_button_release_event () .connect (sigc::mem_fun (*this, &LassoSelection::on_button_release_event), false);
	getBrowser () -> signal_motion_notify_event  () .connect (sigc::mem_fun (*this, &LassoSelection::on_motion_notify_event),  false);

	getBrowser () -> isSensitive (false);
}

bool
LassoSelection::on_button_press_event (GdkEventButton* event)
{
	__LOG__ << std::endl;
	
	if (getBrowser () -> hasControlKey () and getBrowser () -> hasShiftKey ())
		return false;

	getBrowser () -> addEvent ();

	button = event -> button;

	if (event -> button == 1)
	{
		if (getBrowser () -> getActiveLayer ())
			getBrowser () -> getActiveLayer () -> getViewpoint () -> transitionStop ();
		
		getBrowser () -> displayed () .addInterest (this, &LassoSelection::display);
		
		clear ();
		addPoint (event -> x, event -> y);

		return true;
	}

	return false;
}

bool
LassoSelection::on_button_release_event (GdkEventButton* event)
{
	if (getBrowser () -> hasControlKey () and getBrowser () -> hasShiftKey ())
		return false;

	getBrowser () -> addEvent ();

	button = 0;

	if (event -> button == 1)
	{
		getBrowser () -> displayed () .removeInterest (this, &LassoSelection::display);

		return true;
	}

	return false;
}

bool
LassoSelection::on_motion_notify_event (GdkEventMotion* event)
{
	if (getBrowser () -> hasControlKey () and getBrowser () -> hasShiftKey ())
		return false;

	getBrowser () -> addEvent ();

	if (button == 1)
	{
		addPoint (event -> x, event -> y);

		return true;
	}

	return false;
}

void
LassoSelection::addPoint (const double x, const double y)
{
	const auto & viewport = getBrowser () -> getRectangle ();
	const double height   = viewport [3];

	points .emplace_back (x, height - y, 0);
}

void
LassoSelection::clear ()
{
	points .clear ();
}

void
LassoSelection::display ()
{
	__LOG__ << std::endl;

	try
	{
		// Configure HUD

		const auto & viewport = getBrowser () -> getRectangle ();
		const int    width    = viewport [2];
		const int    height   = viewport [3];

		const Matrix4d projection = ortho <float> (0, width, 0, height, -1, 1);

		glDisable (GL_DEPTH_TEST);

		glMatrixMode (GL_PROJECTION);
		glLoadMatrixd (projection .data ());
		glMatrixMode (GL_MODELVIEW);

		// Display Lasso.
		// Draw a black and a white line.

		glLoadIdentity ();

		glLineWidth (2);
		glColor3f (0, 0, 0);

		glEnableClientState (GL_VERTEX_ARRAY);
		glVertexPointer (3, GL_FLOAT, 0, points .data ());

		glDrawArrays (GL_LINE_LOOP, 0, points .size ());

		glLineWidth (1);
		glColor3f (1, 1, 1);
	
		glVertexPointer (3, GL_FLOAT, 0, points .data ());
		glDrawArrays (GL_LINE_LOOP, 0, points .size ());

		glDisableClientState (GL_VERTEX_ARRAY);
		glEnable (GL_DEPTH_TEST);
	}
	catch (const std::domain_error &)
	{
		// unProjectPoint is not posible
	}
}

LassoSelection::~LassoSelection ()
{
	__LOG__ << std::endl;

	getBrowser () -> isSensitive (sensitive);
}

} // X3D
} // titania
