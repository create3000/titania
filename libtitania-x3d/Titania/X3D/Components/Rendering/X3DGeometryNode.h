/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#ifndef __TITANIA_X3D_COMPONENTS_RENDERING_X3DGEOMETRY_NODE_H__
#define __TITANIA_X3D_COMPONENTS_RENDERING_X3DGEOMETRY_NODE_H__

#include "../../Browser/PointingDeviceSensor/Intersection.h"
#include "../../Rendering/TexCoordArray.h"
#include "../../Rendering/X3DCollectableObject.h"
#include "../../Types/Geometry.h"
#include "../Core/X3DNode.h"
#include "../Shaders/X3DVertexAttributeNode.h"
#include "../Texturing/TextureCoordinateGenerator.h"

#include <map>

namespace titania {
namespace X3D {

class ShapeContainer;
class CollisionContainer;
class FrameBuffer;

class X3DGeometryNode :
	virtual public X3DNode
{
public:

	///  @name Member types

	struct Element
	{
		Element (GLenum vertexMode, size_t count) :
			vertexMode (vertexMode),
			     count (count)
	          	{ }

		const GLenum vertexMode;
		const size_t count;

	};

	///  @name Construction

	virtual
	void
	setup () final override;

	virtual
	void
	setExecutionContext (X3DExecutionContext* const)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) override;

	///  @name Tests

	virtual
	bool
	isTransparent () const
	{ return false; }

	virtual
	bool
	isLineGeometry () const = 0;

	///  @name Member access

	virtual
	const SFBool &
	isCameraObject () const
	{ return cameraObject; }

	void
	setCameraObject (const bool);

	virtual
	const Box3d &
	getBBox () const
	{ return bbox; }

	virtual
	bool
	getSolid () const
	{ return solid; }

	virtual
	bool
	getCCW () const
	{ return frontFace == GL_CCW; }
	
	virtual
	const std::vector <Color4f> &
	getPolygonColors () const
	{ return colors; }

	virtual
	const TexCoordArray &
	getPolygonTexCoords () const
	{ return texCoords; }

	virtual
	const std::vector <Vector3f> &
	getPolygonNormals () const
	{ return normals; }

	virtual
	const std::vector <Vector3d> &
	getPolygonVertices () const
	{ return vertices; }
	
	virtual
	const std::vector <Element> &
	getElements () const
	{ return elements; }

	///  @name Operations

	virtual
	bool
	intersects (Line3d, std::vector <IntersectionPtr> &) const;

	virtual
	std::vector <Vector3d>
	intersects (const std::shared_ptr <FrameBuffer> &, const std::shared_ptr <FrameBuffer> &, std::vector <IntersectionPtr> &);

	virtual
	bool
	intersects (CollisionSphere3d, const CollectableObjectArray &) const;

	virtual
	bool
	cut (const Line2d &);

	virtual
	void
	triangulate (std::vector <Color4f> &, TexCoordArray &, std::vector <Vector3f> &, std::vector <Vector3d> &) const;

	virtual
	void
	collision (const CollisionContainer* const);

	virtual
	void
	draw (ShapeContainer* const);

	virtual
	SFNode
	toPrimitive () const
	throw (Error <NOT_SUPPORTED>,
	       Error <DISPOSED>) = 0;

	///  @name Destruction

	virtual
	void
	dispose () override;


protected:

	///  @name Member types

	using NormalIndex = std::map <size_t, std::vector <size_t>>;

	///  @name Construction

	X3DGeometryNode ();

	///  @name Member access

	void
	setAttribs (const X3DPtrArray <X3DVertexAttributeNode> &, const std::vector <std::vector <float>> &);

	const std::vector <X3DVertexAttributeNode*> &
	getAttribs () const
	{ return attribNodes; }

	std::vector <Color4f> &
	getColors ()
	{ return colors; }
	
	const std::vector <Color4f> &
	getColors () const
	{ return colors; }

	TexCoordArray &
	getTexCoords ()
	{ return texCoords; }

	const TexCoordArray &
	getTexCoords () const
	{ return texCoords; }

	#ifdef FIXED_PIPELINE
	void
	setTextureCoordinate (X3DTextureCoordinateNode* const);
	#endif

	std::vector <Vector3f> &
	getNormals ()
	{ return normals; }

	const std::vector <Vector3f> &
	getNormals () const
	{ return normals; }

	std::vector <Vector3d> &
	getVertices ()
	{ return vertices; }

	const std::vector <Vector3d> &
	getVertices () const
	{ return vertices; }

	void
	setSolid (const bool & value)
	{ solid = value; }

	void
	setCCW (const bool & value)
	{ frontFace = value ? GL_CCW : GL_CW; }

	static
	GLenum
	getVertexMode (size_t n)
	{
		switch (n)
		{
			case 3:
				return GL_TRIANGLES;
			case 4:
				return GL_QUADS;
			default:
				return GL_POLYGON;
		}
	}

	void
	addElements (const GLenum count, const size_t vertexCount)
	{ elements .emplace_back (count, vertexCount); }

	const std::vector <GLuint> &
	getAttribBufferIds () const
	{ return attribBufferIds; }

	GLuint
	getColorBufferId () const
	{ return colorBufferId; }

	const std::vector <GLuint> &
	getTexCoordBufferIds () const
	{ return texCoordBufferIds; }

	GLuint
	getNormalBufferId () const
	{ return normalBufferId; }

	GLuint
	getVertexBufferId () const
	{ return vertexBufferId; }

	///  @name Operations

	virtual
	Box3d
	createBBox ();

	virtual
	Matrix4d
	getMatrix () const
	{ return Matrix4d (); }

	void
	getTexCoordParams (Vector3d &, double &, int &, int &);

	void
	refineNormals (const NormalIndex &, std::vector <Vector3f> &, const float, const bool) const;

	void
	addMirrorVertices (const GLenum, const bool);

	void
	update ();

	virtual
	void
	build () = 0;


private:

	///  @name Operations

	bool
	intersects (const Line3d &, const size_t, const size_t, const size_t, const Matrix4d &, std::vector <IntersectionPtr>&) const;

	bool
	isClipped (const Vector3d &, const Matrix4d &) const;

	bool
	isClipped (const Vector3d &, const Matrix4d &, const CollectableObjectArray &) const;

	void
	triangulate (const size_t, const size_t, const size_t, std::vector <Color4f>&, TexCoordArray &, std::vector <Vector3f>&, std::vector <Vector3d>&) const;

	void
	clear ();

	void
	transfer ();

	virtual
	void
	buildTexCoords ();

	///  @name Members

	SFBool                                cameraObject;
	Box3d                                 bbox;
	std::vector <X3DVertexAttributeNode*> attribNodes;
	std::vector <Color4f>                 colors;
	#ifdef FIXED_PIPELINE
	X3DPtr <X3DTextureCoordinateNode>     texCoordNode;
	#endif
	TexCoordArray                         texCoords;
	std::vector <Vector3f>                normals;
	std::vector <Vector3d>                vertices;
	bool                                  solid;
	GLenum                                frontFace;
	std::vector <Element>                 elements;

	std::vector <GLuint> attribBufferIds;
	GLuint               colorBufferId;
	std::vector <GLuint> texCoordBufferIds;
	GLuint               normalBufferId;
	GLuint               vertexBufferId;

};

} // X3D
} // titania

#endif
