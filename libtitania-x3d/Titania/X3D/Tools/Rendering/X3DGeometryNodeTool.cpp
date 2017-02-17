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

#include "X3DGeometryNodeTool.h"

#include "../Rendering/CoordinateTool.h"
#include "../Rendering/NormalTool.h"

#include "../../Browser/Selection.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Components/Core/X3DPrototypeInstance.h"
#include "../../Components/Grouping/Switch.h"
#include "../../Components/NURBS/CoordinateDouble.h"
#include "../../Components/Rendering/LineSet.h"
#include "../../Rendering/ShapeContainer.h"
#include "../../Rendering/X3DRenderObject.h"

namespace titania {
namespace X3D {

X3DGeometryNodeTool::Fields::Fields () :
	  toolType (new SFString ("NONE")),
	normalTool (new SFNode ()),
	 coordTool (new SFNode ())
{ }

X3DGeometryNodeTool::X3DGeometryNodeTool () :
	 X3DGeometryNode (),
	     X3DNodeTool (),
	          fields (),
	       selection (),
	  normalToolNode (new NormalTool (getExecutionContext ())), 
	   coordToolNode (new CoordinateTool (getExecutionContext ())),
	        viewport (),
	projectionMatrix (),
	 modelViewMatrix ()
{
	addType (X3DConstants::X3DGeometryNodeTool);

	normalTool () .set (normalToolNode);
	coordTool  () .set (coordToolNode);

	addChildObjects (selection,
	                 normalToolNode,
	                 coordToolNode);

	setCameraObject (true);
}

void
X3DGeometryNodeTool::setExecutionContext (X3DExecutionContext* const executionContext)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	normalToolNode -> setExecutionContext (executionContext);
	coordToolNode  -> setExecutionContext (executionContext);

