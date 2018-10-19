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

#ifndef __TITANIA_X3D_COMPONENTS_NAVIGATION_ORTHO_VIEWPOINT_H__
#define __TITANIA_X3D_COMPONENTS_NAVIGATION_ORTHO_VIEWPOINT_H__

#include "../Navigation/X3DViewpointNode.h"

namespace titania {
namespace X3D {

class OrthoViewpoint :
	virtual public X3DViewpointNode
{
public:

	///  @name Construction

	OrthoViewpoint (X3DExecutionContext* const executionContext);

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const executionContext) const final override;

	///  @name Common members

	virtual
	ComponentType
	getComponent () const
	throw (Error <DISPOSED>) final override
	{ return component; }

	virtual
	const std::string &
	getTypeName () const
	throw (Error <DISPOSED>) final override
	{ return typeName; }

	virtual
	const std::string &
	getContainerField () const
	throw (Error <DISPOSED>) final override
	{ return containerField; }

	///  @name Fields

	virtual
	SFVec3f &
	position ()
	{ return *fields .position; }

	virtual
	const SFVec3f &
	position () const
	{ return *fields .position; }

	virtual
	SFVec3f &
	centerOfRotation ()
	{ return *fields .centerOfRotation; }

	virtual
	const SFVec3f &
	centerOfRotation () const
	{ return *fields .centerOfRotation; }

	virtual
	MFFloat &
	fieldOfView ()
	{ return *fields .fieldOfView; }

	virtual
	const MFFloat &
	fieldOfView () const
	{ return *fields .fieldOfView; }

	///  @name Member access

	virtual
	void
	setPosition (const Vector3d & value) final override
	{ position () = value; }

	virtual
	Vector3d
	getPosition () const final override
	{ return position () .getValue (); }

	virtual
	void
	setCenterOfRotation (const Vector3d & value) final override
	{ centerOfRotation () = value; }

	virtual
	Vector3d
	getCenterOfRotation () const final override
	{ return centerOfRotation () .getValue (); }

	double
	getMinimumX () const;

	double
	getMinimumY () const;

	double
	getMaximumX () const;

	double
	getMaximumY () const;

	virtual
	double
	getMaxFarValue () const final override
	{ return 1e5; }

	virtual
	Vector3d
	getScreenScale (const Vector3d &, const Vector4i &) const override;

	virtual
	Vector2d
	getViewportSize (const Vector4i &) const;

	virtual
	Matrix4d
	getProjectionMatrix (const double nearValue, const double farValue, const Vector4i & viewport, const bool limit = false) const override;


private:

	///  @name Member access

	double
	getSizeX () const;

	double
	getSizeY () const;

	virtual
	std::pair <double, double>
	getLookAtDistance (const Box3d &) const final override;

	///  @name Static members

	static const ComponentType component;
	static const std::string   typeName;
	static const std::string   containerField;

	///  @name Members

	struct Fields
	{
		Fields ();

		SFVec3f* const position;
		SFVec3f* const centerOfRotation;
		MFFloat* const fieldOfView;
	};

	Fields fields;

};

} // X3D
} // titania

#endif
