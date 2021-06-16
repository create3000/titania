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

#ifndef __TMP_GLAD2CPP_SCULP_TOOL_EDITOR_H__
#define __TMP_GLAD2CPP_SCULP_TOOL_EDITOR_H__

#include "../Base/X3DEditorInterface.h"

#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

/**
 *  Gtk Interface for SculpToolEditor.
 */
class X3DSculpToolEditorInterface :
	public X3DEditorInterface
{
public:

	///  @name Construction

	X3DSculpToolEditorInterface () :
		X3DEditorInterface ()
	{ }

	template <class ... Arguments>
	X3DSculpToolEditorInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DEditorInterface (arguments ...)
	{ create (filename); }

	template <class ... Arguments>
	X3DSculpToolEditorInterface (std::initializer_list <std::string> filenames, const Arguments & ... arguments) :
		X3DEditorInterface (arguments ...)
	{ create (filenames); }

	///  @name Member access

	const Glib::RefPtr <Gtk::Builder> &
	getBuilder () const
	{ return m_builder; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getBrushHardnessAdjustment () const
	{ return m_BrushHardnessAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getBrushHeightAdjustment () const
	{ return m_BrushHeightAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getBrushPressureAdjustment () const
	{ return m_BrushPressureAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getBrushRadiusAdjustment () const
	{ return m_BrushRadiusAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getBrushScaleAdjustment () const
	{ return m_BrushScaleAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getBrushSharpnessAdjustment () const
	{ return m_BrushSharpnessAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getBrushSpacingAdjustment () const
	{ return m_BrushSpacingAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getBrushWarpAdjustment () const
	{ return m_BrushWarpAdjustment; }

	Gtk::Window &
	getWindow () const final override
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const final override
	{ return *m_Widget; }

	Gtk::Paned &
	getPaned () const
	{ return *m_Paned; }

	Gtk::Box &
	getPreviewBox () const
	{ return *m_PreviewBox; }

	Gtk::HeaderBar &
	getHeaderBar () const
	{ return *m_HeaderBar; }

	Gtk::Toolbar &
	getToolbar () const
	{ return *m_Toolbar; }

	Gtk::RadioToolButton &
	getPullPolygonsButton () const
	{ return *m_PullPolygonsButton; }

	Gtk::RadioToolButton &
	getPushPolygonsButton () const
	{ return *m_PushPolygonsButton; }

	Gtk::RadioToolButton &
	getRoughtenPolygonsButton () const
	{ return *m_RoughtenPolygonsButton; }

	Gtk::RadioToolButton &
	getSmoothPolygonsButton () const
	{ return *m_SmoothPolygonsButton; }

	Gtk::RadioToolButton &
	getUndoBrushButton () const
	{ return *m_UndoBrushButton; }

	Gtk::Notebook &
	getNotebook () const
	{ return *m_Notebook; }

	Gtk::Expander &
	getBrushExpander () const
	{ return *m_BrushExpander; }

	Gtk::Grid &
	getBrushBox () const
	{ return *m_BrushBox; }

	Gtk::Scale &
	getBrushWarpScale () const
	{ return *m_BrushWarpScale; }

	Gtk::Scale &
	getBrushSharpnessScale () const
	{ return *m_BrushSharpnessScale; }

	Gtk::Scale &
	getBrushHardnessScale () const
	{ return *m_BrushHardnessScale; }

	Gtk::ComboBoxText &
	getBrushTypeButton () const
	{ return *m_BrushTypeButton; }

	Gtk::SpinButton &
	getBrushSpacingButton () const
	{ return *m_BrushSpacingButton; }

	Gtk::SpinButton &
	getBrushHeightButton () const
	{ return *m_BrushHeightButton; }

	Gtk::Scale &
	getBrushPressureScale () const
	{ return *m_BrushPressureScale; }

	Gtk::SpinButton &
	getBrushRadiusButton () const
	{ return *m_BrushRadiusButton; }

	Gtk::Scale &
	getBrushScaleScale () const
	{ return *m_BrushScaleScale; }

	Gtk::Box &
	getPaletteBox () const
	{ return *m_PaletteBox; }

	Gtk::Box &
	getPalettePreviewBox () const
	{ return *m_PalettePreviewBox; }

	Gtk::Box &
	getChangePaletteBox () const
	{ return *m_ChangePaletteBox; }

	Gtk::ComboBoxText &
	getPaletteComboBoxText () const
	{ return *m_PaletteComboBoxText; }

	Gtk::Button &
	getPalettePreviousButton () const
	{ return *m_PalettePreviousButton; }

	Gtk::Button &
	getPaletteNextButton () const
	{ return *m_PaletteNextButton; }

	Gtk::Dialog &
	getEditPaletteDialog () const
	{ return *m_EditPaletteDialog; }

	Gtk::Button &
	getEditPaletteCancelButton () const
	{ return *m_EditPaletteCancelButton; }

	Gtk::Button &
	getEditPaletteOkButton () const
	{ return *m_EditPaletteOkButton; }

	Gtk::Entry &
	getPaletteNameEntry () const
	{ return *m_PaletteNameEntry; }

	Gtk::Menu &
	getPaletteMenu () const
	{ return *m_PaletteMenu; }

	Gtk::ImageMenuItem &
	getAddPaletteMenuItem () const
	{ return *m_AddPaletteMenuItem; }

	Gtk::ImageMenuItem &
	getRemovePaletteMenuItem () const
	{ return *m_RemovePaletteMenuItem; }

	Gtk::ImageMenuItem &
	getEditPaletteMenuItem () const
	{ return *m_EditPaletteMenuItem; }

	Gtk::ImageMenuItem &
	getAddObjectToPaletteMenuItem () const
	{ return *m_AddObjectToPaletteMenuItem; }

	Gtk::ImageMenuItem &
	getUpdateObjectInPaletteMenuItem () const
	{ return *m_UpdateObjectInPaletteMenuItem; }

	Gtk::ImageMenuItem &
	getRemoveObjectFromPaletteMenuItem () const
	{ return *m_RemoveObjectFromPaletteMenuItem; }

	Gtk::CheckMenuItem &
	getShowDefaultPalettesMenuItem () const
	{ return *m_ShowDefaultPalettesMenuItem; }

	///  @name Signal handlers

	virtual
	void
	on_pull_polygons_toggled () = 0;

	virtual
	void
	on_push_polygons_toggled () = 0;

	virtual
	void
	on_roughten_polygons_toggled () = 0;

	virtual
	void
	on_smooth_polygons_toggled () = 0;

	virtual
	void
	on_undo_brush_toggled () = 0;

	virtual
	bool
	on_palette_button_press_event (GdkEventButton* button_event) = 0;

	virtual
	void
	on_palette_changed () = 0;

	virtual
	void
	on_palette_previous_clicked () = 0;

	virtual
	void
	on_palette_next_clicked () = 0;

	virtual
	void
	on_edit_palette_cancel_clicked () = 0;

	virtual
	void
	on_edit_palette_ok_clicked () = 0;

	virtual
	void
	on_palette_name_changed () = 0;

	virtual
	void
	on_palette_name_delete_text (int start_pos, int end_pos) = 0;

	virtual
	void
	on_palette_name_insert_text (const Glib::ustring & text, int* position) = 0;

	virtual
	void
	on_add_palette_activate () = 0;

	virtual
	void
	on_remove_palette_activate () = 0;

	virtual
	void
	on_edit_palette_activate () = 0;

	virtual
	void
	on_add_object_to_palette_activate () = 0;

	virtual
	void
	on_update_object_in_palette_activate () = 0;

	virtual
	void
	on_remove_object_from_palette_activate () = 0;

	virtual
	void
	on_show_default_palettes_toggled () = 0;

	///  @name Destruction

	virtual
	~X3DSculpToolEditorInterface () override;


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
	Glib::RefPtr <Gtk::Adjustment> m_BrushHardnessAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_BrushHeightAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_BrushPressureAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_BrushRadiusAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_BrushScaleAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_BrushSharpnessAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_BrushSpacingAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_BrushWarpAdjustment;
	Gtk::Window* m_Window;
	Gtk::Box* m_Widget;
	Gtk::Paned* m_Paned;
	Gtk::Box* m_PreviewBox;
	Gtk::HeaderBar* m_HeaderBar;
	Gtk::Toolbar* m_Toolbar;
	Gtk::RadioToolButton* m_PullPolygonsButton;
	Gtk::RadioToolButton* m_PushPolygonsButton;
	Gtk::RadioToolButton* m_RoughtenPolygonsButton;
	Gtk::RadioToolButton* m_SmoothPolygonsButton;
	Gtk::RadioToolButton* m_UndoBrushButton;
	Gtk::Notebook* m_Notebook;
	Gtk::Expander* m_BrushExpander;
	Gtk::Grid* m_BrushBox;
	Gtk::Scale* m_BrushWarpScale;
	Gtk::Scale* m_BrushSharpnessScale;
	Gtk::Scale* m_BrushHardnessScale;
	Gtk::ComboBoxText* m_BrushTypeButton;
	Gtk::SpinButton* m_BrushSpacingButton;
	Gtk::SpinButton* m_BrushHeightButton;
	Gtk::Scale* m_BrushPressureScale;
	Gtk::SpinButton* m_BrushRadiusButton;
	Gtk::Scale* m_BrushScaleScale;
	Gtk::Box* m_PaletteBox;
	Gtk::Box* m_PalettePreviewBox;
	Gtk::Box* m_ChangePaletteBox;
	Gtk::ComboBoxText* m_PaletteComboBoxText;
	Gtk::Button* m_PalettePreviousButton;
	Gtk::Button* m_PaletteNextButton;
	Gtk::Dialog* m_EditPaletteDialog;
	Gtk::Button* m_EditPaletteCancelButton;
	Gtk::Button* m_EditPaletteOkButton;
	Gtk::Entry* m_PaletteNameEntry;
	Gtk::Menu* m_PaletteMenu;
	Gtk::ImageMenuItem* m_AddPaletteMenuItem;
	Gtk::ImageMenuItem* m_RemovePaletteMenuItem;
	Gtk::ImageMenuItem* m_EditPaletteMenuItem;
	Gtk::ImageMenuItem* m_AddObjectToPaletteMenuItem;
	Gtk::ImageMenuItem* m_UpdateObjectInPaletteMenuItem;
	Gtk::ImageMenuItem* m_RemoveObjectFromPaletteMenuItem;
	Gtk::CheckMenuItem* m_ShowDefaultPalettesMenuItem;

};

} // puck
} // titania

#endif
