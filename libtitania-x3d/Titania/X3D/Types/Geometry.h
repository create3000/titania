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

#ifndef __TITANIA_X3D_TYPES_GEOMETRY_H__
#define __TITANIA_X3D_TYPES_GEOMETRY_H__

#include <Titania/Math/Geometry/Box2.h>
#include <Titania/Math/Geometry/Box3.h>
#include <Titania/Math/Geometry/Cylinder3.h>
#include <Titania/Math/Geometry/Line2.h>
#include <Titania/Math/Geometry/Line3.h>
#include <Titania/Math/Geometry/LineSegment2.h>
#include <Titania/Math/Geometry/LineSegment3.h>
#include <Titania/Math/Geometry/Plane3.h>
#include <Titania/Math/Geometry/Sphere3.h>
#include <Titania/Math/Geometry/Spheroid3.h>
#include <Titania/Math/Geometry/Triangle3.h>

namespace titania {
namespace X3D {

using namespace titania::math;

using Box2d             = box2 <double>;
using Box2f             = box2 <float>;
using Box3d             = box3 <double>;
using Box3f             = box3 <float>;
using Cylinder3d        = cylinder3 <double>;
using Cylinder3f        = cylinder3 <float>;
using Line2d            = line2 <double>;
using Line2f            = line2 <float>;
using Line3d            = line3 <double>;
using Line3f            = line3 <float>;
using LineSegment2d     = line_segment2 <double>;
using LineSegment2f     = line_segment2 <float>;
using LineSegment3d     = line_segment3 <double>;
using LineSegment3f     = line_segment3 <float>;
using Plane3d           = plane3 <double>;
using Plane3f           = plane3 <float>;
using Sphere3d          = sphere3 <double>;
using Sphere3f          = sphere3 <float>;
using Spheroid3d        = spheroid3 <double>;
using Spheroid3f        = spheroid3 <double>;

} // X3D
} // titania

#endif
