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

#include "X3DBrowserWindowUI.h"

namespace titania {
namespace puck {

const std::string X3DBrowserWindowUI::m_widgetName = "BrowserWindow";

void
X3DBrowserWindowUI::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	// Get objects.
	m_fileFilerX3D       = Glib::RefPtr <Gtk::FileFilter>::cast_dynamic (m_builder -> get_object ("FileFilerX3D"));
	m_fileFilterAllFiles = Glib::RefPtr <Gtk::FileFilter>::cast_dynamic (m_builder -> get_object ("FileFilterAllFiles"));
	m_iconFactory        = Glib::RefPtr <Gtk::IconFactory>::cast_dynamic (m_builder -> get_object ("IconFactory"));
	m_menuAccelGroup     = Glib::RefPtr <Gtk::AccelGroup>::cast_dynamic (m_builder -> get_object ("MenuAccelGroup"));

	// Get widgets.
	m_builder -> get_widget ("FileOpenDialog", m_fileOpenDialog);
	m_builder -> get_widget ("FileSaveDialog", m_fileSaveDialog);
	m_builder -> get_widget ("SaveCompressedButton", m_saveCompressedButton);
	m_builder -> get_widget ("Window", m_window);
	m_builder -> get_widget ("Widget", m_widget);
	m_builder -> get_widget ("MenuBar", m_menuBar);
	m_builder -> get_widget ("FileMenuItem", m_fileMenuItem);
	m_builder -> get_widget ("NewMenuItem", m_newMenuItem);
	m_builder -> get_widget ("OpenMenuItem", m_openMenuItem);
	m_builder -> get_widget ("SaveMenuItem", m_saveMenuItem);
	m_builder -> get_widget ("SaveAsMenuItem", m_saveAsMenuItem);
	m_builder -> get_widget ("RevertMenuItem", m_revertMenuItem);
	m_builder -> get_widget ("QuitMenuItem", m_quitMenuItem);
	m_builder -> get_widget ("EditMenuItem", m_editMenuItem);
	m_builder -> get_widget ("ViewMenuItem", m_viewMenuItem);
	m_builder -> get_widget ("NavigationBarMenuItem", m_navigationBarMenuItem);
	m_builder -> get_widget ("ToolBarMenuItem", m_toolBarMenuItem);
	m_builder -> get_widget ("SideBarMenuItem", m_sideBarMenuItem);
	m_builder -> get_widget ("FooterMenuItem", m_footerMenuItem);
	m_builder -> get_widget ("StatusBarMenuItem", m_statusBarMenuItem);
	m_builder -> get_widget ("ShadingMenuItem", m_shadingMenuItem);
	m_builder -> get_widget ("PhongMenuItem", m_phongMenuItem);
	m_builder -> get_widget ("GouraudMenuItem", m_gouraudMenuItem);
	m_builder -> get_widget ("FlatMenuItem", m_flatMenuItem);
	m_builder -> get_widget ("WireFrameMenuItem", m_wireFrameMenuItem);
	m_builder -> get_widget ("PointSetMenuItem", m_pointSetMenuItem);
	m_builder -> get_widget ("PrimitiveQualtityMenuItem", m_primitiveQualtityMenuItem);
	m_builder -> get_widget ("HighQualityMenuItem", m_highQualityMenuItem);
	m_builder -> get_widget ("MediumQualityMenuItem", m_mediumQualityMenuItem);
	m_builder -> get_widget ("LowQualityMenuItem", m_lowQualityMenuItem);
	m_builder -> get_widget ("RenderingPropertiesMenuItem", m_renderingPropertiesMenuItem);
	m_builder -> get_widget ("FullScreenMenuItem", m_fullScreenMenuItem);
	m_builder -> get_widget ("NavigationMenuItem", m_navigationMenuItem);
	m_builder -> get_widget ("HeadlightMenuItem", m_headlightMenuItem);
	m_builder -> get_widget ("RubberbandMenuItem", m_rubberbandMenuItem);
	m_builder -> get_widget ("LookAtAllMenuItem", m_lookAtAllMenuItem);
	m_builder -> get_widget ("EnableInlineViewpointsMenuItem", m_enableInlineViewpointsMenuItem);
	m_builder -> get_widget ("ViewpointsMenuItem", m_viewpointsMenuItem);
	m_builder -> get_widget ("HistoryMenuItem", m_historyMenuItem);
	m_builder -> get_widget ("ToolsMenuItem", m_toolsMenuItem);
	m_builder -> get_widget ("OutlineEditorMenuItem", m_outlineEditorMenuItem);
	m_builder -> get_widget ("ViewpointEditorMenuItem", m_viewpointEditorMenuItem);
	m_builder -> get_widget ("MotionBlurMenuItem", m_motionBlurMenuItem);
	m_builder -> get_widget ("ConsoleMenuItem", m_consoleMenuItem);
	m_builder -> get_widget ("HelpMenuItem", m_helpMenuItem);
	m_builder -> get_widget ("InfoMenuItem", m_infoMenuItem);
	m_builder -> get_widget ("StandardSizeMenuItem", m_standardSizeMenuItem);
	m_builder -> get_widget ("Notebook", m_notebook);
	m_builder -> get_widget ("AddTabLabel", m_addTabLabel);
	m_builder -> get_widget ("AddTabButton", m_addTabButton);

