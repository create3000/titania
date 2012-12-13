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

#include "X3DBackgroundNode.h"

#include "../../Browser/Browser.h"
#include "../Layering/X3DLayerNode.h"
#include "../Navigation/X3DViewpointNode.h"
#include "../../Rendering/OpenGL.h"

#define SPHERE_USEG 16.0
#define SPHERE_VSEG 15.0

namespace titania {
namespace X3D {

X3DBackgroundNode::X3DBackgroundNode (bool addToList) :
	X3DBindableNode (),               
	    groundAngle (),               // MFFloat [in,out] groundAngle   [ ]           [0,π/2]
	    groundColor (),               // MFColor [in,out] groundColor   [ ]           [0,1]
	       skyAngle (),               // MFFloat [in,out] skyAngle      [ ]           [0,π]
	       skyColor ({ SFColor () }), // MFColor [in,out] skyColor      0 0 0         [0,1]
	   transparency (),               // SFFloat [in,out] transparency  0             [0,1]
	      addToList (addToList)       
{
	addNodeType (X3DConstants::X3DBackgroundNode);
}

void
X3DBackgroundNode::initialize ()
{
	X3DBindableNode::initialize ();

	if (addToList)
		getScene () -> addBackground (this);

	build ();
}

void
X3DBackgroundNode::bindToLayer (X3DLayerNode* const layer)
{
	layer -> backgroundStack .push (this);
}

void
X3DBackgroundNode::removeFromLayer (X3DLayerNode* const layer)
{
	layer -> backgroundStack .pop (this);
}

Color3f
X3DBackgroundNode::getColor (float theta, const MFColor & color, const MFFloat & angle)
{
	size_t i;

	for (i = 0; i < angle .size (); i ++)
	{
		if (theta <= angle .at (i))
		{
			break;
		}
	}

	if (i == angle .size ())
		return color .at (i);

	float weighting;

	if (i == 0)
		weighting = theta / angle .at (0);
	else
		weighting = (theta - angle .at (i - 1)) / (angle .at (i) - angle .at (i - 1));

	Color3f c1 = color .at (i);
	Color3f c2 = color .at (i + 1);

	return Color3f (c1 .r () + (c2 .r () - c1 .r ()) * weighting,
	                c1 .g () + (c2 .g () - c1 .g ()) * weighting,
	                c1 .b () + (c2 .b () - c1 .b ()) * weighting);
}

void
X3DBackgroundNode::build ()
{
	float radius  = 10000;
	float opacity = 1 - math::clamp <float> (transparency, 0, 1);
	float PI2     = 2 * M_PI;

	glColors .clear ();
	glPoints .clear ();
	numIndices = 0;

	// p2 --- p1
	//  |     |
	//  |     |
	// p3 --- p4

	if (skyColor .size () > skyAngle .size ())
	{
		float vmax = groundColor .size () > groundAngle .size () ? (SPHERE_VSEG / 2) - 1 : SPHERE_VSEG - 1;

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
				c = getColor (theta, skyColor, skyAngle);

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
				c = getColor (theta, skyColor, skyAngle);

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

	if (groundColor .size () > groundAngle .size ())
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
				c = getColor (M_PI - theta, groundColor, groundAngle);

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
				c = getColor (M_PI - theta, groundColor, groundAngle);

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
X3DBackgroundNode::display ()
{
	glGetDoublev (GL_MODELVIEW_MATRIX, matrix .data ());
}

void
X3DBackgroundNode::draw ()
{
	GLint polygonMode [2] = { 0, 0 }; // Front and back value.
	glGetIntegerv (GL_POLYGON_MODE, polygonMode);

	glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
	{
		Vector3d   translation, scale;
		Rotation4f rotation;

		matrix .get (translation, rotation, scale);

		float x, y, z, angle;
		rotation .get (x, y, z, angle);

		//

		getCurrentLayer () -> getViewpoint () -> reshape (1, 20000);

		//

		glDisable (GL_DEPTH_TEST);
		glDepthMask (GL_FALSE);

		if (transparency)
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

void
X3DBackgroundNode::dispose ()
{
	getScene () -> removeBackground (this);

	X3DBindableNode::dispose ();
}

} // X3D
} // titania
