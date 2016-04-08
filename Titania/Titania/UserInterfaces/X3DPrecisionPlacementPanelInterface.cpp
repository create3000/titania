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
	m_BBoxCenterXAdjustment                   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BBoxCenterXAdjustment"));
	m_BBoxCenterYAdjustment                   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BBoxCenterYAdjustment"));
	m_BBoxCenterZAdjustment                   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BBoxCenterZAdjustment"));
	m_BBoxSizeXAdjustment                     = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BBoxSizeXAdjustment"));
	m_BBoxSizeYAdjustment                     = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BBoxSizeYAdjustment"));
	m_BBoxSizeZAdjustment                     = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BBoxSizeZAdjustment"));
	m_BillboardAxisOfRotationXAdjustment      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BillboardAxisOfRotationXAdjustment"));
	m_BillboardAxisOfRotationYAdjustment      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BillboardAxisOfRotationYAdjustment"));
	m_BillboardAxisOfRotationZAdjustment      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BillboardAxisOfRotationZAdjustment"));
	m_GeoLocationGeoCoordsXAdjustment         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoLocationGeoCoordsXAdjustment"));
	m_GeoLocationGeoCoordsYAdjustment         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoLocationGeoCoordsYAdjustment"));
	m_GeoLocationGeoCoordsZAdjustment         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoLocationGeoCoordsZAdjustment"));
	m_GeoLocationZoneAdjustment               = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoLocationZoneAdjustment"));
	m_GeoTransformGeoCenterXAdjustment        = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoTransformGeoCenterXAdjustment"));
	m_GeoTransformGeoCenterYAdjustment        = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoTransformGeoCenterYAdjustment"));
	m_GeoTransformGeoCenterZAdjustment        = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoTransformGeoCenterZAdjustment"));
	m_GeoTransformRotationAAdjustment         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoTransformRotationAAdjustment"));
	m_GeoTransformRotationXAdjustment         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoTransformRotationXAdjustment"));
	m_GeoTransformRotationYAdjustment         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoTransformRotationYAdjustment"));
	m_GeoTransformRotationZAdjustment         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoTransformRotationZAdjustment"));
	m_GeoTransformScaleOrientationAAdjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoTransformScaleOrientationAAdjustment"));
	m_GeoTransformScaleOrientationXAdjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoTransformScaleOrientationXAdjustment"));
	m_GeoTransformScaleOrientationYAdjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoTransformScaleOrientationYAdjustment"));
	m_GeoTransformScaleOrientationZAdjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoTransformScaleOrientationZAdjustment"));
	m_GeoTransformScaleXAdjustment            = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoTransformScaleXAdjustment"));
	m_GeoTransformScaleYAdjustment            = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoTransformScaleYAdjustment"));
	m_GeoTransformScaleZAdjustment            = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoTransformScaleZAdjustment"));
	m_GeoTransformTranslationXAdjustment      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoTransformTranslationXAdjustment"));
	m_GeoTransformTranslationYAdjustment      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoTransformTranslationYAdjustment"));
	m_GeoTransformTranslationZAdjustment      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoTransformTranslationZAdjustment"));
	m_GeoTransformZoneAdjustment              = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoTransformZoneAdjustment"));
	m_LODBBoxCenterXAdjustment                = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("LODBBoxCenterXAdjustment"));
	m_LODBBoxCenterYAdjustment                = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("LODBBoxCenterYAdjustment"));
	m_LODBBoxCenterZAdjustment                = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("LODBBoxCenterZAdjustment"));
	m_LODBBoxSizeXAdjustment                  = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("LODBBoxSizeXAdjustment"));
	m_LODBBoxSizeYAdjustment                  = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("LODBBoxSizeYAdjustment"));
	m_LODBBoxSizeZAdjustment                  = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("LODBBoxSizeZAdjustment"));
	m_LODCenterXAdjustment                    = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("LODCenterXAdjustment"));
	m_LODCenterYAdjustment                    = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("LODCenterYAdjustment"));
	m_LODCenterZAdjustment                    = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("LODCenterZAdjustment"));
	m_LODLevelAdjustment                      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("LODLevelAdjustment"));
	m_LODRangeAdjustment                      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("LODRangeAdjustment"));
	m_LayoutOffsetXAdjustment                 = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("LayoutOffsetXAdjustment"));
	m_LayoutOffsetYAdjustment                 = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("LayoutOffsetYAdjustment"));
	m_LayoutSizeXAdjustment                   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("LayoutSizeXAdjustment"));
	m_LayoutSizeYAdjustment                   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("LayoutSizeYAdjustment"));
	m_RangeColorAdjustment                    = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("RangeColorAdjustment"));
	m_SwitchWhichChoiceAdjustment             = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("SwitchWhichChoiceAdjustment"));
	m_TransformCenterXAdjustment              = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransformCenterXAdjustment"));
	m_TransformCenterYAdjustment              = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransformCenterYAdjustment"));
	m_TransformCenterZAdjustment              = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransformCenterZAdjustment"));
	m_TransformRotationAAdjustment            = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransformRotationAAdjustment"));
	m_TransformRotationXAdjustment            = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransformRotationXAdjustment"));
	m_TransformRotationYAdjustment            = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransformRotationYAdjustment"));
	m_TransformRotationZAdjustment            = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransformRotationZAdjustment"));
	m_TransformScaleOrientationAAdjustment    = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransformScaleOrientationAAdjustment"));
	m_TransformScaleOrientationXAdjustment    = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransformScaleOrientationXAdjustment"));
	m_TransformScaleOrientationYAdjustment    = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransformScaleOrientationYAdjustment"));
	m_TransformScaleOrientationZAdjustment    = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransformScaleOrientationZAdjustment"));
	m_TransformScaleXAdjustment               = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransformScaleXAdjustment"));
	m_TransformScaleYAdjustment               = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransformScaleYAdjustment"));
	m_TransformScaleZAdjustment               = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransformScaleZAdjustment"));
	m_TransformTranslationXAdjustment         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransformTranslationXAdjustment"));
	m_TransformTranslationYAdjustment         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransformTranslationYAdjustment"));
	m_TransformTranslationZAdjustment         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransformTranslationZAdjustment"));
	m_ViewportClipBoundaryBottomAdjustment    = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ViewportClipBoundaryBottomAdjustment"));
	m_ViewportClipBoundaryLeftAdjustment      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ViewportClipBoundaryLeftAdjustment"));
	m_ViewportClipBoundaryRightAdjustment     = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ViewportClipBoundaryRightAdjustment"));
	m_ViewportClipBoundaryTopAdjustment       = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ViewportClipBoundaryTopAdjustment"));

	// Get widgets.
	m_builder -> get_widget ("LODExtraWindow", m_LODExtraWindow);
	m_builder -> get_widget ("RangeColorGrid", m_RangeColorGrid);
	m_builder -> get_widget ("SkyColorsScrolledWindow", m_SkyColorsScrolledWindow);
	m_builder -> get_widget ("AddRangeColorButton", m_AddRangeColorButton);
	m_builder -> get_widget ("RemoveRangeColorButton", m_RemoveRangeColorButton);
	m_builder -> get_widget ("RangeColorBox", m_RangeColorBox);
	m_builder -> get_widget ("RangeColorButton", m_RangeColorButton);
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
	m_builder -> get_widget ("TransformUniformScaleButton", m_TransformUniformScaleButton);
	m_builder -> get_widget ("TransformUniformScaleImage", m_TransformUniformScaleImage);
	m_builder -> get_widget ("TransformCenterBox", m_TransformCenterBox);
	m_builder -> get_widget ("TransformMoveCenterButton", m_TransformMoveCenterButton);
	m_builder -> get_widget ("SwitchExpander", m_SwitchExpander);
	m_builder -> get_widget ("SwitchWhichChoiceSpinButton", m_SwitchWhichChoiceSpinButton);
	m_builder -> get_widget ("BillboardExpander", m_BillboardExpander);
	m_builder -> get_widget ("BillboardBox", m_BillboardBox);
	m_builder -> get_widget ("BillboardAxisOfRotationToolBox", m_BillboardAxisOfRotationToolBox);
	m_builder -> get_widget ("BillboardAxisOfRotationBox", m_BillboardAxisOfRotationBox);
	m_builder -> get_widget ("BillboardAxisOfRotationComboBoxText", m_BillboardAxisOfRotationComboBoxText);
	m_builder -> get_widget ("LODExpander", m_LODExpander);
	m_builder -> get_widget ("LODBox", m_LODBox);
	m_builder -> get_widget ("LODForceTransitionsCheckButton", m_LODForceTransitionsCheckButton);
	m_builder -> get_widget ("LODNameBox", m_LODNameBox);
	m_builder -> get_widget ("LODNameEntry", m_LODNameEntry);
	m_builder -> get_widget ("LODRenameButton", m_LODRenameButton);
	m_builder -> get_widget ("LODCenterBox", m_LODCenterBox);
	m_builder -> get_widget ("LODCenterXSpinButton", m_LODCenterXSpinButton);
	m_builder -> get_widget ("LODCenterYSpinButton", m_LODCenterYSpinButton);
	m_builder -> get_widget ("LODCenterZSpinButton", m_LODCenterZSpinButton);
	m_builder -> get_widget ("LODMoveCenterButton", m_LODMoveCenterButton);
	m_builder -> get_widget ("LODRangeSpinButton", m_LODRangeSpinButton);
	m_builder -> get_widget ("LODLevelSpinButton", m_LODLevelSpinButton);
	m_builder -> get_widget ("LODKeepCurrentLevelCheckButton", m_LODKeepCurrentLevelCheckButton);
	m_builder -> get_widget ("RangeGradientBox", m_RangeGradientBox);
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
	m_builder -> get_widget ("ViewportClipBoundaryTopSpinButton", m_ViewportClipBoundaryTopSpinButton);
	m_builder -> get_widget ("ViewportClipBoundaryLeftSpinButton", m_ViewportClipBoundaryLeftSpinButton);
	m_builder -> get_widget ("ViewportClipBoundaryRightSpinButton", m_ViewportClipBoundaryRightSpinButton);
	m_builder -> get_widget ("ViewportClipBoundaryBottomSpinButton", m_ViewportClipBoundaryBottomSpinButton);
	m_builder -> get_widget ("GeoTransformExpander", m_GeoTransformExpander);
	m_builder -> get_widget ("GeoTransformBox", m_GeoTransformBox);
	m_builder -> get_widget ("GeoTransformTranslationBox", m_GeoTransformTranslationBox);
	m_builder -> get_widget ("GeoTransformRotationBox", m_GeoTransformRotationBox);
	m_builder -> get_widget ("GeoTransformRotationToolBox", m_GeoTransformRotationToolBox);
	m_builder -> get_widget ("GeoTransformScaleOrientationBox", m_GeoTransformScaleOrientationBox);
	m_builder -> get_widget ("GeoTransformScaleOrientationToolBox", m_GeoTransformScaleOrientationToolBox);
	m_builder -> get_widget ("GeoTransformScaleBox", m_GeoTransformScaleBox);
	m_builder -> get_widget ("GeoTransformUniformScaleButton", m_GeoTransformUniformScaleButton);
	m_builder -> get_widget ("GeoTransformUniformScaleImage", m_GeoTransformUniformScaleImage);
	m_builder -> get_widget ("GeoTransformGeoCenterBox", m_GeoTransformGeoCenterBox);
	m_builder -> get_widget ("GeoTransformGeoCenterXSpinButton", m_GeoTransformGeoCenterXSpinButton);
	m_builder -> get_widget ("GeoTransformGeoCenterYSpinButton", m_GeoTransformGeoCenterYSpinButton);
	m_builder -> get_widget ("GeoTransformGeoCenterZSpinButton", m_GeoTransformGeoCenterZSpinButton);
	m_builder -> get_widget ("GeoTransformCoordinateSystemComboBoxText", m_GeoTransformCoordinateSystemComboBoxText);
	m_builder -> get_widget ("GeoTransformEllipsoidBox", m_GeoTransformEllipsoidBox);
	m_builder -> get_widget ("GeoTransformEllipsoidComboBoxText", m_GeoTransformEllipsoidComboBoxText);
	m_builder -> get_widget ("GeoTransformGDOrderComboBoxText", m_GeoTransformGDOrderComboBoxText);
	m_builder -> get_widget ("GeoTransformGeoSystemUTMBox", m_GeoTransformGeoSystemUTMBox);
	m_builder -> get_widget ("GeoTransformHemisphereComboBoxText", m_GeoTransformHemisphereComboBoxText);
	m_builder -> get_widget ("GeoTransformUTMOrderComboBoxText", m_GeoTransformUTMOrderComboBoxText);
	m_builder -> get_widget ("GeoLocationExpander", m_GeoLocationExpander);
	m_builder -> get_widget ("GeoLocationBox", m_GeoLocationBox);
	m_builder -> get_widget ("GeoLocationCoordinateSystemComboBoxText", m_GeoLocationCoordinateSystemComboBoxText);
	m_builder -> get_widget ("GeoLocationEllipsoidBox", m_GeoLocationEllipsoidBox);
	m_builder -> get_widget ("GeoLocationEllipsoidComboBoxText", m_GeoLocationEllipsoidComboBoxText);
	m_builder -> get_widget ("GeoLocationGDOrderComboBoxText", m_GeoLocationGDOrderComboBoxText);
	m_builder -> get_widget ("GeoLocationGeoSystemUTMBox", m_GeoLocationGeoSystemUTMBox);
	m_builder -> get_widget ("GeoLocationHemisphereComboBoxText", m_GeoLocationHemisphereComboBoxText);
	m_builder -> get_widget ("GeoLocationUTMOrderComboBoxText", m_GeoLocationUTMOrderComboBoxText);
	m_builder -> get_widget ("GeoLocationGeoCoordsBox", m_GeoLocationGeoCoordsBox);
	m_builder -> get_widget ("GeoLocationGeoCoordsXSpinButton", m_GeoLocationGeoCoordsXSpinButton);
	m_builder -> get_widget ("GeoLocationGeoCoordsYSpinButton", m_GeoLocationGeoCoordsYSpinButton);
	m_builder -> get_widget ("GeoLocationGeoCoordsZSpinButton", m_GeoLocationGeoCoordsZSpinButton);
	m_builder -> get_widget ("BoundingBoxExpander", m_BoundingBoxExpander);
	m_builder -> get_widget ("BoundingBoxBox", m_BoundingBoxBox);
	m_builder -> get_widget ("BBoxSizeBox", m_BBoxSizeBox);
	m_builder -> get_widget ("BBoxSizeXSpinButton", m_BBoxSizeXSpinButton);
	m_builder -> get_widget ("BBoxSizeYSpinButton", m_BBoxSizeYSpinButton);
	m_builder -> get_widget ("BBoxSizeZSpinButton", m_BBoxSizeZSpinButton);
	m_builder -> get_widget ("BBoxUniformSizeButton", m_BBoxUniformSizeButton);
	m_builder -> get_widget ("BBoxUniformSizeImage", m_BBoxUniformSizeImage);
	m_builder -> get_widget ("BBoxCenterBox", m_BBoxCenterBox);
	m_builder -> get_widget ("BBoxCenterXSpinButton", m_BBoxCenterXSpinButton);
	m_builder -> get_widget ("BBoxCenterYSpinButton", m_BBoxCenterYSpinButton);
	m_builder -> get_widget ("BBoxCenterZSpinButton", m_BBoxCenterZSpinButton);
	m_builder -> get_widget ("FillBoundingBoxFieldsButton", m_FillBoundingBoxFieldsButton);
	m_IndexButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DPrecisionPlacementPanelInterface::on_index_clicked));

	// Connect object Gtk::ToggleButton with id 'TransformUniformScaleButton'.
	m_TransformUniformScaleButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DPrecisionPlacementPanelInterface::on_transform_uniform_scale_clicked));

	// Connect object Gtk::Button with id 'TransformMoveCenterButton'.
	m_TransformMoveCenterButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DPrecisionPlacementPanelInterface::on_transform_move_center_button));

	// Connect object Gtk::ComboBoxText with id 'BillboardAxisOfRotationComboBoxText'.
	m_BillboardAxisOfRotationComboBoxText -> signal_changed () .connect (sigc::mem_fun (*this, &X3DPrecisionPlacementPanelInterface::on_axisOfRotation_changed));

	// Connect object Gtk::Button with id 'LODMoveCenterButton'.
	m_LODMoveCenterButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DPrecisionPlacementPanelInterface::on_lod_move_center_button_clicked));

	// Connect object Gtk::CheckButton with id 'LODKeepCurrentLevelCheckButton'.
	m_LODKeepCurrentLevelCheckButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DPrecisionPlacementPanelInterface::on_lod_keep_current_level_toggled));
	m_LayoutCheckButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DPrecisionPlacementPanelInterface::on_layout_toggled));
	m_ViewportCheckButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DPrecisionPlacementPanelInterface::on_viewport_toggled));

	// Connect object Gtk::ToggleButton with id 'GeoTransformUniformScaleButton'.
	m_GeoTransformUniformScaleButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DPrecisionPlacementPanelInterface::on_geo_transform_uniform_scale_clicked));
	m_BBoxUniformSizeButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DPrecisionPlacementPanelInterface::on_bbox_uniform_size_clicked));

	// Connect object Gtk::Button with id 'FillBoundingBoxFieldsButton'.
	m_FillBoundingBoxFieldsButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DPrecisionPlacementPanelInterface::on_fill_bounding_box_fields_clicked));

	// Call construct handler of base class.
	construct ();
}

X3DPrecisionPlacementPanelInterface::~X3DPrecisionPlacementPanelInterface ()
{
	delete m_LODExtraWindow;
	delete m_Window;
}

} // puck
} // titania
