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

#include "X3DBackgroundNode.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/BindableNodeList.h"
#include "../../Execution/BindableNodeStack.h"
#include "../Layering/X3DLayerNode.h"
#include "../Navigation/X3DViewpointNode.h"

#include <complex>

namespace titania {
namespace X3D {

static constexpr float RADIUS = 1;

//static constexpr float U_DIMENSION = 16;// LOW
//static constexpr float U_DIMENSION = 24;// HIGH
static constexpr float U_DIMENSION = 20; // MEDIUM

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
	        frontTexture (),
	         backTexture (),
	         leftTexture (),
	        rightTexture (),
	          topTexture (),
	       bottomTexture (),
	              hidden (false),
	transformationMatrix (),
	            glColors (),
	            glPoints (),
	          numIndices ()
{
	addType (X3DConstants::X3DBackgroundNode);

	addChildObjects (frontTexture,
	                 backTexture,
	                 leftTexture,
	                 rightTexture,
	                 topTexture,
	                 bottomTexture);
	
	skyAngle ()    .setUnit (UnitCategory::ANGLE);
	groundAngle () .setUnit (UnitCategory::ANGLE);
}

void
X3DBackgroundNode::initialize ()
{
	X3DBindableNode::initialize ();

	addInterest (&X3DBackgroundNode::build, this);

	build ();
}

void
X3DBackgroundNode::isHidden (const bool value)
{
	hidden = value;

	getBrowser () -> addEvent ();
}

bool
X3DBackgroundNode::isTransparent () const
{
	if (hidden)
		return true;

	if (transparency () == 0.0f)
		return false;

	if (not frontTexture  or frontTexture  -> isTransparent ())
			return true;

	if (not backTexture   or backTexture   -> isTransparent ())
			return true;

	if (not leftTexture   or leftTexture   -> isTransparent ())
			return true;

	if (not rightTexture  or rightTexture  -> isTransparent ())
			return true;

	if (not topTexture    or topTexture    -> isTransparent ())
			return true;

	if (not bottomTexture or bottomTexture -> isTransparent ())
			return true;

	return false;
}

void
X3DBackgroundNode::bindToLayer (X3DLayerNode* const layer)
{
	layer -> getBackgroundStack () -> pushOnTop (this);
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
X3DBackgroundNode::getColor (const float theta, const MFColor & color, const MFFloat & angle)
{
	const auto iter  = std::upper_bound (angle .cbegin (), angle .cend (), theta);
	const auto index = iter - angle .cbegin ();

	return color [index];
}

void
X3DBackgroundNode::buildHalfSphere (const float radius, const std::vector <float> & vAngle, const MFFloat & angle, const MFColor & color, const float opacity, const bool bottom)
{
	// p1 --- p4
	//  |     |
	//  |     |
	// p2 --- p3

	float phi = 0;

	std::complex <float> y;
	Vector3f             p;
	
	const auto    vAngleMax   = bottom ? pi_2 <float> : pi <float>;
	const int32_t V_DIMENSION = vAngle .size () - 1;

	numIndices += 4 * U_DIMENSION * V_DIMENSION;

	for (int32_t v = 0; v < V_DIMENSION; ++ v)
	{
		float theta1 = math::clamp <float> (vAngle [v],     0, vAngleMax);
		float theta2 = math::clamp <float> (vAngle [v + 1], 0, vAngleMax);

		if (bottom)
		{
			theta1 = pi <float> - theta1;
			theta2 = pi <float> - theta2;
		}

		const auto z1 = std::polar (radius, theta1);
		const auto z2 = std::polar (radius, theta2);

		const Color3f c1 = getColor (vAngle [v],     color, angle);
		const Color3f c2 = getColor (vAngle [v + 1], color, angle);

		for (size_t u = 0; u < U_DIMENSION; ++ u)
		{
			// The last point is the first one.
			const size_t u1 = u < U_DIMENSION - 1 ? u + 1 : 0;

			// p1
			phi = pi2 <float> * (u / U_DIMENSION);
			y   = std::polar (-z1 .imag (), phi);

			glColors .emplace_back (c1 .r (), c1 .g (), c1 .b (), opacity);
			glPoints .emplace_back (y .imag (), z1 .real (), y .real ());

			// p2
			y = std::polar (-z2 .imag (), phi);

			glColors .emplace_back (c2 .r (), c2 .g (), c2 .b (), opacity);
			glPoints .emplace_back (y .imag (), z2 .real (), y .real ());

			// p3
			phi = pi2 <float> * (u1 / U_DIMENSION);
			y   = std::polar (-z2 .imag (), phi);

			glColors .emplace_back (c2 .r (), c2 .g (), c2 .b (), opacity);
			glPoints .emplace_back (y .imag (), z2 .real (), y .real ());

			// p4
			y = std::polar (-z1 .imag (), phi);

			glColors .emplace_back (c1 .r (), c1 .g (), c1 .b (), opacity);
			glPoints .emplace_back (y .imag (), z1 .real (), y .real ());
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

	const float opacity = 1 - math::clamp <float> (transparency (), 0, 1);

	if (groundColor () .empty () and skyColor () .size () == 1)
	{
		// Build cube

		const float     s = std::sqrt (RADIUS * RADIUS / 2);
		const Color3f & c = skyColor () [0];

		numIndices = 24;

		for (int32_t i = 0; i < numIndices; ++ i)
			glColors .emplace_back (c .r (), c .g (), c .b (), opacity);

		// Back
		glPoints .emplace_back (-s,  s, -s);
		glPoints .emplace_back ( s,  s, -s);
		glPoints .emplace_back ( s, -s, -s);
		glPoints .emplace_back (-s, -s, -s);

		// Front
		glPoints .emplace_back ( s,  s,  s);
		glPoints .emplace_back (-s,  s,  s);
		glPoints .emplace_back (-s, -s,  s);
		glPoints .emplace_back ( s, -s,  s);

		// Left
		glPoints .emplace_back (-s,  s,  s);
		glPoints .emplace_back (-s,  s, -s);
		glPoints .emplace_back (-s, -s, -s);
		glPoints .emplace_back (-s, -s,  s);

		// Right
		glPoints .emplace_back (s,  s, -s);
		glPoints .emplace_back (s,  s,  s);
		glPoints .emplace_back (s, -s,  s);
		glPoints .emplace_back (s, -s, -s);

		// Top
		glPoints .emplace_back (-s,  s,  s);
		glPoints .emplace_back ( s,  s,  s);
		glPoints .emplace_back ( s,  s, -s);
		glPoints .emplace_back (-s,  s, -s);

		// Bottom
		glPoints .emplace_back ( s, -s,  s);
		glPoints .emplace_back (-s, -s,  s);
		glPoints .emplace_back (-s, -s, -s);
		glPoints .emplace_back ( s, -s, -s);
	}
	else
	{
		// Build sphere

		if (skyColor () .size () > skyAngle () .size ())
		{
			std::vector <float> vAngle (skyAngle () .begin (), skyAngle () .end ());

			if (vAngle .empty () or vAngle .front () > 0)
				vAngle .insert (vAngle .begin (), 0);

			const auto vAngleMax = groundColor () .size () > groundAngle () .size () ? pi_2 <float> : pi <float>;

			if (vAngle .back () < vAngleMax)
				vAngle .emplace_back (vAngleMax);

			buildHalfSphere (RADIUS, vAngle, skyAngle (), skyColor (), opacity, false);
		}

		if (groundColor () .size () > groundAngle () .size ())
		{
			std::vector <float> vAngle (groundAngle () .rbegin (), groundAngle () .rend ());

			if (vAngle .empty () or vAngle .front () < pi_2 <float>)
				vAngle .insert (vAngle .begin (), pi_2 <float>);

			if (vAngle .back () > 0)
				vAngle .emplace_back (0);

			buildHalfSphere (RADIUS, vAngle, groundAngle (), groundColor (), opacity, true);
		}
	}
}

void
X3DBackgroundNode::traverse (const TraverseType type, X3DRenderObject* const renderObject)
{
	switch (type)
	{
		case TraverseType::CAMERA:
		{
			renderObject -> getLayer () -> getBackgrounds () -> push_back (this);
			
			transformationMatrix = renderObject -> getModelViewMatrix () .get ();
			break;
		}
		case TraverseType::DISPLAY:
		{
			break;
		}
		default:
			break;
	}
}

void
X3DBackgroundNode::draw (X3DRenderObject* const renderObject, const Vector4i & viewport)
{
	try
	{
__LOG__ << std::endl;

		if (getBrowser () -> getAlphaChannel () .top ())
			return;

		if (hidden)
			return;

		PolygonModeLock polygonMode (GL_FILL);

		// Get background scale.

		const auto farValue        = -ViewVolume::unProjectPoint (0, 0, 0.99999, inverse (renderObject -> getProjectionMatrix () .get ()), viewport) .z ();
		auto       modelViewMatrix = transformationMatrix;

		// Rotate and scale background.

		Vector3d   translation;
		Rotation4d rotation;

		modelViewMatrix .mult_right (renderObject -> getInverseCameraSpaceMatrix () .get ());
		modelViewMatrix .get (translation, rotation);
		modelViewMatrix .set (Vector3d (), rotation, Vector3d (farValue, farValue, farValue));

		glLoadMatrixd (modelViewMatrix .data ());

		// Draw background sphere and texture cube.

		drawSphere (renderObject);
		drawCube   (renderObject);
	}
	catch (const std::domain_error &)
	{ }
}

void
X3DBackgroundNode::drawSphere (X3DRenderObject* const renderObject)
{
	if (transparency () >= 1.0f)

	// Draw

	glDisable (GL_DEPTH_TEST);
	glDepthMask (GL_FALSE);

	if (transparency ())
		glEnable (GL_BLEND);
	else
		glDisable (GL_BLEND);

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
X3DBackgroundNode::drawCube (X3DRenderObject* const renderObject)
{
	static const auto s = std::sqrt (RADIUS * RADIUS / 2);

	glDisable (GL_DEPTH_TEST);
	glDepthMask (GL_FALSE);

	glMatrixMode (GL_TEXTURE);
	glLoadIdentity ();
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

			frontTexture -> draw (renderObject);
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

			backTexture -> draw (renderObject);
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

			leftTexture -> draw (renderObject);
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

			rightTexture -> draw (renderObject);
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

			topTexture -> draw (renderObject);
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

			bottomTexture -> draw (renderObject);
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
