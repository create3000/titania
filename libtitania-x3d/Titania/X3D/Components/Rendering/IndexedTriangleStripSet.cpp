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

#include "IndexedTriangleStripSet.h"

#include "../../Execution/X3DExecutionContext.h"


namespace titania {
namespace X3D {

const Component   IndexedTriangleStripSet::component      = Component ("Rendering", 3);
const std::string IndexedTriangleStripSet::typeName       = "IndexedTriangleStripSet";
const std::string IndexedTriangleStripSet::containerField = "geometry";

IndexedTriangleStripSet::Fields::Fields () :
	set_index (new MFInt32 ()),
	    index (new MFInt32 ())
{ }

IndexedTriangleStripSet::IndexedTriangleStripSet (X3DExecutionContext* const executionContext) :
	            X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DComposedGeometryNode (),
	                 fields (),
	             coordIndex ()
{
	addType (X3DConstants::IndexedTriangleStripSet);

	addField (inputOutput,    "metadata",        metadata ());

	addField (inputOnly,      "set_index",       set_index ());

	addField (initializeOnly, "solid",           solid ());
	addField (initializeOnly, "ccw",             ccw ());
	addField (initializeOnly, "colorPerVertex",  colorPerVertex ());
	addField (initializeOnly, "normalPerVertex", normalPerVertex ());

	addField (initializeOnly, "index",           index ());

	addField (inputOutput,    "attrib",          attrib ());
	addField (inputOutput,    "fogCoord",        fogCoord ());
	addField (inputOutput,    "color",           color ());
	addField (inputOutput,    "texCoord",        texCoord ());
	addField (inputOutput,    "normal",          normal ());
	addField (inputOutput,    "coord",           coord ());
}

X3DBaseNode*
IndexedTriangleStripSet::create (X3DExecutionContext* const executionContext) const
{
	return new IndexedTriangleStripSet (executionContext);
}

void
IndexedTriangleStripSet::initialize ()
{
	X3DComposedGeometryNode::initialize ();

	set_index () .addInterest (index ());
	index ()     .addInterest (&IndexedTriangleStripSet::set_index_, this);

	set_index_ ();
}

void
IndexedTriangleStripSet::set_index_ ()
{
	// Build coordIndex

	coordIndex .clear ();

	for (size_t i = 0, size = index () .size (); i < size; ++ i)
	{
		int32_t first = index () [i];

		if (first < 0)
			continue;

		++ i;

		if (i < size)
		{
			int32_t second = index () [i];

			if (second < 0)
				continue;

			++ i;

			for (size_t face = 0; i < size; ++ i, ++ face)
			{
				const int32_t third = index () [i];

				if (third < 0)
					break;

				coordIndex .emplace_back (first);
				coordIndex .emplace_back (second);
				coordIndex .emplace_back (third);

				if (is_odd (face))
					second = third;

				else
					first = third;
			}
		}
	}
}

void
IndexedTriangleStripSet::build ()
{
	X3DComposedGeometryNode::build (3, coordIndex .size ());
}

void
IndexedTriangleStripSet::addNormals ()
{
	X3DComposedGeometryNode::addNormals (3, coordIndex .size ());
}

SFNode
IndexedTriangleStripSet::toPrimitive () const
{
	return X3DComposedGeometryNode::toPrimitive (3, coordIndex .size ());
}

} // X3D
} // titania
