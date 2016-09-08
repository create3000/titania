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

#ifndef __TITANIA_X3D_BROWSER_POINTING_DEVICE_SENSOR_X3DPOINTING_DEVICE_SENSOR_CONTEXT_H__
#define __TITANIA_X3D_BROWSER_POINTING_DEVICE_SENSOR_X3DPOINTING_DEVICE_SENSOR_CONTEXT_H__

#include "../../Basic/NodeSet.h"
#include "../../Fields.h"
#include "../../Types/Geometry.h"
#include "../../Types/Pointer.h"
#include "HitArray.h"
#include "Intersection.h"
#include "SelectionType.h"

namespace titania {
namespace X3D {

class FrameBuffer;

class X3DPointingDeviceSensorContext :
	virtual public X3DBaseNode
{
public:

	///  @name Outputs

	void
	isPickable (const bool value)
	{ pickable = value; }

	const SFBool &
	isPickable () const
	{ return pickable; }

	///  @name Member access

	const HitPtr &
	getNearestHit () const
	{ return getHits () .back (); }

	const HitArray &
	getHits () const
	{ return hits; }

	X3DLayerNode*
	getSelectedLayer () const
	{ return selectedLayer; }

	void
	setLayerNumber (const size_t value)
	{ layerNumber = value; }

	size_t
	getLayerNumber () const
	{ return layerNumber; }

	void
	setSelectionType (const SelectionType value)
	{ selectionType = value; }

	SelectionType
	getSelectionType () const
	{ return selectionType; }

	const std::shared_ptr <FrameBuffer> &
	getSelectionBuffer () const
	{ return selectionBuffer; }

	std::shared_ptr <FrameBuffer> &
	getSelectionBuffer ()
	{ return selectionBuffer; }

	const std::shared_ptr <FrameBuffer> &
	getDepthBuffer () const
	{ return depthBuffer; }

	std::shared_ptr <FrameBuffer> &
	getDepthBuffer ()
	{ return depthBuffer; }

	///  @name Operations

	void
	touch (const double, const double);

	bool
	isPointerInRectangle (const Vector4i &) const;

	std::vector <PointingDeviceSensorSet> &
	getSensors ()
	{ return enabledSensors; }

	void
	setHitRay (const Vector4i & viewport);

	const Line3d &
	getHitRay () const
	{ return hitRay; }

	void
	addHit (const Matrix4d &, const IntersectionPtr &, X3DShapeNode* const, X3DLayerNode* const);

	///  @name Event handlers

	bool
	setMotionNotifyEvent (const double, const double);

	bool
	setButtonPressEvent (const double, const double);

	bool
	setButtonReleaseEvent ();

	void
	setLeaveNotifyEvent ();

	///  @name Destruction

	virtual
	void
	dispose () override
	{ }

	virtual
	~X3DPointingDeviceSensorContext ();


protected:

	///  @name Construction

	X3DPointingDeviceSensorContext ();

	virtual
	void
	initialize () override;


private:

	///  @name Members access

	virtual
	const WorldPtr &
	getWorld () const = 0;

	///  @name Event handlers

	void
	set_shutdown ();

	///  @name Operations

	void
	motion ();

	//  @name Members

	SFBool                                    pickable;
	Vector2d                                  pointer;
	Line3d                                    hitRay;
	HitArray                                  hits;
	std::vector <PointingDeviceSensorSet>     enabledSensors;
	X3DPtrArray <X3DPointingDeviceSensorNode> overSensors;
	X3DPtrArray <X3DPointingDeviceSensorNode> activeSensors;
	X3DLayerNodePtr                           selectedLayer;
	size_t                                    layerNumber;
	time_type                                 pressTime;
	bool                                      hasMoved;
	SelectionType                             selectionType;
	std::shared_ptr <FrameBuffer>             selectionBuffer;
	std::shared_ptr <FrameBuffer>             depthBuffer;

};

} // X3D
} // titania

#endif
