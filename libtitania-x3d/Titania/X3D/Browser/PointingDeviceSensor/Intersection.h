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

#ifndef __TITANIA_X3D_BROWSER_POINTING_DEVICE_SENSOR_INTERSECTION_H__
#define __TITANIA_X3D_BROWSER_POINTING_DEVICE_SENSOR_INTERSECTION_H__

#include "../../Types/Numbers.h"

#include <array>
#include <memory>
#include <vector>

namespace titania {
namespace X3D {

class Intersection
{
public:

	///  @name Construction

	Intersection (const Vector4f & texCoord,
	              const Vector3f & normal,
	              const Vector3f & faceNormal,
	              const Vector3d & point,
	              const std::array <Vector3d, 3> & triangle,
	              std::vector <Vector3d> && face) :
		  texCoord (texCoord),
		    normal (normal),
		faceNormal (faceNormal),
		     point (point),
		  triangle (triangle),
		      face (std::move (face))
	{ }

	Intersection () :
		  texCoord (),
		    normal (),
		faceNormal (),
		     point (),
		  triangle (),
		      face ()
	{ }

	///  @name Member access

	const Vector4f &
	getTexCoord () const
	{ return texCoord; }

	const Vector3f &
	getNormal () const
	{ return normal; }

	const Vector3f &
	getFaceNormal () const
	{ return faceNormal; }

	const Vector3d &
	getPoint () const
	{ return point; }

	const std::array <Vector3d, 3> &
	getTriangle () const
	{ return triangle; }

	const std::vector <Vector3d> &
	getFace () const
	{ return face; }

	///  @name Destruction

	~Intersection ()
	{ }

protected:

	///  @name Friends

	friend class Shape;

	///  @name Member access

	void
	setNormal (const Vector3f & value)
	{ normal = value; }

	void
	setFaceNormal (const Vector3f & value)
	{ faceNormal = value; }

	void
	setPoint (const Vector3d & value)
	{ point = value; }
	

private:

	///  @name Members

	const Vector4f texCoord;
	Vector3f       normal;
	Vector3f       faceNormal;
	Vector3d       point;

	const std::array <Vector3d, 3> triangle;
	const std::vector <Vector3d>   face;

};

using IntersectionPtr = std::shared_ptr <Intersection>;

} // X3D
} // titania

#endif
