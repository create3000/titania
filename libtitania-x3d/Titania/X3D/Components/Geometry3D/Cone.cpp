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

#include "Cone.h"

#include "../../Bits/Cast.h"
#include "../../Browser/Geometry3D/ConeOptions.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"

#include "../Core/MetadataSet.h"
#include "../Geometry3D/IndexedFaceSet.h"
#include "../Rendering/Coordinate.h"
#include "../Texturing/TextureCoordinate.h"

#include <complex>

namespace titania {
namespace X3D {

const Component   Cone::component      = Component ("Geometry3D", 1);
const std::string Cone::typeName       = "Cone";
const std::string Cone::containerField = "geometry";

Cone::Fields::Fields () :
	        side (new SFBool (true)),
	      bottom (new SFBool (true)),
	      height (new SFFloat (2)),
	bottomRadius (new SFFloat (1)),
	       solid (new SFBool (true)),
	     options (new SFNode ())
{ }

Cone::Cone (X3DExecutionContext* const executionContext) :
	    X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DGeometryNode (),
	         fields (),
	    optionsNode ()
{
	addType (X3DConstants::Cone);

	addField (inputOutput,    "metadata",     metadata ());
	addField (initializeOnly, "side",         side ());
	addField (initializeOnly, "bottom",       bottom ());
	addField (initializeOnly, "height",       height ());
	addField (initializeOnly, "bottomRadius", bottomRadius ());
	addField (initializeOnly, "solid",        solid ());
	addField (initializeOnly, "options",      options ());

	addChildObjects (optionsNode);

	height ()       .setUnit (UnitCategory::LENGTH);
	bottomRadius () .setUnit (UnitCategory::LENGTH);

	options () .isHidden (true);
}

X3DBaseNode*
Cone::create (X3DExecutionContext* const executionContext) const
{
	return new Cone (executionContext);
}

void
Cone::initialize ()
{
	X3DGeometryNode::initialize ();

	options () .addInterest (&Cone::set_options, this);

	try
	{
		const auto metaOptions = getMetadataSet ("/Cone/options");

		optionsNode .set (MakePtr <ConeOptions> (getExecutionContext ()));

		optionsNode -> addInterest (&Cone::requestRebuild, this);
		optionsNode -> fromMetaData (metaOptions);
		optionsNode -> setup ();

		options () .set (optionsNode);
	}
	catch (const X3D::X3DError & error)
	{
		set_options ();
	}
}

void
Cone::setExecutionContext (X3DExecutionContext* const executionContext)
{
	if (isInitialized ())
		getBrowser () -> getConeOptions () .removeInterest (&Cone::requestRebuild, this);

	X3DGeometryNode::setExecutionContext (executionContext);

	if (isInitialized ())
		set_options ();
}

Box3d
Cone::createBBox () const
{
	const double diameter = std::abs (bottomRadius ()) * 2;

	if (not side () and not bottom ())
		return Box3d ();

	else if (not side ())
		return Box3d (Vector3d (diameter, 0, diameter), Vector3d (0, -height () / 2, 0));

	else
		return Box3d (Vector3d (diameter, std::abs (height ()), diameter), Vector3d ());
}

void
Cone::set_options ()
{
	removeMetaData ("/Cone/options");

	if (optionsNode)
		optionsNode -> removeInterest (&Cone::requestRebuild, this);

	optionsNode .set (x3d_cast <ConeOptions*> (options ()));

	if (not optionsNode)
		optionsNode .set (getBrowser () -> getConeOptions ());

	optionsNode -> addInterest (&Cone::requestRebuild, this);
}

void
Cone::build ()
{
	if (options ())
		optionsNode -> toMetaData (createMetadataSet ("/Cone/options"));

	const double xDimension = optionsNode -> xDimension ();
	const double y1         = height () / 2;
	const double y2         = -y1;
	const auto   nz         = X3D::polar <double> (1, -pi <double> / 2 + std::atan (bottomRadius () / height ()));

	getMultiTexCoords () .emplace_back ();

	if (side ())
	{
		for (int32_t i = 0; i < xDimension; ++ i)
		{
			const double u1     = (i + 0.5f) / xDimension;
			const double theta1 = 2 * pi <double> * u1;
			const auto   n1     = X3D::polar <double> (nz .imag (), theta1);

			const double u2     = i / xDimension;
			const double theta2 = 2 * pi <double> * u2;
			const auto   p2     = X3D::polar <double> (-bottomRadius (), theta2);
			const auto   n2     = X3D::polar <double> (nz .imag (), theta2);

			const double u3     = (i + 1) / xDimension;
			const double theta3 = 2 * pi <double> * u3;
			const auto   p3     = X3D::polar <double> (-bottomRadius (), theta3);
			const auto   n3     = X3D::polar <double> (nz .imag (), theta3);

			/*    p1
			 *   /  \
			 *  /    \
			 * p2 -- p3
			 */

			// p1
			getTexCoords () .emplace_back (u1, 1, 0, 1);
			getNormals   () .emplace_back (n1 .imag (), nz .real (), n1 .real ());
			getVertices  () .emplace_back (0, y1, 0);

			// p2
			getTexCoords () .emplace_back (u2, 0, 0, 1);
			getNormals   () .emplace_back (n2 .imag (), nz .real (), n2 .real ());
			getVertices  () .emplace_back (p2 .imag (), y2, p2 .real ());

			// p3
			getTexCoords () .emplace_back (u3, 0, 0, 1);
			getNormals   () .emplace_back (n3 .imag (), nz .real (), n3 .real ());
			getVertices  () .emplace_back (p3 .imag (), y2, p3 .real ());
		}
	}

	if (bottom ())
	{
		auto texCoords = std::vector <Vector4f> ();
		auto points    = std::vector <Vector3d> ();

		for (int32_t i = xDimension - 1; i > -1; -- i)
		{
			const double u     = i / xDimension;
			const double theta = 2 * pi <double> * u;
			const auto   t     = X3D::polar <double> (-1, theta);
			const auto   p     = t * double (bottomRadius () .getValue ());

			texCoords .emplace_back ((t .imag () + 1) / 2, (t .real () + 1) / 2, 0, 1);
			points    .emplace_back (p .imag (), y2, p .real ());
		}

		const auto & t0 = texCoords [0];
		const auto & p0 = points [0];

		for (size_t i = 1, size = points .size () - 1; i < size; ++ i)
		{
			const auto & t1 = texCoords [i];
			const auto & t2 = texCoords [i + 1];
			const auto & p1 = points [i];
			const auto & p2 = points [i + 1];

			getTexCoords () .emplace_back (t0);
			getNormals ()   .emplace_back (0, -1, 0);
			getVertices ()  .emplace_back (p0);

			getTexCoords () .emplace_back (t1);
			getNormals ()   .emplace_back (0, -1, 0);
			getVertices ()  .emplace_back (p1);

			getTexCoords () .emplace_back (t2);
			getNormals ()   .emplace_back (0, -1, 0);
			getVertices ()  .emplace_back (p2);
		}
	}

	if (not getVertices () .empty ())
		addElements (GL_TRIANGLES, getVertices () .size ());

	setSolid (solid ());
}

SFNode
Cone::toPrimitive () const
{
	const double xDimension = optionsNode -> xDimension ();

	const auto texCoord = getExecutionContext () -> createNode <TextureCoordinate> ();
	const auto coord    = getExecutionContext () -> createNode <Coordinate> ();
	const auto geometry = getExecutionContext () -> createNode <IndexedFaceSet> ();

	geometry -> solid ()       = solid ();
	geometry -> creaseAngle () = 1;
	geometry -> texCoord ()    = texCoord;
	geometry -> coord ()       = coord;

	const double y1 = height () / 2;
	const double y2 = -y1;

	if (side ())
	{
		coord -> point () .emplace_back (0, y1, 0);

		for (int32_t i = 0; i < xDimension; ++ i)
		{
			const double u1 = (i + 0.5f) / xDimension;
			const double u2 = i / xDimension;

			texCoord -> point () .emplace_back (u1, 1);
			texCoord -> point () .emplace_back (u2, 0);
		}

		texCoord -> point () .emplace_back (1, 0);
	}

	if (bottom () or side ())
	{
		for (int32_t i = 0; i < xDimension; ++ i)
		{
			const double u     = i / xDimension;
			const double theta = 2 * pi <double> * u;
			const auto   t     = X3D::polar <double> (-1, theta);
			const auto   p     = t * double (bottomRadius () .getValue ());

			if (bottom ())
				texCoord -> point () .emplace_back ((t .imag () + 1) / 2, (t .real () + 1) / 2);

			coord -> point () .emplace_back (p .imag (), y2, p .real ());
		}
	}

	int32_t c = 0;
	int32_t t = 0;

	if (side ())
	{
		for (int32_t i = 1; i < xDimension; ++ i)
		{
			const int32_t i2 = i * 2;

			geometry -> texCoordIndex () .emplace_back (i2 - 2);
			geometry -> texCoordIndex () .emplace_back (i2 - 1);
			geometry -> texCoordIndex () .emplace_back (i2 + 1);
			geometry -> texCoordIndex () .emplace_back (-1);

			geometry -> coordIndex () .emplace_back (0);
			geometry -> coordIndex () .emplace_back (i);
			geometry -> coordIndex () .emplace_back (i + 1);
			geometry -> coordIndex () .emplace_back (-1);
		}

		const int32_t xDimension2 = xDimension * 2;

		geometry -> texCoordIndex () .emplace_back (xDimension2 - 2);
		geometry -> texCoordIndex () .emplace_back (xDimension2 - 1);
		geometry -> texCoordIndex () .emplace_back (xDimension2);
		geometry -> texCoordIndex () .emplace_back (-1);

		geometry -> coordIndex () .emplace_back (0);
		geometry -> coordIndex () .emplace_back (xDimension);
		geometry -> coordIndex () .emplace_back (1);
		geometry -> coordIndex () .emplace_back (-1);

		c += 1;
		t += xDimension2 + 1;
	}

	if (bottom ())
	{
		for (int32_t i = 0, ts = t + xDimension - 1, cs = c + xDimension - 1; i < xDimension; ++ i)
		{
			geometry -> texCoordIndex () .emplace_back (ts - i);
			geometry -> coordIndex ()    .emplace_back (cs - i);
		}

		geometry -> texCoordIndex () .emplace_back (-1);
		geometry -> coordIndex ()    .emplace_back (-1);
	}

	return geometry;
}

Cone::~Cone ()
{ }

} // X3D
} // titania
