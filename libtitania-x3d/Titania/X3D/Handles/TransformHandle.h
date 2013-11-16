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

#ifndef __TITANIA_X3D_HANDLES_TRANSFORM_HANDLE_H__
#define __TITANIA_X3D_HANDLES_TRANSFORM_HANDLE_H__

#include "../Handles/X3DHandleObject.h"

#include "../Components/Grouping/Transform.h"

namespace titania {
namespace X3D {

class Transform;

class TransformHandle :
	public Transform, public X3DHandleObject
{
public:

	TransformHandle (Transform* const, SFBool*, X3DExecutionContext* const);

	///  @name Fields

	virtual
	const SFVec3f &
	translation () const final
	{ return transform -> translation (); }

	virtual
	SFVec3f &
	translation () final
	{ return transform -> translation (); }

	virtual
	SFRotation &
	rotation () final
	{ return transform -> rotation (); }

	virtual
	const SFRotation &
	rotation () const final
	{ return transform -> rotation (); }

	virtual
	SFVec3f &
	scale () final
	{ return transform -> scale (); }

	virtual
	const SFVec3f &
	scale () const final
	{ return transform -> scale (); }

	virtual
	SFRotation &
	scaleOrientation () final
	{ return transform -> scaleOrientation (); }

	virtual
	const SFRotation &
	scaleOrientation () const final
	{ return transform -> scaleOrientation (); }

	virtual
	SFVec3f &
	center () final
	{ return transform -> center (); }

	virtual
	const SFVec3f &
	center () const final
	{ return transform -> center (); }

	virtual
	SFVec3f &
	bboxCenter () final
	{ return transform -> bboxCenter (); }

	virtual
	const SFVec3f &
	bboxCenter () const final
	{ return transform -> bboxCenter (); }

	virtual
	SFVec3f &
	bboxSize () final
	{ return transform -> bboxSize (); }

	virtual
	const SFVec3f &
	bboxSize () const final
	{ return transform -> bboxSize (); }

	virtual
	MFNode &
	addChildren () final
	{ return transform -> addChildren (); }

	virtual
	const MFNode &
	addChildren () const final
	{ return transform -> addChildren (); }

	virtual
	MFNode &
	removeChildren () final
	{ return transform -> removeChildren (); }

	virtual
	const MFNode &
	removeChildren () const final
	{ return transform -> removeChildren (); }

	virtual
	MFNode &
	children () final
	{ return transform -> children (); }

	virtual
	const MFNode &
	children () const final
	{ return transform -> children (); }

	///  @name Member access

	virtual
	void
	setName (const std::string &) final;

	virtual
	void
	setUserData (const UserDataPtr &) final;
	
	virtual
	X3DBaseNode*
	getLocalNode () final
	{ return transform; }

	virtual
	Box3f
	getBBox () final;

	virtual
	void
	setMatrix (const Matrix4f & value) final
	{ transform -> setMatrix (value);  }

	virtual
	Matrix4f
	getMatrix () final
	{ return transform -> getMatrix (); }

	virtual
	void
	addHandle (SFBool*) final
	{ }

	virtual
	void
	removeHandle () final;

	virtual
	void
	addEvent (X3DChildObject* const) final
	{ }

	virtual
	void
	addEvent (X3DChildObject* const, const EventPtr &) final
	{ }

	virtual
	void
	traverse (const TraverseType type) final;

	virtual
	void
	dispose () final;


private:

	virtual
	void
	initialize () final;

	void
	reshape ();

	///  @name Members

	Transform*         transform;
	X3DSFNode <Scene>  scene;

	SFBool* isActive;

};

} // X3D
} // titania

#endif
