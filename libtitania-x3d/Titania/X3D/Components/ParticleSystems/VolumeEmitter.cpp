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

#include "VolumeEmitter.h"

#include "../../Bits/config.h"
#include "../../Execution/X3DExecutionContext.h"

#include <Titania/Math/Utility/Area.h>

namespace titania {
namespace X3D {

const std::string VolumeEmitter::componentName  = "ParticleSystems";
const std::string VolumeEmitter::typeName       = "VolumeEmitter";
const std::string VolumeEmitter::containerField = "emitter";

// TriangleTree

static constexpr int32_t TRIANGLE_TREE_NODE     = 0;
static constexpr int32_t TRIANGLE_TREE_TRIANGLE = 1;

class TriangleTree
{
public:

	struct ArrayValue
	{
		ArrayValue (int32_t type, Vector3f min, Vector3f max, int32_t left, int32_t right) :
			 type (type),
			  min (min),
			  max (max),
			 left (left),
			right (right)
		{ }

		int32_t  type;
		Vector3f min;
		Vector3f max;
		int32_t  left;
		int32_t  right;
	};

	TriangleTree (std::vector <Vector3f>&&);

	std::vector <ArrayValue>
	toArray () const;

private:

	class SortComparator;
	class MedianComparator;

	class X3DNode;
	class Triangle;
	class Node;

	// Members
	
	std::vector <Vector3f>    vertices;
	std::unique_ptr <X3DNode> root;

};

struct TriangleTree::SortComparator
{
	SortComparator (TriangleTree* const tree) :
		tree (tree)
	{ }

	bool
	operator () (const size_t & a, const size_t & b, size_t axis) const
	{
		return min (a, axis) < min (b, axis);
		return center (a, axis) < center (b, axis);
	}
	
	float
	min (size_t triangle, size_t axis) const
	{
		size_t i = triangle * 3;

		return std::min ({ tree -> vertices [i] [axis],
			                tree -> vertices [i + 1] [axis],
			                tree -> vertices [i + 2] [axis] });
	}
	
	float
	center (size_t triangle, size_t axis) const
	{
		size_t i = triangle * 3;

		return (tree -> vertices [i] [axis] +
			     tree -> vertices [i + 1] [axis] +
			     tree -> vertices [i + 2] [axis]) / 3;
	}

	TriangleTree* const tree;

};

struct TriangleTree::MedianComparator
{
	MedianComparator (TriangleTree* const tree) :
		tree (tree)
	{ }

	bool
	operator () (const size_t & a, const float & value, size_t axis) const
	{
		return min (a, axis) < value;
		return center (a, axis) < value;
	}

	float
	min (size_t triangle, size_t axis) const
	{
		size_t i = triangle * 3;

		return std::min ({ tree -> vertices [i] [axis],
			                tree -> vertices [i + 1] [axis],
			                tree -> vertices [i + 2] [axis] });
	}
	
	float
	center (size_t triangle, size_t axis) const
	{
		size_t i = triangle * 3;

		return (tree -> vertices [i] [axis] +
			     tree -> vertices [i + 1] [axis] +
			     tree -> vertices [i + 2] [axis]) / 3;
	}

	TriangleTree* const tree;

};

// TriangleTree::X3DNode

class TriangleTree::X3DNode
{
public:

	X3DNode (TriangleTree* const tree) :
		tree (tree)
	{ }
	
	const Vector3f &
	getVertex (size_t triangle, size_t index) const
	{ return tree -> vertices [triangle * 3 + index]; }

	float
	getMin (size_t triangle, size_t axis) const
	{
		size_t i = triangle * 3;

		return std::min ({ tree -> vertices [i] [axis],
			                tree -> vertices [i + 1] [axis],
			                tree -> vertices [i + 2] [axis] });
	}

	virtual
	size_t
	toArray (std::vector <ArrayValue> &) const = 0;

	// Members

