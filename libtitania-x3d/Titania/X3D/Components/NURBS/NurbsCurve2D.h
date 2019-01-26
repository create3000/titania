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

#ifndef __TITANIA_X3D_COMPONENTS_NURBS_NURBS_CURVE2D_H__
#define __TITANIA_X3D_COMPONENTS_NURBS_NURBS_CURVE2D_H__

#include "../NURBS/X3DNurbsControlCurveNode.h"

namespace titania {
namespace X3D {

class NurbsCurve2D :
	public X3DNurbsControlCurveNode
{
public:

	///  @name Construction

	NurbsCurve2D (X3DExecutionContext* const executionContext);

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

	SFInt32 &
	tessellation ()
	{ return *fields .tessellation; }

	const SFInt32 &
	tessellation () const
	{ return *fields .tessellation; }

	SFBool &
	closed ()
	{ return *fields .closed; }

	const SFBool &
	closed () const
	{ return *fields .closed; }

	SFInt32 &
	order ()
	{ return *fields .order; }

	const SFInt32 &
	order () const
	{ return *fields .order; }

	MFDouble &
	knot ()
	{ return *fields .knot; }

	const MFDouble &
	knot () const
	{ return *fields .knot; }

	MFDouble &
	weight ()
	{ return *fields .weight; }

	const MFDouble &
	weight () const
	{ return *fields .weight; }

	///  @name Operations

	virtual
	std::vector <Vector2f>
	tessellate () const final override;

	virtual
	void
	trim (nurbs_tessellator & tessellator) const final override;
	

private:

	///  @name Operations

	size_t
	getTessellation (const size_t dimension) const;

	bool
	getClosed (const size_t order,
	           const std::vector <double> & knot,
	           const std::vector <double> & weight,
	           const std::vector <Vector2d> & controlPoint) const;

	std::vector <float>
	getKnots (const bool closed,
	          const size_t order,
	          const size_t dimension,
	          const std::vector <double> & knot) const;

	std::vector <Vector3f>
	getControlPoints (const bool closed,
	                  const size_t order,
	                  const std::vector <double> & weight,
	                  const std::vector <Vector2d> & controlPoint) const;

	///  @name Static members

	static const Component   component;
	static const std::string typeName;
	static const std::string containerField;

	///  @name Fields

	struct Fields
	{
		Fields ();

		SFInt32* const tessellation;
		SFBool* const closed;
		SFInt32* const order;
		MFDouble* const knot;
		MFDouble* const weight;
	};

	Fields fields;

};

} // X3D
} // titania

#endif
