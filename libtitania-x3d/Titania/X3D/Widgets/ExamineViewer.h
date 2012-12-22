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
 ******************************************************************************/

#ifndef __TITANIA_X3D_WIDGETS_EXAMINE_VIEWER_H__
#define __TITANIA_X3D_WIDGETS_EXAMINE_VIEWER_H__

#include "../Components/Navigation/Viewpoint.h"
#include "../Components/Navigation/X3DViewpointNode.h"
#include "../Fields/SFNode.h"
#include "../Widgets/X3DViewer.h"

#include <gdkmm.h>

namespace titania {
namespace X3D {

class Surface;
class Scene;

class ExamineViewer :
	public X3DViewer
{
public:

	ExamineViewer (Surface &);

	void
	initialize ();

	virtual
	~ExamineViewer ();


private:

	void
	set_scene ();

	void
	set_activeLayer ();

	void
	set_viewpoint ();

	bool
	on_button_press_event (GdkEventButton*);

	bool
	on_motion_notify_event (GdkEventMotion*);

	bool
	on_button_release_event (GdkEventButton*);

	bool
	on_scroll_event (GdkEventScroll*);

	bool
	spin ();

	void
	addSpinning ();
	
	Vector3f
	getDistance (const SFNode <Viewpoint> &) const;

	Rotation4f
	getOrientation (const SFNode <Viewpoint> &) const;

	Vector3f
	getTranslation (const SFNode <Viewpoint> &) const;
	
	Rotation4f
	getRotation (const SFNode <Viewpoint> &);
	
	Vector3f
	getPoint (double x, double y);

	Vector3f
	trackballProjectToSphere (double, double) const;

	float
	tb_project_to_sphere (const float, const float, const float) const;

	Surface &             surface;
	Vector3f              distance;
	Rotation4f            orientation;
	Rotation4f            rotation;
	Vector3f              fromVector;
	Vector3f              fromPoint;
	guint                 button;
	sigc::connection      button_press_event_connection;
	sigc::connection      motion_notify_event_connection;
	sigc::connection      button_release_event_connection;
	sigc::connection      scroll_event_connection;
	sigc::connection      spin_id;
	SFNode <X3DLayerNode> activeLayer;

};

} // X3D
} // titania

#endif
