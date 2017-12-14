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

#include "X3DBackgroundEditorInterface.h"

namespace titania {
namespace puck {

void
X3DBackgroundEditorInterface::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	create ();
}

void
X3DBackgroundEditorInterface::create (std::initializer_list <std::string> filenames)
{
	// Create Builder.
	m_builder = Gtk::Builder::create ();

	for (const auto & filename : filenames)
		m_builder -> add_from_file (filename);

	create ();
}

void
X3DBackgroundEditorInterface::create ()
{
	// Get objects.
	m_GroundAngleAdjustment       = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GroundAngleAdjustment"));
	m_GroundColorAdjustment       = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GroundColorAdjustment"));
	m_SkyAngleAdjustment          = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("SkyAngleAdjustment"));
	m_SkyColorAdjustment          = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("SkyColorAdjustment"));
	m_TransparencyAdjustment      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransparencyAdjustment"));
	m_FrontURLCellRendererText    = Glib::RefPtr <Gtk::CellRendererText>::cast_dynamic (m_builder -> get_object ("FrontURLCellRendererText"));
	m_FrontURLChooserColumn       = Glib::RefPtr <Gtk::TreeViewColumn>::cast_dynamic (m_builder -> get_object ("FrontURLChooserColumn"));
	m_FrontURLCellrendererPixbuf  = Glib::RefPtr <Gtk::CellRendererPixbuf>::cast_dynamic (m_builder -> get_object ("FrontURLCellrendererPixbuf"));
	m_BackURLCellRendererText     = Glib::RefPtr <Gtk::CellRendererText>::cast_dynamic (m_builder -> get_object ("BackURLCellRendererText"));
	m_BackURLChooserColumn        = Glib::RefPtr <Gtk::TreeViewColumn>::cast_dynamic (m_builder -> get_object ("BackURLChooserColumn"));
	m_BackURLCellrendererPixbuf   = Glib::RefPtr <Gtk::CellRendererPixbuf>::cast_dynamic (m_builder -> get_object ("BackURLCellrendererPixbuf"));
	m_LeftURLCellRendererText     = Glib::RefPtr <Gtk::CellRendererText>::cast_dynamic (m_builder -> get_object ("LeftURLCellRendererText"));
	m_LeftURLChooserColumn        = Glib::RefPtr <Gtk::TreeViewColumn>::cast_dynamic (m_builder -> get_object ("LeftURLChooserColumn"));
	m_LeftURLCellrendererPixbuf   = Glib::RefPtr <Gtk::CellRendererPixbuf>::cast_dynamic (m_builder -> get_object ("LeftURLCellrendererPixbuf"));
	m_RightURLCellRendererText    = Glib::RefPtr <Gtk::CellRendererText>::cast_dynamic (m_builder -> get_object ("RightURLCellRendererText"));
	m_RightURLChooserColumn       = Glib::RefPtr <Gtk::TreeViewColumn>::cast_dynamic (m_builder -> get_object ("RightURLChooserColumn"));
	m_RightURLCellrendererPixbuf  = Glib::RefPtr <Gtk::CellRendererPixbuf>::cast_dynamic (m_builder -> get_object ("RightURLCellrendererPixbuf"));
	m_TopURLCellRendererText      = Glib::RefPtr <Gtk::CellRendererText>::cast_dynamic (m_builder -> get_object ("TopURLCellRendererText"));
	m_TopURLChooserColumn         = Glib::RefPtr <Gtk::TreeViewColumn>::cast_dynamic (m_builder -> get_object ("TopURLChooserColumn"));
	m_TopURLCellrendererPixbuf    = Glib::RefPtr <Gtk::CellRendererPixbuf>::cast_dynamic (m_builder -> get_object ("TopURLCellrendererPixbuf"));
	m_BottomURLCellRendererText   = Glib::RefPtr <Gtk::CellRendererText>::cast_dynamic (m_builder -> get_object ("BottomURLCellRendererText"));
	m_BottomURLChooserColumn      = Glib::RefPtr <Gtk::TreeViewColumn>::cast_dynamic (m_builder -> get_object ("BottomURLChooserColumn"));
	m_BottomURLCellrendererPixbuf = Glib::RefPtr <Gtk::CellRendererPixbuf>::cast_dynamic (m_builder -> get_object ("BottomURLCellrendererPixbuf"));

	// Get widgets.
	m_builder -> get_widget ("Window", m_Window);
	m_builder -> get_widget ("Widget", m_Widget);
	m_builder -> get_widget ("Paned", m_Paned);
	m_builder -> get_widget ("BackgroundListBox", m_BackgroundListBox);
	m_builder -> get_widget ("HeaderBar", m_HeaderBar);
	m_builder -> get_widget ("ActionBox", m_ActionBox);
	m_builder -> get_widget ("NewBackgroundPopupButton", m_NewBackgroundPopupButton);
	m_builder -> get_widget ("RemoveBackgroundButton", m_RemoveBackgroundButton);
	m_builder -> get_widget ("BackgroundExpander", m_BackgroundExpander);
	m_builder -> get_widget ("BackgroundBox", m_BackgroundBox);
	m_builder -> get_widget ("NameBox", m_NameBox);
	m_builder -> get_widget ("NameEntry", m_NameEntry);
	m_builder -> get_widget ("RenameButton", m_RenameButton);
	m_builder -> get_widget ("ColorsExpander", m_ColorsExpander);
	m_builder -> get_widget ("SkyBox", m_SkyBox);
	m_builder -> get_widget ("SkyAngleSpinButton", m_SkyAngleSpinButton);
	m_builder -> get_widget ("SkyGradientBox", m_SkyGradientBox);
	m_builder -> get_widget ("SkyColorGrid", m_SkyColorGrid);
	m_builder -> get_widget ("SkyColorsScrolledWindow", m_SkyColorsScrolledWindow);
	m_builder -> get_widget ("AddSkyColorButton", m_AddSkyColorButton);
	m_builder -> get_widget ("RemoveSkyColorButton", m_RemoveSkyColorButton);
	m_builder -> get_widget ("SkyColorBox", m_SkyColorBox);
	m_builder -> get_widget ("SkyColorButton", m_SkyColorButton);
	m_builder -> get_widget ("GroundBox", m_GroundBox);
	m_builder -> get_widget ("GroundAngleSpinButton", m_GroundAngleSpinButton);
	m_builder -> get_widget ("GroundGradientBox", m_GroundGradientBox);
	m_builder -> get_widget ("GroundColorGrid", m_GroundColorGrid);
	m_builder -> get_widget ("GroundColorsScrolledWindow", m_GroundColorsScrolledWindow);
	m_builder -> get_widget ("AddGroundColorButton", m_AddGroundColorButton);
	m_builder -> get_widget ("RemoveGroundColorButton", m_RemoveGroundColorButton);
	m_builder -> get_widget ("GroundColorBox", m_GroundColorBox);
	m_builder -> get_widget ("GroundColorButton", m_GroundColorButton);
	m_builder -> get_widget ("TransparencyScale", m_TransparencyScale);
	m_builder -> get_widget ("TextureExpander", m_TextureExpander);
	m_builder -> get_widget ("TexturesNotebook", m_TexturesNotebook);
	m_builder -> get_widget ("FrontTextureFormatLabel", m_FrontTextureFormatLabel);
	m_builder -> get_widget ("FrontTextureLoadStateLabel", m_FrontTextureLoadStateLabel);
	m_builder -> get_widget ("FrontTexturePreviewBox", m_FrontTexturePreviewBox);
	m_builder -> get_widget ("FrontURLGrid", m_FrontURLGrid);
	m_builder -> get_widget ("FrontURLBox", m_FrontURLBox);
	m_builder -> get_widget ("FrontURLTreeView", m_FrontURLTreeView);
	m_builder -> get_widget ("FrontURLAddButton", m_FrontURLAddButton);
	m_builder -> get_widget ("FrontURLRemoveButton", m_FrontURLRemoveButton);
	m_builder -> get_widget ("FrontURLReloadButton", m_FrontURLReloadButton);
	m_builder -> get_widget ("BackTextureFormatLabel", m_BackTextureFormatLabel);
	m_builder -> get_widget ("BackTextureLoadStateLabel", m_BackTextureLoadStateLabel);
	m_builder -> get_widget ("BackTexturePreviewBox", m_BackTexturePreviewBox);
	m_builder -> get_widget ("BackURLGrid", m_BackURLGrid);
	m_builder -> get_widget ("BackURLBox", m_BackURLBox);
	m_builder -> get_widget ("BackURLTreeView", m_BackURLTreeView);
	m_builder -> get_widget ("BackURLAddButton", m_BackURLAddButton);
	m_builder -> get_widget ("BackURLRemoveButton", m_BackURLRemoveButton);
	m_builder -> get_widget ("BackURLReloadButton", m_BackURLReloadButton);
	m_builder -> get_widget ("LeftTextureFormatLabel", m_LeftTextureFormatLabel);
	m_builder -> get_widget ("LeftTextureLoadStateLabel", m_LeftTextureLoadStateLabel);
	m_builder -> get_widget ("LeftTexturePreviewBox", m_LeftTexturePreviewBox);
	m_builder -> get_widget ("LeftURLGrid", m_LeftURLGrid);
	m_builder -> get_widget ("LeftURLBox", m_LeftURLBox);
	m_builder -> get_widget ("LeftURLTreeView", m_LeftURLTreeView);
	m_builder -> get_widget ("LeftURLAddButton", m_LeftURLAddButton);
	m_builder -> get_widget ("LeftURLRemoveButton", m_LeftURLRemoveButton);
	m_builder -> get_widget ("LeftURLReloadButton", m_LeftURLReloadButton);
	m_builder -> get_widget ("RightTextureFormatLabel", m_RightTextureFormatLabel);
	m_builder -> get_widget ("RightTextureLoadStateLabel", m_RightTextureLoadStateLabel);
	m_builder -> get_widget ("RightTexturePreviewBox", m_RightTexturePreviewBox);
	m_builder -> get_widget ("RightURLGrid", m_RightURLGrid);
	m_builder -> get_widget ("RightURLBox", m_RightURLBox);
	m_builder -> get_widget ("RightURLTreeView", m_RightURLTreeView);
	m_builder -> get_widget ("RightURLAddButton", m_RightURLAddButton);
	m_builder -> get_widget ("RightURLRemoveButton", m_RightURLRemoveButton);
	m_builder -> get_widget ("RightURLReloadButton", m_RightURLReloadButton);
	m_builder -> get_widget ("TopTextureFormatLabel", m_TopTextureFormatLabel);
	m_builder -> get_widget ("TopTextureLoadStateLabel", m_TopTextureLoadStateLabel);
	m_builder -> get_widget ("TopTexturePreviewBox", m_TopTexturePreviewBox);
	m_builder -> get_widget ("TopURLGrid", m_TopURLGrid);
	m_builder -> get_widget ("TopURLBox", m_TopURLBox);
	m_builder -> get_widget ("TopURLTreeView", m_TopURLTreeView);
	m_builder -> get_widget ("TopURLAddButton", m_TopURLAddButton);
	m_builder -> get_widget ("TopURLRemoveButton", m_TopURLRemoveButton);
	m_builder -> get_widget ("TopURLReloadButton", m_TopURLReloadButton);
	m_builder -> get_widget ("BottomTextureFormatLabel", m_BottomTextureFormatLabel);
	m_builder -> get_widget ("BottomTextureLoadStateLabel", m_BottomTextureLoadStateLabel);
	m_builder -> get_widget ("BottomTexturePreviewBox", m_BottomTexturePreviewBox);
	m_builder -> get_widget ("BottomURLGrid", m_BottomURLGrid);
	m_builder -> get_widget ("BottomURLBox", m_BottomURLBox);
	m_builder -> get_widget ("BottomURLTreeView", m_BottomURLTreeView);
	m_builder -> get_widget ("BottomURLAddButton", m_BottomURLAddButton);
	m_builder -> get_widget ("BottomURLRemoveButton", m_BottomURLRemoveButton);
	m_builder -> get_widget ("BottomURLReloadButton", m_BottomURLReloadButton);
	m_builder -> get_widget ("NewBackgroundPopover", m_NewBackgroundPopover);
	m_builder -> get_widget ("NewBackgroundButton", m_NewBackgroundButton);
	m_builder -> get_widget ("NewTextureBackgroundButton", m_NewTextureBackgroundButton);
	m_NewBackgroundPopupButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DBackgroundEditorInterface::on_new_background_popup_clicked));
	m_RemoveBackgroundButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DBackgroundEditorInterface::on_remove_background_clicked));
	m_NewBackgroundButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DBackgroundEditorInterface::on_new_background_clicked));
	m_NewTextureBackgroundButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DBackgroundEditorInterface::on_new_texture_background_clicked));
}

X3DBackgroundEditorInterface::~X3DBackgroundEditorInterface ()
{
	delete m_Window;
}

} // puck
} // titania
