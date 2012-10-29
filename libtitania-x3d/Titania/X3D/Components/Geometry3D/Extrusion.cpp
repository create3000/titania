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
	             ccw (true),                                                                                   // SFBool     [ ]  ccw               TRUE
	          convex (true),                                                                                   // SFBool     [ ]  convex            TRUE
	     creaseAngle (),                                                                                       // SFFloat    [ ]  creaseAngle       0                                [0,∞)
	    crossSection ({ SFVec2f (1, 1), SFVec2f (1, -1), SFVec2f (-1, -1), SFVec2f (-1, 1), SFVec2f (1, 1) }), // MFVec2f    [ ]  crossSection      [1 1 1 -1 -1 -1 -1 1 1 1]        (-∞,∞)
	          endCap (true),                                                                                   // SFBool     [ ]  endCap            TRUE
	     orientation ({ SFRotation () }),                                                                      // MFRotation [ ]  orientation       0 0 1 0                          [-1,1] or (-∞,∞)
	           scale ({ SFVec2f (1, 1) }),                                                                     // MFVec2f    [ ]  scale             1 1                              (0,∞)
	           solid (true),                                                                                   // SFBool     [ ]  solid             TRUE
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

Vector3f
Extrusion::calculateSCPyAxis (const size_t i, const bool closed)
{
	Vector3f Y;

	if (closed)
	{
		if (i > 0 and i < spine .size () - 1)
			Y = spine .at (i + 1) - spine .at (i - 1);

		else
			Y = spine .at (1) - spine .at (spine .size () - 2);
	}
	else
	{
		if (i == 0)
			Y = spine .at (1) - spine .at (0);

		else if (i == spine .size () - 1)
			Y = spine .at (spine .size () - 1) - spine .at (spine .size () - 2);

		else
			Y = spine .at (i + 1) - spine .at (i - 1);
	}

	return normalize (Y);
}

Vector3f
Extrusion::calculateSCPzAxis (const size_t i, const bool closed)
{
	Vector3f z0, z1;

	if (closed)
	{
		if (i > 0 and i < spine .size () - 1)
		{
			z0 = spine .at (i + 1) - spine .at (i);
			z1 = spine .at (i - 1) - spine .at (i);
		}
		else
		{
			z0 = spine .at (1) - spine .at (0);
			z1 = spine .at (spine .size () - 2) - spine .at (0);
		}
	}
	else
	{
		if (spine .size () == 2)
			return Vector3f ();
		else if (i == 0)
		{
			z0 = spine .at (2) - spine .at (1);
			z1 = spine .at (0) - spine .at (1);
		}
		else if (i == spine .size () - 1)
		{
			z0 = spine .at (spine .size () - 1) - spine .at (spine .size () - 2);
			z1 = spine .at (spine .size () - 3) - spine .at (spine .size () - 2);
		}
		else
		{
			z0 = spine .at (i + 1) - spine .at (i);
			z1 = spine .at (i - 1) - spine .at (i);
		}
	}

	z0 = normalize (z0);
	z1 = normalize (z1);

	// test if spine segments are collinear. If they are, the cross
	// product will not be reliable, and we should just use the previous
	// Z-axis instead.
	if (std::abs (dot (z0, z1)) == 1)
		return Vector3f ();

	Vector3f tmp = cross (z0, z1);

	if (tmp == Vector3f ())
		return Vector3f ();

	return normalize (tmp);
}

