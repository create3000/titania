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

#ifndef __TMP_GLAD2CPP_POLYGON_COLORING_EDITOR_H__
#define __TMP_GLAD2CPP_POLYGON_COLORING_EDITOR_H__

#include "../Base/X3DEditorInterface.h"

#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

/**
 *  Gtk Interface for PolygonColoringEditor.
 */
class X3DPolygonColoringEditorInterface :
	public X3DEditorInterface
{
public:

	///  @name Construction

	X3DPolygonColoringEditorInterface () :
		X3DEditorInterface ()
	{ }

	template <class ... Arguments>
	X3DPolygonColoringEditorInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DEditorInterface (arguments ...)
	{ create (filename); }

	template <class ... Arguments>
	X3DPolygonColoringEditorInterface (std::initializer_list <std::string> filenames, const Arguments & ... arguments) :
		X3DEditorInterface (arguments ...)
	{ create (filenames); }

	///  @name Member access

	const Glib::RefPtr <Gtk::Builder> &
	getBuilder () const
	{ return m_builder; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getColorAdjustment () const
	{ return m_ColorAdjustment; }

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

	Gtk::ImageMenuItem &
	getRemoveUnusedColorsMenuItem () const
	{ return *m_RemoveUnusedColorsMenuItem; }

	Gtk::MenuItem &
	getRemoveDublicateColorsMenuItem () const
	{ return *m_RemoveDublicateColorsMenuItem; }

	Gtk::ToolButton &
	getUndoButton () const
	{ return *m_UndoButton; }

	Gtk::ToolButton &
	getRedoButton () const
	{ return *m_RedoButton; }

	Gtk::Box &
	getPreviewBox () const
	{ return *m_PreviewBox; }

	Gtk::RadioToolButton &
	getHandButton () const
	{ return *m_HandButton; }

	Gtk::RadioToolButton &
	getArrowButton () const
	{ return *m_ArrowButton; }

	Gtk::ToggleToolButton &
	getVisualizeGeometryButton () const
	{ return *m_VisualizeGeometryButton; }

	Gtk::ToolButton &
	getShadingButton () const
	{ return *m_ShadingButton; }

	Gtk::ToggleToolButton &
	getTextureButton () const
	{ return *m_TextureButton; }

	Gtk::ToggleToolButton &
	getCheckerBoardButton () const
	{ return *m_CheckerBoardButton; }

	Gtk::ToggleToolButton &
	getStraightenHorizonButton () const
	{ return *m_StraightenHorizonButton; }

	Gtk::ToolButton &
	getLookAtAllButton () const
	{ return *m_LookAtAllButton; }

	Gtk::ToggleToolButton &
	getLookAtButton () const
	{ return *m_LookAtButton; }

	Gtk::ToggleButton &
	getSelectColorButton () const
	{ return *m_SelectColorButton; }

	Gtk::Button &
	getColorButton () const
	{ return *m_ColorButton; }

	Gtk::Button &
	getAddColorButton () const
	{ return *m_AddColorButton; }

	Gtk::Button &
	getRemoveColorButton () const
	{ return *m_RemoveColorButton; }

	Gtk::RadioButton &
	getSingleVertexButton () const
	{ return *m_SingleVertexButton; }

	Gtk::RadioButton &
	getAdjacentVerticesButton () const
	{ return *m_AdjacentVerticesButton; }

	Gtk::RadioButton &
	getSingleFaceButton () const
	{ return *m_SingleFaceButton; }

	Gtk::RadioButton &
	getWholeObjectButton () const
	{ return *m_WholeObjectButton; }

	Gtk::ScrolledWindow &
	getColorsScrolledWindow () const
	{ return *m_ColorsScrolledWindow; }

	Gtk::Button &
	getRemoveButton () const
	{ return *m_RemoveButton; }

	Gtk::Button &
	getApplyButton () const
	{ return *m_ApplyButton; }

	Gtk::Popover &
	getShadingPopover () const
	{ return *m_ShadingPopover; }

	Gtk::RadioButton &
	getPhongButton () const
	{ return *m_PhongButton; }

	Gtk::RadioButton &
	getGouraudButton () const
	{ return *m_GouraudButton; }

	Gtk::RadioButton &
	getFlatButton () const
	{ return *m_FlatButton; }

	Gtk::RadioButton &
	getWireframeButton () const
	{ return *m_WireframeButton; }

	Gtk::RadioButton &
	getPointsetButton () const
	{ return *m_PointsetButton; }

	///  @name Signal handlers

	virtual
	void
	on_undo () = 0;

	virtual
	void
	on_redo () = 0;

	virtual
	void
	on_remove_unused_colors_activate () = 0;

	virtual
	void
	on_remove_dublicate_colors_activate () = 0;

	virtual
	void
	on_hand_toggled () = 0;

	virtual
	void
	on_arrow_toggled () = 0;

	virtual
	void
	on_visualize_geometry_toggled () = 0;

	virtual
	void
	on_shading_clicked () = 0;

	virtual
	void
	on_texture_toggled () = 0;

	virtual
	void
	on_checkerboard_toggled () = 0;

	virtual
	void
	on_straighten_horizon_toggled () = 0;

	virtual
	void
	on_look_at_all_clicked () = 0;

	virtual
	void
	on_look_at_toggled () = 0;

	virtual
	void
	on_single_vertex_clicked () = 0;

	virtual
	void
	on_adjacent_vertices_clicked () = 0;

	virtual
	void
	on_single_face_clicked () = 0;

	virtual
	void
	on_whole_object_clicked () = 0;

	virtual
	void
	on_remove_clicked () = 0;

	virtual
	void
	on_apply_clicked () = 0;

	virtual
	void
	on_phong_toggled () = 0;

	virtual
	void
	on_gouraud_toggled () = 0;

	virtual
	void
	on_flat_toggled () = 0;

	virtual
	void
	on_wireframe_toggled () = 0;

	virtual
	void
	on_pointset_toggled () = 0;

	///  @name Destruction

	virtual
	~X3DPolygonColoringEditorInterface () override;


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
	Glib::RefPtr <Gtk::Adjustment> m_ColorAdjustment;
	Gtk::Window* m_Window;
	Gtk::Box* m_Widget;
	Gtk::HeaderBar* m_HeaderBar;
	Gtk::ImageMenuItem* m_UndoMenuItem;
	Gtk::ImageMenuItem* m_RedoMenuItem;
	Gtk::ImageMenuItem* m_RemoveUnusedColorsMenuItem;
	Gtk::MenuItem* m_RemoveDublicateColorsMenuItem;
	Gtk::ToolButton* m_UndoButton;
	Gtk::ToolButton* m_RedoButton;
	Gtk::Box* m_PreviewBox;
	Gtk::RadioToolButton* m_HandButton;
	Gtk::RadioToolButton* m_ArrowButton;
	Gtk::ToggleToolButton* m_VisualizeGeometryButton;
	Gtk::ToolButton* m_ShadingButton;
	Gtk::ToggleToolButton* m_TextureButton;
	Gtk::ToggleToolButton* m_CheckerBoardButton;
	Gtk::ToggleToolButton* m_StraightenHorizonButton;
	Gtk::ToolButton* m_LookAtAllButton;
	Gtk::ToggleToolButton* m_LookAtButton;
	Gtk::ToggleButton* m_SelectColorButton;
	Gtk::Button* m_ColorButton;
	Gtk::Button* m_AddColorButton;
	Gtk::Button* m_RemoveColorButton;
	Gtk::RadioButton* m_SingleVertexButton;
	Gtk::RadioButton* m_AdjacentVerticesButton;
	Gtk::RadioButton* m_SingleFaceButton;
	Gtk::RadioButton* m_WholeObjectButton;
	Gtk::ScrolledWindow* m_ColorsScrolledWindow;
	Gtk::Button* m_RemoveButton;
	Gtk::Button* m_ApplyButton;
	Gtk::Popover* m_ShadingPopover;
	Gtk::RadioButton* m_PhongButton;
	Gtk::RadioButton* m_GouraudButton;
	Gtk::RadioButton* m_FlatButton;
	Gtk::RadioButton* m_WireframeButton;
	Gtk::RadioButton* m_PointsetButton;

};

} // puck
} // titania

#endif
