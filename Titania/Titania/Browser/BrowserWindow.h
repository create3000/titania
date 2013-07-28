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
#include "../HistoryEditor/HistoryEditor.h"
#include "../MotionBlurEditor/MotionBlurEditor.h"
#include "../OutlineEditor/OutlineEditor.h"
#include "../ViewpointEditor/ViewpointEditor.h"

namespace titania {
namespace puck {

class BrowserWindow :
	public X3DBrowserWindow
{
public:

	BrowserWindow (const basic::uri &);
	
	const X3D::Keys &
	getKeys () const
	{ return keys; }


private:

	void
	initialize ();

	/// @name Widgets

	MotionBlurEditor &
	getMotionBlurEditor ()
	{ return motionBlurEditor; }

	const MotionBlurEditor &
	getMotionBlurEditor () const
	{ return motionBlurEditor; }

	ViewpointEditor &
	getViewpointEditor ()
	{ return viewpointEditor; }

	const ViewpointEditor &
	getViewpointEditor () const
	{ return viewpointEditor; }

	HistoryEditor &
	getHistoryEditor ()
	{ return historyEditor; }

	const HistoryEditor &
	getHistoryEditor () const
	{ return historyEditor; }

	OutlineEditor &
	getOutlineEditor ()
	{ return outlineEditor; }

	const OutlineEditor &
	getOutlineEditor () const
	{ return outlineEditor; }

	/// @name File menu

	virtual
	void
	on_new () final;

	virtual
	void
	on_open () final;
	
	virtual
	void
	on_open_location_dialog () final;

	virtual
	void
	on_save () final;

	virtual
	void
	on_save_as () final;

	virtual
	void
	on_revert_to_saved () final;

	virtual
	void
	on_close () final;

	/// @name Dialog response

	virtual
	void
	on_fileOpenDialog_response (int) final;

	virtual
	bool
	on_openLocationEntry_key_release_event (GdkEventKey*) final;

	virtual
	void
	on_openLocationDialog_response (int) final;

	virtual
	void
	on_fileSaveDialog_response (int) final;

	/// @name Bar view handling

	virtual
	void
	on_toolBar_toggled () final;

	virtual
	void
	on_sideBar_toggled () final;

	virtual
	void
	on_footer_toggled () final;

	/// @name Shading

	virtual
	void
	phong_activate () final;

	virtual
	void
	gouraud_activate () final;

	virtual
	void
	flat_activate () final;

	virtual
	void
	wireframe_activate () final;

	virtual
	void
	pointset_activate () final;

	///  @name Rendering quality

	virtual
	void
	on_low_quality_activate () final;

	virtual
	void
	on_medium_quality_activate () final;

	virtual
	void
	on_high_quality_activate () final;

	///  @name Rendering properties

	virtual
	void
	on_rendering_properties_toggled () final;

	///  @name Fullscreen modus

	virtual
	void
	on_fullscreen_toggled () final;

	///  @name Navigation

	virtual
	void
	on_headlight_toggled () final;

	virtual
	void
	on_rubberband_toggled () final;

	virtual
	void
	on_look_at_all_activate () final;

	void
	on_enableInlineViewpoints_toggled () final;

	///  @name Editor handling

	virtual
	void
	on_motion_blur_editor_activate () final;

	///  @name Help menu

	virtual
	void
	on_info () final;

	virtual
	void
	on_standard_size () final;

	///  @name Browser toolbar handling

	virtual
	void
	on_hand_button_toggled () final;

	virtual
	void
	on_arrow_button_toggled () final;

	virtual
	void
	on_look_at_all_clicked () final;

	virtual
	void
	on_look_at_toggled () final;

	///  @name Dialog response handling

	void
	on_messageDialog_response (int);

	///  @name Key events

	virtual
	bool
	on_key_press_event (GdkEventKey*) final;

	virtual
	bool
	on_key_release_event (GdkEventKey*) final;

	virtual
	void
	on_drag_data_received (const Glib::RefPtr <Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint info, guint) final;

	///  @name Properties

	MotionBlurEditor motionBlurEditor;
	ViewpointEditor  viewpointEditor;
	HistoryEditor    historyEditor;
	OutlineEditor    outlineEditor;
	X3D::Keys        keys;

};

} // puck
} // titania

#endif