	TriangleTree* const tree;

};

// TriangleTree::Triangle

class TriangleTree::Triangle :
	public X3DNode
{
public:

	Triangle (TriangleTree* const tree, size_t triangle) :
		 X3DNode (tree),
		triangle (triangle)
	{ }

	virtual
	size_t
	toArray (std::vector <ArrayValue> & array) const
	{
		size_t index = array .size ();

		array .emplace_back (TRIANGLE_TREE_TRIANGLE, Vector3f (), Vector3f (), triangle, 0);

		return index;
	}

	size_t triangle;

};

// TriangleTree::Node

class TriangleTree::Node :
	public X3DNode
{
public:

	Node (TriangleTree* const tree, std::vector <size_t> & triangles, size_t first, size_t size) :
		X3DNode (tree),
		    min (),
		    max (),
		   left (),
		  right ()
	{
		using namespace std::placeholders;

		size_t last  = first + size;
		auto   begin = triangles .begin () + first;
		auto   end   = triangles .begin () + last;
	
		// Calculate bbox

		min = getVertex (*begin, 0);
		max = min;

		for (const auto & triangle : basic::adapter (begin, end))
		{
			for (size_t i = 0; i < 3; ++ i)
			{
				Vector3f vertex (getVertex (triangle, i));
				min = math::min (min, vertex);
				max = math::max (max, vertex);
			}
		}

		// Sort array
		
		size_t axis = getLongestAxis (min, max);

		std::sort (begin, end, std::bind (SortComparator (tree), _1, _2, axis));

		// Split array

		size_t leftSize = 0;

		if (size > 2)
		{
			//float value = (min [axis] + max [axis]) / 2;
			float value = (getMin (*begin, axis) + getMin (*(end - 1), axis)) / 2;
			auto  iter  = std::lower_bound (begin, end, value, std::bind (MedianComparator (tree), _1, _2, axis));
			leftSize    = iter - begin;

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
		size_t leftIndex  = left -> toArray (array);
		size_t rightIndex = right -> toArray (array);
		size_t index      = array .size ();

		array .emplace_back (TRIANGLE_TREE_NODE, min, max, leftIndex, rightIndex);

		return index;
	}

private:

	size_t
	getLongestAxis (const Vector3f & min, const Vector3f & max) const
	{
		Vector3f size = max - min;

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

	// Members

	Vector3f min;
	Vector3f max;

	std::unique_ptr <X3DNode> left;
	std::unique_ptr <X3DNode> right;

};

// TriangleTree::TriangleTree

TriangleTree::TriangleTree (std::vector <Vector3f>&& _vertices) :
	vertices (std::move (_vertices)),
	    root ()
{
	size_t size = vertices .size () / 3;

	switch (size)
	{
		case 0:
			break;
		case 1:
		{
			root .reset (new TriangleTree::Triangle (this, 0));
			break;
		}
		default:
		{
			std::vector <size_t> triangles (size);
			std::iota (triangles .begin (), triangles .end (), 0);

			root .reset (new TriangleTree::Node (this, triangles, 0, size));
			break;
		}
	}
}

std::vector <TriangleTree::ArrayValue>
TriangleTree::toArray () const
{
	std::vector <ArrayValue> array;
	
	if (root)
		root -> toArray (array);

	return array;
}

// VolumeEmitter

VolumeEmitter::Fields::Fields () :
	  internal (new SFBool (true)),
	 direction (new SFVec3f (0, 1, 0)),
	coordIndex (new MFInt32 ({ -1 })),
	     coord (new SFNode ())
{ }

VolumeEmitter::VolumeEmitter (X3DExecutionContext* const executionContext) :
	           X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DParticleEmitterNode (),
	                fields (),
	           normalMapId (0),
	        normalBufferId (0),
	          surfaceMapId (0),
	       surfaceBufferId (0),
	      surfaceAreaMapId (0),
	   surfaceAreaBufferId (0),
	     triangleTreeMapId (0),
	  triangleTreeBufferId (0),
	           surfaceNode (new IndexedFaceSet (getExecutionContext ())),
	          pointEmitter (false),
	                 solid (true)
{
	addField (inputOutput,    "metadata",       metadata ());
	addField (initializeOnly, "internal",       internal ());
	addField (inputOutput,    "direction",      direction ());
	addField (inputOutput,    "speed",          speed ());
	addField (inputOutput,    "variation",      variation ());
	addField (initializeOnly, "mass",           mass ());
	addField (initializeOnly, "surfaceArea",    surfaceArea ());
	addField (inputOutput,    "coordIndex",     coordIndex ());
	addField (inputOutput,    "coord",          coord ());

	addChildren (surfaceNode);
}

X3DBaseNode*
VolumeEmitter::create (X3DExecutionContext* const executionContext) const
{
	return new VolumeEmitter (executionContext);
}

void
VolumeEmitter::initialize ()
{
	X3DParticleEmitterNode::initialize ();

	// Surface map

	glGenTextures (1, &normalMapId);
	glGenBuffers  (1, &normalBufferId);

	glGenTextures (1, &surfaceMapId);
	glGenBuffers  (1, &surfaceBufferId);

	glGenTextures (1, &surfaceAreaMapId);
	glGenBuffers  (1, &surfaceAreaBufferId);

	glGenTextures (1, &triangleTreeMapId);
	glGenBuffers  (1, &triangleTreeBufferId);

	// Setup

	coordIndex () .addInterest (surfaceNode -> coordIndex ());
	coord ()      .addInterest (surfaceNode -> coord ());

	surfaceNode -> isInternal (true);
	surfaceNode -> creaseAngle () = 0;
	surfaceNode -> convex ()      = false;
	surfaceNode -> coordIndex ()  = coordIndex ();
	surfaceNode -> coord ()       = coord ();
	surfaceNode -> addInterest (this, &VolumeEmitter::set_geometry);

	surfaceNode -> setup ();

	set_geometry ();
}

void
VolumeEmitter::set_geometry ()
{
	// Triangulate

	std::vector <Color4f>  colors;
	TexCoordArray          texCoords (surfaceNode -> getMultiTexCoords ());
	std::vector <Vector3f> normals;
	std::vector <Vector3f> vertices;

	surfaceNode -> triangulate (colors, texCoords, normals, vertices);

	float               surfaceArea = 0;
	std::vector <float> surfaceAreas (1);

	for (size_t i = 0, size = vertices .size (); i < size; i += 3)
	{
		surfaceArea += area (vertices [i], vertices [i + 1], vertices [i + 2]);
		surfaceAreas .emplace_back (surfaceArea);
	}

	// Upload

	pointEmitter = vertices .empty ();
	solid        = surfaceNode -> getSolid ();

	glBindBuffer (GL_TEXTURE_BUFFER, normalBufferId);
	glBufferData (GL_TEXTURE_BUFFER, normals .size () * sizeof (Vector3f), pointEmitter ? 0 : normals .data (), GL_STATIC_COPY);

	glBindBuffer (GL_TEXTURE_BUFFER, surfaceBufferId);
	glBufferData (GL_TEXTURE_BUFFER, vertices .size () * sizeof (Vector3f), pointEmitter ? 0 : vertices .data (), GL_STATIC_COPY);

	glBindBuffer (GL_TEXTURE_BUFFER, surfaceAreaBufferId);
	glBufferData (GL_TEXTURE_BUFFER, surfaceAreas .size () * sizeof (float), pointEmitter ? 0 : surfaceAreas .data (), GL_STATIC_COPY);

	// Triangle tree

	TriangleTree tree (std::move (vertices));

	auto treeArray = std::move (tree .toArray ());

	glBindBuffer (GL_TEXTURE_BUFFER, triangleTreeBufferId);
	glBufferData (GL_TEXTURE_BUFFER, treeArray .size () * sizeof (TriangleTree::ArrayValue), pointEmitter ? 0 : treeArray .data (), GL_STATIC_COPY);

	// Update textures

	glBindTexture (GL_TEXTURE_BUFFER, normalMapId);
	glTexBuffer (GL_TEXTURE_BUFFER, GL_RGB32F, normalBufferId);

	glBindTexture (GL_TEXTURE_BUFFER, surfaceMapId);
	glTexBuffer (GL_TEXTURE_BUFFER, GL_RGB32F, surfaceBufferId);

	glBindTexture (GL_TEXTURE_BUFFER, surfaceAreaMapId);
	glTexBuffer (GL_TEXTURE_BUFFER, GL_R32F, surfaceAreaBufferId);

	glBindTexture (GL_TEXTURE_BUFFER, triangleTreeMapId);
	glTexBuffer (GL_TEXTURE_BUFFER, GL_R32F, triangleTreeBufferId);

	glBindTexture (GL_TEXTURE_BUFFER, 0);
	glBindBuffer (GL_TEXTURE_BUFFER, 0);
}

MFString
VolumeEmitter::getShaderUrl () const
{
	return { get_shader ("ParticleSystems/VolumeEmitter.vs") .str () };
}

void
VolumeEmitter::addShaderFields (const X3DSFNode <ComposedShader> & shader) const
{
	X3DParticleEmitterNode::addShaderFields (shader);

	shader -> addUserDefinedField (inputOutput, "pointEmitter", new SFBool (pointEmitter));
	shader -> addUserDefinedField (inputOutput, "direction",    new SFVec3f (normalize (direction () .getValue ())));

	shader -> addUserDefinedField (inputOutput, "ttStride",      new SFInt32 (sizeof (TriangleTree::ArrayValue) / sizeof (float)));
	shader -> addUserDefinedField (inputOutput, "ttTypeOffset",  new SFInt32 (offsetof (TriangleTree::ArrayValue, type) / sizeof (float)));
	shader -> addUserDefinedField (inputOutput, "ttMinOffset",   new SFInt32 (offsetof (TriangleTree::ArrayValue, min) / sizeof (float)));
	shader -> addUserDefinedField (inputOutput, "ttMaxOffset",   new SFInt32 (offsetof (TriangleTree::ArrayValue, max) / sizeof (float)));
	shader -> addUserDefinedField (inputOutput, "ttLeftOffset",  new SFInt32 (offsetof (TriangleTree::ArrayValue, left) / sizeof (float)));
	shader -> addUserDefinedField (inputOutput, "ttRightOffset", new SFInt32 (offsetof (TriangleTree::ArrayValue, right) / sizeof (float)));
}

void
VolumeEmitter::setTextureBuffer (const X3DSFNode <ComposedShader> & shader) const
{
	shader -> setTextureBuffer ("normalMap",       normalMapId);
	shader -> setTextureBuffer ("surfaceMap",      surfaceMapId);
	shader -> setTextureBuffer ("surfaceAreaMap",  surfaceAreaMapId);
	shader -> setTextureBuffer ("triangleTreeMap", triangleTreeMapId);
}

void
VolumeEmitter::setShaderFields (const X3DSFNode <ComposedShader> & shader) const
{
	X3DParticleEmitterNode::setShaderFields (shader);

	shader -> setField <SFBool>  ("pointEmitter", pointEmitter, true);
	shader -> setField <SFVec3f> ("direction",    normalize (direction () .getValue ()), true);
}

void
VolumeEmitter::dispose ()
{
	surfaceNode .dispose ();

	// Surface map

	if (normalMapId)
		glDeleteTextures (1, &normalMapId);

	if (normalBufferId)
		glDeleteBuffers (1, &normalBufferId);

	if (surfaceMapId)
		glDeleteTextures (1, &surfaceMapId);

	if (surfaceBufferId)
		glDeleteBuffers (1, &surfaceBufferId);

	if (surfaceAreaMapId)
		glDeleteTextures (1, &surfaceAreaMapId);

	if (surfaceAreaBufferId)
		glDeleteBuffers (1, &surfaceAreaBufferId);

	if (triangleTreeMapId)
		glDeleteTextures (1, &triangleTreeMapId);

	if (triangleTreeBufferId)
		glDeleteBuffers (1, &triangleTreeBufferId);

	// Dispose base

	X3DParticleEmitterNode::dispose ();
}

} // X3D
} // titania
