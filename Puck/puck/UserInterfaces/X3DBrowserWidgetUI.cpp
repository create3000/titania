/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*- */
/*******************************************************************************
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
 ******************************************************************************/

#include "X3DBrowserWidgetUI.h"

namespace titania {
namespace puck {

const std::string X3DBrowserWidgetUI::m_widgetName = "BrowserWidget";

void
X3DBrowserWidgetUI::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	// Get objects.
	m_fileFilerX3D       = Glib::RefPtr <Gtk::FileFilter>::cast_dynamic (m_builder -> get_object ("FileFilerX3D"));
	m_fileFilterAllFiles = Glib::RefPtr <Gtk::FileFilter>::cast_dynamic (m_builder -> get_object ("FileFilterAllFiles"));
	m_footerAction       = Glib::RefPtr <Gtk::ToggleAction>::cast_dynamic (m_builder -> get_object ("FooterAction"));
	m_iconFactory        = Glib::RefPtr <Gtk::IconFactory>::cast_dynamic (m_builder -> get_object ("IconFactory"));
	m_sideBarAction      = Glib::RefPtr <Gtk::ToggleAction>::cast_dynamic (m_builder -> get_object ("SideBarAction"));
	m_menuAccelGroup     = Glib::RefPtr <Gtk::AccelGroup>::cast_dynamic (m_builder -> get_object ("MenuAccelGroup"));

	// Get widgets.
	m_builder -> get_widget ("FileOpenDialog", m_fileOpenDialog);
	m_builder -> get_widget ("FileSaveDialog", m_fileSaveDialog);
	m_builder -> get_widget ("SaveCompressedButton", m_saveCompressedButton);
	m_builder -> get_widget ("MessageDialog", m_messageDialog);
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
	m_builder -> get_widget ("ToolBarMenuItem", m_toolBarMenuItem);
	m_builder -> get_widget ("NavigationBarMenuItem", m_navigationBarMenuItem);
	m_builder -> get_widget ("SideBarMenuItem", m_sideBarMenuItem);
	m_builder -> get_widget ("FooterMenuItem", m_footerMenuItem);
	m_builder -> get_widget ("StatusBarMenuItem", m_statusBarMenuItem);
	m_builder -> get_widget ("ShadingMenuItem", m_shadingMenuItem);
	m_builder -> get_widget ("GouraudMenuItem", m_gouraudMenuItem);
	m_builder -> get_widget ("WireFrameMenuItem", m_wireFrameMenuItem);
	m_builder -> get_widget ("PointSetMenuItem", m_pointSetMenuItem);
	m_builder -> get_widget ("RenderQualtityMenuItem", m_renderQualtityMenuItem);
	m_builder -> get_widget ("LowQualityMenuItem", m_lowQualityMenuItem);
	m_builder -> get_widget ("MediumQualityMenuItem", m_mediumQualityMenuItem);
	m_builder -> get_widget ("HighQualityMenuItem", m_highQualityMenuItem);
	m_builder -> get_widget ("RenderingPropertiesMenuItem", m_renderingPropertiesMenuItem);
	m_builder -> get_widget ("FullScreenMenuItem", m_fullScreenMenuItem);
	m_builder -> get_widget ("NavigationMenuItem", m_navigationMenuItem);
	m_builder -> get_widget ("HeadlightMenuItem", m_headlightMenuItem);
	m_builder -> get_widget ("ShowAllMenuItem", m_showAllMenuItem);
	m_builder -> get_widget ("ViewpointsMenuItem", m_viewpointsMenuItem);
	m_builder -> get_widget ("HistoryMenuItem", m_historyMenuItem);
	m_builder -> get_widget ("ToolsMenuItem", m_toolsMenuItem);
	m_builder -> get_widget ("OutlineEditorMenuItem", m_outlineEditorMenuItem);
	m_builder -> get_widget ("ViewpointEditorMenuItem", m_viewpointEditorMenuItem);
	m_builder -> get_widget ("MotionBlurMenuItem", m_motionBlurMenuItem);
	m_builder -> get_widget ("ConsoleMenuItem", m_consoleMenuItem);
	m_builder -> get_widget ("HelpMenuItem", m_helpMenuItem);
	m_builder -> get_widget ("Toolbar", m_toolbar);
	m_builder -> get_widget ("NewButton", m_newButton);
	m_builder -> get_widget ("OpenButton", m_openButton);
	m_builder -> get_widget ("SaveButton", m_saveButton);
	m_builder -> get_widget ("Fullscreen", m_fullscreen);
	m_builder -> get_widget ("OutlineEditorButton", m_outlineEditorButton);
	m_builder -> get_widget ("ViewpointEditorButton", m_viewpointEditorButton);
	m_builder -> get_widget ("NavigationBar", m_navigationBar);
	m_builder -> get_widget ("ForwardButton", m_forwardButton);
	m_builder -> get_widget ("BackButton", m_backButton);
	m_builder -> get_widget ("ReloadButton", m_reloadButton);
	m_builder -> get_widget ("HomeButton", m_homeButton);
	m_builder -> get_widget ("LocationEntry", m_locationEntry);
	m_builder -> get_widget ("DrawingAreaBox", m_drawingAreaBox);
	m_builder -> get_widget ("ArrowButton", m_arrowButton);
	m_builder -> get_widget ("HandButton", m_handButton);
	m_builder -> get_widget ("ShowAllButton", m_showAllButton);
	m_builder -> get_widget ("Footer", m_footer);
	m_builder -> get_widget ("FooterCloseButton", m_footerCloseButton);
	m_builder -> get_widget ("FooterNotebook", m_footerNotebook);
	m_builder -> get_widget ("ConsoleBox", m_consoleBox);
	m_builder -> get_widget ("SideBar", m_sideBar);
	m_builder -> get_widget ("SideBarCloseButton", m_sideBarCloseButton);
	m_builder -> get_widget ("SideBarNotebook", m_sideBarNotebook);
	m_builder -> get_widget ("HistoryEditorBox", m_historyEditorBox);
	m_builder -> get_widget ("ViewpointEditorBox", m_viewpointEditorBox);
	m_builder -> get_widget ("OutlineEditorBox", m_outlineEditorBox);
	m_builder -> get_widget ("StatusBar", m_statusBar);

