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

#ifndef __TITANIA_X3D_TOOLS_RENDERING_X3DGEOMETRY_NODE_TOOL_H__
#define __TITANIA_X3D_TOOLS_RENDERING_X3DGEOMETRY_NODE_TOOL_H__

#include "../Core/X3DNodeTool.h"
#include "../Rendering/NormalTool.h"

#include "../../Components/Rendering/X3DGeometryNode.h"
#include "../../Rendering/ShapeContainer.h"

namespace titania {
namespace X3D {

template <class Type>
class X3DGeometryNodeTool :
	virtual public X3DNodeTool <Type>
{
public:

	///  @name Private fields

	SFNode &
	normalTool ()
	{ return *fields .normalTool; }

	const SFNode &
	normalTool () const
	{ return *fields .normalTool; }

	SFBool &
	showNormals ()
	{ return *fields .showNormals; }

	const SFBool &
	showNormals () const
	{ return *fields .showNormals; }

	///  @name Member access

	void
	setExecutionContext (X3DExecutionContext* const)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) override;

	virtual
	bool
	isTransparent () const final override
	{ return getNode () -> isTransparent (); }

	virtual
	bool
	isLineGeometry () const final override
	{ return getNode () -> isLineGeometry (); }

	///  @name Member access

	virtual
	const Box3f &
	getBBox () const final override
	{ return getNode () -> getBBox (); }

	virtual
	bool
	getSolid () const final override
	{ return getNode () -> getSolid (); }

	virtual
	bool
	getCCW () const final override
	{ return getNode () -> getCCW (); }

	virtual
	const std::vector <Color4f> &
	getPolygonColors () const final override
	{ return getNode () -> getPolygonColors (); }

	virtual
	const TexCoordArray &
	getPolygonTexCoords () const final override
	{ return getNode () -> getPolygonTexCoords (); }

	virtual
	const std::vector <Vector3f> &
	getPolygonNormals () const final override
	{ return getNode () -> getPolygonNormals (); }

	virtual
	const std::vector <Vector3f> &
	getPolygonVertices () const final override
	{ return getNode () -> getPolygonVertices (); }

	virtual
	const std::vector <X3DGeometryNode::Element> &
	getElements () const final override
	{ return getNode () -> getElements (); }

	///  @name Operations

	virtual
	bool
	intersects (Line3f line, std::vector <IntersectionPtr> & intersections) const final override
	{ return getNode () -> intersects (line, intersections); }

	virtual
	bool
	intersects (CollisionSphere3f sphere, const CollectableObjectArray & collectables) const final override
	{ return getNode () -> intersects (sphere, collectables); }

	virtual
	void
	triangulate (std::vector <Color4f> & colors, TexCoordArray & texCoords, std::vector <Vector3f> & normals, std::vector <Vector3f> & vertices) const final override
	{ getNode () -> triangulate (colors, texCoords, normals, vertices); }

	virtual
	void
	collision (const CollisionContainer* const container) final override
	{ getNode () -> collision (container); }

	virtual
	void
	draw (const ShapeContainer* const container) final override;

	virtual
	SFNode
	toPrimitive () const
	throw (Error <NOT_SUPPORTED>,
	       Error <DISPOSED>) final override
	{ return getNode () -> toPrimitive (); }


	///  @name Destruction

	virtual
	void
	dispose () override;


protected:

	using X3DNodeTool <Type>::getNode;

	///  @name Construction

	X3DGeometryNodeTool ();

	virtual
	void
	initialize () override;


private:

	///  @name Operations

	void
	set_showNormals ();

	void
	eventProcessed ();

	///  @name Members

	struct Fields
	{
		Fields ();

		SFNode* const normalTool;
		SFBool* const showNormals;
	};

	Fields fields;

	X3DPtr <NormalTool> normalToolNode;

};

template <class Type>
X3DGeometryNodeTool <Type>::Fields::Fields () :
	 normalTool (new SFNode ()),
	showNormals (new SFBool ())
{ }

template <class Type>
X3DGeometryNodeTool <Type>::X3DGeometryNodeTool () :
	X3DNodeTool <Type> (),
	            fields (),
	    normalToolNode (new NormalTool (this -> getExecutionContext ()))
{
	normalTool () = normalToolNode;

	normalTool  () .isHidden (true);
	showNormals () .isHidden (true);

	this -> addType (X3DConstants::X3DGeometryNodeTool);

	this -> addField (inputOutput, "normalTool",  normalTool ());
	this -> addField (inputOutput, "showNormals", showNormals ());

	this -> addChildren (normalToolNode);
}

template <class Type>
void
X3DGeometryNodeTool <Type>::setExecutionContext (X3DExecutionContext* const executionContext)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	normalToolNode -> setExecutionContext (executionContext);

	X3DNodeTool <Type>::setExecutionContext (executionContext);
}

template <class Type>
void
X3DGeometryNodeTool <Type>::initialize ()
{
	X3DNodeTool <Type>::initialize ();

	getNode () -> addInterest (this, &X3DGeometryNodeTool::eventProcessed);
	showNormals () .addInterest (normalToolNode -> enabled ());

	normalToolNode -> enabled () = showNormals ();
	normalToolNode -> setup ();

	eventProcessed ();
}

template <class Type>
void
X3DGeometryNodeTool <Type>::eventProcessed ()
{
	const auto & normals  = this -> getNode () -> getPolygonNormals ();
	const auto & vertices = this -> getNode () -> getPolygonVertices ();
	const auto   size     = vertices .size ();

	normalToolNode -> point () .resize (2 * size);

	for (size_t i = 0; i < size; ++ i)
	{
		normalToolNode -> point () [2 * i + 0] = vertices [i];
		normalToolNode -> point () [2 * i + 1] = vertices [i] + normals [i];
	}

	normalToolNode -> vertexCount () .resize (size, SFInt32 (2));
}

template <class Type>
void
X3DGeometryNodeTool <Type>::draw (const ShapeContainer* const container)
{
	normalToolNode -> draw (container);

	this -> getNode () -> draw (container);
}

template <class Type>
void
X3DGeometryNodeTool <Type>::dispose ()
{
	this -> getNode () -> removeUserDefinedField ("showNormals");

	X3DNodeTool <Type>::dispose ();
}

} // X3D
} // titania

#endif
