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
#ifndef __TMP_GLAD2CPP_TEXT_EDITOR_H__
#define __TMP_GLAD2CPP_TEXT_EDITOR_H__

#include "../Base/X3DEditorInterface.h"
#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

using namespace Gtk;

class X3DTextEditorInterface :
	public X3DEditorInterface
{
public:

	template <class ... Arguments>
	X3DTextEditorInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DEditorInterface (m_widgetName, arguments ...),
		          filename (filename)
	{ create (filename); }

	const Glib::RefPtr <Gtk::Builder> &
	getBuilder () const { return m_builder; }

	const std::string &
	getWidgetName () const { return m_widgetName; }

	void
	updateWidget (const Glib::ustring & name) const
	{ getBuilder () -> add_from_file (filename, name); }

	void
	updateWidgets (const std::vector <Glib::ustring> & names) const
	{ getBuilder () -> add_from_file (filename, names); }

	template <class Type>
	Type*
	getWidget (const std::string & name) const
	{
		Type* widget = nullptr;

		m_builder -> get_widget (name, widget);
		widget -> set_name (name);
		return widget;
	}

	const Glib::RefPtr <Gtk::ListStore> &
	getFamilyListStore () const
	{ return m_FamilyListStore; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getMaxExtentAdjustment () const
	{ return m_MaxExtentAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSizeAdjustment () const
	{ return m_SizeAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSpacingAdjustment () const
	{ return m_SpacingAdjustment; }

	const Glib::RefPtr <Gtk::TextBuffer> &
	getStringTextBuffer () const
	{ return m_StringTextBuffer; }

	const Glib::RefPtr <Gtk::TreeSelection> &
	getFamilySelection () const
	{ return m_FamilySelection; }

	const Glib::RefPtr <Gtk::TreeViewColumn> &
	getFamilyNameColumn () const
	{ return m_FamilyNameColumn; }

	const Glib::RefPtr <Gtk::CellRendererText> &
	getFamilyNameCellrendererText () const
	{ return m_FamilyNameCellrendererText; }

	const Glib::RefPtr <Gtk::TreeViewColumn> &
	getFamilyPadColumn () const
	{ return m_FamilyPadColumn; }

	const Glib::RefPtr <Gtk::TreeViewColumn> &
	getFamilyFontColumn () const
	{ return m_FamilyFontColumn; }

	const Glib::RefPtr <Gtk::CellRendererPixbuf> &
	getFamilyFontCellrendererPixbuf () const
	{ return m_FamilyFontCellrendererPixbuf; }

	Gtk::Window &
	getWindow () const
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const
	{ return *m_Widget; }

	Gtk::CheckButton &
	getTextCheckButton () const
	{ return *m_TextCheckButton; }

	Gtk::Grid &
	getTextBox () const
	{ return *m_TextBox; }

	Gtk::SpinButton &
	getMaxExtentSpinButton () const
	{ return *m_MaxExtentSpinButton; }

	Gtk::TextView &
	getStringTextView () const
	{ return *m_StringTextView; }

	Gtk::Box &
	getLenghtBox () const
	{ return *m_LenghtBox; }

	Gtk::Box &
	getFontStyleBox () const
	{ return *m_FontStyleBox; }

	Gtk::ComboBoxText &
	getFontStyleButton () const
	{ return *m_FontStyleButton; }

	Gtk::Grid &
	getFontStyleNodeBox () const
	{ return *m_FontStyleNodeBox; }

	Gtk::Label &
	getSizeLabel () const
	{ return *m_SizeLabel; }

	Gtk::SpinButton &
	getSizeSpinButton () const
	{ return *m_SizeSpinButton; }

	Gtk::SpinButton &
	getSpacingSpinButton () const
	{ return *m_SpacingSpinButton; }

	Gtk::CheckButton &
	getHorizontalCheckButton () const
	{ return *m_HorizontalCheckButton; }

	Gtk::CheckButton &
	getLeftToRightCheckButton () const
	{ return *m_LeftToRightCheckButton; }

	Gtk::CheckButton &
	getTopToBottomCheckButton () const
	{ return *m_TopToBottomCheckButton; }

	Gtk::ComboBoxText &
	getMajorAlignmentButton () const
	{ return *m_MajorAlignmentButton; }

	Gtk::ComboBoxText &
	getMinorAlignMentButton () const
	{ return *m_MinorAlignMentButton; }

	Gtk::ToggleButton &
	getItalicToggleButton () const
	{ return *m_ItalicToggleButton; }

	Gtk::ToggleButton &
	getBoldToggleButton () const
	{ return *m_BoldToggleButton; }

	Gtk::TreeView &
	getFamilyTreeView () const
	{ return *m_FamilyTreeView; }

	Gtk::Button &
	getAddFamilyButton () const
	{ return *m_AddFamilyButton; }

	Gtk::Button &
	getRemoveFamilyButton () const
	{ return *m_RemoveFamilyButton; }

	Gtk::FontChooserDialog &
	getFamilyChooserDialog () const
	{ return *m_FamilyChooserDialog; }

	virtual
	void
	on_string_changed () = 0;

	virtual
	void
	on_text_toggled () = 0;

	virtual
	void
	on_maxExtent_changed () = 0;

	virtual
	void
	on_fontStyle_changed () = 0;

	virtual
	void
	on_size_changed () = 0;

	virtual
	void
	on_spacing_changed () = 0;

	virtual
	void
	on_horizontal_toggled () = 0;

	virtual
	void
	on_leftToRight_toggled () = 0;

	virtual
	void
	on_topToBottom_toggled () = 0;

	virtual
	bool
	on_family_button_release_event (GdkEventButton* event) = 0;

	virtual
	void
	on_family_drag_data_received (const Glib::RefPtr <Gdk::DragContext> & context, int x, int y, const SelectionData & selection_data, guint info, guint time) = 0;

	virtual
	void
	on_family_changed () = 0;

	virtual
	void
	on_family_edited (const Glib::ustring & path, const Glib::ustring & new_text) = 0;

	virtual
	void
	on_add_family_clicked () = 0;

	virtual
	void
	on_remove_family_clicked () = 0;

	virtual
	~X3DTextEditorInterface ();


private:

	void
	create (const std::string &);

	static const std::string m_widgetName;

	std::string                            filename;
	Glib::RefPtr <Gtk::Builder>            m_builder;
	Glib::RefPtr <Gtk::ListStore>          m_FamilyListStore;
	Glib::RefPtr <Gtk::Adjustment>         m_MaxExtentAdjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_SizeAdjustment;
	Glib::RefPtr <Gtk::Adjustment>         m_SpacingAdjustment;
	Glib::RefPtr <Gtk::TextBuffer>         m_StringTextBuffer;
	Glib::RefPtr <Gtk::TreeSelection>      m_FamilySelection;
	Glib::RefPtr <Gtk::TreeViewColumn>     m_FamilyNameColumn;
	Glib::RefPtr <Gtk::CellRendererText>   m_FamilyNameCellrendererText;
	Glib::RefPtr <Gtk::TreeViewColumn>     m_FamilyPadColumn;
	Glib::RefPtr <Gtk::TreeViewColumn>     m_FamilyFontColumn;
	Glib::RefPtr <Gtk::CellRendererPixbuf> m_FamilyFontCellrendererPixbuf;
	Gtk::Window*                           m_Window;
	Gtk::Box*                              m_Widget;
	Gtk::CheckButton*                      m_TextCheckButton;
	Gtk::Grid*                             m_TextBox;
	Gtk::SpinButton*                       m_MaxExtentSpinButton;
	Gtk::TextView*                         m_StringTextView;
	Gtk::Box*                              m_LenghtBox;
	Gtk::Box*                              m_FontStyleBox;
	Gtk::ComboBoxText*                     m_FontStyleButton;
	Gtk::Grid*                             m_FontStyleNodeBox;
	Gtk::Label*                            m_SizeLabel;
	Gtk::SpinButton*                       m_SizeSpinButton;
	Gtk::SpinButton*                       m_SpacingSpinButton;
	Gtk::CheckButton*                      m_HorizontalCheckButton;
	Gtk::CheckButton*                      m_LeftToRightCheckButton;
	Gtk::CheckButton*                      m_TopToBottomCheckButton;
	Gtk::ComboBoxText*                     m_MajorAlignmentButton;
	Gtk::ComboBoxText*                     m_MinorAlignMentButton;
	Gtk::ToggleButton*                     m_ItalicToggleButton;
	Gtk::ToggleButton*                     m_BoldToggleButton;
	Gtk::TreeView*                         m_FamilyTreeView;
	Gtk::Button*                           m_AddFamilyButton;
	Gtk::Button*                           m_RemoveFamilyButton;
	Gtk::FontChooserDialog*                m_FamilyChooserDialog;

};

} // puck
} // titania

#endif
