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

#include "Extrusion.h"

#include "../../Execution/X3DExecutionContext.h"
#include <iostream>

namespace titania {
namespace X3D {

Extrusion::Extrusion (X3DExecutionContext* const executionContext) :
	    X3DBasicNode (executionContext -> getBrowser (), executionContext),                                    
	 X3DGeometryNode (),                                                                                       
	set_crossSection (),                                                                                       // MFVec2f    [in] set_crossSection
	 set_orientation (),                                                                                       // MFRotation [in] set_orientation
	       set_scale (),                                                                                       // MFVec2f    [in] set_scale
	       set_spine (),                                                                                       // MFVec3f    [in] set_spine
	        beginCap (true),                                                                                   // SFBool     [ ]  beginCap          TRUE
	          convex (true),                                                                                   // SFBool     [ ]  convex            TRUE
	    crossSection ({ SFVec2f (1, 1), SFVec2f (1, -1), SFVec2f (-1, -1), SFVec2f (-1, 1), SFVec2f (1, 1) }), // MFVec2f    [ ]  crossSection      [1 1 1 -1 -1 -1 -1 1 1 1]        (-∞,∞)
	          endCap (true),                                                                                   // SFBool     [ ]  endCap            TRUE
	     orientation ({ SFRotation () }),                                                                      // MFRotation [ ]  orientation       0 0 1 0                          [-1,1] or (-∞,∞)
	           scale ({ SFVec2f (1, 1) }),                                                                     // MFVec2f    [ ]  scale             1 1                              (0,∞)
	           spine ({ SFVec3f (), SFVec3f (0, 1, 0) })                                                       // MFVec3f    [ ]  spine             [0 0 0 0 1 0]                    (-∞,∞)
{
	setComponent ("Geometry3D");
	setTypeName ("Extrusion");

	appendField (inputOutput,    "metadata",         metadata);
	appendField (inputOnly,      "set_crossSection", set_crossSection);
	appendField (inputOnly,      "set_orientation",  set_orientation);
	appendField (inputOnly,      "set_scale",        set_scale);
	appendField (inputOnly,      "set_spine",        set_spine);
	appendField (initializeOnly, "beginCap",         beginCap);
	appendField (initializeOnly, "endCap",           endCap);
	appendField (initializeOnly, "ccw",              ccw);
	appendField (initializeOnly, "convex",           convex);
	appendField (initializeOnly, "creaseAngle",      creaseAngle);
	appendField (initializeOnly, "crossSection",     crossSection);
	appendField (initializeOnly, "orientation",      orientation);
	appendField (initializeOnly, "scale",            scale);
	appendField (initializeOnly, "spine",            spine);
	appendField (initializeOnly, "solid",            solid);
}

X3DBasicNode*
Extrusion::create (X3DExecutionContext* const executionContext) const
{
	return new Extrusion (executionContext);
}

void
Extrusion::initialize ()
{
	X3DGeometryNode::initialize ();

	tess = gluNewTess ();

	if (tess)
	{
		gluTessProperty (tess, GLU_TESS_BOUNDARY_ONLY, GLU_FALSE);
		gluTessCallback (tess, GLU_TESS_BEGIN_DATA,  (_GLUfuncptr) & Extrusion::tessBeginData);
		gluTessCallback (tess, GLU_TESS_VERTEX_DATA, (_GLUfuncptr) & Extrusion::tessVertexData);

		//gluTessCallback(tess, GLU_TESS_COMBINE_DATA, (_GLUfuncptr)&Extrusion::tessCombineData);
		gluTessCallback (tess, GLU_TESS_END_DATA, (_GLUfuncptr) & Extrusion::tessEndData);
		gluTessCallback (tess, GLU_TESS_ERROR,    (_GLUfuncptr) & Extrusion::tessError);
	}
}

std::vector <Vector3f>
Extrusion::createPoints ()
{
	std::vector <Vector3f> points;
	points .reserve (spine .size () * crossSection .size ());

	bool closedSpine = spine .front () == spine .back ();

	Vector3f SCPyAxisPrevious;
	Vector3f SCPzAxisPrevious;

	Vector3f SCPxAxis;
	Vector3f SCPyAxis;
	Vector3f SCPzAxis;

	// calculate SCPAxes

	std::vector <Rotation4f> rotations;
	rotations .reserve (spine .size ());

	// SCP for the first point:
	if (closedSpine)
	{
		SCPyAxis = normalize (spine [1] - spine [spine .size () - 2]);
		SCPzAxis = normalize (cross (spine [1] - spine [0], spine [spine .size () - 2] - spine [0]));
	}
	else
	{
		SCPyAxis = normalize (spine [1] - spine [0]);

		// Find first defined Z-axis.
		for (size_t i = 1; i < spine .size () - 1; i ++)
		{
			SCPzAxis = normalize (cross (spine [i + 1] - spine [i], spine [i - 1] - spine [i]));

			if (SCPzAxis not_eq Vector3f ())
				break;
		}
	}

	// The entire spine is collinear:
	if (SCPzAxis == Vector3f ())
		SCPzAxis = Vector3f (0, 0, 1);

	SCPxAxis = cross (SCPyAxis, SCPzAxis);
	rotations .emplace_back (Rotation4f (Vector3f (0, 1, 0), SCPyAxis));

	// For all points other than the first or last:

	SCPzAxisPrevious = SCPzAxis;

	for (size_t i = 1; i < spine .size () - 1; i ++)
	{
		SCPyAxis = normalize (spine [i + 1] - spine [i - 1]);
		SCPzAxis = normalize (cross (spine [i + 1] - spine [i], spine [i - 1] - spine [i]));

		// The three points used in computing the Z-axis are collinear.
		if (SCPzAxis == Vector3f ())
			SCPzAxis = SCPzAxisPrevious;
		else
			SCPzAxisPrevious = SCPzAxis;

		SCPxAxis = cross (SCPyAxis, SCPzAxis);

		rotations .emplace_back (Rotation4f (Vector3f (0, 1, 0), SCPyAxis));

		std::cout << SCPzAxis << std::endl;
	}

	// SCP for the last point
	if (closedSpine)
	{
		// The SCP for the first and last points is the same.
		rotations .emplace_back (rotations .front ());
	}
	else
	{
		SCPyAxis = normalize (spine [spine .size () - 1] - spine [spine .size () - 2]);
		SCPzAxis = normalize (cross (spine [spine .size () - 1] - spine [spine .size () - 2], spine [spine .size () - 3] - spine [spine .size () - 2]));

		if (SCPzAxis == Vector3f ())
			SCPzAxis = SCPzAxisPrevious;

		SCPxAxis = cross (SCPyAxis, SCPzAxis);

		rotations .emplace_back (Rotation4f (Vector3f (0, 1, 0), SCPyAxis));
	}

	// calculate vertices.

	for (size_t i = 0; i < spine .size (); i ++)
	{
		Matrix4f matrix;

		matrix .translate (spine [i]);

		if (orientation .size ())
			matrix .rotate (orientation [std::min (i, orientation .size () - 1)]);

		matrix .rotate (rotations [i]);

		if (scale .size ())
		{
			const Vector2f & s = scale [std::min (i, scale .size () - 1)];

			matrix .scale (Vector3f (s .x (), 1, s .y ()));
		}

		std::cout << matrix << std::endl;

		for (const auto & vector : crossSection)
			points .push_back (matrix .multVecMatrix (Vector3f (vector .getX (), 0, vector .getY ())));

	}

	return points;
}

void
Extrusion::build ()
{
	X3DGeometryNode::build ();

	if (spine .size () < 2 or crossSection .size () < 3)
		return;

	#define INDEX(n, k) ((n) * crossSection .size () + (k))

	bool closedSpine        = spine .front () == spine .back ();
	bool closedCrossSection = crossSection .front () == crossSection .back ();
	size_t capPoints        = closedCrossSection ? crossSection .size () - 1 : crossSection .size ();

	std::vector <Vector3f> points = createPoints ();
	std::vector <Vector2f> texCoords;
	std::vector <Vector3f> normals;
	std::vector <size_t> indices;
	NormalIndex normalIndex;

	size_t reserve = (spine .size () - 1) * (crossSection .size () - 1) * 6 + (beginCap ? (capPoints - 2) * 3 : 0) + (endCap ? (capPoints - 2) * 3 : 0);
	texCoords .reserve (reserve);
	normals .reserve (reserve);
	indices .reserve (reserve);

	for (size_t n = 0; n < spine .size () - 1; n ++)
	{
		for (size_t k = 0; k < crossSection .size () - 1; k ++)
		{
			size_t n1 = closedSpine and n == spine .size () - 2 ? 0 : n + 1;
			size_t k1 = closedCrossSection and k == crossSection .size () - 2 ? 0 : k + 1;

			// k      k+1
			//
			// p3 ----- p4   n+1
			//  | \     |
			//  |   \   |
			//  |     \ |
			// p1 ----- p2   n

			// (p4 - p1) x (p3 - p2)
			Vector3f normal = normalize (cross (points [INDEX (n1, k1)] - points [INDEX (n, k)], points [INDEX (n1, k)] - points [INDEX (n, k1)]));

			std::cout << std::endl;
			// tri 1

			// p1
			texCoords .push_back (Vector2f (k / (float) (crossSection .size () - 1), n / (float) (spine .size () - 1)));
			indices .push_back (INDEX (n, k));
			normalIndex [indices .back ()] .push_back (normals .size ());
			normals .push_back (normal);

			std::cout << indices .back () << " " << points [indices .back ()] << std::endl;

			// p2
			texCoords .push_back (Vector2f ((k + 1) / (float) (crossSection .size () - 1), n / (float) (spine .size () - 1)));
			indices .push_back (INDEX (n, k1));
			normalIndex [indices .back ()] .push_back (normals .size ());
			normals .push_back (normal);

			std::cout << indices .back () << " " << points [indices .back ()] << std::endl;

			// p3
			texCoords .push_back (Vector2f (k / (float) (crossSection .size () - 1), (n + 1) / (float) (spine .size () - 1)));
			indices .push_back (INDEX (n1, k));
			normalIndex [indices .back ()] .push_back (normals .size ());
			normals .push_back (normal);

			std::cout << indices .back () << " " << points [indices .back ()] << std::endl;

			// tri 2

			// p2
			texCoords .push_back (Vector2f ((k + 1) / (float) (crossSection .size () - 1), n / (float) (spine .size () - 1)));
			indices .push_back (INDEX (n, k1));
			normalIndex [indices .back ()] .push_back (normals .size ());
			normals .push_back (normal);

			std::cout << indices .back () << " " << points [indices .back ()] << std::endl;

			// p4
			texCoords .push_back (Vector2f ((k + 1) / (float) (crossSection .size () - 1), (n + 1) / (float) (spine .size () - 1)));
			indices .push_back (INDEX (n1, k1));
			normalIndex [indices .back ()] .push_back (normals .size ());
			normals .push_back (normal);

			std::cout << indices .back () << " " << points [indices .back ()] << std::endl;

			// p3
			texCoords .push_back (Vector2f (k / (float) (crossSection .size () - 1), (n + 1) / (float) (spine .size () - 1)));
			indices .push_back (INDEX (n1, k));
			normalIndex [indices .back ()] .push_back (normals .size ());
			normals .push_back (normal);

			std::cout << indices .back () << " " << points [indices .back ()] << std::endl;
		}
	}

	Vector2f min = crossSection [0];
	Vector2f max = crossSection [0];

	for (size_t k = 1; k < crossSection .size (); k ++)
	{
		min = math::min <float> (min, crossSection [k]);
		max = math::max <float> (max, crossSection [k]);
	}

	Vector2f size = max - min;

	if (size .x () and size .y ())
	{
		if (beginCap)
		{
			Polygon polygon;

			std::vector <Vertex*> vertices;
			vertices .reserve (capPoints * 3);

			for (size_t k = 0; k < capPoints; k ++)
				vertices .push_back (new Vertex (points, INDEX (0, capPoints - 1 - k), capPoints - 1 - k));

			gluTessBeginPolygon (tess, &polygon);
			gluTessBeginContour (tess);

			for (size_t _i = 0; _i < capPoints; ++ _i)
				gluTessVertex (tess, &vertices [_i] -> location [0], vertices [_i]);

			//gluTessEndContour(tess);
			gluEndPolygon (tess);

			Vector3f normal;

			for (const auto & polygonElement : polygon)
			{
				switch (polygonElement .type)
				{
					case GL_TRIANGLE_FAN :

						for (size_t _i = 1; _i < polygonElement .vertices .size () - 1; ++ _i)
						{
							const Vector3f & p1 = points [polygonElement .vertices [0] -> i];
							const Vector3f & p2 = points [polygonElement .vertices [_i] -> i];
							const Vector3f & p3 = points [polygonElement .vertices [_i + 1] -> i];

							normal += normalize (cross (p3 - p2, p1 - p2));
						}

						break;
					case GL_TRIANGLE_STRIP:

						for (size_t _i = 0; _i < polygonElement .vertices .size () - 2; ++ _i)
						{
							const Vector3f & p1 = points [polygonElement .vertices [_i % 2 ? _i + 1 : _i] -> i];
							const Vector3f & p2 = points [polygonElement .vertices [_i % 2 ? _i : _i + 1] -> i];
							const Vector3f & p3 = points [polygonElement .vertices [_i + 2] -> i];

							normal += normalize (cross (p3 - p2, p1 - p2));
						}

						break;
					case GL_TRIANGLES:

						for (size_t _i = 0; _i < polygonElement .vertices .size (); _i += 3)
						{
							const Vector3f & p1 = points [polygonElement .vertices [_i] -> i];
							const Vector3f & p2 = points [polygonElement .vertices [_i + 1] -> i];
							const Vector3f & p3 = points [polygonElement .vertices [_i + 2] -> i];

							normal += normalize (cross (p3 - p2, p1 - p2));
						}

						break;
					default:
						break;
				}
			}

			normal = normalize (normal);

			for (const auto & polygonElement : polygon)
			{
				switch (polygonElement .type)
				{
					case GL_TRIANGLE_FAN :

						for (size_t _i = 1; _i < polygonElement .vertices .size () - 1; ++ _i)
						{
							Vector2f t = (min + crossSection [polygonElement .vertices [0] -> k]) / size;
							texCoords .push_back (Vector2f (t .x (), 1 - t .y ()));
							indices .push_back (polygonElement .vertices [0] -> i);
							normalIndex [indices .back ()] .push_back (normals .size ());
							normals .push_back (normal);

							t = (min + crossSection [polygonElement .vertices [_i] -> k]) / size;
							texCoords .push_back (Vector2f (t .x (), 1 - t .y ()));
							indices .push_back (polygonElement .vertices [_i] -> i);
							normalIndex [indices .back ()] .push_back (normals .size ());
							normals .push_back (normal);

							t = (min + crossSection [polygonElement .vertices [_i + 1] -> k]) / size;
							texCoords .push_back (Vector2f (t .x (), 1 - t .y ()));
							indices .push_back (polygonElement .vertices [_i + 1] -> i);
							normalIndex [indices .back ()] .push_back (normals .size ());
							normals .push_back (normal);
						}

						break;
					case GL_TRIANGLE_STRIP:

						for (size_t _i = 0; _i < polygonElement .vertices .size () - 2; ++ _i)
						{
							Vector2f t = (min + crossSection [polygonElement .vertices [_i % 2 ? _i + 1 : _i] -> k]) / size;
							texCoords .push_back (Vector2f (t .x (), 1 - t .y ()));
							indices .push_back (polygonElement .vertices [_i % 2 ? _i + 1 : _i] -> i);
							normalIndex [indices .back ()] .push_back (normals .size ());
							normals .push_back (normal);

							t = (min + crossSection [polygonElement .vertices [_i % 2 ? _i : _i + 1] -> k]) / size;
							texCoords .push_back (Vector2f (t .x (), 1 - t .y ()));
							indices .push_back (polygonElement .vertices [_i % 2 ? _i : _i + 1] -> i);
							normalIndex [indices .back ()] .push_back (normals .size ());
							normals .push_back (normal);

							t = (min + crossSection [polygonElement .vertices [_i + 2] -> k]) / size;
							texCoords .push_back (Vector2f (t .x (), 1 - t .y ()));
							indices .push_back (polygonElement .vertices [_i + 2] -> i);
							normalIndex [indices .back ()] .push_back (normals .size ());
							normals .push_back (normal);
						}

						break;
					case GL_TRIANGLES:

						for (size_t _i = 0; _i < polygonElement .vertices .size (); _i += 3)
						{
							Vector2f t = (min + crossSection [polygonElement .vertices [_i] -> k]) / size;
							texCoords .push_back (Vector2f (t .x (), 1 - t .y ()));
							indices .push_back (polygonElement .vertices [_i] -> i);
							normalIndex [indices .back ()] .push_back (normals .size ());
							normals .push_back (normal);

							t = (min + crossSection [polygonElement .vertices [_i + 1] -> k]) / size;
							texCoords .push_back (Vector2f (t .x (), 1 - t .y ()));
							indices .push_back (polygonElement .vertices [_i + 1] -> i);
							normalIndex [indices .back ()] .push_back (normals .size ());
							normals .push_back (normal);

							t = (min + crossSection [polygonElement .vertices [_i + 2] -> k]) / size;
							texCoords .push_back (Vector2f (t .x (), 1 - t .y ()));
							indices .push_back (polygonElement .vertices [_i + 2] -> i);
							normalIndex [indices .back ()] .push_back (normals .size ());
							normals .push_back (normal);
						}

						break;
					default:
						break;
				}
			}

			for (size_t k = 0; k < capPoints; k ++)
				delete vertices [k];
		}

		if (endCap)
		{
			size_t j = spine .size () - 1;

			Polygon polygon;

			std::vector <Vertex*> vertices;
			vertices .reserve (capPoints * 3);

			for (size_t k = 0; k < capPoints; k ++)
				vertices .push_back (new Vertex (points, INDEX (j, k), k));

			gluTessBeginPolygon (tess, &polygon);
			gluTessBeginContour (tess);

			for (size_t _i = 0; _i < capPoints; ++ _i)
				gluTessVertex (tess, &vertices [_i] -> location [0], vertices [_i]);

			//gluTessEndContour(tess);
			gluEndPolygon (tess);

			Vector3f normal;

			for (const auto & polygonElement : polygon)
			{
				switch (polygonElement .type)
				{
					case GL_TRIANGLE_FAN :

						for (size_t _i = 1; _i < polygonElement .vertices .size () - 1; ++ _i)
						{
							const Vector3f & p1 = points [polygonElement .vertices [0] -> i];
							const Vector3f & p2 = points [polygonElement .vertices [_i] -> i];
							const Vector3f & p3 = points [polygonElement .vertices [_i + 1] -> i];

							normal += normalize (cross (p3 - p2, p1 - p2));
						}

						break;
					case GL_TRIANGLE_STRIP:

						for (size_t _i = 0; _i < polygonElement .vertices .size () - 2; ++ _i)
						{
							const Vector3f & p1 = points [polygonElement .vertices [_i % 2 ? _i + 1 : _i] -> i];
							const Vector3f & p2 = points [polygonElement .vertices [_i % 2 ? _i : _i + 1] -> i];
							const Vector3f & p3 = points [polygonElement .vertices [_i + 2] -> i];

							normal += normalize (cross (p3 - p2, p1 - p2));
						}

						break;
					case GL_TRIANGLES:

						for (size_t _i = 0; _i < polygonElement .vertices .size (); _i += 3)
						{
							const Vector3f & p1 = points [polygonElement .vertices [_i] -> i];
							const Vector3f & p2 = points [polygonElement .vertices [_i + 1] -> i];
							const Vector3f & p3 = points [polygonElement .vertices [_i + 2] -> i];

							normal += normalize (cross (p3 - p2, p1 - p2));
						}

						break;
					default:
						break;
				}
			}

			normal = normalize (normal);

			for (const auto & polygonElement : polygon)
			{
				switch (polygonElement .type)
				{
					case GL_TRIANGLE_FAN :

						for (size_t _i = 1; _i < polygonElement .vertices .size () - 1; ++ _i)
						{
							Vector2f t = (min + crossSection [polygonElement .vertices [0] -> k]) / size;
							texCoords .push_back (Vector2f (t .x (), 1 - t .y ()));
							indices .push_back (polygonElement .vertices [0] -> i);
							normalIndex [indices .back ()] .push_back (normals .size ());
							normals .push_back (normal);

							t = (min + crossSection [polygonElement .vertices [_i] -> k]) / size;
							texCoords .push_back (Vector2f (t .x (), 1 - t .y ()));
							indices .push_back (polygonElement .vertices [_i] -> i);
							normalIndex [indices .back ()] .push_back (normals .size ());
							normals .push_back (normal);

							t = (min + crossSection [polygonElement .vertices [_i + 1] -> k]) / size;
							texCoords .push_back (Vector2f (t .x (), 1 - t .y ()));
							indices .push_back (polygonElement .vertices [_i + 1] -> i);
							normalIndex [indices .back ()] .push_back (normals .size ());
							normals .push_back (normal);
						}

						break;
					case GL_TRIANGLE_STRIP:

						for (size_t _i = 0; _i < polygonElement .vertices .size () - 2; ++ _i)
						{
							Vector2f t = (min + crossSection [polygonElement .vertices [_i % 2 ? _i + 1 : _i] -> k]) / size;
							texCoords .push_back (Vector2f (t .x (), 1 - t .y ()));
							indices .push_back (polygonElement .vertices [_i % 2 ? _i + 1 : _i] -> i);
							normalIndex [indices .back ()] .push_back (normals .size ());
							normals .push_back (normal);

							t = (min + crossSection [polygonElement .vertices [_i % 2 ? _i : _i + 1] -> k]) / size;
							texCoords .push_back (Vector2f (t .x (), 1 - t .y ()));
							indices .push_back (polygonElement .vertices [_i % 2 ? _i : _i + 1] -> i);
							normalIndex [indices .back ()] .push_back (normals .size ());
							normals .push_back (normal);

							t = (min + crossSection [polygonElement .vertices [_i + 2] -> k]) / size;
							texCoords .push_back (Vector2f (t .x (), 1 - t .y ()));
							indices .push_back (polygonElement .vertices [_i + 2] -> i);
							normalIndex [indices .back ()] .push_back (normals .size ());
							normals .push_back (normal);
						}

						break;
					case GL_TRIANGLES:

						for (size_t _i = 0; _i < polygonElement .vertices .size (); _i += 3)
						{
							Vector2f t = (min + crossSection [polygonElement .vertices [_i] -> k]) / size;
							texCoords .push_back (Vector2f (t .x (), 1 - t .y ()));
							indices .push_back (polygonElement .vertices [_i] -> i);
							normalIndex [indices .back ()] .push_back (normals .size ());
							normals .push_back (normal);

							t = (min + crossSection [polygonElement .vertices [_i + 1] -> k]) / size;
							texCoords .push_back (Vector2f (t .x (), 1 - t .y ()));
							indices .push_back (polygonElement .vertices [_i + 1] -> i);
							normalIndex [indices .back ()] .push_back (normals .size ());
							normals .push_back (normal);

							t = (min + crossSection [polygonElement .vertices [_i + 2] -> k]) / size;
							texCoords .push_back (Vector2f (t .x (), 1 - t .y ()));
							indices .push_back (polygonElement .vertices [_i + 2] -> i);
							normalIndex [indices .back ()] .push_back (normals .size ());
							normals .push_back (normal);
						}

						break;
					default:
						break;
				}
			}

			for (size_t k = 0; k < capPoints; k ++)
				delete vertices [k];
		}
	}

	refineNormals (normalIndex, normals);

	GLsizei glIndices = 0;

	for (size_t i = 0; i < indices .size (); i += 3)
	{
		getTexCoord () .emplace_back (texCoords [i]);
		getNormals  () .emplace_back (normals [i]);
		getVertices () .emplace_back (points [indices [i]]);

		++ glIndices;

		getTexCoord () .emplace_back (texCoords [i + 1]);
		getNormals  () .emplace_back (normals [i + 1]);
		getVertices () .emplace_back (points [indices [i + 1]]);

		++ glIndices;

		getTexCoord () .emplace_back (texCoords [i + 2]);
		getNormals  () .emplace_back (normals [i + 2]);
		getVertices () .emplace_back (points [indices [i + 2]]);

		++ glIndices;
	}

	setVertexMode (GL_TRIANGLES);
	setNumIndices (glIndices);
}

void
Extrusion::tessBeginData (GLenum type, void* polygon_data)
{
	Polygon* polygon = (Polygon*) polygon_data;

	polygon -> push_back (PolygonElement (type));
}

void
Extrusion::tessVertexData (void* vertex_data, void* polygon_data)
{
	Polygon* polygon = (Polygon*) polygon_data;
	Vertex*  vertex  = (Vertex*) vertex_data;

	polygon -> back () .vertices .push_back (vertex);
}

void
Extrusion::tessCombineData (
   GLdouble coords [3], void* vertex_data [4],
   GLfloat weight [4], void** outData,
   void* polygon_data
   )
{
	// not used yet
}

void
Extrusion::tessEndData (void* polygon_data)
{ }

void
Extrusion::tessError (GLenum err_no)
{
	std::clog << "Warning: in function " << __func__ << " '" << (char*) gluErrorString (err_no) << "'." << std::endl;
}

void
Extrusion::dispose ()
{
	if (tess)
		gluDeleteTess (tess);

	X3DGeometryNode::dispose ();
}

} // X3D
} // titania
