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

#include "Disk2D.h"

#include "../../Browser/Geometry2D/Disk2DOptions.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Components/Geometry3D/IndexedFaceSet.h"
#include "../../Components/Rendering/Coordinate.h"
#include "../../Components/Rendering/IndexedLineSet.h"
#include "../../Components/Rendering/PointSet.h"
#include "../../Components/Texturing/TextureCoordinate.h"
#include "../../Execution/X3DExecutionContext.h"

#include "../../Tools/Geometry2D/Disk2DTool.h"

namespace titania {
namespace X3D {

const ComponentType Disk2D::component      = ComponentType::GEOMETRY_2D;
const std::string   Disk2D::typeName       = "Disk2D";
const std::string   Disk2D::containerField = "geometry";

Disk2D::Fields::Fields () :
	innerRadius (new SFFloat ()),
	outerRadius (new SFFloat (1)),
	      solid (new SFBool (true))
{ }

Disk2D::Disk2D (X3DExecutionContext* const executionContext) :
	        X3DBaseNode (executionContext -> getBrowser (), executionContext),
	    X3DGeometryNode (),
	X3DLineGeometryNode (),
	             fields ()
{
	addType (X3DConstants::Disk2D);

	addField (inputOutput,    "metadata",    metadata ());
	addField (inputOutput,    "innerRadius", innerRadius ());
	addField (inputOutput,    "outerRadius", outerRadius ());
	addField (initializeOnly, "solid",       solid ());

	innerRadius () .setUnit (UnitCategory::LENGTH);
	outerRadius () .setUnit (UnitCategory::LENGTH);
}

X3DBaseNode*
Disk2D::create (X3DExecutionContext* const executionContext) const
{
	return new Disk2D (executionContext);
}

void
Disk2D::initialize ()
{
	using E = void (X3DBaseNode::*) ();

	X3DGeometryNode::initialize ();

	getBrowser () -> getDisk2DOptions () .addInterest ((E) &Disk2D::addEvent, this);
}

void
Disk2D::setExecutionContext (X3DExecutionContext* const executionContext)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	using E = void (X3DBaseNode::*) ();

	if (isInitialized ())
		getBrowser () -> getDisk2DOptions () .removeInterest ((E) &Disk2D::addEvent, this);

	X3DGeometryNode::setExecutionContext (executionContext);

