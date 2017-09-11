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

#ifndef __TMP_GLAD2CPP_LIBRARY_VIEW_H__
#define __TMP_GLAD2CPP_LIBRARY_VIEW_H__

#include "../Base/X3DEditorInterface.h"
#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

/**
 *  Gtk Interface for LibraryView.
 */
class X3DLibraryViewInterface :
	public X3DEditorInterface
{
public:

	///  @name Construction

	X3DLibraryViewInterface () :
		X3DEditorInterface ()
	{ }

	template <class ... Arguments>
	X3DLibraryViewInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DEditorInterface (arguments ...)
	{ create (filename); }

	template <class ... Arguments>
	X3DLibraryViewInterface (std::initializer_list <std::string> filenames, const Arguments & ... arguments) :
		X3DEditorInterface (arguments ...)
	{ create (filenames); }

	///  @name Member access

	const Glib::RefPtr <Gtk::Builder> &
	getBuilder () const
	{ return m_builder; }

	const Glib::RefPtr <Gtk::TreeStore> &
	getTreeStore () const
	{ return m_TreeStore; }

	const Glib::RefPtr <Gtk::TreeViewColumn> &
	getFileColumn () const
	{ return m_FileColumn; }

	const Glib::RefPtr <Gtk::CellRendererPixbuf> &
	getIconRenderer () const
	{ return m_IconRenderer; }

	const Glib::RefPtr <Gtk::CellRendererText> &
	getNameRenderer () const
	{ return m_NameRenderer; }

	const Glib::RefPtr <Gtk::TreeViewColumn> &
	getExperimantalColumn () const
	{ return m_ExperimantalColumn; }

	const Glib::RefPtr <Gtk::CellRendererPixbuf> &
	getExperimantalRenderer () const
	{ return m_ExperimantalRenderer; }

	const Glib::RefPtr <Gtk::TreeViewColumn> &
	getTitaniaColumn () const
	{ return m_TitaniaColumn; }

	const Glib::RefPtr <Gtk::CellRendererPixbuf> &
	getTitaniaRenderer () const
	{ return m_TitaniaRenderer; }

	const Glib::RefPtr <Gtk::TreeViewColumn> &
	getCobwebColumn () const
	{ return m_CobwebColumn; }

	const Glib::RefPtr <Gtk::CellRendererPixbuf> &
	getCobwebRenderer () const
	{ return m_CobwebRenderer; }

	Gtk::Window &
	getWindow () const
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const
	{ return *m_Widget; }

	Gtk::HeaderBar &
	getHeaderBar () const
	{ return *m_HeaderBar; }

	Gtk::Notebook &
	getNotebook () const
	{ return *m_Notebook; }

	Gtk::Box &
	getFilesBox () const
	{ return *m_FilesBox; }

	Gtk::ScrolledWindow &
	getScrolledWindow () const
	{ return *m_ScrolledWindow; }

	Gtk::TreeView &
	getTreeView () const
	{ return *m_TreeView; }

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

	///  @name Signal handlers

	virtual
	void
	on_row_activated (const Gtk::TreeModel::Path &, Gtk::TreeViewColumn*) = 0;

	virtual
	bool
	on_palette_button_press_event (GdkEventButton* event) = 0;

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

	///  @name Destruction

	virtual
	~X3DLibraryViewInterface () override;


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
	Glib::RefPtr <Gtk::TreeStore> m_TreeStore;
	Glib::RefPtr <Gtk::TreeViewColumn> m_FileColumn;
	Glib::RefPtr <Gtk::CellRendererPixbuf> m_IconRenderer;
	Glib::RefPtr <Gtk::CellRendererText> m_NameRenderer;
	Glib::RefPtr <Gtk::TreeViewColumn> m_ExperimantalColumn;
	Glib::RefPtr <Gtk::CellRendererPixbuf> m_ExperimantalRenderer;
	Glib::RefPtr <Gtk::TreeViewColumn> m_TitaniaColumn;
	Glib::RefPtr <Gtk::CellRendererPixbuf> m_TitaniaRenderer;
	Glib::RefPtr <Gtk::TreeViewColumn> m_CobwebColumn;
	Glib::RefPtr <Gtk::CellRendererPixbuf> m_CobwebRenderer;
	Gtk::Window* m_Window;
	Gtk::Box* m_Widget;
	Gtk::HeaderBar* m_HeaderBar;
	Gtk::Notebook* m_Notebook;
	Gtk::Box* m_FilesBox;
	Gtk::ScrolledWindow* m_ScrolledWindow;
	Gtk::TreeView* m_TreeView;
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

};

} // puck
} // titania

#endif
