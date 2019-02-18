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

#ifndef __TITANIA_X3D_COMPONENTS_GROUPING_X3DTRANSFORM_NODE_H__
#define __TITANIA_X3D_COMPONENTS_GROUPING_X3DTRANSFORM_NODE_H__

#include "../Grouping/X3DGroupingNode.h"
#include "../Grouping/X3DTransformMatrix3DObject.h"

namespace titania {
namespace X3D {

class X3DTransformNode :
	virtual public X3DGroupingNode,
	virtual public X3DTransformMatrix3DObject
{
public:

	///  @name Fields

	virtual
	SFVec3f &
	translation ()
	{ return *fields .translation; }

	virtual
	const SFVec3f &
	translation () const
	{ return *fields .translation; }

	virtual
	SFRotation &
	rotation ()
	{ return *fields .rotation; }

	virtual
	const SFRotation &
	rotation () const
	{ return *fields .rotation; }

	virtual
	SFVec3f &
	scale ()
	{ return *fields .scale; }

	virtual
	const SFVec3f &
	scale () const
	{ return *fields .scale; }

	virtual
	SFRotation &
	scaleOrientation ()
	{ return *fields .scaleOrientation; }

	virtual
	const SFRotation &
	scaleOrientation () const
	{ return *fields .scaleOrientation; }

	virtual
	SFVec3f &
	center ()
	{ return *fields .center; }

	virtual
	const SFVec3f &
	center () const
	{ return *fields .center; }

	///  @name Member access

	virtual
	Box3d
	getBBox () const override;

	virtual
	bool
	getKeepCenter () const
	{ return false; }

	virtual
	Matrix4d
	getCurrentMatrix () const;

	virtual
	void
	setMatrix (const Matrix4d & matrix) override;

	virtual
	void
	setMatrixWithCenter (const Matrix4d & matrix, const Vector3d & center);

	virtual
	void
	setMatrixKeepCenter (const Matrix4d & matrix);

	///  @name Operations

	virtual
	void
	traverse (const TraverseType type, X3DRenderObject* const renderObject) override;
	
	///  @name Destruction

	virtual
	void
	dispose () override;

	virtual
	~X3DTransformNode () override;


protected:

	///  @name Construction

	X3DTransformNode ();

	virtual
	void
	initialize () override;

	///  @name Event handlers

	void
	eventsProcessed ();


private:

	///  @name Fields

	struct Fields
	{
		Fields ();

		SFVec3f* const translation;
		SFRotation* const rotation;
		SFVec3f* const scale;
		SFRotation* const scaleOrientation;
		SFVec3f* const center;
	};

	Fields fields;

};

} // X3D
} // titania

#endif
