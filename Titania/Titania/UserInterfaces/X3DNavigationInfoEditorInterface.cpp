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

#include "X3DNavigationInfoEditorInterface.h"

namespace titania {
namespace puck {

void
X3DNavigationInfoEditorInterface::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	create ();
}

void
X3DNavigationInfoEditorInterface::create (std::initializer_list <std::string> filenames)
{
	// Create Builder.
	m_builder = Gtk::Builder::create ();

	for (const auto & filename : filenames)
		m_builder -> add_from_file (filename);

	create ();
}

void
X3DNavigationInfoEditorInterface::create ()
{
	// Get objects.
	m_CollisionRadiusAdjustment             = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("CollisionRadiusAdjustment"));
	m_HeightAdjustment                      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("HeightAdjustment"));
	m_NavigationInfoTransitionTypeListStore = Glib::RefPtr <Gtk::ListStore>::cast_dynamic (m_builder -> get_object ("NavigationInfoTransitionTypeListStore"));
	m_NavigationInfoTypeListStore           = Glib::RefPtr <Gtk::ListStore>::cast_dynamic (m_builder -> get_object ("NavigationInfoTypeListStore"));
	m_SpeedAdjustment                       = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("SpeedAdjustment"));
	m_StepHeightAdjustment                  = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("StepHeightAdjustment"));
	m_TransitionTimeAdjustment              = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransitionTimeAdjustment"));
	m_VisibilityLimitAdjustment             = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("VisibilityLimitAdjustment"));
	m_TypeCellRendererCombo                 = Glib::RefPtr <Gtk::CellRendererCombo>::cast_dynamic (m_builder -> get_object ("TypeCellRendererCombo"));
	m_TransitionTypeCellRendererCombo       = Glib::RefPtr <Gtk::CellRendererCombo>::cast_dynamic (m_builder -> get_object ("TransitionTypeCellRendererCombo"));

	// Get widgets.
	m_builder -> get_widget ("Window", m_Window);
	m_builder -> get_widget ("Widget", m_Widget);
	m_builder -> get_widget ("Paned", m_Paned);
	m_builder -> get_widget ("NavigationInfoListBox", m_NavigationInfoListBox);
	m_builder -> get_widget ("HeaderBar", m_HeaderBar);
	m_builder -> get_widget ("NavigationInfoActionBox", m_NavigationInfoActionBox);
	m_builder -> get_widget ("NewNavigationInfoButton", m_NewNavigationInfoButton);
	m_builder -> get_widget ("RemoveNavigationInfoButton", m_RemoveNavigationInfoButton);
	m_builder -> get_widget ("NavigationInfoExpander", m_NavigationInfoExpander);
	m_builder -> get_widget ("NavigationInfoBox", m_NavigationInfoBox);
	m_builder -> get_widget ("AvatarSizeBox", m_AvatarSizeBox);
	m_builder -> get_widget ("AvatarSizeSpinButton0", m_AvatarSizeSpinButton0);
	m_builder -> get_widget ("AvatarSizeSpinButton1", m_AvatarSizeSpinButton1);
	m_builder -> get_widget ("AvatarSizeSpinButton2", m_AvatarSizeSpinButton2);
	m_builder -> get_widget ("SpeedSpinButton", m_SpeedSpinButton);
	m_builder -> get_widget ("HeadlightCheckButton", m_HeadlightCheckButton);
	m_builder -> get_widget ("TransitionTimeSpinButton", m_TransitionTimeSpinButton);
	m_builder -> get_widget ("VisibilityLimitSpinButton", m_VisibilityLimitSpinButton);
	m_builder -> get_widget ("TypeBox", m_TypeBox);
	m_builder -> get_widget ("TypeTreeView", m_TypeTreeView);
	m_builder -> get_widget ("TypeAddButton", m_TypeAddButton);
	m_builder -> get_widget ("TypeRemoveButton", m_TypeRemoveButton);
	m_builder -> get_widget ("TransitionTypeBox", m_TransitionTypeBox);
	m_builder -> get_widget ("TransitionTypeTreeView", m_TransitionTypeTreeView);
	m_builder -> get_widget ("TransitionTypeAddButton", m_TransitionTypeAddButton);
	m_builder -> get_widget ("TransitionTypeRemoveButton", m_TransitionTypeRemoveButton);
	m_builder -> get_widget ("NameBox", m_NameBox);
	m_builder -> get_widget ("NameEntry", m_NameEntry);
	m_builder -> get_widget ("RenameButton", m_RenameButton);

	// Connect object Gtk::Button with id 'NewNavigationInfoButton'.
	m_NewNavigationInfoButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DNavigationInfoEditorInterface::on_new_navigation_info_clicked));
	m_RemoveNavigationInfoButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DNavigationInfoEditorInterface::on_remove_navigation_info_clicked));
}

X3DNavigationInfoEditorInterface::~X3DNavigationInfoEditorInterface ()
{
	delete m_Window;
}

} // puck
} // titania
