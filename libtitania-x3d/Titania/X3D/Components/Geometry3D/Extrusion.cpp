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

#include "Extrusion.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Components/Geometry3D/IndexedFaceSet.h"
#include "../../Components/Rendering/Coordinate.h"
#include "../../Components/Texturing/TextureCoordinate.h"
#include "../../Execution/X3DExecutionContext.h"

#include <Titania/Math/Mesh/Tessellator.h>
#include <Titania/Utility/Range.h>

namespace titania {
namespace X3D {

const Component   Extrusion::component      = Component ("Geometry3D", 4);
const std::string Extrusion::typeName       = "Extrusion";
const std::string Extrusion::containerField = "geometry";

Extrusion::Fields::Fields () :
	set_crossSection (new MFVec2f ()),
	 set_orientation (new MFRotation ()),
	       set_scale (new MFVec2f ()),
	       set_spine (new MFVec3f ()),
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
	addType (X3DConstants::Extrusion);

	addField (inputOutput,    "metadata",         metadata ());

	addField (inputOnly,      "set_crossSection", set_crossSection ());
	addField (inputOnly,      "set_orientation",  set_orientation ());
	addField (inputOnly,      "set_scale",        set_scale ());
	addField (inputOnly,      "set_spine",        set_spine ());

	addField (initializeOnly, "beginCap",         beginCap ());
	addField (initializeOnly, "endCap",           endCap ());
	addField (initializeOnly, "solid",            solid ());
	addField (initializeOnly, "ccw",              ccw ());
	addField (initializeOnly, "convex",           convex ());
	addField (initializeOnly, "creaseAngle",      creaseAngle ());

	addField (initializeOnly, "crossSection",     crossSection ());
	addField (initializeOnly, "orientation",      orientation ());
	addField (initializeOnly, "scale",            scale ());
	addField (initializeOnly, "spine",            spine ());

	creaseAngle ()  .setUnit (UnitCategory::ANGLE);
	crossSection () .setUnit (UnitCategory::LENGTH);
	spine ()        .setUnit (UnitCategory::LENGTH);
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

