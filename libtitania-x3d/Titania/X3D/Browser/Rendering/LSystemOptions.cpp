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

#include "LSystemOptions.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../Types/MatrixStack.h"

#include "../../Components/Core/MetadataSet.h"
#include "../../Components/Rendering/Coordinate.h"
#include "../../Components/Rendering/IndexedLineSet.h"

#include <Titania/Math/Algorithms/TurtleRenderer.h>

namespace titania {
namespace X3D {

struct LSystemOptions::Values
{
	Values (const Vector3d & point,
	        const Matrix3d & matrix,
	        int32_t colorPerVertexIndex,
	        int32_t colorIndex,
	        int32_t coordIndex) :
		              point (point),
		             matrix (matrix),
		colorPerVertexIndex (colorPerVertexIndex),
		         colorIndex (colorIndex),
		         coordIndex (coordIndex)
	{ }

	const Vector3d point;
	const Matrix3d matrix;
	const int32_t  colorPerVertexIndex;
	const int32_t  colorIndex;
	const int32_t  coordIndex;
};

const ComponentType LSystemOptions::component      = ComponentType::TITANIA;
const std::string   LSystemOptions::typeName       = "LSystemOptions";
const std::string   LSystemOptions::containerField = "options";

LSystemOptions::Fields::Fields () :
	iterations (new SFInt32 (8)),
	    xAngle (new SFFloat (0)),
	    yAngle (new SFFloat (0)),
	    zAngle (new SFFloat (radians (45.0))),
	      size (new SFVec3f (2, 2, 2)),
	 constants (new SFString ()),
	     axiom (new SFString ("B")),
	      rule (new MFString ({ "A=C0AA", "B=C1A[+B]-B" }))
{ }

LSystemOptions::LSystemOptions (X3DExecutionContext* const executionContext) :
	           X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DGeometricOptionNode (),
	       indexedLineSets ()
{
	addType (X3DConstants::LSystemOptions);

	addField (inputOutput, "iterations", iterations ());
	addField (inputOutput, "xAngle",     xAngle ());
	addField (inputOutput, "yAngle",     yAngle ());
	addField (inputOutput, "zAngle",     zAngle ());
	addField (inputOutput, "size",       size ());
	addField (inputOutput, "constants",  constants ());
	addField (inputOutput, "axiom",      axiom ());
	addField (inputOutput, "rule",       rule ());

	xAngle () .setUnit (UnitCategory::ANGLE);
	yAngle () .setUnit (UnitCategory::ANGLE);
	zAngle () .setUnit (UnitCategory::ANGLE);
	size ()   .setUnit (UnitCategory::LENGTH);
}

LSystemOptions*
LSystemOptions::create (X3DExecutionContext* const executionContext) const
{
	return new LSystemOptions (executionContext);
}

void
LSystemOptions::addNode (IndexedLineSet* const indexedLineSet)
{
	indexedLineSets .emplace (indexedLineSet);

	indexedLineSet -> colorPerVertex () .addInterest (&LSystemOptions::addEvent, this);

	addEvent ();
}

void
LSystemOptions::removeNode (IndexedLineSet* const indexedLineSet)
{
	indexedLineSet -> colorPerVertex () .removeInterest (&LSystemOptions::addEvent, this);

	indexedLineSets .erase (indexedLineSet);
}

void
LSystemOptions::build ()
{
	try
	{
		using LSystem        = math::lsystem <std::string>;
		using TurtleRenderer = math::turtle_renderer <double, std::string>;

		struct StackValue {

			StackValue (const TurtleRenderer::node_ptr & node,
			            const int32_t lineColor,
			            const int32_t color,
			            const int32_t coord) :
				     node (node),
				lineColor (lineColor),
				    color (color),
				    coord (coord)
			{ }

			const TurtleRenderer::node_ptr node;
			const int32_t                  lineColor;
			const int32_t                  color;
			const int32_t                  coord;

		};

		if (indexedLineSets .empty ())
			return;

		const LSystem lsystem (std::max <int32_t> (0, iterations ()), constants (), axiom (), rule () .begin (), rule () .end ());
		const TurtleRenderer renderer (xAngle (), yAngle (), zAngle (), lsystem);

		// Render tree.

		auto stack                 = std::vector <std::unique_ptr <StackValue>> ();
		auto root                  = renderer .tree ();
		auto colorPerLineIndices   = std::vector <int32_t> ();
		auto colorPerVertexIndices = std::vector <int32_t> ();
		auto coordIndices          = std::vector <int32_t> ();
		auto points                = std::vector <Vector3d> ();

		if (root)
			stack .emplace_back (std::make_unique <StackValue> (root, -2, -2, -2));

		while (not stack .empty ())
		{
			const auto front = std::move (stack .back ());
			auto       node  = front -> node;
			const auto color = node -> color;
			const auto coord = points .size ();

			stack .pop_back ();

			if (front -> coord >= 0)
			{
				colorPerLineIndices   .emplace_back (front -> lineColor);
				colorPerVertexIndices .emplace_back (front -> color);
				coordIndices          .emplace_back (front -> coord);
			}

			points .emplace_back (node -> point);

			switch (node -> children .size ())
			{
				case 0:
				{
					colorPerVertexIndices .emplace_back (color);
					colorPerVertexIndices .emplace_back (-1);

					coordIndices .emplace_back (coord);
					coordIndices .emplace_back (-1);
					break;
				}
				case 1:
				{
					while (node -> children .front () -> next_control_point == 0)
					{
						node = node -> children .front ();
					}

					if (front -> coord not_eq -2)
					{
						colorPerVertexIndices .emplace_back (color);
						colorPerVertexIndices .emplace_back (-1);
	
						coordIndices .emplace_back (coord);
						coordIndices .emplace_back (-1);
					}

					stack .emplace_back (std::make_unique <StackValue> (node -> children .front (), node -> colors .front (), color, coord));
					break;
				}
				default:
				{
					colorPerVertexIndices .emplace_back (color);
					colorPerVertexIndices .emplace_back (-1);

					coordIndices .emplace_back (coord);
					coordIndices .emplace_back (-1);

					for (size_t i = 0, size = node -> children .size (); i < size; ++ i)
						stack .emplace_back (std::make_unique <StackValue> (node -> children [i], node -> colors [i], color, coord));
				}
			}
		}

		// Set size.

		const auto bbox        = Box3d (points .begin (), points .end (), iterator_type ());
		auto       scaleMatrix = Matrix4d ();

		scaleMatrix .scale (size () / maximum_norm (bbox .size ()));
		scaleMatrix .translate (negate (bbox .center ()));

		for (auto & point : points)
			point = point * scaleMatrix;

		// Assign values.

		for (const auto & indexedLineSet : indexedLineSets)
		{
			toMetaData (indexedLineSet -> createMetadataSet ("/IndexedLineSet/options"));

			if (renderer .colors ())
			{
				if (indexedLineSet -> colorPerVertex ())
					indexedLineSet -> colorIndex () .assign (colorPerVertexIndices .begin (), colorPerVertexIndices .end ());
				else
					indexedLineSet -> colorIndex () .assign (colorPerLineIndices .begin (), colorPerLineIndices .end ());
			}

			indexedLineSet -> coordIndex () .assign (coordIndices .begin (), coordIndices .end ());

			if (indexedLineSet -> getCoord ())
			{
				for (size_t i = 0, size = points .size (); i < size; ++ i)
					indexedLineSet -> getCoord () -> set1Point (i, points [i]);

				indexedLineSet -> getCoord () -> resize (points .size ());
			}
			else
			{
				const auto coord = getExecutionContext () -> createNode <Coordinate> ();

				indexedLineSet -> coord () = coord;

				for (size_t i = 0, size = points .size (); i < size; ++ i)
					coord -> set1Point (i, points [i]);
			}
		}
	}
	catch (const std::exception & error)
	{
		getBrowser () -> println (error .what ());
	}
}

} // X3D
} // titania
