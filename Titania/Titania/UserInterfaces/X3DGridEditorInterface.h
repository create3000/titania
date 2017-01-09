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
		X3DEditorInterface (m_widgetName, arguments ...),
		          filename (filename)
	{ create (filename); }

	///  @name Member access

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
	getWindow () const
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const
	{ return *m_Widget; }

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

	Gtk::Box &
	getGridSnappingBox () const
	{ return *m_GridSnappingBox; }

	Gtk::Expander &
	getGridSnappingExpander () const
	{ return *m_GridSnappingExpander; }

	Gtk::CheckButton &
	getGridEnabledCheckButton () const
	{ return *m_GridEnabledCheckButton; }

	Gtk::CheckButton &
	getGridSnapToCenterCheckButton () const
	{ return *m_GridSnapToCenterCheckButton; }

	Gtk::SpinButton &
	getGridSnapDistanceSpinButton () const
	{ return *m_GridSnapDistanceSpinButton; }

	Gtk::Box &
	getAngleSnappingBox () const
	{ return *m_AngleSnappingBox; }

	Gtk::Expander &
	getAngleSnappingExpander () const
	{ return *m_AngleSnappingExpander; }

	Gtk::CheckButton &
	getAngleEnabledCheckButton () const
	{ return *m_AngleEnabledCheckButton; }

	Gtk::CheckButton &
	getAngleSnapToCenterCheckButton () const
	{ return *m_AngleSnapToCenterCheckButton; }

	Gtk::SpinButton &
	getAngleSnapDistanceSpinButton () const
	{ return *m_AngleSnapDistanceSpinButton; }

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

	///  @name Destruction

	virtual
	~X3DGridEditorInterface () override;


private:

	///  @name Construction

	virtual
	void
	construct () final override
	{ X3DEditorInterface::construct (); }

	void
	create (const std::string &);

	///  @name Static members

	static const std::string m_widgetName;

	///  @name Members

	std::string                    filename;
	Glib::RefPtr <Gtk::Builder>    m_builder;
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
	Gtk::Window*                   m_Window;
	Gtk::Box*                      m_Widget;
	Gtk::Notebook*                 m_GridNotebook;
	Gtk::Expander*                 m_GridExpander;
	Gtk::CheckButton*              m_GridCheckButton;
	Gtk::Grid*                     m_GridTransformBox;
	Gtk::ComboBoxText*             m_GridPlaneComboBoxText;
	Gtk::Box*                      m_GridTranslationBox;
	Gtk::Box*                      m_GridDimensionBox;
	Gtk::SpinButton*               m_GridDimensionSpinButton0;
	Gtk::SpinButton*               m_GridDimensionSpinButton1;
	Gtk::SpinButton*               m_GridDimensionSpinButton2;
	Gtk::Box*                      m_GridScaleBox;
	Gtk::ToggleButton*             m_GridUniformScaleButton;
	Gtk::Image*                    m_GridUniformScaleImage;
	Gtk::Expander*                 m_GridMajorLinesExpander;
	Gtk::Grid*                     m_GridMajorLinesBox;
	Gtk::Box*                      m_GridMajorLineOffsetBox;
	Gtk::SpinButton*               m_GridMajorLineOffsetSpinButton0;
	Gtk::SpinButton*               m_GridMajorLineOffsetSpinButton1;
	Gtk::SpinButton*               m_GridMajorLineOffsetSpinButton2;
	Gtk::Box*                      m_GridMajorLineEveryBox;
	Gtk::SpinButton*               m_GridMajorLineEverySpinButton0;
	Gtk::SpinButton*               m_GridMajorLineEverySpinButton1;
	Gtk::SpinButton*               m_GridMajorLineEverySpinButton2;
	Gtk::SpinButton*               m_GridMajorGridSpinButton;
	Gtk::Button*                   m_GridAddMajorGridButton;
	Gtk::Button*                   m_GridRemoveMajorGridButton;
	Gtk::Expander*                 m_GridColorsExpander;
	Gtk::Grid*                     m_GridColorsBox;
	Gtk::Box*                      m_GridMajorLineColorBox;
	Gtk::Button*                   m_GridMajorLineColorButton;
	Gtk::Box*                      m_GridLineColorBox;
	Gtk::Button*                   m_GridLineColorButton;
	Gtk::Box*                      m_GridColorBox;
	Gtk::Button*                   m_GridColorButton;
	Gtk::Expander*                 m_AngleExpander;
	Gtk::CheckButton*              m_AngleCheckButton;
	Gtk::Grid*                     m_AngleTransformBox;
	Gtk::ComboBoxText*             m_AnglePlaneComboBoxText;
	Gtk::Box*                      m_AngleTranslationBox;
	Gtk::Box*                      m_AngleScaleBox;
	Gtk::ToggleButton*             m_AngleUniformScaleButton;
	Gtk::Image*                    m_AngleUniformScaleImage;
	Gtk::Box*                      m_AngleDimensionBox;
	Gtk::SpinButton*               m_AngleDimensionSpinButton0;
	Gtk::SpinButton*               m_AngleDimensionSpinButton1;
	Gtk::SpinButton*               m_AngleDimensionSpinButton2;
	Gtk::Expander*                 m_AngleMajorLinesExpander;
	Gtk::Grid*                     m_AngleMajorLinesBox;
	Gtk::Box*                      m_AngleMajorLineOffsetBox;
	Gtk::SpinButton*               m_AngleMajorLineOffsetSpinButton0;
	Gtk::SpinButton*               m_AngleMajorLineOffsetSpinButton1;
	Gtk::SpinButton*               m_AngleMajorLineOffsetSpinButton2;
	Gtk::Box*                      m_AngleMajorLineEveryBox;
	Gtk::SpinButton*               m_AngleMajorLineEverySpinButton0;
	Gtk::SpinButton*               m_AngleMajorLineEverySpinButton1;
	Gtk::SpinButton*               m_AngleMajorLineEverySpinButton2;
	Gtk::SpinButton*               m_AngleMajorGridSpinButton;
	Gtk::Button*                   m_AngleAddMajorGridButton;
	Gtk::Button*                   m_AngleRemoveMajorGridButton;
	Gtk::Expander*                 m_AngleColorsExpander;
	Gtk::Grid*                     m_AngleColorsBox;
	Gtk::Box*                      m_AngleMajorLineColorBox;
	Gtk::Button*                   m_AngleMajorLineColorButton;
	Gtk::Box*                      m_AngleLineColorBox;
	Gtk::Button*                   m_AngleLineColorButton;
	Gtk::Box*                      m_AngleColorBox;
	Gtk::Button*                   m_AngleColorButton;
	Gtk::Box*                      m_GridSnappingBox;
	Gtk::Expander*                 m_GridSnappingExpander;
	Gtk::CheckButton*              m_GridEnabledCheckButton;
	Gtk::CheckButton*              m_GridSnapToCenterCheckButton;
	Gtk::SpinButton*               m_GridSnapDistanceSpinButton;
	Gtk::Box*                      m_AngleSnappingBox;
	Gtk::Expander*                 m_AngleSnappingExpander;
	Gtk::CheckButton*              m_AngleEnabledCheckButton;
	Gtk::CheckButton*              m_AngleSnapToCenterCheckButton;
	Gtk::SpinButton*               m_AngleSnapDistanceSpinButton;

};

} // puck
} // titania

#endif
