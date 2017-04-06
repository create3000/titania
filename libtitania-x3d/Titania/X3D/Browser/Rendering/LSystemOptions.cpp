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

template <class Type, class String>
class turtle_renderer {
public:

	///  @name Member types

	struct node_type;

	using node_ptr      = std::shared_ptr <node_type>;
	using children_type = std::vector <node_ptr>;

	struct node_type {

		node_type (const vector3 <Type> & point, const int32_t color) :
			             color (color),
			next_control_point (1),
			             point (point),
			          children ()
		{ }

		int32_t        color;
		int32_t        next_control_point;
		vector3 <Type> point;
		children_type  children;

	};

	///  @name Construction

	turtle_renderer (const Type & x_angle, const Type & y_angle, const Type & z_angle);

	turtle_renderer (const Type & x_angle, const Type & y_angle, const Type & z_angle, const lsystem <String> & lsystem);

	///  @name Members

	const Type &
	x_angle () const
	{ return mx_angle; }

	const Type &
	y_angle () const
	{ return my_angle; }

	const Type &
	z_angle () const
	{ return mz_angle; }

	bool
	colors () const
	{ return m_colors; }

	const node_ptr &
	tree () const
	{ return m_tree; }

	///  @name Operations

	void
	render (const lsystem <String> & lsystem);

	///  @name Destruction

	~turtle_renderer ();

private:

	///  @name Members

	Type     mx_angle;
	Type     my_angle;
	Type     mz_angle;
	bool     m_colors;
	node_ptr m_tree; 

};

template <class Type, class String>
turtle_renderer <Type, String>::turtle_renderer (const Type & x_angle, const Type & y_angle, const Type & z_angle) :
	mx_angle (x_angle),
	my_angle (y_angle),
	mz_angle (z_angle),
	m_colors (false),
	  m_tree ()
{ }

template <class Type, class String>
turtle_renderer <Type, String>::turtle_renderer (const Type & x_angle, const Type & y_angle, const Type & z_angle, const lsystem <String> & lsystem) :
	turtle_renderer (x_angle, y_angle, z_angle)
{
	render (lsystem);
}

template <class Type, class String>
void
turtle_renderer <Type, String>::render (const lsystem <String> & lsystem)
{
	struct stack_value
	{
		stack_value (const node_ptr & node,
		             const vector3 <Type> & point,
		             const matrix3 <Type> & matrix,
		             const int32_t color) :
			  node (node),
			 point (point),
			matrix (matrix),
			 color (color)
		{ }
	
		const node_ptr       node;
		const vector3 <Type> point;
		const matrix3 <Type> matrix;
		const int32_t        color;
	};

	bool    change = true;
	auto    matrix = matrix3 <Type> ();
	auto    point  = vector3 <Type> ();
	int32_t color  = 0;
	auto    root   = std::make_shared <node_type> (point, color);
	auto    node   = root;
	auto    stack  = std::vector <stack_value> ();

	std::istringstream isstream (lsystem .commands ());

	while (isstream)
	{
		const auto c = isstream .get ();

		switch (c)
		{
			case 'C': // Color
			{
				int32_t index;

				if ((isstream >> index) and (index >= 0) and (index not_eq color))
				{
					m_colors      = true;
					change        = true;
					color         = index;
					node -> color = index;
				}

				break;
			}
			case '}': // Counterclockwise rotation about local x-axis
			{
				matrix *= matrix3 <Type> (rotation4 <Type> (matrix [0], mx_angle));
				change  = true;
				break;
			}
			case '{': // Clockwise rotation about local x-axis
			{
				matrix *= matrix3 <Type> (rotation4 <Type> (matrix [0], -mx_angle));
				change  = true;
				break;
			}
			case '>': // Counterclockwise rotation about local y-axis
			{
				matrix *= matrix3 <Type> (rotation4 <Type> (matrix [1], my_angle));
				change  = true;
				break;
			}
			case '<': // Clockwise rotation about local y-axis
			{
				matrix *= matrix3 <Type> (rotation4 <Type> (matrix [1], -my_angle));
				change  = true;
				break;
			}
			case '+': // Counterclockwise rotation about local z-axis
			{
				matrix *= matrix3 <Type> (rotation4 <Type> (matrix [2], mz_angle));
				change  = true;
				break;
			}
			case '-': // Clockwise rotation about local z-axis
			{
				matrix *= matrix3 <Type> (rotation4 <Type> (matrix [2], -mz_angle));
				change  = true;
				break;
			}
			case '[': // Push
			{
				stack .emplace_back (node, point, matrix, color);

				change = true;
				break;
			}
			case ']': // Pop
			{
				if (stack .empty ())
					break;

				const auto & back = stack .back ();

				change = true;
				node   = back .node;
				point  = back .point;
				matrix = back .matrix;
				color  = back .color;

				stack .pop_back ();
				break;
			}
			default: // Line
			{
				if (c < 0)
					break;

				if (lsystem .is_constant (c))
					break;

				point += matrix [1];

				if (change)
				{
					change = false;

					auto child = std::make_shared <node_type> (point, color);
	
					node -> children .emplace_back (child);
	
					node = std::move (child);
				}
				else
				{
					node -> point = point;
				}

				break;
			}
		}
	}

	if (root -> children .empty ())
		return;

	m_tree = root;
}

