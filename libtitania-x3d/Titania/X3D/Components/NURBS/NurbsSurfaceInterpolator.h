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

#ifndef __TITANIA_X3D_COMPONENTS_NURBS_NURBS_SURFACE_INTERPOLATOR_H__
#define __TITANIA_X3D_COMPONENTS_NURBS_NURBS_SURFACE_INTERPOLATOR_H__

#include "../Core/X3DChildNode.h"

#include <Titania/Math/Mesh/NurbsTessellator.h>

namespace titania {
namespace X3D {

class NurbsSurfaceInterpolator :
	virtual public X3DChildNode
{
public:

	NurbsSurfaceInterpolator (X3DExecutionContext* const executionContext);

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const executionContext) const final override;

	///  @name Common members

	virtual
	const Component &
	getComponent () const final override
	{ return component; }

	virtual
	const std::string &
	getTypeName () const final override
	{ return typeName; }

	virtual
	const std::string &
	getContainerField () const final override
	{ return containerField; }

	///  @name Fields

	SFVec2f &
	set_fraction ()
	{ return *fields .set_fraction; }

	const SFVec2f &
	set_fraction () const
	{ return *fields .set_fraction; }

	SFInt32 &
	uOrder ()
	{ return *fields .uOrder; }

	const SFInt32 &
	uOrder () const
	{ return *fields .uOrder; }

	SFInt32 &
	vOrder ()
	{ return *fields .vOrder; }

	const SFInt32 &
	vOrder () const
	{ return *fields .vOrder; }

	SFInt32 &
	uDimension ()
	{ return *fields .uDimension; }

	const SFInt32 &
	uDimension () const
	{ return *fields .uDimension; }

	SFInt32 &
	vDimension ()
	{ return *fields .vDimension; }

	const SFInt32 &
	vDimension () const
	{ return *fields .vDimension; }

	MFDouble &
	uKnot ()
	{ return *fields .uKnot; }

	const MFDouble &
	uKnot () const
	{ return *fields .uKnot; }

	MFDouble &
	vKnot ()
	{ return *fields .vKnot; }

	const MFDouble &
	vKnot () const
	{ return *fields .vKnot; }

	MFDouble &
	weight ()
	{ return *fields .weight; }

	const MFDouble &
	weight () const
	{ return *fields .weight; }

	SFNode &
	controlPoint ()
	{ return *fields .controlPoint; }

	const SFNode &
	controlPoint () const
	{ return *fields .controlPoint; }

	SFVec3f &
	normal_changed ()
	{ return *fields .normal_changed; }

	const SFVec3f &
	normal_changed () const
	{ return *fields .normal_changed; }

	SFVec3f &
	position_changed ()
	{ return *fields .position_changed; }

	const SFVec3f &
	position_changed () const
	{ return *fields .position_changed; }

	///  @name Destruction

	virtual
	~NurbsSurfaceInterpolator () final override;


protected:

	virtual
	void
	initialize () final override;


private:

	///  @name Event handlers

	void
	set_fraction_ ();

	///  @name Static members

	static const Component   component;
	static const std::string typeName;
	static const std::string containerField;

	static const size_t U_TESSELLATION;
	static const size_t V_TESSELLATION;

	///  @name Fields

	struct Fields
	{
		Fields ();

		SFVec2f* const set_fraction;
		SFInt32* const uOrder;
		SFInt32* const vOrder;
		SFInt32* const uDimension;
		SFInt32* const vDimension;
		MFDouble* const uKnot;
		MFDouble* const vKnot;
		MFDouble* const weight;
		SFNode* const controlPoint;
		SFVec3f* const normal_changed;
		SFVec3f* const position_changed;
	};

	Fields fields;

	///  @name Members

	X3DPtr <X3DCoordinateNode> controlPointNode;
	X3DPtr <NurbsPatchSurface> geometry;

};

} // X3D
} // titania

#endif
