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
#include "../Rendering/CoordinateTool.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Browser/Selection.h"
#include "../../Components/Rendering/X3DGeometryNode.h"
#include "../../Rendering/FrameBuffer.h"
#include "../../Rendering/ShapeContainer.h"

namespace titania {
namespace X3D {

template <class Type>
class X3DGeometryNodeTool :
	virtual public X3DNodeTool <Type>
{
public:

	///  @name Private fields

	SFBool &
	pickable ()
	{ return *fields .pickable; }

	const SFBool &
	pickable () const
	{ return *fields .pickable; }

	MFVec3d &
	set_selection ()
	{ return *fields .set_selection; }

	const MFVec3d &
	set_selection () const
	{ return *fields .set_selection; }

	SFNode &
	normalTool ()
	{ return *fields .normalTool; }

	const SFNode &
	normalTool () const
	{ return *fields .normalTool; }

	SFNode &
	coordTool ()
	{ return *fields .coordTool; }

	const SFNode &
	coordTool () const
	{ return *fields .coordTool; }

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

	const X3DPtr <NormalTool> &
	getNormalTool () const
	{ return normalToolNode; }

	const X3DPtr <CoordinateTool> &
	getCoordinateTool () const
	{ return coordToolNode; }
	
	///  @name Operations

	virtual
	bool
	intersects (Line3f, std::vector <IntersectionPtr> &) const final override
	{ return false; }

	virtual
	void
	intersects (const std::shared_ptr <FrameBuffer> &, const std::shared_ptr <FrameBuffer> &) const final override;

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
	traverse (const TraverseType) final override;

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

	const Vector4i &
	getViewport () const
	{ return viewport; }

	const Matrix4d &
	getProjectionMatrix () const
	{ return projectionMatrix; }

	const Matrix4d &
	getModelViewMatrix () const
	{ return modelViewMatrix; }


private:

	///  @name Operations

	void
	set_loadState ();

	void
	set_pickable ();

	void
	eventProcessed ();

	///  @name Members

	struct Fields
	{
		Fields ();

		SFBool* const pickable;
		MFVec3d* const set_selection;
		SFNode* const normalTool;
		SFNode* const coordTool;
	};

	Fields fields;

	X3DPtr <NormalTool>     normalToolNode;
	X3DPtr <CoordinateTool> coordToolNode;

	Vector4i viewport;
	Matrix4d projectionMatrix;
	Matrix4d modelViewMatrix;

};

template <class Type>
X3DGeometryNodeTool <Type>::Fields::Fields () :
	         pickable (new SFBool (true)),
	    set_selection (new MFVec3d ()),
	       normalTool (new SFNode ()),
	        coordTool (new SFNode ())
{ }

template <class Type>
X3DGeometryNodeTool <Type>::X3DGeometryNodeTool () :
	X3DNodeTool <Type> (),
	            fields (),
	    normalToolNode (new NormalTool (this -> getExecutionContext ())),
	     coordToolNode (new CoordinateTool (this -> getExecutionContext ())),
             viewport (),
     projectionMatrix (),
      modelViewMatrix ()
{
	normalTool () = normalToolNode;
	coordTool  () = coordToolNode;

//	pickable ()      .isHidden (true);
//	set_selection () .isHidden (true);
//	normalTool ()    .isHidden (true);
//	coordTool  ()    .isHidden (true);

	this -> addType (X3DConstants::X3DGeometryNodeTool);

	this -> addChildren (normalToolNode,
                        coordToolNode);
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

	pickable () .addInterest (this, &X3DGeometryNodeTool::set_pickable);
	this -> getBrowser () -> getSelection () -> isEnabled () .addInterest (this, &X3DGeometryNodeTool::set_pickable);

	normalToolNode -> getInlineNode () -> checkLoadState () .addInterest (this, &X3DGeometryNodeTool::set_loadState);
	normalToolNode -> length () .addInterest (this, &X3DGeometryNodeTool::eventProcessed);
	getNode () -> addInterest (this, &X3DGeometryNodeTool::eventProcessed);

	normalToolNode -> setup ();
	coordToolNode  -> setup ();
}

template <class Type>
void
X3DGeometryNodeTool <Type>::intersects (const std::shared_ptr <FrameBuffer> & frameBuffer, const std::shared_ptr <FrameBuffer> & depthBuffer) const
{
	try
	{
		if (this -> getCurrentLayer () not_eq coordToolNode -> getActiveLayer ())
			return;

		std::vector <Vector3d> selection;
	
		const auto & depth               = depthBuffer -> getDepth ();
		const auto   modelViewProjection = getModelViewMatrix () * getProjectionMatrix ();
		const auto   invProjection       = inverse (getProjectionMatrix ());

		for (const Vector3d & vertex : this -> getVertices ())
		{
			const auto screen = ViewVolume::projectPoint (vertex, modelViewProjection, getViewport ());
			const auto world  = vertex * getModelViewMatrix ();
			const auto x      = std::floor (screen .x ());
			const auto y      = std::floor (screen .y ());
			const auto index  = x * 4 + y * frameBuffer -> getWidth () * 4;

			if (index < 0 or index >= frameBuffer -> getPixels () .size ())
				continue;

			const auto z      = depth [x + y * depthBuffer -> getWidth ()];
			const auto zWorld = ViewVolume::unProjectPoint (x, y, z, invProjection, getViewport ());
	
			if (world .z () - zWorld .z () < -0.05)
				continue;
	
			if (frameBuffer -> getPixels () [index])
				selection .emplace_back (vertex);
		}
	
		std::sort (selection .begin (), selection .end ());
	
		const auto last = std::unique (selection .begin (), selection .end ());
	
		const_cast <X3DGeometryNodeTool*> (this) -> set_selection () .assign (selection .begin (), last);
	}
	catch (const std::exception & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

template <class Type>
void
X3DGeometryNodeTool <Type>::set_loadState ()
{
	try
	{
		set_pickable ();
		eventProcessed ();
	}
	catch (const X3DError & error)
	{
		//__LOG__ << error .what () << std::endl;
	}
}

template <class Type>
void
X3DGeometryNodeTool <Type>::set_pickable ()
{
	try
	{
		const bool enabled = pickable () and this -> getBrowser () -> getSelection () -> isEnabled ();

		coordToolNode  -> getInlineNode () -> getExportedNode ("SelectionShape") -> setField <SFNode> ("geometry", enabled ? getNode () : nullptr, true);
	}
	catch (const X3DError & error)
	{
		//__LOG__ << error .what () << std::endl;
	}
}

template <class Type>
void
X3DGeometryNodeTool <Type>::eventProcessed ()
{
	const auto & normals  = this -> getNode () -> getPolygonNormals ();
	const auto & vertices = this -> getNode () -> getPolygonVertices ();
	const auto & elements = this -> getNode () -> getElements ();
	const auto   size     = vertices .size ();

	try
	{
		// Normals
	
		auto & normalVertexCount = normalToolNode -> getInlineNode () -> getExportedNode ("NormalsLineSet") -> getField <MFInt32> ("vertexCount");
		auto & normalPoint       = normalToolNode -> getInlineNode () -> getExportedNode ("NormalsCoord")   -> getField <MFVec3f> ("point");
	
		normalVertexCount .resize (size, SFInt32 (2));
		normalPoint       .resize (2 * size);
	
		for (size_t i = 0; i < size; ++ i)
		{
			normalPoint [2 * i + 0] = vertices [i];
			normalPoint [2 * i + 1] = vertices [i] + normals [i] * normalToolNode -> length () .getValue ();
		}
	}
	catch (const X3DError & error)
	{
		//__LOG__ << error .what () << std::endl;
	}

	try
	{
		// Points

		auto & edgesVertexCount = coordToolNode -> getInlineNode () -> getExportedNode ("EdgesLineSet") -> getField <MFInt32> ("vertexCount");
		auto & edgesPoint       = coordToolNode -> getInlineNode () -> getExportedNode ("EdgesCoord")   -> getField <MFVec3f> ("point");

		size_t first = 0;
		size_t p     = 0;
		size_t v     = 0;
	
		for (const auto & element : elements)
		{
			switch (element .vertexMode)
			{
				case GL_TRIANGLES :
				{
					for (size_t i = first, size = first + element .count; i < size; i += 3)
					{
						edgesVertexCount .set1Value (v ++, 4);
						edgesPoint       .set1Value (p ++, vertices [i + 0]);
						edgesPoint       .set1Value (p ++, vertices [i + 1]);
						edgesPoint       .set1Value (p ++, vertices [i + 2]);
						edgesPoint       .set1Value (p ++, vertices [i + 0]);
					}
	
					break;
				}
				case GL_QUADS:
				{
					for (size_t i = first, size = first + element .count; i < size; i += 4)
					{
						edgesVertexCount .set1Value (v ++, 5);
						edgesPoint       .set1Value (p ++, vertices [i + 0]);
						edgesPoint       .set1Value (p ++, vertices [i + 1]);
						edgesPoint       .set1Value (p ++, vertices [i + 2]);
						edgesPoint       .set1Value (p ++, vertices [i + 3]);
						edgesPoint       .set1Value (p ++, vertices [i + 0]);
					}
	
					break;
				}
				case GL_QUAD_STRIP:
				{
					for (size_t i = first, size = first + element .count - 2; i < size; i += 2)
					{
						edgesVertexCount .set1Value (v ++, 5);
						edgesPoint       .set1Value (p ++, vertices [i + 0]);
						edgesPoint       .set1Value (p ++, vertices [i + 1]);
						edgesPoint       .set1Value (p ++, vertices [i + 3]);
						edgesPoint       .set1Value (p ++, vertices [i + 2]);
						edgesPoint       .set1Value (p ++, vertices [i + 0]);
					}
	
					break;
				}
				case GL_POLYGON:
				{
					edgesVertexCount .set1Value (v ++, element .count + 1);
	
					for (size_t i = first, size = first + element .count; i < size; ++ i)
						edgesPoint .set1Value (p ++, vertices [i]);
	
					edgesPoint.set1Value (p ++, vertices [first]);
					break;
				}
				default:
					break;
			}
	
			first += element .count;
		}
	
		edgesVertexCount .resize (v);
		edgesPoint       .resize (p);
	}
	catch (const X3DError & error)
	{
		//__LOG__ << error .what () << std::endl;
	}
}

template <class Type>
void
X3DGeometryNodeTool <Type>::traverse (const TraverseType type)
{
	this -> getNode () -> traverse (type);

	normalToolNode -> traverse (type);
	coordToolNode  -> traverse (type);
}

template <class Type>
void
X3DGeometryNodeTool <Type>::draw (const ShapeContainer* const container)
{
	if (PolygonMode (GL_FILL) .front () == GL_FILL)
		this -> getNode () -> draw (container);

	if (this -> getCurrentLayer () not_eq coordToolNode -> getActiveLayer ())
		return;

	viewport         = Viewport4i ();
	projectionMatrix = ProjectionMatrix4d ();
	modelViewMatrix  = container -> getModelViewMatrix ();
}

template <class Type>
void
X3DGeometryNodeTool <Type>::dispose ()
{
	X3DNodeTool <Type>::dispose ();
}

} // X3D
} // titania

#endif
