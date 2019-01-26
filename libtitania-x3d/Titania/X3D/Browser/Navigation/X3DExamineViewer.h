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

#ifndef __TITANIA_X3D_BROWSER_NAVIGATION_X3DEXAMINE_VIEWER_H__
#define __TITANIA_X3D_BROWSER_NAVIGATION_X3DEXAMINE_VIEWER_H__

#include <gdkmm.h>

#include "../../Components/Navigation/Viewpoint.h"
#include "../../Components/Navigation/X3DViewpointNode.h"
#include "../Navigation/X3DViewer.h"

namespace titania {
namespace X3D {

class X3DExamineViewer :
	public X3DViewer
{
public:

	virtual
	~X3DExamineViewer () = default;


protected:

	///  @name Construction

	X3DExamineViewer ();

	virtual
	void
	initialize () override;

	///  @name Event handler

	virtual
	bool
	on_1button1_press_event (GdkEventButton* event);

	virtual
	bool
	on_1button2_press_event (GdkEventButton* event);

	virtual
	bool
	on_1button1_release_event (GdkEventButton* event);

	virtual
	bool
	on_motion1_notify_event (GdkEventMotion* event);

	virtual
	bool
	on_scroll_event (GdkEventScroll* event);


private:

	///  @name Event handler

	void
	disconnect ();

	void
	set_activeViewpoint ();

	bool
	on_button_press_event (GdkEventButton* event);

	bool
	on_1button_press_event (GdkEventButton* event);

	bool
	on_2button_press_event (GdkEventButton* event);

	bool
	on_3button_press_event (GdkEventButton* event);

	bool
	on_button_release_event (GdkEventButton* event);

	bool
	on_1button_release_event (GdkEventButton* event);

	bool
	on_1button2_release_event (GdkEventButton* event);

	bool
	on_2button_release_event (GdkEventButton* event);

	bool
	on_3button_release_event (GdkEventButton* event);

	bool
	on_motion_notify_event (GdkEventMotion* event);

	bool
	on_motion2_notify_event (GdkEventMotion* event);

	///  @name Operations

	bool
	spin ();

	void
	addSpinning ();

	Vector3d
	getPositionOffset () const;

	Rotation4d
	getOrientationOffset (const Rotation4d & rotation, const bool _throw);

	Rotation4d
	getHorizonRotation (const Rotation4d & rotation) const;

	///  @name Static members

	static const Component   component;
	static const std::string typeName;
	static const std::string containerField;

	///  @name Members

	Rotation4d       orientationOffset;
	Rotation4d       rotation;
	Vector3d         fromVector;
	Vector3d         fromPoint;
	time_type        pressTime;
	time_type        motionTime;
	guint            button;
	size_t           numClicks;
	sigc::connection spin_id;

};

} // X3D
} // titania

#endif
