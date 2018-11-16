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

#ifndef __TITANIA_X3D_MISCELLANEOUS_BVH_H__
#define __TITANIA_X3D_MISCELLANEOUS_BVH_H__

#include <Titania/Math/Geometry/Box3.h>
#include <Titania/Math/Geometry/Triangle3.h>
#include <Titania/Math/Numbers/Vector3.h>
#include <Titania/Math/Numbers/Matrix4.h>
#include <Titania/Utility/Range.h>

#include <memory>
#include <vector>

#include <algorithm>
#include <functional>
#include <numeric>

namespace titania {
namespace X3D {

using namespace math;

/**
 *  BVH - class to represent a Bounded volume hierarchy.
 */

template <class Type>
class BVH
{
public:

	///  @name Static members

	static constexpr int32_t NODE     = 0;
	static constexpr int32_t TRIANGLE = 1;

	///  @name Member types

	struct ArrayValue
	{
		ArrayValue (int32_t type, vector3 <Type> min, vector3 <Type> max, int32_t left, int32_t right) :
			 type (type),
			  min (min),
			  max (max),
			 left (left),
			right (right)
	     	{ }

		int32_t        type;
		vector3 <Type> min;
		vector3 <Type> max;
		int32_t        left;
		int32_t        right;
	};

	///  @name Construction

	BVH (const std::vector <vector3 <Type>> & normals,
	     const std::vector <vector3 <Type>> & vertices);

	BVH (std::vector <vector3 <Type>> && normals,
	     std::vector <vector3 <Type>> && vertices);

	///  @name Member access

	const std::vector <vector3 <Type>> &
	getNormals () const
	{ return normals; }

	const std::vector <vector3 <Type>> &
	getVertices () const
	{ return vertices; }

	///  @name Operations

	size_t
	intersects (const line3 <Type> & line,
	            std::vector <vector3 <Type>> & intersections,
	            std::vector <vector3 <Type>> & intersectionNormals) const;

	bool
	intersects (const box3 <Type> & bbox,
	            const std::vector <vector3 <Type>> & points,
	            const std::vector <vector3 <Type>> & edges,
	            const std::vector <vector3 <Type>> & normals,
	            const matrix4 <Type> & matrix,
	            std::vector <vector3 <Type>> & triangles) const;

	///  @name Input/Output

	std::vector <ArrayValue>
	toArray () const;

	///  @name Destruction

	virtual
	~BVH ();

private:

	class SortComparator;
	class MedianComparator;

	class BVHNode;
	class Triangle;
	class Node;
	
	///  @name Members

	std::vector <vector3 <Type>> normals;
	std::vector <vector3 <Type>> vertices;
	std::unique_ptr <BVHNode>    root;

};

// BVH - Bounded volume hierarchy

template <class Type>
class BVH <Type>::SortComparator
{
public:

	SortComparator (BVH* const tree, const size_t axis) :
		tree (tree),
		axis (axis)
	{ }

	bool
	operator () (const size_t & a, const size_t & b) const
	{
		return min (a) < min (b);
	}

private:

	Type
	min (const size_t triangle) const
	{
		const size_t i = triangle * 3;

		return std::min ({ tree -> vertices [i] [axis],
		                   tree -> vertices [i + 1] [axis],
		                   tree -> vertices [i + 2] [axis] });
	}

	BVH* const   tree;
	const size_t axis;

};

template <class Type>
class BVH <Type>::MedianComparator
{
public:

	MedianComparator (BVH* const tree, const size_t axis) :
		tree (tree),
		axis (axis)
	{ }

	bool
	operator () (const size_t & a, const Type & value) const
	{
		return min (a) < value;
	}

private:

	Type
	min (const size_t triangle) const
	{
		const size_t i = triangle * 3;

		return std::min ({ tree -> vertices [i] [axis],
		                   tree -> vertices [i + 1] [axis],
		                   tree -> vertices [i + 2] [axis] });
	}

	BVH* const   tree;
	const size_t axis;

};

// BVH::BVHNode

template <class Type>
class BVH <Type>::BVHNode
{
public:

	BVHNode (BVH* const tree) :
		tree (tree)
	{ }