	X3DNodeTool::setExecutionContext (executionContext);
}

void
X3DGeometryNodeTool::initialize ()
{
	X3DNodeTool::initialize ();

	getBrowser () -> getSelection () -> isEnabled () .addInterest (&X3DGeometryNodeTool::set_pickable, this);

	selection .addInterest (&X3DGeometryNodeTool::set_selection, this);

	normalToolNode -> getInlineNode () -> checkLoadState () .addInterest (&X3DGeometryNodeTool::set_loadState, this);
	normalToolNode -> length () .addInterest (&X3DGeometryNodeTool::eventProcessed, this);
	getNode <X3DGeometryNode> () -> addInterest (&X3DGeometryNodeTool::eventProcessed, this);

	coordToolNode -> getInlineNode () -> checkLoadState () .addInterest (&X3DGeometryNodeTool::set_loadState, this);

	normalToolNode -> setup ();
	coordToolNode  -> setup ();

	toolType () .addInterest (&X3DGeometryNodeTool::set_toolType, this);

	set_loadState ();
}

void
X3DGeometryNodeTool::set_toolType ()
{
	try
	{
		if (toolType () == "NONE")
		{
			const auto & inlineNode = coordToolNode -> getInlineNode ();
			const auto   toolSwich  = inlineNode -> getExportedNode <Switch> ("ToolSwitch");

			toolSwich -> whichChoice () = 0;
		}
	}
	catch (const X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
X3DGeometryNodeTool::set_loadState ()
{
	try
	{
		set_toolType ();
		set_pickable ();
		eventProcessed ();
	}
	catch (const X3DError & error)
	{
		//__LOG__ << error .what () << std::endl;
	}
}

void
X3DGeometryNodeTool::set_pickable ()
{
	try
	{
		const bool enabled = getBrowser () -> getSelection () -> isEnabled () and
		                     not dynamic_cast <X3DPrototypeInstance*> (getExecutionContext ());

		coordToolNode  -> getInlineNode () -> getExportedNode ("SelectionShape") -> setField <SFNode> ("geometry", enabled ? getNode <X3DGeometryNode> () : nullptr, true);
	}
	catch (const X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

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

		const auto & inlineNode        = normalToolNode -> getInlineNode ();
		auto &       normalVertexCount = inlineNode -> getExportedNode <LineSet> ("NormalsLineSet") -> vertexCount ();
		auto &       normalPoint       = inlineNode -> getExportedNode <CoordinateDouble> ("NormalsCoord") -> point ();

		if (normals .empty ())
		{
			normalVertexCount .clear ();
			normalPoint       .clear ();
		}
		else
		{
			normalVertexCount .resize (size, SFInt32 (2));
			normalPoint       .resize (2 * size);
	
			for (size_t i = 0; i < size; ++ i)
			{
				normalPoint [2 * i + 0] = vertices [i];
				normalPoint [2 * i + 1] = vertices [i] + Vector3d (normals [i] * normalToolNode -> length () .getValue ());
			}
		}
	}
	catch (const X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}

	try
	{
		// Points

		const auto & inlineNode       = coordToolNode -> getInlineNode ();
		auto &       edgesVertexCount = inlineNode -> getExportedNode <LineSet> ("EdgesLineSet") -> vertexCount ();
		auto &       edgesPoint       = inlineNode -> getExportedNode <CoordinateDouble> ("EdgesCoord") -> point ();

		size_t p = 0;
		size_t v = 0;

		for (const auto & element : elements)
		{
			switch (element .vertexMode ())
			{
				case GL_TRIANGLES :
					{
						for (size_t i = element .first (), size = element .last (); i < size; i += 3)
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
					for (size_t i = element .first (), size = element .last (); i < size; i += 4)
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
				case GL_POLYGON:
				{
					edgesVertexCount .set1Value (v ++, element .count () + 1);

					for (size_t i = element .first (), size = element .last (); i < size; ++ i)
						edgesPoint .set1Value (p ++, vertices [i]);

					edgesPoint .set1Value (p ++, vertices [element .first ()]);
					break;
				}
				default:
					break;
			}
		}

		edgesVertexCount .resize (v);
		edgesPoint       .resize (p);
	}
	catch (const X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

bool
X3DGeometryNodeTool::intersects (Line3d hitRay,
                                 const ClipPlaneContainerArray & clipPlanes,
                                 Matrix4d modelViewMatrix, 
                                 std::vector <IntersectionPtr> & intersections) const
{
	return getNode <X3DGeometryNode> () -> intersects (hitRay, clipPlanes, modelViewMatrix, intersections);
}

bool
X3DGeometryNodeTool::intersects (Box3d box,
                                 const ClipPlaneContainerArray & clipPlanes,
                                 Matrix4d modelViewMatrix) const
{
	return getNode <X3DGeometryNode> () -> intersects (box, clipPlanes, modelViewMatrix);
}

std::vector <Vector3d>
X3DGeometryNodeTool::intersects (X3DRenderObject* const renderObject,
                                 const std::shared_ptr <FrameBuffer> & frameBuffer,
                                 const std::shared_ptr <FrameBuffer> & depthBuffer,
                                 std::vector <IntersectionPtr> & intersections)
{
	const auto hitPoints = getNode <X3DGeometryNode> () -> intersects (renderObject, frameBuffer, depthBuffer, intersections);

	selection .insert (selection .end (), hitPoints .begin (), hitPoints .end ());

	return hitPoints;
}

void
X3DGeometryNodeTool::set_selection (const MFVec3d &)
{
	selection .set ({ });
}

void
X3DGeometryNodeTool::traverse (const TraverseType type, X3DRenderObject* const renderObject)
{
	getNode <X3DGeometryNode> () -> traverse (type, renderObject);

	normalToolNode -> traverse (type, renderObject);
	coordToolNode  -> traverse (type, renderObject);
}

void
X3DGeometryNodeTool::draw (ShapeContainer* const context)
{
	if (PolygonMode (GL_FILL) .front () == GL_FILL)
	{
		PolygonOffset polygonOffset (GL_POLYGON_OFFSET_FILL, 1, 1);

		getNode <X3DGeometryNode> () -> draw (context);
	}

	viewport         = context -> getRenderer () -> getViewVolumes () .back () .getViewport ();
	projectionMatrix = context -> getRenderer () -> getProjectionMatrix () .get ();
	modelViewMatrix  = context -> getModelViewMatrix ();
}

void
X3DGeometryNodeTool::dispose ()
{
	X3DNodeTool::dispose ();
}

X3DGeometryNodeTool::~X3DGeometryNodeTool ()
{ }

} // X3D
} // titania
