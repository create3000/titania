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
#ifndef __TMP_GLAD2CPP_VIEWPOINT_EDITOR_H__
#define __TMP_GLAD2CPP_VIEWPOINT_EDITOR_H__

#include "../Base/X3DEditorInterface.h"
#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

class X3DViewpointEditorInterface :
	public X3DEditorInterface
{
public:

	X3DViewpointEditorInterface () :
		X3DEditorInterface ()
	{ }

	template <class ... Arguments>
	X3DViewpointEditorInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DEditorInterface (m_widgetName, arguments ...),
		          filename (filename)
	{ create (filename); }

	const Glib::RefPtr <Gtk::Builder> &
	getBuilder () const
	{ return m_builder; }

	virtual
	const std::string &
	getWidgetName () const
	{ return m_widgetName; }

	template <class Type>
	Type*
	createWidget (const std::string & name) const
	{
		getBuilder () -> add_from_file (filename, name);

		Type* widget = nullptr;
		m_builder -> get_widget (name, widget);
		return widget;
	}

	const Glib::RefPtr <Gtk::Adjustment> &
	getGeoViewpointCenterOfRotationXAdjustment () const
	{ return m_GeoViewpointCenterOfRotationXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGeoViewpointCenterOfRotationYAdjustment () const
	{ return m_GeoViewpointCenterOfRotationYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGeoViewpointCenterOfRotationZAdjustment () const
	{ return m_GeoViewpointCenterOfRotationZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGeoViewpointFieldOfViewAdjustment () const
	{ return m_GeoViewpointFieldOfViewAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGeoViewpointOrientationAAdjustment () const
	{ return m_GeoViewpointOrientationAAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGeoViewpointOrientationXAdjustment () const
	{ return m_GeoViewpointOrientationXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGeoViewpointOrientationYAdjustment () const
	{ return m_GeoViewpointOrientationYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGeoViewpointOrientationZAdjustment () const
	{ return m_GeoViewpointOrientationZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGeoViewpointPositionXAdjustment () const
	{ return m_GeoViewpointPositionXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGeoViewpointPositionYAdjustment () const
	{ return m_GeoViewpointPositionYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGeoViewpointPositionZAdjustment () const
	{ return m_GeoViewpointPositionZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGeoViewpointSpeedFactorAdjustment () const
	{ return m_GeoViewpointSpeedFactorAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGeoViewpointZoneAdjustment () const
	{ return m_GeoViewpointZoneAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getOrthoViewpointCenterOfRotationXAdjustment () const
	{ return m_OrthoViewpointCenterOfRotationXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getOrthoViewpointCenterOfRotationYAdjustment () const
	{ return m_OrthoViewpointCenterOfRotationYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getOrthoViewpointCenterOfRotationZAdjustment () const
	{ return m_OrthoViewpointCenterOfRotationZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getOrthoViewpointFieldOfViewMaxXAdjustment () const
	{ return m_OrthoViewpointFieldOfViewMaxXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getOrthoViewpointFieldOfViewMaxYAdjustment () const
	{ return m_OrthoViewpointFieldOfViewMaxYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getOrthoViewpointFieldOfViewMinXAdjustment () const
	{ return m_OrthoViewpointFieldOfViewMinXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getOrthoViewpointFieldOfViewMinYAdjustment () const
	{ return m_OrthoViewpointFieldOfViewMinYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getOrthoViewpointOrientationAAdjustment () const
	{ return m_OrthoViewpointOrientationAAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getOrthoViewpointOrientationXAdjustment () const
	{ return m_OrthoViewpointOrientationXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getOrthoViewpointOrientationYAdjustment () const
	{ return m_OrthoViewpointOrientationYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getOrthoViewpointOrientationZAdjustment () const
	{ return m_OrthoViewpointOrientationZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getOrthoViewpointPositionXAdjustment () const
	{ return m_OrthoViewpointPositionXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getOrthoViewpointPositionYAdjustment () const
	{ return m_OrthoViewpointPositionYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getOrthoViewpointPositionZAdjustment () const
	{ return m_OrthoViewpointPositionZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getPerspectiveViewpointCenterOfRotationXAdjustment () const
	{ return m_PerspectiveViewpointCenterOfRotationXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getPerspectiveViewpointCenterOfRotationYAdjustment () const
	{ return m_PerspectiveViewpointCenterOfRotationYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getPerspectiveViewpointCenterOfRotationZAdjustment () const
	{ return m_PerspectiveViewpointCenterOfRotationZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getPerspectiveViewpointFieldOfViewAdjustment () const
	{ return m_PerspectiveViewpointFieldOfViewAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getPerspectiveViewpointOrientationAAdjustment () const
	{ return m_PerspectiveViewpointOrientationAAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getPerspectiveViewpointOrientationXAdjustment () const
	{ return m_PerspectiveViewpointOrientationXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getPerspectiveViewpointOrientationYAdjustment () const
	{ return m_PerspectiveViewpointOrientationYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getPerspectiveViewpointOrientationZAdjustment () const
	{ return m_PerspectiveViewpointOrientationZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getPerspectiveViewpointPositionXAdjustment () const
	{ return m_PerspectiveViewpointPositionXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getPerspectiveViewpointPositionYAdjustment () const
	{ return m_PerspectiveViewpointPositionYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getPerspectiveViewpointPositionZAdjustment () const
	{ return m_PerspectiveViewpointPositionZAdjustment; }

	const Glib::RefPtr <Gtk::TextBuffer> &
	getViewpointDescriptionTextBuffer () const
	{ return m_ViewpointDescriptionTextBuffer; }

	const Glib::RefPtr <Gtk::ListStore> &
	getViewpointListStore () const
	{ return m_ViewpointListStore; }

	Gtk::Menu &
	getNewViewpointMenu () const
	{ return *m_NewViewpointMenu; }

	Gtk::MenuItem &
	getNewViewpointMenuItem () const
	{ return *m_NewViewpointMenuItem; }

	Gtk::MenuItem &
	getNewOrthoViewpointMenuItem () const
	{ return *m_NewOrthoViewpointMenuItem; }

	Gtk::MenuItem &
	getNewGeoViewpointMenuItem () const
	{ return *m_NewGeoViewpointMenuItem; }

	Gtk::Window &
	getWindow () const
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const
	{ return *m_Widget; }

	Gtk::Box &
	getViewpointListBox () const
	{ return *m_ViewpointListBox; }

	Gtk::Box &
	getViewpointActionBox () const
	{ return *m_ViewpointActionBox; }

	Gtk::Button &
	getRemoveViewpointButton () const
	{ return *m_RemoveViewpointButton; }

	Gtk::Expander &
	getViewpointExpander () const
	{ return *m_ViewpointExpander; }

	Gtk::Grid &
	getViewpointBox () const
	{ return *m_ViewpointBox; }

	Gtk::CheckButton &
	getViewpointJumpCheckButton () const
	{ return *m_ViewpointJumpCheckButton; }

	Gtk::CheckButton &
	getViewpointRetainUserOffsetsCheckButton () const
	{ return *m_ViewpointRetainUserOffsetsCheckButton; }

	Gtk::Box &
	getViewpointNameBox () const
	{ return *m_ViewpointNameBox; }

	Gtk::Entry &
	getViewpointNameEntry () const
	{ return *m_ViewpointNameEntry; }

	Gtk::Button &
	getViewpointRenameButton () const
	{ return *m_ViewpointRenameButton; }

	Gtk::TextView &
	getViewpointDescriptionTextView () const
	{ return *m_ViewpointDescriptionTextView; }

	Gtk::Button &
	getUpdateViewpointButton () const
	{ return *m_UpdateViewpointButton; }

	Gtk::ToggleButton &
	getLockToCameraButton () const
	{ return *m_LockToCameraButton; }

	Gtk::Image &
	getLockToCameraImage () const
	{ return *m_LockToCameraImage; }

	Gtk::Expander &
	getPerspectiveViewpointExpander () const
	{ return *m_PerspectiveViewpointExpander; }

	Gtk::Grid &
	getPerspectiveViewpointBox () const
	{ return *m_PerspectiveViewpointBox; }

	Gtk::Box &
	getPerspectiveViewpointFieldOfViewBox () const
	{ return *m_PerspectiveViewpointFieldOfViewBox; }

	Gtk::Box &
	getPerspectiveViewpointPositionBox () const
	{ return *m_PerspectiveViewpointPositionBox; }

	Gtk::Box &
	getPerspectiveViewpointCenterOfRotationBox () const
	{ return *m_PerspectiveViewpointCenterOfRotationBox; }

	Gtk::Box &
	getPerspectiveViewpointOrientationBox () const
	{ return *m_PerspectiveViewpointOrientationBox; }

	Gtk::Box &
	getPerspectiveViewpointOrientationToolBox () const
	{ return *m_PerspectiveViewpointOrientationToolBox; }

	Gtk::Expander &
	getOrthoViewpointExpander () const
	{ return *m_OrthoViewpointExpander; }

	Gtk::Grid &
	getOrthoViewpointBox () const
	{ return *m_OrthoViewpointBox; }

	Gtk::Box &
	getOrthoViewpointPositionBox () const
	{ return *m_OrthoViewpointPositionBox; }

	Gtk::Box &
	getOrthoViewpointCenterOfRotationBox () const
	{ return *m_OrthoViewpointCenterOfRotationBox; }

	Gtk::Box &
	getOrthoViewpointOrientationBox () const
	{ return *m_OrthoViewpointOrientationBox; }

	Gtk::Box &
	getOthoViewpointOrientationToolBox () const
	{ return *m_OthoViewpointOrientationToolBox; }

	Gtk::Grid &
	getOrthoViewpointFieldOfViewBox () const
	{ return *m_OrthoViewpointFieldOfViewBox; }

	Gtk::SpinButton &
	getOrthoViewpointFieldOfViewSpinButton3 () const
	{ return *m_OrthoViewpointFieldOfViewSpinButton3; }

	Gtk::SpinButton &
	getOrthoViewpointFieldOfViewSpinButton0 () const
	{ return *m_OrthoViewpointFieldOfViewSpinButton0; }

	Gtk::SpinButton &
	getOrthoViewpointFieldOfViewSpinButton2 () const
	{ return *m_OrthoViewpointFieldOfViewSpinButton2; }

	Gtk::SpinButton &
	getOrthoViewpointFieldOfViewSpinButton1 () const
	{ return *m_OrthoViewpointFieldOfViewSpinButton1; }

	Gtk::Expander &
	getGeoViewpointExpander () const
	{ return *m_GeoViewpointExpander; }

	Gtk::Grid &
	getGeoViewpointBox () const
	{ return *m_GeoViewpointBox; }

	Gtk::Box &
	getGeoViewpointFieldOfViewBox () const
	{ return *m_GeoViewpointFieldOfViewBox; }

	Gtk::Box &
	getGeoViewpointPositionBox () const
	{ return *m_GeoViewpointPositionBox; }

	Gtk::SpinButton &
	getGeoViewpointPositionXSpinButton () const
	{ return *m_GeoViewpointPositionXSpinButton; }

	Gtk::SpinButton &
	getGeoViewpointPositionYSpinButton () const
	{ return *m_GeoViewpointPositionYSpinButton; }

	Gtk::SpinButton &
	getGeoViewpointPositionZSpinButton () const
	{ return *m_GeoViewpointPositionZSpinButton; }

	Gtk::Box &
	getGeoViewpointCenterOfRotationBox () const
	{ return *m_GeoViewpointCenterOfRotationBox; }

	Gtk::SpinButton &
	getGeoViewpointCenterOfRotationXSpinButton () const
	{ return *m_GeoViewpointCenterOfRotationXSpinButton; }

	Gtk::SpinButton &
	getGeoViewpointCenterOfRotationYSpinButton () const
	{ return *m_GeoViewpointCenterOfRotationYSpinButton; }

	Gtk::SpinButton &
	getGeoViewpointCenterOfRotationZSpinButton () const
	{ return *m_GeoViewpointCenterOfRotationZSpinButton; }

	Gtk::Box &
	getGeoViewpointOrientationBox () const
	{ return *m_GeoViewpointOrientationBox; }

	Gtk::Box &
	getGeoViewpointOrientationToolBox () const
	{ return *m_GeoViewpointOrientationToolBox; }

	Gtk::SpinButton &
	getGeoViewpointSpeedFactorSpinButton () const
	{ return *m_GeoViewpointSpeedFactorSpinButton; }

	Gtk::ComboBoxText &
	getGeoViewpointCoordinateSystemComboBoxText () const
	{ return *m_GeoViewpointCoordinateSystemComboBoxText; }

	Gtk::Box &
	getGeoViewpointEllipsoidBox () const
	{ return *m_GeoViewpointEllipsoidBox; }

	Gtk::ComboBoxText &
	getGeoViewpointEllipsoidComboBoxText () const
	{ return *m_GeoViewpointEllipsoidComboBoxText; }

	Gtk::ComboBoxText &
	getGeoViewpointGDOrderComboBoxText () const
	{ return *m_GeoViewpointGDOrderComboBoxText; }

	Gtk::Box &
	getGeoViewpointGeoSystemUTMBox () const
	{ return *m_GeoViewpointGeoSystemUTMBox; }

	Gtk::ComboBoxText &
	getGeoViewpointHemisphereComboBoxText () const
	{ return *m_GeoViewpointHemisphereComboBoxText; }

	Gtk::ComboBoxText &
	getGeoViewpointUTMOrderComboBoxText () const
	{ return *m_GeoViewpointUTMOrderComboBoxText; }

	virtual
	void
	on_new_viewpoint_activated () = 0;

	virtual
	void
	on_new_ortho_viewpoint_activated () = 0;

	virtual
	void
	on_new_geo_viewpoint_activated () = 0;

	virtual
	void
	on_remove_viewpoint_clicked () = 0;

	virtual
	void
	on_update_viewpoint_clicked () = 0;

	virtual
	void
	on_lock_to_camera_toggled () = 0;

	virtual
	~X3DViewpointEditorInterface ();


private:

	virtual
	void
	construct () final override
	{ X3DEditorInterface::construct (); }

	void
	create (const std::string &);

	static const std::string m_widgetName;

	std::string                    filename;
	Glib::RefPtr <Gtk::Builder>    m_builder;
	std::deque <sigc::connection>  m_connections;
	Glib::RefPtr <Gtk::Adjustment> m_GeoViewpointCenterOfRotationXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GeoViewpointCenterOfRotationYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GeoViewpointCenterOfRotationZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GeoViewpointFieldOfViewAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GeoViewpointOrientationAAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GeoViewpointOrientationXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GeoViewpointOrientationYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GeoViewpointOrientationZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GeoViewpointPositionXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GeoViewpointPositionYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GeoViewpointPositionZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GeoViewpointSpeedFactorAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GeoViewpointZoneAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_OrthoViewpointCenterOfRotationXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_OrthoViewpointCenterOfRotationYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_OrthoViewpointCenterOfRotationZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_OrthoViewpointFieldOfViewMaxXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_OrthoViewpointFieldOfViewMaxYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_OrthoViewpointFieldOfViewMinXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_OrthoViewpointFieldOfViewMinYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_OrthoViewpointOrientationAAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_OrthoViewpointOrientationXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_OrthoViewpointOrientationYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_OrthoViewpointOrientationZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_OrthoViewpointPositionXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_OrthoViewpointPositionYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_OrthoViewpointPositionZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_PerspectiveViewpointCenterOfRotationXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_PerspectiveViewpointCenterOfRotationYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_PerspectiveViewpointCenterOfRotationZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_PerspectiveViewpointFieldOfViewAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_PerspectiveViewpointOrientationAAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_PerspectiveViewpointOrientationXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_PerspectiveViewpointOrientationYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_PerspectiveViewpointOrientationZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_PerspectiveViewpointPositionXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_PerspectiveViewpointPositionYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_PerspectiveViewpointPositionZAdjustment;
	Glib::RefPtr <Gtk::TextBuffer> m_ViewpointDescriptionTextBuffer;
	Glib::RefPtr <Gtk::ListStore>  m_ViewpointListStore;
	Gtk::Menu*                     m_NewViewpointMenu;
	Gtk::MenuItem*                 m_NewViewpointMenuItem;
	Gtk::MenuItem*                 m_NewOrthoViewpointMenuItem;
	Gtk::MenuItem*                 m_NewGeoViewpointMenuItem;
	Gtk::Window*                   m_Window;
	Gtk::Box*                      m_Widget;
	Gtk::Box*                      m_ViewpointListBox;
	Gtk::Box*                      m_ViewpointActionBox;
	Gtk::Button*                   m_RemoveViewpointButton;
	Gtk::Expander*                 m_ViewpointExpander;
	Gtk::Grid*                     m_ViewpointBox;
	Gtk::CheckButton*              m_ViewpointJumpCheckButton;
	Gtk::CheckButton*              m_ViewpointRetainUserOffsetsCheckButton;
	Gtk::Box*                      m_ViewpointNameBox;
	Gtk::Entry*                    m_ViewpointNameEntry;
	Gtk::Button*                   m_ViewpointRenameButton;
	Gtk::TextView*                 m_ViewpointDescriptionTextView;
	Gtk::Button*                   m_UpdateViewpointButton;
	Gtk::ToggleButton*             m_LockToCameraButton;
	Gtk::Image*                    m_LockToCameraImage;
	Gtk::Expander*                 m_PerspectiveViewpointExpander;
	Gtk::Grid*                     m_PerspectiveViewpointBox;
	Gtk::Box*                      m_PerspectiveViewpointFieldOfViewBox;
	Gtk::Box*                      m_PerspectiveViewpointPositionBox;
	Gtk::Box*                      m_PerspectiveViewpointCenterOfRotationBox;
	Gtk::Box*                      m_PerspectiveViewpointOrientationBox;
	Gtk::Box*                      m_PerspectiveViewpointOrientationToolBox;
	Gtk::Expander*                 m_OrthoViewpointExpander;
	Gtk::Grid*                     m_OrthoViewpointBox;
	Gtk::Box*                      m_OrthoViewpointPositionBox;
	Gtk::Box*                      m_OrthoViewpointCenterOfRotationBox;
	Gtk::Box*                      m_OrthoViewpointOrientationBox;
	Gtk::Box*                      m_OthoViewpointOrientationToolBox;
	Gtk::Grid*                     m_OrthoViewpointFieldOfViewBox;
	Gtk::SpinButton*               m_OrthoViewpointFieldOfViewSpinButton3;
	Gtk::SpinButton*               m_OrthoViewpointFieldOfViewSpinButton0;
	Gtk::SpinButton*               m_OrthoViewpointFieldOfViewSpinButton2;
	Gtk::SpinButton*               m_OrthoViewpointFieldOfViewSpinButton1;
	Gtk::Expander*                 m_GeoViewpointExpander;
	Gtk::Grid*                     m_GeoViewpointBox;
	Gtk::Box*                      m_GeoViewpointFieldOfViewBox;
	Gtk::Box*                      m_GeoViewpointPositionBox;
	Gtk::SpinButton*               m_GeoViewpointPositionXSpinButton;
	Gtk::SpinButton*               m_GeoViewpointPositionYSpinButton;
	Gtk::SpinButton*               m_GeoViewpointPositionZSpinButton;
	Gtk::Box*                      m_GeoViewpointCenterOfRotationBox;
	Gtk::SpinButton*               m_GeoViewpointCenterOfRotationXSpinButton;
	Gtk::SpinButton*               m_GeoViewpointCenterOfRotationYSpinButton;
	Gtk::SpinButton*               m_GeoViewpointCenterOfRotationZSpinButton;
	Gtk::Box*                      m_GeoViewpointOrientationBox;
	Gtk::Box*                      m_GeoViewpointOrientationToolBox;
	Gtk::SpinButton*               m_GeoViewpointSpeedFactorSpinButton;
	Gtk::ComboBoxText*             m_GeoViewpointCoordinateSystemComboBoxText;
	Gtk::Box*                      m_GeoViewpointEllipsoidBox;
	Gtk::ComboBoxText*             m_GeoViewpointEllipsoidComboBoxText;
	Gtk::ComboBoxText*             m_GeoViewpointGDOrderComboBoxText;
	Gtk::Box*                      m_GeoViewpointGeoSystemUTMBox;
	Gtk::ComboBoxText*             m_GeoViewpointHemisphereComboBoxText;
	Gtk::ComboBoxText*             m_GeoViewpointUTMOrderComboBoxText;

};

} // puck
} // titania

#endif
