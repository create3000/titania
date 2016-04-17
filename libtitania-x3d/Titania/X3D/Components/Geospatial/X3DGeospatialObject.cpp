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
 * Copyright 1999, 2012 Holger Seelig <holger.seelig@yahoo.de>.
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

#include "X3DGeospatialObject.h"

#include "../../Browser/Core/Cast.h"
#include "../../Types/Geometry.h"
#include "../../Execution/X3DScene.h"

#include <Titania/Geospatial/Interpolation.h>

namespace titania {
namespace X3D {

X3DGeospatialObject::Fields::Fields () :
	geoSystem (new MFString ({ "GD", "WE" })),
	geoOrigin (new SFNode ())
{ }

X3DGeospatialObject::X3DGeospatialObject () :
	     X3DBaseNode (),
	          fields (),
	coordinateSystem (Geospatial::CoordinateSystemType::GD),
	  referenceFrame (),
	  elevationFrame (),
	   standardOrder (true),
	   geoOriginNode (),
	          origin (),
	    originMatrix (),
	 invOriginMatrix (),
	       rotateYUp (false),
	         radians (false)
{
	addType (X3DConstants::X3DGeospatialObject);

	addChildren (geoOriginNode);
}

void
X3DGeospatialObject::initialize ()
{
	geoSystem () .addInterest (this, &X3DGeospatialObject::set_geoSystem);
	geoOrigin () .addInterest (this, &X3DGeospatialObject::set_geoOrigin);

	set_geoSystem ();
	set_geoOrigin ();
}

void
X3DGeospatialObject::set_geoSystem ()
{
	coordinateSystem = Geospatial::getCoordinateSystem (geoSystem ());
	referenceFrame   = Geospatial::getReferenceFrame (geoSystem (), radians);
	elevationFrame   = Geospatial::getElevationFrame (geoSystem (), radians);
	standardOrder    = Geospatial::isStandardOrder (geoSystem ());
}

void
X3DGeospatialObject::set_geoOrigin ()
{
	if (geoOriginNode)
	{
		geoOriginNode -> removeInterest (this, &X3DGeospatialObject::set_origin);
		geoOriginNode -> removeInterest (this);
	}

	geoOriginNode .set (x3d_cast <GeoOrigin*> (geoOrigin ()));

	if (geoOriginNode)
	{
		geoOriginNode -> addInterest (this, &X3DGeospatialObject::set_origin);
		geoOriginNode -> addInterest (this);
	}

	set_origin ();
	set_rotateYUp ();
}

void
X3DGeospatialObject::set_origin ()
{
	if (geoOriginNode)
		origin = geoOriginNode -> getOrigin ();
	else
		origin = Vector3d ();

	set_originMatrix ();
}

void
X3DGeospatialObject::set_originMatrix ()
{
	try
	{
		const Vector3d t = origin;
	
		// Let's work out the orientation at that location in order
		// to maintain a view where +Y is in the direction of gravitional
		// up for that region of the planet's surface. This will be the
		// value of the rotation matrix for the transform.
	
		const Vector3d y = elevationFrame -> normal (t);
		Vector3d       x = cross (Vector3d (0, 0, 1), y);
	
		// Tool poles
	
		if (x == Vector3d ())
			x = Vector3d (1, 0, 0);
	
		Vector3d z = cross (x, y);
	
		x .normalize ();
		z .normalize ();
	
		originMatrix = Matrix4d (x [0], x [1], x [2], 0,
		                         y [0], y [1], y [2], 0,
		                         z [0], z [1], z [2], 0,
		                         t [0], t [1], t [2], 1);
	
		invOriginMatrix = ~originMatrix;
	}
	catch (std::domain_error &)
	{
		/// ???
	}
}

void
X3DGeospatialObject::set_rotateYUp ()
{
	rotateYUp = geoOriginNode and geoOriginNode -> rotateYUp ();
}

Vector3d
X3DGeospatialObject::getCoord (const Vector3d & geoPoint) const
{
	const auto point = referenceFrame -> convert (geoPoint);

	if (rotateYUp)
		return point * invOriginMatrix;

	return point - origin;
}

Vector3d
X3DGeospatialObject::getGeoCoord (const Vector3d & point) const
{
	if (rotateYUp)
		return referenceFrame -> apply (point * originMatrix);

	return referenceFrame -> apply (point + origin);
}

double
X3DGeospatialObject::getElevation (const Vector3d & point) const
{
	return getGeoCoord (point) .z ();
}

Vector3d
X3DGeospatialObject::getUpVector (const Vector3d & point) const
{
	if (rotateYUp)
		return invOriginMatrix .mult_dir_matrix (elevationFrame -> normal (point * originMatrix));

	return elevationFrame -> normal (point + origin);
}

Matrix4d
X3DGeospatialObject::getStandardLocationMatrix (const Vector3d & geoPoint) const
{
	// Position

	const Vector3d t = referenceFrame -> convert (geoPoint);

	// Let's work out the orientation at that location in order
	// to maintain a view where +Y is in the direction of gravitional
	// up for that region of the planet's surface. This will be the
	// value of the rotation matrix for the transform.

	const Vector3d y = elevationFrame -> normal (t);
	Vector3d       x = cross (Vector3d (0, 0, 1), y);

	// Tool poles

	if (x == Vector3d ())
		x = Vector3d (1, 0, 0);

	Vector3d z = cross (x, y);

	x .normalize ();
	z .normalize ();

	return Matrix4d (x [0], x [1], x [2], 0,
	                 y [0], y [1], y [2], 0,
	                 z [0], z [1], z [2], 0,
	                 t [0], t [1], t [2], 1);
}

Matrix4d
X3DGeospatialObject::getLocationMatrix (const Vector3d & geoPoint) const
{
	auto locationMatrix = getStandardLocationMatrix (geoPoint);

	if (rotateYUp)
		return locationMatrix * invOriginMatrix;

	locationMatrix .translate (origin);
	return locationMatrix;
}

Vector3d
X3DGeospatialObject::lerp (const Vector3d & source, const Vector3d & destination, const double weight)
{
	switch (getCoordinateSystem ())
	{
		case Geospatial::CoordinateSystemType::GD:
			return geospatial::gd_lerp <double> (source, destination, weight, standardOrder, radians);

		case Geospatial::CoordinateSystemType::UTM:
			return geospatial::utm_lerp (source, destination, weight);

		case Geospatial::CoordinateSystemType::GC:
			return geospatial::gc_lerp (source, destination, weight);
	}

	return Vector3d ();
}

} // X3D
} // titania
