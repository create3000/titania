/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#include "LightSaber.h"

#include "../../Components/Layering/X3DLayerNode.h"
#include "../../Rendering/OpenGL.h"
#include "../ContextLock.h"

#include <Titania/Math/Geometry/Camera.h>

namespace titania {
namespace X3D {

const ComponentType LightSaber::component      = ComponentType::TITANIA;
const std::string   LightSaber::typeName       = "LightSaber";
const std::string   LightSaber::containerField = "viewer";

LightSaber::LightSaber (X3DExecutionContext* const executionContext) :
	     X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DExamineViewer (),
	          points (2)
{
	addType (X3DConstants::LightSaber);
}

LightSaber*
LightSaber::create (X3DExecutionContext* const executionContext) const
{
	return new LightSaber (executionContext);
}

void
LightSaber::initialize ()
{
	X3DExamineViewer::initialize ();
}

bool
LightSaber::on_1button1_press_event (GdkEventButton* event)
{
	getBrowser () -> addEvent ();
	getBrowser () -> displayed () .addInterest (this, &LightSaber::display);

	points [0] = points [1] = getPoint (event -> x, event -> y);
	return false;
}

bool
LightSaber::on_1button1_release_event (GdkEventButton* event)
{
	try
	{
		getBrowser () -> addEvent ();
		getBrowser () -> displayed () .removeInterest (this, &LightSaber::display);

		if (points [0] not_eq points [1])
		{
			const auto selectionType = getBrowser () -> getSelectionType ();
			getBrowser () -> setSelectionType (SelectionType::CUT);
			getBrowser () -> setCutLine (Line2d (points [0], points [1], points_type ()));
			getBrowser () -> touch (points [0] .x (), points [0] .y ());
			getBrowser () -> setSelectionType (selectionType);
		}

		return false;
	}
	catch (const X3DError & error)
	{
		return false;
	}
}

bool
LightSaber::on_motion1_notify_event (GdkEventMotion* event)
{
	getBrowser () -> addEvent ();

	points [1] = getPoint (event -> x, event -> y);

	if (getBrowser () -> getControlKey () and not getBrowser () -> getShiftKey ())
	{
		const auto direction = points [1] - points [0];

		if (std::abs (direction .x ()) < std::abs (direction .y ()))
			points [1] .x (points [0] .x ());
		else
			points [1] .y (points [0] .y ());
	}

	return false;
}

void
LightSaber::display ()
{
	// Configure HUD

	const auto & viewport = getBrowser () -> getRectangle ();
	const int    width    = viewport [2];
	const int    height   = viewport [3];

	const Matrix4d projection = ortho <float> (0, width, 0, height, -1, 1);

	glMatrixMode (GL_PROJECTION);
	glLoadMatrixd (projection .data ());
	glMatrixMode (GL_MODELVIEW);

	// Draw a black and a white line.

	glDisable (GL_DEPTH_TEST);
	glLoadIdentity ();

	glEnableClientState (GL_VERTEX_ARRAY);
	glVertexPointer (2, GL_DOUBLE, 0, points .data ());

	glLineWidth (2);
	glColor3f (0, 0, 0);
	glDrawArrays (GL_LINES, 0, points .size ());

	glLineWidth (1);
	glColor3f (1, 1, 1);
	glDrawArrays (GL_LINES, 0, points .size ());

	glDisableClientState (GL_VERTEX_ARRAY);
	glEnable (GL_DEPTH_TEST);
}

Vector2d
LightSaber::getPoint (const double x, const double y) const
{
	const auto & viewport = getBrowser () -> getRectangle ();
	const double height   = viewport [3];

	return Vector2d (x, height - y);
}

void
LightSaber::dispose ()
{
	X3DExamineViewer::dispose ();
}

LightSaber::~LightSaber ()
{ }

} // X3D
} // titania
