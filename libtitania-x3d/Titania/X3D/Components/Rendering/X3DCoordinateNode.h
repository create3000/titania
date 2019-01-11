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

#ifndef __TITANIA_X3D_COMPONENTS_RENDERING_X3DCOORDINATE_NODE_H__
#define __TITANIA_X3D_COMPONENTS_RENDERING_X3DCOORDINATE_NODE_H__

#include "../Rendering/X3DGeometricPropertyNode.h"
#include "../../Types/Geometry.h"

namespace titania {
namespace math {

template <class Type, class ... Args>
class tessellator;

} // math
} // titania

namespace titania {
namespace X3D {

class X3DCoordinateNode :
	public X3DGeometricPropertyNode
{
public:

	///  @name Member access

	virtual
	Box3d
	getBBox () const = 0;
	
	virtual
	void
	set1Point (const size_t index, const Vector3d & value) = 0;

	virtual
	Vector3d
	get1Point (const size_t index) const = 0;

	virtual
	Vector3f
	getNormal (const size_t index1, const size_t index2, const size_t index3) const = 0;

	virtual
	Vector3f
	getNormal (const size_t index1, const size_t index2, const size_t index3, const size_t index4) const = 0;

	virtual
	void
	addVertex (math::tessellator <double, size_t> & tessellator, const size_t index, const size_t i) const = 0;

	virtual
	void
	addVertex (std::vector <Vector3d> & vertices, const size_t index) const = 0;

	virtual
	std::vector <Vector4f>
	getControlPoints (const bool uClosed,
	                  const bool vClosed,
	                  const int32_t uOrder,
	                  const int32_t vOrder,
	                  const std::vector <double> & weight,
	                  const int32_t uDimension,
	                  const int32_t vDimension) const = 0;

	virtual
	void
	erasePoint (const size_t index) = 0;

	virtual
	bool
	isEmpty () const = 0;

	virtual
	size_t
	getSize () const = 0;

	///  @name Operations

	void
	assign (const X3DPtr <X3DCoordinateNode> & other);
	
	virtual
	void
	resize (const size_t value) = 0;


protected:

	X3DCoordinateNode ();

};

} // X3D
} // titania

#endif
