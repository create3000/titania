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

#ifndef __TMP_GLAD2CPP_TEXTURE_MAPPING_EDITOR_H__
#define __TMP_GLAD2CPP_TEXTURE_MAPPING_EDITOR_H__

#include "../Base/X3DEditorInterface.h"

#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

/**
 *  Gtk Interface for TextureMappingEditor.
 */
class X3DTextureMappingEditorInterface :
	public X3DEditorInterface
{
public:

	///  @name Construction

	X3DTextureMappingEditorInterface () :
		X3DEditorInterface ()
	{ }

	template <class ... Arguments>
	X3DTextureMappingEditorInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DEditorInterface (arguments ...)
	{ create (filename); }

	template <class ... Arguments>
	X3DTextureMappingEditorInterface (std::initializer_list <std::string> filenames, const Arguments & ... arguments) :
		X3DEditorInterface (arguments ...)
	{ create (filenames); }

	///  @name Member access

	const Glib::RefPtr <Gtk::Builder> &
	getBuilder () const
	{ return m_builder; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTextureStageAdjustment () const
	{ return m_TextureStageAdjustment; }

	Gtk::Window &
	getWindow () const final override
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const final override
	{ return *m_Widget; }

	Gtk::HeaderBar &
	getHeaderBar () const
	{ return *m_HeaderBar; }

	Gtk::ImageMenuItem &
	getUndoMenuItem () const
	{ return *m_UndoMenuItem; }

	Gtk::ImageMenuItem &
	getRedoMenuItem () const
	{ return *m_RedoMenuItem; }

	Gtk::MenuItem &
	getSelectionMenuItem () const
	{ return *m_SelectionMenuItem; }

	Gtk::ImageMenuItem &
	getSelectAllMenuItem () const
	{ return *m_SelectAllMenuItem; }

	Gtk::MenuItem &
	getDeselectAllMenuItem () const
	{ return *m_DeselectAllMenuItem; }

	Gtk::MenuItem &
	getMappingsMenuItem () const
	{ return *m_MappingsMenuItem; }

	Gtk::MenuItem &
	getXPlaneMenuItem () const
	{ return *m_XPlaneMenuItem; }

	Gtk::MenuItem &
	getYPlaneMenuItem () const
	{ return *m_YPlaneMenuItem; }

	Gtk::MenuItem &
	getZPlaneMenuItem () const
	{ return *m_ZPlaneMenuItem; }

	Gtk::MenuItem &
	getCameraMenuItem () const
	{ return *m_CameraMenuItem; }

	Gtk::MenuItem &
	getBoxMenuItem () const
	{ return *m_BoxMenuItem; }

	Gtk::MenuItem &
	getCylinderXMenuItem () const
	{ return *m_CylinderXMenuItem; }

	Gtk::MenuItem &
	getCylinderYMenuItem () const
	{ return *m_CylinderYMenuItem; }

	Gtk::MenuItem &
	getCylinderZMenuItem () const
	{ return *m_CylinderZMenuItem; }

	Gtk::MenuItem &
	getSphereXMenuItem () const
	{ return *m_SphereXMenuItem; }

	Gtk::MenuItem &
	getSphereYMenuItem () const
	{ return *m_SphereYMenuItem; }

	Gtk::MenuItem &
	getSphereZMenuItem () const
	{ return *m_SphereZMenuItem; }

	Gtk::ToolButton &
	getUndoButton () const
	{ return *m_UndoButton; }

	Gtk::ToolButton &
	getRedoButton () const
	{ return *m_RedoButton; }

	Gtk::ToolButton &
	getRotateCounterlockwiseButton () const
	{ return *m_RotateCounterlockwiseButton; }

	Gtk::ToolButton &
	getRotateClockwiseButton () const
	{ return *m_RotateClockwiseButton; }

	Gtk::ToolButton &
	getFlipButton () const
	{ return *m_FlipButton; }

	Gtk::ToolButton &
	getFlopButton () const
	{ return *m_FlopButton; }

	Gtk::ToolButton &
	getMergePointsButton () const
	{ return *m_MergePointsButton; }

	Gtk::ToolButton &
	getSplitPointButton () const
	{ return *m_SplitPointButton; }

	Gtk::Box &
	getLeftBox () const
	{ return *m_LeftBox; }

	Gtk::RadioToolButton &
	getLeftHandButton () const
	{ return *m_LeftHandButton; }

	Gtk::RadioToolButton &
	getLeftArrowButton () const
	{ return *m_LeftArrowButton; }

	Gtk::RadioToolButton &
	getRotateButton () const
	{ return *m_RotateButton; }

	Gtk::RadioToolButton &
	getScaleButton () const
	{ return *m_ScaleButton; }

	Gtk::ToggleToolButton &
	getSnapCenterButton () const
	{ return *m_SnapCenterButton; }

	Gtk::ToolButton &
	getLeftLookAtButton () const
	{ return *m_LeftLookAtButton; }

	Gtk::Box &
	getRightBox () const
	{ return *m_RightBox; }

	Gtk::RadioToolButton &
	getRightHandButton () const
	{ return *m_RightHandButton; }

	Gtk::RadioToolButton &
	getRightArrowButton () const
	{ return *m_RightArrowButton; }

	Gtk::ToggleToolButton &
	getRightStraightenHorizonButton () const
	{ return *m_RightStraightenHorizonButton; }

	Gtk::ToolButton &
	getRightLookAtAllButton () const
	{ return *m_RightLookAtAllButton; }

	Gtk::ToggleToolButton &
	getRightLookAtButton () const
	{ return *m_RightLookAtButton; }

	Gtk::Button &
	getRemoveButton () const
	{ return *m_RemoveButton; }

	Gtk::Button &
	getApplyButton () const
	{ return *m_ApplyButton; }

	///  @name Signal handlers

	virtual
	void
	on_texture_stage_changed () = 0;

	virtual
	bool
	on_key_press_event (GdkEventKey* key_event) = 0;

	virtual
	bool
	on_key_release_event (GdkEventKey* key_event) = 0;

	virtual
	void
	on_undo () = 0;

	virtual
	void
	on_redo () = 0;

	virtual
	void
	on_select_all_activate () = 0;

	virtual
	void
	on_deselect_all_activate () = 0;

	virtual
	void
	on_x_plane_activate () = 0;

	virtual
	void
	on_y_plane_activate () = 0;

	virtual
	void
	on_z_plane_activate () = 0;

	virtual
	void
	on_camera_activate () = 0;

	virtual
	void
	on_box_activate () = 0;

	virtual
	void
	on_cylinder_x_activate () = 0;

	virtual
	void
	on_cylinder_y_activate () = 0;

	virtual
	void
	on_cylinder_z_activate () = 0;

	virtual
	void
	on_sphere_x_activate () = 0;

	virtual
	void
	on_sphere_y_activate () = 0;

	virtual
	void
	on_sphere_z_activate () = 0;

	virtual
	void
	on_rotate_counterclockwise () = 0;

	virtual
	void
	on_rotate_clockwise () = 0;

	virtual
	void
	on_flip () = 0;

	virtual
	void
	on_flop () = 0;

	virtual
	void
	on_merge_points () = 0;

	virtual
	void
	on_split_point () = 0;

	virtual
	void
	on_left_hand_toggled () = 0;

	virtual
	void
	on_left_arrow_toggled () = 0;

	virtual
	void
	on_left_rotate_toggled () = 0;

	virtual
	void
	on_left_scale_toggled () = 0;

	virtual
	void
	on_left_snap_center_toggled () = 0;

	virtual
	void
	on_left_look_at_all_clicked () = 0;

	virtual
	void
	on_right_hand_toggled () = 0;

	virtual
	void
	on_right_arrow_toggled () = 0;

	virtual
	void
	on_right_straighten_horizon_toggled () = 0;

	virtual
	void
	on_right_look_at_all_clicked () = 0;

	virtual
	void
	on_right_look_at_toggled () = 0;

	virtual
	void
	on_remove_clicked () = 0;

	virtual
	void
	on_apply_clicked () = 0;

	///  @name Destruction

	virtual
	~X3DTextureMappingEditorInterface () override;


private:

	///  @name Construction

	void
	create (const std::string &);

	void
	create (std::initializer_list <std::string>);

	void
	create ();

	///  @name Static members

	///  @name Members

	Glib::RefPtr <Gtk::Builder> m_builder;
	Glib::RefPtr <Gtk::Adjustment> m_TextureStageAdjustment;
	Gtk::Window* m_Window;
	Gtk::Box* m_Widget;
	Gtk::HeaderBar* m_HeaderBar;
	Gtk::ImageMenuItem* m_UndoMenuItem;
	Gtk::ImageMenuItem* m_RedoMenuItem;
	Gtk::MenuItem* m_SelectionMenuItem;
	Gtk::ImageMenuItem* m_SelectAllMenuItem;
	Gtk::MenuItem* m_DeselectAllMenuItem;
	Gtk::MenuItem* m_MappingsMenuItem;
	Gtk::MenuItem* m_XPlaneMenuItem;
	Gtk::MenuItem* m_YPlaneMenuItem;
	Gtk::MenuItem* m_ZPlaneMenuItem;
	Gtk::MenuItem* m_CameraMenuItem;
	Gtk::MenuItem* m_BoxMenuItem;
	Gtk::MenuItem* m_CylinderXMenuItem;
	Gtk::MenuItem* m_CylinderYMenuItem;
	Gtk::MenuItem* m_CylinderZMenuItem;
	Gtk::MenuItem* m_SphereXMenuItem;
	Gtk::MenuItem* m_SphereYMenuItem;
	Gtk::MenuItem* m_SphereZMenuItem;
	Gtk::ToolButton* m_UndoButton;
	Gtk::ToolButton* m_RedoButton;
	Gtk::ToolButton* m_RotateCounterlockwiseButton;
	Gtk::ToolButton* m_RotateClockwiseButton;
	Gtk::ToolButton* m_FlipButton;
	Gtk::ToolButton* m_FlopButton;
	Gtk::ToolButton* m_MergePointsButton;
	Gtk::ToolButton* m_SplitPointButton;
	Gtk::Box* m_LeftBox;
	Gtk::RadioToolButton* m_LeftHandButton;
	Gtk::RadioToolButton* m_LeftArrowButton;
	Gtk::RadioToolButton* m_RotateButton;
	Gtk::RadioToolButton* m_ScaleButton;
	Gtk::ToggleToolButton* m_SnapCenterButton;
	Gtk::ToolButton* m_LeftLookAtButton;
	Gtk::Box* m_RightBox;
	Gtk::RadioToolButton* m_RightHandButton;
	Gtk::RadioToolButton* m_RightArrowButton;
	Gtk::ToggleToolButton* m_RightStraightenHorizonButton;
	Gtk::ToolButton* m_RightLookAtAllButton;
	Gtk::ToggleToolButton* m_RightLookAtButton;
	Gtk::Button* m_RemoveButton;
	Gtk::Button* m_ApplyButton;

};

} // puck
} // titania

#endif
