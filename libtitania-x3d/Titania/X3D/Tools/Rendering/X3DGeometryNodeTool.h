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
#include "../../Components/Core/X3DPrototypeInstance.h"
#include "../../Components/NURBS/CoordinateDouble.h"
#include "../../Components/Rendering/LineSet.h"
#include "../../Components/Rendering/X3DGeometryNode.h"
#include "../../Rendering/FrameBuffer.h"
#include "../../Rendering/ShapeContainer.h"

namespace titania {
namespace X3D {

class X3DGeometryNodeTool :
	virtual public X3DGeometryNode,
	virtual public X3DNodeTool
{
public:

	///  @name Private fields

	MFVec3d &
	set_selection ()
	{ return *fields .set_selection; }

	const MFVec3d &
	set_selection () const
	{ return *fields .set_selection; }

	SFBool &
	pickable ()
	{ return *fields .pickable; }

	const SFBool &
	pickable () const
	{ return *fields .pickable; }

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
	isTransparent () const override
	{ return getNode <X3DGeometryNode> () -> isTransparent (); }

	virtual
	bool
	isLineGeometry () const override
	{ return getNode <X3DGeometryNode> () -> isLineGeometry (); }

	///  @name Member access

	virtual
	const Box3d &
	getBBox () const final override
	{ return getNode <X3DGeometryNode> () -> getBBox (); }

	virtual
	bool
	getSolid () const final override
	{ return getNode <X3DGeometryNode> () -> getSolid (); }

	virtual
	bool
	getCCW () const final override
	{ return getNode <X3DGeometryNode> () -> getCCW (); }

	virtual
	const std::vector <Color4f> &
	getPolygonColors () const final override
	{ return getNode <X3DGeometryNode> () -> getPolygonColors (); }

	virtual
	const TexCoordArray &
	getPolygonTexCoords () const final override
	{ return getNode <X3DGeometryNode> () -> getPolygonTexCoords (); }

	virtual
	const std::vector <Vector3f> &
	getPolygonNormals () const final override
	{ return getNode <X3DGeometryNode> () -> getPolygonNormals (); }

	virtual
	const std::vector <Vector3d> &
	getPolygonVertices () const final override
	{ return getNode <X3DGeometryNode> () -> getPolygonVertices (); }

	virtual
	const std::vector <X3DGeometryNode::Element> &
	getElements () const final override
	{ return getNode <X3DGeometryNode> () -> getElements (); }

	const X3DPtr <NormalTool> &
	getNormalTool () const
	{ return normalToolNode; }

	const X3DPtr <CoordinateTool> &
	getCoordinateTool () const
	{ return coordToolNode; }
	
	///  @name Operations

	virtual
	bool
	intersects (Line3d, std::vector <IntersectionPtr> &) const final override
	{ return false; }

	virtual
	void
	intersects (const std::shared_ptr <FrameBuffer> &, const std::shared_ptr <FrameBuffer> &) const final override;

	virtual
	bool
	intersects (CollisionSphere3d sphere, const CollectableObjectArray & collectables) const final override
	{ return getNode <X3DGeometryNode> () -> intersects (sphere, collectables); }

	virtual
	void
	triangulate (std::vector <Color4f> & colors, TexCoordArray & texCoords, std::vector <Vector3f> & normals, std::vector <Vector3d> & vertices) const final override
	{ getNode <X3DGeometryNode> () -> triangulate (colors, texCoords, normals, vertices); }

	virtual
	void
	collision (const CollisionContainer* const container) final override
	{ getNode <X3DGeometryNode> () -> collision (container); }

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
	       Error <DISPOSED>) override
	{ return getNode <X3DGeometryNode> () -> toPrimitive (); }

	///  @name Destruction

	virtual
	void
	dispose () override;


protected:

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

		MFVec3d* const set_selection;
		SFBool* const pickable;
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

inline
X3DGeometryNodeTool::Fields::Fields () :
	    set_selection (new MFVec3d ()),
	         pickable (new SFBool (true)),
	       normalTool (new SFNode ()),
	        coordTool (new SFNode ())
{ }

inline
X3DGeometryNodeTool::X3DGeometryNodeTool () :
	 X3DGeometryNode (),
	     X3DNodeTool (),
	          fields (),
	  normalToolNode (new NormalTool (getExecutionContext ())),
	   coordToolNode (new CoordinateTool (getExecutionContext ())),
	        viewport (),
	projectionMatrix (),
	 modelViewMatrix ()
{
	normalTool () = normalToolNode;
	coordTool  () = coordToolNode;

	#ifndef DEBUG
	set_selection () .isHidden (true);
	pickable ()      .isHidden (true);
	normalTool ()    .isHidden (true);
	coordTool  ()    .isHidden (true);
	#endif

	addType (X3DConstants::X3DGeometryNodeTool);

	addChildren (normalToolNode,
                        coordToolNode);
}

inline
void
X3DGeometryNodeTool::setExecutionContext (X3DExecutionContext* const executionContext)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	normalToolNode -> setExecutionContext (executionContext);

