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
 ******************************************************************************/

#include "X3DRenderer.h"

#include "../Browser/Browser.h"
#include "../Components/Navigation/X3DViewpointNode.h"
#include <algorithm>

namespace titania {
namespace X3D {

X3DRenderer::X3DRenderer () :
	                 X3DNode (),  
	            traverseTime (0), 
	                drawTime (0), 
	                numNodes (0), 
	           numNodesDrawn (0), 
	          numOpaqueNodes (0), 
	     numTransparentNodes (0), 
	numTransparentNodesDrawn (0)  
{ }

void
X3DRenderer::addShape (X3DShapeNode* shape, const float distance)
{
	const LightContainerArray & localLights = getCurrentLayer () -> getLocalLights ();
	X3DFogObject*               fog         = getCurrentLayer () -> getFog ();

	if (shape -> isTransparent ())
	{
		if (numTransparentNodes < transparentShapes .size ())
			*transparentShapes [numTransparentNodes] = ShapeContainer (shape, distance, localLights, fog);
		else
			transparentShapes .push_back (new ShapeContainer (shape, distance, localLights, fog));

		++ numTransparentNodes;
	}
	else
	{
		if (numOpaqueNodes < shapes .size ())
			*shapes [numOpaqueNodes] = ShapeContainer (shape, distance, localLights, fog);
		else
			shapes .push_back (new ShapeContainer (shape, distance, localLights, fog));

		++ numOpaqueNodes;
	}
}

void
X3DRenderer::display ()
{
	numOpaqueNodes      = 0;
	numTransparentNodes = 0;

	getBrowser () -> pushRenderer (this);

	time_type t0 = getCurrentTime ();
	draw ();
	traverseTime = getCurrentTime () - t0;

	t0 = getCurrentTime ();
	redraw ();
	drawTime = getCurrentTime () - t0;

	getBrowser () -> popRenderer ();
}

void
X3DRenderer::redraw ()
{
	numNodes                 = numOpaqueNodes + numTransparentNodes;
	numNodesDrawn            = 0;
	numTransparentNodesDrawn = 0;

	X3DViewpointNode* currentViewpoint = getCurrentLayer () -> getViewpoint ();
	Matrix4d          correctionMatrix = ~currentViewpoint -> getMatrix () * currentViewpoint -> getCurrentMatrix ();

	getCurrentLayer () -> correctLights (correctionMatrix);

	// enable global lights

	const LightContainerArray & globalLights = getCurrentLayer () -> getGlobalLights ();

	if (globalLights .size ())
	{
		for (const auto & light : globalLights)
			light -> enable ();
	}

	glClear (GL_DEPTH_BUFFER_BIT);

	// render opaque objects first

	glEnable (GL_DEPTH_TEST);
	glDepthMask (GL_TRUE);
	glDisable (GL_BLEND);

	auto end = shapes .cbegin () + numOpaqueNodes;

	for (auto shape = shapes .cbegin (); shape not_eq end; ++ shape)
	{
		numNodesDrawn += (*shape) -> redraw (correctionMatrix);
	}

	// render transparent objects

	glDepthMask (GL_FALSE);
	glEnable (GL_BLEND);

	std::stable_sort (transparentShapes .begin (), transparentShapes .begin () + numTransparentNodes);

	end = transparentShapes .begin () + numTransparentNodes;

	for (auto shape = transparentShapes .begin (); shape not_eq end; ++ shape)
	{
		numTransparentNodesDrawn += (*shape) -> redraw (correctionMatrix);
	}

	glDepthMask (GL_TRUE);
	glDisable (GL_BLEND);

	// disable global lights

	if (globalLights .size ())
	{
		LightContainerArray::const_reverse_iterator light;

		for (light = globalLights .rbegin (); light not_eq globalLights .rend (); ++ light)
			(*light) -> disable ();
	}

	numNodesDrawn += numTransparentNodesDrawn;
}

void
X3DRenderer::clear ()
{
	for (const auto & shape : shapes)
		delete shape;

	shapes .clear ();

	for (const auto & shape : transparentShapes)
		delete shape;

	transparentShapes .clear ();
}

X3DRenderer::~X3DRenderer ()
{
	clear ();
}

} // X3D
} // titania
