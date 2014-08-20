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
#include "X3DViewpointEditorInterface.h"

namespace titania {
namespace puck {

const std::string X3DViewpointEditorInterface::m_widgetName = "ViewpointEditor";

void
X3DViewpointEditorInterface::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	// Get objects.
	m_GeoViewpointCenterOfRotationXAdjustment         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoViewpointCenterOfRotationXAdjustment"));
	m_GeoViewpointCenterOfRotationYAdjustment         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoViewpointCenterOfRotationYAdjustment"));
	m_GeoViewpointCenterOfRotationZAdjustment         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoViewpointCenterOfRotationZAdjustment"));
	m_GeoViewpointFieldOfViewAdjustment               = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoViewpointFieldOfViewAdjustment"));
	m_GeoViewpointOrientationAAdjustment              = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoViewpointOrientationAAdjustment"));
	m_GeoViewpointOrientationXAdjustment              = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoViewpointOrientationXAdjustment"));
	m_GeoViewpointOrientationYAdjustment              = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoViewpointOrientationYAdjustment"));
	m_GeoViewpointOrientationZAdjustment              = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoViewpointOrientationZAdjustment"));
	m_GeoViewpointPositionXAdjustment                 = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoViewpointPositionXAdjustment"));
	m_GeoViewpointPositionYAdjustment                 = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoViewpointPositionYAdjustment"));
	m_GeoViewpointPositionZAdjustment                 = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoViewpointPositionZAdjustment"));
	m_GeoViewpointSpeedFactorAdjustment               = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoViewpointSpeedFactorAdjustment"));
	m_GeoViewpointZoneAdjustment                      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoViewpointZoneAdjustment"));
	m_OrthoViewpointCenterOfRotationXAdjustment       = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("OrthoViewpointCenterOfRotationXAdjustment"));
	m_OrthoViewpointCenterOfRotationYAdjustment       = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("OrthoViewpointCenterOfRotationYAdjustment"));
	m_OrthoViewpointCenterOfRotationZAdjustment       = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("OrthoViewpointCenterOfRotationZAdjustment"));
	m_OrthoViewpointFieldOfViewMaxXAdjustment         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("OrthoViewpointFieldOfViewMaxXAdjustment"));
	m_OrthoViewpointFieldOfViewMaxYAdjustment         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("OrthoViewpointFieldOfViewMaxYAdjustment"));
	m_OrthoViewpointFieldOfViewMinXAdjustment         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("OrthoViewpointFieldOfViewMinXAdjustment"));
	m_OrthoViewpointFieldOfViewMinYAdjustment         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("OrthoViewpointFieldOfViewMinYAdjustment"));
	m_OrthoViewpointOrientationAAdjustment            = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("OrthoViewpointOrientationAAdjustment"));
	m_OrthoViewpointOrientationXAdjustment            = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("OrthoViewpointOrientationXAdjustment"));
	m_OrthoViewpointOrientationYAdjustment            = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("OrthoViewpointOrientationYAdjustment"));
	m_OrthoViewpointOrientationZAdjustment            = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("OrthoViewpointOrientationZAdjustment"));
	m_OrthoViewpointPositionXAdjustment               = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("OrthoViewpointPositionXAdjustment"));
	m_OrthoViewpointPositionYAdjustment               = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("OrthoViewpointPositionYAdjustment"));
	m_OrthoViewpointPositionZAdjustment               = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("OrthoViewpointPositionZAdjustment"));
	m_PerspectiveViewpointCenterOfRotationXAdjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("PerspectiveViewpointCenterOfRotationXAdjustment"));
	m_PerspectiveViewpointCenterOfRotationYAdjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("PerspectiveViewpointCenterOfRotationYAdjustment"));
	m_PerspectiveViewpointCenterOfRotationZAdjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("PerspectiveViewpointCenterOfRotationZAdjustment"));
	m_PerspectiveViewpointFieldOfViewAdjustment       = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("PerspectiveViewpointFieldOfViewAdjustment"));
	m_PerspectiveViewpointOrientationAAdjustment      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("PerspectiveViewpointOrientationAAdjustment"));
	m_PerspectiveViewpointOrientationXAdjustment      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("PerspectiveViewpointOrientationXAdjustment"));
	m_PerspectiveViewpointOrientationYAdjustment      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("PerspectiveViewpointOrientationYAdjustment"));
	m_PerspectiveViewpointOrientationZAdjustment      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("PerspectiveViewpointOrientationZAdjustment"));
	m_PerspectiveViewpointPositionXAdjustment         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("PerspectiveViewpointPositionXAdjustment"));
	m_PerspectiveViewpointPositionYAdjustment         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("PerspectiveViewpointPositionYAdjustment"));
	m_PerspectiveViewpointPositionZAdjustment         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("PerspectiveViewpointPositionZAdjustment"));
	m_ViewpointDescriptionTextBuffer                  = Glib::RefPtr <Gtk::TextBuffer>::cast_dynamic (m_builder -> get_object ("ViewpointDescriptionTextBuffer"));
	m_ViewpointListStore                              = Glib::RefPtr <Gtk::ListStore>::cast_dynamic (m_builder -> get_object ("ViewpointListStore"));

	// Get widgets.
	m_builder -> get_widget ("Window", m_Window);
	m_Window -> set_name ("Window");
	m_builder -> get_widget ("Widget", m_Widget);
	m_Widget -> set_name ("Widget");
	m_builder -> get_widget ("ViewpointListBox", m_ViewpointListBox);
	m_ViewpointListBox -> set_name ("ViewpointListBox");
	m_builder -> get_widget ("ViewpointExpander", m_ViewpointExpander);
	m_ViewpointExpander -> set_name ("ViewpointExpander");
	m_builder -> get_widget ("ViewpointBox", m_ViewpointBox);
	m_ViewpointBox -> set_name ("ViewpointBox");
	m_builder -> get_widget ("ViewpointJumpCheckButton", m_ViewpointJumpCheckButton);
	m_ViewpointJumpCheckButton -> set_name ("ViewpointJumpCheckButton");
	m_builder -> get_widget ("ViewpointRetainUserOffsetsCheckButton", m_ViewpointRetainUserOffsetsCheckButton);
	m_ViewpointRetainUserOffsetsCheckButton -> set_name ("ViewpointRetainUserOffsetsCheckButton");
	m_builder -> get_widget ("ViewpointNameBox", m_ViewpointNameBox);
	m_ViewpointNameBox -> set_name ("ViewpointNameBox");
	m_builder -> get_widget ("ViewpointNameEntry", m_ViewpointNameEntry);
	m_ViewpointNameEntry -> set_name ("ViewpointNameEntry");
	m_builder -> get_widget ("ViewpointRenameButton", m_ViewpointRenameButton);
	m_ViewpointRenameButton -> set_name ("ViewpointRenameButton");
	m_builder -> get_widget ("ViewpointDescriptionTextView", m_ViewpointDescriptionTextView);
	m_ViewpointDescriptionTextView -> set_name ("ViewpointDescriptionTextView");
	m_builder -> get_widget ("UpdateViewpointButton", m_UpdateViewpointButton);
	m_UpdateViewpointButton -> set_name ("UpdateViewpointButton");
	m_builder -> get_widget ("PerspectiveViewpointExpander", m_PerspectiveViewpointExpander);
	m_PerspectiveViewpointExpander -> set_name ("PerspectiveViewpointExpander");
	m_builder -> get_widget ("PerspectiveViewpointFieldOfViewBox", m_PerspectiveViewpointFieldOfViewBox);
	m_PerspectiveViewpointFieldOfViewBox -> set_name ("PerspectiveViewpointFieldOfViewBox");
	m_builder -> get_widget ("PerspectiveViewpointPositionBox", m_PerspectiveViewpointPositionBox);
	m_PerspectiveViewpointPositionBox -> set_name ("PerspectiveViewpointPositionBox");
	m_builder -> get_widget ("PerspectiveViewpointCenterOfRotationBox", m_PerspectiveViewpointCenterOfRotationBox);
	m_PerspectiveViewpointCenterOfRotationBox -> set_name ("PerspectiveViewpointCenterOfRotationBox");
	m_builder -> get_widget ("PerspectiveViewpointOrientationBox", m_PerspectiveViewpointOrientationBox);
	m_PerspectiveViewpointOrientationBox -> set_name ("PerspectiveViewpointOrientationBox");
	m_builder -> get_widget ("OrthoViewpointExpander", m_OrthoViewpointExpander);
	m_OrthoViewpointExpander -> set_name ("OrthoViewpointExpander");
	m_builder -> get_widget ("OrthoViewpointPositionBox", m_OrthoViewpointPositionBox);
	m_OrthoViewpointPositionBox -> set_name ("OrthoViewpointPositionBox");
	m_builder -> get_widget ("OrthoViewpointCenterOfRotationBox", m_OrthoViewpointCenterOfRotationBox);
	m_OrthoViewpointCenterOfRotationBox -> set_name ("OrthoViewpointCenterOfRotationBox");
	m_builder -> get_widget ("OrthoViewpointOrientationBox", m_OrthoViewpointOrientationBox);
	m_OrthoViewpointOrientationBox -> set_name ("OrthoViewpointOrientationBox");
	m_builder -> get_widget ("OrthoViewpointFieldOfViewBox", m_OrthoViewpointFieldOfViewBox);
	m_OrthoViewpointFieldOfViewBox -> set_name ("OrthoViewpointFieldOfViewBox");
	m_builder -> get_widget ("GeoViewpointExpander", m_GeoViewpointExpander);
	m_GeoViewpointExpander -> set_name ("GeoViewpointExpander");
	m_builder -> get_widget ("GeoViewpointFieldOfViewBox", m_GeoViewpointFieldOfViewBox);
	m_GeoViewpointFieldOfViewBox -> set_name ("GeoViewpointFieldOfViewBox");
	m_builder -> get_widget ("GeoViewpointPositionBox", m_GeoViewpointPositionBox);
	m_GeoViewpointPositionBox -> set_name ("GeoViewpointPositionBox");
	m_builder -> get_widget ("GeoViewpointCenterOfRotationBox", m_GeoViewpointCenterOfRotationBox);
	m_GeoViewpointCenterOfRotationBox -> set_name ("GeoViewpointCenterOfRotationBox");
	m_builder -> get_widget ("GeoViewpointOrientationBox", m_GeoViewpointOrientationBox);
	m_GeoViewpointOrientationBox -> set_name ("GeoViewpointOrientationBox");
	m_builder -> get_widget ("GeoViewpointSpeedFactorSpinButton", m_GeoViewpointSpeedFactorSpinButton);
	m_GeoViewpointSpeedFactorSpinButton -> set_name ("GeoViewpointSpeedFactorSpinButton");
	m_builder -> get_widget ("GeoViewpointCoordinateSystemComboBoxText", m_GeoViewpointCoordinateSystemComboBoxText);
	m_GeoViewpointCoordinateSystemComboBoxText -> set_name ("GeoViewpointCoordinateSystemComboBoxText");
	m_builder -> get_widget ("GeoViewpointEllipsoidBox", m_GeoViewpointEllipsoidBox);
	m_GeoViewpointEllipsoidBox -> set_name ("GeoViewpointEllipsoidBox");
	m_builder -> get_widget ("GeoViewpointEllipsoidComboBoxText", m_GeoViewpointEllipsoidComboBoxText);
	m_GeoViewpointEllipsoidComboBoxText -> set_name ("GeoViewpointEllipsoidComboBoxText");
	m_builder -> get_widget ("GeoViewpointGDOrderComboBoxText", m_GeoViewpointGDOrderComboBoxText);
	m_GeoViewpointGDOrderComboBoxText -> set_name ("GeoViewpointGDOrderComboBoxText");
	m_builder -> get_widget ("GeoViewpointGeoSystemUTMBox", m_GeoViewpointGeoSystemUTMBox);
	m_GeoViewpointGeoSystemUTMBox -> set_name ("GeoViewpointGeoSystemUTMBox");
	m_builder -> get_widget ("GeoViewpointHemisphereComboBoxText", m_GeoViewpointHemisphereComboBoxText);
	m_GeoViewpointHemisphereComboBoxText -> set_name ("GeoViewpointHemisphereComboBoxText");
	m_builder -> get_widget ("GeoViewpointUTMOrderComboBoxText", m_GeoViewpointUTMOrderComboBoxText);
	m_GeoViewpointUTMOrderComboBoxText -> set_name ("GeoViewpointUTMOrderComboBoxText");
	m_UpdateViewpointButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DViewpointEditorInterface::on_update_viewpoint_clicked));

	// Call construct handler of base class.
	construct ();
}

X3DViewpointEditorInterface::~X3DViewpointEditorInterface ()
{
	delete m_Window;
}

} // puck
} // titania