	// Connect object Gtk::FileChooserDialog with id 'FileOpenDialog'.
	connections .emplace_back (m_fileOpenDialog -> signal_response () .connect (sigc::mem_fun (*this, &X3DBrowserWindowUI::on_fileOpenDialog_response)));
	connections .emplace_back (m_fileSaveDialog -> signal_response () .connect (sigc::mem_fun (*this, &X3DBrowserWindowUI::on_fileSaveDialog_response)));

	// Connect object Gtk::ImageMenuItem with id 'NewMenuItem'.
	connections .emplace_back (m_newMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowUI::on_new)));
	connections .emplace_back (m_openMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowUI::on_open)));
	connections .emplace_back (m_saveMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowUI::on_save)));
	connections .emplace_back (m_saveAsMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowUI::on_save_as)));
	connections .emplace_back (m_revertMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowUI::on_revert_to_saved)));
	connections .emplace_back (m_quitMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowUI::on_close)));

	// Connect object Gtk::CheckMenuItem with id 'NavigationBarMenuItem'.
	connections .emplace_back (m_navigationBarMenuItem -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowUI::on_navigationBar_toggled)));
	connections .emplace_back (m_toolBarMenuItem -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowUI::on_toolBar_toggled)));
	connections .emplace_back (m_sideBarMenuItem -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowUI::on_sideBar_toggled)));
	connections .emplace_back (m_footerMenuItem -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowUI::on_footer_toggled)));
	connections .emplace_back (m_statusBarMenuItem -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowUI::on_statusBar_toggled)));

	// Connect object Gtk::RadioMenuItem with id 'PhongMenuItem'.
	connections .emplace_back (m_phongMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowUI::phong_activate)));
	connections .emplace_back (m_gouraudMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowUI::gouraud_activate)));
	connections .emplace_back (m_flatMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowUI::flat_activate)));
	connections .emplace_back (m_wireFrameMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowUI::wireframe_activate)));
	connections .emplace_back (m_pointSetMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowUI::pointset_activate)));
	connections .emplace_back (m_highQualityMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowUI::on_high_quality_activate)));
	connections .emplace_back (m_mediumQualityMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowUI::on_medium_quality_activate)));
	connections .emplace_back (m_lowQualityMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowUI::on_low_quality_activate)));

	// Connect object Gtk::CheckMenuItem with id 'RenderingPropertiesMenuItem'.
	connections .emplace_back (m_renderingPropertiesMenuItem -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowUI::on_rendering_properties_toggled)));
	connections .emplace_back (m_fullScreenMenuItem -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowUI::on_fullscreen_toggled)));
	connections .emplace_back (m_headlightMenuItem -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowUI::on_headlight_toggled)));
	connections .emplace_back (m_rubberbandMenuItem -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowUI::on_rubberband_toggled)));

	// Connect object Gtk::MenuItem with id 'LookAtAllMenuItem'.
	connections .emplace_back (m_lookAtAllMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowUI::on_look_at_all_activate)));

	// Connect object Gtk::CheckMenuItem with id 'EnableInlineViewpointsMenuItem'.
	connections .emplace_back (m_enableInlineViewpointsMenuItem -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowUI::on_enableInlineViewpoints_toggled)));

	// Connect object Gtk::MenuItem with id 'OutlineEditorMenuItem'.
	connections .emplace_back (m_outlineEditorMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowUI::on_outline_editor_activate)));
	connections .emplace_back (m_viewpointEditorMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowUI::on_viewpoint_editor_activate)));
	connections .emplace_back (m_motionBlurMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowUI::on_motion_blur_editor_activate)));

	// Connect object Gtk::ImageMenuItem with id 'InfoMenuItem'.
	connections .emplace_back (m_infoMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowUI::on_info)));
	connections .emplace_back (m_standardSizeMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowUI::on_standard_size)));

	// Connect object Gtk::Notebook with id 'Notebook'.
	connections .emplace_back (m_notebook -> signal_switch_page () .connect (sigc::mem_fun (*this, &X3DBrowserWindowUI::on_switch_page)));

	// Connect object Gtk::Button with id 'AddTabButton'.
	connections .emplace_back (m_addTabButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowUI::on_add_tab)));

	// Call construct handler of base class.
	construct ();
}

void
X3DBrowserWindowUI::dispose ()
{
	for (auto & connection : connections)
		connection .disconnect ();
}

} // puck
} // titania
