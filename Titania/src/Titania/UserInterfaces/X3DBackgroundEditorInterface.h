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

#ifndef __TMP_GLAD2CPP_BACKGROUND_EDITOR_H__
#define __TMP_GLAD2CPP_BACKGROUND_EDITOR_H__

#include "../Base/X3DEditorInterface.h"

#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

/**
 *  Gtk Interface for BackgroundEditor.
 */
class X3DBackgroundEditorInterface :
	public X3DEditorInterface
{
public:

	///  @name Construction

	X3DBackgroundEditorInterface () :
		X3DEditorInterface ()
	{ }

	template <class ... Arguments>
	X3DBackgroundEditorInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DEditorInterface (arguments ...)
	{ create (filename); }

	template <class ... Arguments>
	X3DBackgroundEditorInterface (std::initializer_list <std::string> filenames, const Arguments & ... arguments) :
		X3DEditorInterface (arguments ...)
	{ create (filenames); }

	///  @name Member access

	const Glib::RefPtr <Gtk::Builder> &
	getBuilder () const
	{ return m_builder; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGroundAngleAdjustment () const
	{ return m_GroundAngleAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGroundColorAdjustment () const
	{ return m_GroundColorAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSkyAngleAdjustment () const
	{ return m_SkyAngleAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSkyColorAdjustment () const
	{ return m_SkyColorAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTransparencyAdjustment () const
	{ return m_TransparencyAdjustment; }

	const Glib::RefPtr <Gtk::CellRendererText> &
	getFrontURLCellRendererText () const
	{ return m_FrontURLCellRendererText; }

	const Glib::RefPtr <Gtk::TreeViewColumn> &
	getFrontURLChooserColumn () const
	{ return m_FrontURLChooserColumn; }

	const Glib::RefPtr <Gtk::CellRendererPixbuf> &
	getFrontURLCellrendererPixbuf () const
	{ return m_FrontURLCellrendererPixbuf; }

	const Glib::RefPtr <Gtk::CellRendererText> &
	getBackURLCellRendererText () const
	{ return m_BackURLCellRendererText; }

	const Glib::RefPtr <Gtk::TreeViewColumn> &
	getBackURLChooserColumn () const
	{ return m_BackURLChooserColumn; }

	const Glib::RefPtr <Gtk::CellRendererPixbuf> &
	getBackURLCellrendererPixbuf () const
	{ return m_BackURLCellrendererPixbuf; }

	const Glib::RefPtr <Gtk::CellRendererText> &
	getLeftURLCellRendererText () const
	{ return m_LeftURLCellRendererText; }

	const Glib::RefPtr <Gtk::TreeViewColumn> &
	getLeftURLChooserColumn () const
	{ return m_LeftURLChooserColumn; }

	const Glib::RefPtr <Gtk::CellRendererPixbuf> &
	getLeftURLCellrendererPixbuf () const
	{ return m_LeftURLCellrendererPixbuf; }

	const Glib::RefPtr <Gtk::CellRendererText> &
	getRightURLCellRendererText () const
	{ return m_RightURLCellRendererText; }

	const Glib::RefPtr <Gtk::TreeViewColumn> &
	getRightURLChooserColumn () const
	{ return m_RightURLChooserColumn; }

	const Glib::RefPtr <Gtk::CellRendererPixbuf> &
	getRightURLCellrendererPixbuf () const
	{ return m_RightURLCellrendererPixbuf; }

	const Glib::RefPtr <Gtk::CellRendererText> &
	getTopURLCellRendererText () const
	{ return m_TopURLCellRendererText; }

	const Glib::RefPtr <Gtk::TreeViewColumn> &
	getTopURLChooserColumn () const
	{ return m_TopURLChooserColumn; }

	const Glib::RefPtr <Gtk::CellRendererPixbuf> &
	getTopURLCellrendererPixbuf () const
	{ return m_TopURLCellrendererPixbuf; }

	const Glib::RefPtr <Gtk::CellRendererText> &
	getBottomURLCellRendererText () const
	{ return m_BottomURLCellRendererText; }

	const Glib::RefPtr <Gtk::TreeViewColumn> &
	getBottomURLChooserColumn () const
	{ return m_BottomURLChooserColumn; }

	const Glib::RefPtr <Gtk::CellRendererPixbuf> &
	getBottomURLCellrendererPixbuf () const
	{ return m_BottomURLCellrendererPixbuf; }

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
	getBackgroundListBox () const
	{ return *m_BackgroundListBox; }

	Gtk::HeaderBar &
	getHeaderBar () const
	{ return *m_HeaderBar; }

	Gtk::Box &
	getActionBox () const
	{ return *m_ActionBox; }

	Gtk::Button &
	getNewBackgroundPopupButton () const
	{ return *m_NewBackgroundPopupButton; }

	Gtk::Button &
	getRemoveBackgroundButton () const
	{ return *m_RemoveBackgroundButton; }

	Gtk::Expander &
	getBackgroundExpander () const
	{ return *m_BackgroundExpander; }

	Gtk::Grid &
	getBackgroundBox () const
	{ return *m_BackgroundBox; }

	Gtk::Box &
	getNameBox () const
	{ return *m_NameBox; }

	Gtk::Entry &
	getNameEntry () const
	{ return *m_NameEntry; }

	Gtk::Button &
	getRenameButton () const
	{ return *m_RenameButton; }

	Gtk::Expander &
	getColorsExpander () const
	{ return *m_ColorsExpander; }

	Gtk::Box &
	getSkyBox () const
	{ return *m_SkyBox; }

	Gtk::SpinButton &
	getSkyAngleSpinButton () const
	{ return *m_SkyAngleSpinButton; }

	Gtk::Box &
	getSkyGradientBox () const
	{ return *m_SkyGradientBox; }

	Gtk::Grid &
	getSkyColorGrid () const
	{ return *m_SkyColorGrid; }

	Gtk::ScrolledWindow &
	getSkyColorsScrolledWindow () const
	{ return *m_SkyColorsScrolledWindow; }

	Gtk::Button &
	getAddSkyColorButton () const
	{ return *m_AddSkyColorButton; }

	Gtk::Button &
	getRemoveSkyColorButton () const
	{ return *m_RemoveSkyColorButton; }

	Gtk::Box &
	getSkyColorBox () const
	{ return *m_SkyColorBox; }

	Gtk::Button &
	getSkyColorButton () const
	{ return *m_SkyColorButton; }

	Gtk::Box &
	getGroundBox () const
	{ return *m_GroundBox; }

	Gtk::SpinButton &
	getGroundAngleSpinButton () const
	{ return *m_GroundAngleSpinButton; }

	Gtk::Box &
	getGroundGradientBox () const
	{ return *m_GroundGradientBox; }

	Gtk::Grid &
	getGroundColorGrid () const
	{ return *m_GroundColorGrid; }

	Gtk::ScrolledWindow &
	getGroundColorsScrolledWindow () const
	{ return *m_GroundColorsScrolledWindow; }

	Gtk::Button &
	getAddGroundColorButton () const
	{ return *m_AddGroundColorButton; }

	Gtk::Button &
	getRemoveGroundColorButton () const
	{ return *m_RemoveGroundColorButton; }

	Gtk::Box &
	getGroundColorBox () const
	{ return *m_GroundColorBox; }

	Gtk::Button &
	getGroundColorButton () const
	{ return *m_GroundColorButton; }

	Gtk::Scale &
	getTransparencyScale () const
	{ return *m_TransparencyScale; }

	Gtk::Expander &
	getTextureExpander () const
	{ return *m_TextureExpander; }

	Gtk::Notebook &
	getTexturesNotebook () const
	{ return *m_TexturesNotebook; }

	Gtk::Label &
	getFrontTextureFormatLabel () const
	{ return *m_FrontTextureFormatLabel; }

	Gtk::Label &
	getFrontTextureLoadStateLabel () const
	{ return *m_FrontTextureLoadStateLabel; }

	Gtk::Box &
	getFrontTexturePreviewBox () const
	{ return *m_FrontTexturePreviewBox; }

	Gtk::Grid &
	getFrontURLGrid () const
	{ return *m_FrontURLGrid; }

	Gtk::Box &
	getFrontURLBox () const
	{ return *m_FrontURLBox; }

	Gtk::TreeView &
	getFrontURLTreeView () const
	{ return *m_FrontURLTreeView; }

	Gtk::Button &
	getFrontURLAddButton () const
	{ return *m_FrontURLAddButton; }

	Gtk::Button &
	getFrontURLRemoveButton () const
	{ return *m_FrontURLRemoveButton; }

	Gtk::Button &
	getFrontURLReloadButton () const
	{ return *m_FrontURLReloadButton; }

	Gtk::Label &
	getBackTextureFormatLabel () const
	{ return *m_BackTextureFormatLabel; }

	Gtk::Label &
	getBackTextureLoadStateLabel () const
	{ return *m_BackTextureLoadStateLabel; }

	Gtk::Box &
	getBackTexturePreviewBox () const
	{ return *m_BackTexturePreviewBox; }

	Gtk::Grid &
	getBackURLGrid () const
	{ return *m_BackURLGrid; }

	Gtk::Box &
	getBackURLBox () const
	{ return *m_BackURLBox; }

	Gtk::TreeView &
	getBackURLTreeView () const
	{ return *m_BackURLTreeView; }

	Gtk::Button &
	getBackURLAddButton () const
	{ return *m_BackURLAddButton; }

	Gtk::Button &
	getBackURLRemoveButton () const
	{ return *m_BackURLRemoveButton; }

	Gtk::Button &
	getBackURLReloadButton () const
	{ return *m_BackURLReloadButton; }

	Gtk::Label &
	getLeftTextureFormatLabel () const
	{ return *m_LeftTextureFormatLabel; }

	Gtk::Label &
	getLeftTextureLoadStateLabel () const
	{ return *m_LeftTextureLoadStateLabel; }

	Gtk::Box &
	getLeftTexturePreviewBox () const
	{ return *m_LeftTexturePreviewBox; }

	Gtk::Grid &
	getLeftURLGrid () const
	{ return *m_LeftURLGrid; }

	Gtk::Box &
	getLeftURLBox () const
	{ return *m_LeftURLBox; }

	Gtk::TreeView &
	getLeftURLTreeView () const
	{ return *m_LeftURLTreeView; }

	Gtk::Button &
	getLeftURLAddButton () const
	{ return *m_LeftURLAddButton; }

	Gtk::Button &
	getLeftURLRemoveButton () const
	{ return *m_LeftURLRemoveButton; }

	Gtk::Button &
	getLeftURLReloadButton () const
	{ return *m_LeftURLReloadButton; }

	Gtk::Label &
	getRightTextureFormatLabel () const
	{ return *m_RightTextureFormatLabel; }

	Gtk::Label &
	getRightTextureLoadStateLabel () const
	{ return *m_RightTextureLoadStateLabel; }

	Gtk::Box &
	getRightTexturePreviewBox () const
	{ return *m_RightTexturePreviewBox; }

	Gtk::Grid &
	getRightURLGrid () const
	{ return *m_RightURLGrid; }

	Gtk::Box &
	getRightURLBox () const
	{ return *m_RightURLBox; }

	Gtk::TreeView &
	getRightURLTreeView () const
	{ return *m_RightURLTreeView; }

	Gtk::Button &
	getRightURLAddButton () const
	{ return *m_RightURLAddButton; }

	Gtk::Button &
	getRightURLRemoveButton () const
	{ return *m_RightURLRemoveButton; }

	Gtk::Button &
	getRightURLReloadButton () const
	{ return *m_RightURLReloadButton; }

	Gtk::Label &
	getTopTextureFormatLabel () const
	{ return *m_TopTextureFormatLabel; }

	Gtk::Label &
	getTopTextureLoadStateLabel () const
	{ return *m_TopTextureLoadStateLabel; }

	Gtk::Box &
	getTopTexturePreviewBox () const
	{ return *m_TopTexturePreviewBox; }

	Gtk::Grid &
	getTopURLGrid () const
	{ return *m_TopURLGrid; }

	Gtk::Box &
	getTopURLBox () const
	{ return *m_TopURLBox; }

	Gtk::TreeView &
	getTopURLTreeView () const
	{ return *m_TopURLTreeView; }

	Gtk::Button &
	getTopURLAddButton () const
	{ return *m_TopURLAddButton; }

	Gtk::Button &
	getTopURLRemoveButton () const
	{ return *m_TopURLRemoveButton; }

	Gtk::Button &
	getTopURLReloadButton () const
	{ return *m_TopURLReloadButton; }

	Gtk::Label &
	getBottomTextureFormatLabel () const
	{ return *m_BottomTextureFormatLabel; }

	Gtk::Label &
	getBottomTextureLoadStateLabel () const
	{ return *m_BottomTextureLoadStateLabel; }

	Gtk::Box &
	getBottomTexturePreviewBox () const
	{ return *m_BottomTexturePreviewBox; }

	Gtk::Grid &
	getBottomURLGrid () const
	{ return *m_BottomURLGrid; }

	Gtk::Box &
	getBottomURLBox () const
	{ return *m_BottomURLBox; }

	Gtk::TreeView &
	getBottomURLTreeView () const
	{ return *m_BottomURLTreeView; }

	Gtk::Button &
	getBottomURLAddButton () const
	{ return *m_BottomURLAddButton; }

	Gtk::Button &
	getBottomURLRemoveButton () const
	{ return *m_BottomURLRemoveButton; }

	Gtk::Button &
	getBottomURLReloadButton () const
	{ return *m_BottomURLReloadButton; }

	Gtk::Popover &
	getNewBackgroundPopover () const
	{ return *m_NewBackgroundPopover; }

	Gtk::Button &
	getNewBackgroundButton () const
	{ return *m_NewBackgroundButton; }

	Gtk::Button &
	getNewTextureBackgroundButton () const
	{ return *m_NewTextureBackgroundButton; }

	///  @name Signal handlers

	virtual
	void
	on_new_background_popup_clicked () = 0;

	virtual
	void
	on_remove_background_clicked () = 0;

	virtual
	void
	on_new_background_clicked () = 0;

	virtual
	void
	on_new_texture_background_clicked () = 0;

	///  @name Destruction

	virtual
	~X3DBackgroundEditorInterface () override;


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
	Glib::RefPtr <Gtk::Adjustment> m_GroundAngleAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GroundColorAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_SkyAngleAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_SkyColorAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TransparencyAdjustment;
	Glib::RefPtr <Gtk::CellRendererText> m_FrontURLCellRendererText;
	Glib::RefPtr <Gtk::TreeViewColumn> m_FrontURLChooserColumn;
	Glib::RefPtr <Gtk::CellRendererPixbuf> m_FrontURLCellrendererPixbuf;
	Glib::RefPtr <Gtk::CellRendererText> m_BackURLCellRendererText;
	Glib::RefPtr <Gtk::TreeViewColumn> m_BackURLChooserColumn;
	Glib::RefPtr <Gtk::CellRendererPixbuf> m_BackURLCellrendererPixbuf;
	Glib::RefPtr <Gtk::CellRendererText> m_LeftURLCellRendererText;
	Glib::RefPtr <Gtk::TreeViewColumn> m_LeftURLChooserColumn;
	Glib::RefPtr <Gtk::CellRendererPixbuf> m_LeftURLCellrendererPixbuf;
	Glib::RefPtr <Gtk::CellRendererText> m_RightURLCellRendererText;
	Glib::RefPtr <Gtk::TreeViewColumn> m_RightURLChooserColumn;
	Glib::RefPtr <Gtk::CellRendererPixbuf> m_RightURLCellrendererPixbuf;
	Glib::RefPtr <Gtk::CellRendererText> m_TopURLCellRendererText;
	Glib::RefPtr <Gtk::TreeViewColumn> m_TopURLChooserColumn;
	Glib::RefPtr <Gtk::CellRendererPixbuf> m_TopURLCellrendererPixbuf;
	Glib::RefPtr <Gtk::CellRendererText> m_BottomURLCellRendererText;
	Glib::RefPtr <Gtk::TreeViewColumn> m_BottomURLChooserColumn;
	Glib::RefPtr <Gtk::CellRendererPixbuf> m_BottomURLCellrendererPixbuf;
	Gtk::Window* m_Window;
	Gtk::Box* m_Widget;
	Gtk::Paned* m_Paned;
	Gtk::Box* m_BackgroundListBox;
	Gtk::HeaderBar* m_HeaderBar;
	Gtk::Box* m_ActionBox;
	Gtk::Button* m_NewBackgroundPopupButton;
	Gtk::Button* m_RemoveBackgroundButton;
	Gtk::Expander* m_BackgroundExpander;
	Gtk::Grid* m_BackgroundBox;
	Gtk::Box* m_NameBox;
	Gtk::Entry* m_NameEntry;
	Gtk::Button* m_RenameButton;
	Gtk::Expander* m_ColorsExpander;
	Gtk::Box* m_SkyBox;
	Gtk::SpinButton* m_SkyAngleSpinButton;
	Gtk::Box* m_SkyGradientBox;
	Gtk::Grid* m_SkyColorGrid;
	Gtk::ScrolledWindow* m_SkyColorsScrolledWindow;
	Gtk::Button* m_AddSkyColorButton;
	Gtk::Button* m_RemoveSkyColorButton;
	Gtk::Box* m_SkyColorBox;
	Gtk::Button* m_SkyColorButton;
	Gtk::Box* m_GroundBox;
	Gtk::SpinButton* m_GroundAngleSpinButton;
	Gtk::Box* m_GroundGradientBox;
	Gtk::Grid* m_GroundColorGrid;
	Gtk::ScrolledWindow* m_GroundColorsScrolledWindow;
	Gtk::Button* m_AddGroundColorButton;
	Gtk::Button* m_RemoveGroundColorButton;
	Gtk::Box* m_GroundColorBox;
	Gtk::Button* m_GroundColorButton;
	Gtk::Scale* m_TransparencyScale;
	Gtk::Expander* m_TextureExpander;
	Gtk::Notebook* m_TexturesNotebook;
	Gtk::Label* m_FrontTextureFormatLabel;
	Gtk::Label* m_FrontTextureLoadStateLabel;
	Gtk::Box* m_FrontTexturePreviewBox;
	Gtk::Grid* m_FrontURLGrid;
	Gtk::Box* m_FrontURLBox;
	Gtk::TreeView* m_FrontURLTreeView;
	Gtk::Button* m_FrontURLAddButton;
	Gtk::Button* m_FrontURLRemoveButton;
	Gtk::Button* m_FrontURLReloadButton;
	Gtk::Label* m_BackTextureFormatLabel;
	Gtk::Label* m_BackTextureLoadStateLabel;
	Gtk::Box* m_BackTexturePreviewBox;
	Gtk::Grid* m_BackURLGrid;
	Gtk::Box* m_BackURLBox;
	Gtk::TreeView* m_BackURLTreeView;
	Gtk::Button* m_BackURLAddButton;
	Gtk::Button* m_BackURLRemoveButton;
	Gtk::Button* m_BackURLReloadButton;
	Gtk::Label* m_LeftTextureFormatLabel;
	Gtk::Label* m_LeftTextureLoadStateLabel;
	Gtk::Box* m_LeftTexturePreviewBox;
	Gtk::Grid* m_LeftURLGrid;
	Gtk::Box* m_LeftURLBox;
	Gtk::TreeView* m_LeftURLTreeView;
	Gtk::Button* m_LeftURLAddButton;
	Gtk::Button* m_LeftURLRemoveButton;
	Gtk::Button* m_LeftURLReloadButton;
	Gtk::Label* m_RightTextureFormatLabel;
	Gtk::Label* m_RightTextureLoadStateLabel;
	Gtk::Box* m_RightTexturePreviewBox;
	Gtk::Grid* m_RightURLGrid;
	Gtk::Box* m_RightURLBox;
	Gtk::TreeView* m_RightURLTreeView;
	Gtk::Button* m_RightURLAddButton;
	Gtk::Button* m_RightURLRemoveButton;
	Gtk::Button* m_RightURLReloadButton;
	Gtk::Label* m_TopTextureFormatLabel;
	Gtk::Label* m_TopTextureLoadStateLabel;
	Gtk::Box* m_TopTexturePreviewBox;
	Gtk::Grid* m_TopURLGrid;
	Gtk::Box* m_TopURLBox;
	Gtk::TreeView* m_TopURLTreeView;
	Gtk::Button* m_TopURLAddButton;
	Gtk::Button* m_TopURLRemoveButton;
	Gtk::Button* m_TopURLReloadButton;
	Gtk::Label* m_BottomTextureFormatLabel;
	Gtk::Label* m_BottomTextureLoadStateLabel;
	Gtk::Box* m_BottomTexturePreviewBox;
	Gtk::Grid* m_BottomURLGrid;
	Gtk::Box* m_BottomURLBox;
	Gtk::TreeView* m_BottomURLTreeView;
	Gtk::Button* m_BottomURLAddButton;
	Gtk::Button* m_BottomURLRemoveButton;
	Gtk::Button* m_BottomURLReloadButton;
	Gtk::Popover* m_NewBackgroundPopover;
	Gtk::Button* m_NewBackgroundButton;
	Gtk::Button* m_NewTextureBackgroundButton;

};

} // puck
} // titania

#endif
