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

#include "X3DBackgroundNode.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/BindableNodeList.h"
#include "../../Execution/BindableNodeStack.h"
#include "../Layering/X3DLayerNode.h"
#include "../Navigation/X3DViewpointNode.h"

namespace titania {
namespace X3D {

static constexpr float SIZE = 10000;

// LOW
//static constexpr int SPHERE_USEG = 16;

// HIGH
//static constexpr int SPHERE_USEG = 32;

// MEDIUM
static constexpr int SPHERE_USEG = 22;

X3DBackgroundNode::Fields::Fields () :
	groundAngle (new MFFloat ()),
	groundColor (new MFColor ()),
	skyAngle (new MFFloat ()),
	skyColor (new MFColor ({ SFColor () })),
	transparency (new SFFloat ())
{ }

X3DBackgroundNode::X3DBackgroundNode () :
	X3DBindableNode (),
	         fields (),
	     frontTexture (nullptr),
	      backTexture (nullptr),
	      leftTexture (nullptr),
	     rightTexture (nullptr),
	       topTexture (nullptr),
	    bottomTexture (nullptr)
{
	addNodeType (X3DConstants::X3DBackgroundNode);
}

void
X3DBackgroundNode::initialize ()
{
	X3DBindableNode::initialize ();
	
	addInterest (this, &X3DBackgroundNode::build);

	build ();
}

void
X3DBackgroundNode::bindToLayer (X3DLayerNode* const layer)
{
	layer -> getBackgroundStack () -> push (this);
}

void
X3DBackgroundNode::unbindFromLayer (X3DLayerNode* const layer)
{
	layer -> getBackgroundStack () -> pop (this);
}

void
X3DBackgroundNode::removeFromLayer (X3DLayerNode* const layer)
{
	layer -> getBackgroundStack () -> erase (this);
}

Color3f
X3DBackgroundNode::getColor (float theta, const MFColor & color, const MFFloat & angle)
{
	auto iter = std::upper_bound (angle .cbegin (), angle .cend (), theta);

	size_t i = iter - angle .cbegin ();

	return color [i];
}

void
X3DBackgroundNode::build (float radius, const MFFloat & vangle, const MFFloat & angle, const MFColor & color, float opacity, bool bottom)
{
	// p1 --- p4
	//  |     |
	//  |     |
	// p2 --- p3

	float useg1 = SPHERE_USEG - 1;

	for (size_t v = 0, size = vangle .size () - 1; v < size; ++ v)
	{
		float theta  = math::clamp <float> (vangle [v], 0, M_PI);
		float theta1 = math::clamp <float> (vangle [v + 1], 0, M_PI);

		if (bottom)
		{
			theta  = M_PI - theta;
			theta1 = M_PI - theta1;
		}

		float y  = cos (theta);
		float y1 = cos (theta1);

		float r  = sin (theta);
		float r1 = sin (theta1);

		Color3f c  = getColor (vangle [v],     color, angle);
		Color3f c1 = getColor (vangle [v + 1], color, angle);

		for (size_t u = 0; u < useg1; ++ u)
		{
			// The last point is the first one.
			size_t u1 = u < useg1 - 1 ? u + 1 : 0;

			float    x, z, phi;
			Vector3f p;

			// p1
			phi = M_PI2 * (u / useg1);
			x   = -sin (phi) * r;
			z   = -cos (phi) * r;
			p   = Vector3f (x, y, z) * radius;

			glColors .emplace_back (c .r (), c .g (), c .b (), opacity);
			glPoints .emplace_back (p);

			++ numIndices;

			// p2
			x = -sin (phi) * r1;
			z = -cos (phi) * r1;
			p = Vector3f (x, y1, z) * radius;

			glColors .emplace_back (c1 .r (), c1 .g (), c1 .b (), opacity);
			glPoints .emplace_back (p);

			++ numIndices;

			// p3
			phi = M_PI2 * (u1 / useg1);
			x   = -sin (phi) * r1;
			z   = -cos (phi) * r1;
			p   = Vector3f (x, y1, z) * radius;

			glColors .emplace_back (c1 .r (), c1 .g (), c1 .b (), opacity);
			glPoints .emplace_back (p);

			++ numIndices;

			// p4
			x = -sin (phi) * r;
			z = -cos (phi) * r;
			p = Vector3f (x, y, z) * radius;

			glColors .emplace_back (c .r (), c .g (), c .b (), opacity);
			glPoints .emplace_back (p);

			++ numIndices;
		}
	}
}

void
X3DBackgroundNode::build ()
{
	glColors .clear ();
	glPoints .clear ();
	numIndices = 0;

	if (transparency () >= 1.0f)
		return;

	float opacity = 1 - math::clamp <float> (transparency (), 0, 1);

	if (groundColor () .empty () and skyColor () .size () == 1)
	{
		// Build cube

		float r = SIZE;

		const Color3f & c = skyColor () [0];

		numIndices = 24;

		for (int i = 0; i < numIndices; ++ i)
			glColors .emplace_back (c .r (), c .g (), c .b (), opacity);

		// Back
		glPoints .emplace_back (-r,  r, -r);
		glPoints .emplace_back (r,  r, -r);
		glPoints .emplace_back (r, -r, -r);
		glPoints .emplace_back (-r, -r, -r);

		// Front
		glPoints .emplace_back (r,  r,  r);
		glPoints .emplace_back (-r,  r,  r);
		glPoints .emplace_back (-r, -r,  r);
		glPoints .emplace_back (r, -r,  r);

		// Left
		glPoints .emplace_back (-r,  r,  r);
		glPoints .emplace_back (-r,  r, -r);
		glPoints .emplace_back (-r, -r, -r);
		glPoints .emplace_back (-r, -r,  r);

		// Right
		glPoints .emplace_back (r,  r, -r);
		glPoints .emplace_back (r,  r,  r);
		glPoints .emplace_back (r, -r,  r);
		glPoints .emplace_back (r, -r, -r);

		// Top
		glPoints .emplace_back (-r,  r,  r);
		glPoints .emplace_back (r,  r,  r);
		glPoints .emplace_back (r,  r, -r);
		glPoints .emplace_back (-r,  r, -r);

		// Bottom
		glPoints .emplace_back (r, -r,  r);
		glPoints .emplace_back (-r, -r,  r);
		glPoints .emplace_back (-r, -r, -r);
		glPoints .emplace_back (r, -r, -r);
	}
	else
	{
		// Build sphere

		float radius = std::sqrt (2 * std::pow (SIZE, 2));

		if (skyColor () .size () > skyAngle () .size ())
		{
			MFFloat vangle = skyAngle ();

			if (vangle .empty () or vangle .front () > 0)
				vangle .emplace_front (0);

			if (vangle .back () < M_PI1_2)
				vangle .emplace_back (M_PI1_2);

			if (groundColor () .size () <= groundAngle () .size () and vangle .back () < M_PI)
				vangle .emplace_back (M_PI);

			build (radius, vangle, skyAngle (), skyColor (), opacity, false);
		}

		if (groundColor () .size () > groundAngle () .size ())
		{
			MFFloat vangle;
			vangle .assign (groundAngle () .rbegin (), groundAngle () .rend ());

			if (vangle .empty () or vangle .front () < M_PI1_2)
				vangle .emplace_front (M_PI1_2);

			if (vangle .back () > 0)
				vangle .emplace_back (0);

			build (radius, vangle, groundAngle (), groundColor (), opacity, true);
		}
	}
}

void
X3DBackgroundNode::traverse (const TraverseType type)
{
	switch (type)
	{
		case TraverseType::CAMERA:
		{
			getCurrentLayer () -> getBackgrounds () -> push_back (this);
			break;
		}
		case TraverseType::COLLECT:
		{
			matrix = ModelViewMatrix4f ();
			break;
		}
		default:
			break;
	}
}

void
X3DBackgroundNode::draw ()
{
	PolygonMode polygonMode (GL_FILL);

	// Scale background

	auto viewport = Viewport4i ();
	auto scale    = getCurrentViewpoint () -> getScreenScale (SIZE, viewport);
	
	scale *= double (viewport [2] > viewport [3] ? viewport [2] : viewport [3]);

	getCurrentViewpoint () -> reshape (1, std::max (2.0, 2 * SIZE * scale .z ()));

	glScalef (scale .x (), scale .y (), scale .z ());
	
	// Draw
	
	drawSphere ();
	drawCube ();
}

void
X3DBackgroundNode::drawSphere ()
{
	if (transparency () >= 1.0f)
		return;

	// Rotate background

	Vector3d   translation;
	Rotation4f rotation;
	float      x, y, z, angle;

	matrix .get (translation, rotation);
	rotation .get (x, y, z, angle);

	glRotatef (math::degrees (angle), x, y, z);

	// Draw

	glDisable (GL_DEPTH_TEST);
	glDepthMask (GL_FALSE);

	if (transparency ())
		glEnable (GL_BLEND);

	glEnable (GL_CULL_FACE);

	glFrontFace (GL_CW);

	glEnableClientState (GL_COLOR_ARRAY);
	glColorPointer (4, GL_FLOAT, 0, glColors .data ());

	glEnableClientState (GL_VERTEX_ARRAY);
	glVertexPointer (3, GL_FLOAT, 0, glPoints .data ());

	glDrawArrays (GL_QUADS, 0, numIndices);

	glDisableClientState (GL_COLOR_ARRAY);
	glDisableClientState (GL_VERTEX_ARRAY);

	glDisable (GL_BLEND);
	glDepthMask (GL_TRUE);
	glEnable (GL_DEPTH_TEST);
}

void
X3DBackgroundNode::drawCube ()
{
	float radius = 10000;
	float s      = std::sqrt (std::pow (2 * radius, 2) / 2) / 2;

	glDisable (GL_DEPTH_TEST);
	glDepthMask (GL_FALSE);

	glMatrixMode (GL_TEXTURE);
	glLoadIdentity ();
	glTranslatef (0, 1, 0);
	glScalef (1, -1, 1);
	glMatrixMode (GL_MODELVIEW);

	glFrontFace (GL_CCW);
	glColor4f (1, 1, 1, 1);

	if (frontTexture)
	{
		if (frontTexture -> checkLoadState () == COMPLETE_STATE)
		{
			if (frontTexture -> isTransparent ())
				glEnable (GL_BLEND);
			else
				glDisable (GL_BLEND);

			frontTexture -> draw ();
			glBegin (GL_QUADS);
			glTexCoord2f (1, 1);
			glVertex3f (s, s, -s);
			glTexCoord2f (0, 1);
			glVertex3f (-s, s, -s);
			glTexCoord2f (0, 0);
			glVertex3f (-s, -s, -s);
			glTexCoord2f (1, 0);
			glVertex3f (s, -s, -s);
			glEnd ();
		}
	}

	if (backTexture)
	{
		if (backTexture -> checkLoadState () == COMPLETE_STATE)
		{
			if (frontTexture -> isTransparent ())
				glEnable (GL_BLEND);
			else
				glDisable (GL_BLEND);

			backTexture -> draw ();
			glBegin (GL_QUADS);
			glTexCoord2f (0, 0);
			glVertex3f (s, -s, s);
			glTexCoord2f (1, 0);
			glVertex3f (-s, -s, s);
			glTexCoord2f (1, 1);
			glVertex3f (-s, s, s);
			glTexCoord2f (0, 1);
			glVertex3f (s, s, s);
			glEnd ();
		}
	}

	if (leftTexture)
	{
		if (leftTexture -> checkLoadState () == COMPLETE_STATE)
		{
			if (frontTexture -> isTransparent ())
				glEnable (GL_BLEND);
			else
				glDisable (GL_BLEND);

			leftTexture -> draw ();
			glBegin (GL_QUADS);
			glTexCoord2f (0, 1);
			glVertex3f (-s, s, s);
			glTexCoord2f (0, 0);
			glVertex3f (-s, -s, s);
			glTexCoord2f (1, 0);
			glVertex3f (-s, -s, -s);
			glTexCoord2f (1, 1);
			glVertex3f (-s, s, -s);
			glEnd ();
		}
	}

	if (rightTexture)
	{
		if (rightTexture -> checkLoadState () == COMPLETE_STATE)
		{
			if (frontTexture -> isTransparent ())
				glEnable (GL_BLEND);
			else
				glDisable (GL_BLEND);

			rightTexture -> draw ();
			glBegin (GL_QUADS);
			glTexCoord2f (0, 1);
			glVertex3f (s, s, -s);
			glTexCoord2f (0, 0);
			glVertex3f (s, -s, -s);
			glTexCoord2f (1, 0);
			glVertex3f (s, -s, s);
			glTexCoord2f (1, 1);
			glVertex3f (s, s, s);
			glEnd ();
		}
	}

	if (topTexture)
	{
		if (topTexture -> checkLoadState () == COMPLETE_STATE)
		{
			if (frontTexture -> isTransparent ())
				glEnable (GL_BLEND);
			else
				glDisable (GL_BLEND);

			topTexture -> draw ();
			glBegin (GL_QUADS);
			glTexCoord2f (0, 1);
			glVertex3f (-s, s, s);
			glTexCoord2f (0, 0);
			glVertex3f (-s, s, -s);
			glTexCoord2f (1, 0);
			glVertex3f (s, s, -s);
			glTexCoord2f (1, 1);
			glVertex3f (s, s, s);
			glEnd ();
		}
	}

	if (bottomTexture)
	{
		if (bottomTexture -> checkLoadState () == COMPLETE_STATE)
		{
			if (frontTexture -> isTransparent ())
				glEnable (GL_BLEND);
			else
				glDisable (GL_BLEND);

			bottomTexture -> draw ();
			glBegin (GL_QUADS);
			glTexCoord2f (1, 0);
			glVertex3f (s, -s, s);
			glTexCoord2f (1, 1);
			glVertex3f (s, -s, -s);
			glTexCoord2f (0, 1);
			glVertex3f (-s, -s, -s);
			glTexCoord2f (0, 0);
			glVertex3f (-s, -s, s);
			glEnd ();
		}
	}

	glDisable (GL_BLEND);
	glDisable (GL_TEXTURE_2D);

	glDepthMask (GL_TRUE);
	glEnable (GL_DEPTH_TEST);
}

} // X3D
} // titania