template <class Type, class String>
turtle_renderer <Type, String>::~turtle_renderer ()
{ }

void
LSystemOptions::build ()
{
	try
	{
		using LSystem        = math::lsystem <std::string>;
		using TurtleRenderer = turtle_renderer <double, std::string>;

		struct StackValue {

			StackValue (const TurtleRenderer::node_ptr & node,
			            const int32_t color,
			            const int32_t coord) :
				 node (node),
				color (color),
				coord (coord)
			{ }

			const TurtleRenderer::node_ptr node;
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
		{
			stack               .emplace_back (std::make_unique <StackValue> (root, -1, -1));
			colorPerLineIndices .emplace_back (root -> color);
		}

		while (not stack .empty ())
		{
			const auto   front = std::move (stack .back ());
			const auto & node  = front -> node;
			const auto   color = node -> color;
			const auto   coord = points .size ();

			stack .pop_back ();

			if (front -> coord not_eq -1)
			{
				colorPerLineIndices   .emplace_back (front -> color);
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
					if (node -> next_control_point == 0)
					{
						colorPerVertexIndices .emplace_back (color);
						coordIndices          .emplace_back (coord);
	
						for (const auto & child : node -> children)
							stack .emplace_back (std::make_unique <StackValue> (child, -1, -1));

						break;
					}

					// Proceed with next case:
				}
				default:
				{
					colorPerVertexIndices .emplace_back (color);
					colorPerVertexIndices .emplace_back (-1);

					coordIndices .emplace_back (coord);
					coordIndices .emplace_back (-1);

					for (const auto & child : node -> children)
						stack .emplace_back (std::make_unique <StackValue> (child, color, coord));
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


//		auto    stack                 = std::vector <Values> ();
//		auto    matrix                = Matrix3d ();
//		auto    point                 = Vector3d ();
//		bool    change                = true;
//		bool    first                 = true;
//		bool    colors                = false;
//		auto    usedColorIndices      = std::set <int32_t> ();
//		int32_t colorPerVertexIndex   = -1;
//		int32_t colorIndex            = 0;
//		int32_t coordIndex            = 0;
//		auto    colorPerLineIndices   = std::vector <int32_t> ();
//		auto    colorPerVertexIndices = std::vector <int32_t> ();
//		auto    coordIndices          = std::vector <int32_t> ();
//		auto    points                = std::vector <Vector3d> ();
//
//		points .emplace_back (Vector3d ());
//
//		std::istringstream isstream (lsystem .commands ());
//
//		while (isstream)
//		{
//			const auto c = isstream .get ();
//
//			switch (c)
//			{
//				case 'C': // Color
//				{
//					int32_t index;
//	
//					if ((isstream >> index) and (index >= 0) and (index not_eq colorIndex))
//					{
//						change     = true;
//						first      = true;
//						colors     = true;
//						colorIndex = index;
//
//						if (colorPerVertexIndex >= 0 and not usedColorIndices .count (colorPerVertexIndex))
//							colorPerVertexIndices [colorPerVertexIndex] = index;
//
//						colorPerVertexIndices .emplace_back (-1);
//						coordIndices          .emplace_back (-1);
//					}
//
//					break;
//				}
//				case '}': // Counterclockwise rotation about local x-axis
//				{
//					matrix *= Matrix3d (Rotation4d (matrix [0], xAngle ()));
//					change  = true;
//					break;
//				}
//				case '{': // Clockwise rotation about local x-axis
//				{
//					matrix *= Matrix3d (Rotation4d (matrix [0], -xAngle ()));
//					change  = true;
//					break;
//				}
//				case '>': // Counterclockwise rotation about local y-axis
//				{
//					matrix *= Matrix3d (Rotation4d (matrix [1], yAngle ()));
//					change  = true;
//					break;
//				}
//				case '<': // Clockwise rotation about local y-axis
//				{
//					matrix *= Matrix3d (Rotation4d (matrix [1], -yAngle ()));
//					change  = true;
//					break;
//				}
//				case '+': // Counterclockwise rotation about local z-axis
//				{
//					matrix *= Matrix3d (Rotation4d (matrix [2], zAngle ()));
//					change  = true;
//					break;
//				}
//				case '-': // Clockwise rotation about local z-axis
//				{
//					matrix *= Matrix3d (Rotation4d (matrix [2], -zAngle ()));
//					change  = true;
//					break;
//				}
//				case '[': // Push
//				{
//					stack .emplace_back (point, matrix, colorPerVertexIndex, colorIndex, coordIndex);
//
//					if (coordIndices .empty ())
//						break;
//
//					change = true;
//					first  = true;
//
//					colorPerVertexIndices .emplace_back (-1);
//					coordIndices          .emplace_back (-1);
//					break;
//				}
//				case ']': // Pop
//				{
//					if (stack .empty ())
//						break;
//
//					change              = true;
//					first               = true;
//					point               = stack .back () .point;
//					matrix              = stack .back () .matrix;
//					colorPerVertexIndex = stack .back () .colorPerVertexIndex;
//					colorIndex          = stack .back () .colorIndex;
//					coordIndex          = stack .back () .coordIndex;
//
//					stack .pop_back ();
//
//					colorPerVertexIndices .emplace_back (-1);
//					coordIndices          .emplace_back (-1);
//					break;
//				}
//				default: // Line
//				{
//					if (c < 0)
//						break;
//
//					if (lsystem .is_constant (c))
//						break;
//
//					point += matrix [1];
//
//					if (change)
//					{
//						change = false;
//
//						if (first)
//						{
//							first = false;
//	
//							colorPerLineIndices .emplace_back (colorIndex);
//	
//							if (colorPerVertexIndex >= 0)
//							{
//								usedColorIndices .emplace (colorPerVertexIndex);
//								colorPerVertexIndices .emplace_back (colorPerVertexIndices [colorPerVertexIndex]);
//							}
//							else
//								colorPerVertexIndices .emplace_back (colorIndex);
//	
//							coordIndices .emplace_back (coordIndex);
//						}
//	
//						colorPerVertexIndex = colorPerVertexIndices .size ();
//						colorPerVertexIndices .emplace_back (colorIndex);
//
//						coordIndex = points .size ();
//						coordIndices .emplace_back (coordIndex);
//						points       .emplace_back (point);
//					}
//					else
//					{
//						points [coordIndex] = point;
//					}
//
//					break;
//				}
//			}
//		}
//
//		// Set size.
//
//		const auto bbox        = Box3d (points .begin (), points .end (), iterator_type ());
//		auto       scaleMatrix = Matrix4d ();
//
//		scaleMatrix .scale (size () / maximum_norm (bbox .size ()));
//		scaleMatrix .translate (negate (bbox .center ()));
//
//		for (auto & point : points)
//			point = point * scaleMatrix;
//
//		// Remove consecutive -1 from colorIndices.
//
//		colorPerVertexIndices .emplace_back (-1);
//
//		colorPerVertexIndices .erase (std::unique (colorPerVertexIndices .begin (),
//		                                           colorPerVertexIndices .end (),
//		                                           [ ] (int32_t l, int32_t r) { return l == -1 && r == -1; }),
//		                              colorPerVertexIndices .end ());
//
//		// Special case.
//
//		if (colorPerVertexIndices .size () == 1 and colorPerVertexIndices [0] == -1)
//			colorPerVertexIndices .clear ();
//
//		if (not colors)
//		{
//			colorPerLineIndices .clear ();
//			colorPerVertexIndices .clear ();
//		}
//
//		// Remove consecutive -1 from coordIndices.
//
//		coordIndices .emplace_back (-1);
//
//		coordIndices .erase (std::unique (coordIndices .begin (),
//		                                  coordIndices .end (),
//		                                  [ ] (int32_t l, int32_t r) { return l == -1 && r == -1; }),
//		                     coordIndices .end ());
//
//		// Special case.
//
//		if (coordIndices .size () == 1 and coordIndices [0] == -1)
//			coordIndices .clear ();
//
//		if (coordIndices .empty ())
//			points .resize (0);
//
//		// Assign values.
//
//		for (const auto & indexedLineSet : indexedLineSets)
//		{
//			toMetaData (indexedLineSet -> createMetadataSet ("/IndexedLineSet/options"));
//
//			if (indexedLineSet -> colorPerVertex ())
//				indexedLineSet -> colorIndex () .assign (colorPerVertexIndices .begin (), colorPerVertexIndices .end ());
//			else
//				indexedLineSet -> colorIndex () .assign (colorPerLineIndices .begin (), colorPerLineIndices .end ());
//
//			indexedLineSet -> coordIndex () .assign (coordIndices .begin (), coordIndices .end ());
//
//			if (indexedLineSet -> getCoord ())
//			{
//				for (size_t i = 0, size = points .size (); i < size; ++ i)
//					indexedLineSet -> getCoord () -> set1Point (i, points [i]);
//
//				indexedLineSet -> getCoord () -> resize (points .size ());
//			}
//			else
//			{
//				const auto coord = getExecutionContext () -> createNode <Coordinate> ();
//
//				indexedLineSet -> coord () = coord;
//
//				for (size_t i = 0, size = points .size (); i < size; ++ i)
//					coord -> set1Point (i, points [i]);
//			}
//		}
	}
	catch (const std::exception & error)
	{
		getBrowser () -> println (error .what ());
	}
}

} // X3D
} // titania
