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

#include "L-System.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../Types/MatrixStack.h"

#include "../../Components/Core/MetadataSet.h"
#include "../../Components/Rendering/Coordinate.h"
#include "../../Components/Rendering/IndexedLineSet.h"

#include <Titania/Math/Algorithms/TurtleRenderer.h>

namespace titania {
namespace X3D {

struct LSystem::Values
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

const Component   LSystem::component      = Component ("Titania", 1);
const std::string LSystem::typeName       = "L-System";
const std::string LSystem::containerField = "options";

LSystem::Fields::Fields () :
	     iterations (new SFInt32 (8)),
	         xAngle (new SFDouble (0)),
	         yAngle (new SFDouble (0)),
	         zAngle (new SFDouble (radians (45.0))),
	 angleVariation (new SFDouble (0)),
	lengthVariation (new SFDouble (0)),
	           size (new SFVec3d (2, 2, 2)),
	      constants (new SFString ()),
	          axiom (new SFString ("B")),
	           rule (new MFString ({ "A=C0AA", "B=C1A[+B]-B" }))
{ }

LSystem::LSystem (X3DExecutionContext* const executionContext) :
	           X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DGeometricOptionNode (),
	       indexedLineSets ()
{
	addType (X3DConstants::LSystem);

	addField (inputOutput, "iterations",      iterations ());
	addField (inputOutput, "xAngle",          xAngle ());
	addField (inputOutput, "yAngle",          yAngle ());
	addField (inputOutput, "zAngle",          zAngle ());
	addField (inputOutput, "angleVariation",  angleVariation ());
	addField (inputOutput, "lengthVariation", lengthVariation ());
	addField (inputOutput, "size",            size ());
	addField (inputOutput, "constants",       constants ());
	addField (inputOutput, "axiom",           axiom ());
	addField (inputOutput, "rule",            rule ());

	xAngle () .setUnit (UnitCategory::ANGLE);
	yAngle () .setUnit (UnitCategory::ANGLE);
	zAngle () .setUnit (UnitCategory::ANGLE);
	size ()   .setUnit (UnitCategory::LENGTH);
}

LSystem*
LSystem::create (X3DExecutionContext* const executionContext) const
{
	return new LSystem (executionContext);
}

void
LSystem::addNode (IndexedLineSet* const indexedLineSet)
{
	indexedLineSets .emplace (indexedLineSet);

	indexedLineSet -> colorPerVertex () .addInterest (&LSystem::addEvent, this);

	addEvent ();
}

void
LSystem::removeNode (IndexedLineSet* const indexedLineSet)
{
	indexedLineSet -> colorPerVertex () .removeInterest (&LSystem::addEvent, this);

	indexedLineSets .erase (indexedLineSet);
}

void
LSystem::build ()
{
	try
	{
		static constexpr double tolerance = 1e-5;

		using LSystem        = math::lsystem <std::string>;
		using TurtleRenderer = math::turtle_renderer <double, std::string>;

		struct StackValue
		{

			StackValue (const TurtleRenderer::node_ptr & child,
			            const int32_t color,
			            const int32_t coord) :
				    child (child),
				    color (color),
				    coord (coord)
			{ }

			const TurtleRenderer::node_ptr child;
			const int32_t                  color;
			const int32_t                  coord;

		};

		if (indexedLineSets .empty ())
			return;

		// Generate L-System and setup turtle renderer.

		const LSystem lsystem (std::max <int32_t> (0, iterations ()), constants (), axiom (), rule () .cbegin (), rule () .cend ());

		TurtleRenderer renderer;

		renderer .x_angle (xAngle ());
		renderer .y_angle (yAngle ());
		renderer .z_angle (zAngle ());
		renderer .angle_variation (angleVariation ());
		renderer .length_variation (lengthVariation ());
		renderer .tolerance (tolerance);

		renderer .render (lsystem);

		// Render tree.

		auto stack                 = std::vector <std::unique_ptr <StackValue>> ();
		auto root                  = renderer .tree ();
		auto colorPerLineIndices   = std::vector <int32_t> ();
		auto colorPerVertexIndices = std::vector <int32_t> ();
		auto coordPerLineIndices   = std::vector <int32_t> ();
		auto coordPerVertexIndices = std::vector <int32_t> ();
		auto points                = std::vector <Vector3d> ();

		if (root)
		{
			// Handle first point.

			const auto color = root -> color;
			const auto coord = points .size ();

			// Root has always children, thus we can emplace a point.
			points .emplace_back (root -> point);

			for (const auto & child : root -> children)
				stack .emplace_back (std::make_unique <StackValue> (child, color, coord));

			// Traverse tree.

			while (not stack .empty ())
			{
				const auto first  = std::move (stack .back ());
				auto       second = first -> child;
				auto       color  = second -> color;
				auto       coord  = points .size ();
	
				stack .pop_back ();

				points .emplace_back (second -> point);
	
				colorPerLineIndices   .emplace_back (second -> line_color);
				colorPerVertexIndices .emplace_back (first -> color);
				coordPerLineIndices   .emplace_back (first -> coord);
				coordPerVertexIndices .emplace_back (first -> coord);

				while (second -> children .size () == 1)
				{
					const auto lineColor = second -> line_color;

					colorPerVertexIndices .emplace_back (color);
					coordPerLineIndices   .emplace_back (coord);
					coordPerVertexIndices .emplace_back (coord);

					second = second -> children .front ();

					if (lineColor not_eq second -> line_color)
					{
						colorPerLineIndices .emplace_back (second -> line_color);
						coordPerLineIndices .emplace_back (-1);
						coordPerLineIndices .emplace_back (coord);
					}

					color = second -> color;
					coord = points .size ();

					points .emplace_back (second -> point);
				}

				colorPerVertexIndices .emplace_back (color);
				colorPerVertexIndices .emplace_back (-1);
		
				coordPerLineIndices   .emplace_back (coord);
				coordPerLineIndices   .emplace_back (-1);
				coordPerVertexIndices .emplace_back (coord);
				coordPerVertexIndices .emplace_back (-1);
	
				for (const auto & child : second -> children)
					stack .emplace_back (std::make_unique <StackValue> (child, color, coord));
			}
	
			// Set size.

			const auto pointsBBox  = Box3d (points .begin (), points .end (), iterator_type ());
			const auto scaleBBox   = pointsBBox + Box3d (pointsBBox .size (), -pointsBBox .center ());
			auto       scaleMatrix = Matrix4d ();

			scaleMatrix .scale (size () / maximum_norm (scaleBBox .size ()));

			for (auto & point : points)
				point = point * scaleMatrix;
		}

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
			else
				indexedLineSet -> colorIndex () .clear ();

			if (indexedLineSet -> colorPerVertex ())
				indexedLineSet -> coordIndex () .assign (coordPerVertexIndices .begin (), coordPerVertexIndices .end ());
			else
				indexedLineSet -> coordIndex () .assign (coordPerLineIndices .begin (), coordPerLineIndices .end ());

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
		getBrowser () -> getConsole () -> error (error .what (), "\n");
	}
}

} // X3D
} // titania
