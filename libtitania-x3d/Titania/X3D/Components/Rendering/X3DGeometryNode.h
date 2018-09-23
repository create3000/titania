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

#ifndef __TITANIA_X3D_COMPONENTS_RENDERING_X3DGEOMETRY_NODE_H__
#define __TITANIA_X3D_COMPONENTS_RENDERING_X3DGEOMETRY_NODE_H__

#include "../../Browser/ParticleSystems/SoftParticle.h"
#include "../../Browser/PointingDeviceSensor/Intersection.h"
#include "../../Browser/Rendering/GeometryType.h"
#include "../../Browser/Shaders/ShadingType.h"
#include "../../Rendering/TexCoordArray.h"
#include "../../Rendering/ClipPlaneContainer.h"
#include "../../Types/Geometry.h"
#include "../Core/X3DNode.h"

#include <map>

namespace titania {
namespace X3D {

class ShapeContainer;
class X3DShapeContainer;
class FrameBuffer;

class X3DGeometryNode :
	virtual public X3DNode
{
public:

	///  @name Member types

	class Element
	{
	public:

		Element (const GLenum vertexMode, const size_t first, const size_t last) :
			m_vertexMode (vertexMode),
			     m_first (first),
			      m_last (last)
		{ }

		GLenum
		vertexMode () const
		{ return m_vertexMode; }

		size_t
		first () const
		{ return m_first; }

		size_t
		last () const
		{ return m_last; }

		size_t
		count () const
		{ return m_last - m_first; }


	private:

		const GLenum m_vertexMode;
		const size_t m_first;
		const size_t m_last;

	};

	///  @name Construction

	virtual
	void
	setup () override;

	virtual
	void
	setExecutionContext (X3DExecutionContext* const executionContext)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) override;

	///  @name Tests

	virtual
	bool
	isTransparent () const
	{ return false; }

	virtual
	GeometryType
	getGeometryType () const
	{ return geometryType; }

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
	intersects (Line3d line,
	            const ClipPlaneContainerArray & clipPlanes,
	            Matrix4d modelViewMatrix,
	            std::vector <IntersectionPtr> & intersections) const;

	virtual
	bool
	intersects (Box3d,
	            const ClipPlaneContainerArray & clipPlanes,
	            Matrix4d modelViewMatrix) const;

	virtual
	std::vector <Vector3d>
	intersects (X3DRenderObject* const renderObject,
	            const std::shared_ptr <FrameBuffer> & frameBuffer,
	            const std::shared_ptr <FrameBuffer> & depthBuffer,
	            std::vector <IntersectionPtr> & intersections);

	virtual
	bool
	cut (X3DRenderObject* const renderObject, const Line2d & cutLine);

	virtual
	void
	triangulate (std::vector <Color4f> &, TexCoordArray &, std::vector <Vector3f> &, std::vector <Vector3d> &) const;

	virtual
	void
	depth (const X3DShapeContainer* const context);

	virtual
	void
	draw (ShapeContainer* const context);

	virtual
	void
	drawParticles (ShapeContainer* const context, const std::vector <SoftParticle> & particles, const size_t numParticles);

	virtual
	NodeType
	getPrimitiveType () const
	throw (Error <NOT_SUPPORTED>,
	       Error <DISPOSED>) = 0;

	virtual
	SFNode
	toPrimitive () const
	throw (Error <NOT_SUPPORTED>,
	       Error <DISPOSED>) = 0;

	///  @name Destruction

	virtual
	void
	dispose () override;

	virtual
	~X3DGeometryNode () override;


protected:

	///  @name Member types

	using NormalIndex = std::map <size_t, std::vector <size_t>>;

	///  @name Construction

	X3DGeometryNode ();

	///  @name Member access

	void
	setAttribs (const X3DPtrArray <X3DVertexAttributeNode> &, const std::vector <std::vector <float>> &);

	const X3DPtrArray <X3DVertexAttributeNode> &
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

	void
	setTextureCoordinate (X3DTextureCoordinateNode* const);

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
	setGeometryType (const GeometryType value)
	{ geometryType = value; }

	void
	setSolid (const bool value)
	{ solid = value; }

	void
	setCCW (const bool value)
	{ frontFace = value ? GL_CCW : GL_CW; }

	static
	GLenum
	getVertexMode (const size_t n);

	void
	addElements (const GLenum vertexMode, const size_t vertexCount);

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
	createBBox () const;

	virtual
	const Matrix4d &
	getMatrix () const
	{ return matrix; }

	void
	getTexCoordParams (Vector3d &, double &, int32_t &, int32_t &);

	void
	refineNormals (const NormalIndex &, std::vector <Vector3f> &, const float, const bool) const;

	void
	addMirrorVertices (const GLenum, const bool);

	void
	set_live ();

	void
	set_shading (const ShadingType & value);

	void
	set_fixedPipeline ();

	void
	update ();

	virtual
	void
	build () = 0;


private:

	///  @name Operations

	bool
	intersects (const Line3d & line,
	            const size_t i0,
	            const size_t i1,
	            const size_t i2,
	            const ClipPlaneContainerArray & clipPlanes,
	            const Matrix4d & modelViewMatrix,
	            std::vector <IntersectionPtr> & intersections) const;

	bool
	isClipped (const Vector3d &, const ClipPlaneContainerArray &) const;

	void
	triangulate (const size_t, const size_t, const size_t, std::vector <Color4f>&, TexCoordArray &, std::vector <Vector3f>&, std::vector <Vector3d>&) const;

	void
	clear ();

	void
	transfer ();

	virtual
	void
	buildTexCoords ();

	///  @name Static members

	static const Matrix4d matrix;

	///  @name Members

	SFBool                               cameraObject;
	Box3d                                bbox;
	X3DPtrArray <X3DVertexAttributeNode> attribNodes;
	std::vector <Color4f>                colors;
	X3DPtr <X3DTextureCoordinateNode>    texCoordNode;
	TexCoordArray                        texCoords;
	std::vector <Vector3f>               normals;
	std::vector <Vector3f>               faceNormals;
	std::vector <Vector3d>               vertices;
	GeometryType                         geometryType;
	bool                                 solid;
	GLenum                               frontFace;
	bool                                 flatShading;
	std::vector <Element>                elements;

	std::vector <GLuint> attribBufferIds;
	GLuint               colorBufferId;
	std::vector <GLuint> texCoordBufferIds;
	GLuint               normalBufferId;
	GLuint               vertexBufferId;

};

} // X3D
} // titania

#endif
