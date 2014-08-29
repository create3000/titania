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
#include "X3DLODEditorInterface.h"

namespace titania {
namespace puck {

const std::string X3DLODEditorInterface::m_widgetName = "LODEditor";

void
X3DLODEditorInterface::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	// Get objects.
	m_LODBBoxCenterXAdjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("LODBBoxCenterXAdjustment"));
	m_LODBBoxCenterYAdjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("LODBBoxCenterYAdjustment"));
	m_LODBBoxCenterZAdjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("LODBBoxCenterZAdjustment"));
	m_LODBBoxSizeXAdjustment   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("LODBBoxSizeXAdjustment"));
	m_LODBBoxSizeYAdjustment   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("LODBBoxSizeYAdjustment"));
	m_LODBBoxSizeZAdjustment   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("LODBBoxSizeZAdjustment"));
	m_LODCenterXAdjustment     = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("LODCenterXAdjustment"));
	m_LODCenterYAdjustment     = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("LODCenterYAdjustment"));
	m_LODCenterZAdjustment     = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("LODCenterZAdjustment"));
	m_LODLevelAdjustment       = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("LODLevelAdjustment"));
	m_LODRangeMaxAdjustment    = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("LODRangeMaxAdjustment"));
	m_LODRangeMinAdjustment    = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("LODRangeMinAdjustment"));

	// Get widgets.
	m_builder -> get_widget ("Window", m_Window);
	m_builder -> get_widget ("Widget", m_Widget);
	m_builder -> get_widget ("IndexButton", m_IndexButton);
	m_builder -> get_widget ("LODExpander", m_LODExpander);
	m_builder -> get_widget ("LODBox", m_LODBox);
	m_builder -> get_widget ("LODBBoxCenterBox", m_LODBBoxCenterBox);
	m_builder -> get_widget ("LODBBoxCenterXSpinButton", m_LODBBoxCenterXSpinButton);
	m_builder -> get_widget ("LODBBoxCenterYSpinButton", m_LODBBoxCenterYSpinButton);
	m_builder -> get_widget ("LODBBoxCenterZSpinButton", m_LODBBoxCenterZSpinButton);
	m_builder -> get_widget ("LODBBoxSizeBox", m_LODBBoxSizeBox);
	m_builder -> get_widget ("LODBBoxSizeXSpinButton", m_LODBBoxSizeXSpinButton);
	m_builder -> get_widget ("LODBBoxSizeYSpinButton", m_LODBBoxSizeYSpinButton);
	m_builder -> get_widget ("LODBBoxSizeZSpinButton", m_LODBBoxSizeZSpinButton);
	m_builder -> get_widget ("LODLevelSpinButton", m_LODLevelSpinButton);
	m_builder -> get_widget ("LODKeepCurrentLevelCheckButton", m_LODKeepCurrentLevelCheckButton);
	m_builder -> get_widget ("LODForceTransitionsCheckButton", m_LODForceTransitionsCheckButton);
	m_builder -> get_widget ("LODCenterBox", m_LODCenterBox);
	m_builder -> get_widget ("LODCenterXSpinButton", m_LODCenterXSpinButton);
	m_builder -> get_widget ("LODCenterYSpinButton", m_LODCenterYSpinButton);
	m_builder -> get_widget ("LODCenterZSpinButton", m_LODCenterZSpinButton);
	m_builder -> get_widget ("LODRangeBox", m_LODRangeBox);
	m_builder -> get_widget ("LODRangeMinSpinButton", m_LODRangeMinSpinButton);
	m_builder -> get_widget ("LODRangeMaxSpinButton", m_LODRangeMaxSpinButton);
	m_builder -> get_widget ("LODMaxCheckButton", m_LODMaxCheckButton);
	m_builder -> get_widget ("LODNameBox", m_LODNameBox);
	m_builder -> get_widget ("LODNameEntry", m_LODNameEntry);
	m_builder -> get_widget ("LODRenameButton", m_LODRenameButton);

	// Connect object Gtk::Button with id 'IndexButton'.
	m_IndexButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DLODEditorInterface::on_index_clicked));

	// Connect object Gtk::CheckButton with id 'LODKeepCurrentLevelCheckButton'.
	m_LODKeepCurrentLevelCheckButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DLODEditorInterface::on_lod_keep_current_level_activate));

	// Call construct handler of base class.
	construct ();
}

X3DLODEditorInterface::~X3DLODEditorInterface ()
{
	delete m_Window;
}

} // puck
} // titania