	virtual
	void
	intersects (const line3 <Type> & line,
	            std::vector <vector3 <Type>> & intersections,
	            std::vector <vector3 <Type>> & intersectionNormals) const = 0;

	virtual
	bool
	intersects (const box3 <Type> & bbox,
	            const std::vector <vector3 <Type>> & points,
	            const std::vector <vector3 <Type>> & edges,
	            const std::vector <vector3 <Type>> & normals,
	            const matrix4 <Type> & matrix,
	            std::vector <vector3 <Type>> & triangles) const = 0;

	virtual
	size_t
	toArray (std::vector <ArrayValue> &) const = 0;

protected:

	const vector3 <Type> &
	getNormal (const size_t triangle, const size_t index) const
	{ return tree -> normals [triangle * 3 + index]; }

	const vector3 <Type> &
	getVertex (const size_t triangle, const size_t index) const
	{ return tree -> vertices [triangle * 3 + index]; }

	Type
	getMin (const size_t triangle, const size_t axis) const
	{
		const size_t i = triangle * 3;

		return std::min ({ tree -> vertices [i] [axis],
		                   tree -> vertices [i + 1] [axis],
		                   tree -> vertices [i + 2] [axis] });
	}

private:

	///  @name Members

	BVH* const tree;

};

template <class Type>
class BVH <Type>::Triangle :
	public BVHNode
{
public:

	Triangle (BVH* const tree, size_t triangle) :
		 BVHNode (tree),
		triangle (triangle)
	{ }

	virtual
	void
	intersects (const line3 <Type> & line,
	            std::vector <vector3 <Type>> & intersections,
	            std::vector <vector3 <Type>> & intersectionNormals) const final override
	{
		const auto & a = this -> getVertex (triangle, 0);
		const auto & b = this -> getVertex (triangle, 1);
		const auto & c = this -> getVertex (triangle, 2);

		const auto & [intersection, intersected] = line .intersects (a, b, c);

		if (intersected)
		{
			const auto u = intersection .x ();
			const auto v = intersection .y ();
			const auto t = intersection .z ();

			// Determine vectors for X3DPointingDeviceSensors.

			intersections .emplace_back (u * a + v * b + t * c);

			const auto & n0 = this -> getNormal (triangle, 0);
			const auto & n1 = this -> getNormal (triangle, 1);
			const auto & n2 = this -> getNormal (triangle, 2);

			intersectionNormals .emplace_back (t * n0 + u * n1 + v * n2);
		}
	}

	virtual
	bool
	intersects (const box3 <Type> & bbox,
	            const std::vector <vector3 <Type>> & points,
	            const std::vector <vector3 <Type>> & edges,
	            const std::vector <vector3 <Type>> & normals,
	            const matrix4 <Type> & matrix,
	            std::vector <vector3 <Type>> & triangles) const final override
	{
		const auto & a = this -> getVertex (triangle, 0);
		const auto & b = this -> getVertex (triangle, 1);
		const auto & c = this -> getVertex (triangle, 2);

		if (triangle3 <Type> (a * matrix, b * matrix, c * matrix) .intersects (points, edges, normals))
		{
			triangles .emplace_back (a);
			triangles .emplace_back (b);
			triangles .emplace_back (c);

			return true;
		}

		return false;
	}

	virtual
	size_t
	toArray (std::vector <ArrayValue> & array) const final override
	{
		const size_t index = array .size ();

		array .emplace_back (TRIANGLE, vector3 <Type> (), vector3 <Type> (), triangle * 3, 0);

		return index;
	}


private:

	const size_t triangle;

};

// BVH::Node

template <class Type>
class BVH <Type>::Node :
	public BVHNode
{
public:

	Node (BVH* const tree, std::vector <size_t> & triangles, size_t first, size_t size) :
		BVHNode (tree),
		   bbox (),
		   left (),
		  right ()
	{
		using namespace std::placeholders;

		const size_t last  = first + size;
		const auto   begin = triangles .begin () + first;
		const auto   end   = triangles .begin () + last;

		// Calculate bbox

		auto min = this -> getVertex (*begin, 0);
		auto max = min;

		for (const auto & triangle : std::make_pair (begin, end))
		{
			for (size_t i = 0; i < 3; ++ i)
			{
				vector3 <Type> vertex (this -> getVertex (triangle, i));
				min = math::min (min, vertex);
				max = math::max (max, vertex);
			}
		}

		bbox = box3 <Type> (min, max, extents_type ());

		// Sort and split array

		size_t leftSize = 0;

		if (size > 2)
		{
			// Sort array

			const size_t axis = getLongestAxis (min, max);

			std::sort (begin, end, SortComparator (tree, axis));

			// Split array

			const auto value = (this -> getMin (*begin, axis) + this -> getMin (*(end - 1), axis)) / 2;
			const auto iter  = std::lower_bound (begin, end, value, MedianComparator (tree, axis));

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
			left .reset (new Triangle (tree, triangles [first]));

		if (rightSize > 1)
			right .reset (new Node (tree, triangles, first + leftSize, rightSize));
		else
			right .reset (new Triangle (tree, triangles [first + leftSize]));
	}

	virtual
	void
	intersects (const line3 <Type> & line,
	            std::vector <vector3 <Type>> & intersections,
	            std::vector <vector3 <Type>> & intersectionNormals) const final override
	{
		if (bbox .intersects (line))
		{
			left  -> intersects (line, intersections, intersectionNormals);
			right -> intersects (line, intersections, intersectionNormals);
		}
	}

	virtual
	bool
	intersects (const box3 <Type> & bbox,
	            const std::vector <vector3 <Type>> & points,
	            const std::vector <vector3 <Type>> & edges,
	            const std::vector <vector3 <Type>> & normals,
	            const matrix4 <Type> & matrix,
	            std::vector <vector3 <Type>> & triangles) const final override
	{
		if (this -> bbox .intersects (bbox))
		{
			bool intersects = false;

			intersects |= left -> intersects  (bbox, points, edges, normals, matrix, triangles);
			intersects |= right -> intersects (bbox, points, edges, normals, matrix, triangles);

			return intersects;
		}

		return false;
	}

	virtual
	size_t
	toArray (std::vector <ArrayValue> & array) const final override
	{
		const size_t leftIndex  = left -> toArray (array);
		const size_t rightIndex = right -> toArray (array);
		const size_t index      = array .size ();
		const auto   extents    = bbox .extents ();

		array .emplace_back (NODE, extents .first, extents .second, leftIndex, rightIndex);

		return index;
	}

private:

	size_t
	getLongestAxis (const vector3 <Type> & min, const vector3 <Type> & max) const
	{
		const vector3 <Type> size = max - min;

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

	box3 <Type> bbox;

	std::unique_ptr <BVHNode> left;
	std::unique_ptr <BVHNode> right;

};

// BVH

template <class Type>
BVH <Type>::BVH (const std::vector <vector3 <Type>> & normals_,
                 const std::vector <vector3 <Type>> & vertices_) :
	BVH (std::vector <vector3 <Type>> (normals_), std::vector <vector3 <Type>> (vertices_))
{ }

template <class Type>
BVH <Type>::BVH (std::vector <vector3 <Type>> && normals_,
                 std::vector <vector3 <Type>> && vertices_) :
	 normals (std::move (normals_)),
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

template <class Type>
size_t
BVH <Type>::intersects (const line3 <Type> & line,
                        std::vector <vector3 <Type>> & intersections,
                        std::vector <vector3 <Type>> & intersectionNormals) const
{
	if (root)
		root -> intersects (line, intersections, intersectionNormals);

	return intersections .size ();
}

template <class Type>
bool
BVH <Type>::intersects (const box3 <Type> & bbox,
	                     const std::vector <vector3 <Type>> & points,
                        const std::vector <vector3 <Type>> & edges,
                        const std::vector <vector3 <Type>> & normals,
                        const matrix4 <Type> & matrix,
                        std::vector <vector3 <Type>> & triangles) const
{
	try
	{
		if (root)
			return root -> intersects (bbox * inverse (matrix), points, edges, normals, matrix, triangles);
	}
	catch (const std::domain_error &)
	{ }

	return false;
}

template <class Type>
std::vector <typename BVH <Type>::ArrayValue>
BVH <Type>::toArray () const
{
	std::vector <ArrayValue> array;

	if (root)
		root -> toArray (array);

	return array;
}

template <class Type>
BVH <Type>::~BVH ()
{ }

} // X3D
} // titania

#endif
