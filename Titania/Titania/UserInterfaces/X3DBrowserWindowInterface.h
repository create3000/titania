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
#ifndef __TMP_GLAD2CPP_BROWSER_WINDOW_H__
#define __TMP_GLAD2CPP_BROWSER_WINDOW_H__

#include "../Base/X3DUserInterface.h"
#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

using namespace Gtk;

class X3DBrowserWindowInterface :
	public X3DUserInterface
{
public:

	template <class ... Arguments>
	X3DBrowserWindowInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DUserInterface (m_widgetName, arguments ...),
		connections ()
	{ create (filename); }

	const std::string &
	getWidgetName () const { return m_widgetName; }

	const Glib::RefPtr <Gtk::Action> &
	getNewAction () const { return m_newAction; }

	const Glib::RefPtr <Gtk::Action> &
	getOpenAction () const { return m_openAction; }

	const Glib::RefPtr <Gtk::Action> &
	getRevertAction () const { return m_revertAction; }

	const Glib::RefPtr <Gtk::Action> &
	getSaveAction () const { return m_saveAction; }

	const Glib::RefPtr <Gtk::FileFilter> &
	getFileFilerX3D () const { return m_fileFilerX3D; }

	const Glib::RefPtr <Gtk::FileFilter> &
	getFileFilterAllFiles () const { return m_fileFilterAllFiles; }

	const Glib::RefPtr <Gtk::IconFactory> &
	getIconFactory () const { return m_iconFactory; }

	const Glib::RefPtr <Gtk::AccelGroup> &
	getMenuAccelGroup () const { return m_menuAccelGroup; }

	Gtk::FileChooserDialog &
	getFileOpenDialog () const { return *m_fileOpenDialog; }

	Gtk::FileChooserDialog &
	getFileSaveDialog () const { return *m_fileSaveDialog; }

	Gtk::CheckButton &
	getSaveCompressedButton () const { return *m_saveCompressedButton; }

	Gtk::MessageDialog &
	getMessageDialog () const { return *m_messageDialog; }

	Gtk::Window &
	getWindow () const { return *m_window; }

	Gtk::VBox &
	getWidget () const { return *m_widget; }

	Gtk::MenuBar &
	getMenuBar () const { return *m_menuBar; }

	Gtk::MenuItem &
	getFileMenuItem () const { return *m_fileMenuItem; }

	Gtk::ImageMenuItem &
	getNewMenuItem () const { return *m_newMenuItem; }

	Gtk::ImageMenuItem &
	getOpenMenuItem () const { return *m_openMenuItem; }

	Gtk::ImageMenuItem &
	getSaveMenuItem () const { return *m_saveMenuItem; }

	Gtk::ImageMenuItem &
	getSaveAsMenuItem () const { return *m_saveAsMenuItem; }

	Gtk::ImageMenuItem &
	getRevertMenuItem () const { return *m_revertMenuItem; }

	Gtk::ImageMenuItem &
	getQuitMenuItem () const { return *m_quitMenuItem; }

	Gtk::MenuItem &
	getEditMenuItem () const { return *m_editMenuItem; }

	Gtk::MenuItem &
	getViewMenuItem () const { return *m_viewMenuItem; }

	Gtk::CheckMenuItem &
	getToolBarMenuItem () const { return *m_toolBarMenuItem; }

	Gtk::CheckMenuItem &
	getSideBarMenuItem () const { return *m_sideBarMenuItem; }

	Gtk::CheckMenuItem &
	getFooterMenuItem () const { return *m_footerMenuItem; }

	Gtk::MenuItem &
	getShadingMenuItem () const { return *m_shadingMenuItem; }

	Gtk::RadioMenuItem &
	getPhongMenuItem () const { return *m_phongMenuItem; }

	Gtk::RadioMenuItem &
	getGouraudMenuItem () const { return *m_gouraudMenuItem; }

	Gtk::RadioMenuItem &
	getFlatMenuItem () const { return *m_flatMenuItem; }

	Gtk::RadioMenuItem &
	getWireFrameMenuItem () const { return *m_wireFrameMenuItem; }

	Gtk::RadioMenuItem &
	getPointSetMenuItem () const { return *m_pointSetMenuItem; }

	Gtk::MenuItem &
	getPrimitiveQualtityMenuItem () const { return *m_primitiveQualtityMenuItem; }

	Gtk::RadioMenuItem &
	getHighQualityMenuItem () const { return *m_highQualityMenuItem; }

	Gtk::RadioMenuItem &
	getMediumQualityMenuItem () const { return *m_mediumQualityMenuItem; }

	Gtk::RadioMenuItem &
	getLowQualityMenuItem () const { return *m_lowQualityMenuItem; }

	Gtk::CheckMenuItem &
	getRenderingPropertiesMenuItem () const { return *m_renderingPropertiesMenuItem; }

	Gtk::CheckMenuItem &
	getFullScreenMenuItem () const { return *m_fullScreenMenuItem; }

	Gtk::MenuItem &
	getNavigationMenuItem () const { return *m_navigationMenuItem; }

	Gtk::CheckMenuItem &
	getHeadlightMenuItem () const { return *m_headlightMenuItem; }

	Gtk::CheckMenuItem &
	getRubberbandMenuItem () const { return *m_rubberbandMenuItem; }

	Gtk::MenuItem &
	getLookAtAllMenuItem () const { return *m_lookAtAllMenuItem; }

	Gtk::CheckMenuItem &
	getEnableInlineViewpointsMenuItem () const { return *m_enableInlineViewpointsMenuItem; }

	Gtk::MenuItem &
	getToolsMenuItem () const { return *m_toolsMenuItem; }

	Gtk::MenuItem &
	getMotionBlurMenuItem () const { return *m_motionBlurMenuItem; }

	Gtk::MenuItem &
	getHelpMenuItem () const { return *m_helpMenuItem; }

	Gtk::ImageMenuItem &
	getInfoMenuItem () const { return *m_infoMenuItem; }

	Gtk::ImageMenuItem &
	getStandardSizeMenuItem () const { return *m_standardSizeMenuItem; }

	Gtk::Toolbar &
	getToolBar () const { return *m_toolBar; }

	Gtk::ToolButton &
	getNewButton () const { return *m_newButton; }

	Gtk::MenuToolButton &
	getOpenButton () const { return *m_openButton; }

	Gtk::ToolButton &
	getSaveButton () const { return *m_saveButton; }

	Gtk::ToolButton &
	getRefreshButton () const { return *m_refreshButton; }

	Gtk::Paned &
	getVPaned () const { return *m_vPaned; }

	Gtk::Paned &
	getHPaned () const { return *m_hPaned; }

	Gtk::HBox &
	getSurfaceBox () const { return *m_surfaceBox; }

	Gtk::RadioToolButton &
	getHandButton () const { return *m_handButton; }

	Gtk::RadioToolButton &
	getArrowButton () const { return *m_arrowButton; }

	Gtk::ToolButton &
	getLookAtAllButton () const { return *m_lookAtAllButton; }

	Gtk::ToggleToolButton &
	getLookAtButton () const { return *m_lookAtButton; }

	Gtk::Box &
	getFooter () const { return *m_footer; }

	Gtk::Button &
	getFooterCloseButton () const { return *m_footerCloseButton; }

	Gtk::Notebook &
	getFooterNotebook () const { return *m_footerNotebook; }

	Gtk::Box &
	getConsoleBox () const { return *m_consoleBox; }

	Gtk::TextView &
	getConsole () const { return *m_console; }

	Gtk::Box &
	getSideBar () const { return *m_sideBar; }

	Gtk::Button &
	getSideBarCloseButton () const { return *m_sideBarCloseButton; }

	Gtk::Notebook &
	getSideBarNotebook () const { return *m_sideBarNotebook; }

	Gtk::Box &
	getHistoryEditorBox () const { return *m_historyEditorBox; }

	Gtk::Box &
	getViewpointEditorBox () const { return *m_viewpointEditorBox; }

	Gtk::Box &
	getOutlineEditorBox () const { return *m_outlineEditorBox; }

	virtual
	void
	on_new () = 0;

	virtual
	void
	on_open () = 0;

	virtual
	void
	on_revert_to_saved () = 0;

	virtual
	void
	on_save () = 0;

	virtual
	void
	on_fileOpenDialog_response (int response_id) = 0;

	virtual
	void
	on_fileSaveDialog_response (int response_id) = 0;

	virtual
	void
	on_messageDialog_response (int response_id) = 0;

	virtual
	bool
	on_key_release_event (GdkEventKey* event) = 0;

	virtual
	bool
	on_key_press_event (GdkEventKey* event) = 0;

	virtual
	void
	on_save_as () = 0;

	virtual
	void
	on_close () = 0;

	virtual
	void
	on_toolBar_toggled () = 0;

	virtual
	void
	on_sideBar_toggled () = 0;

	virtual
	void
	on_footer_toggled () = 0;

	virtual
	void
	phong_activate () = 0;

	virtual
	void
	gouraud_activate () = 0;

	virtual
	void
	flat_activate () = 0;

	virtual
	void
	wireframe_activate () = 0;

	virtual
	void
	pointset_activate () = 0;

	virtual
	void
	on_high_quality_activate () = 0;

	virtual
	void
	on_medium_quality_activate () = 0;

	virtual
	void
	on_low_quality_activate () = 0;

	virtual
	void
	on_rendering_properties_toggled () = 0;

	virtual
	void
	on_fullscreen_toggled () = 0;

	virtual
	void
	on_headlight_toggled () = 0;

	virtual
	void
	on_rubberband_toggled () = 0;

	virtual
	void
	on_look_at_all_activate () = 0;

	virtual
	void
	on_enableInlineViewpoints_toggled () = 0;

	virtual
	void
	on_motion_blur_editor_activate () = 0;

	virtual
	void
	on_info () = 0;

	virtual
	void
	on_standard_size () = 0;

	virtual
	void
	on_hand_button_toggled () = 0;

	virtual
	void
	on_arrow_button_toggled () = 0;

	virtual
	void
	on_look_at_all_clicked () = 0;

	virtual
	void
	on_look_at_toggled () = 0;


private:

	void
	create (const std::string &);

	static const std::string m_widgetName;

	std::deque <sigc::connection>   connections;
	Glib::RefPtr <Gtk::Builder>     m_builder;
	Glib::RefPtr <Gtk::Action>      m_newAction;
	Glib::RefPtr <Gtk::Action>      m_openAction;
	Glib::RefPtr <Gtk::Action>      m_revertAction;
	Glib::RefPtr <Gtk::Action>      m_saveAction;
	Glib::RefPtr <Gtk::FileFilter>  m_fileFilerX3D;
	Glib::RefPtr <Gtk::FileFilter>  m_fileFilterAllFiles;
	Glib::RefPtr <Gtk::IconFactory> m_iconFactory;
	Glib::RefPtr <Gtk::AccelGroup>  m_menuAccelGroup;
	Gtk::FileChooserDialog*         m_fileOpenDialog;
	Gtk::FileChooserDialog*         m_fileSaveDialog;
	Gtk::CheckButton*               m_saveCompressedButton;
	Gtk::MessageDialog*             m_messageDialog;
	Gtk::Window*                    m_window;
	Gtk::VBox*                      m_widget;
	Gtk::MenuBar*                   m_menuBar;
	Gtk::MenuItem*                  m_fileMenuItem;
	Gtk::ImageMenuItem*             m_newMenuItem;
	Gtk::ImageMenuItem*             m_openMenuItem;
	Gtk::ImageMenuItem*             m_saveMenuItem;
	Gtk::ImageMenuItem*             m_saveAsMenuItem;
	Gtk::ImageMenuItem*             m_revertMenuItem;
	Gtk::ImageMenuItem*             m_quitMenuItem;
	Gtk::MenuItem*                  m_editMenuItem;
	Gtk::MenuItem*                  m_viewMenuItem;
	Gtk::CheckMenuItem*             m_toolBarMenuItem;
	Gtk::CheckMenuItem*             m_sideBarMenuItem;
	Gtk::CheckMenuItem*             m_footerMenuItem;
	Gtk::MenuItem*                  m_shadingMenuItem;
	Gtk::RadioMenuItem*             m_phongMenuItem;
	Gtk::RadioMenuItem*             m_gouraudMenuItem;
	Gtk::RadioMenuItem*             m_flatMenuItem;
	Gtk::RadioMenuItem*             m_wireFrameMenuItem;
	Gtk::RadioMenuItem*             m_pointSetMenuItem;
	Gtk::MenuItem*                  m_primitiveQualtityMenuItem;
	Gtk::RadioMenuItem*             m_highQualityMenuItem;
	Gtk::RadioMenuItem*             m_mediumQualityMenuItem;
	Gtk::RadioMenuItem*             m_lowQualityMenuItem;
	Gtk::CheckMenuItem*             m_renderingPropertiesMenuItem;
	Gtk::CheckMenuItem*             m_fullScreenMenuItem;
	Gtk::MenuItem*                  m_navigationMenuItem;
	Gtk::CheckMenuItem*             m_headlightMenuItem;
	Gtk::CheckMenuItem*             m_rubberbandMenuItem;
	Gtk::MenuItem*                  m_lookAtAllMenuItem;
	Gtk::CheckMenuItem*             m_enableInlineViewpointsMenuItem;
	Gtk::MenuItem*                  m_toolsMenuItem;
	Gtk::MenuItem*                  m_motionBlurMenuItem;
	Gtk::MenuItem*                  m_helpMenuItem;
	Gtk::ImageMenuItem*             m_infoMenuItem;
	Gtk::ImageMenuItem*             m_standardSizeMenuItem;
	Gtk::Toolbar*                   m_toolBar;
	Gtk::ToolButton*                m_newButton;
	Gtk::MenuToolButton*            m_openButton;
	Gtk::ToolButton*                m_saveButton;
	Gtk::ToolButton*                m_refreshButton;
	Gtk::Paned*                     m_vPaned;
	Gtk::Paned*                     m_hPaned;
	Gtk::HBox*                      m_surfaceBox;
	Gtk::RadioToolButton*           m_handButton;
	Gtk::RadioToolButton*           m_arrowButton;
	Gtk::ToolButton*                m_lookAtAllButton;
	Gtk::ToggleToolButton*          m_lookAtButton;
	Gtk::Box*                       m_footer;
	Gtk::Button*                    m_footerCloseButton;
	Gtk::Notebook*                  m_footerNotebook;
	Gtk::Box*                       m_consoleBox;
	Gtk::TextView*                  m_console;
	Gtk::Box*                       m_sideBar;
	Gtk::Button*                    m_sideBarCloseButton;
	Gtk::Notebook*                  m_sideBarNotebook;
	Gtk::Box*                       m_historyEditorBox;
	Gtk::Box*                       m_viewpointEditorBox;
	Gtk::Box*                       m_outlineEditorBox;

};

} // puck
} // titania

#endif
