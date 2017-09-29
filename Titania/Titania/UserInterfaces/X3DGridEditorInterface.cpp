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

#include "X3DGridEditorInterface.h"

namespace titania {
namespace puck {

void
X3DGridEditorInterface::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	create ();
}

void
X3DGridEditorInterface::create (std::initializer_list <std::string> filenames)
{
	// Create Builder.
	m_builder = Gtk::Builder::create ();

	for (const auto & filename : filenames)
		m_builder -> add_from_file (filename);

	create ();
}

void
X3DGridEditorInterface::create ()
{
	// Get objects.
	m_AngleColorAdjustment                      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AngleColorAdjustment"));
	m_AngleDimension0Adjustment                 = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AngleDimension0Adjustment"));
	m_AngleDimension1Adjustment                 = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AngleDimension1Adjustment"));
	m_AngleDimensionYAdjustment                 = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AngleDimensionYAdjustment"));
	m_AngleLineColorAdjustment                  = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AngleLineColorAdjustment"));
	m_AngleMajorGridAdjustment                  = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AngleMajorGridAdjustment"));
	m_AngleMajorLineColorAdjustment             = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AngleMajorLineColorAdjustment"));
	m_AngleMajorLineEvery0Adjustment            = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AngleMajorLineEvery0Adjustment"));
	m_AngleMajorLineEvery1Adjustment            = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AngleMajorLineEvery1Adjustment"));
	m_AngleMajorLineEveryYAdjustment            = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AngleMajorLineEveryYAdjustment"));
	m_AngleMajorLineOffset0Adjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AngleMajorLineOffset0Adjustment"));
	m_AngleMajorLineOffset1Adjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AngleMajorLineOffset1Adjustment"));
	m_AngleMajorLineOffsetYAdjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AngleMajorLineOffsetYAdjustment"));
	m_AngleScaleXAdjustment                     = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AngleScaleXAdjustment"));
	m_AngleScaleYAdjustment                     = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AngleScaleYAdjustment"));
	m_AngleScaleZAdjustment                     = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AngleScaleZAdjustment"));
	m_AngleSnapDistanceAdjustment               = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AngleSnapDistanceAdjustment"));
	m_AngleTranslationXAdjustment               = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AngleTranslationXAdjustment"));
	m_AngleTranslationYAdjustment               = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AngleTranslationYAdjustment"));
	m_AngleTranslationZAdjustment               = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AngleTranslationZAdjustment"));
	m_AxonometricGridAngleAlphaAdjustment       = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AxonometricGridAngleAlphaAdjustment"));
	m_AxonometricGridAngleBetaAdjustment        = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AxonometricGridAngleBetaAdjustment"));
	m_AxonometricGridAngleGammaAdjustment       = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AxonometricGridAngleGammaAdjustment"));
	m_AxonometricGridColorAdjustment            = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AxonometricGridColorAdjustment"));
	m_AxonometricGridGapXAdjustment             = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AxonometricGridGapXAdjustment"));
	m_AxonometricGridGapYAdjustment             = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AxonometricGridGapYAdjustment"));
	m_AxonometricGridGapZAdjustment             = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AxonometricGridGapZAdjustment"));
	m_AxonometricGridLineColorAdjustment        = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AxonometricGridLineColorAdjustment"));
	m_AxonometricGridMajorGridAdjustment        = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AxonometricGridMajorGridAdjustment"));
	m_AxonometricGridMajorLineColorAdjustment   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AxonometricGridMajorLineColorAdjustment"));
	m_AxonometricGridMajorLineEveryTAdjustment  = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AxonometricGridMajorLineEveryTAdjustment"));
	m_AxonometricGridMajorLineEveryUAdjustment  = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AxonometricGridMajorLineEveryUAdjustment"));
	m_AxonometricGridMajorLineEveryVAdjustment  = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AxonometricGridMajorLineEveryVAdjustment"));
	m_AxonometricGridMajorLineEveryYAdjustment  = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AxonometricGridMajorLineEveryYAdjustment"));
	m_AxonometricGridMajorLineOffsetTAdjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AxonometricGridMajorLineOffsetTAdjustment"));
	m_AxonometricGridMajorLineOffsetUAdjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AxonometricGridMajorLineOffsetUAdjustment"));
	m_AxonometricGridMajorLineOffsetVAdjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AxonometricGridMajorLineOffsetVAdjustment"));
	m_AxonometricGridMajorLineOffsetYAdjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AxonometricGridMajorLineOffsetYAdjustment"));
	m_AxonometricGridScaleXAdjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AxonometricGridScaleXAdjustment"));
	m_AxonometricGridScaleYAdjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AxonometricGridScaleYAdjustment"));
	m_AxonometricGridScaleZAdjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AxonometricGridScaleZAdjustment"));
	m_AxonometricGridSnapDistanceAdjustment     = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AxonometricGridSnapDistanceAdjustment"));
	m_AxonometricGridTranslationXAdjustment     = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AxonometricGridTranslationXAdjustment"));
	m_AxonometricGridTranslationYAdjustment     = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AxonometricGridTranslationYAdjustment"));
	m_AxonometricGridTranslationZAdjustment     = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AxonometricGridTranslationZAdjustment"));
	m_AxonometricGridTransparencyAdjustment     = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AxonometricGridTransparencyAdjustment"));
	m_AxonometricGridUVTDimensionAdjustment     = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AxonometricGridUVTDimensionAdjustment"));
	m_AxonometricGridYDimensionAdjustment       = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AxonometricGridYDimensionAdjustment"));
	m_GridColorAdjustment                       = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridColorAdjustment"));
	m_GridGapXAdjustment                        = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridGapXAdjustment"));
	m_GridGapYAdjustment                        = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridGapYAdjustment"));
	m_GridGapZAdjustment                        = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridGapZAdjustment"));
	m_GridLineColorAdjustment                   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridLineColorAdjustment"));
	m_GridMajorGridAdjustment                   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridMajorGridAdjustment"));
	m_GridMajorLineColorAdjustment              = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridMajorLineColorAdjustment"));
	m_GridMajorLineEveryXAdjustment             = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridMajorLineEveryXAdjustment"));
	m_GridMajorLineEveryYAdjustment             = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridMajorLineEveryYAdjustment"));
	m_GridMajorLineEveryZAdjustment             = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridMajorLineEveryZAdjustment"));
	m_GridMajorLineOffsetXAdjustment            = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridMajorLineOffsetXAdjustment"));
	m_GridMajorLineOffsetYAdjustment            = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridMajorLineOffsetYAdjustment"));
	m_GridMajorLineOffsetZAdjustment            = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridMajorLineOffsetZAdjustment"));
	m_GridScaleXAdjustment                      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridScaleXAdjustment"));
	m_GridScaleYAdjustment                      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridScaleYAdjustment"));
	m_GridScaleZAdjustment                      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridScaleZAdjustment"));
	m_GridSnapDistanceAdjustment                = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridSnapDistanceAdjustment"));
	m_GridTranslationXAdjustment                = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridTranslationXAdjustment"));
	m_GridTranslationYAdjustment                = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridTranslationYAdjustment"));
	m_GridTranslationZAdjustment                = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridTranslationZAdjustment"));
	m_GridTransparencyAdjustment                = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridTransparencyAdjustment"));
	m_GridXDimensionAdjustment                  = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridXDimensionAdjustment"));
	m_GridYDimensionAdjustment                  = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridYDimensionAdjustment"));
	m_GridZDimensionAdjustment                  = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GridZDimensionAdjustment"));

	// Get widgets.
	m_builder -> get_widget ("Window", m_Window);
	m_builder -> get_widget ("Widget", m_Widget);
	m_builder -> get_widget ("HeaderBar", m_HeaderBar);
	m_builder -> get_widget ("GridNotebook", m_GridNotebook);
	m_builder -> get_widget ("GridExpander", m_GridExpander);
	m_builder -> get_widget ("GridCheckButton", m_GridCheckButton);
	m_builder -> get_widget ("GridTransformBox", m_GridTransformBox);
	m_builder -> get_widget ("GridPlaneComboBoxText", m_GridPlaneComboBoxText);
	m_builder -> get_widget ("GridTranslationBox", m_GridTranslationBox);
	m_builder -> get_widget ("GridDimensionBox", m_GridDimensionBox);
	m_builder -> get_widget ("GridDimensionSpinButton0", m_GridDimensionSpinButton0);
	m_builder -> get_widget ("GridDimensionSpinButton1", m_GridDimensionSpinButton1);
	m_builder -> get_widget ("GridDimensionSpinButton2", m_GridDimensionSpinButton2);
	m_builder -> get_widget ("GridScaleBox", m_GridScaleBox);
	m_builder -> get_widget ("GridUniformScaleButton", m_GridUniformScaleButton);
	m_builder -> get_widget ("GridUniformScaleImage", m_GridUniformScaleImage);
	m_builder -> get_widget ("GridMajorLinesExpander", m_GridMajorLinesExpander);
	m_builder -> get_widget ("GridMajorLinesBox", m_GridMajorLinesBox);
	m_builder -> get_widget ("GridMajorLineOffsetBox", m_GridMajorLineOffsetBox);
	m_builder -> get_widget ("GridMajorLineOffsetSpinButton0", m_GridMajorLineOffsetSpinButton0);
	m_builder -> get_widget ("GridMajorLineOffsetSpinButton1", m_GridMajorLineOffsetSpinButton1);
	m_builder -> get_widget ("GridMajorLineOffsetSpinButton2", m_GridMajorLineOffsetSpinButton2);
	m_builder -> get_widget ("GridMajorLineEveryBox", m_GridMajorLineEveryBox);
	m_builder -> get_widget ("GridMajorLineEverySpinButton0", m_GridMajorLineEverySpinButton0);
	m_builder -> get_widget ("GridMajorLineEverySpinButton1", m_GridMajorLineEverySpinButton1);
	m_builder -> get_widget ("GridMajorLineEverySpinButton2", m_GridMajorLineEverySpinButton2);
	m_builder -> get_widget ("GridMajorGridSpinButton", m_GridMajorGridSpinButton);
	m_builder -> get_widget ("GridAddMajorGridButton", m_GridAddMajorGridButton);
	m_builder -> get_widget ("GridRemoveMajorGridButton", m_GridRemoveMajorGridButton);
	m_builder -> get_widget ("GridColorsExpander", m_GridColorsExpander);
	m_builder -> get_widget ("GridColorsBox", m_GridColorsBox);
	m_builder -> get_widget ("GridMajorLineColorBox", m_GridMajorLineColorBox);
	m_builder -> get_widget ("GridMajorLineColorButton", m_GridMajorLineColorButton);
	m_builder -> get_widget ("GridLineColorBox", m_GridLineColorBox);
	m_builder -> get_widget ("GridLineColorButton", m_GridLineColorButton);
	m_builder -> get_widget ("GridColorBox", m_GridColorBox);
	m_builder -> get_widget ("GridColorButton", m_GridColorButton);
	m_builder -> get_widget ("AngleExpander", m_AngleExpander);
	m_builder -> get_widget ("AngleCheckButton", m_AngleCheckButton);
	m_builder -> get_widget ("AngleTransformBox", m_AngleTransformBox);
	m_builder -> get_widget ("AnglePlaneComboBoxText", m_AnglePlaneComboBoxText);
	m_builder -> get_widget ("AngleTranslationBox", m_AngleTranslationBox);
	m_builder -> get_widget ("AngleScaleBox", m_AngleScaleBox);
	m_builder -> get_widget ("AngleUniformScaleButton", m_AngleUniformScaleButton);
	m_builder -> get_widget ("AngleUniformScaleImage", m_AngleUniformScaleImage);
	m_builder -> get_widget ("AngleDimensionBox", m_AngleDimensionBox);
	m_builder -> get_widget ("AngleDimensionSpinButton0", m_AngleDimensionSpinButton0);
	m_builder -> get_widget ("AngleDimensionSpinButton1", m_AngleDimensionSpinButton1);
	m_builder -> get_widget ("AngleDimensionSpinButton2", m_AngleDimensionSpinButton2);
	m_builder -> get_widget ("AngleMajorLinesExpander", m_AngleMajorLinesExpander);
	m_builder -> get_widget ("AngleMajorLinesBox", m_AngleMajorLinesBox);
	m_builder -> get_widget ("AngleMajorLineOffsetBox", m_AngleMajorLineOffsetBox);
	m_builder -> get_widget ("AngleMajorLineOffsetSpinButton0", m_AngleMajorLineOffsetSpinButton0);
	m_builder -> get_widget ("AngleMajorLineOffsetSpinButton1", m_AngleMajorLineOffsetSpinButton1);
	m_builder -> get_widget ("AngleMajorLineOffsetSpinButton2", m_AngleMajorLineOffsetSpinButton2);
	m_builder -> get_widget ("AngleMajorLineEveryBox", m_AngleMajorLineEveryBox);
	m_builder -> get_widget ("AngleMajorLineEverySpinButton0", m_AngleMajorLineEverySpinButton0);
	m_builder -> get_widget ("AngleMajorLineEverySpinButton1", m_AngleMajorLineEverySpinButton1);
	m_builder -> get_widget ("AngleMajorLineEverySpinButton2", m_AngleMajorLineEverySpinButton2);
	m_builder -> get_widget ("AngleMajorGridSpinButton", m_AngleMajorGridSpinButton);
	m_builder -> get_widget ("AngleAddMajorGridButton", m_AngleAddMajorGridButton);
	m_builder -> get_widget ("AngleRemoveMajorGridButton", m_AngleRemoveMajorGridButton);
	m_builder -> get_widget ("AngleColorsExpander", m_AngleColorsExpander);
	m_builder -> get_widget ("AngleColorsBox", m_AngleColorsBox);
	m_builder -> get_widget ("AngleMajorLineColorBox", m_AngleMajorLineColorBox);
	m_builder -> get_widget ("AngleMajorLineColorButton", m_AngleMajorLineColorButton);
	m_builder -> get_widget ("AngleLineColorBox", m_AngleLineColorBox);
	m_builder -> get_widget ("AngleLineColorButton", m_AngleLineColorButton);
	m_builder -> get_widget ("AngleColorBox", m_AngleColorBox);
	m_builder -> get_widget ("AngleColorButton", m_AngleColorButton);
	m_builder -> get_widget ("AxonometricGridExpander", m_AxonometricGridExpander);
	m_builder -> get_widget ("AxonometricGridCheckButton", m_AxonometricGridCheckButton);
	m_builder -> get_widget ("AxonometricGridTransformBox", m_AxonometricGridTransformBox);
	m_builder -> get_widget ("AxonometricGridPlaneComboBoxText", m_AxonometricGridPlaneComboBoxText);
	m_builder -> get_widget ("AxonometricGridTranslationBox", m_AxonometricGridTranslationBox);
	m_builder -> get_widget ("AxonometricGridDimensionBox", m_AxonometricGridDimensionBox);
	m_builder -> get_widget ("AxonometricGridDimensionSpinButton0", m_AxonometricGridDimensionSpinButton0);
	m_builder -> get_widget ("AxonometricGridDimensionSpinButton1", m_AxonometricGridDimensionSpinButton1);
	m_builder -> get_widget ("AxonometricGridScaleBox", m_AxonometricGridScaleBox);
	m_builder -> get_widget ("AxonometricGridUniformScaleButton", m_AxonometricGridUniformScaleButton);
	m_builder -> get_widget ("AxonometricGridUniformScaleImage", m_AxonometricGridUniformScaleImage);
	m_builder -> get_widget ("AxonometricGridAngleBox", m_AxonometricGridAngleBox);
	m_builder -> get_widget ("AxonometricGridAngleGammaSpinButton", m_AxonometricGridAngleGammaSpinButton);
	m_builder -> get_widget ("AxonometricGridMajorLinesExpander", m_AxonometricGridMajorLinesExpander);
	m_builder -> get_widget ("AxonometricGridMajorLinesBox", m_AxonometricGridMajorLinesBox);
	m_builder -> get_widget ("AxonometricGridMajorLineOffsetBox", m_AxonometricGridMajorLineOffsetBox);
	m_builder -> get_widget ("AxonometricGridMajorLineOffsetSpinButton0", m_AxonometricGridMajorLineOffsetSpinButton0);
	m_builder -> get_widget ("AxonometricGridMajorLineOffsetSpinButton1", m_AxonometricGridMajorLineOffsetSpinButton1);
	m_builder -> get_widget ("AxonometricGridMajorLineOffsetSpinButton2", m_AxonometricGridMajorLineOffsetSpinButton2);
	m_builder -> get_widget ("AxonometricGridMajorLineOffsetSpinButton3", m_AxonometricGridMajorLineOffsetSpinButton3);
	m_builder -> get_widget ("AxonometricGridMajorLineEveryBox", m_AxonometricGridMajorLineEveryBox);
	m_builder -> get_widget ("AxonometricGridMajorLineEverySpinButton0", m_AxonometricGridMajorLineEverySpinButton0);
	m_builder -> get_widget ("AxonometricGridMajorLineEverySpinButton1", m_AxonometricGridMajorLineEverySpinButton1);
	m_builder -> get_widget ("AxonometricGridMajorLineEverySpinButton2", m_AxonometricGridMajorLineEverySpinButton2);
	m_builder -> get_widget ("AxonometricGridMajorLineEverySpinButton3", m_AxonometricGridMajorLineEverySpinButton3);
	m_builder -> get_widget ("AxonometricGridMajorGridSpinButton", m_AxonometricGridMajorGridSpinButton);
	m_builder -> get_widget ("AxonometricGridAddMajorGridButton", m_AxonometricGridAddMajorGridButton);
	m_builder -> get_widget ("AxonometricGridRemoveMajorGridButton", m_AxonometricGridRemoveMajorGridButton);
	m_builder -> get_widget ("AxonometricGridColorsExpander", m_AxonometricGridColorsExpander);
	m_builder -> get_widget ("AxonometricGridColorsBox", m_AxonometricGridColorsBox);
	m_builder -> get_widget ("AxonometricGridMajorLineColorBox", m_AxonometricGridMajorLineColorBox);
	m_builder -> get_widget ("AxonometricGridMajorLineColorButton", m_AxonometricGridMajorLineColorButton);
	m_builder -> get_widget ("AxonometricGridLineColorBox", m_AxonometricGridLineColorBox);
	m_builder -> get_widget ("AxonometricGridLineColorButton", m_AxonometricGridLineColorButton);
	m_builder -> get_widget ("AxonometricGridColorBox", m_AxonometricGridColorBox);
	m_builder -> get_widget ("AxonometricGridColorButton", m_AxonometricGridColorButton);
	m_builder -> get_widget ("GridAdditonalScrolledWindow", m_GridAdditonalScrolledWindow);
	m_builder -> get_widget ("GridSnappingExpander", m_GridSnappingExpander);
	m_builder -> get_widget ("GridEnableSnappingCheckButton", m_GridEnableSnappingCheckButton);
	m_builder -> get_widget ("GridSnapToCenterCheckButton", m_GridSnapToCenterCheckButton);
	m_builder -> get_widget ("GridSnapDistanceSpinButton", m_GridSnapDistanceSpinButton);
	m_builder -> get_widget ("GridCollisionExpander", m_GridCollisionExpander);
	m_builder -> get_widget ("GridCollisionCheckButton", m_GridCollisionCheckButton);
	m_builder -> get_widget ("AngleAdditonalScrolledWindow", m_AngleAdditonalScrolledWindow);
	m_builder -> get_widget ("AngleSnappingExpander", m_AngleSnappingExpander);
	m_builder -> get_widget ("AngleEnableSnappingCheckButton", m_AngleEnableSnappingCheckButton);
	m_builder -> get_widget ("AngleSnapToCenterCheckButton", m_AngleSnapToCenterCheckButton);
	m_builder -> get_widget ("AngleSnapDistanceSpinButton", m_AngleSnapDistanceSpinButton);
	m_builder -> get_widget ("AngleCollisionExpander", m_AngleCollisionExpander);
	m_builder -> get_widget ("AngleCollisionCheckButton", m_AngleCollisionCheckButton);
	m_builder -> get_widget ("AxonometricGridAdditonalScrolledWindow", m_AxonometricGridAdditonalScrolledWindow);
	m_builder -> get_widget ("AxonometricGridSnappingExpander", m_AxonometricGridSnappingExpander);
	m_builder -> get_widget ("AxonometricGridEnableSnappingCheckButton", m_AxonometricGridEnableSnappingCheckButton);
	m_builder -> get_widget ("AxonometricGridSnapToCenterCheckButton", m_AxonometricGridSnapToCenterCheckButton);
	m_builder -> get_widget ("AxonometricGridSnapDistanceSpinButton", m_AxonometricGridSnapDistanceSpinButton);
	m_builder -> get_widget ("AxonometricGridCollisiongExpander", m_AxonometricGridCollisiongExpander);
	m_builder -> get_widget ("AxonometricGridCollsionCheckButton", m_AxonometricGridCollsionCheckButton);

	// Connect object Gtk::CheckButton with id 'GridCheckButton'.
	m_GridCheckButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DGridEditorInterface::on_grid_toggled));

	// Connect object Gtk::ComboBoxText with id 'GridPlaneComboBoxText'.
	m_GridPlaneComboBoxText -> signal_changed () .connect (sigc::mem_fun (this, &X3DGridEditorInterface::on_grid_plane_changed));

	// Connect object Gtk::ToggleButton with id 'GridUniformScaleButton'.
	m_GridUniformScaleButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DGridEditorInterface::on_grid_uniform_scale_clicked));

	// Connect object Gtk::SpinButton with id 'GridMajorGridSpinButton'.
	m_GridMajorGridSpinButton -> signal_value_changed () .connect (sigc::mem_fun (this, &X3DGridEditorInterface::on_major_line_grid_value_changed));

	// Connect object Gtk::Button with id 'GridAddMajorGridButton'.
	m_GridAddMajorGridButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DGridEditorInterface::on_add_major_line_grid));
	m_GridRemoveMajorGridButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DGridEditorInterface::on_remove_major_line_grid));

	// Connect object Gtk::CheckButton with id 'AngleCheckButton'.
	m_AngleCheckButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DGridEditorInterface::on_angle_toggled));

	// Connect object Gtk::ComboBoxText with id 'AnglePlaneComboBoxText'.
	m_AnglePlaneComboBoxText -> signal_changed () .connect (sigc::mem_fun (this, &X3DGridEditorInterface::on_angle_plane_changed));

	// Connect object Gtk::ToggleButton with id 'AngleUniformScaleButton'.
	m_AngleUniformScaleButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DGridEditorInterface::on_angle_uniform_scale_clicked));

	// Connect object Gtk::SpinButton with id 'AngleMajorGridSpinButton'.
	m_AngleMajorGridSpinButton -> signal_value_changed () .connect (sigc::mem_fun (this, &X3DGridEditorInterface::on_angle_major_line_grid_value_changed));

	// Connect object Gtk::Button with id 'AngleAddMajorGridButton'.
	m_AngleAddMajorGridButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DGridEditorInterface::on_angle_add_major_line_grid));
	m_AngleRemoveMajorGridButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DGridEditorInterface::on_angle_remove_major_line_grid));

	// Connect object Gtk::CheckButton with id 'AxonometricGridCheckButton'.
	m_AxonometricGridCheckButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DGridEditorInterface::on_axonometric_grid_toggled));

	// Connect object Gtk::ComboBoxText with id 'AxonometricGridPlaneComboBoxText'.
	m_AxonometricGridPlaneComboBoxText -> signal_changed () .connect (sigc::mem_fun (this, &X3DGridEditorInterface::on_axonometric_grid_plane_changed));

	// Connect object Gtk::ToggleButton with id 'AxonometricGridUniformScaleButton'.
	m_AxonometricGridUniformScaleButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DGridEditorInterface::on_axonometric_grid_uniform_scale_clicked));

	// Connect object Gtk::SpinButton with id 'AxonometricGridMajorGridSpinButton'.
	m_AxonometricGridMajorGridSpinButton -> signal_value_changed () .connect (sigc::mem_fun (this, &X3DGridEditorInterface::on_axonometric_major_line_grid_value_changed));

	// Connect object Gtk::Button with id 'AxonometricGridAddMajorGridButton'.
	m_AxonometricGridAddMajorGridButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DGridEditorInterface::on_axonometric_add_major_line_grid));
	m_AxonometricGridRemoveMajorGridButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DGridEditorInterface::on_axonometric_remove_major_line_grid));
}

X3DGridEditorInterface::~X3DGridEditorInterface ()
{
	delete m_Window;
}

} // puck
} // titania
