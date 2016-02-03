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

#ifndef __TITANIA_X3D_BROWSER_NAVIGATION_X3DVIEWER_H__
#define __TITANIA_X3D_BROWSER_NAVIGATION_X3DVIEWER_H__

#include "../X3DBrowserObject.h"

#include "../../Components/Navigation/NavigationInfo.h"
#include "../../Components/Navigation/X3DViewpointNode.h"

namespace titania {
namespace X3D {

class X3DViewer :
	public X3DBrowserObject
{
public:

	///  @name Constructors

	X3DViewer ();

	///  @name Fields

	virtual
	SFBool &
	isActive ()
	{ return *fields .isActive; }

	virtual
	const SFBool &
	isActive () const
	{ return *fields .isActive; }

	virtual
	SFTime &
	scrollTime ()
	{ return *fields .scrollTime; }

	virtual
	const SFTime &
	scrollTime () const
	{ return *fields .scrollTime; }


protected:

	///  @name Member access

	NavigationInfo*
	getNavigationInfo () const;

	const X3DLayerNodePtr &
	getActiveLayer () const;

	X3DViewpointNode*
	getActiveViewpoint () const;

	///  @name Operations

	Vector3f
	getPointOnCenterPlane (const double, const double);

	Vector3f
	getDistanceToCenter () const;

	Vector3f
	trackballProjectToSphere (double, double) const;


private:

	///  @name Operations

	static
	double
	tb_project_to_sphere (const double, const double, const double);

	///  @name Members

	struct Fields
	{
		Fields ();

		SFBool* const isActive;
		SFTime* const scrollTime;
	};

	Fields fields;

};

} // X3D
} // titania

#endif