	X3DNodeTool::setExecutionContext (executionContext);
}

inline
void
X3DGeometryNodeTool::initialize ()
{
	X3DNodeTool::initialize ();

	pickable () .addInterest (this, &X3DGeometryNodeTool::set_pickable);
	getBrowser () -> getSelection () -> isEnabled () .addInterest (this, &X3DGeometryNodeTool::set_pickable);

	normalToolNode -> getInlineNode () -> checkLoadState () .addInterest (this, &X3DGeometryNodeTool::set_loadState);
	normalToolNode -> length () .addInterest (this, &X3DGeometryNodeTool::eventProcessed);
	getNode <X3DGeometryNode> () -> addInterest (this, &X3DGeometryNodeTool::eventProcessed);

	coordToolNode -> getInlineNode () -> checkLoadState () .addInterest (this, &X3DGeometryNodeTool::set_loadState);

	normalToolNode -> setup ();
	coordToolNode  -> setup ();
}

inline
void
X3DGeometryNodeTool::intersects (const std::shared_ptr <FrameBuffer> & frameBuffer, const std::shared_ptr <FrameBuffer> & depthBuffer) const
{
	try
	{
		if (getCurrentLayer () not_eq coordToolNode -> getActiveLayer ())
			return;

		std::vector <Vector3d> selection;
	
		const auto & depth               = depthBuffer -> getDepth ();
		const auto   modelViewProjection = getModelViewMatrix () * getProjectionMatrix ();
		const auto   invProjection       = inverse (getProjectionMatrix ());

		for (const Vector3d & vertex : getVertices ())
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

inline
void
X3DGeometryNodeTool::set_loadState ()
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

inline
void
X3DGeometryNodeTool::set_pickable ()
{
	try
	{
		const bool enabled = pickable () and
		                     getBrowser () -> getSelection () -> isEnabled () and
		                     not dynamic_cast <X3DPrototypeInstance*> (getExecutionContext ());

		coordToolNode  -> getInlineNode () -> getExportedNode ("SelectionShape") -> setField <SFNode> ("geometry", enabled ? getNode <X3DGeometryNode> () : nullptr, true);
	}
	catch (const X3DError & error)
	{
		//__LOG__ << error .what () << std::endl;
	}
}

inline
void
X3DGeometryNodeTool::eventProcessed ()
{
	const auto & normals  = getNode <X3DGeometryNode> () -> getPolygonNormals ();
	const auto & vertices = getNode <X3DGeometryNode> () -> getPolygonVertices ();
	const auto & elements = getNode <X3DGeometryNode> () -> getElements ();
	const auto   size     = vertices .size ();

	try
	{
		// Normals
	
		const auto & inlineNode = normalToolNode -> getInlineNode ();
		auto &       normalVertexCount = inlineNode -> getExportedNode <LineSet> ("NormalsLineSet") -> vertexCount ();
		auto &       normalPoint       = inlineNode -> getExportedNode <CoordinateDouble> ("NormalsCoord") -> point ();
	
		normalVertexCount .resize (size, SFInt32 (2));
		normalPoint       .resize (2 * size);
	
		for (size_t i = 0; i < size; ++ i)
		{
			normalPoint [2 * i + 0] = vertices [i];
			normalPoint [2 * i + 1] = vertices [i] + Vector3d (normals [i] * normalToolNode -> length () .getValue ());
		}
	}
	catch (const X3DError & error)
	{
		//__LOG__ << error .what () << std::endl;
	}

	try
	{
		// Points

		const auto & inlineNode       = coordToolNode -> getInlineNode ();
		auto &       edgesVertexCount = inlineNode -> getExportedNode <LineSet> ("EdgesLineSet") -> vertexCount ();
		auto &       edgesPoint       = inlineNode -> getExportedNode <CoordinateDouble> ("EdgesCoord") -> point ();

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

inline
void
X3DGeometryNodeTool::traverse (const TraverseType type)
{
	getNode <X3DGeometryNode> () -> traverse (type);

	normalToolNode -> traverse (type);
	coordToolNode  -> traverse (type);
}

inline
void
X3DGeometryNodeTool::draw (const ShapeContainer* const container)
{
	if (PolygonMode (GL_FILL) .front () == GL_FILL)
		getNode <X3DGeometryNode> () -> draw (container);

	if (getCurrentLayer () not_eq coordToolNode -> getActiveLayer ())
		return;

	viewport         = Viewport4i ();
	projectionMatrix = ProjectionMatrix4d ();
	modelViewMatrix  = container -> getModelViewMatrix ();
}

inline
void
X3DGeometryNodeTool::dispose ()
{
	X3DNodeTool::dispose ();
}

} // X3D
} // titania

#endif
