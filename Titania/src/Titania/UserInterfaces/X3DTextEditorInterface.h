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

#ifndef __TMP_GLAD2CPP_TEXT_EDITOR_H__
#define __TMP_GLAD2CPP_TEXT_EDITOR_H__

#include "../Base/X3DEditorInterface.h"

#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

/**
 *  Gtk Interface for TextEditor.
 */
class X3DTextEditorInterface :
	public X3DEditorInterface
{
public:

	///  @name Construction

	X3DTextEditorInterface () :
		X3DEditorInterface ()
	{ }

	template <class ... Arguments>
	X3DTextEditorInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DEditorInterface (arguments ...)
	{ create (filename); }

	template <class ... Arguments>
	X3DTextEditorInterface (std::initializer_list <std::string> filenames, const Arguments & ... arguments) :
		X3DEditorInterface (arguments ...)
	{ create (filenames); }

	///  @name Member access

	const Glib::RefPtr <Gtk::Builder> &
	getBuilder () const
	{ return m_builder; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getFontStyleBezierDimensionAdjustment () const
	{ return m_FontStyleBezierDimensionAdjustment; }

	const Glib::RefPtr <Gtk::ListStore> &
	getFontStyleFamilyListStore () const
	{ return m_FontStyleFamilyListStore; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getFontStylePointSizeAdjustment () const
	{ return m_FontStylePointSizeAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getFontStyleSizeAdjustment () const
	{ return m_FontStyleSizeAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getFontStyleSpacingAdjustment () const
	{ return m_FontStyleSpacingAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTextCharSpacingAdjustment () const
	{ return m_TextCharSpacingAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTextMaxExtentAdjustment () const
	{ return m_TextMaxExtentAdjustment; }

	const Glib::RefPtr <Gtk::TextBuffer> &
	getTextStringTextBuffer () const
	{ return m_TextStringTextBuffer; }

	const Glib::RefPtr <Gtk::TreeSelection> &
	getFontStyleFamilySelection () const
	{ return m_FontStyleFamilySelection; }

	const Glib::RefPtr <Gtk::TreeViewColumn> &
	getFontStyleFamilyColumn () const
	{ return m_FontStyleFamilyColumn; }

	const Glib::RefPtr <Gtk::CellRendererText> &
	getFontStyleFamilyCellrendererText () const
	{ return m_FontStyleFamilyCellrendererText; }

	const Glib::RefPtr <Gtk::TreeViewColumn> &
	getFontStyleFamilyChooserColumn () const
	{ return m_FontStyleFamilyChooserColumn; }

	const Glib::RefPtr <Gtk::TreeViewColumn> &
	getFontStyleFileChooserColumn () const
	{ return m_FontStyleFileChooserColumn; }

	Gtk::Window &
	getWindow () const final override
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const final override
	{ return *m_Widget; }

	Gtk::HeaderBar &
	getHeaderBar () const
	{ return *m_HeaderBar; }

	Gtk::Notebook &
	getTextNotebook () const
	{ return *m_TextNotebook; }

	Gtk::Expander &
	getTextExpander () const
	{ return *m_TextExpander; }

	Gtk::Box &
	getSelectTextBox () const
	{ return *m_SelectTextBox; }

	Gtk::CheckButton &
	getTextCheckButton () const
	{ return *m_TextCheckButton; }

	Gtk::Button &
	getTextUnlinkButton () const
	{ return *m_TextUnlinkButton; }

	Gtk::Grid &
	getTextBox () const
	{ return *m_TextBox; }

	Gtk::SpinButton &
	getTextMaxExtentSpinButton () const
	{ return *m_TextMaxExtentSpinButton; }

	Gtk::TextView &
	getTextStringTextView () const
	{ return *m_TextStringTextView; }

	Gtk::Box &
	getLenghtBox () const
	{ return *m_LenghtBox; }

	Gtk::SpinButton &
	getTextCharSpacingSpinButton () const
	{ return *m_TextCharSpacingSpinButton; }

	Gtk::CheckButton &
	getTextWrappingCheckButton () const
	{ return *m_TextWrappingCheckButton; }

	Gtk::Expander &
	getFontStyleExpander () const
	{ return *m_FontStyleExpander; }

	Gtk::Box &
	getFontStyleBox () const
	{ return *m_FontStyleBox; }

	Gtk::Box &
	getSelectFontStyleBox () const
	{ return *m_SelectFontStyleBox; }

	Gtk::ComboBoxText &
	getFontStyleComboBoxText () const
	{ return *m_FontStyleComboBoxText; }

	Gtk::Button &
	getFontStyleUnlinkButton () const
	{ return *m_FontStyleUnlinkButton; }

	Gtk::Grid &
	getFontStyleNodeBox () const
	{ return *m_FontStyleNodeBox; }

	Gtk::Label &
	getFontStyleSizeLabel () const
	{ return *m_FontStyleSizeLabel; }

	Gtk::Label &
	getFontStylePointSizeLabel () const
	{ return *m_FontStylePointSizeLabel; }

	Gtk::SpinButton &
	getFontStyleSizeSpinButton () const
	{ return *m_FontStyleSizeSpinButton; }

	Gtk::SpinButton &
	getFontStyleSpacingSpinButton () const
	{ return *m_FontStyleSpacingSpinButton; }

	Gtk::CheckButton &
	getFontStyleHorizontalCheckButton () const
	{ return *m_FontStyleHorizontalCheckButton; }

	Gtk::CheckButton &
	getFontStyleLeftToRightCheckButton () const
	{ return *m_FontStyleLeftToRightCheckButton; }

	Gtk::CheckButton &
	getFontStyleTopToBottomCheckButton () const
	{ return *m_FontStyleTopToBottomCheckButton; }

	Gtk::ComboBoxText &
	getFontStyleMajorAlignmentComboBoxText () const
	{ return *m_FontStyleMajorAlignmentComboBoxText; }

	Gtk::ComboBoxText &
	getFontStyleMinorAlignmentComboBoxText () const
	{ return *m_FontStyleMinorAlignmentComboBoxText; }

	Gtk::ToggleButton &
	getFontStyleBoldToggleButton () const
	{ return *m_FontStyleBoldToggleButton; }

	Gtk::ToggleButton &
	getFontStyleItalicToggleButton () const
	{ return *m_FontStyleItalicToggleButton; }

	Gtk::TreeView &
	getFontStyleFamilyTreeView () const
	{ return *m_FontStyleFamilyTreeView; }

	Gtk::Button &
	getFontStyleFamilyAddButton () const
	{ return *m_FontStyleFamilyAddButton; }

	Gtk::Button &
	getFontStyleFamilyRemoveButton () const
	{ return *m_FontStyleFamilyRemoveButton; }

	Gtk::SpinButton &
	getFontStylePointSizeSpinButton () const
	{ return *m_FontStylePointSizeSpinButton; }

	Gtk::Grid &
	getFontStyleGlobalOptionsBox () const
	{ return *m_FontStyleGlobalOptionsBox; }

	Gtk::SpinButton &
	getFontStyleBezierDimensionSpinButton () const
	{ return *m_FontStyleBezierDimensionSpinButton; }

	///  @name Signal handlers

	virtual
	void
	on_char_spacing_changed () = 0;

	virtual
	void
	on_text_toggled () = 0;

	virtual
	void
	on_text_unlink_clicked () = 0;

	virtual
	void
	on_fontStyle_changed () = 0;

	virtual
	void
	on_fontStyle_unlink_clicked () = 0;

	virtual
	void
	on_style_toggled () = 0;

	///  @name Destruction

	virtual
	~X3DTextEditorInterface () override;


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
	Glib::RefPtr <Gtk::Adjustment> m_FontStyleBezierDimensionAdjustment;
	Glib::RefPtr <Gtk::ListStore> m_FontStyleFamilyListStore;
	Glib::RefPtr <Gtk::Adjustment> m_FontStylePointSizeAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_FontStyleSizeAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_FontStyleSpacingAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TextCharSpacingAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TextMaxExtentAdjustment;
	Glib::RefPtr <Gtk::TextBuffer> m_TextStringTextBuffer;
	Glib::RefPtr <Gtk::TreeSelection> m_FontStyleFamilySelection;
	Glib::RefPtr <Gtk::TreeViewColumn> m_FontStyleFamilyColumn;
	Glib::RefPtr <Gtk::CellRendererText> m_FontStyleFamilyCellrendererText;
	Glib::RefPtr <Gtk::TreeViewColumn> m_FontStyleFamilyChooserColumn;
	Glib::RefPtr <Gtk::TreeViewColumn> m_FontStyleFileChooserColumn;
	Gtk::Window* m_Window;
	Gtk::Box* m_Widget;
	Gtk::HeaderBar* m_HeaderBar;
	Gtk::Notebook* m_TextNotebook;
	Gtk::Expander* m_TextExpander;
	Gtk::Box* m_SelectTextBox;
	Gtk::CheckButton* m_TextCheckButton;
	Gtk::Button* m_TextUnlinkButton;
	Gtk::Grid* m_TextBox;
	Gtk::SpinButton* m_TextMaxExtentSpinButton;
	Gtk::TextView* m_TextStringTextView;
	Gtk::Box* m_LenghtBox;
	Gtk::SpinButton* m_TextCharSpacingSpinButton;
	Gtk::CheckButton* m_TextWrappingCheckButton;
	Gtk::Expander* m_FontStyleExpander;
	Gtk::Box* m_FontStyleBox;
	Gtk::Box* m_SelectFontStyleBox;
	Gtk::ComboBoxText* m_FontStyleComboBoxText;
	Gtk::Button* m_FontStyleUnlinkButton;
	Gtk::Grid* m_FontStyleNodeBox;
	Gtk::Label* m_FontStyleSizeLabel;
	Gtk::Label* m_FontStylePointSizeLabel;
	Gtk::SpinButton* m_FontStyleSizeSpinButton;
	Gtk::SpinButton* m_FontStyleSpacingSpinButton;
	Gtk::CheckButton* m_FontStyleHorizontalCheckButton;
	Gtk::CheckButton* m_FontStyleLeftToRightCheckButton;
	Gtk::CheckButton* m_FontStyleTopToBottomCheckButton;
	Gtk::ComboBoxText* m_FontStyleMajorAlignmentComboBoxText;
	Gtk::ComboBoxText* m_FontStyleMinorAlignmentComboBoxText;
	Gtk::ToggleButton* m_FontStyleBoldToggleButton;
	Gtk::ToggleButton* m_FontStyleItalicToggleButton;
	Gtk::TreeView* m_FontStyleFamilyTreeView;
	Gtk::Button* m_FontStyleFamilyAddButton;
	Gtk::Button* m_FontStyleFamilyRemoveButton;
	Gtk::SpinButton* m_FontStylePointSizeSpinButton;
	Gtk::Grid* m_FontStyleGlobalOptionsBox;
	Gtk::SpinButton* m_FontStyleBezierDimensionSpinButton;

};

} // puck
} // titania

#endif
