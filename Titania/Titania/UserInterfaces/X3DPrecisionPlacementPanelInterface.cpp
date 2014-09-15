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
#include "X3DPrecisionPlacementPanelInterface.h"

namespace titania {
namespace puck {

const std::string X3DPrecisionPlacementPanelInterface::m_widgetName = "PrecisionPlacementPanel";

void
X3DPrecisionPlacementPanelInterface::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	// Get objects.
	m_BBoxCenterXAdjustment                = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BBoxCenterXAdjustment"));
	m_BBoxCenterYAdjustment                = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BBoxCenterYAdjustment"));
	m_BBoxCenterZAdjustment                = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BBoxCenterZAdjustment"));
	m_BBoxSizeXAdjustment                  = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BBoxSizeXAdjustment"));
	m_BBoxSizeYAdjustment                  = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BBoxSizeYAdjustment"));
	m_BBoxSizeZAdjustment                  = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BBoxSizeZAdjustment"));
	m_LayoutOffsetXAdjustment              = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("LayoutOffsetXAdjustment"));
	m_LayoutOffsetYAdjustment              = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("LayoutOffsetYAdjustment"));
	m_LayoutSizeXAdjustment                = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("LayoutSizeXAdjustment"));
	m_LayoutSizeYAdjustment                = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("LayoutSizeYAdjustment"));
	m_TransformCenterXAdjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransformCenterXAdjustment"));
	m_TransformCenterYAdjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransformCenterYAdjustment"));
	m_TransformCenterZAdjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransformCenterZAdjustment"));
	m_TransformRotationAAdjustment         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransformRotationAAdjustment"));
	m_TransformRotationXAdjustment         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransformRotationXAdjustment"));
	m_TransformRotationYAdjustment         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransformRotationYAdjustment"));
	m_TransformRotationZAdjustment         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransformRotationZAdjustment"));
	m_TransformScaleOrientationAAdjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransformScaleOrientationAAdjustment"));
	m_TransformScaleOrientationXAdjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransformScaleOrientationXAdjustment"));
	m_TransformScaleOrientationYAdjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransformScaleOrientationYAdjustment"));
	m_TransformScaleOrientationZAdjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransformScaleOrientationZAdjustment"));
	m_TransformScaleXAdjustment            = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransformScaleXAdjustment"));
	m_TransformScaleYAdjustment            = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransformScaleYAdjustment"));
	m_TransformScaleZAdjustment            = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransformScaleZAdjustment"));
	m_TransformTranslationXAdjustment      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransformTranslationXAdjustment"));
	m_TransformTranslationYAdjustment      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransformTranslationYAdjustment"));
	m_TransformTranslationZAdjustment      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransformTranslationZAdjustment"));
	m_ViewportClipBoundaryBottomAdjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ViewportClipBoundaryBottomAdjustment"));
	m_ViewportClipBoundaryLeftAdjustment   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ViewportClipBoundaryLeftAdjustment"));
	m_ViewportClipBoundaryRightAdjustment  = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ViewportClipBoundaryRightAdjustment"));
	m_ViewportClipBoundaryTopAdjustment    = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ViewportClipBoundaryTopAdjustment"));

	// Get widgets.
	m_builder -> get_widget ("Window", m_Window);
	m_builder -> get_widget ("Widget", m_Widget);
	m_builder -> get_widget ("IndexButton", m_IndexButton);
	m_builder -> get_widget ("NameBox", m_NameBox);
	m_builder -> get_widget ("NameEntry", m_NameEntry);
	m_builder -> get_widget ("RenameButton", m_RenameButton);
	m_builder -> get_widget ("TransformExpander", m_TransformExpander);
	m_builder -> get_widget ("TransformBox", m_TransformBox);
	m_builder -> get_widget ("TransformTranslationBox", m_TransformTranslationBox);
	m_builder -> get_widget ("TransformRotationBox", m_TransformRotationBox);
	m_builder -> get_widget ("TransformRotationToolBox", m_TransformRotationToolBox);
	m_builder -> get_widget ("TransformScaleOrientationBox", m_TransformScaleOrientationBox);
	m_builder -> get_widget ("TransformScaleOrientationToolBox", m_TransformScaleOrientationToolBox);
	m_builder -> get_widget ("TransformScaleBox", m_TransformScaleBox);
	m_builder -> get_widget ("TransformCenterBox", m_TransformCenterBox);
	m_builder -> get_widget ("LayoutExpander", m_LayoutExpander);
	m_builder -> get_widget ("CreateLayoutBox", m_CreateLayoutBox);
	m_builder -> get_widget ("LayoutCheckButton", m_LayoutCheckButton);
	m_builder -> get_widget ("LayoutBox", m_LayoutBox);
	m_builder -> get_widget ("LayoutOffsetBox", m_LayoutOffsetBox);
	m_builder -> get_widget ("LayoutOffsetXSpinButton", m_LayoutOffsetXSpinButton);
	m_builder -> get_widget ("LayoutOffsetYSpinButton", m_LayoutOffsetYSpinButton);
	m_builder -> get_widget ("LayoutSizeBox", m_LayoutSizeBox);
	m_builder -> get_widget ("LayoutSizeXSpinButton", m_LayoutSizeXSpinButton);
	m_builder -> get_widget ("LayoutSizeYSpinButton", m_LayoutSizeYSpinButton);
	m_builder -> get_widget ("LayoutAlignXComboBoxText", m_LayoutAlignXComboBoxText);
	m_builder -> get_widget ("LayoutAlignYComboBoxText", m_LayoutAlignYComboBoxText);
	m_builder -> get_widget ("LayoutOffsetUnitsXComboBoxText", m_LayoutOffsetUnitsXComboBoxText);
	m_builder -> get_widget ("LayoutOffsetUnitsYComboBoxText", m_LayoutOffsetUnitsYComboBoxText);
	m_builder -> get_widget ("LayoutSizeUnitsXComboBoxText", m_LayoutSizeUnitsXComboBoxText);
	m_builder -> get_widget ("LayoutSizeUnitsYComboBoxText", m_LayoutSizeUnitsYComboBoxText);
	m_builder -> get_widget ("LayoutScaleModeXComboBoxText", m_LayoutScaleModeXComboBoxText);
	m_builder -> get_widget ("LayoutScaleModeYComboBoxText", m_LayoutScaleModeYComboBoxText);
	m_builder -> get_widget ("ViewportExpander", m_ViewportExpander);
	m_builder -> get_widget ("CreateViewportBox", m_CreateViewportBox);
	m_builder -> get_widget ("ViewportCheckButton", m_ViewportCheckButton);
	m_builder -> get_widget ("ViewportBox", m_ViewportBox);
	m_builder -> get_widget ("ViewportClipBoundaryBox", m_ViewportClipBoundaryBox);
	m_builder -> get_widget ("BoundingBoxExpander", m_BoundingBoxExpander);
	m_builder -> get_widget ("BoundingBoxBox", m_BoundingBoxBox);
	m_builder -> get_widget ("BBoxSizeBox", m_BBoxSizeBox);
	m_builder -> get_widget ("BBoxSizeXSpinButton", m_BBoxSizeXSpinButton);
	m_builder -> get_widget ("BBoxSizeYSpinButton", m_BBoxSizeYSpinButton);
	m_builder -> get_widget ("BBoxSizeZSpinButton", m_BBoxSizeZSpinButton);
	m_builder -> get_widget ("BBoxCenterBox", m_BBoxCenterBox);
	m_builder -> get_widget ("BBoxCenterXSpinButton", m_BBoxCenterXSpinButton);
	m_builder -> get_widget ("BBoxCenterYSpinButton", m_BBoxCenterYSpinButton);
	m_builder -> get_widget ("BBoxCenterZSpinButton", m_BBoxCenterZSpinButton);
	m_builder -> get_widget ("FillBoundingBoxFieldsButton", m_FillBoundingBoxFieldsButton);
	m_IndexButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DPrecisionPlacementPanelInterface::on_index_clicked));

	// Connect object Gtk::CheckButton with id 'LayoutCheckButton'.
	m_LayoutCheckButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DPrecisionPlacementPanelInterface::on_layout_toggled));
	m_ViewportCheckButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DPrecisionPlacementPanelInterface::on_viewport_toggled));

	// Connect object Gtk::Button with id 'FillBoundingBoxFieldsButton'.
	m_FillBoundingBoxFieldsButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DPrecisionPlacementPanelInterface::on_fill_bounding_box_fields_clicked));

	// Call construct handler of base class.
	construct ();
}

X3DPrecisionPlacementPanelInterface::~X3DPrecisionPlacementPanelInterface ()
{
	delete m_Window;
}

} // puck
} // titania
