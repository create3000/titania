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

#ifndef __TITANIA_X3D_TOOLS_NAVIGATION_VIEWPOINT_TOOL_H__
#define __TITANIA_X3D_TOOLS_NAVIGATION_VIEWPOINT_TOOL_H__

#include "../Navigation/X3DViewpointNodeTool.h"

#include "../../Components/Navigation/Viewpoint.h"

namespace titania {
namespace X3D {

class ViewpointTool :
	virtual public Viewpoint,
	public X3DViewpointNodeTool
{
public:

	///  @name Construction

	ViewpointTool (X3DBaseNode* const node);

	///  @name Fields

	virtual
	SFVec3f &
	position () final override
	{ return getNode <Viewpoint> () -> position (); }

	virtual
	const SFVec3f &
	position () const final override
	{ return getNode <Viewpoint> () -> position (); }

	virtual
	SFVec3f &
	centerOfRotation () final override
	{ return getNode <Viewpoint> () -> centerOfRotation (); }

	virtual
	const SFVec3f &
	centerOfRotation () const final override
	{ return getNode <Viewpoint> () -> centerOfRotation (); }

	virtual
	SFFloat &
	fieldOfView () final override
	{ return getNode <Viewpoint> () -> fieldOfView (); }

	virtual
	const SFFloat &
	fieldOfView () const final override
	{ return getNode <Viewpoint> () -> fieldOfView (); }

	///  @name Member access

	virtual
	Matrix4d
	getProjectionMatrix (const double nearValue, const double farValue, const Vector4i & viewport, const bool limit = false) const final override
	{ return getNode <Viewpoint> () -> getProjectionMatrix (nearValue, farValue, viewport, limit); }

	virtual
	Vector3d
	getScreenScale (const Vector3d & point, const Vector4i & viewport) const final override
	{ return getNode <Viewpoint> () -> getScreenScale (point, viewport); }

	virtual
	Vector2d
	getViewportSize (const Vector4i & viewport, const double nearValue) const final override
	{ return getNode <Viewpoint> () -> getViewportSize (viewport, nearValue); }

	///  @name Destruction

	virtual
	~ViewpointTool () final override;


private:

	virtual
	void
	realize () final override;

};

} // X3D
} // titania

#endif
