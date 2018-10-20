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

#ifndef __TITANIA_X3D_TOOLS_GRIDS_X3DGRID_TOOL_H__
#define __TITANIA_X3D_TOOLS_GRIDS_X3DGRID_TOOL_H__

#include "../Layering/X3DActiveLayerTool.h"

#include "../../Fields/X3DWeakPtrArray.h"

namespace titania {
namespace X3D {

class X3DTransformNode;

class X3DGridTool :
	public X3DActiveLayerTool
{
public:

	///  @name Common members

	virtual
	void
	setExecutionContext (X3DExecutionContext* const executionContext)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final override;

	///  @name Fields

	SFVec3f &
	translation ()
	{ return *fields .translation; }

	const SFVec3f &
	translation () const
	{ return *fields .translation; }

	SFRotation &
	rotation ()
	{ return *fields .rotation; }

	const SFRotation &
	rotation () const
	{ return *fields .rotation; }

	SFVec3f &
	scale ()
	{ return *fields .scale; }

	const SFVec3f &
	scale () const
	{ return *fields .scale; }

	MFInt32 &
	dimension ()
	{ return *fields .dimension; }

	const MFInt32 &
	dimension () const
	{ return *fields .dimension; }

	MFInt32 &
	majorLineEvery ()
	{ return *fields .majorLineEvery; }

	const MFInt32 &
	majorLineEvery () const
	{ return *fields .majorLineEvery; }

	MFInt32 &
	majorLineOffset ()
	{ return *fields .majorLineOffset; }

	const MFInt32 &
	majorLineOffset () const
	{ return *fields .majorLineOffset; }

	SFColorRGBA &
	color ()
	{ return *fields .color; }

	const SFColorRGBA &
	color () const
	{ return *fields .color; }

	SFColorRGBA &
	lineColor ()
	{ return *fields .lineColor; }

	const SFColorRGBA &
	lineColor () const
	{ return *fields .lineColor; }

	SFColorRGBA &
	majorLineColor ()
	{ return *fields .majorLineColor; }

	const SFColorRGBA &
	majorLineColor () const
	{ return *fields .majorLineColor; }

	SFBool &
	snapToCenter ()
	{ return *fields .snapToCenter; }

	const SFBool &
	snapToCenter () const
	{ return *fields .snapToCenter; }

	SFDouble &
	snapDistance ()
	{ return *fields .snapDistance; }

	const SFDouble &
	snapDistance () const
	{ return *fields .snapDistance; }

	SFBool &
	snapping ()
	{ return *fields .snapping; }

	const SFBool &
	snapping () const
	{ return *fields .snapping; }

	SFBool &
	collision ()
	{ return *fields .collision; }

	const SFBool &
	collision () const
	{ return *fields .collision; }

	SFBool &
	isActive ()
	{ return *fields .isActive; }

	const SFBool &
	isActive () const
	{ return *fields .isActive; }

	///  @name Destruction

	virtual
	~X3DGridTool () override;

protected:

	///  @name Construction

	X3DGridTool ();

	virtual
	void
	initialize () override;

	virtual
	void
	realize () override;

	///  @name Member access

	virtual
	bool
	getPickable () const final override;

	///  @name Operations

	virtual
	Vector3d
	getSnapPosition (const Vector3d &, const bool = true) = 0;

	virtual
	Vector3d
	getSnapPosition (const Vector3d &, const Vector3d &) = 0;


private:

	///  @name Event handlers

	void
	set_color ();

	void
	set_lineColor ();

	void
	set_majorLineColor ();

	void
	set_transform_tools (const X3DWeakPtrArray <X3DTransformNodeTool> & value);

	void
	set_translation (const X3DWeakPtr <X3DTransformNodeTool> & master);

	void
	set_rotation (const X3DWeakPtr <X3DTransformNodeTool> & master);

	void
	set_scale (const X3DWeakPtr <X3DTransformNodeTool> & master);

	Matrix4d
	getScaleMatrix (const X3DWeakPtr <X3DTransformNodeTool> & master, const size_t);

	Matrix4d
	getUniformScaleMatrix (const X3DWeakPtr <X3DTransformNodeTool> & master, const size_t);

	Matrix4d
	getOffset (const X3DWeakPtr <X3DTransformNodeTool> & master, const Box3d &, const Matrix4d, const Vector3d &) const;

	void
	setTransformGroup (const X3DWeakPtr <X3DTransformNodeTool> & master, const Matrix4d & snapMatrix);

	void
	connectTranslation (const X3DWeakPtr <X3DTransformNodeTool> & tool);

	void
	connectRotation (const X3DWeakPtr <X3DTransformNodeTool> & tool);

	void
	connectScale (const X3DWeakPtr <X3DTransformNodeTool> & tool);

	bool
	getScaleFromEdge (const X3DWeakPtr <X3DTransformNodeTool> & master) const;

	///  @name Fields

	struct Fields
	{
		Fields ();

		SFVec3f* const translation;
		SFRotation* const rotation;
		SFVec3f* const scale;
		MFInt32* const dimension;
		MFInt32* const majorLineEvery;
		MFInt32* const majorLineOffset;
		SFColorRGBA* const color;
		SFColorRGBA* const lineColor;
		SFColorRGBA* const majorLineColor;
		SFBool* const snapToCenter;
		SFDouble* const snapDistance;
		SFBool* const snapping;
		SFBool* const collision;
		SFBool* const isActive;
	};

	Fields fields;

	///  @name Members
	
	X3DWeakPtrArray <X3DTransformNode> transformNodes;

};

} // X3D
} // titania

#endif
