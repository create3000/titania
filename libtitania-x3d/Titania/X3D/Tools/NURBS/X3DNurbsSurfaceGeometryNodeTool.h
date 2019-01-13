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

#ifndef __TITANIA_X3D_TOOLS_NURBS_X3DNURBS_SURFACE_GEOMETRY_NODE_TOOL_H__
#define __TITANIA_X3D_TOOLS_NURBS_X3DNURBS_SURFACE_GEOMETRY_NODE_TOOL_H__

#include "../NURBS/X3DParametricGeometryNodeTool.h"

#include "../../Components/NURBS/X3DNurbsSurfaceGeometryNode.h"

namespace titania {
namespace X3D {

class X3DNurbsSurfaceGeometryNodeTool :
	virtual public X3DNurbsSurfaceGeometryNode,
	public X3DParametricGeometryNodeTool
{
public:

	///  @name Fields

	virtual
	SFBool &
	solid () final override
	{ return getNode <X3DNurbsSurfaceGeometryNode> () -> solid (); }

	virtual
	const SFBool &
	solid () const final override
	{ return getNode <X3DNurbsSurfaceGeometryNode> () -> solid (); }

	virtual
	SFInt32 &
	uTessellation () final override
	{ return getNode <X3DNurbsSurfaceGeometryNode> () -> uTessellation (); }

	virtual
	const SFInt32 &
	uTessellation () const final override
	{ return getNode <X3DNurbsSurfaceGeometryNode> () -> uTessellation (); }

	virtual
	SFInt32 &
	vTessellation () final override
	{ return getNode <X3DNurbsSurfaceGeometryNode> () -> vTessellation (); }

	virtual
	const SFInt32 &
	vTessellation () const final override
	{ return getNode <X3DNurbsSurfaceGeometryNode> () -> vTessellation (); }

	virtual
	SFBool &
	uClosed () final override
	{ return getNode <X3DNurbsSurfaceGeometryNode> () -> uClosed (); }

	virtual
	const SFBool &
	uClosed () const final override
	{ return getNode <X3DNurbsSurfaceGeometryNode> () -> uClosed (); }

	virtual
	SFBool &
	vClosed () final override
	{ return getNode <X3DNurbsSurfaceGeometryNode> () -> vClosed (); }

	virtual
	const SFBool &
	vClosed () const final override
	{ return getNode <X3DNurbsSurfaceGeometryNode> () -> vClosed (); }

	virtual
	SFInt32 &
	uOrder () final override
	{ return getNode <X3DNurbsSurfaceGeometryNode> () -> uOrder (); }

	virtual
	const SFInt32 &
	uOrder () const final override
	{ return getNode <X3DNurbsSurfaceGeometryNode> () -> uOrder (); }

	virtual
	SFInt32 &
	vOrder () final override
	{ return getNode <X3DNurbsSurfaceGeometryNode> () -> vOrder (); }

	virtual
	const SFInt32 &
	vOrder () const final override
	{ return getNode <X3DNurbsSurfaceGeometryNode> () -> vOrder (); }

	virtual
	SFInt32 &
	uDimension () final override
	{ return getNode <X3DNurbsSurfaceGeometryNode> () -> uDimension (); }

	virtual
	const SFInt32 &
	uDimension () const final override
	{ return getNode <X3DNurbsSurfaceGeometryNode> () -> uDimension (); }

	virtual
	SFInt32 &
	vDimension () final override
	{ return getNode <X3DNurbsSurfaceGeometryNode> () -> vDimension (); }

	virtual
	const SFInt32 &
	vDimension () const final override
	{ return getNode <X3DNurbsSurfaceGeometryNode> () -> vDimension (); }

	virtual
	MFDouble &
	uKnot () final override
	{ return getNode <X3DNurbsSurfaceGeometryNode> () -> uKnot (); }

	virtual
	const MFDouble &
	uKnot () const final override
	{ return getNode <X3DNurbsSurfaceGeometryNode> () -> uKnot (); }

	virtual
	MFDouble &
	vKnot () final override
	{ return getNode <X3DNurbsSurfaceGeometryNode> () -> vKnot (); }

	virtual
	const MFDouble &
	vKnot () const final override
	{ return getNode <X3DNurbsSurfaceGeometryNode> () -> vKnot (); }

	virtual
	MFDouble &
	weight () final override
	{ return getNode <X3DNurbsSurfaceGeometryNode> () -> weight (); }

	virtual
	const MFDouble &
	weight () const final override
	{ return getNode <X3DNurbsSurfaceGeometryNode> () -> weight (); }

	virtual
	SFNode &
	texCoord () final override
	{ return getNode <X3DNurbsSurfaceGeometryNode> () -> texCoord (); }

	virtual
	const SFNode &
	texCoord () const final override
	{ return getNode <X3DNurbsSurfaceGeometryNode> () -> texCoord (); }

	virtual
	SFNode &
	controlPoint () final override
	{ return getNode <X3DNurbsSurfaceGeometryNode> () -> controlPoint (); }

	virtual
	const SFNode &
	controlPoint () const final override
	{ return getNode <X3DNurbsSurfaceGeometryNode> () -> controlPoint (); }

	///  @name Member access

	virtual
	const X3DPtr <X3DCoordinateNode> &
	getControlPoint () const final override
	{ return getNode <X3DNurbsSurfaceGeometryNode> () -> getControlPoint (); }

	///  @name Destruction

	virtual
	void
	dispose () override;

	virtual
	~X3DNurbsSurfaceGeometryNodeTool () override;


protected:

	///  @name Construction

	X3DNurbsSurfaceGeometryNodeTool ();

	virtual
	void
	initialize () override;


private:

	///  @name Event handlers

	void
	set_load_state ();

	void
	set_controlPoint ();

	void
	eventProcessed ();

	///  @name Members

	X3DPtr <IndexedLineSet> controlPointLines;

};

} // X3D
} // titania

#endif
