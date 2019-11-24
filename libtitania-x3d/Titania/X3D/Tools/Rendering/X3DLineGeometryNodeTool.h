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

#ifndef __TITANIA_X3D_TOOLS_RENDERING_X3DLINE_GEOMETRY_NODE_TOOL_H__
#define __TITANIA_X3D_TOOLS_RENDERING_X3DLINE_GEOMETRY_NODE_TOOL_H__

#include "../Rendering/X3DGeometryNodeTool.h"

#include "../../Components/Rendering/X3DLineGeometryNode.h"

namespace titania {
namespace X3D {

class X3DLineGeometryNodeTool :
	virtual public X3DLineGeometryNode,
	virtual public X3DGeometryNodeTool
{
public:

	///  @name Operations

	virtual
	bool
	intersects (Line3d line,
	            const CollectableObjectArray & clipPlanes,
	            Matrix4d modelViewMatrix,
	            std::vector <IntersectionPtr> & intersections) const override
	{ return X3DGeometryNodeTool::intersects (line, clipPlanes, modelViewMatrix, intersections); }

	virtual
	bool
	intersects (Box3d box,
	            const CollectableObjectArray & clipPlanes,
	            Matrix4d modelViewMatrix) const override
	{ return X3DGeometryNodeTool::intersects (box, clipPlanes, modelViewMatrix); }

	virtual
	std::vector <Vector3d>
	intersects (X3DRenderObject* const renderObject,
	            const std::shared_ptr <FrameBuffer> & frameBuffer,
	            const std::shared_ptr <FrameBuffer> & depthBuffer,
	            std::vector <IntersectionPtr> & intersections) override
	{ return X3DGeometryNodeTool::intersects (renderObject, frameBuffer, depthBuffer, intersections); }

	virtual
	void
	depth (const X3DShapeContainer* const context) override
	{ X3DGeometryNodeTool::depth (context); }

	virtual
	void
	draw (ShapeContainer* const context) override
	{ X3DGeometryNodeTool::draw (context); }

	virtual
	void
	drawParticles (ShapeContainer* const context, const std::vector <SoftParticle> & particles, const size_t numParticles) final override
	{ X3DGeometryNodeTool::drawParticles (context, particles, numParticles); }

	///  @name Destruction

	virtual
	void
	dispose () override;

	virtual
	~X3DLineGeometryNodeTool () override;


protected:

	///  @name Construction

	X3DLineGeometryNodeTool ();

	virtual
	void
	initialize () override;


private:

};

} // X3D
} // titania

#endif
