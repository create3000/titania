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

#include "Extrusion.h"

#include "../../Execution/X3DExecutionContext.h"
#include <Titania/Utility/Adapter.h>

namespace titania {
namespace X3D {

const std::string Extrusion::componentName  = "Geometry3D";
const std::string Extrusion::typeName       = "Extrusion";
const std::string Extrusion::containerField = "geometry";

Extrusion::Fields::Fields () :
	    beginCap (new SFBool (true)),
	      endCap (new SFBool (true)),
	       solid (new SFBool (true)),
	         ccw (new SFBool (true)),
	      convex (new SFBool (true)),
	 creaseAngle (new SFFloat ()),
	crossSection (new MFVec2f ({ SFVec2f (1, 1), SFVec2f (1, -1), SFVec2f (-1, -1), SFVec2f (-1, 1), SFVec2f (1, 1) })),
	 orientation (new MFRotation ({ SFRotation () })),
	       scale (new MFVec2f ({ SFVec2f (1, 1) })),
	       spine (new MFVec3f ({ SFVec3f (0, 0, 0), SFVec3f (0, 1, 0) }))
{ }

Extrusion::Extrusion (X3DExecutionContext* const executionContext) :
	    X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DGeometryNode (),
	         fields ()
{
	addField (inputOutput,    "metadata",         metadata ());

	addField (initializeOnly, "beginCap",         beginCap ());
	addField (initializeOnly, "endCap",           endCap ());
	addField (initializeOnly, "solid",            solid ());
	addField (initializeOnly, "ccw",              ccw ());
	addField (initializeOnly, "convex",           convex ());
	addField (initializeOnly, "creaseAngle",      creaseAngle ());

	addField (inputOutput,    "crossSection",     crossSection ());
	addField (inputOutput,    "orientation",      orientation ());
	addField (inputOutput,    "scale",            scale ());
	addField (inputOutput,    "spine",            spine ());
}

X3DBaseNode*
Extrusion::create (X3DExecutionContext* const executionContext) const
{
	return new Extrusion (executionContext);
}

void
Extrusion::initialize ()
{
	X3DGeometryNode::initialize ();
}

std::vector <Vector3f>
Extrusion::createPoints (bool hasCaps)
{
	size_t reserve = spine () .size () * crossSection () .size ();

	if (hasCaps)
	{
		if (beginCap ())
			reserve += crossSection () .size ();

		if (endCap ())
			reserve += crossSection () .size ();
	}

	std::vector <Vector3f> points;
	points .reserve (reserve);

	// calculate SCP rotations

	std::vector <Matrix4f> rotations = std::move (createRotations ());

	// calculate vertices.

	for (size_t i = 0, size = spine () .size (); i < size; i ++)
	{
		Matrix4f matrix;

		matrix .translate (spine () [i]);

		if (orientation () .size ())
			matrix .rotate (orientation () [std::min (i, orientation () .size () - 1)]);

		matrix .multLeft (rotations [i]);

		if (scale () .size ())
		{
			const Vector2f & s = scale () [std::min (i, scale () .size () - 1)];

			matrix .scale (Vector3f (s .x (), 1, s .y ()));
		}

		for (const auto & vector : crossSection ())
			points .emplace_back (Vector3f (vector .getX (), 0, vector .getY ()) * matrix);

	}

	// Copy points for caps

	if (hasCaps)
	{
		auto last = points .end () - crossSection () .size ();

		if (beginCap ())
		{
			for (const auto & point : basic::adapter (points .begin (), points .begin () + crossSection () .size ()))
				points .emplace_back (point);
		}

		if (endCap ())
		{
			for (const auto & point : basic::adapter (last, last + crossSection () .size ()))
				points .emplace_back (point);
		}
	}

	return points;
}

std::vector <Matrix4f>
Extrusion::createRotations ()
{
	std::vector <Matrix4f> rotations;
	rotations .reserve (spine () .size ());

	// calculate SCP rotations

	bool closedSpine = spine () .front () == spine () .back ();

	Vector3f SCPyAxisPrevious;
	Vector3f SCPzAxisPrevious;

	Vector3f SCPxAxis;
	Vector3f SCPyAxis;
	Vector3f SCPzAxis;

	// SCP for the first point:
	if (closedSpine)
	{
		SCPyAxis = normalize (spine () [1] - spine () [spine () .size () - 2]);
		SCPzAxis = normalize (cross (spine () [1] - spine () [0],
		                             spine () [spine () .size () - 2] - spine () [0]));
	}
	else
	{
		SCPyAxis = normalize (spine () [1] - spine () [0]);

		// Find first defined Z-axis.
		for (size_t i = 1, size = spine () .size () - 1; i < size; i ++)
		{
			SCPzAxis = normalize (cross (spine () [i + 1] - spine () [i],
			                             spine () [i - 1] - spine () [i]));

			if (SCPzAxis not_eq Vector3f ())
				break;
		}
	}

	// The entire spine is collinear:
	if (SCPzAxis == Vector3f ())
		SCPzAxis = Rotation4f (Vector3f (0, 1, 0), SCPyAxis) * Vector3f (0, 0, 1);

	SCPxAxis = cross (SCPyAxis, SCPzAxis);

	rotations .emplace_back (SCPxAxis .x (), SCPxAxis .y (), SCPxAxis .z (), 0,
	                         SCPyAxis .x (), SCPyAxis .y (), SCPyAxis .z (), 0,
	                         SCPzAxis .x (), SCPzAxis .y (), SCPzAxis .z (), 0,
	                         0,              0,              0,              1);

	// For all points other than the first or last:

	SCPzAxisPrevious = SCPzAxis;

	for (size_t i = 1, size = spine () .size () - 1; i < size; i ++)
	{
		SCPyAxis = normalize (spine () [i + 1] - spine () [i - 1]);
		SCPzAxis = normalize (cross (spine () [i + 1] - spine () [i],
		                             spine () [i - 1] - spine () [i]));

		// d.
		if (dot (SCPzAxisPrevious, SCPzAxis) < 0)
			SCPzAxis = -SCPzAxis;

		// The three points used in computing the Z-axis are collinear.
		if (SCPzAxis == Vector3f ())
			SCPzAxis = SCPzAxisPrevious;
		else
			SCPzAxisPrevious = SCPzAxis;

		SCPxAxis = cross (SCPyAxis, SCPzAxis);

		rotations .emplace_back (SCPxAxis .x (), SCPxAxis .y (), SCPxAxis .z (), 0,
		                         SCPyAxis .x (), SCPyAxis .y (), SCPyAxis .z (), 0,
		                         SCPzAxis .x (), SCPzAxis .y (), SCPzAxis .z (), 0,
		                         0,              0,              0,              1);
	}

	// SCP for the last point
	if (closedSpine)
	{
		// The SCP for the first and last points is the same.
		rotations .emplace_back (rotations .front ());
	}
	else
	{
		SCPyAxis = normalize (spine () [spine () .size () - 1] - spine () [spine () .size () - 2]);

		if (spine () .size () > 2)
			SCPzAxis = normalize (cross (spine () [spine () .size () - 1] - spine () [spine () .size () - 2],
			                             spine () [spine () .size () - 3] - spine () [spine () .size () - 2]));

		// d.
		if (dot (SCPzAxisPrevious, SCPzAxis) < 0)
			SCPzAxis = -SCPzAxis;

		// The three points used in computing the Z-axis are collinear.
		if (SCPzAxis == Vector3f ())
			SCPzAxis = SCPzAxisPrevious;

		SCPxAxis = cross (SCPyAxis, SCPzAxis);

		rotations .emplace_back (SCPxAxis .x (), SCPxAxis .y (), SCPxAxis .z (), 0,
		                         SCPyAxis .x (), SCPyAxis .y (), SCPyAxis .z (), 0,
		                         SCPzAxis .x (), SCPzAxis .y (), SCPzAxis .z (), 0,
		                         0,              0,              0,              1);
	}

	return rotations;
}

void
Extrusion::build ()
{
	if (spine () .size () < 2 or crossSection () .size () < 3)
		return;

	getTexCoord () .emplace_back ();

	size_t crossSectionSize = crossSection () .size (); // This one is used only in the INDEX macro.

	#define INDEX(n, k) ((n) * crossSectionSize + (k))

	bool closedSpine        = spine () .front () == spine () .back ();
	bool closedCrossSection = crossSection () .front () == crossSection () .back ();

	// For caps calculation

	Vector2f min = crossSection () [0];
	Vector2f max = crossSection () [0];

	for (size_t k = 1, size = crossSection () .size (); k < size; k ++)
	{
		min = math::min <float> (min, crossSection () [k]);
		max = math::max <float> (max, crossSection () [k]);
	}

	Vector2f capSize = max - min;

	bool hasCaps        = capSize .x () and capSize .y ();
	size_t numCapPoints = closedCrossSection ? crossSection () .size () - 1 : crossSection () .size ();

	// Create

	std::vector <Vector3f> points = std::move (createPoints (hasCaps));
	std::vector <size_t> coordIndex;
	NormalIndex normalIndex;

	size_t reserve = (spine () .size () - 1) * (crossSection () .size () - 1) * 6 + (beginCap () ? (numCapPoints - 2) * 3 : 0) + (endCap () ? (numCapPoints - 2) * 3 : 0);
	coordIndex .reserve (reserve);
	getTexCoord () [0] .reserve (reserve);
	getNormals  () .reserve (reserve);

	// Build body.

	for (size_t n = 0, size = spine () .size () - 1; n < size; ++ n)
	{
		for (size_t k = 0, size = crossSection () .size () - 1; k < size; ++ k)
		{
			size_t n1 = closedSpine and n == spine () .size () - 2 ? 0 : n + 1;
			size_t k1 = closedCrossSection and k == crossSection () .size () - 2 ? 0 : k + 1;

			// k      k+1
			//
			// p4 ----- p3   n+1
			//  |       |
			//  |       |
			//  |       |
			// p1 ----- p2   n

			// p1
			getTexCoord () [0] .emplace_back (k / (float) (crossSection () .size () - 1), n / (float) (spine () .size () - 1), 0, 1);
			coordIndex .emplace_back (INDEX (n, k));
			normalIndex [coordIndex .back ()] .emplace_back (getNormals () .size ());
			getNormals () .emplace_back (math::normal (points [INDEX (n1, k)], points [INDEX (n, k)], points [INDEX (n, k1)]));

			// p2
			getTexCoord () [0] .emplace_back ((k + 1) / (float) (crossSection () .size () - 1), n / (float) (spine () .size () - 1), 0, 1);
			coordIndex .emplace_back (INDEX (n, k1));
			normalIndex [coordIndex .back ()] .emplace_back (getNormals () .size ());
			getNormals () .emplace_back (math::normal (points [INDEX (n, k)], points [INDEX (n, k1)], points [INDEX (n1, k1)]));

			// p3
			getTexCoord () [0] .emplace_back ((k + 1) / (float) (crossSection () .size () - 1), (n + 1) / (float) (spine () .size () - 1), 0, 1);
			coordIndex .emplace_back (INDEX (n1, k1));
			normalIndex [coordIndex .back ()] .emplace_back (getNormals () .size ());
			getNormals () .emplace_back (math::normal (points [INDEX (n, k1)], points [INDEX (n1, k1)], points [INDEX (n1, k)]));

			// p4
			getTexCoord () [0] .emplace_back (k / (float) (crossSection () .size () - 1), (n + 1) / (float) (spine () .size () - 1), 0, 1);
			coordIndex .emplace_back (INDEX (n1, k));
			normalIndex [coordIndex .back ()] .emplace_back (getNormals () .size ());
			getNormals () .emplace_back (math::normal (points [INDEX (n1, k1)], points [INDEX (n1, k)], points [INDEX (n, k)]));
		}
	}

	// Refine normals and build vertices.

	refineNormals (normalIndex, getNormals (), creaseAngle (), ccw ());

	for (size_t i = 0, size = coordIndex .size (); i < size; i += 4)
	{
		getVertices () .emplace_back (points [coordIndex [i]]);
		getVertices () .emplace_back (points [coordIndex [i + 1]]);
		getVertices () .emplace_back (points [coordIndex [i + 2]]);
		getVertices () .emplace_back (points [coordIndex [i + 3]]);
	}

	addElements (GL_QUADS, getVertices () .size ());

	// Build caps

	if (hasCaps)
	{
		if (beginCap ())
		{
			size_t j = spine () .size ();

			if (convex ())
			{
				Vector3f normal = math::normal (points [INDEX (j, 2)],
				                                points [INDEX (j, 1)],
				                                points [INDEX (j, 0)]);

				for (size_t k = 0; k < numCapPoints; ++ k)
				{
					Vector2f t = (min + crossSection () [numCapPoints - 1 - k]) / capSize;
					getTexCoord () [0] .emplace_back (t .x (), 1 - t .y (), 0, 1);
					getNormals () .emplace_back (normal);
					getVertices () .emplace_back (points [INDEX (j, numCapPoints - 1 - k)]);
				}

				addElements (getVertexMode (numCapPoints), numCapPoints);
			}
			else
			{
				Tesselator tesselator;

				for (size_t k = 0; k < numCapPoints; ++ k)
					tesselator .add_vertex (points [INDEX (j, numCapPoints - 1 - k)], INDEX (j, numCapPoints - 1 - k), numCapPoints - 1 - k);

				tesselator .tesselate ();

				tesselateCap (tesselator, points, min, capSize);
			}
		}

		if (endCap ())
		{
			size_t j = spine () .size () + beginCap ();

			if (convex ())
			{
				Vector3f normal = math::normal (points [INDEX (j, 0)],
				                                points [INDEX (j, 1)],
				                                points [INDEX (j, 2)]);

				for (size_t k = 0; k < numCapPoints; ++ k)
				{
					Vector2f t = (min + crossSection () [k]) / capSize;
					getTexCoord () [0] .emplace_back (t .x (), 1 - t .y (), 0, 1);
					getNormals () .emplace_back (normal);
					getVertices () .emplace_back (points [INDEX (j, k)]);
				}

				addElements (getVertexMode (numCapPoints), numCapPoints);
			}
			else
			{
				Tesselator tesselator;

				for (size_t k = 0; k < numCapPoints; ++ k)
					tesselator .add_vertex (points [INDEX (j, k)], INDEX (j, k), k);

				tesselator .tesselate ();

				tesselateCap (tesselator, points, min, capSize);
			}
		}
	}

	setSolid (solid ());
	setCCW (ccw ());

	#undef INDEX
}

void
Extrusion::tesselateCap (const Tesselator & tesselator,
                         std::vector <Vector3f> & points,
                         const Vector2f & min,
                         const Vector2f & capSize)
{
	#define I 0
	#define K 1

	size_t size = getVertices () .size ();

	Vector3f normal;

	for (const auto & polygonElement : tesselator .polygon ())
	{
		switch (polygonElement .type ())
		{
			case GL_TRIANGLE_FAN :
				{
					for (size_t i = 1, size = polygonElement .size () - 1; i < size; ++ i)
					{
						normal += math::normal (points [std::get < I > (polygonElement [0] .data ())],
						                        points [std::get < I > (polygonElement [i] .data ())],
						                        points [std::get < I > (polygonElement [i + 1] .data ())]);
					}

					break;
				}
			case GL_TRIANGLE_STRIP:
			{
				for (size_t i = 0, size = polygonElement .size () - 2; i < size; ++ i)
				{
					normal += math::normal (points [std::get < I > (polygonElement [is_odd (i) ? i + 1 : i] .data ())],
					                        points [std::get < I > (polygonElement [is_odd (i) ? i : i + 1] .data ())],
					                        points [std::get < I > (polygonElement [i + 2] .data ())]);
				}

				break;
			}
			case GL_TRIANGLES:
			{
				for (size_t i = 0, size = polygonElement .size (); i < size; i += 3)
				{
					normal += math::normal (points [std::get < I > (polygonElement [i] .data ())],
					                        points [std::get < I > (polygonElement [i + 1] .data ())],
					                        points [std::get < I > (polygonElement [i + 2] .data ())]);
				}

				break;
			}
			default:
				break;
		}
	}

	normal = normalize (normal);

	for (const auto & polygonElement : tesselator .polygon ())
	{
		switch (polygonElement .type ())
		{
			case GL_TRIANGLE_FAN :
				{
					for (size_t i = 1, size = polygonElement .size () - 1; i < size; ++ i)
					{
						Vector2f t = (min + crossSection () [std::get < K > (polygonElement [0] .data ())]) / capSize;
						getTexCoord () [0] .emplace_back (t .x (), 1 - t .y (), 0, 1);
						getNormals () .emplace_back (normal);
						getVertices () .emplace_back (points [std::get < I > (polygonElement [0] .data ())]);

						t = (min + crossSection () [std::get < K > (polygonElement [i] .data ())]) / capSize;
						getTexCoord () [0] .emplace_back (t .x (), 1 - t .y (), 0, 1);
						getNormals () .emplace_back (normal);
						getVertices () .emplace_back (points [std::get < I > (polygonElement [i] .data ())]);

						t = (min + crossSection () [std::get < K > (polygonElement [i + 1] .data ())]) / capSize;
						getTexCoord () [0] .emplace_back (t .x (), 1 - t .y (), 0, 1);
						getNormals () .emplace_back (normal);
						getVertices () .emplace_back (points [std::get < I > (polygonElement [i + 1] .data ())]);
					}

					break;
				}
			case GL_TRIANGLE_STRIP:
			{
				for (size_t i = 0, size = polygonElement .size () - 2; i < size; ++ i)
				{
					Vector2f t = (min + crossSection () [std::get < K > (polygonElement [is_odd (i) ? i + 1 : i] .data ())]) / capSize;
					getTexCoord () [0] .emplace_back (t .x (), 1 - t .y (), 0, 1);
					getNormals () .emplace_back (normal);
					getVertices () .emplace_back (points [std::get < I > (polygonElement [is_odd (i) ? i + 1 : i] .data ())]);

					t = (min + crossSection () [std::get < K > (polygonElement [is_odd (i) ? i : i + 1] .data ())]) / capSize;
					getTexCoord () [0] .emplace_back (t .x (), 1 - t .y (), 0, 1);
					getNormals () .emplace_back (normal);
					getVertices () .emplace_back (points [std::get < I > (polygonElement [is_odd (i) ? i : i + 1] .data ())]);

					t = (min + crossSection () [std::get < K > (polygonElement [i + 2] .data ())]) / capSize;
					getTexCoord () [0] .emplace_back (t .x (), 1 - t .y (), 0, 1);
					getNormals () .emplace_back (normal);
					getVertices () .emplace_back (points [std::get < I > (polygonElement [i + 2] .data ())]);
				}

				break;
			}
			case GL_TRIANGLES:
			{
				for (size_t i = 0, size = polygonElement .size (); i < size; i += 3)
				{
					Vector2f t = (min + crossSection () [std::get < K > (polygonElement [i] .data ())]) / capSize;
					getTexCoord () [0] .emplace_back (t .x (), 1 - t .y (), 0, 1);
					getNormals () .emplace_back (normal);
					getVertices () .emplace_back (points [std::get < I > (polygonElement [i] .data ())]);

					t = (min + crossSection () [std::get < K > (polygonElement [i + 1] .data ())]) / capSize;
					getTexCoord () [0] .emplace_back (t .x (), 1 - t .y (), 0, 1);
					getNormals () .emplace_back (normal);
					getVertices () .emplace_back (points [std::get < I > (polygonElement [i + 1] .data ())]);

					t = (min + crossSection () [std::get < K > (polygonElement [i + 2] .data ())]) / capSize;
					getTexCoord () [0] .emplace_back (t .x (), 1 - t .y (), 0, 1);
					getNormals () .emplace_back (normal);
					getVertices () .emplace_back (points [std::get < I > (polygonElement [i + 2] .data ())]);
				}

				break;
			}
			default:
				break;
		}
	}

	addElements (GL_TRIANGLES, getVertices () .size () - size);

	#undef I
	#undef K
}

void
Extrusion::dispose ()
{
	X3DGeometryNode::dispose ();
}

} // X3D
} // titania
