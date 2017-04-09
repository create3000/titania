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

#ifndef __TITANIA_MATH_ALGORITHMS_TURTLE_RENDERER_H__
#define __TITANIA_MATH_ALGORITHMS_TURTLE_RENDERER_H__

#include "../Numbers/Vector3.h"
#include "../Utility/almost_equal.h"
#include "L-System.h"

#include <chrono>
#include <memory>
#include <random>
#include <vector>

namespace titania {
namespace math {

template <class Type>
struct turtle_renderer_node {

	using value_type    = Type;
	using node_ptr      = std::shared_ptr <turtle_renderer_node>;
	using children_type = std::vector <node_ptr>;

	turtle_renderer_node (const vector3 <Type> & point, const int32_t color) :
		     point (point),
		     color (color),
		line_color (-1),
		 direction (),
		  distance (0),
		  children ()
	{ }

	vector3 <Type> point;
	int32_t        color;
	int32_t        line_color;
	vector3 <Type> direction;
	Type           distance;
	children_type  children;

};

template <class Type, class String>
class turtle_renderer {
public:

	///  @name Member types

	using value_type  = Type;
	using string_type = String;
	using node_type   = turtle_renderer_node <Type>;
	using node_ptr    = typename node_type::node_ptr;

	///  @name Construction

	turtle_renderer ();

	turtle_renderer (const Type & x_angle, const Type & y_angle, const Type & z_angle, const lsystem <String> & lsystem, const Type & tolerance = 0);

	///  @name Members

	void
	x_angle (const Type & value)
	{ mx_angle = value; }

	const Type &
	x_angle () const
	{ return mx_angle; }

	void
	y_angle (const Type & value)
	{ my_angle = value; }

	const Type &
	y_angle () const
	{ return my_angle; }

	void
	z_angle (const Type & value)
	{ mz_angle = value; }

	const Type &
	z_angle () const
	{ return mz_angle; }

	void
	angle_variation (const Type & value)
	{ m_angle_variation = value; }

	const Type &
	angle_variation () const
	{ return m_angle_variation; }

	void
	length_variation (const Type & value)
	{ m_length_variation = value; }

	const Type &
	length_variation () const
	{ return m_length_variation; }

	void
	tolerance (const Type & value)
	{ m_tolerance = value; }

	const Type &
	tolerance () const
	{ return m_tolerance; }

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

	///  @name Operations

	Type
	variation (const Type & value, const Type & variation);

	Type
	random (const Type & min, const Type & max);

	///  @name Static members

	static std::uniform_real_distribution <Type> uniform_real_distribution;
	static std::default_random_engine            random_engine;

	///  @name Members

	Type     mx_angle;
	Type     my_angle;
	Type     mz_angle;
	Type     m_angle_variation;
	Type     m_length_variation;
	Type     m_tolerance;
	bool     m_colors;
	node_ptr m_tree; 

};

template <class Type, class String>
std::uniform_real_distribution <Type>
turtle_renderer <Type, String>::uniform_real_distribution (-1, 1);

template <class Type, class String>
std::default_random_engine
turtle_renderer <Type, String>::random_engine (std::chrono::system_clock::now () .time_since_epoch () .count ());


template <class Type, class String>
turtle_renderer <Type, String>::turtle_renderer () :
	          mx_angle (0),
	          my_angle (0),
	          mz_angle (0),
	 m_angle_variation (0),
	m_length_variation (0),
	       m_tolerance (0),
	          m_colors (false),
	            m_tree ()
{ }       

template <class Type, class String>
turtle_renderer <Type, String>::turtle_renderer (const Type & x_angle, const Type & y_angle, const Type & z_angle, const lsystem <String> & lsystem, const Type & tolerance) :
	          mx_angle (x_angle),
	          my_angle (y_angle),
	          mz_angle (z_angle),
	 m_angle_variation (0),
	m_length_variation (0),
	       m_tolerance (tolerance),
	          m_colors (false),
	            m_tree ()
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
		             const rotation4 <Type> & rotation,
		             const int32_t color) :
			    node (node),
			   point (point),
			rotation (rotation),
			   color (color)
		{ }
	
