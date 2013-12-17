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

#ifndef __TITANIA_X3D_BROWSER_VIEWER_X3DFLY_VIEWER_H__
#define __TITANIA_X3D_BROWSER_VIEWER_X3DFLY_VIEWER_H__

#include <gdkmm.h>

#include "../../Components/Navigation/Viewpoint.h"
#include "../../Components/Navigation/X3DViewpointNode.h"
#include "../../Fields/SFNode.h"
#include "../../Miscellaneous/Keys.h"
#include "../Viewer/X3DViewer.h"

namespace titania {
namespace X3D {

class X3DFlyViewer :
	public X3DViewer
{
public:

	X3DFlyViewer (X3DBrowserSurface* const, NavigationInfo* const);

	virtual
	NavigationInfo*
	getNavigationInfo () const final override
	{ return navigationInfo; }


protected:

	static Vector3f upVector;

	virtual
	Vector3f
	getTranslationOffset (const Vector3f &) = 0;


private:

	virtual
	void
	initialize () override;

	void
	set_collisionNormal ();

	bool
	on_button_press_event (GdkEventButton*);

	bool
	on_button_release_event (GdkEventButton*);

	bool
	on_motion_notify_event (GdkEventMotion*);

	bool
	on_scroll_event (GdkEventScroll*);

	bool
	on_key_press_event (GdkEventKey*);

	bool
	on_key_release_event (GdkEventKey*);

	bool
	fly ();

	bool
	pan ();

	bool
	roll ();

	Vector3f
	getTranslation (const Vector3f &) const;

	void
	addFly ();

	void
	addPan ();

	void
	addRoll ();

	void
	disconnect ();

	void
	display ();

	NavigationInfo* const navigationInfo;

	Rotation4f       orientation;
	Vector3f         fromVector;
	Vector3f         toVector;
	Vector3f         direction;
	Rotation4f       sourceRotation;
	Rotation4f       destinationRotation;
	time_type        startTime;
	guint            button;
	Keys             keys;
	sigc::connection fly_id;
	sigc::connection pan_id;
	sigc::connection roll_id;

};

} // X3D
} // titania

#endif
