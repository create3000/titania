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

#ifndef __TMP_GLAD2CPP_GEOMETRY_EDITOR_H__
#define __TMP_GLAD2CPP_GEOMETRY_EDITOR_H__

#include "../Base/X3DRevealerInterface.h"

#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

/**
 *  Gtk Interface for GeometryEditor.
 */
class X3DGeometryEditorInterface :
	public X3DRevealerInterface
{
public:

	///  @name Construction

	X3DGeometryEditorInterface () :
		X3DRevealerInterface ()
	{ }

	template <class ... Arguments>
	X3DGeometryEditorInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DRevealerInterface (arguments ...)
	{ create (filename); }

	template <class ... Arguments>
	X3DGeometryEditorInterface (std::initializer_list <std::string> filenames, const Arguments & ... arguments) :
		X3DRevealerInterface (arguments ...)
	{ create (filenames); }

	///  @name Member access

	const Glib::RefPtr <Gtk::Builder> &
	getBuilder () const
	{ return m_builder; }

	Gtk::Menu &
	getCutPolygonsMenu () const
	{ return *m_CutPolygonsMenu; }

	Gtk::CheckMenuItem &
	getCutPolygonsEnableSnappingMenuItem () const
	{ return *m_CutPolygonsEnableSnappingMenuItem; }

	Gtk::Menu &
	getTransformToolMenu () const
	{ return *m_TransformToolMenu; }

	Gtk::CheckMenuItem &
	getAxisAlignedBoundingBoxMenuItem () const
	{ return *m_AxisAlignedBoundingBoxMenuItem; }

	Gtk::Window &
	getWindow () const final override
	{ return *m_Window; }

	Gtk::Revealer &
	getWidget () const final override
	{ return *m_Widget; }

	Gtk::Box &
	getGeometryEditorBox () const
	{ return *m_GeometryEditorBox; }

	Gtk::ToggleButton &
	getEditToggleButton () const
	{ return *m_EditToggleButton; }

	Gtk::Button &
	getHammerButton () const
	{ return *m_HammerButton; }

	Gtk::Button &
	getTitleButton () const
	{ return *m_TitleButton; }

	Gtk::Grid &
	getGeometryToolsBox () const
	{ return *m_GeometryToolsBox; }

	Gtk::ToggleButton &
	getNormalEnabledButton () const
	{ return *m_NormalEnabledButton; }

	Gtk::Button &
	getFormNewFaceButton () const
	{ return *m_FormNewFaceButton; }

	Gtk::Button &
	getExtrudeSelectedEdgesButton () const
	{ return *m_ExtrudeSelectedEdgesButton; }

	Gtk::Button &
	getExtrudeSelectedFacesButton () const
	{ return *m_ExtrudeSelectedFacesButton; }

	Gtk::Button &
	getChipOfFacesButton () const
	{ return *m_ChipOfFacesButton; }

	Gtk::Button &
	getDeleteFacesButton () const
	{ return *m_DeleteFacesButton; }

	Gtk::Button &
	getFlipVertexOrderingButton () const
	{ return *m_FlipVertexOrderingButton; }

	Gtk::RadioButton &
	getPaintSelectionButton () const
	{ return *m_PaintSelectionButton; }

	Gtk::Image &
	getPaintSelectionImage () const
	{ return *m_PaintSelectionImage; }

	Gtk::Button &
	getSplitPointsButton () const
	{ return *m_SplitPointsButton; }

	Gtk::Button &
	getMergePointsButton () const
	{ return *m_MergePointsButton; }

	Gtk::ToggleButton &
	getTransformToolButton () const
	{ return *m_TransformToolButton; }

	Gtk::RadioButton &
	getCutPolygonsButton () const
	{ return *m_CutPolygonsButton; }

	Gtk::Menu &
	getSelectionTypeMenu () const
	{ return *m_SelectionTypeMenu; }

	Gtk::ImageMenuItem &
	getBrushMenuItem () const
	{ return *m_BrushMenuItem; }

	Gtk::ImageMenuItem &
	getRectangleMenuItem () const
	{ return *m_RectangleMenuItem; }

	Gtk::ImageMenuItem &
	getLassoMenuItem () const
	{ return *m_LassoMenuItem; }

	Gtk::RadioMenuItem &
	getPointsMenuItem () const
	{ return *m_PointsMenuItem; }

	Gtk::RadioMenuItem &
	getEdgesMenuItem () const
	{ return *m_EdgesMenuItem; }

	Gtk::RadioMenuItem &
	getFacesMenuItem () const
	{ return *m_FacesMenuItem; }

	Gtk::CheckMenuItem &
	getSelectLineLoopMenuItem () const
	{ return *m_SelectLineLoopMenuItem; }

	///  @name Signal handlers

	virtual
	void
	on_cut_polygons_enable_snapping_toggled () = 0;

	virtual
	void
	on_axis_aligned_bounding_box_toggled () = 0;

	virtual
	void
	on_map () = 0;

	virtual
	void
	on_unmap () = 0;

	virtual
	void
	on_edit_toggled () = 0;

	virtual
	void
	on_hammer_clicked () = 0;

	virtual
	void
	on_form_new_face_clicked () = 0;

	virtual
	void
	on_extrude_selected_edges_clicked () = 0;

	virtual
	void
	on_extrude_selected_faces_clicked () = 0;

	virtual
	void
	on_chip_of_face_clicked () = 0;

	virtual
	void
	on_delete_selected_faces_clicked () = 0;

	virtual
	void
	on_flip_vertex_ordering_clicked () = 0;

	virtual
	bool
	on_selection_type_button_press_event (GdkEventButton* button_event) = 0;

	virtual
	void
	on_paint_selection_toggled () = 0;

	virtual
	void
	on_split_points_clicked () = 0;

	virtual
	void
	on_merge_points_clicked () = 0;

	virtual
	bool
	on_transform_tool_button_press_event (GdkEventButton* button_event) = 0;

	virtual
	void
	on_transform_tool_toggled () = 0;

	virtual
	bool
	on_cut_polygons_button_press_event (GdkEventButton* button_event) = 0;

	virtual
	void
	on_cut_polygons_toggled () = 0;

	virtual
	void
	on_brush_activated () = 0;

	virtual
	void
	on_rectangle_activated () = 0;

	virtual
	void
	on_lasso_activated () = 0;

	virtual
	void
	on_points_toggled () = 0;

	virtual
	void
	on_edges_toggled () = 0;

	virtual
	void
	on_faces_toggled () = 0;

	virtual
	void
	on_select_line_loop_toggled () = 0;

	///  @name Destruction

	virtual
	~X3DGeometryEditorInterface () override;


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
	Gtk::Menu* m_CutPolygonsMenu;
	Gtk::CheckMenuItem* m_CutPolygonsEnableSnappingMenuItem;
	Gtk::Menu* m_TransformToolMenu;
	Gtk::CheckMenuItem* m_AxisAlignedBoundingBoxMenuItem;
	Gtk::Window* m_Window;
	Gtk::Revealer* m_Widget;
	Gtk::Box* m_GeometryEditorBox;
	Gtk::ToggleButton* m_EditToggleButton;
	Gtk::Button* m_HammerButton;
	Gtk::Button* m_TitleButton;
	Gtk::Grid* m_GeometryToolsBox;
	Gtk::ToggleButton* m_NormalEnabledButton;
	Gtk::Button* m_FormNewFaceButton;
	Gtk::Button* m_ExtrudeSelectedEdgesButton;
	Gtk::Button* m_ExtrudeSelectedFacesButton;
	Gtk::Button* m_ChipOfFacesButton;
	Gtk::Button* m_DeleteFacesButton;
	Gtk::Button* m_FlipVertexOrderingButton;
	Gtk::RadioButton* m_PaintSelectionButton;
	Gtk::Image* m_PaintSelectionImage;
	Gtk::Button* m_SplitPointsButton;
	Gtk::Button* m_MergePointsButton;
	Gtk::ToggleButton* m_TransformToolButton;
	Gtk::RadioButton* m_CutPolygonsButton;
	Gtk::Menu* m_SelectionTypeMenu;
	Gtk::ImageMenuItem* m_BrushMenuItem;
	Gtk::ImageMenuItem* m_RectangleMenuItem;
	Gtk::ImageMenuItem* m_LassoMenuItem;
	Gtk::RadioMenuItem* m_PointsMenuItem;
	Gtk::RadioMenuItem* m_EdgesMenuItem;
	Gtk::RadioMenuItem* m_FacesMenuItem;
	Gtk::CheckMenuItem* m_SelectLineLoopMenuItem;

};

} // puck
} // titania

#endif
