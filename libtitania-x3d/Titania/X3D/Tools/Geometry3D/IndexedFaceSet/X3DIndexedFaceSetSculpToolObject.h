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

#ifndef __TITANIA_X3D_TOOLS_GEOMETRY3D_INDEXED_FACE_SET_X3DINDEXED_FACE_SET_SCULP_TOOL_OBJECT_H__
#define __TITANIA_X3D_TOOLS_GEOMETRY3D_INDEXED_FACE_SET_X3DINDEXED_FACE_SET_SCULP_TOOL_OBJECT_H__

#include "X3DIndexedFaceSetBrushObject.h"

namespace titania {
namespace X3D {

class X3DIndexedFaceSetSculpToolObject :
	virtual public X3DIndexedFaceSetBrushObject
{
public:

	///  @name Destruction

	virtual
	void
	dispose ()
	{ }

	virtual
	~X3DIndexedFaceSetSculpToolObject () override;


protected:

	///  @name Construction

	X3DIndexedFaceSetSculpToolObject ();

	virtual
	void
	initialize () override;

	///  @name Member access

	virtual
	int32_t
	getToolNumber () const final override
	{ return ToolNumber::SCULP; }


private:

	///  @name Event handlers

	void
	set_loadState ();

	void
	set_coord ();

	void
	set_coord_points ();

	void
	set_touch_sensor_active ();

	void
	set_touch_sensor_hitPoint ();

	Vector3d
	getHeight (const Vector3d & hitNormal, const Vector3d & hitPoint, const Vector3d & point);

	Vector3d
	getSmoothHeight (const Vector3d & hitNormal, const Vector3d & hitPoint, const Vector3d & point);

	Vector3d
	getUndoHeight (const Vector3d & hitNormal, const Vector3d & hitPoint, const Vector3d & point, const Vector3d & undoPoint);

	double
	getCircularHeight (const Vector2d & v, const double w, const double s, const double e);
	
	double
	getSquaredHeight (const Vector2d & v, const double w, const double s, const double e);

	///  @name Members

	X3DPtr <TouchSensor>       touchSensor;
	X3DPtr <X3DCoordinateNode> coordNode;
	Vector3d                   lastHitPoint;
	double                     pointerDistance;
	std::vector <Vector3d>     undoPoints;
	UndoStepPtr                undoStep;

};

} // X3D
} // titania

#endif
