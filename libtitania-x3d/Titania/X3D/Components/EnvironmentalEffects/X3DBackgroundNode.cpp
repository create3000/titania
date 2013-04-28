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

#include "X3DBackgroundNode.h"

#include "../../Browser/X3DBrowser.h"
#include "../Layering/X3DLayerNode.h"
#include "../Navigation/X3DViewpointNode.h"

// LOW
//#define SPHERE_USEG 16.0
//#define SPHERE_VSEG 15.0

// HIGH
//#define SPHERE_USEG 32.0
//#define SPHERE_VSEG 41.0

// MEDIUM
#define SPHERE_USEG 22.0
#define SPHERE_VSEG 31.0

namespace titania {
namespace X3D {

X3DBackgroundNode::Fields::Fields () :
	groundAngle (new MFFloat ()),
	groundColor (new MFColor ()),
	skyAngle (new MFFloat ()),
	skyColor (new MFColor ({ SFColor () })),
	transparency (new SFFloat ())
{ }

X3DBackgroundNode::X3DBackgroundNode (bool displayed) :
	X3DBindableNode (displayed), 
	         fields ()           
{
	addNodeType (X3DConstants::X3DBackgroundNode);
}

void
X3DBackgroundNode::initialize ()
{
	X3DBindableNode::initialize ();

	build ();
}

void
X3DBackgroundNode::addToLayer (X3DLayerNode* const layer)
{
	layer -> getBackgrounds () .push_back (this);
}

void
X3DBackgroundNode::removeFromLayer (X3DLayerNode* const layer)
{
	layer -> getBackgrounds () .erase (this);
}

void
X3DBackgroundNode::bindToLayer (X3DLayerNode* const layer)
{
	layer -> getBackgroundStack () .push (this);
}

void
X3DBackgroundNode::unbindFromLayer (X3DLayerNode* const layer)
{
	layer -> getBackgroundStack () .pop (this);
}

Color3f
X3DBackgroundNode::getColor (float theta, const MFColor & color, const MFFloat & angle)
{
	size_t i;

	for (i = 0; i < angle .size (); i ++)
	{
		if (theta <= angle [i])
		{
			break;
		}
	}

	if (i == angle .size ())
		return color [i];

	float weight;

	if (i == 0)
		weight = theta / angle [0];
	else
		weight = (theta - angle [i - 1]) / (angle [i] - angle [i - 1]);

	return math::clerp <float> (color [i], color [i + 1], weight);
}

void
X3DBackgroundNode::build ()
{
	glColors .clear ();
	glPoints .clear ();
	numIndices = 0;

	if (transparency () == 1.0f)
		return;

	float radius  = 10000;
	float opacity = 1 - math::clamp <float> (transparency (), 0, 1);
	float PI2     = 2 * M_PI;

	// p2 --- p1
	//  |     |
	//  |     |
	// p3 --- p4

	if (skyColor () .size () > skyAngle () .size ())
	{
		float vmax = groundColor () .size () > groundAngle () .size () ? (SPHERE_VSEG / 2) - 1 : SPHERE_VSEG - 1;

		for (int v = 0; v < vmax; ++ v)
		{
			for (int u = 0; u < SPHERE_USEG - 1; ++ u)
			{
				float    y, r, x, z, theta, phi;
				Vector3f p;
				Color3f  c;

				// p1
				theta = M_PI * (v / (SPHERE_VSEG - 1));
				phi   = PI2 * ((u + 1) / (SPHERE_USEG - 1));
				y     = cos (theta);
				r     = sin (theta);
				x     = -sin (phi) * r;
				z     = -cos (phi) * r;

				p = Vector3f (x, y, z) * radius;
				c = getColor (theta, skyColor (), skyAngle ());

				glColors .push_back (c .r ());
				glColors .push_back (c .g ());
				glColors .push_back (c .b ());
				glColors .push_back (opacity);

				glPoints .push_back (p .x ());
				glPoints .push_back (p .y ());
				glPoints .push_back (p .z ());

				++ numIndices;

				// p2
				theta = M_PI * (v / (SPHERE_VSEG - 1));
				phi   = PI2 * (u / (SPHERE_USEG - 1));
				y     = cos (theta);
				r     = sin (theta);
				x     = -sin (phi) * r;
				z     = -cos (phi) * r;

				p = Vector3f (x, y, z) * radius;

				glColors .push_back (c .r ());
				glColors .push_back (c .g ());
				glColors .push_back (c .b ());
				glColors .push_back (opacity);

				glPoints .push_back (p .x ());
				glPoints .push_back (p .y ());
				glPoints .push_back (p .z ());

				++ numIndices;

				// p3
				theta = M_PI * ((v + 1) / (SPHERE_VSEG - 1));
				phi   = PI2 * (u / (SPHERE_USEG - 1));
				y     = cos (theta);
				r     = sin (theta);
				x     = -sin (phi) * r;
				z     = -cos (phi) * r;

				p = Vector3f (x, y, z) * radius;
				c = getColor (theta, skyColor (), skyAngle ());

				glColors .push_back (c .r ());
				glColors .push_back (c .g ());
				glColors .push_back (c .b ());
				glColors .push_back (opacity);

				glPoints .push_back (p .x ());
				glPoints .push_back (p .y ());
				glPoints .push_back (p .z ());

				++ numIndices;

				// p4
				theta = M_PI * ((v + 1) / (SPHERE_VSEG - 1));
				phi   = PI2 * ((u + 1) / (SPHERE_USEG - 1));
				y     = cos (theta);
				r     = sin (theta);
				x     = -sin (phi) * r;
				z     = -cos (phi) * r;

				p = Vector3f (x, y, z) * radius;

				glColors .push_back (c .r ());
				glColors .push_back (c .g ());
				glColors .push_back (c .b ());
				glColors .push_back (opacity);

				glPoints .push_back (p .x ());
				glPoints .push_back (p .y ());
				glPoints .push_back (p .z ());

				++ numIndices;
			}
		}
	}

	if (groundColor () .size () > groundAngle () .size ())
	{
		for (int v = SPHERE_VSEG / 2; v < SPHERE_VSEG - 1; ++ v)
		{
			for (int u = 0; u < SPHERE_USEG - 1; ++ u)
			{
				float    y, r, x, z, theta, phi;
				Vector3f p;
				Color3f  c;

				// p1
				theta = M_PI * (v / (SPHERE_VSEG - 1));
				phi   = PI2 * ((u + 1) / (SPHERE_USEG - 1));
				y     = cos (theta);
				r     = sin (theta);
				x     = -sin (phi) * r;
				z     = -cos (phi) * r;

				p = Vector3f (x, y, z) * radius;
				c = getColor (M_PI - theta, groundColor (), groundAngle ());

				glColors .push_back (c .r ());
				glColors .push_back (c .g ());
				glColors .push_back (c .b ());
				glColors .push_back (opacity);

				glPoints .push_back (p .x ());
				glPoints .push_back (p .y ());
				glPoints .push_back (p .z ());

				++ numIndices;

				// p2
				theta = M_PI * (v / (SPHERE_VSEG - 1));
				phi   = PI2 * (u / (SPHERE_USEG - 1));
				y     = cos (theta);
				r     = sin (theta);
				x     = -sin (phi) * r;
				z     = -cos (phi) * r;

				p = Vector3f (x, y, z) * radius;

				glColors .push_back (c .r ());
				glColors .push_back (c .g ());
				glColors .push_back (c .b ());
				glColors .push_back (opacity);

				glPoints .push_back (p .x ());
				glPoints .push_back (p .y ());
				glPoints .push_back (p .z ());

				++ numIndices;

				// p3
				theta = M_PI * ((v + 1) / (SPHERE_VSEG - 1));
				phi   = PI2 * (u / (SPHERE_USEG - 1));
				y     = cos (theta);
				r     = sin (theta);
				x     = -sin (phi) * r;
				z     = -cos (phi) * r;

				p = Vector3f (x, y, z) * radius;
				c = getColor (M_PI - theta, groundColor (), groundAngle ());

				glColors .push_back (c .r ());
				glColors .push_back (c .g ());
				glColors .push_back (c .b ());
				glColors .push_back (opacity);

				glPoints .push_back (p .x ());
				glPoints .push_back (p .y ());
				glPoints .push_back (p .z ());

				++ numIndices;

				// p4
				theta = M_PI * ((v + 1) / (SPHERE_VSEG - 1));
				phi   = PI2 * ((u + 1) / (SPHERE_USEG - 1));
				y     = cos (theta);
				r     = sin (theta);
				x     = -sin (phi) * r;
				z     = -cos (phi) * r;

				p = Vector3f (x, y, z) * radius;

				glColors .push_back (c .r ());
				glColors .push_back (c .g ());
				glColors .push_back (c .b ());
				glColors .push_back (opacity);

				glPoints .push_back (p .x ());
				glPoints .push_back (p .y ());
				glPoints .push_back (p .z ());

				++ numIndices;
			}
		}
	}
}

void
X3DBackgroundNode::traverse (TraverseType type)
{
	if (type == TraverseType::COLLECT)
		matrix = ModelViewMatrix4f ();
}

void
X3DBackgroundNode::draw ()
{
	if (transparency () == 1.0f)
		return;

	//

	GLint polygonMode [2] = { 0, 0 }; // Front and back value.
	glGetIntegerv (GL_POLYGON_MODE, polygonMode);

	glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
	{
		Vector3d   translation;
		Rotation4f rotation;

		matrix .get (translation, rotation);

		float x, y, z, angle;
		rotation .get (x, y, z, angle);

		//

		getCurrentViewpoint () -> reshape (1, 20000);

		//

		glDisable (GL_DEPTH_TEST);
		glDepthMask (GL_FALSE);

		if (transparency ())
			glEnable (GL_BLEND);
		else
			glDisable (GL_BLEND);

		glEnable (GL_CULL_FACE);

		glFrontFace (GL_CW);

		glLoadIdentity ();
		glRotatef (math::degree (angle), x, y, z);

		glEnableClientState (GL_COLOR_ARRAY);
		glColorPointer (4, GL_FLOAT, 0, &glColors [0]);

		glEnableClientState (GL_VERTEX_ARRAY);
		glVertexPointer (3, GL_FLOAT, 0, &glPoints [0]);

		glDrawArrays (GL_QUADS, 0, numIndices);

		glDisableClientState (GL_COLOR_ARRAY);
		glDisableClientState (GL_VERTEX_ARRAY);
	}

	glPolygonMode (GL_FRONT_AND_BACK, polygonMode [0]);
}

} // X3D
} // titania
