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

#ifndef __TMP_GLAD2CPP_PRECISION_PLACEMENT_PANEL_H__
#define __TMP_GLAD2CPP_PRECISION_PLACEMENT_PANEL_H__

#include "../Base/X3DEditorInterface.h"
#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

/**
 *  Gtk Interface for PrecisionPlacementPanel.
 */
class X3DPrecisionPlacementPanelInterface :
	public X3DEditorInterface
{
public:

	///  @name Construction

	X3DPrecisionPlacementPanelInterface () :
		X3DEditorInterface ()
	{ }

	template <class ... Arguments>
	X3DPrecisionPlacementPanelInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DEditorInterface (arguments ...)
	{ create (filename); }

	template <class ... Arguments>
	X3DPrecisionPlacementPanelInterface (std::initializer_list <std::string> filenames, const Arguments & ... arguments) :
		X3DEditorInterface (arguments ...)
	{ create (filenames); }

	///  @name Member access

	const Glib::RefPtr <Gtk::Builder> &
	getBuilder () const
	{ return m_builder; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getBBoxCenterXAdjustment () const
	{ return m_BBoxCenterXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getBBoxCenterYAdjustment () const
	{ return m_BBoxCenterYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getBBoxCenterZAdjustment () const
	{ return m_BBoxCenterZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getBBoxSizeXAdjustment () const
	{ return m_BBoxSizeXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getBBoxSizeYAdjustment () const
	{ return m_BBoxSizeYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getBBoxSizeZAdjustment () const
	{ return m_BBoxSizeZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getBillboardAxisOfRotationXAdjustment () const
	{ return m_BillboardAxisOfRotationXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getBillboardAxisOfRotationYAdjustment () const
	{ return m_BillboardAxisOfRotationYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getBillboardAxisOfRotationZAdjustment () const
	{ return m_BillboardAxisOfRotationZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getEmitterMassAdjustment () const
	{ return m_EmitterMassAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getEmitterSpeedAdjustment () const
	{ return m_EmitterSpeedAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getEmitterSurfaceAreaAdjustment () const
	{ return m_EmitterSurfaceAreaAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getEmitterVariationAdjustment () const
	{ return m_EmitterVariationAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getEnvironmentalSensorCenterXAdjustment () const
	{ return m_EnvironmentalSensorCenterXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getEnvironmentalSensorCenterYAdjustment () const
	{ return m_EnvironmentalSensorCenterYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getEnvironmentalSensorCenterZAdjustment () const
	{ return m_EnvironmentalSensorCenterZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getEnvironmentalSensorSizeXAdjustment () const
	{ return m_EnvironmentalSensorSizeXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getEnvironmentalSensorSizeYAdjustment () const
	{ return m_EnvironmentalSensorSizeYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getEnvironmentalSensorSizeZAdjustment () const
	{ return m_EnvironmentalSensorSizeZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGeoLocationGeoCoordsXAdjustment () const
	{ return m_GeoLocationGeoCoordsXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGeoLocationGeoCoordsYAdjustment () const
	{ return m_GeoLocationGeoCoordsYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGeoLocationGeoCoordsZAdjustment () const
	{ return m_GeoLocationGeoCoordsZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGeoLocationZoneAdjustment () const
	{ return m_GeoLocationZoneAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGeoTransformGeoCenterXAdjustment () const
	{ return m_GeoTransformGeoCenterXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGeoTransformGeoCenterYAdjustment () const
	{ return m_GeoTransformGeoCenterYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGeoTransformGeoCenterZAdjustment () const
	{ return m_GeoTransformGeoCenterZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGeoTransformRotationAAdjustment () const
	{ return m_GeoTransformRotationAAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGeoTransformRotationXAdjustment () const
	{ return m_GeoTransformRotationXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGeoTransformRotationYAdjustment () const
	{ return m_GeoTransformRotationYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGeoTransformRotationZAdjustment () const
	{ return m_GeoTransformRotationZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGeoTransformScaleOrientationAAdjustment () const
	{ return m_GeoTransformScaleOrientationAAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGeoTransformScaleOrientationXAdjustment () const
	{ return m_GeoTransformScaleOrientationXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGeoTransformScaleOrientationYAdjustment () const
	{ return m_GeoTransformScaleOrientationYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGeoTransformScaleOrientationZAdjustment () const
	{ return m_GeoTransformScaleOrientationZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGeoTransformScaleXAdjustment () const
	{ return m_GeoTransformScaleXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGeoTransformScaleYAdjustment () const
	{ return m_GeoTransformScaleYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGeoTransformScaleZAdjustment () const
	{ return m_GeoTransformScaleZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGeoTransformTranslationXAdjustment () const
	{ return m_GeoTransformTranslationXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGeoTransformTranslationYAdjustment () const
	{ return m_GeoTransformTranslationYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGeoTransformTranslationZAdjustment () const
	{ return m_GeoTransformTranslationZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGeoTransformZoneAdjustment () const
	{ return m_GeoTransformZoneAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGeometrySelectionRotationAAdjustment () const
	{ return m_GeometrySelectionRotationAAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGeometrySelectionRotationXAdjustment () const
	{ return m_GeometrySelectionRotationXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGeometrySelectionRotationYAdjustment () const
	{ return m_GeometrySelectionRotationYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGeometrySelectionRotationZAdjustment () const
	{ return m_GeometrySelectionRotationZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGeometrySelectionScaleXAdjustment () const
	{ return m_GeometrySelectionScaleXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGeometrySelectionScaleYAdjustment () const
	{ return m_GeometrySelectionScaleYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGeometrySelectionScaleZAdjustment () const
	{ return m_GeometrySelectionScaleZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGeometrySelectionTranslationXAdjustment () const
	{ return m_GeometrySelectionTranslationXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGeometrySelectionTranslationYAdjustment () const
	{ return m_GeometrySelectionTranslationYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGeometrySelectionTranslationZAdjustment () const
	{ return m_GeometrySelectionTranslationZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getLODBBoxCenterXAdjustment () const
	{ return m_LODBBoxCenterXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getLODBBoxCenterYAdjustment () const
	{ return m_LODBBoxCenterYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getLODBBoxCenterZAdjustment () const
	{ return m_LODBBoxCenterZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getLODBBoxSizeXAdjustment () const
	{ return m_LODBBoxSizeXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getLODBBoxSizeYAdjustment () const
	{ return m_LODBBoxSizeYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getLODBBoxSizeZAdjustment () const
	{ return m_LODBBoxSizeZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getLODCenterXAdjustment () const
	{ return m_LODCenterXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getLODCenterYAdjustment () const
	{ return m_LODCenterYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getLODCenterZAdjustment () const
	{ return m_LODCenterZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getLODLevelAdjustment () const
	{ return m_LODLevelAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getLODRangeAdjustment () const
	{ return m_LODRangeAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getLayoutOffsetXAdjustment () const
	{ return m_LayoutOffsetXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getLayoutOffsetYAdjustment () const
	{ return m_LayoutOffsetYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getLayoutSizeXAdjustment () const
	{ return m_LayoutSizeXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getLayoutSizeYAdjustment () const
	{ return m_LayoutSizeYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getParticleSystemLifetimeVariationAdjustment () const
	{ return m_ParticleSystemLifetimeVariationAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getParticleSystemMaxParticlesAdjustment () const
	{ return m_ParticleSystemMaxParticlesAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getParticleSystemParticleLifetimeAdjustment () const
	{ return m_ParticleSystemParticleLifetimeAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getParticleSystemParticleSizeXAdjustment () const
	{ return m_ParticleSystemParticleSizeXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getParticleSystemParticleSizeYAdjustment () const
	{ return m_ParticleSystemParticleSizeYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getRangeColorAdjustment () const
	{ return m_RangeColorAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSwitchWhichChoiceAdjustment () const
	{ return m_SwitchWhichChoiceAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTransformCenterXAdjustment () const
	{ return m_TransformCenterXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTransformCenterYAdjustment () const
	{ return m_TransformCenterYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTransformCenterZAdjustment () const
	{ return m_TransformCenterZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTransformRotationAAdjustment () const
	{ return m_TransformRotationAAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTransformRotationXAdjustment () const
	{ return m_TransformRotationXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTransformRotationYAdjustment () const
	{ return m_TransformRotationYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTransformRotationZAdjustment () const
	{ return m_TransformRotationZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTransformScaleOrientationAAdjustment () const
	{ return m_TransformScaleOrientationAAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTransformScaleOrientationXAdjustment () const
	{ return m_TransformScaleOrientationXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTransformScaleOrientationYAdjustment () const
	{ return m_TransformScaleOrientationYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTransformScaleOrientationZAdjustment () const
	{ return m_TransformScaleOrientationZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTransformScaleXAdjustment () const
	{ return m_TransformScaleXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTransformScaleYAdjustment () const
	{ return m_TransformScaleYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTransformScaleZAdjustment () const
	{ return m_TransformScaleZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTransformTranslationXAdjustment () const
	{ return m_TransformTranslationXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTransformTranslationYAdjustment () const
	{ return m_TransformTranslationYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTransformTranslationZAdjustment () const
	{ return m_TransformTranslationZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getViewportClipBoundaryBottomAdjustment () const
	{ return m_ViewportClipBoundaryBottomAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getViewportClipBoundaryLeftAdjustment () const
	{ return m_ViewportClipBoundaryLeftAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getViewportClipBoundaryRightAdjustment () const
	{ return m_ViewportClipBoundaryRightAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getViewportClipBoundaryTopAdjustment () const
	{ return m_ViewportClipBoundaryTopAdjustment; }

	Gtk::Window &
	getLODExtraWindow () const
	{ return *m_LODExtraWindow; }

	Gtk::Grid &
	getRangeColorGrid () const
	{ return *m_RangeColorGrid; }

	Gtk::ScrolledWindow &
	getSkyColorsScrolledWindow () const
	{ return *m_SkyColorsScrolledWindow; }

	Gtk::Button &
	getAddRangeColorButton () const
	{ return *m_AddRangeColorButton; }

	Gtk::Button &
	getRemoveRangeColorButton () const
	{ return *m_RemoveRangeColorButton; }

	Gtk::Box &
	getRangeColorBox () const
	{ return *m_RangeColorBox; }

	Gtk::Button &
	getRangeColorButton () const
	{ return *m_RangeColorButton; }

	Gtk::Window &
	getWindow () const
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const
	{ return *m_Widget; }

	Gtk::HeaderBar &
	getHeaderBar () const
	{ return *m_HeaderBar; }

	Gtk::Button &
	getIndexButton () const
	{ return *m_IndexButton; }

	Gtk::Expander &
	getNodePropertiesExpander () const
	{ return *m_NodePropertiesExpander; }

	Gtk::Box &
	getNameBox () const
	{ return *m_NameBox; }

	Gtk::Entry &
	getNameEntry () const
	{ return *m_NameEntry; }

	Gtk::Button &
	getRenameButton () const
	{ return *m_RenameButton; }

	Gtk::Stack &
	getStack () const
	{ return *m_Stack; }

	Gtk::Expander &
	getTransformExpander () const
	{ return *m_TransformExpander; }

	Gtk::Grid &
	getTransformBox () const
	{ return *m_TransformBox; }

	Gtk::Box &
	getTransformTranslationBox () const
	{ return *m_TransformTranslationBox; }

	Gtk::Box &
	getTransformRotationBox () const
	{ return *m_TransformRotationBox; }

	Gtk::Box &
	getTransformRotationToolBox () const
	{ return *m_TransformRotationToolBox; }

	Gtk::Box &
	getTransformScaleOrientationBox () const
	{ return *m_TransformScaleOrientationBox; }

	Gtk::Box &
	getTransformScaleOrientationToolBox () const
	{ return *m_TransformScaleOrientationToolBox; }

	Gtk::Box &
	getTransformScaleBox () const
	{ return *m_TransformScaleBox; }

	Gtk::ToggleButton &
	getTransformUniformScaleButton () const
	{ return *m_TransformUniformScaleButton; }

	Gtk::Image &
	getTransformUniformScaleImage () const
	{ return *m_TransformUniformScaleImage; }

	Gtk::Box &
	getTransformCenterBox () const
	{ return *m_TransformCenterBox; }

	Gtk::Button &
	getTransformMoveCenterButton () const
	{ return *m_TransformMoveCenterButton; }

	Gtk::Expander &
	getSwitchExpander () const
	{ return *m_SwitchExpander; }

	Gtk::SpinButton &
	getSwitchWhichChoiceSpinButton () const
	{ return *m_SwitchWhichChoiceSpinButton; }

	Gtk::Expander &
	getBillboardExpander () const
	{ return *m_BillboardExpander; }

	Gtk::Grid &
	getBillboardBox () const
	{ return *m_BillboardBox; }

	Gtk::Box &
	getBillboardAxisOfRotationToolBox () const
	{ return *m_BillboardAxisOfRotationToolBox; }

	Gtk::Box &
	getBillboardAxisOfRotationBox () const
	{ return *m_BillboardAxisOfRotationBox; }

	Gtk::ComboBoxText &
	getBillboardAxisOfRotationComboBoxText () const
	{ return *m_BillboardAxisOfRotationComboBoxText; }

	Gtk::Expander &
	getCollisionExpander () const
	{ return *m_CollisionExpander; }

	Gtk::CheckButton &
	getCollisionEnabledCheckButton () const
	{ return *m_CollisionEnabledCheckButton; }

	Gtk::Expander &
	getLODExpander () const
	{ return *m_LODExpander; }

	Gtk::Grid &
	getLODBox () const
	{ return *m_LODBox; }

	Gtk::CheckButton &
	getLODForceTransitionsCheckButton () const
	{ return *m_LODForceTransitionsCheckButton; }

	Gtk::Box &
	getLODNameBox () const
	{ return *m_LODNameBox; }

	Gtk::Entry &
	getLODNameEntry () const
	{ return *m_LODNameEntry; }

	Gtk::Button &
	getLODRenameButton () const
	{ return *m_LODRenameButton; }

	Gtk::Box &
	getLODCenterBox () const
	{ return *m_LODCenterBox; }

	Gtk::SpinButton &
	getLODCenterXSpinButton () const
	{ return *m_LODCenterXSpinButton; }

	Gtk::SpinButton &
	getLODCenterYSpinButton () const
	{ return *m_LODCenterYSpinButton; }

	Gtk::SpinButton &
	getLODCenterZSpinButton () const
	{ return *m_LODCenterZSpinButton; }

	Gtk::Button &
	getLODMoveCenterButton () const
	{ return *m_LODMoveCenterButton; }

	Gtk::SpinButton &
	getLODRangeSpinButton () const
	{ return *m_LODRangeSpinButton; }

	Gtk::SpinButton &
	getLODLevelSpinButton () const
	{ return *m_LODLevelSpinButton; }

	Gtk::CheckButton &
	getLODKeepCurrentLevelCheckButton () const
	{ return *m_LODKeepCurrentLevelCheckButton; }

	Gtk::Box &
	getRangeGradientBox () const
	{ return *m_RangeGradientBox; }

	Gtk::Expander &
	getGeoTransformExpander () const
	{ return *m_GeoTransformExpander; }

	Gtk::Grid &
	getGeoTransformBox () const
	{ return *m_GeoTransformBox; }

	Gtk::Box &
	getGeoTransformTranslationBox () const
	{ return *m_GeoTransformTranslationBox; }

	Gtk::Box &
	getGeoTransformRotationBox () const
	{ return *m_GeoTransformRotationBox; }

	Gtk::Box &
	getGeoTransformRotationToolBox () const
	{ return *m_GeoTransformRotationToolBox; }

	Gtk::Box &
	getGeoTransformScaleOrientationBox () const
	{ return *m_GeoTransformScaleOrientationBox; }

	Gtk::Box &
	getGeoTransformScaleOrientationToolBox () const
	{ return *m_GeoTransformScaleOrientationToolBox; }

	Gtk::Box &
	getGeoTransformScaleBox () const
	{ return *m_GeoTransformScaleBox; }

	Gtk::ToggleButton &
	getGeoTransformUniformScaleButton () const
	{ return *m_GeoTransformUniformScaleButton; }

	Gtk::Image &
	getGeoTransformUniformScaleImage () const
	{ return *m_GeoTransformUniformScaleImage; }

	Gtk::Box &
	getGeoTransformGeoCenterBox () const
	{ return *m_GeoTransformGeoCenterBox; }

	Gtk::SpinButton &
	getGeoTransformGeoCenterXSpinButton () const
	{ return *m_GeoTransformGeoCenterXSpinButton; }

	Gtk::SpinButton &
	getGeoTransformGeoCenterYSpinButton () const
	{ return *m_GeoTransformGeoCenterYSpinButton; }

	Gtk::SpinButton &
	getGeoTransformGeoCenterZSpinButton () const
	{ return *m_GeoTransformGeoCenterZSpinButton; }

	Gtk::ComboBoxText &
	getGeoTransformCoordinateSystemComboBoxText () const
	{ return *m_GeoTransformCoordinateSystemComboBoxText; }

	Gtk::Box &
	getGeoTransformEllipsoidBox () const
	{ return *m_GeoTransformEllipsoidBox; }

	Gtk::ComboBoxText &
	getGeoTransformEllipsoidComboBoxText () const
	{ return *m_GeoTransformEllipsoidComboBoxText; }

	Gtk::ComboBoxText &
	getGeoTransformGDOrderComboBoxText () const
	{ return *m_GeoTransformGDOrderComboBoxText; }

	Gtk::Box &
	getGeoTransformGeoSystemUTMBox () const
	{ return *m_GeoTransformGeoSystemUTMBox; }

	Gtk::ComboBoxText &
	getGeoTransformHemisphereComboBoxText () const
	{ return *m_GeoTransformHemisphereComboBoxText; }

	Gtk::ComboBoxText &
	getGeoTransformUTMOrderComboBoxText () const
	{ return *m_GeoTransformUTMOrderComboBoxText; }

	Gtk::Expander &
	getGeoLocationExpander () const
	{ return *m_GeoLocationExpander; }

	Gtk::Grid &
	getGeoLocationBox () const
	{ return *m_GeoLocationBox; }

	Gtk::ComboBoxText &
	getGeoLocationCoordinateSystemComboBoxText () const
	{ return *m_GeoLocationCoordinateSystemComboBoxText; }

	Gtk::Box &
	getGeoLocationEllipsoidBox () const
	{ return *m_GeoLocationEllipsoidBox; }

	Gtk::ComboBoxText &
	getGeoLocationEllipsoidComboBoxText () const
	{ return *m_GeoLocationEllipsoidComboBoxText; }

	Gtk::ComboBoxText &
	getGeoLocationGDOrderComboBoxText () const
	{ return *m_GeoLocationGDOrderComboBoxText; }

	Gtk::Box &
	getGeoLocationGeoSystemUTMBox () const
	{ return *m_GeoLocationGeoSystemUTMBox; }

	Gtk::ComboBoxText &
	getGeoLocationHemisphereComboBoxText () const
	{ return *m_GeoLocationHemisphereComboBoxText; }

	Gtk::ComboBoxText &
	getGeoLocationUTMOrderComboBoxText () const
	{ return *m_GeoLocationUTMOrderComboBoxText; }

	Gtk::Box &
	getGeoLocationGeoCoordsBox () const
	{ return *m_GeoLocationGeoCoordsBox; }

	Gtk::SpinButton &
	getGeoLocationGeoCoordsXSpinButton () const
	{ return *m_GeoLocationGeoCoordsXSpinButton; }

	Gtk::SpinButton &
	getGeoLocationGeoCoordsYSpinButton () const
	{ return *m_GeoLocationGeoCoordsYSpinButton; }

	Gtk::SpinButton &
	getGeoLocationGeoCoordsZSpinButton () const
	{ return *m_GeoLocationGeoCoordsZSpinButton; }

	Gtk::Expander &
	getEnvironmentalSensorExpander () const
	{ return *m_EnvironmentalSensorExpander; }

	Gtk::Grid &
	getEnvironmentalSensorBox () const
	{ return *m_EnvironmentalSensorBox; }

	Gtk::Box &
	getEnvironmentalSensorSizeBox () const
	{ return *m_EnvironmentalSensorSizeBox; }

	Gtk::ToggleButton &
	getEnvironmentalSensorUniformSizeButton () const
	{ return *m_EnvironmentalSensorUniformSizeButton; }

	Gtk::Image &
	getEnvironmentalSensorUniformSizeImage () const
	{ return *m_EnvironmentalSensorUniformSizeImage; }

	Gtk::Box &
	getEnvironmentalSensorCenterBox () const
	{ return *m_EnvironmentalSensorCenterBox; }

	Gtk::CheckButton &
	getEnvironmentalSensorEnabledCheckButton () const
	{ return *m_EnvironmentalSensorEnabledCheckButton; }

	Gtk::Expander &
	getParticleSystemExpander () const
	{ return *m_ParticleSystemExpander; }

	Gtk::ComboBoxText &
	getParticleSystemGeometryTypeButton () const
	{ return *m_ParticleSystemGeometryTypeButton; }

	Gtk::CheckButton &
	getParticleSystemEnabledCheckButton () const
	{ return *m_ParticleSystemEnabledCheckButton; }

	Gtk::CheckButton &
	getParticleSystemCreateParticlesCheckButton () const
	{ return *m_ParticleSystemCreateParticlesCheckButton; }

	Gtk::SpinButton &
	getParticleSystemMaxParticlesSpinButton () const
	{ return *m_ParticleSystemMaxParticlesSpinButton; }

	Gtk::SpinButton &
	getParticleSystemParticleLifetimeSpinButton () const
	{ return *m_ParticleSystemParticleLifetimeSpinButton; }

	Gtk::SpinButton &
	getParticleSystemLifetimeVariationSpinButton () const
	{ return *m_ParticleSystemLifetimeVariationSpinButton; }

	Gtk::Box &
	getParticleSystemParticleSizeBox () const
	{ return *m_ParticleSystemParticleSizeBox; }

	Gtk::ToggleButton &
	getParticleSystemUniformParticleSizeButton () const
	{ return *m_ParticleSystemUniformParticleSizeButton; }

	Gtk::Image &
	getParticleSystemUniformParticleSizeImage () const
	{ return *m_ParticleSystemUniformParticleSizeImage; }

	Gtk::Expander &
	getLayoutExpander () const
	{ return *m_LayoutExpander; }

	Gtk::Grid &
	getCreateLayoutBox () const
	{ return *m_CreateLayoutBox; }

	Gtk::CheckButton &
	getLayoutCheckButton () const
	{ return *m_LayoutCheckButton; }

	Gtk::Button &
	getLayoutUnlinkButton () const
	{ return *m_LayoutUnlinkButton; }

	Gtk::Grid &
	getLayoutBox () const
	{ return *m_LayoutBox; }

	Gtk::Box &
	getLayoutOffsetBox () const
	{ return *m_LayoutOffsetBox; }

	Gtk::SpinButton &
	getLayoutOffsetXSpinButton () const
	{ return *m_LayoutOffsetXSpinButton; }

	Gtk::SpinButton &
	getLayoutOffsetYSpinButton () const
	{ return *m_LayoutOffsetYSpinButton; }

	Gtk::Box &
	getLayoutSizeBox () const
	{ return *m_LayoutSizeBox; }

	Gtk::SpinButton &
	getLayoutSizeXSpinButton () const
	{ return *m_LayoutSizeXSpinButton; }

	Gtk::SpinButton &
	getLayoutSizeYSpinButton () const
	{ return *m_LayoutSizeYSpinButton; }

	Gtk::ComboBoxText &
	getLayoutAlignXComboBoxText () const
	{ return *m_LayoutAlignXComboBoxText; }

	Gtk::ComboBoxText &
	getLayoutAlignYComboBoxText () const
	{ return *m_LayoutAlignYComboBoxText; }

	Gtk::ComboBoxText &
	getLayoutOffsetUnitsXComboBoxText () const
	{ return *m_LayoutOffsetUnitsXComboBoxText; }

	Gtk::ComboBoxText &
	getLayoutOffsetUnitsYComboBoxText () const
	{ return *m_LayoutOffsetUnitsYComboBoxText; }

	Gtk::ComboBoxText &
	getLayoutSizeUnitsXComboBoxText () const
	{ return *m_LayoutSizeUnitsXComboBoxText; }

	Gtk::ComboBoxText &
	getLayoutSizeUnitsYComboBoxText () const
	{ return *m_LayoutSizeUnitsYComboBoxText; }

	Gtk::ComboBoxText &
	getLayoutScaleModeXComboBoxText () const
	{ return *m_LayoutScaleModeXComboBoxText; }

	Gtk::ComboBoxText &
	getLayoutScaleModeYComboBoxText () const
	{ return *m_LayoutScaleModeYComboBoxText; }

	Gtk::Expander &
	getViewportExpander () const
	{ return *m_ViewportExpander; }

	Gtk::Grid &
	getCreateViewportBox () const
	{ return *m_CreateViewportBox; }

	Gtk::CheckButton &
	getViewportCheckButton () const
	{ return *m_ViewportCheckButton; }

	Gtk::Button &
	getViewportUnlinkButton () const
	{ return *m_ViewportUnlinkButton; }

	Gtk::Grid &
	getViewportBox () const
	{ return *m_ViewportBox; }

	Gtk::Grid &
	getViewportClipBoundaryBox () const
	{ return *m_ViewportClipBoundaryBox; }

	Gtk::SpinButton &
	getViewportClipBoundaryTopSpinButton () const
	{ return *m_ViewportClipBoundaryTopSpinButton; }

	Gtk::SpinButton &
	getViewportClipBoundaryLeftSpinButton () const
	{ return *m_ViewportClipBoundaryLeftSpinButton; }

	Gtk::SpinButton &
	getViewportClipBoundaryRightSpinButton () const
	{ return *m_ViewportClipBoundaryRightSpinButton; }

	Gtk::SpinButton &
	getViewportClipBoundaryBottomSpinButton () const
	{ return *m_ViewportClipBoundaryBottomSpinButton; }

	Gtk::Expander &
	getEmitterExpander () const
	{ return *m_EmitterExpander; }

	Gtk::Grid &
	getCreateEmitterBox () const
	{ return *m_CreateEmitterBox; }

	Gtk::ComboBoxText &
	getEmitterTypeButton () const
	{ return *m_EmitterTypeButton; }

	Gtk::Button &
	getEmitterUnlinkButton () const
	{ return *m_EmitterUnlinkButton; }

	Gtk::Grid &
	getPointEmitterBox () const
	{ return *m_PointEmitterBox; }

	Gtk::Grid &
	getConeEmitterBox () const
	{ return *m_ConeEmitterBox; }

	Gtk::Grid &
	getPolylineEmitterBox () const
	{ return *m_PolylineEmitterBox; }

	Gtk::Grid &
	getVolumeEmitterBox () const
	{ return *m_VolumeEmitterBox; }

	Gtk::Grid &
	getExplosionEmitterBox () const
	{ return *m_ExplosionEmitterBox; }

	Gtk::Grid &
	getEmitterBox () const
	{ return *m_EmitterBox; }

	Gtk::SpinButton &
	getEmitterSpeedSpinButton () const
	{ return *m_EmitterSpeedSpinButton; }

	Gtk::SpinButton &
	getEmitterVariationSpinButton () const
	{ return *m_EmitterVariationSpinButton; }

	Gtk::SpinButton &
	getEmitterMassSpinButton () const
	{ return *m_EmitterMassSpinButton; }

	Gtk::SpinButton &
	getEmitterSurfaceAreaSpinButton () const
	{ return *m_EmitterSurfaceAreaSpinButton; }

	Gtk::Expander &
	getGeometrySelectionExpander () const
	{ return *m_GeometrySelectionExpander; }

	Gtk::Grid &
	getGeometrySelectionBox () const
	{ return *m_GeometrySelectionBox; }

	Gtk::Box &
	getGeometrySelectionTranslationBox () const
	{ return *m_GeometrySelectionTranslationBox; }

	Gtk::SpinButton &
	getGeometrySelectionTranslationXButton () const
	{ return *m_GeometrySelectionTranslationXButton; }

	Gtk::SpinButton &
	getGeometrySelectionTranslationYXButton () const
	{ return *m_GeometrySelectionTranslationYXButton; }

	Gtk::SpinButton &
	getGeometrySelectionTranslationZButton () const
	{ return *m_GeometrySelectionTranslationZButton; }

	Gtk::Box &
	getGeometrySelectionScaleBox () const
	{ return *m_GeometrySelectionScaleBox; }

	Gtk::SpinButton &
	getGeometrySelectionScaleXButton () const
	{ return *m_GeometrySelectionScaleXButton; }

	Gtk::SpinButton &
	getGeometrySelectionScaleYButton () const
	{ return *m_GeometrySelectionScaleYButton; }

	Gtk::SpinButton &
	getGeometrySelectionScaleZButton () const
	{ return *m_GeometrySelectionScaleZButton; }

	Gtk::ToggleButton &
	getGeometrySelectionUniformScaleButton () const
	{ return *m_GeometrySelectionUniformScaleButton; }

	Gtk::Image &
	getGeometrySelectionUniformScaleImage () const
	{ return *m_GeometrySelectionUniformScaleImage; }

	Gtk::Label &
	getSelectedPointsLabel () const
	{ return *m_SelectedPointsLabel; }

	Gtk::Label &
	getSelectedEdgesLabel () const
	{ return *m_SelectedEdgesLabel; }

	Gtk::Label &
	getSelectedHolesLabel () const
	{ return *m_SelectedHolesLabel; }

	Gtk::Label &
	getSelectedFacesLabel () const
	{ return *m_SelectedFacesLabel; }

	Gtk::Box &
	getGeometrySelectionRotationBox () const
	{ return *m_GeometrySelectionRotationBox; }

	Gtk::Box &
	getGeometrySelectionRotationToolBox () const
	{ return *m_GeometrySelectionRotationToolBox; }

	Gtk::SpinButton &
	getGeometrySelectionRotationXButton () const
	{ return *m_GeometrySelectionRotationXButton; }

	Gtk::SpinButton &
	getGeometrySelectionRotationYButton () const
	{ return *m_GeometrySelectionRotationYButton; }

	Gtk::SpinButton &
	getGeometrySelectionRotationZButton () const
	{ return *m_GeometrySelectionRotationZButton; }

	Gtk::SpinButton &
	getGeometrySelectionRotationAButton () const
	{ return *m_GeometrySelectionRotationAButton; }

	Gtk::Expander &
	getBoundingBoxExpander () const
	{ return *m_BoundingBoxExpander; }

	Gtk::Grid &
	getBoundingBoxBox () const
	{ return *m_BoundingBoxBox; }

	Gtk::Box &
	getBBoxSizeBox () const
	{ return *m_BBoxSizeBox; }

	Gtk::SpinButton &
	getBBoxSizeXSpinButton () const
	{ return *m_BBoxSizeXSpinButton; }

	Gtk::SpinButton &
	getBBoxSizeYSpinButton () const
	{ return *m_BBoxSizeYSpinButton; }

	Gtk::SpinButton &
	getBBoxSizeZSpinButton () const
	{ return *m_BBoxSizeZSpinButton; }

	Gtk::ToggleButton &
	getBBoxUniformSizeButton () const
	{ return *m_BBoxUniformSizeButton; }

	Gtk::Image &
	getBBoxUniformSizeImage () const
	{ return *m_BBoxUniformSizeImage; }

	Gtk::Label &
	getBBoxSizeXLabel () const
	{ return *m_BBoxSizeXLabel; }

	Gtk::Label &
	getBBoxSizeYLabel () const
	{ return *m_BBoxSizeYLabel; }

	Gtk::Label &
	getBBoxSizeZLabel () const
	{ return *m_BBoxSizeZLabel; }

	Gtk::Label &
	getBBoxCenterXLabel () const
	{ return *m_BBoxCenterXLabel; }

	Gtk::Label &
	getBBoxCenterYLabel () const
	{ return *m_BBoxCenterYLabel; }

	Gtk::Label &
	getBBoxCenterZLabel () const
	{ return *m_BBoxCenterZLabel; }

	Gtk::Button &
	getFillBoundingBoxFieldsButton () const
	{ return *m_FillBoundingBoxFieldsButton; }

	Gtk::Box &
	getBBoxCenterBox () const
	{ return *m_BBoxCenterBox; }

	Gtk::SpinButton &
	getBBoxCenterXSpinButton () const
	{ return *m_BBoxCenterXSpinButton; }

	Gtk::SpinButton &
	getBBoxCenterYSpinButton () const
	{ return *m_BBoxCenterYSpinButton; }

	Gtk::SpinButton &
	getBBoxCenterZSpinButton () const
	{ return *m_BBoxCenterZSpinButton; }

	///  @name Signal handlers

	virtual
	void
	on_map () = 0;

	virtual
	void
	on_unmap () = 0;

	virtual
	void
	on_index_clicked () = 0;

	virtual
	void
	on_transform_uniform_scale_clicked () = 0;

	virtual
	void
	on_transform_move_center_button () = 0;

	virtual
	void
	on_axisOfRotation_changed () = 0;

	virtual
	void
	on_lod_move_center_button_clicked () = 0;

	virtual
	void
	on_lod_keep_current_level_toggled () = 0;

	virtual
	void
	on_geo_transform_uniform_scale_clicked () = 0;

	virtual
	void
	on_proximity_sensor_uniform_size_clicked () = 0;

	virtual
	void
	on_particle_system_uniform_particle_size_toggled () = 0;

	virtual
	void
	on_layout_toggled () = 0;

	virtual
	void
	on_layout_unlink_clicked () = 0;

	virtual
	void
	on_viewport_toggled () = 0;

	virtual
	void
	on_viewport_unlink_clicked () = 0;

	virtual
	void
	on_emitter_type_changed () = 0;

	virtual
	void
	on_emitter_unlink_clicked () = 0;

	virtual
	bool
	on_geometry_selection_focus_in_event (GdkEventFocus* focus_event) = 0;

	virtual
	void
	on_geometry_selection_uniform_scale_toggled () = 0;

	virtual
	void
	on_bbox_uniform_size_clicked () = 0;

	virtual
	void
	on_fill_bounding_box_fields_clicked () = 0;

	///  @name Destruction

	virtual
	~X3DPrecisionPlacementPanelInterface () override;


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
	Glib::RefPtr <Gtk::Adjustment> m_BBoxCenterXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_BBoxCenterYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_BBoxCenterZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_BBoxSizeXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_BBoxSizeYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_BBoxSizeZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_BillboardAxisOfRotationXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_BillboardAxisOfRotationYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_BillboardAxisOfRotationZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_EmitterMassAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_EmitterSpeedAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_EmitterSurfaceAreaAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_EmitterVariationAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_EnvironmentalSensorCenterXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_EnvironmentalSensorCenterYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_EnvironmentalSensorCenterZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_EnvironmentalSensorSizeXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_EnvironmentalSensorSizeYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_EnvironmentalSensorSizeZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GeoLocationGeoCoordsXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GeoLocationGeoCoordsYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GeoLocationGeoCoordsZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GeoLocationZoneAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GeoTransformGeoCenterXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GeoTransformGeoCenterYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GeoTransformGeoCenterZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GeoTransformRotationAAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GeoTransformRotationXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GeoTransformRotationYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GeoTransformRotationZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GeoTransformScaleOrientationAAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GeoTransformScaleOrientationXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GeoTransformScaleOrientationYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GeoTransformScaleOrientationZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GeoTransformScaleXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GeoTransformScaleYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GeoTransformScaleZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GeoTransformTranslationXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GeoTransformTranslationYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GeoTransformTranslationZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GeoTransformZoneAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GeometrySelectionRotationAAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GeometrySelectionRotationXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GeometrySelectionRotationYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GeometrySelectionRotationZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GeometrySelectionScaleXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GeometrySelectionScaleYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GeometrySelectionScaleZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GeometrySelectionTranslationXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GeometrySelectionTranslationYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GeometrySelectionTranslationZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_LODBBoxCenterXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_LODBBoxCenterYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_LODBBoxCenterZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_LODBBoxSizeXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_LODBBoxSizeYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_LODBBoxSizeZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_LODCenterXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_LODCenterYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_LODCenterZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_LODLevelAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_LODRangeAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_LayoutOffsetXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_LayoutOffsetYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_LayoutSizeXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_LayoutSizeYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_ParticleSystemLifetimeVariationAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_ParticleSystemMaxParticlesAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_ParticleSystemParticleLifetimeAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_ParticleSystemParticleSizeXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_ParticleSystemParticleSizeYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_RangeColorAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_SwitchWhichChoiceAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TransformCenterXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TransformCenterYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TransformCenterZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TransformRotationAAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TransformRotationXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TransformRotationYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TransformRotationZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TransformScaleOrientationAAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TransformScaleOrientationXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TransformScaleOrientationYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TransformScaleOrientationZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TransformScaleXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TransformScaleYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TransformScaleZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TransformTranslationXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TransformTranslationYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TransformTranslationZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_ViewportClipBoundaryBottomAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_ViewportClipBoundaryLeftAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_ViewportClipBoundaryRightAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_ViewportClipBoundaryTopAdjustment;
	Gtk::Window* m_LODExtraWindow;
	Gtk::Grid* m_RangeColorGrid;
	Gtk::ScrolledWindow* m_SkyColorsScrolledWindow;
	Gtk::Button* m_AddRangeColorButton;
	Gtk::Button* m_RemoveRangeColorButton;
	Gtk::Box* m_RangeColorBox;
	Gtk::Button* m_RangeColorButton;
	Gtk::Window* m_Window;
	Gtk::Box* m_Widget;
	Gtk::HeaderBar* m_HeaderBar;
	Gtk::Button* m_IndexButton;
	Gtk::Expander* m_NodePropertiesExpander;
	Gtk::Box* m_NameBox;
	Gtk::Entry* m_NameEntry;
	Gtk::Button* m_RenameButton;
	Gtk::Stack* m_Stack;
	Gtk::Expander* m_TransformExpander;
	Gtk::Grid* m_TransformBox;
	Gtk::Box* m_TransformTranslationBox;
	Gtk::Box* m_TransformRotationBox;
	Gtk::Box* m_TransformRotationToolBox;
	Gtk::Box* m_TransformScaleOrientationBox;
	Gtk::Box* m_TransformScaleOrientationToolBox;
	Gtk::Box* m_TransformScaleBox;
	Gtk::ToggleButton* m_TransformUniformScaleButton;
	Gtk::Image* m_TransformUniformScaleImage;
	Gtk::Box* m_TransformCenterBox;
	Gtk::Button* m_TransformMoveCenterButton;
	Gtk::Expander* m_SwitchExpander;
	Gtk::SpinButton* m_SwitchWhichChoiceSpinButton;
	Gtk::Expander* m_BillboardExpander;
	Gtk::Grid* m_BillboardBox;
	Gtk::Box* m_BillboardAxisOfRotationToolBox;
	Gtk::Box* m_BillboardAxisOfRotationBox;
	Gtk::ComboBoxText* m_BillboardAxisOfRotationComboBoxText;
	Gtk::Expander* m_CollisionExpander;
	Gtk::CheckButton* m_CollisionEnabledCheckButton;
	Gtk::Expander* m_LODExpander;
	Gtk::Grid* m_LODBox;
	Gtk::CheckButton* m_LODForceTransitionsCheckButton;
	Gtk::Box* m_LODNameBox;
	Gtk::Entry* m_LODNameEntry;
	Gtk::Button* m_LODRenameButton;
	Gtk::Box* m_LODCenterBox;
	Gtk::SpinButton* m_LODCenterXSpinButton;
	Gtk::SpinButton* m_LODCenterYSpinButton;
	Gtk::SpinButton* m_LODCenterZSpinButton;
	Gtk::Button* m_LODMoveCenterButton;
	Gtk::SpinButton* m_LODRangeSpinButton;
	Gtk::SpinButton* m_LODLevelSpinButton;
	Gtk::CheckButton* m_LODKeepCurrentLevelCheckButton;
	Gtk::Box* m_RangeGradientBox;
	Gtk::Expander* m_GeoTransformExpander;
	Gtk::Grid* m_GeoTransformBox;
	Gtk::Box* m_GeoTransformTranslationBox;
	Gtk::Box* m_GeoTransformRotationBox;
	Gtk::Box* m_GeoTransformRotationToolBox;
	Gtk::Box* m_GeoTransformScaleOrientationBox;
	Gtk::Box* m_GeoTransformScaleOrientationToolBox;
	Gtk::Box* m_GeoTransformScaleBox;
	Gtk::ToggleButton* m_GeoTransformUniformScaleButton;
	Gtk::Image* m_GeoTransformUniformScaleImage;
	Gtk::Box* m_GeoTransformGeoCenterBox;
	Gtk::SpinButton* m_GeoTransformGeoCenterXSpinButton;
	Gtk::SpinButton* m_GeoTransformGeoCenterYSpinButton;
	Gtk::SpinButton* m_GeoTransformGeoCenterZSpinButton;
	Gtk::ComboBoxText* m_GeoTransformCoordinateSystemComboBoxText;
	Gtk::Box* m_GeoTransformEllipsoidBox;
	Gtk::ComboBoxText* m_GeoTransformEllipsoidComboBoxText;
	Gtk::ComboBoxText* m_GeoTransformGDOrderComboBoxText;
	Gtk::Box* m_GeoTransformGeoSystemUTMBox;
	Gtk::ComboBoxText* m_GeoTransformHemisphereComboBoxText;
	Gtk::ComboBoxText* m_GeoTransformUTMOrderComboBoxText;
	Gtk::Expander* m_GeoLocationExpander;
	Gtk::Grid* m_GeoLocationBox;
	Gtk::ComboBoxText* m_GeoLocationCoordinateSystemComboBoxText;
	Gtk::Box* m_GeoLocationEllipsoidBox;
	Gtk::ComboBoxText* m_GeoLocationEllipsoidComboBoxText;
	Gtk::ComboBoxText* m_GeoLocationGDOrderComboBoxText;
	Gtk::Box* m_GeoLocationGeoSystemUTMBox;
	Gtk::ComboBoxText* m_GeoLocationHemisphereComboBoxText;
	Gtk::ComboBoxText* m_GeoLocationUTMOrderComboBoxText;
	Gtk::Box* m_GeoLocationGeoCoordsBox;
	Gtk::SpinButton* m_GeoLocationGeoCoordsXSpinButton;
	Gtk::SpinButton* m_GeoLocationGeoCoordsYSpinButton;
	Gtk::SpinButton* m_GeoLocationGeoCoordsZSpinButton;
	Gtk::Expander* m_EnvironmentalSensorExpander;
	Gtk::Grid* m_EnvironmentalSensorBox;
	Gtk::Box* m_EnvironmentalSensorSizeBox;
	Gtk::ToggleButton* m_EnvironmentalSensorUniformSizeButton;
	Gtk::Image* m_EnvironmentalSensorUniformSizeImage;
	Gtk::Box* m_EnvironmentalSensorCenterBox;
	Gtk::CheckButton* m_EnvironmentalSensorEnabledCheckButton;
	Gtk::Expander* m_ParticleSystemExpander;
	Gtk::ComboBoxText* m_ParticleSystemGeometryTypeButton;
	Gtk::CheckButton* m_ParticleSystemEnabledCheckButton;
	Gtk::CheckButton* m_ParticleSystemCreateParticlesCheckButton;
	Gtk::SpinButton* m_ParticleSystemMaxParticlesSpinButton;
	Gtk::SpinButton* m_ParticleSystemParticleLifetimeSpinButton;
	Gtk::SpinButton* m_ParticleSystemLifetimeVariationSpinButton;
	Gtk::Box* m_ParticleSystemParticleSizeBox;
	Gtk::ToggleButton* m_ParticleSystemUniformParticleSizeButton;
	Gtk::Image* m_ParticleSystemUniformParticleSizeImage;
	Gtk::Expander* m_LayoutExpander;
	Gtk::Grid* m_CreateLayoutBox;
	Gtk::CheckButton* m_LayoutCheckButton;
	Gtk::Button* m_LayoutUnlinkButton;
	Gtk::Grid* m_LayoutBox;
	Gtk::Box* m_LayoutOffsetBox;
	Gtk::SpinButton* m_LayoutOffsetXSpinButton;
	Gtk::SpinButton* m_LayoutOffsetYSpinButton;
	Gtk::Box* m_LayoutSizeBox;
	Gtk::SpinButton* m_LayoutSizeXSpinButton;
	Gtk::SpinButton* m_LayoutSizeYSpinButton;
	Gtk::ComboBoxText* m_LayoutAlignXComboBoxText;
	Gtk::ComboBoxText* m_LayoutAlignYComboBoxText;
	Gtk::ComboBoxText* m_LayoutOffsetUnitsXComboBoxText;
	Gtk::ComboBoxText* m_LayoutOffsetUnitsYComboBoxText;
	Gtk::ComboBoxText* m_LayoutSizeUnitsXComboBoxText;
	Gtk::ComboBoxText* m_LayoutSizeUnitsYComboBoxText;
	Gtk::ComboBoxText* m_LayoutScaleModeXComboBoxText;
	Gtk::ComboBoxText* m_LayoutScaleModeYComboBoxText;
	Gtk::Expander* m_ViewportExpander;
	Gtk::Grid* m_CreateViewportBox;
	Gtk::CheckButton* m_ViewportCheckButton;
	Gtk::Button* m_ViewportUnlinkButton;
	Gtk::Grid* m_ViewportBox;
	Gtk::Grid* m_ViewportClipBoundaryBox;
	Gtk::SpinButton* m_ViewportClipBoundaryTopSpinButton;
	Gtk::SpinButton* m_ViewportClipBoundaryLeftSpinButton;
	Gtk::SpinButton* m_ViewportClipBoundaryRightSpinButton;
	Gtk::SpinButton* m_ViewportClipBoundaryBottomSpinButton;
	Gtk::Expander* m_EmitterExpander;
	Gtk::Grid* m_CreateEmitterBox;
	Gtk::ComboBoxText* m_EmitterTypeButton;
	Gtk::Button* m_EmitterUnlinkButton;
	Gtk::Grid* m_PointEmitterBox;
	Gtk::Grid* m_ConeEmitterBox;
	Gtk::Grid* m_PolylineEmitterBox;
	Gtk::Grid* m_VolumeEmitterBox;
	Gtk::Grid* m_ExplosionEmitterBox;
	Gtk::Grid* m_EmitterBox;
	Gtk::SpinButton* m_EmitterSpeedSpinButton;
	Gtk::SpinButton* m_EmitterVariationSpinButton;
	Gtk::SpinButton* m_EmitterMassSpinButton;
	Gtk::SpinButton* m_EmitterSurfaceAreaSpinButton;
	Gtk::Expander* m_GeometrySelectionExpander;
	Gtk::Grid* m_GeometrySelectionBox;
	Gtk::Box* m_GeometrySelectionTranslationBox;
	Gtk::SpinButton* m_GeometrySelectionTranslationXButton;
	Gtk::SpinButton* m_GeometrySelectionTranslationYXButton;
	Gtk::SpinButton* m_GeometrySelectionTranslationZButton;
	Gtk::Box* m_GeometrySelectionScaleBox;
	Gtk::SpinButton* m_GeometrySelectionScaleXButton;
	Gtk::SpinButton* m_GeometrySelectionScaleYButton;
	Gtk::SpinButton* m_GeometrySelectionScaleZButton;
	Gtk::ToggleButton* m_GeometrySelectionUniformScaleButton;
	Gtk::Image* m_GeometrySelectionUniformScaleImage;
	Gtk::Label* m_SelectedPointsLabel;
	Gtk::Label* m_SelectedEdgesLabel;
	Gtk::Label* m_SelectedHolesLabel;
	Gtk::Label* m_SelectedFacesLabel;
	Gtk::Box* m_GeometrySelectionRotationBox;
	Gtk::Box* m_GeometrySelectionRotationToolBox;
	Gtk::SpinButton* m_GeometrySelectionRotationXButton;
	Gtk::SpinButton* m_GeometrySelectionRotationYButton;
	Gtk::SpinButton* m_GeometrySelectionRotationZButton;
	Gtk::SpinButton* m_GeometrySelectionRotationAButton;
	Gtk::Expander* m_BoundingBoxExpander;
	Gtk::Grid* m_BoundingBoxBox;
	Gtk::Box* m_BBoxSizeBox;
	Gtk::SpinButton* m_BBoxSizeXSpinButton;
	Gtk::SpinButton* m_BBoxSizeYSpinButton;
	Gtk::SpinButton* m_BBoxSizeZSpinButton;
	Gtk::ToggleButton* m_BBoxUniformSizeButton;
	Gtk::Image* m_BBoxUniformSizeImage;
	Gtk::Label* m_BBoxSizeXLabel;
	Gtk::Label* m_BBoxSizeYLabel;
	Gtk::Label* m_BBoxSizeZLabel;
	Gtk::Label* m_BBoxCenterXLabel;
	Gtk::Label* m_BBoxCenterYLabel;
	Gtk::Label* m_BBoxCenterZLabel;
	Gtk::Button* m_FillBoundingBoxFieldsButton;
	Gtk::Box* m_BBoxCenterBox;
	Gtk::SpinButton* m_BBoxCenterXSpinButton;
	Gtk::SpinButton* m_BBoxCenterYSpinButton;
	Gtk::SpinButton* m_BBoxCenterZSpinButton;

};

} // puck
} // titania

#endif
