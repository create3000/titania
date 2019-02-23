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

#ifndef __TITANIA_X3D_TOOLS_GROUPING_X3DBOUNDED_OBJECT_TOOL_H__
#define __TITANIA_X3D_TOOLS_GROUPING_X3DBOUNDED_OBJECT_TOOL_H__

#include "../Core/X3DBaseTool.h"
#include "../../Browser/Shape/Linetypes.h"
#include "../../Components/Grouping/X3DBoundedObject.h"

namespace titania {
namespace X3D {

class X3DBoundedObjectTool :
	virtual public X3DBoundedObject,
	virtual public X3DBaseTool
{
public:

	///  @name Fields

	virtual
	SFVec3f &
	bboxCenter () final override
	{ return getNode <X3DBoundedObject> () -> bboxCenter (); }

	virtual
	const SFVec3f &
	bboxCenter () const final override
	{ return getNode <X3DBoundedObject> () -> bboxCenter (); }

	virtual
	SFVec3f &
	bboxSize () final override
	{ return getNode <X3DBoundedObject> () -> bboxSize (); }

	virtual
	const SFVec3f &
	bboxSize () const final override
	{ return getNode <X3DBoundedObject> () -> bboxSize (); }

	///  @name Member access

	virtual
	Box3d
	getBBox () const override
	{ return getNode <X3DBoundedObject> () -> getBBox (); }

	virtual
	void
	addTransformSensor (TransformSensor* const transformSensor) final override
	{ return getNode <X3DBoundedObject> () -> addTransformSensor (transformSensor); }

	virtual
	void
	removeTransformSensor (TransformSensor* const transformSensor) final override
	{ return getNode <X3DBoundedObject> () -> removeTransformSensor (transformSensor); }

	virtual
	const X3DPtrArray <TransformSensor> &
	getTransformSensors () const final override
	{ return getNode <X3DBoundedObject> () -> getTransformSensors (); }

	///  @name Operatations

	virtual
	void
	traverse (const TraverseType type, X3DRenderObject* const renderObject) override;

	///  @name Destruction

	virtual
	void
	dispose () override
	{ }


protected:

	///  @name Construction

	X3DBoundedObjectTool (const Color3f & color);

	virtual
	void
	initialize () override;

	virtual
	void
	realize () override;

	///  @name Member acces

	void
	setDisplayCenter (const bool value)
	{ displayCenter = value; }

	void
	setLinetype (const LineType value);

	LineType
	getLinetype () const
	{ return linetype; }

	virtual
	const Matrix4d &
	getMatrix () const
	{ return matrix; }


protected:

	///  @name Operations

	Box3d
	getGroupBBox () const;

	virtual
	void
	reshape ();


private:

	///  @name Members

	LineType linetype;
	bool     displayCenter;
	Color3f  color;
	Matrix4d matrix;

};

} // X3D
} // titania

#endif
