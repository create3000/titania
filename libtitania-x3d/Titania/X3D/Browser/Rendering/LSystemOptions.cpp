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

#include <Titania/Math/Algorithms/LSystem.h>

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
	      tilt (new SFFloat (radians (45.0))),
	     twist (new SFFloat (0)),
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
	addField (inputOutput, "tilt",       tilt ());
	addField (inputOutput, "twist",      twist ());
	addField (inputOutput, "size",       size ());
	addField (inputOutput, "constants",  constants ());
	addField (inputOutput, "axiom",      axiom ());
	addField (inputOutput, "rule",       rule ());

	tilt () .setUnit (UnitCategory::ANGLE);
	twist () .setUnit (UnitCategory::ANGLE);
	size ()  .setUnit (UnitCategory::LENGTH);
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
		if (indexedLineSets .empty ())
			return;

		const math::lsystem lsystem (std::max <int32_t> (0, iterations ()), constants (), axiom (), std::vector <std::string> (rule () .begin (), rule () .end ()));

		const auto coord = getExecutionContext () -> createNode <Coordinate> ();

		auto    stack                 = std::vector <Values> ();
		auto    matrix                = Matrix3d ();
		auto    point                 = Vector3d ();
		bool    change                = true;
		bool    first                 = true;
		bool    colors                = false;
		auto    usedColorIndices      = std::set <int32_t> ();
		int32_t colorPerVertexIndex   = -1;
		int32_t colorIndex            = 0;
		int32_t coordIndex            = 0;
		auto    colorPerLineIndices   = MFInt32 ();
		auto    colorPerVertexIndices = MFInt32 ();
		auto    coordIndices          = MFInt32 ();

		coord -> set1Point (coordIndex, Vector3d ());

		std::istringstream isstream (lsystem .commands ());

		while (isstream)
		{
			const auto c = isstream .get ();

			switch (c)
			{
				case 'C': // Color
				{
					int32_t index;
	
					if ((isstream >> index) and (index >= 0) and (index not_eq colorIndex))
					{
						change     = true;
						first      = true;
						colors     = true;
						colorIndex = index;

						if (colorPerVertexIndex >= 0 and not usedColorIndices .count (colorPerVertexIndex))
							colorPerVertexIndices [colorPerVertexIndex] = index;

						colorPerVertexIndices .emplace_back (-1);
						coordIndices          .emplace_back (-1);
					}

					break;
				}
				case '+': // Counterclockwise tilt
				{
					matrix *= Matrix3d (Rotation4d (matrix [2], tilt ()));
					change  = true;
					break;
				}
				case '-': // Clockwise tilt
				{
					matrix *= Matrix3d (Rotation4d (matrix [2], -tilt ()));
					change  = true;
					break;
				}
				case '>': // Counterclockwise twist
				{
					matrix *= Matrix3d (Rotation4d (matrix [1], twist ()));
					change  = true;
					break;
				}
				case '<': // Clockwise twist
				{
					matrix *= Matrix3d (Rotation4d (matrix [1], -twist ()));
					change  = true;
					break;
				}
				case '[': // Push
				{
					stack .emplace_back (point, matrix, colorPerVertexIndex, colorIndex, coordIndex);

					if (coordIndices .empty ())
						break;

					change = true;
					first  = true;

					colorPerVertexIndices .emplace_back (-1);
					coordIndices          .emplace_back (-1);
					break;
				}
				case ']': // Pop
				{
					if (stack .empty ())
						break;

					change              = true;
					first               = true;
					point               = stack .back () .point;
					matrix              = stack .back () .matrix;
					colorPerVertexIndex = stack .back () .colorPerVertexIndex;
					colorIndex          = stack .back () .colorIndex;
					coordIndex          = stack .back () .coordIndex;

					stack .pop_back ();

					colorPerVertexIndices .emplace_back (-1);
					coordIndices          .emplace_back (-1);
					break;
				}
				default: // Line
				{
					if (c < 0)
						break;

					if (lsystem .constants () [c])
						break;

					point += matrix [1];

					if (change)
					{
						change = false;

						if (first)
						{
							first = false;
	
							colorPerLineIndices .emplace_back (colorIndex);
	
							if (colorPerVertexIndex >= 0)
							{
								usedColorIndices .emplace (colorPerVertexIndex);
								colorPerVertexIndices .emplace_back (colorPerVertexIndices [colorPerVertexIndex]);
							}
							else
								colorPerVertexIndices .emplace_back (colorIndex);
	
							coordIndices .emplace_back (coordIndex);
						}
	
						colorPerVertexIndex = colorPerVertexIndices .size ();
						colorPerVertexIndices .emplace_back (colorIndex);

						coordIndex = coord -> getSize ();
						coordIndices .emplace_back (coordIndex);
						coord -> set1Point (coordIndex, point);
					}
					else
					{
						coord -> set1Point (coordIndex, point);
					}

					break;
				}
			}
		}

		// Set size.

		const auto bbox        = coord -> getBBox ();
		auto       scaleMatrix = Matrix4f ();

		scaleMatrix .scale (size () / maximum_norm (bbox .size ()));
		scaleMatrix .translate (negate (bbox .center ()));

		for (auto & point : coord -> point ())
			point = point .getValue () * scaleMatrix;

		// Remove consecutive -1 from colorIndices.

		colorPerVertexIndices .emplace_back (-1);

		colorPerVertexIndices .erase (std::unique (colorPerVertexIndices .begin (),
		                                           colorPerVertexIndices .end (),
		                                           [ ] (int32_t l, int32_t r) { return l == -1 && r == -1; }),
		                              colorPerVertexIndices .end ());

		// Special case.

		if (colorPerVertexIndices .size () == 1 and colorPerVertexIndices [0] == -1)
			colorPerVertexIndices .clear ();

		if (not colors)
		{
			colorPerLineIndices .clear ();
			colorPerVertexIndices .clear ();
		}

		// Remove consecutive -1 from coordIndices.

		coordIndices .emplace_back (-1);

		coordIndices .erase (std::unique (coordIndices .begin (),
		                                  coordIndices .end (),
		                                  [ ] (int32_t l, int32_t r) { return l == -1 && r == -1; }),
		                     coordIndices .end ());

		// Special case.

		if (coordIndices .size () == 1 and coordIndices [0] == -1)
			coordIndices .clear ();

		if (coordIndices .empty ())
			coord -> resize (0);

		// Assign values.

		for (const auto & indexedLineSet : indexedLineSets)
		{
			toMetaData (indexedLineSet -> createMetadataSet ("/IndexedLineSet/options"));

			if (indexedLineSet -> colorPerVertex ())
				indexedLineSet -> colorIndex () = colorPerVertexIndices;
			else
				indexedLineSet -> colorIndex () = colorPerLineIndices;

			indexedLineSet -> coordIndex () = coordIndices;
			indexedLineSet -> coord ()      = coord;
		}
	}
	catch (const std::exception & error)
	{
		getBrowser () -> println (error .what ());
	}
}

} // X3D
} // titania