	set_crossSection () .addInterest (crossSection ());
	set_orientation ()  .addInterest (orientation ());
	set_scale ()        .addInterest (scale ());
	set_spine ()        .addInterest (spine ());
}

bool
Extrusion::getClosedCrossSection () const
{
	return crossSection () .front () == crossSection () .back ();
}

bool
Extrusion::getClosedSpine () const
{
	return spine () .front () == spine () .back () and (orientation () .empty () or orientation () .front () == orientation () .back ());
}

std::vector <Vector3d>
Extrusion::createPoints (const bool hasCaps) const
{
	std::vector <Vector3d> points;

	// calculate SCP rotations

	auto rotations = createRotations ();

	// calculate vertices.

	for (size_t i = 0, size = spine () .size (); i < size; i ++)
	{
		auto & matrix = rotations [i];

		if (orientation () .size ())
			matrix .rotate (orientation () [std::min (i, orientation () .size () - 1)]);

		if (scale () .size ())
		{
			const Vector2f & s = scale () [std::min (i, scale () .size () - 1)];

			matrix .scale (Vector3f (s .x (), 1, s .y ()));
		}

		for (const auto & vector : crossSection ())
			points .emplace_back (Vector3d (vector .x (), 0, vector .y ()) * matrix);
	}

	// Copy points for caps

	if (hasCaps)
	{
		const auto last = points .end () - crossSection () .size ();

		if (beginCap ())
		{
			for (const auto & point : basic::make_range (points .cbegin (), crossSection () .size ()))
				points .emplace_back (point);
		}

		if (endCap ())
		{
			for (const auto & point : basic::make_range (last, crossSection () .size ()))
				points .emplace_back (point);
		}
	}

	return points;
}

std::vector <Matrix4d>
Extrusion::createRotations () const
{
	std::vector <Matrix4d> rotations;

	// calculate SCP rotations

	const bool closedSpine = getClosedSpine ();

	Vector3f SCPyAxisPrevious;
	Vector3f SCPzAxisPrevious;

	Vector3f SCPxAxis;
	Vector3f SCPyAxis;
	Vector3f SCPzAxis;

	// SCP for the first point:
	if (closedSpine)
	{
		// Find first defined Y-axis.
		for (size_t i = 1, size = spine () .size () - 2; i < size; i ++)
		{
			SCPyAxis = normalize (spine () [i] - spine () [size]);

			if (SCPyAxis not_eq Vector3f ())
				break;
		}

		// Find first defined Z-axis.
		for (size_t i = 0, size = spine () .size () - 2; i < size; i ++)
		{
			SCPzAxis = normalize (cross (spine () [i + 1] - spine () [i],
			                             spine () [size] - spine () [i]));

			if (SCPzAxis not_eq Vector3f ())
				break;
		}
	}
	else
	{
		// Find first defined Y-axis.
		for (size_t i = 0, size = spine () .size () - 1; i < size; i ++)
		{
			SCPyAxis = normalize (spine () [i + 1] - spine () [i]);

			if (SCPyAxis not_eq Vector3f ())
				break;
		}

		// Find first defined Z-axis.
		for (size_t i = 1, size = spine () .size () - 1; i < size; i ++)
		{
			SCPzAxis = normalize (cross (spine () [i + 1] - spine () [i],
			                             spine () [i - 1] - spine () [i]));

			if (SCPzAxis not_eq Vector3f ())
				break;
		}
	}

	// The entire spine is coincident:
	if (SCPyAxis == Vector3f ())
		SCPyAxis = Vector3f (0, 1, 0);

	// The entire spine is collinear:
	if (SCPzAxis == Vector3f ())
		SCPzAxis = Vector3f (0, 0, 1) * Rotation4f (Vector3f (0, 1, 0), SCPyAxis);

	// We do not have to normalize SCPxAxis, as SCPyAxis and SCPzAxis are orthogonal.
	SCPxAxis = cross (SCPyAxis, SCPzAxis);

	// Get first spine point.
	const auto s = spine () [0];

	rotations .emplace_back (SCPxAxis .x (), SCPxAxis .y (), SCPxAxis .z (), 0,
	                         SCPyAxis .x (), SCPyAxis .y (), SCPyAxis .z (), 0,
	                         SCPzAxis .x (), SCPzAxis .y (), SCPzAxis .z (), 0,
	                         s .x (),        s .y (),        s .z (),        1);

	// For all points other than the first or last:

	SCPyAxisPrevious = SCPyAxis;
	SCPzAxisPrevious = SCPzAxis;

	for (size_t i = 1, size = spine () .size () - 1; i < size; i ++)
	{
		const auto s = spine () [i];

		SCPyAxis = normalize (spine () [i + 1] - spine () [i - 1]);
		SCPzAxis = normalize (cross (spine () [i + 1] - s,
		                             spine () [i - 1] - s));

		// g.
		if (dot (SCPzAxisPrevious, SCPzAxis) < 0)
			SCPzAxis = -SCPzAxis;

		// The two points used in computing the Y-axis are coincident.
		if (SCPyAxis == Vector3f ())
			SCPyAxis = SCPyAxisPrevious;
		else
			SCPyAxisPrevious = SCPyAxis;

		// The three points used in computing the Z-axis are collinear.
		if (SCPzAxis == Vector3f ())
			SCPzAxis = SCPzAxisPrevious;
		else
			SCPzAxisPrevious = SCPzAxis;

		// We do not have to normalize SCPxAxis, as SCPyAxis and SCPzAxis are orthogonal.
		SCPxAxis = cross (SCPyAxis, SCPzAxis);

		rotations .emplace_back (SCPxAxis .x (), SCPxAxis .y (), SCPxAxis .z (), 0,
		                         SCPyAxis .x (), SCPyAxis .y (), SCPyAxis .z (), 0,
		                         SCPzAxis .x (), SCPzAxis .y (), SCPzAxis .z (), 0,
		                         s .x (),        s .y (),        s .z (),        1);
	}

	// SCP for the last point
	if (closedSpine)
	{
		// The SCP for the first and last points is the same.
		rotations .emplace_back (rotations .front ());
	}
	else
	{
		const auto s = spine () [spine () .size () - 1];

		SCPyAxis = normalize (s - spine () [spine () .size () - 2]);

		if (spine () .size () > 2)
			SCPzAxis = normalize (cross (s - spine () [spine () .size () - 2],
			                             spine () [spine () .size () - 3] - spine () [spine () .size () - 2]));

		// g.
		if (dot (SCPzAxisPrevious, SCPzAxis) < 0)
			SCPzAxis = -SCPzAxis;

		// The two points used in computing the Y-axis are coincident.
		if (SCPyAxis == Vector3f ())
			SCPyAxis = SCPyAxisPrevious;
		else
			SCPyAxisPrevious = SCPyAxis;

		// The three points used in computing the Z-axis are collinear.
		if (SCPzAxis == Vector3f ())
			SCPzAxis = SCPzAxisPrevious;

		// We do not have to normalize SCPxAxis, as SCPyAxis and SCPzAxis are orthogonal.
		SCPxAxis = cross (SCPyAxis, SCPzAxis);

		rotations .emplace_back (SCPxAxis .x (), SCPxAxis .y (), SCPxAxis .z (), 0,
		                         SCPyAxis .x (), SCPyAxis .y (), SCPyAxis .z (), 0,
		                         SCPzAxis .x (), SCPzAxis .y (), SCPzAxis .z (), 0,
		                         s .x (),        s .y (),        s .z (),        1);
	}

	return rotations;
}

void
Extrusion::build ()
{
	if (spine () .size () < 2 or crossSection () .size () < 2)
		return;

	getMultiTexCoords () .emplace_back ();

	const size_t crossSectionSize = crossSection () .size (); // This one is used only in the INDEX macro.

	#define INDEX(n, k) ((n) * crossSectionSize + (k))

	const bool closedSpine        = getClosedSpine ();
	const bool closedCrossSection = getClosedCrossSection ();

	// For caps calculation

	Vector2f min = crossSection () [0];
	Vector2f max = crossSection () [0];

	for (size_t k = 1, size = crossSection () .size (); k < size; k ++)
	{
		min = math::min <float> (min, crossSection () [k]);
		max = math::max <float> (max, crossSection () [k]);
	}

	const auto   capSize      = max - min;
	const auto   capMax       = std::max (capSize .x (), capSize .y ());
	const size_t numCapPoints = closedCrossSection ? crossSection () .size () - 1 : crossSection () .size ();

	// Create

	const auto points = createPoints (capMax);
	std::vector <size_t> coordIndex;
	NormalIndex normalIndex;

	// Build body.

	const float  numCrossSection_1 = crossSection () .size () - 1;
	const size_t numCrossSection_2 = crossSection () .size () - 2;
	const float  numSpine_1        = spine () .size () - 1;
	const size_t numSpine_2        = spine () .size () - 2;

	// Remember the most left and most right points for better normal generation.

	size_t indexLeft  = INDEX (0, 0);
	size_t indexRight = INDEX (0, closedCrossSection ? 0 : crossSection () .size () - 1);

	for (size_t n = 0, size = spine () .size () - 1; n < size; ++ n)
	{
		for (size_t k = 0, size = crossSection () .size () - 1; k < size; ++ k)
		{
			const size_t n1 = closedSpine        and n == numSpine_2        ? 0 : n + 1;
			const size_t k1 = closedCrossSection and k == numCrossSection_2 ? 0 : k + 1;

			// k      k+1
			//
			// p4 ----- p3   n+1
			//  |     / |
			//  |   /   |
			//  | /     |
			// p1 ----- p2   n

			auto       p1 = INDEX (n,  k);
			const auto p2 = INDEX (n,  k1);
			auto       p3 = INDEX (n1, k1);
			const auto p4 = INDEX (n1, k);

			const bool length1   = abs (points [p2] - points [p3]) >= 1e-7;
			const bool length2   = abs (points [p4] - points [p1]) >= 1e-7;
			const auto texCoord1 = Vector2f (      k / numCrossSection_1,       n / numSpine_1);
			const auto texCoord2 = Vector2f ((k + 1) / numCrossSection_1,       n / numSpine_1);
			const auto texCoord3 = Vector2f ((k + 1) / numCrossSection_1, (n + 1) / numSpine_1);
			const auto texCoord4 = Vector2f (      k / numCrossSection_1, (n + 1) / numSpine_1);
			const auto normal1   = Triangle3f (points [p1], points [p2], points [p3]) .normal ();
			const auto normal2   = Triangle3f (points [p1], points [p3], points [p4]) .normal ();

			// Merge points on the most left and most right side if spine is coincident for better normal generation.

			if (k == 0)
			{
				if (length2)
					indexLeft = p1;
				else
					p1 = indexLeft;
			}

			if (k == numCrossSection_2)
			{
				if (length1)
					indexRight = p2;
				else
					p3 = indexRight;
			}

			// If there are coincident spine points then one length can be zero.

			if (length1)
			{
				// p1
				if (length2)
				{
					getTexCoords () .emplace_back (texCoord1 .x (), texCoord1 .y (), 0, 1);
				}
				else
				{
					// Cone case on the right side:
					const auto texCoord = (texCoord1 + texCoord4) / 2.0f;

					getTexCoords () .emplace_back (texCoord .x (), texCoord .y (), 0, 1);
				}

				coordIndex .emplace_back (p1);
				normalIndex [coordIndex .back ()] .emplace_back (getNormals () .size ());
				getNormals () .emplace_back (normal1);

				// p2
				getTexCoords () .emplace_back (texCoord2 .x (), texCoord2 .y (), 0, 1);
				coordIndex .emplace_back (p2);
				normalIndex [coordIndex .back ()] .emplace_back (getNormals () .size ());
				getNormals () .emplace_back (normal1);

				// p3
				getTexCoords () .emplace_back (texCoord3 .x (), texCoord3 .y (), 0, 1);
				coordIndex .emplace_back (p3);
				normalIndex [coordIndex .back ()] .emplace_back (getNormals () .size ());
				getNormals () .emplace_back (normal1);
			}

			if (length2)
			{
				// p1
				getTexCoords () .emplace_back (texCoord1 .x (), texCoord1 .y (), 0, 1);
				coordIndex .emplace_back (p1);
				normalIndex [coordIndex .back ()] .emplace_back (getNormals () .size ());
				getNormals () .emplace_back (normal2);

				// p3
				if (length1)
					getTexCoords () .emplace_back (texCoord3 .x (), texCoord3 .y (), 0, 1);
				else
				{
					// Cone case on the left side:
					const auto texCoord = (texCoord3 + texCoord2) / 2.0f;

					getTexCoords () .emplace_back (texCoord .x (), texCoord .y (), 0, 1);
				}

				coordIndex .emplace_back (p3);
				normalIndex [coordIndex .back ()] .emplace_back (getNormals () .size ());
				getNormals () .emplace_back (normal2);

				// p4
				getTexCoords () .emplace_back (texCoord4 .x (), texCoord4 .y (), 0, 1);
				coordIndex .emplace_back (p4);
				normalIndex [coordIndex .back ()] .emplace_back (getNormals () .size ());
				getNormals () .emplace_back (normal2);
			}
		}
	}

	// Refine normals and build vertices.

	refineNormals (normalIndex, getNormals (), creaseAngle (), ccw ());

	for (size_t i = 0, size = coordIndex .size (); i < size; ++ i)
		getVertices () .emplace_back (points [coordIndex [i]]);

	// Build caps

	if (capMax and numCapPoints > 2)
	{
		if (beginCap ())
		{
			const size_t j = spine () .size ();

			if (convex ())
			{
				auto texCoords = std::vector <Vector4f> ();
				auto capPoints = std::vector <Vector3d> ();

				for (size_t k = 0; k < numCapPoints; ++ k)
				{
					const auto t = (crossSection () .get1Value (numCapPoints - 1 - k) - min) / capMax;

					texCoords .emplace_back (t .x (), t .y (), 0, 1);
					capPoints .emplace_back (points [INDEX (j, numCapPoints - 1 - k)]);
				}

				auto normal = Triangle3f (points [INDEX (j, 2)], points [INDEX (j, 1)], points [INDEX (j, 0)]) .normal ();

				if (not ccw ())
					normal .negate ();

				const auto & t0 = texCoords [0];
				const auto & p0 = capPoints [0];

				for (size_t i = 1, size = capPoints .size () - 1; i < size; ++ i)
				{
					const auto & t1 = texCoords [i];
					const auto & t2 = texCoords [i + 1];
					const auto & p1 = capPoints [i];
					const auto & p2 = capPoints [i + 1];

					getTexCoords () .emplace_back (t0);
					getNormals ()   .emplace_back (normal);
					getVertices ()  .emplace_back (p0);

					getTexCoords () .emplace_back (t1);
					getNormals ()   .emplace_back (normal);
					getVertices ()  .emplace_back (p1);

					getTexCoords () .emplace_back (t2);
					getNormals ()   .emplace_back (normal);
					getVertices ()  .emplace_back (p2);
				}
			}
			else
			{
				Tessellator tessellator;
				tessellator .begin_polygon ();
				tessellator .begin_contour ();

				for (size_t k = 0; k < numCapPoints; ++ k)
					tessellator .add_vertex (points [INDEX (j, numCapPoints - 1 - k)], INDEX (j, numCapPoints - 1 - k), numCapPoints - 1 - k);

				tessellator .end_contour ();
				tessellator .end_polygon ();

				tessellateCap (tessellator, points, min, capMax);
			}
		}

		if (endCap ())
		{
			const size_t j = spine () .size () + beginCap ();

			if (convex ())
			{
				auto texCoords = std::vector <Vector4f> ();
				auto capPoints = std::vector <Vector3d> ();

				for (size_t k = 0; k < numCapPoints; ++ k)
				{
					const auto t = (crossSection () .get1Value (k) - min) / capMax;

					texCoords .emplace_back (t .x (), t .y (), 0, 1);
					capPoints .emplace_back (points [INDEX (j, k)]);
				}

				auto normal = Triangle3f (points [INDEX (j, 0)], points [INDEX (j, 1)], points [INDEX (j, 2)]) .normal ();

				if (not ccw ())
					normal .negate ();

				const auto & t0 = texCoords [0];
				const auto & p0 = capPoints [0];

				for (size_t i = 1, size = capPoints .size () - 1; i < size; ++ i)
				{
					const auto & t1 = texCoords [i];
					const auto & t2 = texCoords [i + 1];
					const auto & p1 = capPoints [i];
					const auto & p2 = capPoints [i + 1];

					getTexCoords () .emplace_back (t0);
					getNormals ()   .emplace_back (normal);
					getVertices ()  .emplace_back (p0);

					getTexCoords () .emplace_back (t1);
					getNormals ()   .emplace_back (normal);
					getVertices ()  .emplace_back (p1);

					getTexCoords () .emplace_back (t2);
					getNormals ()   .emplace_back (normal);
					getVertices ()  .emplace_back (p2);
				}
			}
			else
			{
				Tessellator tessellator;
				tessellator .begin_polygon ();
				tessellator .begin_contour ();

				for (size_t k = 0; k < numCapPoints; ++ k)
					tessellator .add_vertex (points [INDEX (j, k)], INDEX (j, k), k);

				tessellator .end_contour ();
				tessellator .end_polygon ();

				tessellateCap (tessellator, points, min, capMax);
			}
		}
	}

	addElements (GL_TRIANGLES, getVertices () .size ());
	setSolid (solid ());
	setCCW (ccw ());

	#undef INDEX
}

void
Extrusion::tessellateCap (const Tessellator & tessellator,
                          const std::vector <Vector3d> & points,
                          const Vector2f & min,
                          const float capMax)
{
	static constexpr size_t I = 0;
	static constexpr size_t K = 1;

	Vector3f normal;

	for (const auto & polygon : tessellator .polygons ())
	{
		switch (polygon .type ())
		{
			case GL_TRIANGLE_FAN:
			{
				for (size_t i = 1, size = polygon .size () - 1; i < size; ++ i)
				{
					normal += Triangle3f (points [std::get <I> (polygon [0] .data ())],
					                      points [std::get <I> (polygon [i] .data ())],
					                      points [std::get <I> (polygon [i + 1] .data ())]) .normal ();
				}

				break;
			}
			case GL_TRIANGLE_STRIP:
			{
				for (size_t i = 0, size = polygon .size () - 2; i < size; ++ i)
				{
					normal += Triangle3f (points [std::get <I> (polygon [is_odd (i) ? i + 1 : i] .data ())],
					                      points [std::get <I> (polygon [is_odd (i) ? i : i + 1] .data ())],
					                      points [std::get <I> (polygon [i + 2] .data ())]) .normal ();
				}

				break;
			}
			case GL_TRIANGLES:
			{
				for (size_t i = 0, size = polygon .size (); i < size; i += 3)
				{
					normal += Triangle3f (points [std::get <I> (polygon [i] .data ())],
					                      points [std::get <I> (polygon [i + 1] .data ())],
					                      points [std::get <I> (polygon [i + 2] .data ())]) .normal ();
				}

				break;
			}
			default:
				break;
		}
	}

	normal .normalize ();

	if (not ccw ())
		normal .negate ();

	for (const auto & polygon : tessellator .polygons ())
	{
		switch (polygon .type ())
		{
			case GL_TRIANGLE_FAN:
			{
				for (size_t i = 1, size = polygon .size () - 1; i < size; ++ i)
				{
					Vector2f t = (crossSection () .get1Value (std::get <K> (polygon [0] .data ())) - min) / capMax;
					getTexCoords () .emplace_back (t .x (), t .y (), 0, 1);
					getNormals   () .emplace_back (normal);
					getVertices  () .emplace_back (points [std::get <I> (polygon [0] .data ())]);

					t = (crossSection () .get1Value (std::get <K> (polygon [i] .data ())) - min) / capMax;
					getTexCoords () .emplace_back (t .x (), t .y (), 0, 1);
					getNormals   () .emplace_back (normal);
					getVertices  () .emplace_back (points [std::get <I> (polygon [i] .data ())]);

					t = (crossSection () .get1Value (std::get <K> (polygon [i + 1] .data ())) - min) / capMax;
					getTexCoords () .emplace_back (t .x (), t .y (), 0, 1);
					getNormals   () .emplace_back (normal);
					getVertices  () .emplace_back (points [std::get <I> (polygon [i + 1] .data ())]);
				}

				break;
			}
			case GL_TRIANGLE_STRIP:
			{
				for (size_t i = 0, size = polygon .size () - 2; i < size; ++ i)
				{
					Vector2f t = (crossSection () .get1Value (std::get <K> (polygon [is_odd (i) ? i + 1 : i] .data ())) - min) / capMax;
					getTexCoords () .emplace_back (t .x (), t .y (), 0, 1);
					getNormals   () .emplace_back (normal);
					getVertices  () .emplace_back (points [std::get <I> (polygon [is_odd (i) ? i + 1 : i] .data ())]);

					t = (crossSection () .get1Value (std::get <K> (polygon [is_odd (i) ? i : i + 1] .data ())) - min) / capMax;
					getTexCoords () .emplace_back (t .x (), t .y (), 0, 1);
					getNormals   () .emplace_back (normal);
					getVertices  () .emplace_back (points [std::get <I> (polygon [is_odd (i) ? i : i + 1] .data ())]);

					t = (crossSection () .get1Value (std::get <K> (polygon [i + 2] .data ())) - min) / capMax;
					getTexCoords () .emplace_back (t .x (), t .y (), 0, 1);
					getNormals   () .emplace_back (normal);
					getVertices  () .emplace_back (points [std::get <I> (polygon [i + 2] .data ())]);
				}

				break;
			}
			case GL_TRIANGLES:
			{
				for (size_t i = 0, size = polygon .size (); i < size; i += 3)
				{
					Vector2f t = (crossSection () .get1Value (std::get <K> (polygon [i] .data ())) - min) / capMax;
					getTexCoords () .emplace_back (t .x (), t .y (), 0, 1);
					getNormals   () .emplace_back (normal);
					getVertices  () .emplace_back (points [std::get <I> (polygon [i] .data ())]);

					t = (crossSection () .get1Value (std::get <K> (polygon [i + 1] .data ())) - min) / capMax;
					getTexCoords () .emplace_back (t .x (), t .y (), 0, 1);
					getNormals   () .emplace_back (normal);
					getVertices  () .emplace_back (points [std::get <I> (polygon [i + 1] .data ())]);

					t = (crossSection () .get1Value (std::get <K> (polygon [i + 2] .data ())) - min) / capMax;
					getTexCoords () .emplace_back (t .x (), t .y (), 0, 1);
					getNormals   () .emplace_back (normal);
					getVertices  () .emplace_back (points [std::get <I> (polygon [i + 2] .data ())]);
				}

				break;
			}
			default:
				break;
		}
	}
}

SFNode
Extrusion::toPrimitive () const
{
	const auto texCoord = getExecutionContext () -> createNode <TextureCoordinate> ();
	const auto coord    = getExecutionContext () -> createNode <Coordinate> ();
	const auto geometry = getExecutionContext () -> createNode <IndexedFaceSet> ();

	geometry -> solid ()       = solid ();
	geometry -> ccw ()         = ccw ();
	geometry -> convex ()      = convex ();
	geometry -> creaseAngle () = creaseAngle ();
	geometry -> texCoord ()    = texCoord;
	geometry -> coord ()       = coord;

	// Build geometry.

	const size_t crossSectionSize = crossSection () .size (); // This one is used only in the INDEX macro.

	#define INDEX(n, k) ((n) * crossSectionSize + (k))

	const bool closedSpine        = getClosedSpine ();
	const bool closedCrossSection = getClosedCrossSection ();

	// For caps calculation

	Vector2f min = crossSection () [0];
	Vector2f max = crossSection () [0];

	for (size_t k = 1, size = crossSection () .size (); k < size; k ++)
	{
		min = math::min <float> (min, crossSection () [k]);
		max = math::max <float> (max, crossSection () [k]);
	}

	const auto   capSize      = max - min;
	const auto   capMax       = std::max (capSize .x (), capSize .y ());
	const size_t numCapPoints = closedCrossSection ? crossSection () .size () - 1 : crossSection () .size ();

	// Create

	const auto points = createPoints (capMax);

	coord -> point () .assign (points .cbegin (), points .cend ());

	// Build body.

	const float numCrossSection_1 = crossSection () .size () - 1;
	const float numSpine_1        = spine () .size () - 1;

	size_t indexLeft  = INDEX (0, 0);
	size_t indexRight = INDEX (0, closedCrossSection ? 0 : crossSection () .size () - 1);

	for (size_t n = 0, size = spine () .size () - 1; n < size; ++ n)
	{
		for (size_t k = 0, size = crossSection () .size () - 1; k < size; ++ k)
		{
			const size_t n1 = closedSpine        and n == spine ()        .size () - 2 ? 0 : n + 1;
			const size_t k1 = closedCrossSection and k == crossSection () .size () - 2 ? 0 : k + 1;

			// k      k+1
			//
			// p4 ----- p3   n+1
			//  |     / |
			//  |   /   |
			//  | /     |
			// p1 ----- p2   n

			auto       p1 = INDEX (n,  k);
			const auto p2 = INDEX (n,  k1);
			auto       p3 = INDEX (n1, k1);
			const auto p4 = INDEX (n1, k);

			const auto t1 = INDEX (n,     k);
			const auto t2 = INDEX (n,     k + 1);
			const auto t3 = INDEX (n + 1, k + 1);
			const auto t4 = INDEX (n + 1, k);

			const auto texCoord1 = Vector2f (      k / numCrossSection_1,       n / numSpine_1);
			const auto texCoord2 = Vector2f ((k + 1) / numCrossSection_1,       n / numSpine_1);
			const auto texCoord3 = Vector2f ((k + 1) / numCrossSection_1, (n + 1) / numSpine_1);
			const auto texCoord4 = Vector2f (      k / numCrossSection_1, (n + 1) / numSpine_1);

			const bool length1 = abs (points [p2] - points [p3]) >= 1e-7;
			const bool length2 = abs (points [p4] - points [p1]) >= 1e-7;

			// Merge points on the left and right side if spine is coincident for better normal generation.

			if (k == 0)
			{
				if (length2)
					indexLeft = p1;
				else
					p1 = indexLeft;
			}

			if (k == crossSection () .size () - 2)
			{
				if (length1)
					indexRight = p2;
				else
					p3 = indexRight;
			}

			// Add a Texture coordinate point for p1.

			if (length2)
				texCoord -> point () .emplace_back (texCoord1);
			else
				// Cone case:
				texCoord -> point () .emplace_back ((texCoord1 + texCoord4) / 2.0f);

			// Add another Texture coordinate point for p2 if we are on the right side.

			if (k == crossSection () .size () - 2)
			{
				if (length1)
					texCoord -> point () .emplace_back (texCoord2);
				else
					// Cone case:
					texCoord -> point () .emplace_back ((texCoord2 + texCoord3) / 2.0f);
			}

			// If there are coincident spine points then one length can be zero.

			if (length1)
			{
				// p1
				geometry -> texCoordIndex () .emplace_back (t1);
				geometry -> coordIndex ()    .emplace_back (p1);

				// p2
				geometry -> texCoordIndex () .emplace_back (t2);
				geometry -> coordIndex ()    .emplace_back (p2);

				// p3
				geometry -> texCoordIndex () .emplace_back (t3);
				geometry -> coordIndex ()    .emplace_back (p3);

				// Close polygon.
				geometry -> texCoordIndex () .emplace_back (-1);
				geometry -> coordIndex ()    .emplace_back (-1);
			}

			if (length2)
			{
				// p1
				geometry -> texCoordIndex () .emplace_back (t1);
				geometry -> coordIndex ()    .emplace_back (p1);

				// p3
				geometry -> texCoordIndex () .emplace_back (length1 ? t3 : t2);
				geometry -> coordIndex ()    .emplace_back (p3);

				// p4
				geometry -> texCoordIndex () .emplace_back (t4);
				geometry -> coordIndex ()    .emplace_back (p4);

				// Close polygon.
				geometry -> texCoordIndex () .emplace_back (-1);
				geometry -> coordIndex ()    .emplace_back (-1);
			}
		}
	}

	// Add TextureCoordinate points for the top cross section.
	{
		const auto n = spine () .size () - 2;

		for (size_t k = 0, size = crossSection () .size () - 1; k < size; ++ k)
		{
			const auto texCoord3 = Vector2f ((k + 1) / numCrossSection_1, (n + 1) / numSpine_1);
			const auto texCoord4 = Vector2f (      k / numCrossSection_1, (n + 1) / numSpine_1);

			// Add a Texture coordinate point for p4.

			texCoord -> point () .emplace_back (texCoord4);

			// Add another Texture coordinate point for p3 if we are on the right side.

			if (k == crossSection () .size () - 2)
				texCoord -> point () .emplace_back (texCoord3);
		}
	}

	// Build caps

	if (capMax and numCapPoints > 2)
	{
		if (beginCap ())
		{
			const size_t j = spine () .size ();

			for (size_t k = 0; k < numCapPoints; ++ k)
			{
				geometry -> texCoordIndex () .emplace_back (texCoord -> point () .size ());
				geometry -> coordIndex ()    .emplace_back (INDEX (j, numCapPoints - 1 - k));

				texCoord -> point () .emplace_back ((crossSection () [numCapPoints - 1 - k] - min) / capMax);
			}

			// Close polygon.
			geometry -> texCoordIndex () .emplace_back (-1);
			geometry -> coordIndex ()    .emplace_back (-1);
		}

		if (endCap ())
		{
			const size_t j = spine () .size () + beginCap ();

			for (size_t k = 0; k < numCapPoints; ++ k)
			{
				geometry -> texCoordIndex () .emplace_back (texCoord -> point () .size ());
				geometry -> coordIndex ()    .emplace_back (INDEX (j, k));

				texCoord -> point () .emplace_back ((crossSection () [k] - min) / capMax);
			}

			// Close polygon.
			geometry -> texCoordIndex () .emplace_back (-1);
			geometry -> coordIndex ()    .emplace_back (-1);
		}
	}

	#undef INDEX

	getExecutionContext () -> realize ();

	// Remove unused points from Coordinate and rebuild indices.
	geometry -> rebuildTexCoord ();
	geometry -> rebuildCoord ();

	return geometry;
}

} // X3D
} // titania