	if (isInitialized ())
		getBrowser () -> getDisk2DOptions () .addInterest ((E) &Disk2D::addEvent, this);
}

const X3DPtr <ComposedShader> &
Disk2D::getShaderNode (X3DBrowser* const browser)
{
	return browser -> getWireframeShader ();
}

Box3d
Disk2D::createBBox () const
{
	const auto diameter = std::abs (std::max (innerRadius (), outerRadius ())) * 2;

	return Box3d (Vector3d (diameter, diameter, 0), Vector3d ());
}

void
Disk2D::build ()
{
	const auto & options = getBrowser () -> getDisk2DOptions ();

	if (innerRadius () == outerRadius ())
	{
		const double radius = std::abs (outerRadius ());

		// Point

		if (radius == 0)
		{
			getVertices () .emplace_back (0, 0, 0);
			addElements (GL_POINTS, getVertices () .size ());
			setGeometryType (GeometryType::GEOMETRY_POINTS);
			setSolid (false);
			return;
		}

		// Circle

		if (radius == 1)
			getVertices () = options -> getVertices ();

		else
		{
			getVertices () .reserve (options -> getVertices () .size ());

			for (const auto & vertex : options -> getVertices ())
				getVertices () .emplace_back (vertex * radius);
		}

		addElements (GL_LINE_LOOP, getVertices () .size ());
		setGeometryType (GeometryType::GEOMETRY_LINES);
		setSolid (false);
		return;
	}

	if (innerRadius () == 0.0f or outerRadius () == 0.0f)
	{
		// Disk

		const double radius   = std::abs (std::max (innerRadius (), outerRadius ()));
		const size_t elements = solid () ? 1 : 2;

		getTexCoords () .emplace_back ();
		getTexCoords () [0] .reserve (elements * options -> getTexCoords () .size ());
		getTexCoords () [0] = options -> getTexCoords ();

		getNormals () .reserve (elements * options -> getNormals  () .size ());
		getNormals () = options -> getNormals  ();

		getVertices () .reserve (elements * options -> getVertices () .size ());

		if (radius == 1)
			getVertices () = options -> getVertices ();

		else
		{
			getVertices () .reserve (options -> getVertices () .size ());

			for (const auto & vertex : options -> getVertices ())
				getVertices () .emplace_back (vertex * radius);
		}

		addElements (GL_POLYGON, getVertices () .size ());
		setGeometryType (GeometryType::GEOMETRY_2D);
		setSolid (solid ());
		addMirrorVertices (options -> getVertexMode (), true);
		return;
	}

	// Disk with hole

	const size_t elements = solid () ? 1 : 2;

	getTexCoords () .emplace_back ();
	getTexCoords () [0] .reserve (elements * (options -> getTexCoords () .size () + 2));

	getNormals  () .reserve (elements * (options -> getNormals  () .size () + 2));
	getVertices () .reserve (elements * (options -> getVertices () .size () + 2));

	// Texture Coordinates

	const auto & texCoords = options -> getTexCoords ();
	const auto & normals   = options -> getNormals ();
	const auto & vertices  = options -> getVertices ();
	const double maxRadius = std::abs (std::max (innerRadius (), outerRadius ()));
	const double minRadius = std::abs (std::min (innerRadius (), outerRadius ()));
	const double scale     = minRadius / maxRadius;

	for (size_t i = 0, size = options -> getVertices () .size (); i < size; ++ i)
	{
		const auto i1 = (i + 1) % size;

		// TexCoords
	
		getTexCoords () [0] .emplace_back (texCoords [i] .x () * scale + (1 - scale) / 2, texCoords [i] .y () * scale + (1 - scale) / 2, 0, 1);
		getTexCoords () [0] .emplace_back (texCoords [i]);
		getTexCoords () [0] .emplace_back (texCoords [i1]);
		getTexCoords () [0] .emplace_back (texCoords [i1] .x () * scale + (1 - scale) / 2, texCoords [i1] .y () * scale + (1 - scale) / 2, 0, 1);

		// Normals
	
		getNormals () .emplace_back (normals [i]);
		getNormals () .emplace_back (normals [i]);
		getNormals () .emplace_back (normals [i1]);
		getNormals () .emplace_back (normals [i1]);

		// Vertices

		getVertices () .emplace_back (vertices [i]  * minRadius);
		getVertices () .emplace_back (vertices [i]  * maxRadius);
		getVertices () .emplace_back (vertices [i1] * maxRadius);
		getVertices () .emplace_back (vertices [i1] * minRadius);
	}

	addElements (GL_QUADS, getVertices () .size ());
	setGeometryType (GeometryType::GEOMETRY_2D);
	setSolid (solid ());
	addMirrorVertices (GL_QUADS, true);
}

bool
Disk2D::intersects (Line3d line,
                    const ClipPlaneContainerArray & clipPlanes,
                    Matrix4d modelViewMatrix,
                    std::vector <IntersectionPtr> & intersections) const
{
	if (innerRadius () == outerRadius ())
		return X3DLineGeometryNode::intersects (line, clipPlanes, modelViewMatrix, intersections);

	return X3DGeometryNode::intersects (line, clipPlanes, modelViewMatrix, intersections);
}

bool
Disk2D::intersects (Box3d box,
                    const ClipPlaneContainerArray & clipPlanes,
                    Matrix4d modelViewMatrix) const
{
	if (innerRadius () == outerRadius ())
		return X3DLineGeometryNode::intersects (box, clipPlanes, modelViewMatrix);

	return X3DGeometryNode::intersects (box, clipPlanes, modelViewMatrix);
}

std::vector <Vector3d>
Disk2D::intersects (X3DRenderObject* const renderObject,
                    const std::shared_ptr <FrameBuffer> & frameBuffer,
                    const std::shared_ptr <FrameBuffer> & depthBuffer,
                    std::vector <IntersectionPtr> & intersections)
{
	if (innerRadius () == outerRadius ())
		return X3DLineGeometryNode::intersects (renderObject, frameBuffer, depthBuffer, intersections);

	return X3DGeometryNode::intersects (renderObject, frameBuffer, depthBuffer, intersections);
}

void
Disk2D::depth (const X3DShapeContainer* const context)
{ }

void
Disk2D::draw (ShapeContainer* const context)
{
	if (innerRadius () == outerRadius ())
		X3DLineGeometryNode::draw (context);
	else
		X3DGeometryNode::draw (context);
}

NodeType
Disk2D::getPrimitiveType () const
throw (Error <NOT_SUPPORTED>,
       Error <DISPOSED>)
{
	if (getElements () [0] .vertexMode () == GL_POINTS)
		return X3DConstants::PointSet;

	if (getElements () [0] .vertexMode () == GL_LINE_LOOP)
		return X3DConstants::IndexedLineSet;

	return X3DConstants::IndexedFaceSet;
}

SFNode
Disk2D::toPrimitive () const
throw (Error <NOT_SUPPORTED>,
       Error <DISPOSED>)
{
	if (getElements () .empty ())
		throw Error <DISPOSED> ("Disk2D::toPrimitive");

	if (getElements () [0] .vertexMode () == GL_POINTS)
	{
		// Point
	
		const auto coord    = getExecutionContext () -> createNode <Coordinate> ();
		const auto geometry = getExecutionContext () -> createNode <PointSet> ();

		geometry -> metadata () = metadata ();
		geometry -> coord ()    = coord;

		coord -> point () .assign (getVertices () .begin (), getVertices () .end ());

		getExecutionContext () -> realize ();
		return SFNode (geometry);
	}

	if (getElements () [0] .vertexMode () == GL_LINE_LOOP)
	{
		// Circle
	
		const auto coord    = getExecutionContext () -> createNode <Coordinate> ();
		const auto geometry = getExecutionContext () -> createNode <IndexedLineSet> ();

		geometry -> metadata () = metadata ();
		geometry -> coord ()    = coord;

		coord -> point () .assign (getVertices () .begin (), getVertices () .end ());

		for (int32_t i = 0, size = getVertices () .size (); i < size; ++ i)
			geometry -> coordIndex () .emplace_back (i);

		geometry -> coordIndex () .emplace_back (0);
		geometry -> coordIndex () .emplace_back (-1);

		getExecutionContext () -> realize ();
		return SFNode (geometry);
	}

	const auto texCoord = getExecutionContext () -> createNode <TextureCoordinate> ();
	const auto coord    = getExecutionContext () -> createNode <Coordinate> ();
	const auto geometry = getExecutionContext () -> createNode <IndexedFaceSet> ();

	geometry -> metadata () = metadata ();
	geometry -> texCoord () = texCoord;
	geometry -> coord ()    = coord;
		
	if (getElements () [0] .vertexMode () == GL_POLYGON)
	{
		// Disk
	
		coord -> point () .assign (getVertices () .begin (), getVertices () .begin () + getElements () [0] .count ());

		for (int32_t i = 0, size = getElements () [0] .count (); i < size; ++ i)
		{
			texCoord -> point () .emplace_back (getTexCoords () [0] [i] .x (), getTexCoords () [0] [i] .y ());
			geometry -> texCoordIndex () .emplace_back (i);
			geometry -> coordIndex ()    .emplace_back (i);
		}

		geometry -> texCoordIndex () .emplace_back (-1);
		geometry -> coordIndex ()    .emplace_back (-1);
		
		if (not solid ())
		{
			for (int32_t i = 1, size = getElements () [0] .count (); i < size; ++ i)
			{
				texCoord -> point () .emplace_back (1 - texCoord -> point () [size - i] .getX (), texCoord -> point () [size - i] .getY ());
				geometry -> texCoordIndex () .emplace_back (i - 1 + getElements () [0] .count ());
				geometry -> coordIndex ()    .emplace_back (size - i);
			}

			texCoord -> point () .emplace_back (1 - texCoord -> point () [0] .getX (), texCoord -> point () [0] .getY ());
			geometry -> texCoordIndex () .emplace_back (getElements () [0] .count ());
			geometry -> coordIndex ()    .emplace_back (0);
			geometry -> texCoordIndex () .emplace_back (-1);
			geometry -> coordIndex ()    .emplace_back (-1);
		}
	}
	else
	{
		// Disk with hole

		coord -> point () .assign (getVertices () .begin (), getVertices () .begin () + getElements () [0] .count () - 2);

		for (const auto & point : basic::make_range (getTexCoords () [0] .begin (), getElements () [0] .count ()))
			texCoord -> point () .emplace_back (point .x (), point .y ());

		int32_t       i    = 0;
		const int32_t size = getElements () [0] .count () - 4;

		for (; i < size; i += 2)
		{
			geometry -> texCoordIndex () .emplace_back (i);
			geometry -> texCoordIndex () .emplace_back (i + 1);
			geometry -> texCoordIndex () .emplace_back (i + 3);
			geometry -> texCoordIndex () .emplace_back (i + 2);
			geometry -> texCoordIndex () .emplace_back (-1);

			geometry -> coordIndex () .emplace_back (i);
			geometry -> coordIndex () .emplace_back (i + 1);
			geometry -> coordIndex () .emplace_back (i + 3);
			geometry -> coordIndex () .emplace_back (i + 2);
			geometry -> coordIndex () .emplace_back (-1);
		}

		geometry -> texCoordIndex () .emplace_back (i);
		geometry -> texCoordIndex () .emplace_back (i + 1);
		geometry -> texCoordIndex () .emplace_back (i + 3);
		geometry -> texCoordIndex () .emplace_back (i + 2);
		geometry -> texCoordIndex () .emplace_back (-1);

		geometry -> coordIndex () .emplace_back (i);
		geometry -> coordIndex () .emplace_back (i + 1);
		geometry -> coordIndex () .emplace_back (1);
		geometry -> coordIndex () .emplace_back (0);
		geometry -> coordIndex () .emplace_back (-1);

		if (not solid ())
		{
			const int32_t ts = texCoord -> point () .size ();

			for (const auto & point : basic::make_range (getTexCoords () [0] .begin (), getElements () [0] .count ()))
				texCoord -> point () .emplace_back (1 - point .x (), point .y ());

			i = 0;

			for (; i < size; i += 2)
			{
				geometry -> texCoordIndex () .emplace_back (ts + i);
				geometry -> texCoordIndex () .emplace_back (ts + i + 2);
				geometry -> texCoordIndex () .emplace_back (ts + i + 3);
				geometry -> texCoordIndex () .emplace_back (ts + i + 1);
				geometry -> texCoordIndex () .emplace_back (-1);

				geometry -> coordIndex () .emplace_back (i);
				geometry -> coordIndex () .emplace_back (i + 2);
				geometry -> coordIndex () .emplace_back (i + 3);
				geometry -> coordIndex () .emplace_back (i + 1);
				geometry -> coordIndex () .emplace_back (-1);
			}

			geometry -> texCoordIndex () .emplace_back (ts + i);
			geometry -> texCoordIndex () .emplace_back (ts + i + 2);
			geometry -> texCoordIndex () .emplace_back (ts + i + 3);
			geometry -> texCoordIndex () .emplace_back (ts + i + 1);
			geometry -> texCoordIndex () .emplace_back (-1);

			geometry -> coordIndex () .emplace_back (i);
			geometry -> coordIndex () .emplace_back (0);
			geometry -> coordIndex () .emplace_back (1);
			geometry -> coordIndex () .emplace_back (i + 1);
			geometry -> coordIndex () .emplace_back (-1);
		}
	}

	getExecutionContext () -> realize ();
	return SFNode (geometry);
}

void
Disk2D::addTool ()
{
	X3DGeometryNode::addTool (new Disk2DTool (this));
}

} // X3D
} // titania
