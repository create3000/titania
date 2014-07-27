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

#include "BVH.h"

#include <Titania/LOG.h>
#include <Titania/Utility/Range.h>
#include <algorithm>

namespace titania {
namespace X3D {

// BVH - Bounded volume hierarchy

static constexpr int32_t BVH_NODE     = 0;
static constexpr int32_t BVH_TRIANGLE = 1;

struct BVH::SortComparator
{
	SortComparator (BVH* const tree) :
		tree (tree)
	{ }

	bool
	operator () (const size_t & a, const size_t & b, size_t axis) const
	{
		return min (a, axis) < min (b, axis);
	}

	float
	min (size_t triangle, size_t axis) const
	{
		size_t i = triangle * 3;

		return std::min ({ tree -> vertices [i] [axis],
		                   tree -> vertices [i + 1] [axis],
		                   tree -> vertices [i + 2] [axis] });
	}

	BVH* const tree;

};

struct BVH::MedianComparator
{
	MedianComparator (BVH* const tree) :
		tree (tree)
	{ }

	bool
	operator () (const size_t & a, const float & value, size_t axis) const
	{
		return min (a, axis) < value;
	}

	float
	min (size_t triangle, size_t axis) const
	{
		const size_t i = triangle * 3;

		return std::min ({ tree -> vertices [i] [axis],
		                   tree -> vertices [i + 1] [axis],
		                   tree -> vertices [i + 2] [axis] });
	}

	BVH* const tree;

};

// BVH::X3DNode

class BVH::X3DNode
{
public:

	X3DNode (BVH* const tree) :
		tree (tree)
	{ }

	const Vector3f &
	getVertex (size_t triangle, size_t index) const
	{ return tree -> vertices [triangle * 3 + index]; }

	float
	getMin (size_t triangle, size_t axis) const
	{
		const size_t i = triangle * 3;

		return std::min ({ tree -> vertices [i] [axis],
		                   tree -> vertices [i + 1] [axis],
		                   tree -> vertices [i + 2] [axis] });
	}

	virtual
	size_t
	toArray (std::vector <ArrayValue> &) const = 0;

	///  @name Members

	BVH* const tree;

};

// BVH::Triangle

class BVH::Triangle :
	public X3DNode
{
public:

	Triangle (BVH* const tree, size_t triangle) :
		 X3DNode (tree),
		triangle (triangle)
	{ }

	virtual
	size_t
	toArray (std::vector <ArrayValue> & array) const
	{
		const size_t index = array .size ();

		array .emplace_back (BVH_TRIANGLE, Vector3f (), Vector3f (), triangle, 0);

		return index;
	}

	size_t triangle;

};

// BVH::Node

class BVH::Node :
	public X3DNode
{
public:

	Node (BVH* const tree, std::vector <size_t> & triangles, size_t first, size_t size) :
		X3DNode (tree),
		    min (),
		    max (),
		   left (),
		  right ()
	{
		using namespace std::placeholders;

		const size_t last  = first + size;
		const auto   begin = triangles .begin () + first;
		const auto   end   = triangles .begin () + last;

		// Calculate bbox

		min = getVertex (*begin, 0);
		max = min;

		for (const auto & triangle : std::make_pair (begin, end))
		{
			for (size_t i = 0; i < 3; ++ i)
			{
				Vector3f vertex (getVertex (triangle, i));
				min = math::min (min, vertex);
				max = math::max (max, vertex);
			}
		}

		// Sort and split array

		size_t leftSize = 0;

		if (size > 2)
		{
			// Sort array

			const size_t axis = getLongestAxis (min, max);

			std::sort (begin, end, std::bind (SortComparator (tree), _1, _2, axis));

			// Split array

			const float value = (getMin (*begin, axis) + getMin (*(end - 1), axis)) / 2;
			const auto  iter  = std::lower_bound (begin, end, value, std::bind (MedianComparator (tree), _1, _2, axis));

			leftSize = iter - begin;

			if (leftSize == 0 or leftSize == size)
				leftSize = size >> 1;
		}
		else
			leftSize = size >> 1;

		size_t rightSize = size - leftSize;

		// Construct left and right node

		if (leftSize > 1)
			left  .reset (new Node (tree, triangles, first, leftSize));
		else
			left .reset (new Triangle (tree, triangles [first] * 3));

		if (rightSize > 1)
			right .reset (new Node (tree, triangles, first + leftSize, rightSize));
		else
			right .reset (new Triangle (tree, triangles [first + leftSize] * 3));
	}

	virtual
	size_t
	toArray (std::vector <ArrayValue> & array) const
	{
		const size_t leftIndex  = left -> toArray (array);
		const size_t rightIndex = right -> toArray (array);
		const size_t index      = array .size ();

		array .emplace_back (BVH_NODE, min, max, leftIndex, rightIndex);

		return index;
	}

private:

	size_t
	getLongestAxis (const Vector3f & min, const Vector3f & max) const
	{
		const Vector3f size = max - min;

		if (size .x () < size .y ())
		{
			if (size .y () < size .z ())
				return 2;
			else
				return 1;
		}
		else
		{
			if (size .x () < size .z ())
				return 2;
			else
				return 0;
		}
	}

	///  @name Members

	Vector3f min;
	Vector3f max;

	std::unique_ptr <X3DNode> left;
	std::unique_ptr <X3DNode> right;

};

// BVH

BVH::BVH (std::vector <Vector3f> && vertices_) :
	vertices (std::move (vertices_)),
	    root ()
{
	const size_t size = vertices .size () / 3;

	switch (size)
	{
		case 0:
			break;
		case 1:
		{
			root .reset (new BVH::Triangle (this, 0));
			break;
		}
		default:
		{
			std::vector <size_t> triangles (size);
			std::iota (triangles .begin (), triangles .end (), 0);

			root .reset (new BVH::Node (this, triangles, 0, size));
			break;
		}
	}
}

std::vector <BVH::ArrayValue>
BVH::toArray () const
{
	std::vector <ArrayValue> array;

	if (root)
		root -> toArray (array);

	return array;
}

BVH::~BVH ()
{ }

} // X3D
} // titania/
