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

#ifndef __TITANIA_X3D_TOOLS_GROUPING_TRANSFORM_TOOL_H__
#define __TITANIA_X3D_TOOLS_GROUPING_TRANSFORM_TOOL_H__

#include "../Core/X3DNodeTool.h"

#include "../../Components/Grouping/Transform.h"
#include "../../Types/Pointer.h"

namespace titania {
namespace X3D {

using X3DTransformToolNode = class X3DNodeTool <Transform>;

class TransformTool :
	public X3DTransformToolNode
{
public:

	///  @name Construction

	TransformTool (Transform* const);

	///  @name Fields

	virtual
	const SFVec3f &
	translation () const final override
	{ return getNode () -> translation (); }

	virtual
	SFVec3f &
	translation () final override
	{ return getNode () -> translation (); }

	virtual
	SFRotation &
	rotation () final override
	{ return getNode () -> rotation (); }

	virtual
	const SFRotation &
	rotation () const final override
	{ return getNode () -> rotation (); }

	virtual
	SFVec3f &
	scale () final override
	{ return getNode () -> scale (); }

	virtual
	const SFVec3f &
	scale () const final override
	{ return getNode () -> scale (); }

	virtual
	SFRotation &
	scaleOrientation () final override
	{ return getNode () -> scaleOrientation (); }

	virtual
	const SFRotation &
	scaleOrientation () const final override
	{ return getNode () -> scaleOrientation (); }

	virtual
	SFVec3f &
	center () final override
	{ return getNode () -> center (); }

	virtual
	const SFVec3f &
	center () const final override
	{ return getNode () -> center (); }

	virtual
	SFVec3f &
	bboxCenter () final override
	{ return getNode () -> bboxCenter (); }

	virtual
	const SFVec3f &
	bboxCenter () const final override
	{ return getNode () -> bboxCenter (); }

	virtual
	SFVec3f &
	bboxSize () final override
	{ return getNode () -> bboxSize (); }

	virtual
	const SFVec3f &
	bboxSize () const final override
	{ return getNode () -> bboxSize (); }

	virtual
	MFNode &
	addChildren () final override
	{ return getNode () -> addChildren (); }

	virtual
	const MFNode &
	addChildren () const final override
	{ return getNode () -> addChildren (); }

	virtual
	MFNode &
	removeChildren () final override
	{ return getNode () -> removeChildren (); }

	virtual
	const MFNode &
	removeChildren () const final override
	{ return getNode () -> removeChildren (); }

	virtual
	MFNode &
	children () final override
	{ return getNode () -> children (); }

	virtual
	const MFNode &
	children () const final override
	{ return getNode () -> children (); }

	///  @name Member access

	virtual
	void
	setMatrix (const Matrix4d &) final override;

	virtual
	void
	setMatrixWithCenter (const Matrix4d &, const Vector3f &) final override;

	virtual
	const Matrix4f &
	getMatrix () const final override
	{ return getNode () -> getMatrix (); }

	///  @name Root node handling

	virtual
	MFNode &
	getRootNodes ()
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final override;

	virtual
	const MFNode &
	getRootNodes () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final override;

	///  @name Operatations

	virtual
	Box3f
	getBBox () const final override;

	virtual
	void
	traverse (const TraverseType type) final override;


private:

	///  @name Construction

	virtual
	void
	initialize () final override;

	void
	interestsProcessed ();

	///  @name Operatations

	void
	addAbsoluteMatrix (const Matrix4d &);

	void
	reshape ();

	///  @name Members

	ScenePtr scene;

	Matrix4d parentMatrix;
	Matrix4d matrix;
	size_t   interestEvents;

};

} // X3D
} // titania

#endif