		const node_ptr         node;
		const vector3 <Type>   point;
		const rotation4 <Type> rotation;
		const int32_t          color;
	};

	bool    change        = true;                                        // True, if a new point must be inserted.
	int32_t color         = 0;                                           // Current color index.
	int32_t lastColor     = -1;                                          // First color applied to node.
	auto    rotation      = rotation4 <Type> ();                         // Rotation. The y-axis is the direction vector for a new step.
	auto    point         = vector3 <Type> ();                           // Current point in space.
	auto    lastDirection = vector3 <Type> ();  
	Type    distance      = 0;                                           // Number of steps.
	auto    root          = std::make_shared <node_type> (point, color); // Root node of tree.
	auto    node          = root;                                        // Current node.
	auto    stack         = std::vector <stack_value> ();                // Stack for '[' and ']'.

	std::istringstream isstream (lsystem .commands ());

	while (isstream)
	{
		const auto c = isstream .get ();

		switch (c)
		{
			case '#': // Color
			{
				int32_t index;

				if ((isstream >> index) and (index >= 0))
				{
					m_colors      = true;
					change        = index not_eq lastColor;
					color         = index;
					node -> color = index;
				}

				break;
			}
			case '\\': // Counterclockwise rotation about local x-axis
			{
				rotation *= rotation4 <Type> (vector3 <Type> (1, 0, 0) * rotation, variation (mx_angle, angle_variation ()));
				break;
			}
			case '/': // Clockwise rotation about local x-axis
			{
					rotation *= rotation4 <Type> (vector3 <Type> (1, 0, 0) * rotation, -variation (mx_angle, angle_variation ()));
			break;
			}
			case '>': // Counterclockwise rotation about local y-axis
			{
				rotation *= rotation4 <Type> (vector3 <Type> (0, 1, 0) * rotation, variation (my_angle, angle_variation ()));
				break;
			}
			case '<': // Clockwise rotation about local y-axis
			{
				rotation *= rotation4 <Type> (vector3 <Type> (0, 1, 0) * rotation, -variation (my_angle, angle_variation ()));
				break;
			}
			case '+': // Counterclockwise rotation about local z-axis
			{
				rotation *= rotation4 <Type> (vector3 <Type> (0, 0, 1) * rotation, variation (mz_angle, angle_variation ()));
				break;
			}
			case '-': // Clockwise rotation about local z-axis
			{
				rotation *= rotation4 <Type> (vector3 <Type> (0, 0, 1) * rotation, -variation (mz_angle, angle_variation ()));
				break;
			}
			case '|': // Turn around 180°„
			{
				rotation *= rotation4 <Type> (vector3 <Type> (0, 0, 1) * rotation, pi <Type>);
				break;
			}
			case '[': // Push
			{
				stack .emplace_back (node, point, rotation, color);

				change    = true;
				lastColor = -1;
				distance  = 0;
				break;
			}
			case ']': // Pop
			{
				if (stack .empty ())
					break;

				const auto & back = stack .back ();

				change    = true;
				color     = back .color;
				lastColor = -1;
				rotation  = back .rotation;
				point     = back .point;
				distance  = 0;
				node      = back .node;

				stack .pop_back ();
				break;
			}
			default: // Line
			{
				if (c < 0)
					break;

				if (lsystem .is_constant (c))
					break;

				const auto length    = variation (1, length_variation ());
				const auto direction = vector3 <Type> (0, 1, 0) * rotation;

				point    += direction * length;
				distance += length;

				if (change or not almost_equal (lastDirection, direction, tolerance ()))
				{
					change        = false;
					lastColor     = color;
					lastDirection = direction;

					auto child = std::make_shared <node_type> (point, color);

					child -> color      = color;
					child -> line_color = color;
					child -> direction  = direction;
					child -> distance   = distance;

					node -> children .emplace_back (child);
	
					node     = std::move (child);
					distance = 0;
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
Type
turtle_renderer <Type, String>::variation (const Type & value, const Type & variation)
{
	const Type v = value * variation;

	return random (std::max <Type> (0, value - v), value + v);
}

template <class Type, class String>
Type
turtle_renderer <Type, String>::random (const Type & min, const Type & max)
{
	return min + ((uniform_real_distribution (random_engine) + 1) / 2) * (max - min);
}

template <class Type, class String>
turtle_renderer <Type, String>::~turtle_renderer ()
{
	// Manually dispose tree.

	if (m_tree)
	{
		auto nodes = std::vector <node_ptr> ();
		auto stack = std::vector <node_ptr> ();

		stack .emplace_back (std::move (m_tree));

		while (not stack .empty ())
		{
			nodes .emplace_back (std::move (stack .back ()));

			stack .pop_back ();

			for (auto & child : nodes .back () -> children)
				stack .emplace_back (std::move (child));
		}
	}
}

} // math
} // titania

#endif
