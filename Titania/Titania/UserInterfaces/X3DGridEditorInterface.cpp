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
#include "X3DGridEditorInterface.h"

namespace titania {
namespace puck {

const std::string X3DGridEditorInterface::m_widgetName = "GridEditor";

void
X3DGridEditorInterface::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	// Get objects.
	m_GridColorAdjustment            = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridColorAdjustment"));
	m_GridLineColorAdjustment        = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridLineColorAdjustment"));
	m_GridMajorLineColorAdjustment   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridMajorLineColorAdjustment"));
	m_GridMajorLinesEveryXAdjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridMajorLinesEveryXAdjustment"));
	m_GridMajorLinesEveryYAdjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridMajorLinesEveryYAdjustment"));
	m_GridMajorLinesEveryZAdjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridMajorLinesEveryZAdjustment"));
	m_GridTranslationXAdjustment     = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridTranslationXAdjustment"));
	m_GridTranslationYAdjustment     = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridTranslationYAdjustment"));
	m_GridTranslationZAdjustment     = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridTranslationZAdjustment"));
	m_GridTransparencyAdjustment     = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridTransparencyAdjustment"));
	m_GridXDimensionAdjustment       = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridXDimensionAdjustment"));
	m_GridXSpacingAdjustment         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridXSpacingAdjustment"));
	m_GridYDimensionAdjustment       = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridYDimensionAdjustment"));
	m_GridYSpacingAdjustment         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridYSpacingAdjustment"));
	m_GridZDimensionAdjustment       = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridZDimensionAdjustment"));
	m_GridZSpacingAdjustment         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridZSpacingAdjustment"));

	// Get widgets.
	m_builder -> get_widget ("Window", m_Window);
	m_builder -> get_widget ("Widget", m_Widget);
	m_builder -> get_widget ("GridExpander", m_GridExpander);
	m_builder -> get_widget ("GridBox", m_GridBox);
	m_builder -> get_widget ("GridPlaneComboBoxText", m_GridPlaneComboBoxText);
	m_builder -> get_widget ("GridColorBox", m_GridColorBox);
	m_builder -> get_widget ("GridColorButton", m_GridColorButton);
	m_builder -> get_widget ("GridTranslationBox", m_GridTranslationBox);
	m_builder -> get_widget ("GridDimensionBox", m_GridDimensionBox);
	m_builder -> get_widget ("GridSpacingBox", m_GridSpacingBox);
	m_builder -> get_widget ("GridMajorLinesEveryBox", m_GridMajorLinesEveryBox);
	m_builder -> get_widget ("GridLineColorBox", m_GridLineColorBox);
	m_builder -> get_widget ("GridLineColorButton", m_GridLineColorButton);
	m_builder -> get_widget ("GridMajorLineColorBox", m_GridMajorLineColorBox);
	m_builder -> get_widget ("GridMajorLineColorButton", m_GridMajorLineColorButton);

	// Connect object Gtk::ComboBoxText with id 'GridPlaneComboBoxText'.
	m_GridPlaneComboBoxText -> signal_changed () .connect (sigc::mem_fun (*this, &X3DGridEditorInterface::on_grid_plane_changed));

	// Call construct handler of base class.
	construct ();
}

X3DGridEditorInterface::~X3DGridEditorInterface ()
{
	delete m_Window;
}

} // puck
} // titania
