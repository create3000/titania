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

#ifndef __TITANIA_X3D_TOOLS_SNAP_TOOL_X3DSNAP_TOOL_H__
#define __TITANIA_X3D_TOOLS_SNAP_TOOL_X3DSNAP_TOOL_H__

#include "../Layering/X3DActiveLayerTool.h"

#include "../../Browser/PointingDeviceSensor/Hit.h"

namespace titania {
namespace X3D {

class X3DSnapTool :
	public X3DActiveLayerTool,
	public sigc::trackable
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
	position ()
	{ return *fields .position; }

	const SFVec3f &
	position () const
	{ return *fields .position; }

	SFVec3f &
	normal ()
	{ return *fields .normal; }

	const SFVec3f &
	normal () const
	{ return *fields .normal; }

	///  @name Destruction

	virtual
	void
	dispose () override;

	virtual
	~X3DSnapTool () override;


protected:

	///  @name Construction

	X3DSnapTool ();

	virtual
	void
	initialize () override;

	virtual
	void
	realize () override;


protected:

	///  @name Events

	virtual
	bool
	on_button_press_event (GdkEventButton* event);


private:

	///  @name Events

	void
	set_enabled ();

	bool
	on_button_release_event (GdkEventButton* event);

	bool
	on_motion_notify_event (GdkEventMotion* event);

	bool
	touch (const double x, const double y) const;

	void
	update ();

	Vector3d
	snapToVerticesAndCenters (const Vector3d & hitPoint, const HitPtr & hit, const Matrix4d & invPickingMatrix) const;

	X3DPtr <X3DGridTool>
	getGrid (const HitPtr & hit) const;

	///  @name Fields

	struct Fields
	{
		Fields ();

		SFVec3f* const position;
		SFVec3f* const normal;
	};

	Fields fields;

	uint32_t         button;
	sigc::connection buttonPressConnection;
	sigc::connection buttonReleaseConnection;
	sigc::connection motionNotifyConnection;

};

} // X3D
} // titania

#endif
