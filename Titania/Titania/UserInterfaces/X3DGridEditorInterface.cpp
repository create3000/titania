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
	m_AngleColorAdjustment            = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AngleColorAdjustment"));
	m_AngleDimensionXAdjustment       = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AngleDimensionXAdjustment"));
	m_AngleDimensionYAdjustment       = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AngleDimensionYAdjustment"));
	m_AngleLineColorAdjustment        = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AngleLineColorAdjustment"));
	m_AngleMajorGridAdjustment        = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AngleMajorGridAdjustment"));
	m_AngleMajorLineColorAdjustment   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AngleMajorLineColorAdjustment"));
	m_AngleMajorLineEvery0Adjustment  = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AngleMajorLineEvery0Adjustment"));
	m_AngleMajorLineEvery1Adjustment  = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AngleMajorLineEvery1Adjustment"));
	m_AngleMajorLineOffset0Adjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AngleMajorLineOffset0Adjustment"));
	m_AngleMajorLineOffset1Adjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AngleMajorLineOffset1Adjustment"));
	m_AngleScaleXAdjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AngleScaleXAdjustment"));
	m_AngleScaleYAdjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AngleScaleYAdjustment"));
	m_AngleScaleZAdjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AngleScaleZAdjustment"));
	m_AngleTranslationXAdjustment     = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AngleTranslationXAdjustment"));
	m_AngleTranslationYAdjustment     = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AngleTranslationYAdjustment"));
	m_AngleTranslationZAdjustment     = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AngleTranslationZAdjustment"));
	m_GridColorAdjustment             = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridColorAdjustment"));
	m_GridGapXAdjustment              = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridGapXAdjustment"));
	m_GridGapYAdjustment              = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridGapYAdjustment"));
	m_GridGapZAdjustment              = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridGapZAdjustment"));
	m_GridLineColorAdjustment         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridLineColorAdjustment"));
	m_GridMajorGridAdjustment         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridMajorGridAdjustment"));
	m_GridMajorLineColorAdjustment    = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridMajorLineColorAdjustment"));
	m_GridMajorLineEveryXAdjustment   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridMajorLineEveryXAdjustment"));
	m_GridMajorLineEveryYAdjustment   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridMajorLineEveryYAdjustment"));
	m_GridMajorLineEveryZAdjustment   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridMajorLineEveryZAdjustment"));
	m_GridMajorLineOffsetXAdjustment  = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridMajorLineOffsetXAdjustment"));
	m_GridMajorLineOffsetYAdjustment  = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridMajorLineOffsetYAdjustment"));
	m_GridMajorLineOffsetZAdjustment  = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridMajorLineOffsetZAdjustment"));
	m_GridScaleXAdjustment            = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridScaleXAdjustment"));
	m_GridScaleYAdjustment            = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridScaleYAdjustment"));
	m_GridScaleZAdjustment            = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridScaleZAdjustment"));
	m_GridTranslationXAdjustment      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridTranslationXAdjustment"));
	m_GridTranslationYAdjustment      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridTranslationYAdjustment"));
	m_GridTranslationZAdjustment      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridTranslationZAdjustment"));
	m_GridTransparencyAdjustment      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridTransparencyAdjustment"));
	m_GridXDimensionAdjustment        = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridXDimensionAdjustment"));
	m_GridYDimensionAdjustment        = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridYDimensionAdjustment"));
	m_GridZDimensionAdjustment        = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridZDimensionAdjustment"));

	// Get widgets.
	m_builder -> get_widget ("Window", m_Window);
	m_builder -> get_widget ("Widget", m_Widget);
	m_builder -> get_widget ("GridNotebook", m_GridNotebook);
	m_builder -> get_widget ("GridExpander", m_GridExpander);
	m_builder -> get_widget ("GridCheckButton", m_GridCheckButton);
	m_builder -> get_widget ("GridBox", m_GridBox);
	m_builder -> get_widget ("GridPlaneComboBoxText", m_GridPlaneComboBoxText);
	m_builder -> get_widget ("GridColorBox", m_GridColorBox);
	m_builder -> get_widget ("GridColorButton", m_GridColorButton);
	m_builder -> get_widget ("GridTranslationBox", m_GridTranslationBox);
	m_builder -> get_widget ("GridDimensionBox", m_GridDimensionBox);
	m_builder -> get_widget ("GridMajorLineEveryBox", m_GridMajorLineEveryBox);
	m_builder -> get_widget ("GridLineColorBox", m_GridLineColorBox);
	m_builder -> get_widget ("GridLineColorButton", m_GridLineColorButton);
	m_builder -> get_widget ("GridMajorLineColorBox", m_GridMajorLineColorBox);
	m_builder -> get_widget ("GridMajorLineColorButton", m_GridMajorLineColorButton);
	m_builder -> get_widget ("GridScaleBox", m_GridScaleBox);
	m_builder -> get_widget ("GridMajorLineOffsetBox", m_GridMajorLineOffsetBox);
	m_builder -> get_widget ("GridMajorGridSpinButton", m_GridMajorGridSpinButton);
	m_builder -> get_widget ("GridAddMajorGridButton", m_GridAddMajorGridButton);
	m_builder -> get_widget ("GridRemoveMajorGridButton", m_GridRemoveMajorGridButton);
	m_builder -> get_widget ("AngleExpander", m_AngleExpander);
	m_builder -> get_widget ("AngleCheckButton", m_AngleCheckButton);
	m_builder -> get_widget ("AngleBox", m_AngleBox);
	m_builder -> get_widget ("AnglePlaneComboBoxText", m_AnglePlaneComboBoxText);
	m_builder -> get_widget ("AngleColorBox", m_AngleColorBox);
	m_builder -> get_widget ("AngleColorButton", m_AngleColorButton);
	m_builder -> get_widget ("AngleTranslationBox", m_AngleTranslationBox);
	m_builder -> get_widget ("AngleDimensionBox", m_AngleDimensionBox);
	m_builder -> get_widget ("AngleMajorLineEveryBox", m_AngleMajorLineEveryBox);
	m_builder -> get_widget ("AngleLineColorBox", m_AngleLineColorBox);
	m_builder -> get_widget ("AngleLineColorButton", m_AngleLineColorButton);
	m_builder -> get_widget ("AngleMajorLineColorBox", m_AngleMajorLineColorBox);
	m_builder -> get_widget ("AngleMajorLineColorButton", m_AngleMajorLineColorButton);
	m_builder -> get_widget ("AngleScaleBox", m_AngleScaleBox);
	m_builder -> get_widget ("AngleMajorLineOffsetBox", m_AngleMajorLineOffsetBox);
	m_builder -> get_widget ("AngleMajorGridSpinButton", m_AngleMajorGridSpinButton);
	m_builder -> get_widget ("AngleAddMajorGridButton", m_AngleAddMajorGridButton);
	m_builder -> get_widget ("AngleRemoveMajorGridButton", m_AngleRemoveMajorGridButton);
	m_GridCheckButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DGridEditorInterface::on_grid_toggled));

	// Connect object Gtk::ComboBoxText with id 'GridPlaneComboBoxText'.
	m_GridPlaneComboBoxText -> signal_changed () .connect (sigc::mem_fun (*this, &X3DGridEditorInterface::on_grid_plane_changed));

	// Connect object Gtk::SpinButton with id 'GridMajorGridSpinButton'.
	m_GridMajorGridSpinButton -> signal_value_changed () .connect (sigc::mem_fun (*this, &X3DGridEditorInterface::on_major_line_grid_value_changed));

	// Connect object Gtk::Button with id 'GridAddMajorGridButton'.
	m_GridAddMajorGridButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DGridEditorInterface::on_add_major_line_grid));
	m_GridRemoveMajorGridButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DGridEditorInterface::on_remove_major_line_grid));

	// Connect object Gtk::CheckButton with id 'AngleCheckButton'.
	m_AngleCheckButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DGridEditorInterface::on_angle_toggled));

	// Connect object Gtk::ComboBoxText with id 'AnglePlaneComboBoxText'.
	m_AnglePlaneComboBoxText -> signal_changed () .connect (sigc::mem_fun (*this, &X3DGridEditorInterface::on_angle_plane_changed));

	// Connect object Gtk::SpinButton with id 'AngleMajorGridSpinButton'.
	m_AngleMajorGridSpinButton -> signal_value_changed () .connect (sigc::mem_fun (*this, &X3DGridEditorInterface::on_angle_major_line_grid_value_changed));

	// Connect object Gtk::Button with id 'AngleAddMajorGridButton'.
	m_AngleAddMajorGridButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DGridEditorInterface::on_angle_add_major_line_grid));
	m_AngleRemoveMajorGridButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DGridEditorInterface::on_angle_remove_major_line_grid));

	// Call construct handler of base class.
	construct ();
}

X3DGridEditorInterface::~X3DGridEditorInterface ()
{
	delete m_Window;
}

} // puck
} // titania
