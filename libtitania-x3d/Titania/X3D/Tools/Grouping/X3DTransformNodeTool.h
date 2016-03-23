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

#ifndef __TITANIA_X3D_TOOLS_GROUPING_X3DTRANSFORM_NODE_TOOL_H__
#define __TITANIA_X3D_TOOLS_GROUPING_X3DTRANSFORM_NODE_TOOL_H__

#include "../Grouping/X3DTransformMatrix3DNodeTool.h"
#include "../ToolColors.h"

#include "../../Browser/Networking/config.h"
#include "../../Browser/Selection.h"
#include "../../Browser/Tools/TransformToolOptions.h"
#include "../../Browser/X3DBrowser.h"

#include "../../Components/Grouping/X3DTransformNode.h"
#include "../../Components/Layering/X3DLayerNode.h"

namespace titania {
namespace X3D {

class X3DTransformNodeTool :
	virtual public X3DTransformNode,
	public X3DTransformMatrix3DNodeTool
{
public:

	///  @name Fields

	virtual
	const SFVec3f &
	translation () const final override
	{ return getNode <X3DTransformNode> () -> translation (); }

	virtual
	SFVec3f &
	translation () final override
	{ return getNode <X3DTransformNode> () -> translation (); }

	virtual
	SFRotation &
	rotation () final override
	{ return getNode <X3DTransformNode> () -> rotation (); }

	virtual
	const SFRotation &
	rotation () const final override
	{ return getNode <X3DTransformNode> () -> rotation (); }

	virtual
	SFVec3f &
	scale () final override
	{ return getNode <X3DTransformNode> () -> scale (); }

	virtual
	const SFVec3f &
	scale () const final override
	{ return getNode <X3DTransformNode> () -> scale (); }

	virtual
	SFRotation &
	scaleOrientation () final override
	{ return getNode <X3DTransformNode> () -> scaleOrientation (); }

	virtual
	const SFRotation &
	scaleOrientation () const final override
	{ return getNode <X3DTransformNode> () -> scaleOrientation (); }

	virtual
	SFVec3f &
	center () final override
	{ return getNode <X3DTransformNode> () -> center (); }

	virtual
	const SFVec3f &
	center () const final override
	{ return getNode <X3DTransformNode> () -> center (); }

	///  @name Hidden fields

	virtual
	SFBool &
	bbox ()
	{ return *fields .bbox; }

	virtual
	const SFBool &
	bbox () const
	{ return *fields .bbox; }

	virtual
	SFBool &
	isActive ()
	{ return *fields .isActive; }

	virtual
	const SFBool &
	isActive () const
	{ return *fields .isActive; }

	///  @name Member access

	virtual
	bool
	getKeepCenter () const final override;

	virtual
	Matrix4d
	getCurrentMatrix () const final override
	{ return getNode <X3DTransformNode> () -> getCurrentMatrix (); }

	virtual
	void
	setMatrix (const Matrix4d &) final override;

	virtual
	void
	setMatrixWithCenter (const Matrix4d &, const Vector3f &) final override;

	virtual
	void
	setMatrixKeepCenter (const Matrix4d &) final override;

	virtual
	void
	addAbsoluteMatrix (const Matrix4d &, const bool)
	throw (Error <NOT_SUPPORTED>) final override;

	virtual
	const Matrix4d &
	getTransformationMatrix () const
	throw (Error <NOT_SUPPORTED>) final override
	{ return transformationMatrix; }

	virtual
	Selection::ToolType
	getActiveTool () const final override
	{ return Selection::ToolType (getToolNode () -> template getField <SFInt32> ("activeTool_changed") .getValue ()); }

	///  @name Operatations

	virtual
	void
	traverse (const TraverseType type) override;


protected:

	///  @name Construction

	X3DTransformNodeTool ();

	virtual
	void
	initialize () override;


private:

	///  @name Construction

	virtual
	void
	realize () final override;

	void
	eventsProcessed ();

	///  @name Operatations

	void
	reshape ();

	///  @name Members

	struct Fields
	{
		Fields ();

		SFBool* const bbox;
		SFBool* const isActive;
	};

	Fields fields;

	Matrix4d transformationMatrix;
	Matrix4d matrix;
	bool     changing;

};

} // X3D
} // titania

#endif
