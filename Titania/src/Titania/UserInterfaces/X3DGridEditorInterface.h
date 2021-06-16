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

#ifndef __TMP_GLAD2CPP_GRID_EDITOR_H__
#define __TMP_GLAD2CPP_GRID_EDITOR_H__

#include "../Base/X3DEditorInterface.h"

#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

/**
 *  Gtk Interface for GridEditor.
 */
class X3DGridEditorInterface :
	public X3DEditorInterface
{
public:

	///  @name Construction

	X3DGridEditorInterface () :
		X3DEditorInterface ()
	{ }

	template <class ... Arguments>
	X3DGridEditorInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DEditorInterface (arguments ...)
	{ create (filename); }

	template <class ... Arguments>
	X3DGridEditorInterface (std::initializer_list <std::string> filenames, const Arguments & ... arguments) :
		X3DEditorInterface (arguments ...)
	{ create (filenames); }

	///  @name Member access

	const Glib::RefPtr <Gtk::Builder> &
	getBuilder () const
	{ return m_builder; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getAngleColorAdjustment () const
	{ return m_AngleColorAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getAngleDimension0Adjustment () const
	{ return m_AngleDimension0Adjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getAngleDimension1Adjustment () const
	{ return m_AngleDimension1Adjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getAngleDimensionYAdjustment () const
	{ return m_AngleDimensionYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getAngleLineColorAdjustment () const
	{ return m_AngleLineColorAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getAngleMajorGridAdjustment () const
	{ return m_AngleMajorGridAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getAngleMajorLineColorAdjustment () const
	{ return m_AngleMajorLineColorAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getAngleMajorLineEvery0Adjustment () const
	{ return m_AngleMajorLineEvery0Adjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getAngleMajorLineEvery1Adjustment () const
	{ return m_AngleMajorLineEvery1Adjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getAngleMajorLineEveryYAdjustment () const
	{ return m_AngleMajorLineEveryYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getAngleMajorLineOffset0Adjustment () const
	{ return m_AngleMajorLineOffset0Adjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getAngleMajorLineOffset1Adjustment () const
	{ return m_AngleMajorLineOffset1Adjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getAngleMajorLineOffsetYAdjustment () const
	{ return m_AngleMajorLineOffsetYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getAngleScaleXAdjustment () const
	{ return m_AngleScaleXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getAngleScaleYAdjustment () const
	{ return m_AngleScaleYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getAngleScaleZAdjustment () const
	{ return m_AngleScaleZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getAngleSnapDistanceAdjustment () const
	{ return m_AngleSnapDistanceAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getAngleTranslationXAdjustment () const
	{ return m_AngleTranslationXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getAngleTranslationYAdjustment () const
	{ return m_AngleTranslationYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getAngleTranslationZAdjustment () const
	{ return m_AngleTranslationZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getAxonometricGridAngleAlphaAdjustment () const
	{ return m_AxonometricGridAngleAlphaAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getAxonometricGridAngleBetaAdjustment () const
	{ return m_AxonometricGridAngleBetaAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getAxonometricGridAngleGammaAdjustment () const
	{ return m_AxonometricGridAngleGammaAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getAxonometricGridColorAdjustment () const
	{ return m_AxonometricGridColorAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getAxonometricGridGapXAdjustment () const
	{ return m_AxonometricGridGapXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getAxonometricGridGapYAdjustment () const
	{ return m_AxonometricGridGapYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getAxonometricGridGapZAdjustment () const
	{ return m_AxonometricGridGapZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getAxonometricGridLineColorAdjustment () const
	{ return m_AxonometricGridLineColorAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getAxonometricGridMajorGridAdjustment () const
	{ return m_AxonometricGridMajorGridAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getAxonometricGridMajorLineColorAdjustment () const
	{ return m_AxonometricGridMajorLineColorAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getAxonometricGridMajorLineEveryTAdjustment () const
	{ return m_AxonometricGridMajorLineEveryTAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getAxonometricGridMajorLineEveryUAdjustment () const
	{ return m_AxonometricGridMajorLineEveryUAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getAxonometricGridMajorLineEveryVAdjustment () const
	{ return m_AxonometricGridMajorLineEveryVAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getAxonometricGridMajorLineEveryYAdjustment () const
	{ return m_AxonometricGridMajorLineEveryYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getAxonometricGridMajorLineOffsetTAdjustment () const
	{ return m_AxonometricGridMajorLineOffsetTAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getAxonometricGridMajorLineOffsetUAdjustment () const
	{ return m_AxonometricGridMajorLineOffsetUAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getAxonometricGridMajorLineOffsetVAdjustment () const
	{ return m_AxonometricGridMajorLineOffsetVAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getAxonometricGridMajorLineOffsetYAdjustment () const
	{ return m_AxonometricGridMajorLineOffsetYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getAxonometricGridScaleXAdjustment () const
	{ return m_AxonometricGridScaleXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getAxonometricGridScaleYAdjustment () const
	{ return m_AxonometricGridScaleYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getAxonometricGridScaleZAdjustment () const
	{ return m_AxonometricGridScaleZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getAxonometricGridSnapDistanceAdjustment () const
	{ return m_AxonometricGridSnapDistanceAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getAxonometricGridTranslationXAdjustment () const
	{ return m_AxonometricGridTranslationXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getAxonometricGridTranslationYAdjustment () const
	{ return m_AxonometricGridTranslationYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getAxonometricGridTranslationZAdjustment () const
	{ return m_AxonometricGridTranslationZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getAxonometricGridTransparencyAdjustment () const
	{ return m_AxonometricGridTransparencyAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getAxonometricGridUVTDimensionAdjustment () const
	{ return m_AxonometricGridUVTDimensionAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getAxonometricGridYDimensionAdjustment () const
	{ return m_AxonometricGridYDimensionAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGridColorAdjustment () const
	{ return m_GridColorAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGridGapXAdjustment () const
	{ return m_GridGapXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGridGapYAdjustment () const
	{ return m_GridGapYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGridGapZAdjustment () const
	{ return m_GridGapZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGridLineColorAdjustment () const
	{ return m_GridLineColorAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGridMajorGridAdjustment () const
	{ return m_GridMajorGridAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGridMajorLineColorAdjustment () const
	{ return m_GridMajorLineColorAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGridMajorLineEveryXAdjustment () const
	{ return m_GridMajorLineEveryXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGridMajorLineEveryYAdjustment () const
	{ return m_GridMajorLineEveryYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGridMajorLineEveryZAdjustment () const
	{ return m_GridMajorLineEveryZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGridMajorLineOffsetXAdjustment () const
	{ return m_GridMajorLineOffsetXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGridMajorLineOffsetYAdjustment () const
	{ return m_GridMajorLineOffsetYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGridMajorLineOffsetZAdjustment () const
	{ return m_GridMajorLineOffsetZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGridScaleXAdjustment () const
	{ return m_GridScaleXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGridScaleYAdjustment () const
	{ return m_GridScaleYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGridScaleZAdjustment () const
	{ return m_GridScaleZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGridSnapDistanceAdjustment () const
	{ return m_GridSnapDistanceAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGridTranslationXAdjustment () const
	{ return m_GridTranslationXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGridTranslationYAdjustment () const
	{ return m_GridTranslationYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGridTranslationZAdjustment () const
	{ return m_GridTranslationZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGridTransparencyAdjustment () const
	{ return m_GridTransparencyAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGridXDimensionAdjustment () const
	{ return m_GridXDimensionAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGridYDimensionAdjustment () const
	{ return m_GridYDimensionAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGridZDimensionAdjustment () const
	{ return m_GridZDimensionAdjustment; }

	Gtk::Window &
	getWindow () const final override
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const final override
	{ return *m_Widget; }

	Gtk::HeaderBar &
	getHeaderBar () const
	{ return *m_HeaderBar; }

	Gtk::Notebook &
	getGridNotebook () const
	{ return *m_GridNotebook; }

	Gtk::Expander &
	getGridExpander () const
	{ return *m_GridExpander; }

	Gtk::CheckButton &
	getGridCheckButton () const
	{ return *m_GridCheckButton; }

	Gtk::Grid &
	getGridTransformBox () const
	{ return *m_GridTransformBox; }

	Gtk::ComboBoxText &
	getGridPlaneComboBoxText () const
	{ return *m_GridPlaneComboBoxText; }

	Gtk::Box &
	getGridTranslationBox () const
	{ return *m_GridTranslationBox; }

	Gtk::Box &
	getGridDimensionBox () const
	{ return *m_GridDimensionBox; }

	Gtk::SpinButton &
	getGridDimensionSpinButton0 () const
	{ return *m_GridDimensionSpinButton0; }

	Gtk::SpinButton &
	getGridDimensionSpinButton1 () const
	{ return *m_GridDimensionSpinButton1; }

	Gtk::SpinButton &
	getGridDimensionSpinButton2 () const
	{ return *m_GridDimensionSpinButton2; }

	Gtk::Box &
	getGridScaleBox () const
	{ return *m_GridScaleBox; }

	Gtk::ToggleButton &
	getGridUniformScaleButton () const
	{ return *m_GridUniformScaleButton; }

	Gtk::Image &
	getGridUniformScaleImage () const
	{ return *m_GridUniformScaleImage; }

	Gtk::Expander &
	getGridMajorLinesExpander () const
	{ return *m_GridMajorLinesExpander; }

	Gtk::Grid &
	getGridMajorLinesBox () const
	{ return *m_GridMajorLinesBox; }

	Gtk::Box &
	getGridMajorLineOffsetBox () const
	{ return *m_GridMajorLineOffsetBox; }

	Gtk::SpinButton &
	getGridMajorLineOffsetSpinButton0 () const
	{ return *m_GridMajorLineOffsetSpinButton0; }

	Gtk::SpinButton &
	getGridMajorLineOffsetSpinButton1 () const
	{ return *m_GridMajorLineOffsetSpinButton1; }

	Gtk::SpinButton &
	getGridMajorLineOffsetSpinButton2 () const
	{ return *m_GridMajorLineOffsetSpinButton2; }

	Gtk::Box &
	getGridMajorLineEveryBox () const
	{ return *m_GridMajorLineEveryBox; }

	Gtk::SpinButton &
	getGridMajorLineEverySpinButton0 () const
	{ return *m_GridMajorLineEverySpinButton0; }

	Gtk::SpinButton &
	getGridMajorLineEverySpinButton1 () const
	{ return *m_GridMajorLineEverySpinButton1; }

	Gtk::SpinButton &
	getGridMajorLineEverySpinButton2 () const
	{ return *m_GridMajorLineEverySpinButton2; }

	Gtk::SpinButton &
	getGridMajorGridSpinButton () const
	{ return *m_GridMajorGridSpinButton; }

	Gtk::Button &
	getGridAddMajorGridButton () const
	{ return *m_GridAddMajorGridButton; }

	Gtk::Button &
	getGridRemoveMajorGridButton () const
	{ return *m_GridRemoveMajorGridButton; }

	Gtk::Expander &
	getGridColorsExpander () const
	{ return *m_GridColorsExpander; }

	Gtk::Grid &
	getGridColorsBox () const
	{ return *m_GridColorsBox; }

	Gtk::Box &
	getGridMajorLineColorBox () const
	{ return *m_GridMajorLineColorBox; }

	Gtk::Button &
	getGridMajorLineColorButton () const
	{ return *m_GridMajorLineColorButton; }

	Gtk::Box &
	getGridLineColorBox () const
	{ return *m_GridLineColorBox; }

	Gtk::Button &
	getGridLineColorButton () const
	{ return *m_GridLineColorButton; }

	Gtk::Box &
	getGridColorBox () const
	{ return *m_GridColorBox; }

	Gtk::Button &
	getGridColorButton () const
	{ return *m_GridColorButton; }

	Gtk::Expander &
	getAngleExpander () const
	{ return *m_AngleExpander; }

	Gtk::CheckButton &
	getAngleCheckButton () const
	{ return *m_AngleCheckButton; }

	Gtk::Grid &
	getAngleTransformBox () const
	{ return *m_AngleTransformBox; }

	Gtk::ComboBoxText &
	getAnglePlaneComboBoxText () const
	{ return *m_AnglePlaneComboBoxText; }

	Gtk::Box &
	getAngleTranslationBox () const
	{ return *m_AngleTranslationBox; }

	Gtk::Box &
	getAngleScaleBox () const
	{ return *m_AngleScaleBox; }

	Gtk::ToggleButton &
	getAngleUniformScaleButton () const
	{ return *m_AngleUniformScaleButton; }

	Gtk::Image &
	getAngleUniformScaleImage () const
	{ return *m_AngleUniformScaleImage; }

	Gtk::Box &
	getAngleDimensionBox () const
	{ return *m_AngleDimensionBox; }

	Gtk::SpinButton &
	getAngleDimensionSpinButton0 () const
	{ return *m_AngleDimensionSpinButton0; }

	Gtk::SpinButton &
	getAngleDimensionSpinButton1 () const
	{ return *m_AngleDimensionSpinButton1; }

	Gtk::SpinButton &
	getAngleDimensionSpinButton2 () const
	{ return *m_AngleDimensionSpinButton2; }

	Gtk::Expander &
	getAngleMajorLinesExpander () const
	{ return *m_AngleMajorLinesExpander; }

	Gtk::Grid &
	getAngleMajorLinesBox () const
	{ return *m_AngleMajorLinesBox; }

	Gtk::Box &
	getAngleMajorLineOffsetBox () const
	{ return *m_AngleMajorLineOffsetBox; }

	Gtk::SpinButton &
	getAngleMajorLineOffsetSpinButton0 () const
	{ return *m_AngleMajorLineOffsetSpinButton0; }

	Gtk::SpinButton &
	getAngleMajorLineOffsetSpinButton1 () const
	{ return *m_AngleMajorLineOffsetSpinButton1; }

	Gtk::SpinButton &
	getAngleMajorLineOffsetSpinButton2 () const
	{ return *m_AngleMajorLineOffsetSpinButton2; }

	Gtk::Box &
	getAngleMajorLineEveryBox () const
	{ return *m_AngleMajorLineEveryBox; }

	Gtk::SpinButton &
	getAngleMajorLineEverySpinButton0 () const
	{ return *m_AngleMajorLineEverySpinButton0; }

	Gtk::SpinButton &
	getAngleMajorLineEverySpinButton1 () const
	{ return *m_AngleMajorLineEverySpinButton1; }

	Gtk::SpinButton &
	getAngleMajorLineEverySpinButton2 () const
	{ return *m_AngleMajorLineEverySpinButton2; }

	Gtk::SpinButton &
	getAngleMajorGridSpinButton () const
	{ return *m_AngleMajorGridSpinButton; }

	Gtk::Button &
	getAngleAddMajorGridButton () const
	{ return *m_AngleAddMajorGridButton; }

	Gtk::Button &
	getAngleRemoveMajorGridButton () const
	{ return *m_AngleRemoveMajorGridButton; }

	Gtk::Expander &
	getAngleColorsExpander () const
	{ return *m_AngleColorsExpander; }

	Gtk::Grid &
	getAngleColorsBox () const
	{ return *m_AngleColorsBox; }

	Gtk::Box &
	getAngleMajorLineColorBox () const
	{ return *m_AngleMajorLineColorBox; }

	Gtk::Button &
	getAngleMajorLineColorButton () const
	{ return *m_AngleMajorLineColorButton; }

	Gtk::Box &
	getAngleLineColorBox () const
	{ return *m_AngleLineColorBox; }

	Gtk::Button &
	getAngleLineColorButton () const
	{ return *m_AngleLineColorButton; }

	Gtk::Box &
	getAngleColorBox () const
	{ return *m_AngleColorBox; }

	Gtk::Button &
	getAngleColorButton () const
	{ return *m_AngleColorButton; }

	Gtk::Expander &
	getAxonometricGridExpander () const
	{ return *m_AxonometricGridExpander; }

	Gtk::CheckButton &
	getAxonometricGridCheckButton () const
	{ return *m_AxonometricGridCheckButton; }

	Gtk::Grid &
	getAxonometricGridTransformBox () const
	{ return *m_AxonometricGridTransformBox; }

	Gtk::ComboBoxText &
	getAxonometricGridPlaneComboBoxText () const
	{ return *m_AxonometricGridPlaneComboBoxText; }

	Gtk::Box &
	getAxonometricGridTranslationBox () const
	{ return *m_AxonometricGridTranslationBox; }

	Gtk::Box &
	getAxonometricGridDimensionBox () const
	{ return *m_AxonometricGridDimensionBox; }

	Gtk::SpinButton &
	getAxonometricGridDimensionSpinButton0 () const
	{ return *m_AxonometricGridDimensionSpinButton0; }

	Gtk::SpinButton &
	getAxonometricGridDimensionSpinButton1 () const
	{ return *m_AxonometricGridDimensionSpinButton1; }

	Gtk::Box &
	getAxonometricGridScaleBox () const
	{ return *m_AxonometricGridScaleBox; }

	Gtk::ToggleButton &
	getAxonometricGridUniformScaleButton () const
	{ return *m_AxonometricGridUniformScaleButton; }

	Gtk::Image &
	getAxonometricGridUniformScaleImage () const
	{ return *m_AxonometricGridUniformScaleImage; }

	Gtk::Box &
	getAxonometricGridAngleBox () const
	{ return *m_AxonometricGridAngleBox; }

	Gtk::SpinButton &
	getAxonometricGridAngleGammaSpinButton () const
	{ return *m_AxonometricGridAngleGammaSpinButton; }

	Gtk::Expander &
	getAxonometricGridMajorLinesExpander () const
	{ return *m_AxonometricGridMajorLinesExpander; }

	Gtk::Grid &
	getAxonometricGridMajorLinesBox () const
	{ return *m_AxonometricGridMajorLinesBox; }

	Gtk::Box &
	getAxonometricGridMajorLineOffsetBox () const
	{ return *m_AxonometricGridMajorLineOffsetBox; }

	Gtk::SpinButton &
	getAxonometricGridMajorLineOffsetSpinButton0 () const
	{ return *m_AxonometricGridMajorLineOffsetSpinButton0; }

	Gtk::SpinButton &
	getAxonometricGridMajorLineOffsetSpinButton1 () const
	{ return *m_AxonometricGridMajorLineOffsetSpinButton1; }

	Gtk::SpinButton &
	getAxonometricGridMajorLineOffsetSpinButton2 () const
	{ return *m_AxonometricGridMajorLineOffsetSpinButton2; }

	Gtk::SpinButton &
	getAxonometricGridMajorLineOffsetSpinButton3 () const
	{ return *m_AxonometricGridMajorLineOffsetSpinButton3; }

	Gtk::Box &
	getAxonometricGridMajorLineEveryBox () const
	{ return *m_AxonometricGridMajorLineEveryBox; }

	Gtk::SpinButton &
	getAxonometricGridMajorLineEverySpinButton0 () const
	{ return *m_AxonometricGridMajorLineEverySpinButton0; }

	Gtk::SpinButton &
	getAxonometricGridMajorLineEverySpinButton1 () const
	{ return *m_AxonometricGridMajorLineEverySpinButton1; }

	Gtk::SpinButton &
	getAxonometricGridMajorLineEverySpinButton2 () const
	{ return *m_AxonometricGridMajorLineEverySpinButton2; }

	Gtk::SpinButton &
	getAxonometricGridMajorLineEverySpinButton3 () const
	{ return *m_AxonometricGridMajorLineEverySpinButton3; }

	Gtk::SpinButton &
	getAxonometricGridMajorGridSpinButton () const
	{ return *m_AxonometricGridMajorGridSpinButton; }

	Gtk::Button &
	getAxonometricGridAddMajorGridButton () const
	{ return *m_AxonometricGridAddMajorGridButton; }

	Gtk::Button &
	getAxonometricGridRemoveMajorGridButton () const
	{ return *m_AxonometricGridRemoveMajorGridButton; }

	Gtk::Expander &
	getAxonometricGridColorsExpander () const
	{ return *m_AxonometricGridColorsExpander; }

	Gtk::Grid &
	getAxonometricGridColorsBox () const
	{ return *m_AxonometricGridColorsBox; }

	Gtk::Box &
	getAxonometricGridMajorLineColorBox () const
	{ return *m_AxonometricGridMajorLineColorBox; }

	Gtk::Button &
	getAxonometricGridMajorLineColorButton () const
	{ return *m_AxonometricGridMajorLineColorButton; }

	Gtk::Box &
	getAxonometricGridLineColorBox () const
	{ return *m_AxonometricGridLineColorBox; }

	Gtk::Button &
	getAxonometricGridLineColorButton () const
	{ return *m_AxonometricGridLineColorButton; }

	Gtk::Box &
	getAxonometricGridColorBox () const
	{ return *m_AxonometricGridColorBox; }

	Gtk::Button &
	getAxonometricGridColorButton () const
	{ return *m_AxonometricGridColorButton; }

	Gtk::ScrolledWindow &
	getGridAdditonalScrolledWindow () const
	{ return *m_GridAdditonalScrolledWindow; }

	Gtk::Expander &
	getGridSnappingExpander () const
	{ return *m_GridSnappingExpander; }

	Gtk::CheckButton &
	getGridEnableSnappingCheckButton () const
	{ return *m_GridEnableSnappingCheckButton; }

	Gtk::CheckButton &
	getGridSnapToCenterCheckButton () const
	{ return *m_GridSnapToCenterCheckButton; }

	Gtk::SpinButton &
	getGridSnapDistanceSpinButton () const
	{ return *m_GridSnapDistanceSpinButton; }

	Gtk::Expander &
	getGridCollisionExpander () const
	{ return *m_GridCollisionExpander; }

	Gtk::CheckButton &
	getGridCollisionCheckButton () const
	{ return *m_GridCollisionCheckButton; }

	Gtk::ScrolledWindow &
	getAngleAdditonalScrolledWindow () const
	{ return *m_AngleAdditonalScrolledWindow; }

	Gtk::Expander &
	getAngleSnappingExpander () const
	{ return *m_AngleSnappingExpander; }

	Gtk::CheckButton &
	getAngleEnableSnappingCheckButton () const
	{ return *m_AngleEnableSnappingCheckButton; }

	Gtk::CheckButton &
	getAngleSnapToCenterCheckButton () const
	{ return *m_AngleSnapToCenterCheckButton; }

	Gtk::SpinButton &
	getAngleSnapDistanceSpinButton () const
	{ return *m_AngleSnapDistanceSpinButton; }

	Gtk::Expander &
	getAngleCollisionExpander () const
	{ return *m_AngleCollisionExpander; }

	Gtk::CheckButton &
	getAngleCollisionCheckButton () const
	{ return *m_AngleCollisionCheckButton; }

	Gtk::ScrolledWindow &
	getAxonometricGridAdditonalScrolledWindow () const
	{ return *m_AxonometricGridAdditonalScrolledWindow; }

	Gtk::Expander &
	getAxonometricGridSnappingExpander () const
	{ return *m_AxonometricGridSnappingExpander; }

	Gtk::CheckButton &
	getAxonometricGridEnableSnappingCheckButton () const
	{ return *m_AxonometricGridEnableSnappingCheckButton; }

	Gtk::CheckButton &
	getAxonometricGridSnapToCenterCheckButton () const
	{ return *m_AxonometricGridSnapToCenterCheckButton; }

	Gtk::SpinButton &
	getAxonometricGridSnapDistanceSpinButton () const
	{ return *m_AxonometricGridSnapDistanceSpinButton; }

	Gtk::Expander &
	getAxonometricGridCollisiongExpander () const
	{ return *m_AxonometricGridCollisiongExpander; }

	Gtk::CheckButton &
	getAxonometricGridCollsionCheckButton () const
	{ return *m_AxonometricGridCollsionCheckButton; }

	///  @name Signal handlers

	virtual
	void
	on_grid_toggled () = 0;

	virtual
	void
	on_grid_plane_changed () = 0;

	virtual
	void
	on_grid_uniform_scale_clicked () = 0;

	virtual
	void
	on_major_line_grid_value_changed () = 0;

	virtual
	void
	on_add_major_line_grid () = 0;

	virtual
	void
	on_remove_major_line_grid () = 0;

	virtual
	void
	on_angle_toggled () = 0;

	virtual
	void
	on_angle_plane_changed () = 0;

	virtual
	void
	on_angle_uniform_scale_clicked () = 0;

	virtual
	void
	on_angle_major_line_grid_value_changed () = 0;

	virtual
	void
	on_angle_add_major_line_grid () = 0;

	virtual
	void
	on_angle_remove_major_line_grid () = 0;

	virtual
	void
	on_axonometric_grid_toggled () = 0;

	virtual
	void
	on_axonometric_grid_plane_changed () = 0;

	virtual
	void
	on_axonometric_grid_uniform_scale_clicked () = 0;

	virtual
	void
	on_axonometric_major_line_grid_value_changed () = 0;

	virtual
	void
	on_axonometric_add_major_line_grid () = 0;

	virtual
	void
	on_axonometric_remove_major_line_grid () = 0;

	///  @name Destruction

	virtual
	~X3DGridEditorInterface () override;


private:

	///  @name Construction

	void
	create (const std::string &);

	void
	create (std::initializer_list <std::string>);

	void
	create ();

	///  @name Static members

	///  @name Members

	Glib::RefPtr <Gtk::Builder> m_builder;
	Glib::RefPtr <Gtk::Adjustment> m_AngleColorAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_AngleDimension0Adjustment;
	Glib::RefPtr <Gtk::Adjustment> m_AngleDimension1Adjustment;
	Glib::RefPtr <Gtk::Adjustment> m_AngleDimensionYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_AngleLineColorAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_AngleMajorGridAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_AngleMajorLineColorAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_AngleMajorLineEvery0Adjustment;
	Glib::RefPtr <Gtk::Adjustment> m_AngleMajorLineEvery1Adjustment;
	Glib::RefPtr <Gtk::Adjustment> m_AngleMajorLineEveryYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_AngleMajorLineOffset0Adjustment;
	Glib::RefPtr <Gtk::Adjustment> m_AngleMajorLineOffset1Adjustment;
	Glib::RefPtr <Gtk::Adjustment> m_AngleMajorLineOffsetYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_AngleScaleXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_AngleScaleYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_AngleScaleZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_AngleSnapDistanceAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_AngleTranslationXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_AngleTranslationYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_AngleTranslationZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_AxonometricGridAngleAlphaAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_AxonometricGridAngleBetaAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_AxonometricGridAngleGammaAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_AxonometricGridColorAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_AxonometricGridGapXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_AxonometricGridGapYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_AxonometricGridGapZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_AxonometricGridLineColorAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_AxonometricGridMajorGridAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_AxonometricGridMajorLineColorAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_AxonometricGridMajorLineEveryTAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_AxonometricGridMajorLineEveryUAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_AxonometricGridMajorLineEveryVAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_AxonometricGridMajorLineEveryYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_AxonometricGridMajorLineOffsetTAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_AxonometricGridMajorLineOffsetUAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_AxonometricGridMajorLineOffsetVAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_AxonometricGridMajorLineOffsetYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_AxonometricGridScaleXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_AxonometricGridScaleYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_AxonometricGridScaleZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_AxonometricGridSnapDistanceAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_AxonometricGridTranslationXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_AxonometricGridTranslationYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_AxonometricGridTranslationZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_AxonometricGridTransparencyAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_AxonometricGridUVTDimensionAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_AxonometricGridYDimensionAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GridColorAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GridGapXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GridGapYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GridGapZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GridLineColorAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GridMajorGridAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GridMajorLineColorAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GridMajorLineEveryXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GridMajorLineEveryYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GridMajorLineEveryZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GridMajorLineOffsetXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GridMajorLineOffsetYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GridMajorLineOffsetZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GridScaleXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GridScaleYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GridScaleZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GridSnapDistanceAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GridTranslationXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GridTranslationYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GridTranslationZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GridTransparencyAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GridXDimensionAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GridYDimensionAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GridZDimensionAdjustment;
	Gtk::Window* m_Window;
	Gtk::Box* m_Widget;
	Gtk::HeaderBar* m_HeaderBar;
	Gtk::Notebook* m_GridNotebook;
	Gtk::Expander* m_GridExpander;
	Gtk::CheckButton* m_GridCheckButton;
	Gtk::Grid* m_GridTransformBox;
	Gtk::ComboBoxText* m_GridPlaneComboBoxText;
	Gtk::Box* m_GridTranslationBox;
	Gtk::Box* m_GridDimensionBox;
	Gtk::SpinButton* m_GridDimensionSpinButton0;
	Gtk::SpinButton* m_GridDimensionSpinButton1;
	Gtk::SpinButton* m_GridDimensionSpinButton2;
	Gtk::Box* m_GridScaleBox;
	Gtk::ToggleButton* m_GridUniformScaleButton;
	Gtk::Image* m_GridUniformScaleImage;
	Gtk::Expander* m_GridMajorLinesExpander;
	Gtk::Grid* m_GridMajorLinesBox;
	Gtk::Box* m_GridMajorLineOffsetBox;
	Gtk::SpinButton* m_GridMajorLineOffsetSpinButton0;
	Gtk::SpinButton* m_GridMajorLineOffsetSpinButton1;
	Gtk::SpinButton* m_GridMajorLineOffsetSpinButton2;
	Gtk::Box* m_GridMajorLineEveryBox;
	Gtk::SpinButton* m_GridMajorLineEverySpinButton0;
	Gtk::SpinButton* m_GridMajorLineEverySpinButton1;
	Gtk::SpinButton* m_GridMajorLineEverySpinButton2;
	Gtk::SpinButton* m_GridMajorGridSpinButton;
	Gtk::Button* m_GridAddMajorGridButton;
	Gtk::Button* m_GridRemoveMajorGridButton;
	Gtk::Expander* m_GridColorsExpander;
	Gtk::Grid* m_GridColorsBox;
	Gtk::Box* m_GridMajorLineColorBox;
	Gtk::Button* m_GridMajorLineColorButton;
	Gtk::Box* m_GridLineColorBox;
	Gtk::Button* m_GridLineColorButton;
	Gtk::Box* m_GridColorBox;
	Gtk::Button* m_GridColorButton;
	Gtk::Expander* m_AngleExpander;
	Gtk::CheckButton* m_AngleCheckButton;
	Gtk::Grid* m_AngleTransformBox;
	Gtk::ComboBoxText* m_AnglePlaneComboBoxText;
	Gtk::Box* m_AngleTranslationBox;
	Gtk::Box* m_AngleScaleBox;
	Gtk::ToggleButton* m_AngleUniformScaleButton;
	Gtk::Image* m_AngleUniformScaleImage;
	Gtk::Box* m_AngleDimensionBox;
	Gtk::SpinButton* m_AngleDimensionSpinButton0;
	Gtk::SpinButton* m_AngleDimensionSpinButton1;
	Gtk::SpinButton* m_AngleDimensionSpinButton2;
	Gtk::Expander* m_AngleMajorLinesExpander;
	Gtk::Grid* m_AngleMajorLinesBox;
	Gtk::Box* m_AngleMajorLineOffsetBox;
	Gtk::SpinButton* m_AngleMajorLineOffsetSpinButton0;
	Gtk::SpinButton* m_AngleMajorLineOffsetSpinButton1;
	Gtk::SpinButton* m_AngleMajorLineOffsetSpinButton2;
	Gtk::Box* m_AngleMajorLineEveryBox;
	Gtk::SpinButton* m_AngleMajorLineEverySpinButton0;
	Gtk::SpinButton* m_AngleMajorLineEverySpinButton1;
	Gtk::SpinButton* m_AngleMajorLineEverySpinButton2;
	Gtk::SpinButton* m_AngleMajorGridSpinButton;
	Gtk::Button* m_AngleAddMajorGridButton;
	Gtk::Button* m_AngleRemoveMajorGridButton;
	Gtk::Expander* m_AngleColorsExpander;
	Gtk::Grid* m_AngleColorsBox;
	Gtk::Box* m_AngleMajorLineColorBox;
	Gtk::Button* m_AngleMajorLineColorButton;
	Gtk::Box* m_AngleLineColorBox;
	Gtk::Button* m_AngleLineColorButton;
	Gtk::Box* m_AngleColorBox;
	Gtk::Button* m_AngleColorButton;
	Gtk::Expander* m_AxonometricGridExpander;
	Gtk::CheckButton* m_AxonometricGridCheckButton;
	Gtk::Grid* m_AxonometricGridTransformBox;
	Gtk::ComboBoxText* m_AxonometricGridPlaneComboBoxText;
	Gtk::Box* m_AxonometricGridTranslationBox;
	Gtk::Box* m_AxonometricGridDimensionBox;
	Gtk::SpinButton* m_AxonometricGridDimensionSpinButton0;
	Gtk::SpinButton* m_AxonometricGridDimensionSpinButton1;
	Gtk::Box* m_AxonometricGridScaleBox;
	Gtk::ToggleButton* m_AxonometricGridUniformScaleButton;
	Gtk::Image* m_AxonometricGridUniformScaleImage;
	Gtk::Box* m_AxonometricGridAngleBox;
	Gtk::SpinButton* m_AxonometricGridAngleGammaSpinButton;
	Gtk::Expander* m_AxonometricGridMajorLinesExpander;
	Gtk::Grid* m_AxonometricGridMajorLinesBox;
	Gtk::Box* m_AxonometricGridMajorLineOffsetBox;
	Gtk::SpinButton* m_AxonometricGridMajorLineOffsetSpinButton0;
	Gtk::SpinButton* m_AxonometricGridMajorLineOffsetSpinButton1;
	Gtk::SpinButton* m_AxonometricGridMajorLineOffsetSpinButton2;
	Gtk::SpinButton* m_AxonometricGridMajorLineOffsetSpinButton3;
	Gtk::Box* m_AxonometricGridMajorLineEveryBox;
	Gtk::SpinButton* m_AxonometricGridMajorLineEverySpinButton0;
	Gtk::SpinButton* m_AxonometricGridMajorLineEverySpinButton1;
	Gtk::SpinButton* m_AxonometricGridMajorLineEverySpinButton2;
	Gtk::SpinButton* m_AxonometricGridMajorLineEverySpinButton3;
	Gtk::SpinButton* m_AxonometricGridMajorGridSpinButton;
	Gtk::Button* m_AxonometricGridAddMajorGridButton;
	Gtk::Button* m_AxonometricGridRemoveMajorGridButton;
	Gtk::Expander* m_AxonometricGridColorsExpander;
	Gtk::Grid* m_AxonometricGridColorsBox;
	Gtk::Box* m_AxonometricGridMajorLineColorBox;
	Gtk::Button* m_AxonometricGridMajorLineColorButton;
	Gtk::Box* m_AxonometricGridLineColorBox;
	Gtk::Button* m_AxonometricGridLineColorButton;
	Gtk::Box* m_AxonometricGridColorBox;
	Gtk::Button* m_AxonometricGridColorButton;
	Gtk::ScrolledWindow* m_GridAdditonalScrolledWindow;
	Gtk::Expander* m_GridSnappingExpander;
	Gtk::CheckButton* m_GridEnableSnappingCheckButton;
	Gtk::CheckButton* m_GridSnapToCenterCheckButton;
	Gtk::SpinButton* m_GridSnapDistanceSpinButton;
	Gtk::Expander* m_GridCollisionExpander;
	Gtk::CheckButton* m_GridCollisionCheckButton;
	Gtk::ScrolledWindow* m_AngleAdditonalScrolledWindow;
	Gtk::Expander* m_AngleSnappingExpander;
	Gtk::CheckButton* m_AngleEnableSnappingCheckButton;
	Gtk::CheckButton* m_AngleSnapToCenterCheckButton;
	Gtk::SpinButton* m_AngleSnapDistanceSpinButton;
	Gtk::Expander* m_AngleCollisionExpander;
	Gtk::CheckButton* m_AngleCollisionCheckButton;
	Gtk::ScrolledWindow* m_AxonometricGridAdditonalScrolledWindow;
	Gtk::Expander* m_AxonometricGridSnappingExpander;
	Gtk::CheckButton* m_AxonometricGridEnableSnappingCheckButton;
	Gtk::CheckButton* m_AxonometricGridSnapToCenterCheckButton;
	Gtk::SpinButton* m_AxonometricGridSnapDistanceSpinButton;
	Gtk::Expander* m_AxonometricGridCollisiongExpander;
	Gtk::CheckButton* m_AxonometricGridCollsionCheckButton;

};

} // puck
} // titania

#endif