std::vector <Matrix4f>
Extrusion::createCorrectionRotations (const std::vector <Vector3f> & SCPzAxes, const std::vector <Matrix4f> & rotations)
{
	std::vector <Matrix4f> correctionRotations (spine .size ());

	// testPoint is used to find the angle that gives the smallest distance
	// between the previous and current rotation. Find a point that is not
	// in the origin.
	Vector3f testPoint = Vector3f (crossSection .at (0) .getX (), 0, crossSection .at (0) .getY ());

	for (size_t i = 0; i < crossSection .size (); i ++)
	{
		if (crossSection .at (i) not_eq Vector2f ())
		{
			testPoint = Vector3f (crossSection .at (i) .getX (), 0, crossSection .at (i) .getY ());
			break;
		}
	}

	// Fix the orientations by using the angle between previous SCPzAxis and current SCPzAxis
	for (size_t i = 1; i < spine .size (); i ++)
	{
		float cosAngle = math::clamp (dot (SCPzAxes [i], SCPzAxes [i - 1]), -1.0f, 1.0f);

		if (cosAngle < 1)
		{
			float angle = std::acos (cosAngle);

			//Point3f previous = testPoint;
			// Test with negative angle:
			Vector3f previous = rotations [i - 1] .multLeft (correctionRotations [i - 1]) .multVecMatrix (testPoint);

			Matrix4f delta = Rotation4f (0, 1, 0, -angle);
			correctionRotations [i] = correctionRotations [i] .multLeft (delta);

			float distNegative = abs (rotations [i] .multLeft (correctionRotations [i]) .multVecMatrix (testPoint) - previous);

			// Test with positive angle
			delta                   = Rotation4f (0, 1, 0, angle * 2);
			correctionRotations [i] = correctionRotations [i] .multLeft (delta);

			float distPositive = abs (rotations [i] .multLeft (correctionRotations [i]) .multVecMatrix (testPoint) - previous); // FIXME multVecMatrix (Vector3f::Null) ???

			if (distPositive > distNegative)
			{
				// Reset correctionRotations to negative angle
				delta                   = Rotation4f (0, 1, 0, -angle * 2);
				correctionRotations [i] = correctionRotations [i] .multLeft (delta);
			}

			// Check that the angle is not more than PI.
			// If it is subtract PI from angle
			float checkAngle = correctionRotations [i] .getRotation () .angle ();

			if (checkAngle >= M_PI)
				correctionRotations [i] = Rotation4f (0, 1, 0, checkAngle - M_PI);
		}
	}

	return correctionRotations;
}

