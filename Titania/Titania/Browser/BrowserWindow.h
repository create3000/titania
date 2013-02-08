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

#ifndef __TITANIA_BROWSER_BROWSER_WINDOW_H__
#define __TITANIA_BROWSER_BROWSER_WINDOW_H__

#include "../Browser/X3DBrowserWindow.h"
#include "../MotionBlurEditor/MotionBlurEditor.h"

namespace titania {
namespace puck {

class BrowserWindow :
	public X3DBrowserWindow
{
public:

	BrowserWindow (int &, char** &);


private:

	void
	initialize ();

	/// @name Element access

	MotionBlurEditor &
	getMotionBlurEditor () { return motionBlurEditor; }

	const MotionBlurEditor &
	getMotionBlurEditor () const { return motionBlurEditor; }

	/// @name File menu

	virtual
	void
	on_new ();

	virtual
	void
	on_home ();

	virtual
	void
	on_open ();

	virtual
	void
	on_save ();

	virtual
	void
	on_save_as ();

	virtual
	void
	on_revert_to_saved ();

	virtual
	void
	on_reload ();

	virtual
	void
	on_close ();

	/// @name File open dialog response

	virtual
	void
	on_fileOpenDialog_response (int);

	/// @name File save dialog response

	virtual
	void
	on_fileSaveDialog_response (int);

	/// @name Bar view handling

	virtual
	void
	on_navigationBar_toggled ();

	virtual
	void
	on_toolBar_toggled ();

	virtual
	void
	on_sideBar_toggled ();

	virtual
	void
	on_footer_toggled ();

	virtual
	void
	on_statusBar_toggled ();

	/// @name Shading

	virtual
	void
	phong_activate ();

	virtual
	void
	gouraud_activate ();

	virtual
	void
	flat_activate ();

	virtual
	void
	wireframe_activate ();

	virtual
	void
	pointset_activate ();

	///  @name Rendering quality

	virtual
	void
	on_low_quality_activate ();

	virtual
	void
	on_medium_quality_activate ();

	virtual
	void
	on_high_quality_activate ();

	///  @name Rendering properties

	virtual
	void
	on_rendering_properties_toggled ();

	///  @name Fullscreen modus

	virtual
	void
	on_fullscreen_toggled ();

	///  @name Navigation

	virtual
	void
	on_headlight_toggled ();

	virtual
	void
	on_rubberband_toggled ();

	virtual
	void
	on_look_at_all_activate ();

	void
	on_enableInlineViewpoints_toggled ();

	///  @name Editor handling

	virtual
	void
	on_outline_editor_activate ();

	virtual
	void
	on_viewpoint_editor_activate ();

	virtual
	void
	on_motion_blur_editor_activate ();

	///  @name Help menu

	virtual
	void
	on_info ();

	virtual
	void
	on_standard_size ();

	///  @name Notebook handling

	virtual
	void
	on_switch_page (Gtk::Widget *, guint);

	virtual
	void
	on_add_tab ();

	virtual
	void
	on_close_tab (Gtk::Widget &);

	size_t           currentPage;
	MotionBlurEditor motionBlurEditor;

};

} // puck
} // titania

#endif
