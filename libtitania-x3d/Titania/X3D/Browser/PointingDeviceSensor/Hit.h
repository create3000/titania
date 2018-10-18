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

#ifndef __TITANIA_X3D_BROWSER_POINTING_DEVICE_SENSOR_HIT_H__
#define __TITANIA_X3D_BROWSER_POINTING_DEVICE_SENSOR_HIT_H__

#include "../../Fields.h"
#include "../../Types/Geometry.h"
#include "../../Types/Pointer.h"
#include "Intersection.h"

#include <memory>
#include <tuple>

namespace titania {
namespace X3D {

class PointingDeviceSensorContainer;

using PointingDeviceSensorContainerPtr = std::shared_ptr <PointingDeviceSensorContainer>;
using PointingDeviceSensorContainerSet = std::set <PointingDeviceSensorContainerPtr>;

class Hit
{
public:

	///  @name Construction

	Hit (const Vector2d & pointer,
	     const Matrix4d & modelViewMatrix,
	     const Line3d & hitRay,
	     const IntersectionPtr & intersection,
	     const PointingDeviceSensorContainerSet & sensors,
	     const X3DPtr <X3DShapeNode> & shape,
	     const X3DPtr <X3DLayerNode> & layer,
	     const size_t layerNumber,
	     const double depthOffset,
	     MFNode && hierarchy);

	Hit (const Vector2d & pointer,
	     const Line3d & hitRay,
	     const X3DPtr <X3DLayerNode> & layer);

	///  @name Member access

	const Vector2d &
	getPointer () const
	{ return pointer; }

	const Matrix4d &
	getModelViewMatrix () const
	{ return modelViewMatrix; }

	const Line3d &
	getHitRay () const
	{ return hitRay; }

	const IntersectionPtr &
	getIntersection () const
	{ return intersection;   }     
         
	const double &
	getDistance () const
	{ return distance; }

	const PointingDeviceSensorContainerSet &
	getSensors () const
	{ return sensors; }

	const X3DPtr <X3DShapeNode> &
	getShape () const
	{ return shape; }

	const X3DPtr <X3DLayerNode> &
	getLayer () const
	{ return layer; }

	size_t
	getLayerNumber () const
	{ return layerNumber; }

	const MFNode &
	getHierarchy () const
	{ return hierarchy; }

	///  @name Destruction

	~Hit ();

private:

	///  @name Members

	const Vector2d                         pointer;
	const Matrix4d                         modelViewMatrix;
	const Line3d                           hitRay;
	const IntersectionPtr                  intersection;                
	const double                           distance;
	const PointingDeviceSensorContainerSet sensors;
	const X3DPtr <X3DShapeNode>            shape;
	const X3DPtr <X3DLayerNode>            layer;
	const size_t                           layerNumber;
	const MFNode                           hierarchy;

};

using HitPtr = std::shared_ptr <Hit>;

class HitComp
{
public:

	bool
	operator () (const HitPtr & lhs, const HitPtr & rhs) const
	{
		return std::make_tuple (lhs -> getLayerNumber (), lhs -> getDistance ()) < std::make_tuple (rhs -> getLayerNumber (), rhs -> getDistance ());
	}

};

} // X3D
} // titania

#endif