std::vector <Vector3f>
Extrusion::getPoints ()
{
	std::vector <Vector3f> points;
	points .reserve (spine .size () * crossSection .size ());

	bool closedSpine = spine .front () == spine .back ();

	Vector3f SCPyAxisPrevious;
	Vector3f SCPzAxisPrevious;

	Vector3f SCPxAxis;
	Vector3f SCPyAxis;
	Vector3f SCPzAxis;

	bool colinear = false;

	// find first non-collinear spine segments and calculate the first
	// valid Y and Z axis
	for (size_t i = 0; i < spine .size (); i ++)
	{
		SCPyAxis = calculateSCPyAxis (i, closedSpine);

		if (SCPyAxis not_eq Vector3f ())
		{
			SCPyAxisPrevious = SCPyAxis;
			break;
		}
	}

	for (size_t i = 0; i < spine .size (); i ++)
	{
		SCPzAxis = calculateSCPzAxis (i, closedSpine);

		if (SCPzAxis not_eq Vector3f ())
		{
			SCPzAxisPrevious = SCPzAxis;
			break;
		}
	}

	if (SCPyAxisPrevious == Vector3f ())
		SCPyAxisPrevious = Vector3f (0, 1, 0);

	if (SCPzAxisPrevious == Vector3f ())
	{
		// all spine segments are colinear, calculate constant Z axis
		SCPzAxisPrevious = Vector3f (0, 0, 1);

		if (SCPyAxisPrevious not_eq Vector3f (0, 1, 0))
			SCPzAxisPrevious = Rotation4f (Vector3f (0, 1, 0), SCPyAxisPrevious) * SCPzAxisPrevious;

		colinear = true;
	}

	// calculate SCPAxes

	std::vector <Vector3f> SCPzAxes;
	std::vector <Matrix4f> rotations;
	SCPzAxes .reserve (spine .size ());
	rotations .reserve (spine .size ());

	for (size_t i = 0; i < spine .size (); i ++)
	{
		if (colinear)
		{
			SCPyAxis = SCPyAxisPrevious;
			SCPzAxis = SCPzAxisPrevious;
		}
		else
		{
			SCPyAxis = calculateSCPyAxis (i, closedSpine);

			if (SCPyAxis == Vector3f ())
				SCPyAxis = SCPyAxisPrevious;

			SCPzAxis = calculateSCPzAxis (i, closedSpine);

			if (SCPzAxis == Vector3f ())
				SCPzAxis = SCPzAxisPrevious;

			if (dot (SCPzAxis, SCPzAxisPrevious) < 0)
				SCPzAxis = -SCPzAxis;
		}

		SCPxAxis = normalize (cross (SCPyAxis, SCPzAxis));

		SCPyAxisPrevious = SCPyAxis;
		SCPzAxisPrevious = SCPzAxis;

		SCPzAxes .push_back (SCPzAxis);

		rotations .push_back (Matrix4f (SCPxAxis .x (), SCPxAxis .y (), SCPxAxis .z (), 0,
		                                SCPyAxis .x (), SCPyAxis .y (), SCPyAxis .z (), 0,
		                                SCPzAxis .x (), SCPzAxis .y (), SCPzAxis .z (), 0,
		                                0,              0,              0,              1));
	}

	std::vector <Matrix4f> correctionRotations = createCorrectionRotations (SCPzAxes, rotations);

	// calculate vertices.

	for (size_t i = 0; i < spine .size (); i ++)
	{
		Matrix4f matrix;

		matrix .set (spine .at (i));

		matrix = matrix .multLeft (rotations [i]) .multLeft (correctionRotations [i]);

		if (orientation .size ())
			matrix = matrix .multLeft (Matrix4f (orientation .at (std::min (i, orientation .size () - 1))));

		if (scale .size ())
		{
			const Vector2f & s = scale .at (std::min (i, scale .size () - 1));

			Matrix4f scaleMatrix;
			scaleMatrix [0] [0] = s .x ();
			scaleMatrix [2] [2] = s .y ();

			matrix = matrix .multLeft (scaleMatrix);
		}

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

	#define INDEX(a, b) ((a) * crossSection .size () + (b))

	bool closedSpine        = spine .front () == spine .back ();
	bool closedCrossSection = crossSection .front () == crossSection .back ();
	size_t capPoints        = closedCrossSection ? crossSection .size () - 1 : crossSection .size ();

	std::vector <Vector3f> points = getPoints ();
	std::vector <Vector2f> texCoords;
	std::vector <Vector3f> normals;
	std::vector <size_t> indices;
	VertexMap vertexMap;

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

			// p4 - p3
			//  | / |
			// p1 - p2

			// (p3 - p1) x (p4 - p2)
			Vector3f normal = normalize (cross (points [INDEX (n1, k1)] - points [INDEX (n1, k)], points [INDEX (n1, k)] - points [INDEX (n, k1)]));

			// tri 1

			// p1
			texCoords .push_back (Vector2f (k / (float) (crossSection .size () - 1), n / (float) (spine .size () - 1)));
			indices .push_back (INDEX (n, k));
			vertexMap [indices .back ()] .push_back (normals .size ());
			normals .push_back (normal);

			// p2
			texCoords .push_back (Vector2f ((k + 1) / (float) (crossSection .size () - 1), n / (float) (spine .size () - 1)));
			indices .push_back (INDEX (n, k1));
			vertexMap [indices .back ()] .push_back (normals .size ());
			normals .push_back (normal);

			// p3
			texCoords .push_back (Vector2f ((k + 1) / (float) (crossSection .size () - 1), (n + 1) / (float) (spine .size () - 1)));
			indices .push_back (INDEX (n1, k1));
			vertexMap [indices .back ()] .push_back (normals .size ());
			normals .push_back (normal);

			// tri 2

			// p3
			texCoords .push_back (Vector2f ((k + 1) / (float) (crossSection .size () - 1), (n + 1) / (float) (spine .size () - 1)));
			indices .push_back (INDEX (n1, k1));
			vertexMap [indices .back ()] .push_back (normals .size ());
			normals .push_back (normal);

			// p4
			texCoords .push_back (Vector2f (k / (float) (crossSection .size () - 1), (n + 1) / (float) (spine .size () - 1)));
			indices .push_back (INDEX (n1, k));
			vertexMap [indices .back ()] .push_back (normals .size ());
			normals .push_back (normal);

			// p1
			texCoords .push_back (Vector2f (k / (float) (crossSection .size () - 1), n / (float) (spine .size () - 1)));
			indices .push_back (INDEX (n, k));
			vertexMap [indices .back ()] .push_back (normals .size ());
			normals .push_back (normal);
		}
	}

	Vector2f min = crossSection .at (0);
	Vector2f max = crossSection .at (0);

	for (size_t k = 1; k < crossSection .size (); k ++)
	{
		min = math::min <float> (min, crossSection .at (k));
		max = math::max <float> (max, crossSection .at (k));
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

			Polygon::const_iterator polygonElement;

			for (polygonElement = polygon .begin (); polygonElement not_eq polygon .end (); ++ polygonElement)
			{
				switch (polygonElement -> type)
				{
					case GL_TRIANGLE_FAN:

						for (size_t _i = 1; _i < polygonElement -> vertices .size () - 1; ++ _i)
						{
							const Vector3f & p1 = points [polygonElement -> vertices [0] -> i];
							const Vector3f & p2 = points [polygonElement -> vertices [_i] -> i];
							const Vector3f & p3 = points [polygonElement -> vertices [_i + 1] -> i];

							normal += normalize (cross (p3 - p2, p1 - p2));
						}

						break;
					case GL_TRIANGLE_STRIP:

						for (size_t _i = 0; _i < polygonElement -> vertices .size () - 2; ++ _i)
						{
							const Vector3f & p1 = points [polygonElement -> vertices [_i % 2 ? _i + 1 : _i] -> i];
							const Vector3f & p2 = points [polygonElement -> vertices [_i % 2 ? _i : _i + 1] -> i];
							const Vector3f & p3 = points [polygonElement -> vertices [_i + 2] -> i];

							normal += normalize (cross (p3 - p2, p1 - p2));
						}

						break;
					case GL_TRIANGLES:

						for (size_t _i = 0; _i < polygonElement -> vertices .size (); _i += 3)
						{
							const Vector3f & p1 = points [polygonElement -> vertices [_i] -> i];
							const Vector3f & p2 = points [polygonElement -> vertices [_i + 1] -> i];
							const Vector3f & p3 = points [polygonElement -> vertices [_i + 2] -> i];

							normal += normalize (cross (p3 - p2, p1 - p2));
						}

						break;
					default:
						break;
				}
			}

			normal = normalize (normal);

			for (polygonElement = polygon .begin (); polygonElement not_eq polygon .end (); ++ polygonElement)
			{
				switch (polygonElement -> type)
				{
					case GL_TRIANGLE_FAN:

						for (size_t _i = 1; _i < polygonElement -> vertices .size () - 1; ++ _i)
						{
							Vector2f t = (min + crossSection .at (polygonElement -> vertices [0] -> k)) / size;
							texCoords .push_back (Vector2f (t .x (), 1 - t .y ()));
							indices .push_back (polygonElement -> vertices [0] -> i);
							vertexMap [indices .back ()] .push_back (normals .size ());
							normals .push_back (normal);

							t = (min + crossSection .at (polygonElement -> vertices [_i] -> k)) / size;
							texCoords .push_back (Vector2f (t .x (), 1 - t .y ()));
							indices .push_back (polygonElement -> vertices [_i] -> i);
							vertexMap [indices .back ()] .push_back (normals .size ());
							normals .push_back (normal);

							t = (min + crossSection .at (polygonElement -> vertices [_i + 1] -> k)) / size;
							texCoords .push_back (Vector2f (t .x (), 1 - t .y ()));
							indices .push_back (polygonElement -> vertices [_i + 1] -> i);
							vertexMap [indices .back ()] .push_back (normals .size ());
							normals .push_back (normal);
						}

						break;
					case GL_TRIANGLE_STRIP:

						for (size_t _i = 0; _i < polygonElement -> vertices .size () - 2; ++ _i)
						{
							Vector2f t = (min + crossSection .at (polygonElement -> vertices [_i % 2 ? _i + 1 : _i] -> k)) / size;
							texCoords .push_back (Vector2f (t .x (), 1 - t .y ()));
							indices .push_back (polygonElement -> vertices [_i % 2 ? _i + 1 : _i] -> i);
							vertexMap [indices .back ()] .push_back (normals .size ());
							normals .push_back (normal);

							t = (min + crossSection .at (polygonElement -> vertices [_i % 2 ? _i : _i + 1] -> k)) / size;
							texCoords .push_back (Vector2f (t .x (), 1 - t .y ()));
							indices .push_back (polygonElement -> vertices [_i % 2 ? _i : _i + 1] -> i);
							vertexMap [indices .back ()] .push_back (normals .size ());
							normals .push_back (normal);

							t = (min + crossSection .at (polygonElement -> vertices [_i + 2] -> k)) / size;
							texCoords .push_back (Vector2f (t .x (), 1 - t .y ()));
							indices .push_back (polygonElement -> vertices [_i + 2] -> i);
							vertexMap [indices .back ()] .push_back (normals .size ());
							normals .push_back (normal);
						}

						break;
					case GL_TRIANGLES:

						for (size_t _i = 0; _i < polygonElement -> vertices .size (); _i += 3)
						{
							Vector2f t = (min + crossSection .at (polygonElement -> vertices [_i] -> k)) / size;
							texCoords .push_back (Vector2f (t .x (), 1 - t .y ()));
							indices .push_back (polygonElement -> vertices [_i] -> i);
							vertexMap [indices .back ()] .push_back (normals .size ());
							normals .push_back (normal);

							t = (min + crossSection .at (polygonElement -> vertices [_i + 1] -> k)) / size;
							texCoords .push_back (Vector2f (t .x (), 1 - t .y ()));
							indices .push_back (polygonElement -> vertices [_i + 1] -> i);
							vertexMap [indices .back ()] .push_back (normals .size ());
							normals .push_back (normal);

							t = (min + crossSection .at (polygonElement -> vertices [_i + 2] -> k)) / size;
							texCoords .push_back (Vector2f (t .x (), 1 - t .y ()));
							indices .push_back (polygonElement -> vertices [_i + 2] -> i);
							vertexMap [indices .back ()] .push_back (normals .size ());
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

			Polygon::const_iterator polygonElement;

			for (polygonElement = polygon .begin (); polygonElement not_eq polygon .end (); ++ polygonElement)
			{
				switch (polygonElement -> type)
				{
					case GL_TRIANGLE_FAN:

						for (size_t _i = 1; _i < polygonElement -> vertices .size () - 1; ++ _i)
						{
							const Vector3f & p1 = points [polygonElement -> vertices [0] -> i];
							const Vector3f & p2 = points [polygonElement -> vertices [_i] -> i];
							const Vector3f & p3 = points [polygonElement -> vertices [_i + 1] -> i];

							normal += normalize (cross (p3 - p2, p1 - p2));
						}

						break;
					case GL_TRIANGLE_STRIP:

						for (size_t _i = 0; _i < polygonElement -> vertices .size () - 2; ++ _i)
						{
							const Vector3f & p1 = points [polygonElement -> vertices [_i % 2 ? _i + 1 : _i] -> i];
							const Vector3f & p2 = points [polygonElement -> vertices [_i % 2 ? _i : _i + 1] -> i];
							const Vector3f & p3 = points [polygonElement -> vertices [_i + 2] -> i];

							normal += normalize (cross (p3 - p2, p1 - p2));
						}

						break;
					case GL_TRIANGLES:

						for (size_t _i = 0; _i < polygonElement -> vertices .size (); _i += 3)
						{
							const Vector3f & p1 = points [polygonElement -> vertices [_i] -> i];
							const Vector3f & p2 = points [polygonElement -> vertices [_i + 1] -> i];
							const Vector3f & p3 = points [polygonElement -> vertices [_i + 2] -> i];

							normal += normalize (cross (p3 - p2, p1 - p2));
						}

						break;
					default:
						break;
				}
			}

			normal = normalize (normal);

			for (polygonElement = polygon .begin (); polygonElement not_eq polygon .end (); ++ polygonElement)
			{
				switch (polygonElement -> type)
				{
					case GL_TRIANGLE_FAN:

						for (size_t _i = 1; _i < polygonElement -> vertices .size () - 1; ++ _i)
						{
							Vector2f t = (min + crossSection .at (polygonElement -> vertices [0] -> k)) / size;
							texCoords .push_back (Vector2f (t .x (), 1 - t .y ()));
							indices .push_back (polygonElement -> vertices [0] -> i);
							vertexMap [indices .back ()] .push_back (normals .size ());
							normals .push_back (normal);

							t = (min + crossSection .at (polygonElement -> vertices [_i] -> k)) / size;
							texCoords .push_back (Vector2f (t .x (), 1 - t .y ()));
							indices .push_back (polygonElement -> vertices [_i] -> i);
							vertexMap [indices .back ()] .push_back (normals .size ());
							normals .push_back (normal);

							t = (min + crossSection .at (polygonElement -> vertices [_i + 1] -> k)) / size;
							texCoords .push_back (Vector2f (t .x (), 1 - t .y ()));
							indices .push_back (polygonElement -> vertices [_i + 1] -> i);
							vertexMap [indices .back ()] .push_back (normals .size ());
							normals .push_back (normal);
						}

						break;
					case GL_TRIANGLE_STRIP:

						for (size_t _i = 0; _i < polygonElement -> vertices .size () - 2; ++ _i)
						{
							Vector2f t = (min + crossSection .at (polygonElement -> vertices [_i % 2 ? _i + 1 : _i] -> k)) / size;
							texCoords .push_back (Vector2f (t .x (), 1 - t .y ()));
							indices .push_back (polygonElement -> vertices [_i % 2 ? _i + 1 : _i] -> i);
							vertexMap [indices .back ()] .push_back (normals .size ());
							normals .push_back (normal);

							t = (min + crossSection .at (polygonElement -> vertices [_i % 2 ? _i : _i + 1] -> k)) / size;
							texCoords .push_back (Vector2f (t .x (), 1 - t .y ()));
							indices .push_back (polygonElement -> vertices [_i % 2 ? _i : _i + 1] -> i);
							vertexMap [indices .back ()] .push_back (normals .size ());
							normals .push_back (normal);

							t = (min + crossSection .at (polygonElement -> vertices [_i + 2] -> k)) / size;
							texCoords .push_back (Vector2f (t .x (), 1 - t .y ()));
							indices .push_back (polygonElement -> vertices [_i + 2] -> i);
							vertexMap [indices .back ()] .push_back (normals .size ());
							normals .push_back (normal);
						}

						break;
					case GL_TRIANGLES:

						for (size_t _i = 0; _i < polygonElement -> vertices .size (); _i += 3)
						{
							Vector2f t = (min + crossSection .at (polygonElement -> vertices [_i] -> k)) / size;
							texCoords .push_back (Vector2f (t .x (), 1 - t .y ()));
							indices .push_back (polygonElement -> vertices [_i] -> i);
							vertexMap [indices .back ()] .push_back (normals .size ());
							normals .push_back (normal);

							t = (min + crossSection .at (polygonElement -> vertices [_i + 1] -> k)) / size;
							texCoords .push_back (Vector2f (t .x (), 1 - t .y ()));
							indices .push_back (polygonElement -> vertices [_i + 1] -> i);
							vertexMap [indices .back ()] .push_back (normals .size ());
							normals .push_back (normal);

							t = (min + crossSection .at (polygonElement -> vertices [_i + 2] -> k)) / size;
							texCoords .push_back (Vector2f (t .x (), 1 - t .y ()));
							indices .push_back (polygonElement -> vertices [_i + 2] -> i);
							vertexMap [indices .back ()] .push_back (normals .size ());
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

	refineNormals (normals, vertexMap, creaseAngle, ccw);

	GLsizei glIndices = 0;

	for (size_t i = 0; i < indices .size (); i += 3)
	{
		getGLTexCoord () -> push_back (texCoords [i] .x ());
		getGLTexCoord () -> push_back (texCoords [i] .y ());

		getGLNormals () -> push_back (normals [i] .x ());
		getGLNormals () -> push_back (normals [i] .y ());
		getGLNormals () -> push_back (normals [i] .z ());

		getGLPoints () -> push_back (points [indices [i]] .x ());
		getGLPoints () -> push_back (points [indices [i]] .y ());
		getGLPoints () -> push_back (points [indices [i]] .z ());

		++ glIndices;

		getGLTexCoord () -> push_back (texCoords [i + 1] .x ());
		getGLTexCoord () -> push_back (texCoords [i + 1] .y ());

		getGLNormals () -> push_back (normals [i + 1] .x ());
		getGLNormals () -> push_back (normals [i + 1] .y ());
		getGLNormals () -> push_back (normals [i + 1] .z ());

		getGLPoints () -> push_back (points [indices [i + 1]] .x ());
		getGLPoints () -> push_back (points [indices [i + 1]] .y ());
		getGLPoints () -> push_back (points [indices [i + 1]] .z ());

		++ glIndices;

		getGLTexCoord () -> push_back (texCoords [i + 2] .x ());
		getGLTexCoord () -> push_back (texCoords [i + 2] .y ());

		getGLNormals () -> push_back (normals [i + 2] .x ());
		getGLNormals () -> push_back (normals [i + 2] .y ());
		getGLNormals () -> push_back (normals [i + 2] .z ());

		getGLPoints () -> push_back (points [indices [i + 2]] .x ());
		getGLPoints () -> push_back (points [indices [i + 2]] .y ());
		getGLPoints () -> push_back (points [indices [i + 2]] .z ());

		++ glIndices;
	}

	setGLSolid (solid);
	setGLCCW (ccw ? GL_CCW : GL_CW);
	setGLMode (GL_TRIANGLES);
	setGLIndices (glIndices);
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
