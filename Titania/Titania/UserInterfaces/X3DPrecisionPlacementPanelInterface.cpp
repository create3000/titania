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

#include "X3DPrecisionPlacementPanelInterface.h"

namespace titania {
namespace puck {

void
X3DPrecisionPlacementPanelInterface::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	create ();
}

void
X3DPrecisionPlacementPanelInterface::create (std::initializer_list <std::string> filenames)
{
	// Create Builder.
	m_builder = Gtk::Builder::create ();

	for (const auto & filename : filenames)
		m_builder -> add_from_file (filename);

	create ();
}

void
X3DPrecisionPlacementPanelInterface::create ()
{
	// Get objects.
	m_BBoxCenterXAdjustment                             = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BBoxCenterXAdjustment"));
	m_BBoxCenterYAdjustment                             = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BBoxCenterYAdjustment"));
	m_BBoxCenterZAdjustment                             = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BBoxCenterZAdjustment"));
	m_BBoxSizeXAdjustment                               = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BBoxSizeXAdjustment"));
	m_BBoxSizeYAdjustment                               = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BBoxSizeYAdjustment"));
	m_BBoxSizeZAdjustment                               = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BBoxSizeZAdjustment"));
	m_BillboardAxisOfRotationXAdjustment                = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BillboardAxisOfRotationXAdjustment"));
	m_BillboardAxisOfRotationYAdjustment                = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BillboardAxisOfRotationYAdjustment"));
	m_BillboardAxisOfRotationZAdjustment                = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BillboardAxisOfRotationZAdjustment"));
	m_ColorKeyAdjustment                                = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ColorKeyAdjustment"));
	m_ColorRampAdjustment                               = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ColorRampAdjustment"));
	m_ColorRampRGBAAdjustment                           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ColorRampRGBAAdjustment"));
	m_ConeEmitterAngleAdjustment                        = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ConeEmitterAngleAdjustment"));
	m_ConeEmitterDirectionXAdjustment                   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ConeEmitterDirectionXAdjustment"));
	m_ConeEmitterDirectionYAdjustment                   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ConeEmitterDirectionYAdjustment"));
	m_ConeEmitterDirectionZAdjustment                   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ConeEmitterDirectionZAdjustment"));
	m_ConeEmitterPositionXAdjustment                    = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ConeEmitterPositionXAdjustment"));
	m_ConeEmitterPositionYAdjustment                    = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ConeEmitterPositionYAdjustment"));
	m_ConeEmitterPositionZAdjustment                    = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ConeEmitterPositionZAdjustment"));
	m_EmitterMassAdjustment                             = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("EmitterMassAdjustment"));
	m_EmitterSpeedAdjustment                            = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("EmitterSpeedAdjustment"));
	m_EmitterSurfaceAreaAdjustment                      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("EmitterSurfaceAreaAdjustment"));
	m_EmitterVariationAdjustment                        = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("EmitterVariationAdjustment"));
	m_EnvironmentalSensorCenterXAdjustment              = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("EnvironmentalSensorCenterXAdjustment"));
	m_EnvironmentalSensorCenterYAdjustment              = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("EnvironmentalSensorCenterYAdjustment"));
	m_EnvironmentalSensorCenterZAdjustment              = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("EnvironmentalSensorCenterZAdjustment"));
	m_EnvironmentalSensorSizeXAdjustment                = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("EnvironmentalSensorSizeXAdjustment"));
	m_EnvironmentalSensorSizeYAdjustment                = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("EnvironmentalSensorSizeYAdjustment"));
	m_EnvironmentalSensorSizeZAdjustment                = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("EnvironmentalSensorSizeZAdjustment"));
	m_ExplosionEmitterPositionXAdjustment               = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ExplosionEmitterPositionXAdjustment"));
	m_ExplosionEmitterPositionYAdjustment               = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ExplosionEmitterPositionYAdjustment"));
	m_ExplosionEmitterPositionZAdjustment               = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ExplosionEmitterPositionZAdjustment"));
	m_GeoLocationGeoCoordsXAdjustment                   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoLocationGeoCoordsXAdjustment"));
	m_GeoLocationGeoCoordsYAdjustment                   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoLocationGeoCoordsYAdjustment"));
	m_GeoLocationGeoCoordsZAdjustment                   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoLocationGeoCoordsZAdjustment"));
	m_GeoLocationZoneAdjustment                         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoLocationZoneAdjustment"));
	m_GeoTransformGeoCenterXAdjustment                  = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoTransformGeoCenterXAdjustment"));
	m_GeoTransformGeoCenterYAdjustment                  = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoTransformGeoCenterYAdjustment"));
	m_GeoTransformGeoCenterZAdjustment                  = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoTransformGeoCenterZAdjustment"));
	m_GeoTransformRotationAAdjustment                   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoTransformRotationAAdjustment"));
	m_GeoTransformRotationEulerXAdjustment              = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoTransformRotationEulerXAdjustment"));
	m_GeoTransformRotationEulerYAdjustment              = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoTransformRotationEulerYAdjustment"));
	m_GeoTransformRotationEulerZAdjustment              = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoTransformRotationEulerZAdjustment"));
	m_GeoTransformRotationXAdjustment                   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoTransformRotationXAdjustment"));
	m_GeoTransformRotationYAdjustment                   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoTransformRotationYAdjustment"));
	m_GeoTransformRotationZAdjustment                   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoTransformRotationZAdjustment"));
	m_GeoTransformScaleOrientationAAdjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoTransformScaleOrientationAAdjustment"));
	m_GeoTransformScaleOrientationEulerXAdjustment      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoTransformScaleOrientationEulerXAdjustment"));
	m_GeoTransformScaleOrientationEulerYAdjustment      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoTransformScaleOrientationEulerYAdjustment"));
	m_GeoTransformScaleOrientationEulerZAdjustment      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoTransformScaleOrientationEulerZAdjustment"));
	m_GeoTransformScaleOrientationXAdjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoTransformScaleOrientationXAdjustment"));
	m_GeoTransformScaleOrientationYAdjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoTransformScaleOrientationYAdjustment"));
	m_GeoTransformScaleOrientationZAdjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoTransformScaleOrientationZAdjustment"));
	m_GeoTransformScaleXAdjustment                      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoTransformScaleXAdjustment"));
	m_GeoTransformScaleYAdjustment                      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoTransformScaleYAdjustment"));
	m_GeoTransformScaleZAdjustment                      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoTransformScaleZAdjustment"));
	m_GeoTransformTranslationXAdjustment                = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoTransformTranslationXAdjustment"));
	m_GeoTransformTranslationYAdjustment                = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoTransformTranslationYAdjustment"));
	m_GeoTransformTranslationZAdjustment                = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoTransformTranslationZAdjustment"));
	m_GeoTransformZoneAdjustment                        = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoTransformZoneAdjustment"));
	m_GeometrySelectionRotationAAdjustment              = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeometrySelectionRotationAAdjustment"));
	m_GeometrySelectionRotationEulerXAdjustment         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeometrySelectionRotationEulerXAdjustment"));
	m_GeometrySelectionRotationEulerYAdjustment         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeometrySelectionRotationEulerYAdjustment"));
	m_GeometrySelectionRotationEulerZAdjustment         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeometrySelectionRotationEulerZAdjustment"));
	m_GeometrySelectionRotationXAdjustment              = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeometrySelectionRotationXAdjustment"));
	m_GeometrySelectionRotationYAdjustment              = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeometrySelectionRotationYAdjustment"));
	m_GeometrySelectionRotationZAdjustment              = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeometrySelectionRotationZAdjustment"));
	m_GeometrySelectionScaleXAdjustment                 = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeometrySelectionScaleXAdjustment"));
	m_GeometrySelectionScaleYAdjustment                 = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeometrySelectionScaleYAdjustment"));
	m_GeometrySelectionScaleZAdjustment                 = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeometrySelectionScaleZAdjustment"));
	m_GeometrySelectionTranslationXAdjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeometrySelectionTranslationXAdjustment"));
	m_GeometrySelectionTranslationYAdjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeometrySelectionTranslationYAdjustment"));
	m_GeometrySelectionTranslationZAdjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeometrySelectionTranslationZAdjustment"));
	m_LODBBoxCenterXAdjustment                          = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("LODBBoxCenterXAdjustment"));
	m_LODBBoxCenterYAdjustment                          = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("LODBBoxCenterYAdjustment"));
	m_LODBBoxCenterZAdjustment                          = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("LODBBoxCenterZAdjustment"));
	m_LODBBoxSizeXAdjustment                            = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("LODBBoxSizeXAdjustment"));
	m_LODBBoxSizeYAdjustment                            = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("LODBBoxSizeYAdjustment"));
	m_LODBBoxSizeZAdjustment                            = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("LODBBoxSizeZAdjustment"));
	m_LODCenterXAdjustment                              = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("LODCenterXAdjustment"));
	m_LODCenterYAdjustment                              = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("LODCenterYAdjustment"));
	m_LODCenterZAdjustment                              = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("LODCenterZAdjustment"));
	m_LODLevelAdjustment                                = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("LODLevelAdjustment"));
	m_LODRangeAdjustment                                = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("LODRangeAdjustment"));
	m_LayoutOffsetXAdjustment                           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("LayoutOffsetXAdjustment"));
	m_LayoutOffsetYAdjustment                           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("LayoutOffsetYAdjustment"));
	m_LayoutSizeXAdjustment                             = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("LayoutSizeXAdjustment"));
	m_LayoutSizeYAdjustment                             = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("LayoutSizeYAdjustment"));
	m_ParticleSystemLifetimeVariationAdjustment         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ParticleSystemLifetimeVariationAdjustment"));
	m_ParticleSystemMaxParticlesAdjustment              = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ParticleSystemMaxParticlesAdjustment"));
	m_ParticleSystemParticleLifetimeAdjustment          = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ParticleSystemParticleLifetimeAdjustment"));
	m_ParticleSystemParticleSizeXAdjustment             = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ParticleSystemParticleSizeXAdjustment"));
	m_ParticleSystemParticleSizeYAdjustment             = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ParticleSystemParticleSizeYAdjustment"));
	m_PointEmitterDirectionXAdjustment                  = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("PointEmitterDirectionXAdjustment"));
	m_PointEmitterDirectionYAdjustment                  = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("PointEmitterDirectionYAdjustment"));
	m_PointEmitterDirectionZAdjustment                  = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("PointEmitterDirectionZAdjustment"));
	m_PointEmitterPositionXAdjustment                   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("PointEmitterPositionXAdjustment"));
	m_PointEmitterPositionYAdjustment                   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("PointEmitterPositionYAdjustment"));
	m_PointEmitterPositionZAdjustment                   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("PointEmitterPositionZAdjustment"));
	m_PolylineEmitterDirectionXAdjustment               = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("PolylineEmitterDirectionXAdjustment"));
	m_PolylineEmitterDirectionYAdjustment               = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("PolylineEmitterDirectionYAdjustment"));
	m_PolylineEmitterDirectionZAdjustment               = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("PolylineEmitterDirectionZAdjustment"));
	m_RangeColorAdjustment                              = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("RangeColorAdjustment"));
	m_SwitchWhichChoiceAdjustment                       = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("SwitchWhichChoiceAdjustment"));
	m_TextureProjectorPerspectiveAspectRatioAdjustment  = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureProjectorPerspectiveAspectRatioAdjustment"));
	m_TextureProjectorPerspectiveDescriptionTextBuffer  = Glib::RefPtr <Gtk::TextBuffer>::cast_dynamic (m_builder -> get_object ("TextureProjectorPerspectiveDescriptionTextBuffer"));
	m_TextureProjectorPerspectiveDirectionXAdjustment   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureProjectorPerspectiveDirectionXAdjustment"));
	m_TextureProjectorPerspectiveDirectionYAdjustment   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureProjectorPerspectiveDirectionYAdjustment"));
	m_TextureProjectorPerspectiveDirectionZAdjustment   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureProjectorPerspectiveDirectionZAdjustment"));
	m_TextureProjectorPerspectiveFarDistanceAdjustment  = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureProjectorPerspectiveFarDistanceAdjustment"));
	m_TextureProjectorPerspectiveFieldOfViewAdjustment  = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureProjectorPerspectiveFieldOfViewAdjustment"));
	m_TextureProjectorPerspectiveLocationXAdjustment    = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureProjectorPerspectiveLocationXAdjustment"));
	m_TextureProjectorPerspectiveLocationYAdjustment    = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureProjectorPerspectiveLocationYAdjustment"));
	m_TextureProjectorPerspectiveLocationZAdjustment    = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureProjectorPerspectiveLocationZAdjustment"));
	m_TextureProjectorPerspectiveNearDistanceAdjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureProjectorPerspectiveNearDistanceAdjustment"));
	m_TextureProjectorPerspectiveUpVectorXAdjustment    = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureProjectorPerspectiveUpVectorXAdjustment"));
	m_TextureProjectorPerspectiveUpVectorYAdjustment    = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureProjectorPerspectiveUpVectorYAdjustment"));
	m_TextureProjectorPerspectiveUpVectorZAdjustment    = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureProjectorPerspectiveUpVectorZAdjustment"));
	m_TransformCenterXAdjustment                        = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransformCenterXAdjustment"));
	m_TransformCenterYAdjustment                        = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransformCenterYAdjustment"));
	m_TransformCenterZAdjustment                        = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransformCenterZAdjustment"));
	m_TransformRotationAAdjustment                      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransformRotationAAdjustment"));
	m_TransformRotationEulerXAdjustment                 = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransformRotationEulerXAdjustment"));
	m_TransformRotationEulerYAdjustment                 = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransformRotationEulerYAdjustment"));
	m_TransformRotationEulerZAdjustment                 = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransformRotationEulerZAdjustment"));
	m_TransformRotationXAdjustment                      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransformRotationXAdjustment"));
	m_TransformRotationYAdjustment                      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransformRotationYAdjustment"));
	m_TransformRotationZAdjustment                      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransformRotationZAdjustment"));
	m_TransformScaleOrientationAAdjustment              = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransformScaleOrientationAAdjustment"));
	m_TransformScaleOrientationEulerXAdjustment         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransformScaleOrientationEulerXAdjustment"));
	m_TransformScaleOrientationEulerYAdjustment         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransformScaleOrientationEulerYAdjustment"));
	m_TransformScaleOrientationEulerZAdjustment         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransformScaleOrientationEulerZAdjustment"));
	m_TransformScaleOrientationXAdjustment              = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransformScaleOrientationXAdjustment"));
	m_TransformScaleOrientationYAdjustment              = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransformScaleOrientationYAdjustment"));
	m_TransformScaleOrientationZAdjustment              = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransformScaleOrientationZAdjustment"));
	m_TransformScaleXAdjustment                         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransformScaleXAdjustment"));
	m_TransformScaleYAdjustment                         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransformScaleYAdjustment"));
	m_TransformScaleZAdjustment                         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransformScaleZAdjustment"));
	m_TransformTranslationXAdjustment                   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransformTranslationXAdjustment"));
	m_TransformTranslationYAdjustment                   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransformTranslationYAdjustment"));
	m_TransformTranslationZAdjustment                   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransformTranslationZAdjustment"));
	m_ViewportClipBoundaryBottomAdjustment              = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ViewportClipBoundaryBottomAdjustment"));
	m_ViewportClipBoundaryLeftAdjustment                = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ViewportClipBoundaryLeftAdjustment"));
	m_ViewportClipBoundaryRightAdjustment               = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ViewportClipBoundaryRightAdjustment"));
	m_ViewportClipBoundaryTopAdjustment                 = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ViewportClipBoundaryTopAdjustment"));
	m_VolumeEmitterDirectionXAdjustment                 = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("VolumeEmitterDirectionXAdjustment"));
	m_VolumeEmitterDirectionYAdjustment                 = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("VolumeEmitterDirectionYAdjustment"));
	m_VolumeEmitterDirectionZAdjustment                 = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("VolumeEmitterDirectionZAdjustment"));

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
	m_builder -> get_widget ("HeaderBar", m_HeaderBar);
	m_builder -> get_widget ("IndexButton", m_IndexButton);
	m_builder -> get_widget ("NodePropertiesExpander", m_NodePropertiesExpander);
	m_builder -> get_widget ("NameBox", m_NameBox);
	m_builder -> get_widget ("NameEntry", m_NameEntry);
	m_builder -> get_widget ("RenameButton", m_RenameButton);
	m_builder -> get_widget ("Stack", m_Stack);
	m_builder -> get_widget ("TransformExpander", m_TransformExpander);
	m_builder -> get_widget ("TransformBox", m_TransformBox);
	m_builder -> get_widget ("TransformTranslationBox", m_TransformTranslationBox);
	m_builder -> get_widget ("TransformRotationToolBox", m_TransformRotationToolBox);
	m_builder -> get_widget ("TransformRotationNotebook", m_TransformRotationNotebook);
	m_builder -> get_widget ("TransformRotationBox", m_TransformRotationBox);
	m_builder -> get_widget ("TransformRotationEulerBox", m_TransformRotationEulerBox);
	m_builder -> get_widget ("TransformRotationTypeButton", m_TransformRotationTypeButton);
	m_builder -> get_widget ("TransformScaleOrientationToolBox", m_TransformScaleOrientationToolBox);
	m_builder -> get_widget ("TransformScaleOrientationNotebook", m_TransformScaleOrientationNotebook);
	m_builder -> get_widget ("TransformScaleOrientationBox", m_TransformScaleOrientationBox);
	m_builder -> get_widget ("TransformScaleOrientationEulerBox", m_TransformScaleOrientationEulerBox);
	m_builder -> get_widget ("TransformScaleOrientationTypeButton", m_TransformScaleOrientationTypeButton);
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
	m_builder -> get_widget ("CollisionExpander", m_CollisionExpander);
	m_builder -> get_widget ("CollisionEnabledCheckButton", m_CollisionEnabledCheckButton);
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
	m_builder -> get_widget ("GeoTransformExpander", m_GeoTransformExpander);
	m_builder -> get_widget ("GeoTransformBox", m_GeoTransformBox);
	m_builder -> get_widget ("GeoTransformTranslationBox", m_GeoTransformTranslationBox);
	m_builder -> get_widget ("GeoTransformRotationToolBox", m_GeoTransformRotationToolBox);
	m_builder -> get_widget ("GeoTransformRotationNotebook", m_GeoTransformRotationNotebook);
	m_builder -> get_widget ("GeoTransformRotationBox", m_GeoTransformRotationBox);
	m_builder -> get_widget ("GeoTransformRotationEulerBox", m_GeoTransformRotationEulerBox);
	m_builder -> get_widget ("GeoTransformRotationTypeButton", m_GeoTransformRotationTypeButton);
	m_builder -> get_widget ("GeoTransformScaleOrientationToolBox", m_GeoTransformScaleOrientationToolBox);
	m_builder -> get_widget ("GeoTransformScaleOrientationNotebook", m_GeoTransformScaleOrientationNotebook);
	m_builder -> get_widget ("GeoTransformScaleOrientationBox", m_GeoTransformScaleOrientationBox);
	m_builder -> get_widget ("GeoTransformScaleOrientationEulerBox", m_GeoTransformScaleOrientationEulerBox);
	m_builder -> get_widget ("GeoTransformScaleOrientationTypeButton", m_GeoTransformScaleOrientationTypeButton);
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
	m_builder -> get_widget ("EnvironmentalSensorExpander", m_EnvironmentalSensorExpander);
	m_builder -> get_widget ("EnvironmentalSensorBox", m_EnvironmentalSensorBox);
	m_builder -> get_widget ("EnvironmentalSensorSizeBox", m_EnvironmentalSensorSizeBox);
	m_builder -> get_widget ("EnvironmentalSensorUniformSizeButton", m_EnvironmentalSensorUniformSizeButton);
	m_builder -> get_widget ("EnvironmentalSensorUniformSizeImage", m_EnvironmentalSensorUniformSizeImage);
	m_builder -> get_widget ("EnvironmentalSensorCenterBox", m_EnvironmentalSensorCenterBox);
	m_builder -> get_widget ("EnvironmentalSensorEnabledCheckButton", m_EnvironmentalSensorEnabledCheckButton);
	m_builder -> get_widget ("ParticleSystemExpander", m_ParticleSystemExpander);
	m_builder -> get_widget ("ParticleSystemGeometryTypeButton", m_ParticleSystemGeometryTypeButton);
	m_builder -> get_widget ("ParticleSystemEnabledCheckButton", m_ParticleSystemEnabledCheckButton);
	m_builder -> get_widget ("ParticleSystemCreateParticlesCheckButton", m_ParticleSystemCreateParticlesCheckButton);
	m_builder -> get_widget ("ParticleSystemMaxParticlesSpinButton", m_ParticleSystemMaxParticlesSpinButton);
	m_builder -> get_widget ("ParticleSystemParticleLifetimeSpinButton", m_ParticleSystemParticleLifetimeSpinButton);
	m_builder -> get_widget ("ParticleSystemLifetimeVariationSpinButton", m_ParticleSystemLifetimeVariationSpinButton);
	m_builder -> get_widget ("ParticleSystemParticleSizeBox", m_ParticleSystemParticleSizeBox);
	m_builder -> get_widget ("ParticleSystemUniformParticleSizeButton", m_ParticleSystemUniformParticleSizeButton);
	m_builder -> get_widget ("ParticleSystemUniformParticleSizeImage", m_ParticleSystemUniformParticleSizeImage);
	m_builder -> get_widget ("TextureProjectorPerspectiveExpander", m_TextureProjectorPerspectiveExpander);
	m_builder -> get_widget ("TextureProjectorPerspectiveOnCheckButton", m_TextureProjectorPerspectiveOnCheckButton);
	m_builder -> get_widget ("TextureProjectorPerspectiveDescriptionTextView", m_TextureProjectorPerspectiveDescriptionTextView);
	m_builder -> get_widget ("TextureProjectorPerspectiveGlobalCheckButton", m_TextureProjectorPerspectiveGlobalCheckButton);
	m_builder -> get_widget ("TextureProjectorPerspectiveLocationBox", m_TextureProjectorPerspectiveLocationBox);
	m_builder -> get_widget ("TextureProjectorPerspectiveLocationXSpinButton", m_TextureProjectorPerspectiveLocationXSpinButton);
	m_builder -> get_widget ("TextureProjectorPerspectiveLocationYSpinButton", m_TextureProjectorPerspectiveLocationYSpinButton);
	m_builder -> get_widget ("TextureProjectorPerspectiveLocationZSpinButton", m_TextureProjectorPerspectiveLocationZSpinButton);
	m_builder -> get_widget ("TextureProjectorPerspectiveDirectionToolBox", m_TextureProjectorPerspectiveDirectionToolBox);
	m_builder -> get_widget ("TextureProjectorPerspectiveDirectionBox", m_TextureProjectorPerspectiveDirectionBox);
	m_builder -> get_widget ("TextureProjectorPerspectiveDirectionXSpinButton", m_TextureProjectorPerspectiveDirectionXSpinButton);
	m_builder -> get_widget ("TextureProjectorPerspectiveDirectionYSpinButton", m_TextureProjectorPerspectiveDirectionYSpinButton);
	m_builder -> get_widget ("TextureProjectorPerspectiveDirectionZSpinButton", m_TextureProjectorPerspectiveDirectionZSpinButton);
	m_builder -> get_widget ("TextureProjectorPerspectiveUpVectorToolBox", m_TextureProjectorPerspectiveUpVectorToolBox);
	m_builder -> get_widget ("TextureProjectorPerspectiveUpVectorBox", m_TextureProjectorPerspectiveUpVectorBox);
	m_builder -> get_widget ("TextureProjectorPerspectiveUpVectorXSpinButton", m_TextureProjectorPerspectiveUpVectorXSpinButton);
	m_builder -> get_widget ("TextureProjectorPerspectiveUpVectorYSpinButton", m_TextureProjectorPerspectiveUpVectorYSpinButton);
	m_builder -> get_widget ("TextureProjectorPerspectiveUpVectorZSpinButton", m_TextureProjectorPerspectiveUpVectorZSpinButton);
	m_builder -> get_widget ("TextureProjectorPerspectiveFieldOfViewBox", m_TextureProjectorPerspectiveFieldOfViewBox);
	m_builder -> get_widget ("TextureProjectorPerspectiveNearDistanceSpinButton", m_TextureProjectorPerspectiveNearDistanceSpinButton);
	m_builder -> get_widget ("TextureProjectorPerspectiveFarDistanceSpinButton", m_TextureProjectorPerspectiveFarDistanceSpinButton);
	m_builder -> get_widget ("TextureProjectorPerspectiveAspectRatioSpinButton", m_TextureProjectorPerspectiveAspectRatioSpinButton);
	m_builder -> get_widget ("LayoutExpander", m_LayoutExpander);
	m_builder -> get_widget ("CreateLayoutBox", m_CreateLayoutBox);
	m_builder -> get_widget ("LayoutCheckButton", m_LayoutCheckButton);
	m_builder -> get_widget ("LayoutUnlinkButton", m_LayoutUnlinkButton);
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
	m_builder -> get_widget ("ViewportUnlinkButton", m_ViewportUnlinkButton);
	m_builder -> get_widget ("ViewportBox", m_ViewportBox);
	m_builder -> get_widget ("ViewportClipBoundaryBox", m_ViewportClipBoundaryBox);
	m_builder -> get_widget ("ViewportClipBoundaryTopSpinButton", m_ViewportClipBoundaryTopSpinButton);
	m_builder -> get_widget ("ViewportClipBoundaryLeftSpinButton", m_ViewportClipBoundaryLeftSpinButton);
	m_builder -> get_widget ("ViewportClipBoundaryRightSpinButton", m_ViewportClipBoundaryRightSpinButton);
	m_builder -> get_widget ("ViewportClipBoundaryBottomSpinButton", m_ViewportClipBoundaryBottomSpinButton);
	m_builder -> get_widget ("EmitterExpander", m_EmitterExpander);
	m_builder -> get_widget ("CreateEmitterBox", m_CreateEmitterBox);
	m_builder -> get_widget ("EmitterTypeButton", m_EmitterTypeButton);
	m_builder -> get_widget ("EmitterUnlinkButton", m_EmitterUnlinkButton);
	m_builder -> get_widget ("EmitterBox", m_EmitterBox);
	m_builder -> get_widget ("PointEmitterBox", m_PointEmitterBox);
	m_builder -> get_widget ("PointEmitterPositionBox", m_PointEmitterPositionBox);
	m_builder -> get_widget ("PointEmitterDirectionToolBox", m_PointEmitterDirectionToolBox);
	m_builder -> get_widget ("PointEmitterDirectionBox", m_PointEmitterDirectionBox);
	m_builder -> get_widget ("PointEmitterDirectionXSpinButton", m_PointEmitterDirectionXSpinButton);
	m_builder -> get_widget ("PointEmitterDirectionYSpinButton", m_PointEmitterDirectionYSpinButton);
	m_builder -> get_widget ("PointEmitterDirectionZSpinButton", m_PointEmitterDirectionZSpinButton);
	m_builder -> get_widget ("ConeEmitterBox", m_ConeEmitterBox);
	m_builder -> get_widget ("ConeEmitterPositionBox", m_ConeEmitterPositionBox);
	m_builder -> get_widget ("ConeEmitterDirectionToolBox", m_ConeEmitterDirectionToolBox);
	m_builder -> get_widget ("ConeEmitterDirectionBox", m_ConeEmitterDirectionBox);
	m_builder -> get_widget ("ConeEmitterDirectionXSpinButton", m_ConeEmitterDirectionXSpinButton);
	m_builder -> get_widget ("ConeEmitterDirectionYSpinButton", m_ConeEmitterDirectionYSpinButton);
	m_builder -> get_widget ("ConeEmitterDirectionZSpinButton", m_ConeEmitterDirectionZSpinButton);
	m_builder -> get_widget ("ConeEmitterAngleBox", m_ConeEmitterAngleBox);
	m_builder -> get_widget ("ConeEmitterAngleSpinButton", m_ConeEmitterAngleSpinButton);
	m_builder -> get_widget ("PolylineEmitterBox", m_PolylineEmitterBox);
	m_builder -> get_widget ("PolylineEmitterDirectionToolBox", m_PolylineEmitterDirectionToolBox);
	m_builder -> get_widget ("PolylineEmitterDirectionBox", m_PolylineEmitterDirectionBox);
	m_builder -> get_widget ("PolylineEmitterDirectionXSpinButton", m_PolylineEmitterDirectionXSpinButton);
	m_builder -> get_widget ("PolylineEmitterDirectionYSpinButton", m_PolylineEmitterDirectionYSpinButton);
	m_builder -> get_widget ("PolylineEmitterDirectionZSpinButton", m_PolylineEmitterDirectionZSpinButton);
	m_builder -> get_widget ("VolumeEmitterBox", m_VolumeEmitterBox);
	m_builder -> get_widget ("VolumeEmitterDirectionToolBox", m_VolumeEmitterDirectionToolBox);
	m_builder -> get_widget ("VolumeEmitterDirectionBox", m_VolumeEmitterDirectionBox);
	m_builder -> get_widget ("VolumeEmitterDirectionXSpinButton", m_VolumeEmitterDirectionXSpinButton);
	m_builder -> get_widget ("VolumeEmitterDirectionYSpinButton", m_VolumeEmitterDirectionYSpinButton);
	m_builder -> get_widget ("VolumeEmitterDirectionZSpinButton", m_VolumeEmitterDirectionZSpinButton);
	m_builder -> get_widget ("VolumeEmitterInternalCheckButton", m_VolumeEmitterInternalCheckButton);
	m_builder -> get_widget ("ExplosionEmitterBox", m_ExplosionEmitterBox);
	m_builder -> get_widget ("ExplosionEmitterPositionBox", m_ExplosionEmitterPositionBox);
	m_builder -> get_widget ("EmitterSpeedSpinButton", m_EmitterSpeedSpinButton);
	m_builder -> get_widget ("EmitterVariationSpinButton", m_EmitterVariationSpinButton);
	m_builder -> get_widget ("EmitterMassSpinButton", m_EmitterMassSpinButton);
	m_builder -> get_widget ("EmitterSurfaceAreaSpinButton", m_EmitterSurfaceAreaSpinButton);
	m_builder -> get_widget ("PhysicsExpander", m_PhysicsExpander);
	m_builder -> get_widget ("PhysicsGrid", m_PhysicsGrid);
	m_builder -> get_widget ("PhysicsAddButton", m_PhysicsAddButton);
	m_builder -> get_widget ("PhysicsLinkButton", m_PhysicsLinkButton);
	m_builder -> get_widget ("ColorRampExpander", m_ColorRampExpander);
	m_builder -> get_widget ("CreateColorRampBox", m_CreateColorRampBox);
	m_builder -> get_widget ("ColorRampTypeButton", m_ColorRampTypeButton);
	m_builder -> get_widget ("ColorRampUnlinkButton", m_ColorRampUnlinkButton);
	m_builder -> get_widget ("ColorRampBox", m_ColorRampBox);
	m_builder -> get_widget ("ColorRampGradientKeyBox", m_ColorRampGradientKeyBox);
	m_builder -> get_widget ("ColorKeySpinButton", m_ColorKeySpinButton);
	m_builder -> get_widget ("ColorRampGradientBox", m_ColorRampGradientBox);
	m_builder -> get_widget ("ColorRampGrid", m_ColorRampGrid);
	m_builder -> get_widget ("ColorRampButtonBox", m_ColorRampButtonBox);
	m_builder -> get_widget ("ColorRampButton", m_ColorRampButton);
	m_builder -> get_widget ("ColorRampScrolledWindow", m_ColorRampScrolledWindow);
	m_builder -> get_widget ("AddColorRampButton", m_AddColorRampButton);
	m_builder -> get_widget ("RemoveColorRampButton", m_RemoveColorRampButton);
	m_builder -> get_widget ("ColorRampRGBAGrid", m_ColorRampRGBAGrid);
	m_builder -> get_widget ("ColorRampRGBAButtonBox", m_ColorRampRGBAButtonBox);
	m_builder -> get_widget ("ColorRampRGBAButton", m_ColorRampRGBAButton);
	m_builder -> get_widget ("ColorRampRGBAScrolledWindow", m_ColorRampRGBAScrolledWindow);
	m_builder -> get_widget ("AddColorRampRGBAButton", m_AddColorRampRGBAButton);
	m_builder -> get_widget ("RemoveColorRampRGBAButton", m_RemoveColorRampRGBAButton);
	m_builder -> get_widget ("ForcePhysicsModelBox", m_ForcePhysicsModelBox);
	m_builder -> get_widget ("WindPhysicsModelBox", m_WindPhysicsModelBox);
	m_builder -> get_widget ("GeometrySelectionExpander", m_GeometrySelectionExpander);
	m_builder -> get_widget ("GeometrySelectionBox", m_GeometrySelectionBox);
	m_builder -> get_widget ("GeometrySelectionTranslationBox", m_GeometrySelectionTranslationBox);
	m_builder -> get_widget ("GeometrySelectionTranslationXButton", m_GeometrySelectionTranslationXButton);
	m_builder -> get_widget ("GeometrySelectionTranslationYXButton", m_GeometrySelectionTranslationYXButton);
	m_builder -> get_widget ("GeometrySelectionTranslationZButton", m_GeometrySelectionTranslationZButton);
	m_builder -> get_widget ("GeometrySelectionScaleBox", m_GeometrySelectionScaleBox);
	m_builder -> get_widget ("GeometrySelectionScaleXButton", m_GeometrySelectionScaleXButton);
	m_builder -> get_widget ("GeometrySelectionScaleYButton", m_GeometrySelectionScaleYButton);
	m_builder -> get_widget ("GeometrySelectionScaleZButton", m_GeometrySelectionScaleZButton);
	m_builder -> get_widget ("GeometrySelectionUniformScaleButton", m_GeometrySelectionUniformScaleButton);
	m_builder -> get_widget ("GeometrySelectionUniformScaleImage", m_GeometrySelectionUniformScaleImage);
	m_builder -> get_widget ("SelectedPointsLabel", m_SelectedPointsLabel);
	m_builder -> get_widget ("SelectedEdgesLabel", m_SelectedEdgesLabel);
	m_builder -> get_widget ("SelectedHolesLabel", m_SelectedHolesLabel);
	m_builder -> get_widget ("SelectedFacesLabel", m_SelectedFacesLabel);
	m_builder -> get_widget ("GeometrySelectionRotationToolBox", m_GeometrySelectionRotationToolBox);
	m_builder -> get_widget ("GeometrySelectionRotationNotebook", m_GeometrySelectionRotationNotebook);
	m_builder -> get_widget ("GeometrySelectionRotationBox", m_GeometrySelectionRotationBox);
	m_builder -> get_widget ("GeometrySelectionRotationXButton", m_GeometrySelectionRotationXButton);
	m_builder -> get_widget ("GeometrySelectionRotationYButton", m_GeometrySelectionRotationYButton);
	m_builder -> get_widget ("GeometrySelectionRotationZButton", m_GeometrySelectionRotationZButton);
	m_builder -> get_widget ("GeometrySelectionRotationAButton", m_GeometrySelectionRotationAButton);
	m_builder -> get_widget ("GeometrySelectionRotationEulerBox", m_GeometrySelectionRotationEulerBox);
	m_builder -> get_widget ("GeometrySelectionRotationTypeButton", m_GeometrySelectionRotationTypeButton);
	m_builder -> get_widget ("BoundingBoxExpander", m_BoundingBoxExpander);
	m_builder -> get_widget ("BoundingBoxBox", m_BoundingBoxBox);
	m_builder -> get_widget ("BBoxSizeBox", m_BBoxSizeBox);
	m_builder -> get_widget ("BBoxSizeXSpinButton", m_BBoxSizeXSpinButton);
	m_builder -> get_widget ("BBoxSizeYSpinButton", m_BBoxSizeYSpinButton);
	m_builder -> get_widget ("BBoxSizeZSpinButton", m_BBoxSizeZSpinButton);
	m_builder -> get_widget ("BBoxUniformSizeButton", m_BBoxUniformSizeButton);
	m_builder -> get_widget ("BBoxUniformSizeImage", m_BBoxUniformSizeImage);
	m_builder -> get_widget ("BBoxSizeXLabel", m_BBoxSizeXLabel);
	m_builder -> get_widget ("BBoxSizeYLabel", m_BBoxSizeYLabel);
	m_builder -> get_widget ("BBoxSizeZLabel", m_BBoxSizeZLabel);
	m_builder -> get_widget ("BBoxCenterXLabel", m_BBoxCenterXLabel);
	m_builder -> get_widget ("BBoxCenterYLabel", m_BBoxCenterYLabel);
	m_builder -> get_widget ("BBoxCenterZLabel", m_BBoxCenterZLabel);
	m_builder -> get_widget ("FillBoundingBoxFieldsButton", m_FillBoundingBoxFieldsButton);
	m_builder -> get_widget ("BBoxCenterBox", m_BBoxCenterBox);
	m_builder -> get_widget ("BBoxCenterXSpinButton", m_BBoxCenterXSpinButton);
	m_builder -> get_widget ("BBoxCenterYSpinButton", m_BBoxCenterYSpinButton);
	m_builder -> get_widget ("BBoxCenterZSpinButton", m_BBoxCenterZSpinButton);
	m_builder -> get_widget ("GeoTransformRotationPopover", m_GeoTransformRotationPopover);
	m_builder -> get_widget ("GeoTransformRotationAxisAngleButton", m_GeoTransformRotationAxisAngleButton);
	m_builder -> get_widget ("GeoTransformRotationEulerButton", m_GeoTransformRotationEulerButton);
	m_builder -> get_widget ("GeoTransformScaleOrientationPopover", m_GeoTransformScaleOrientationPopover);
	m_builder -> get_widget ("GeoTransformScaleOrientationAxisAngleButton", m_GeoTransformScaleOrientationAxisAngleButton);
	m_builder -> get_widget ("GeoTransformScaleOrientationEulerButton", m_GeoTransformScaleOrientationEulerButton);
	m_builder -> get_widget ("GeometrySelectionRotationPopover", m_GeometrySelectionRotationPopover);
	m_builder -> get_widget ("GeometrySelectionRotationAxisAngleButton", m_GeometrySelectionRotationAxisAngleButton);
	m_builder -> get_widget ("GeometrySelectionRotationEulerButton", m_GeometrySelectionRotationEulerButton);
	m_builder -> get_widget ("NewPhysicsPopover", m_NewPhysicsPopover);
	m_builder -> get_widget ("NewForcePhysicsModelButton", m_NewForcePhysicsModelButton);
	m_builder -> get_widget ("NewWindPhysicsModelButton", m_NewWindPhysicsModelButton);
	m_builder -> get_widget ("TransformRotationPopover", m_TransformRotationPopover);
	m_builder -> get_widget ("TransformRotationAxisAngleButton", m_TransformRotationAxisAngleButton);
	m_builder -> get_widget ("TransformRotationEulerButton", m_TransformRotationEulerButton);
	m_builder -> get_widget ("TransformScaleOrientationPopover", m_TransformScaleOrientationPopover);
	m_builder -> get_widget ("TransformScaleOrientationAxisAngleButton", m_TransformScaleOrientationAxisAngleButton);
	m_builder -> get_widget ("TransformScaleOrientationEulerButton", m_TransformScaleOrientationEulerButton);

	// Connect object Gtk::Box with id 'Widget'.
	m_Widget -> signal_map () .connect (sigc::mem_fun (this, &X3DPrecisionPlacementPanelInterface::on_geometry_selection_map));
	m_Widget -> signal_map () .connect (sigc::mem_fun (this, &X3DPrecisionPlacementPanelInterface::on_map));
	m_Widget -> signal_unmap () .connect (sigc::mem_fun (this, &X3DPrecisionPlacementPanelInterface::on_geometry_selection_unmap));
	m_Widget -> signal_unmap () .connect (sigc::mem_fun (this, &X3DPrecisionPlacementPanelInterface::on_unmap));

	// Connect object Gtk::Button with id 'IndexButton'.
	m_IndexButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DPrecisionPlacementPanelInterface::on_index_clicked));
	m_TransformRotationTypeButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DPrecisionPlacementPanelInterface::on_transform_rotation_type_clicked));
	m_TransformScaleOrientationTypeButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DPrecisionPlacementPanelInterface::on_transform_scale_orientation_type_clicked));

	// Connect object Gtk::ToggleButton with id 'TransformUniformScaleButton'.
	m_TransformUniformScaleButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DPrecisionPlacementPanelInterface::on_transform_uniform_scale_clicked));

	// Connect object Gtk::Button with id 'TransformMoveCenterButton'.
	m_TransformMoveCenterButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DPrecisionPlacementPanelInterface::on_transform_move_center_button));

	// Connect object Gtk::ComboBoxText with id 'BillboardAxisOfRotationComboBoxText'.
	m_BillboardAxisOfRotationComboBoxText -> signal_changed () .connect (sigc::mem_fun (this, &X3DPrecisionPlacementPanelInterface::on_axisOfRotation_changed));

	// Connect object Gtk::Button with id 'LODMoveCenterButton'.
	m_LODMoveCenterButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DPrecisionPlacementPanelInterface::on_lod_move_center_button_clicked));

	// Connect object Gtk::CheckButton with id 'LODKeepCurrentLevelCheckButton'.
	m_LODKeepCurrentLevelCheckButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DPrecisionPlacementPanelInterface::on_lod_keep_current_level_toggled));

	// Connect object Gtk::Button with id 'GeoTransformRotationTypeButton'.
	m_GeoTransformRotationTypeButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DPrecisionPlacementPanelInterface::on_geo_transform_rotation_type_clicked));
	m_GeoTransformScaleOrientationTypeButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DPrecisionPlacementPanelInterface::on_geo_transform_scale_orientation_type_clicked));

	// Connect object Gtk::ToggleButton with id 'GeoTransformUniformScaleButton'.
	m_GeoTransformUniformScaleButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DPrecisionPlacementPanelInterface::on_geo_transform_uniform_scale_clicked));
	m_EnvironmentalSensorUniformSizeButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DPrecisionPlacementPanelInterface::on_proximity_sensor_uniform_size_clicked));
	m_ParticleSystemUniformParticleSizeButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DPrecisionPlacementPanelInterface::on_particle_system_uniform_particle_size_toggled));

	// Connect object Gtk::CheckButton with id 'LayoutCheckButton'.
	m_LayoutCheckButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DPrecisionPlacementPanelInterface::on_layout_toggled));

	// Connect object Gtk::Button with id 'LayoutUnlinkButton'.
	m_LayoutUnlinkButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DPrecisionPlacementPanelInterface::on_layout_unlink_clicked));

	// Connect object Gtk::CheckButton with id 'ViewportCheckButton'.
	m_ViewportCheckButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DPrecisionPlacementPanelInterface::on_viewport_toggled));

	// Connect object Gtk::Button with id 'ViewportUnlinkButton'.
	m_ViewportUnlinkButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DPrecisionPlacementPanelInterface::on_viewport_unlink_clicked));

	// Connect object Gtk::ComboBoxText with id 'EmitterTypeButton'.
	m_EmitterTypeButton -> signal_changed () .connect (sigc::mem_fun (this, &X3DPrecisionPlacementPanelInterface::on_emitter_type_changed));

	// Connect object Gtk::Button with id 'EmitterUnlinkButton'.
	m_EmitterUnlinkButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DPrecisionPlacementPanelInterface::on_emitter_unlink_clicked));
	m_PhysicsAddButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DPrecisionPlacementPanelInterface::on_add_physics_clicked));
	m_PhysicsLinkButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DPrecisionPlacementPanelInterface::on_physics_link_clicked));

	// Connect object Gtk::ComboBoxText with id 'ColorRampTypeButton'.
	m_ColorRampTypeButton -> signal_changed () .connect (sigc::mem_fun (this, &X3DPrecisionPlacementPanelInterface::on_color_ramp_type_changed));

	// Connect object Gtk::Button with id 'ColorRampUnlinkButton'.
	m_ColorRampUnlinkButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DPrecisionPlacementPanelInterface::on_color_ramp_unlink_clicked));

	// Connect object Gtk::SpinButton with id 'GeometrySelectionTranslationXButton'.
	m_GeometrySelectionTranslationXButton -> signal_focus_in_event () .connect (sigc::mem_fun (this, &X3DPrecisionPlacementPanelInterface::on_geometry_selection_focus_in_event));
	m_GeometrySelectionTranslationYXButton -> signal_focus_in_event () .connect (sigc::mem_fun (this, &X3DPrecisionPlacementPanelInterface::on_geometry_selection_focus_in_event));
	m_GeometrySelectionTranslationZButton -> signal_focus_in_event () .connect (sigc::mem_fun (this, &X3DPrecisionPlacementPanelInterface::on_geometry_selection_focus_in_event));
	m_GeometrySelectionScaleXButton -> signal_focus_in_event () .connect (sigc::mem_fun (this, &X3DPrecisionPlacementPanelInterface::on_geometry_selection_focus_in_event));
	m_GeometrySelectionScaleYButton -> signal_focus_in_event () .connect (sigc::mem_fun (this, &X3DPrecisionPlacementPanelInterface::on_geometry_selection_focus_in_event));
	m_GeometrySelectionScaleZButton -> signal_focus_in_event () .connect (sigc::mem_fun (this, &X3DPrecisionPlacementPanelInterface::on_geometry_selection_focus_in_event));

	// Connect object Gtk::ToggleButton with id 'GeometrySelectionUniformScaleButton'.
	m_GeometrySelectionUniformScaleButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DPrecisionPlacementPanelInterface::on_geometry_selection_uniform_scale_toggled));

	// Connect object Gtk::SpinButton with id 'GeometrySelectionRotationXButton'.
	m_GeometrySelectionRotationXButton -> signal_focus_in_event () .connect (sigc::mem_fun (this, &X3DPrecisionPlacementPanelInterface::on_geometry_selection_focus_in_event));
	m_GeometrySelectionRotationYButton -> signal_focus_in_event () .connect (sigc::mem_fun (this, &X3DPrecisionPlacementPanelInterface::on_geometry_selection_focus_in_event));
	m_GeometrySelectionRotationZButton -> signal_focus_in_event () .connect (sigc::mem_fun (this, &X3DPrecisionPlacementPanelInterface::on_geometry_selection_focus_in_event));
	m_GeometrySelectionRotationAButton -> signal_focus_in_event () .connect (sigc::mem_fun (this, &X3DPrecisionPlacementPanelInterface::on_geometry_selection_focus_in_event));

	// Connect object Gtk::Button with id 'GeometrySelectionRotationTypeButton'.
	m_GeometrySelectionRotationTypeButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DPrecisionPlacementPanelInterface::on_geometry_selection_rotation_type_clicked));

	// Connect object Gtk::ToggleButton with id 'BBoxUniformSizeButton'.
	m_BBoxUniformSizeButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DPrecisionPlacementPanelInterface::on_bbox_uniform_size_clicked));

	// Connect object Gtk::Button with id 'FillBoundingBoxFieldsButton'.
	m_FillBoundingBoxFieldsButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DPrecisionPlacementPanelInterface::on_fill_bounding_box_fields_clicked));

	// Connect object Gtk::RadioButton with id 'GeoTransformRotationAxisAngleButton'.
	m_GeoTransformRotationAxisAngleButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DPrecisionPlacementPanelInterface::on_geo_transform_rotation_axis_angle_toggled));
	m_GeoTransformRotationEulerButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DPrecisionPlacementPanelInterface::on_geo_transform_rotation_euler_toggled));
	m_GeoTransformScaleOrientationAxisAngleButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DPrecisionPlacementPanelInterface::on_geo_transform_scale_orientation_axis_angle_toggled));
	m_GeoTransformScaleOrientationEulerButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DPrecisionPlacementPanelInterface::on_geo_transform_scale_orientation_euler_toggled));
	m_GeometrySelectionRotationAxisAngleButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DPrecisionPlacementPanelInterface::on_geometry_selection_rotation_axis_angle_toggled));
	m_GeometrySelectionRotationEulerButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DPrecisionPlacementPanelInterface::on_geometry_selection_rotation_euler_toggled));

	// Connect object Gtk::Button with id 'NewForcePhysicsModelButton'.
	m_NewForcePhysicsModelButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DPrecisionPlacementPanelInterface::on_new_force_physics_model_clicked));
	m_NewWindPhysicsModelButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DPrecisionPlacementPanelInterface::on_new_wind_physics_model_clicked));

	// Connect object Gtk::RadioButton with id 'TransformRotationAxisAngleButton'.
	m_TransformRotationAxisAngleButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DPrecisionPlacementPanelInterface::on_transform_rotation_axis_angle_toggled));
	m_TransformRotationEulerButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DPrecisionPlacementPanelInterface::on_transform_rotation_euler_toggled));
	m_TransformScaleOrientationAxisAngleButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DPrecisionPlacementPanelInterface::on_transform_scale_orientation_axis_angle_toggled));
	m_TransformScaleOrientationEulerButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DPrecisionPlacementPanelInterface::on_transform_scale_orientation_euler_toggled));
}

X3DPrecisionPlacementPanelInterface::~X3DPrecisionPlacementPanelInterface ()
{
	delete m_LODExtraWindow;
	delete m_Window;
}

} // puck
} // titania