	// Connect object Gtk::ToggleAction with id 'FooterAction'.
	m_footerAction -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWidgetUI::on_footer_toggled));
	m_sideBarAction -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWidgetUI::on_sideBar_toggled));

	// Connect object Gtk::FileChooserDialog with id 'FileOpenDialog'.
	m_fileOpenDialog -> signal_response () .connect (sigc::mem_fun (*this, &X3DBrowserWidgetUI::on_fileOpenDialog_response));
	m_fileSaveDialog -> signal_response () .connect (sigc::mem_fun (*this, &X3DBrowserWidgetUI::on_fileSaveDialog_response));

	// Connect object Gtk::MessageDialog with id 'MessageDialog'.
	m_messageDialog -> signal_response () .connect (sigc::mem_fun (*this, &X3DBrowserWidgetUI::messageDialogResponse));

	// Connect object Gtk::VBox with id 'Widget'.
	m_widget -> signal_unmap () .connect (sigc::mem_fun (*this, &X3DBrowserWidgetUI::on_unmap));
	m_widget -> signal_map () .connect (sigc::mem_fun (*this, &X3DBrowserWidgetUI::on_map));

	// Connect object Gtk::ImageMenuItem with id 'NewMenuItem'.
	m_newMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWidgetUI::on_new));
	m_openMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWidgetUI::on_open));
	m_saveMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWidgetUI::on_save));
	m_saveAsMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWidgetUI::on_save_as));
	m_revertMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWidgetUI::on_revert_to_saved));
	m_quitMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWidgetUI::on_close));

	// Connect object Gtk::CheckMenuItem with id 'ToolBarMenuItem'.
	m_toolBarMenuItem -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWidgetUI::on_toolBar_toggled));
	m_navigationBarMenuItem -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWidgetUI::on_navigationBar_toggled));
	m_statusBarMenuItem -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWidgetUI::on_statusBar_toggled));

	// Connect object Gtk::RadioMenuItem with id 'GouraudMenuItem'.
	m_gouraudMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWidgetUI::gouraud_activate));
	m_wireFrameMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWidgetUI::wireframe_activate));
	m_pointSetMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWidgetUI::pointset_activate));
	m_lowQualityMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWidgetUI::on_low_quality_activate));
	m_mediumQualityMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWidgetUI::on_medium_quality_activate));
	m_highQualityMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWidgetUI::on_high_quality_activate));

	// Connect object Gtk::CheckMenuItem with id 'RenderingPropertiesMenuItem'.
	m_renderingPropertiesMenuItem -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWidgetUI::on_rendering_properties_toggled));
	m_fullScreenMenuItem -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWidgetUI::on_fullscreen_toggled));
	m_headlightMenuItem -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWidgetUI::on_headlight_toggled));

	// Connect object Gtk::MenuItem with id 'ShowAllMenuItem'.
	m_showAllMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWidgetUI::on_show_all_toggled));
	m_outlineEditorMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWidgetUI::on_outline_editor_activate));
	m_viewpointEditorMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWidgetUI::on_viewpoint_editor_activate));
	m_motionBlurMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWidgetUI::on_motion_blur_editor_activate));

	// Connect object Gtk::ToolButton with id 'NewButton'.
	m_newButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWidgetUI::on_new));
	m_openButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWidgetUI::on_open));
	m_saveButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWidgetUI::on_save));
	m_outlineEditorButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWidgetUI::on_outline_editor_activate));
	m_viewpointEditorButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWidgetUI::on_viewpoint_editor_activate));
	m_reloadButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWidgetUI::on_reload));
	m_homeButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWidgetUI::on_home));

	// Connect object Gtk::Entry with id 'LocationEntry'.
	m_locationEntry -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWidgetUI::on_locationEntry_activate));
	m_locationEntry -> signal_icon_release () .connect (sigc::mem_fun (*this, &X3DBrowserWidgetUI::on_locationEntryIcon_activate));

	// Connect object Gtk::RadioToolButton with id 'ArrowButton'.
	m_arrowButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWidgetUI::on_arrow_button_toggled));
	m_handButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWidgetUI::on_hand_button_toggled));

	// Connect object Gtk::ToolButton with id 'ShowAllButton'.
	m_showAllButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWidgetUI::on_show_all_toggled));

	// Call construct handler of base class.
	construct ();
}

} // puck
} // titania
