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

#ifndef __TITANIA_X3D_TOOLS_GEOMETRY2D_DISK2DTOOL_H__
#define __TITANIA_X3D_TOOLS_GEOMETRY2D_DISK2DTOOL_H__

#include "../../Components/Geometry2D/Disk2D.h"
#include "../Rendering/X3DGeometryNodeTool.h"
#include "../Rendering/X3DLineGeometryNodeTool.h"

namespace titania {
namespace X3D {

class Disk2DTool :
	virtual public Disk2D,
	virtual public X3DGeometryNodeTool,
	public X3DLineGeometryNodeTool
{
public:

	///  @name Construction

	Disk2DTool (X3DBaseNode* const node);

	virtual
	void
	setExecutionContext (X3DExecutionContext* const executionContext)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final override
	{ X3DLineGeometryNodeTool::setExecutionContext (executionContext); }

	///  @name Fields

	virtual
	SFFloat &
	innerRadius () final override
	{ return getNode <Disk2D> () -> innerRadius (); }

	virtual
	const SFFloat &
	innerRadius () const final override
	{ return getNode <Disk2D> () -> innerRadius (); }

	virtual
	SFFloat &
	outerRadius () final override
	{ return getNode <Disk2D> () -> outerRadius (); }

	virtual
	const SFFloat &
	outerRadius () const final override
	{ return getNode <Disk2D> () -> outerRadius (); }

	virtual
	SFBool &
	solid () final override
	{ return getNode <Disk2D> () -> solid (); }

	virtual
	const SFBool &
	solid () const final override
	{ return getNode <Disk2D> () -> solid (); }

	///  @name Hidden fields

	virtual
	SFNode &
	options () final override
	{ return getNode <Disk2D> () -> options (); }

	virtual
	const SFNode &
	options () const final override
	{ return getNode <Disk2D> () -> options (); }

	///  @name Operations

	virtual
	bool
	intersects (Line3d line,
	            const ClipPlaneContainerArray & clipPlanes,
	            Matrix4d modelViewMatrix,
	            std::vector <IntersectionPtr> & intersections) const final override
	{ return X3DGeometryNodeTool::intersects (line, clipPlanes, modelViewMatrix, intersections); }

	virtual
	bool
	intersects (Box3d box,
	            const ClipPlaneContainerArray & clipPlanes,
	            Matrix4d modelViewMatrix) const final override
	{ return X3DGeometryNodeTool::intersects (box, clipPlanes, modelViewMatrix); }

	virtual
	std::vector <Vector3d>
	intersects (X3DRenderObject* const renderObject,
	            const std::shared_ptr <FrameBuffer> & frameBuffer,
	            const std::shared_ptr <FrameBuffer> & depthBuffer,
	            std::vector <IntersectionPtr> & intersections) final override
	{ return X3DGeometryNodeTool::intersects (renderObject, frameBuffer, depthBuffer, intersections); }

	virtual
	void
	depth (const X3DShapeContainer* const context) final override
	{ X3DGeometryNodeTool::depth (context); }

	virtual
	void
	draw (ShapeContainer* const context) final override
	{ X3DGeometryNodeTool::draw (context); }

	virtual
	NodeType
	getPrimitiveType () const
	throw (Error <NOT_SUPPORTED>,
	       Error <DISPOSED>) final override
	{ return getNode <Disk2D> () -> getPrimitiveType (); }

	virtual
	SFNode
	toPrimitive () const
	throw (Error <NOT_SUPPORTED>,
	       Error <DISPOSED>) final override
	{ return getNode <Disk2D> () -> toPrimitive (); }

	virtual
	void
	beginUndo () final override;

	virtual
	void
	endUndo (const UndoStepPtr & undoStep) final override;

	///  @name Destruction

	virtual
	void
	dispose () final override;

	virtual
	~Disk2DTool () final override;


protected:

	///  @name Construction

	virtual
	void
	initialize () final override;


private:

	///  @name Event handlers

	void
	set_loadState ();

	void
	set_transform_tools ();

	void
	set_innerRadius ();

	void
	set_outerRadius ();
	
	void
	set_scale1 ();
	
	void
	set_scale2 ();
	
	void
	connectInnerRadius (const SFFloat & field);
	
	void
	connectOuterRadius (const SFFloat & field);
	
	void
	connectScale1 (const SFVec3f & field);
	
	void
	connectScale2 (const SFVec3f & field);

	///  @name Fields

	struct Fields
	{
		Fields ();

	};

	Fields fields;

	///  @name Members

	float startInnerRadius;
	float startOuterRadius;

};

} // X3D
} // titania

#endif
