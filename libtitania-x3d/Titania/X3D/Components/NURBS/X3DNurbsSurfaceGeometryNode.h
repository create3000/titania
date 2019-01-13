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

#ifndef __TITANIA_X3D_COMPONENTS_NURBS_X3DNURBS_SURFACE_GEOMETRY_NODE_H__
#define __TITANIA_X3D_COMPONENTS_NURBS_X3DNURBS_SURFACE_GEOMETRY_NODE_H__

#include "../NURBS/X3DParametricGeometryNode.h"
#include "../Rendering/X3DCoordinateNode.h"
#include "../Texturing/X3DTextureCoordinateNode.h"

#include <Titania/Math/Mesh/NurbsTessellator.h>

namespace titania {
namespace X3D {

class X3DNurbsSurfaceGeometryNode :
	virtual public X3DParametricGeometryNode
{
public:

	///  @name Fields

	virtual
	SFBool &
	solid ()
	{ return *fields .solid; }

	virtual
	const SFBool &
	solid () const
	{ return *fields .solid; }

	virtual
	SFInt32 &
	uTessellation ()
	{ return *fields .uTessellation; }

	virtual
	const SFInt32 &
	uTessellation () const
	{ return *fields .uTessellation; }

	virtual
	SFInt32 &
	vTessellation ()
	{ return *fields .vTessellation; }

	virtual
	const SFInt32 &
	vTessellation () const
	{ return *fields .vTessellation; }

	virtual
	SFBool &
	uClosed ()
	{ return *fields .uClosed; }

	virtual
	const SFBool &
	uClosed () const
	{ return *fields .uClosed; }

	virtual
	SFBool &
	vClosed ()
	{ return *fields .vClosed; }

	virtual
	const SFBool &
	vClosed () const
	{ return *fields .vClosed; }

	virtual
	SFInt32 &
	uOrder ()
	{ return *fields .uOrder; }

	virtual
	const SFInt32 &
	uOrder () const
	{ return *fields .uOrder; }

	virtual
	SFInt32 &
	vOrder ()
	{ return *fields .vOrder; }

	virtual
	const SFInt32 &
	vOrder () const
	{ return *fields .vOrder; }

	virtual
	SFInt32 &
	uDimension ()
	{ return *fields .uDimension; }

	virtual
	const SFInt32 &
	uDimension () const
	{ return *fields .uDimension; }

	virtual
	SFInt32 &
	vDimension ()
	{ return *fields .vDimension; }

	virtual
	const SFInt32 &
	vDimension () const
	{ return *fields .vDimension; }

	virtual
	MFDouble &
	uKnot ()
	{ return *fields .uKnot; }

	virtual
	const MFDouble &
	uKnot () const
	{ return *fields .uKnot; }

	virtual
	MFDouble &
	vKnot ()
	{ return *fields .vKnot; }

	virtual
	const MFDouble &
	vKnot () const
	{ return *fields .vKnot; }

	virtual
	MFDouble &
	weight ()
	{ return *fields .weight; }

	virtual
	const MFDouble &
	weight () const
	{ return *fields .weight; }

	virtual
	SFNode &
	texCoord ()
	{ return *fields .texCoord; }

	virtual
	const SFNode &
	texCoord () const
	{ return *fields .texCoord; }

	virtual
	SFNode &
	controlPoint ()
	{ return *fields .controlPoint; }

	virtual
	const SFNode &
	controlPoint () const
	{ return *fields .controlPoint; }

	///  @name Member access

	virtual
	const X3DPtr <X3DCoordinateNode> &
	getControlPoint () const
	{ return controlPointNode; }

	///  @name Destruction

	~X3DNurbsSurfaceGeometryNode ();


protected:

	///  @name Construction

	X3DNurbsSurfaceGeometryNode ();

	virtual
	void
	initialize () override;

	///  @name Operations

	virtual
	void
	trimSurface (nurbs_tessellator & tessellator) const
	{ }


private:

	///  @name Event handlers

	void
	set_texCoord ();

	void
	set_controlPoint ();

	///  @name Operations

	size_t
	getUTessellation (const size_t uDimension) const;

	size_t
	getVTessellation (const size_t vDimension) const;

	bool
	getUClosed (const size_t uOrder,
	            const size_t uDimension,
	            const size_t vDimension,
	            const std::vector <double> & uKnot,
	            const std::vector <double> & weight) const;

	bool
	getVClosed (const size_t vOrder,
	            const size_t uDimension,
	            const size_t vDimension,
	            const std::vector <double> & vKnot,
	            const std::vector <double> & weight) const;

	std::vector <float>
	getKnots (const std::vector <double> & knot, const bool closed, const size_t order, const size_t dimension) const;

	std::vector <Vector4f>
	getTexControlPoints (const bool uClosed,
                        const bool vClosed,
                        const size_t uOrder,
                        const size_t vOrder,
                        const size_t uDimension,
                        const size_t vDimension) const;

	std::vector <Vector4f>
	getControlPoints (const bool uClosed,
                     const bool vClosed,
                     const size_t uOrder,
                     const size_t vOrder,
                     const size_t uDimension,
                     const size_t vDimension,
                     const std::vector <double> & weight) const;

	virtual
	void
	build () final override;

	///  @name Members
	
	struct Fields
	{
		Fields ();

		SFBool* const solid;
		SFInt32* const uTessellation;
		SFInt32* const vTessellation;
		SFBool* const uClosed;
		SFBool* const vClosed;
		SFInt32* const uOrder;
		SFInt32* const vOrder;
		SFInt32* const uDimension;
		SFInt32* const vDimension;
		MFDouble* const uKnot;
		MFDouble* const vKnot;
		MFDouble* const weight;
		SFNode* const texCoord;
		SFNode* const controlPoint;
	};

	Fields fields;
	
	X3DPtr <X3DTextureCoordinateNode> texCoordNode;
	X3DPtr <NurbsTextureCoordinate>   nurbsTexCoordNode;
	X3DPtr <X3DCoordinateNode>        controlPointNode;

};

} // X3D
} // titania

#endif
