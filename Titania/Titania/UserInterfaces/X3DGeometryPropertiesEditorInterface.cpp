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

#include "X3DGeometryPropertiesEditorInterface.h"

namespace titania {
namespace puck {

void
X3DGeometryPropertiesEditorInterface::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	create ();
}

void
X3DGeometryPropertiesEditorInterface::create (std::initializer_list <std::string> filenames)
{
	// Create Builder.
	m_builder = Gtk::Builder::create ();

	for (const auto & filename : filenames)
		m_builder -> add_from_file (filename);

	create ();
}

void
X3DGeometryPropertiesEditorInterface::create ()
{
	// Get objects.
	m_Arc2DDimensionAdjustment                       = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("Arc2DDimensionAdjustment"));
	m_Arc2DEndAngleAdjustment                        = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("Arc2DEndAngleAdjustment"));
	m_Arc2DRadiusAdjustment                          = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("Arc2DRadiusAdjustment"));
	m_Arc2DStartAngleAdjustment                      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("Arc2DStartAngleAdjustment"));
	m_ArcClose2DDimensionAdjustment                  = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ArcClose2DDimensionAdjustment"));
	m_ArcClose2DEndAngleAdjustment                   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ArcClose2DEndAngleAdjustment"));
	m_ArcClose2DRadiusAdjustment                     = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ArcClose2DRadiusAdjustment"));
	m_ArcClose2DStartAngleAdjustment                 = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ArcClose2DStartAngleAdjustment"));
	m_BoxSizeXAdjustment                             = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BoxSizeXAdjustment"));
	m_BoxSizeYAdjustment                             = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BoxSizeYAdjustment"));
	m_BoxSizeZAdjustment                             = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BoxSizeZAdjustment"));
	m_Circle2DDimensionAdjustment                    = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("Circle2DDimensionAdjustment"));
	m_Circle2DRadiusAdjustment                       = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("Circle2DRadiusAdjustment"));
	m_ColorAdjustment                                = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ColorAdjustment"));
	m_ColorRGBAAdjustment                            = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ColorRGBAAdjustment"));
	m_ConeBottomRadiusAdjustment                     = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ConeBottomRadiusAdjustment"));
	m_ConeHeightAdjustment                           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ConeHeightAdjustment"));
	m_ConeXDimensionAdjustment                       = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ConeXDimensionAdjustment"));
	m_CreaseAngleAdjustment                          = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("CreaseAngleAdjustment"));
	m_CreaseAngleDoubleAdjustment                    = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("CreaseAngleDoubleAdjustment"));
	m_CylinderHeightAdjustment                       = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("CylinderHeightAdjustment"));
	m_CylinderRadiusAdjustment                       = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("CylinderRadiusAdjustment"));
	m_CylinderXDimensionAdjustment                   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("CylinderXDimensionAdjustment"));
	m_Disk2DDimensionAdjustment                      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("Disk2DDimensionAdjustment"));
	m_Disk2DInnerRadiusAdjustment                    = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("Disk2DInnerRadiusAdjustment"));
	m_Disk2DOuterRadiusAdjustment                    = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("Disk2DOuterRadiusAdjustment"));
	m_EdgeColorAdjustment                            = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("EdgeColorAdjustment"));
	m_ElevationGridHeightMapMaxHeightAdjustment      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ElevationGridHeightMapMaxHeightAdjustment"));
	m_ElevationGridHeightMapMinHeightAdjustment      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ElevationGridHeightMapMinHeightAdjustment"));
	m_ElevationGridXDimensionAdjustment              = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ElevationGridXDimensionAdjustment"));
	m_ElevationGridXSpacingAdjustment                = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ElevationGridXSpacingAdjustment"));
	m_ElevationGridZDimensionAdjustment              = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ElevationGridZDimensionAdjustment"));
	m_ElevationGridZSpacingAdjustment                = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ElevationGridZSpacingAdjustment"));
	m_GeoElevationGridGeoGridOriginXAdjustment       = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoElevationGridGeoGridOriginXAdjustment"));
	m_GeoElevationGridGeoGridOriginYAdjustment       = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoElevationGridGeoGridOriginYAdjustment"));
	m_GeoElevationGridGeoGridOriginZAdjustment       = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoElevationGridGeoGridOriginZAdjustment"));
	m_GeoElevationGridHeightMapMaxHeightAdjustment   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoElevationGridHeightMapMaxHeightAdjustment"));
	m_GeoElevationGridHeightMapMinHeightAdjustment   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoElevationGridHeightMapMinHeightAdjustment"));
	m_GeoElevationGridXDimensionAdjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoElevationGridXDimensionAdjustment"));
	m_GeoElevationGridXSpacingAdjustment             = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoElevationGridXSpacingAdjustment"));
	m_GeoElevationGridYScaleAdjustment               = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoElevationGridYScaleAdjustment"));
	m_GeoElevationGridZDimensionAdjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoElevationGridZDimensionAdjustment"));
	m_GeoElevationGridZSpacingAdjustment             = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoElevationGridZSpacingAdjustment"));
	m_GeoElevationGridZoneAdjustment                 = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoElevationGridZoneAdjustment"));
	m_IcosahedronDimensionAdjustment                 = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("IcosahedronDimensionAdjustment"));
	m_IndexedLineSetLSystemAngleVariationAdjustment  = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("IndexedLineSetLSystemAngleVariationAdjustment"));
	m_IndexedLineSetLSystemIterationsAdjustment      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("IndexedLineSetLSystemIterationsAdjustment"));
	m_IndexedLineSetLSystemLengthVariationAdjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("IndexedLineSetLSystemLengthVariationAdjustment"));
	m_IndexedLineSetLSystemSizeXAdjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("IndexedLineSetLSystemSizeXAdjustment"));
	m_IndexedLineSetLSystemSizeYAdjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("IndexedLineSetLSystemSizeYAdjustment"));
	m_IndexedLineSetLSystemSizeZAdjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("IndexedLineSetLSystemSizeZAdjustment"));
	m_IndexedLineSetLSystemXAngleAdjustment          = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("IndexedLineSetLSystemXAngleAdjustment"));
	m_IndexedLineSetLSystemYAngleAdjustment          = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("IndexedLineSetLSystemYAngleAdjustment"));
	m_IndexedLineSetLSystemZAngleAdjustment          = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("IndexedLineSetLSystemZAngleAdjustment"));
	m_NormalColorAdjustment                          = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("NormalColorAdjustment"));
	m_NormalLengthAdjustment                         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("NormalLengthAdjustment"));
	m_NurbsCurveOrderAdjustment                      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("NurbsCurveOrderAdjustment"));
	m_NurbsCurveTessellationAdjustment               = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("NurbsCurveTessellationAdjustment"));
	m_NurbsPatchSurfaceUDimensionAdjustment          = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("NurbsPatchSurfaceUDimensionAdjustment"));
	m_NurbsPatchSurfaceUOrderAdjustment              = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("NurbsPatchSurfaceUOrderAdjustment"));
	m_NurbsPatchSurfaceUTessellationAdjustment       = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("NurbsPatchSurfaceUTessellationAdjustment"));
	m_NurbsPatchSurfaceVDimensionAdjustment          = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("NurbsPatchSurfaceVDimensionAdjustment"));
	m_NurbsPatchSurfaceVOrderAdjustment              = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("NurbsPatchSurfaceVOrderAdjustment"));
	m_NurbsPatchSurfaceVTessellationAdjustment       = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("NurbsPatchSurfaceVTessellationAdjustment"));
	m_NurbsTrimmedSurfaceUDimensionAdjustment        = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("NurbsTrimmedSurfaceUDimensionAdjustment"));
	m_NurbsTrimmedSurfaceUOrderAdjustment            = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("NurbsTrimmedSurfaceUOrderAdjustment"));
	m_NurbsTrimmedSurfaceUTessellationAdjustment     = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("NurbsTrimmedSurfaceUTessellationAdjustment"));
	m_NurbsTrimmedSurfaceVDimensionAdjustment        = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("NurbsTrimmedSurfaceVDimensionAdjustment"));
	m_NurbsTrimmedSurfaceVOrderAdjustment            = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("NurbsTrimmedSurfaceVOrderAdjustment"));
	m_NurbsTrimmedSurfaceVTessellationAdjustment     = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("NurbsTrimmedSurfaceVTessellationAdjustment"));
	m_OctahedronDimensionAdjustment                  = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("OctahedronDimensionAdjustment"));
	m_PrimitiveEdgeColorAdjustment                   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("PrimitiveEdgeColorAdjustment"));
	m_QuadSphereXDimensionAdjustment                 = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("QuadSphereXDimensionAdjustment"));
	m_QuadSphereYDimensionAdjustment                 = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("QuadSphereYDimensionAdjustment"));
	m_Rectangle2DSizeXAdjustment                     = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("Rectangle2DSizeXAdjustment"));
	m_Rectangle2DSizeYAdjustment                     = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("Rectangle2DSizeYAdjustment"));
	m_SphereRadiusAdjustment                         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("SphereRadiusAdjustment"));

	// Get widgets.
	m_builder -> get_widget ("Window", m_Window);
	m_builder -> get_widget ("Widget", m_Widget);
	m_builder -> get_widget ("HeaderBar", m_HeaderBar);
	m_builder -> get_widget ("GeometryChildNotebook", m_GeometryChildNotebook);
	m_builder -> get_widget ("SelectGeometryBox", m_SelectGeometryBox);
	m_builder -> get_widget ("GeometryComboBoxText", m_GeometryComboBoxText);
	m_builder -> get_widget ("GeometryUnlinkButton", m_GeometryUnlinkButton);
	m_builder -> get_widget ("GeometryBox", m_GeometryBox);
	m_builder -> get_widget ("GeometryStack", m_GeometryStack);
	m_builder -> get_widget ("Arc2DBox", m_Arc2DBox);
	m_builder -> get_widget ("Arc2DExpander", m_Arc2DExpander);
	m_builder -> get_widget ("Arc2DRadiusSpinButton", m_Arc2DRadiusSpinButton);
	m_builder -> get_widget ("Arc2DStartAngleSpinButton", m_Arc2DStartAngleSpinButton);
	m_builder -> get_widget ("Arc2DEndAngleSpinButton", m_Arc2DEndAngleSpinButton);
	m_builder -> get_widget ("Arc2DOptionsExpander", m_Arc2DOptionsExpander);
	m_builder -> get_widget ("Arc2DUseGlobalOptionsCheckButton", m_Arc2DUseGlobalOptionsCheckButton);
	m_builder -> get_widget ("Arc2DOptionsGrid", m_Arc2DOptionsGrid);
	m_builder -> get_widget ("Arc2DDimensionSpinButton", m_Arc2DDimensionSpinButton);
	m_builder -> get_widget ("ArcClose2DBox", m_ArcClose2DBox);
	m_builder -> get_widget ("ArcClose2DExpander", m_ArcClose2DExpander);
	m_builder -> get_widget ("ArcClose2DRadiusSpinButton", m_ArcClose2DRadiusSpinButton);
	m_builder -> get_widget ("ArcClose2DClosureTypeComboBoxText", m_ArcClose2DClosureTypeComboBoxText);
	m_builder -> get_widget ("ArcClose2DStartAngleSpinButton", m_ArcClose2DStartAngleSpinButton);
	m_builder -> get_widget ("ArcClose2DEndAngleSpinButton", m_ArcClose2DEndAngleSpinButton);
	m_builder -> get_widget ("ArcClose2DOptionsExpander", m_ArcClose2DOptionsExpander);
	m_builder -> get_widget ("ArcClose2DUseGlobalOptionsCheckButton", m_ArcClose2DUseGlobalOptionsCheckButton);
	m_builder -> get_widget ("ArcClose2DOptionsGrid", m_ArcClose2DOptionsGrid);
	m_builder -> get_widget ("ArcClose2DDimensionSpinButton", m_ArcClose2DDimensionSpinButton);
	m_builder -> get_widget ("Circle2DBox", m_Circle2DBox);
	m_builder -> get_widget ("Circle2DExpander", m_Circle2DExpander);
	m_builder -> get_widget ("Circle2DRadiusSpinButton", m_Circle2DRadiusSpinButton);
	m_builder -> get_widget ("Circle2DOptionsExpander", m_Circle2DOptionsExpander);
	m_builder -> get_widget ("Circle2DUseGlobalOptionsCheckButton", m_Circle2DUseGlobalOptionsCheckButton);
	m_builder -> get_widget ("Circle2DOptionsGrid", m_Circle2DOptionsGrid);
	m_builder -> get_widget ("Circle2DDimensionSpinButton", m_Circle2DDimensionSpinButton);
	m_builder -> get_widget ("Disk2DBox", m_Disk2DBox);
	m_builder -> get_widget ("Disk2DExpander", m_Disk2DExpander);
	m_builder -> get_widget ("Disk2DInnerRadiusSpinButton", m_Disk2DInnerRadiusSpinButton);
	m_builder -> get_widget ("Disk2DOuterRadiusSpinButton", m_Disk2DOuterRadiusSpinButton);
	m_builder -> get_widget ("Disk2DOptionsExpander", m_Disk2DOptionsExpander);
	m_builder -> get_widget ("Disk2DUseGlobalOptionsCheckButton", m_Disk2DUseGlobalOptionsCheckButton);
	m_builder -> get_widget ("Disk2DOptionsGrid", m_Disk2DOptionsGrid);
	m_builder -> get_widget ("Disk2DDimensionSpinButton", m_Disk2DDimensionSpinButton);
	m_builder -> get_widget ("Rectangle2DExpander", m_Rectangle2DExpander);
	m_builder -> get_widget ("Rectangle2DSizeBox", m_Rectangle2DSizeBox);
	m_builder -> get_widget ("Rectangle2DSizeXSpinButton", m_Rectangle2DSizeXSpinButton);
	m_builder -> get_widget ("Rectangle2DSizeYSpinButton", m_Rectangle2DSizeYSpinButton);
	m_builder -> get_widget ("Rectangle2DUniformSizeButton", m_Rectangle2DUniformSizeButton);
	m_builder -> get_widget ("Rectangle2DUniformSizeImage", m_Rectangle2DUniformSizeImage);
	m_builder -> get_widget ("BoxExpander", m_BoxExpander);
	m_builder -> get_widget ("BoxSizeBox", m_BoxSizeBox);
	m_builder -> get_widget ("BoxSizeXSpinButton", m_BoxSizeXSpinButton);
	m_builder -> get_widget ("BoxSizeYSpinButton", m_BoxSizeYSpinButton);
	m_builder -> get_widget ("BoxSizeZSpinButton", m_BoxSizeZSpinButton);
	m_builder -> get_widget ("BoxUniformSizeButton", m_BoxUniformSizeButton);
	m_builder -> get_widget ("BoxUniformSizeImage", m_BoxUniformSizeImage);
	m_builder -> get_widget ("ConeBox", m_ConeBox);
	m_builder -> get_widget ("ConeExpander", m_ConeExpander);
	m_builder -> get_widget ("ConeBottomRadiusSpinButton", m_ConeBottomRadiusSpinButton);
	m_builder -> get_widget ("ConeHeightSpinButton", m_ConeHeightSpinButton);
	m_builder -> get_widget ("ConeSideCheckButton", m_ConeSideCheckButton);
	m_builder -> get_widget ("ConeBottomCheckButton", m_ConeBottomCheckButton);
	m_builder -> get_widget ("ConeOptionsExpander", m_ConeOptionsExpander);
	m_builder -> get_widget ("ConeUseGlobalOptionsCheckButton", m_ConeUseGlobalOptionsCheckButton);
	m_builder -> get_widget ("ConeOptionsGrid", m_ConeOptionsGrid);
	m_builder -> get_widget ("ConeXDimensionSpinButton", m_ConeXDimensionSpinButton);
	m_builder -> get_widget ("CylinderBox", m_CylinderBox);
	m_builder -> get_widget ("CylinderExpander", m_CylinderExpander);
	m_builder -> get_widget ("CylinderRadiusSpinButton", m_CylinderRadiusSpinButton);
	m_builder -> get_widget ("CylinderHeightSpinButton", m_CylinderHeightSpinButton);
	m_builder -> get_widget ("CylinderBottomCheckButton", m_CylinderBottomCheckButton);
	m_builder -> get_widget ("CylinderSideCheckButton", m_CylinderSideCheckButton);
	m_builder -> get_widget ("CylinderTopCheckButton", m_CylinderTopCheckButton);
	m_builder -> get_widget ("CylinderOptionsExpander", m_CylinderOptionsExpander);
	m_builder -> get_widget ("CylinderUseGlobalOptionsCheckButton", m_CylinderUseGlobalOptionsCheckButton);
	m_builder -> get_widget ("CylinderOptionsGrid", m_CylinderOptionsGrid);
	m_builder -> get_widget ("CylinderXDimensionSpinButton", m_CylinderXDimensionSpinButton);
	m_builder -> get_widget ("ElevationGridBox", m_ElevationGridBox);
	m_builder -> get_widget ("ElevationGridExpander", m_ElevationGridExpander);
	m_builder -> get_widget ("ElevationGridXDimensionSpinButton", m_ElevationGridXDimensionSpinButton);
	m_builder -> get_widget ("ElevationGridZDimensionSpinButton", m_ElevationGridZDimensionSpinButton);
	m_builder -> get_widget ("ElevationGridXSpacingSpinButton", m_ElevationGridXSpacingSpinButton);
	m_builder -> get_widget ("ElevationGridZSpacingSpinButton", m_ElevationGridZSpacingSpinButton);
	m_builder -> get_widget ("ElevationGridHeightMapExpander", m_ElevationGridHeightMapExpander);
	m_builder -> get_widget ("ElevationGridHeightMapMinHeightSpinButton", m_ElevationGridHeightMapMinHeightSpinButton);
	m_builder -> get_widget ("ElevationGridHeightMapMaxHeightSpinButton", m_ElevationGridHeightMapMaxHeightSpinButton);
	m_builder -> get_widget ("ElevationGridHeightMapImageChooserButton", m_ElevationGridHeightMapImageChooserButton);
	m_builder -> get_widget ("ElevationGridHeightMapImageReloadButton", m_ElevationGridHeightMapImageReloadButton);
	m_builder -> get_widget ("ElevationGridHeightMapImageRemoveButton", m_ElevationGridHeightMapImageRemoveButton);
	m_builder -> get_widget ("ElevationGridHeightMapLoadStateLabel", m_ElevationGridHeightMapLoadStateLabel);
	m_builder -> get_widget ("ExtrusionExpander", m_ExtrusionExpander);
	m_builder -> get_widget ("ExtrusionBeginCapCheckButton", m_ExtrusionBeginCapCheckButton);
	m_builder -> get_widget ("ExtrusionEndCapCheckButton", m_ExtrusionEndCapCheckButton);
	m_builder -> get_widget ("SphereBox", m_SphereBox);
	m_builder -> get_widget ("SphereExpander", m_SphereExpander);
	m_builder -> get_widget ("SphereRadiusSpinButton", m_SphereRadiusSpinButton);
	m_builder -> get_widget ("SphereOptionsExpander", m_SphereOptionsExpander);
	m_builder -> get_widget ("SphereTypeButton", m_SphereTypeButton);
	m_builder -> get_widget ("SphereUseGlobalOptionsCheckButton", m_SphereUseGlobalOptionsCheckButton);
	m_builder -> get_widget ("SphereOptionsStack", m_SphereOptionsStack);
	m_builder -> get_widget ("QuadSphereOptionsGrid", m_QuadSphereOptionsGrid);
	m_builder -> get_widget ("QuadSphereXDimensionSpinButton", m_QuadSphereXDimensionSpinButton);
	m_builder -> get_widget ("QuadSphereYDimensionSpinButton", m_QuadSphereYDimensionSpinButton);
	m_builder -> get_widget ("OctahedronOptionsGrid", m_OctahedronOptionsGrid);
	m_builder -> get_widget ("OctahedronOrderSpinButton", m_OctahedronOrderSpinButton);
	m_builder -> get_widget ("OctahedronSphericalInterpolationCheckButton", m_OctahedronSphericalInterpolationCheckButton);
	m_builder -> get_widget ("IcosahedronOptionsGrid", m_IcosahedronOptionsGrid);
	m_builder -> get_widget ("IcosahedronOrderSpinButton", m_IcosahedronOrderSpinButton);
	m_builder -> get_widget ("IcosahedronSphericalInterpolationCheckButton", m_IcosahedronSphericalInterpolationCheckButton);
	m_builder -> get_widget ("GeoElevationGridBox", m_GeoElevationGridBox);
	m_builder -> get_widget ("GeoElevationGridExpander", m_GeoElevationGridExpander);
	m_builder -> get_widget ("GeoElevationGridXDimensionSpinButton", m_GeoElevationGridXDimensionSpinButton);
	m_builder -> get_widget ("GeoElevationGridZDimensionSpinButton", m_GeoElevationGridZDimensionSpinButton);
	m_builder -> get_widget ("GeoElevationGridXSpacingSpinButton", m_GeoElevationGridXSpacingSpinButton);
	m_builder -> get_widget ("GeoElevationGridZSpacingSpinButton", m_GeoElevationGridZSpacingSpinButton);
	m_builder -> get_widget ("GeoElevationGridYScaleSpinButton", m_GeoElevationGridYScaleSpinButton);
	m_builder -> get_widget ("GeoElevationGridCoordinateSystemComboBoxText", m_GeoElevationGridCoordinateSystemComboBoxText);
	m_builder -> get_widget ("GeoElevationGridEllipsoidBox", m_GeoElevationGridEllipsoidBox);
	m_builder -> get_widget ("GeoElevationGridEllipsoidComboBoxText", m_GeoElevationGridEllipsoidComboBoxText);
	m_builder -> get_widget ("GeoElevationGridGDOrderComboBoxText", m_GeoElevationGridGDOrderComboBoxText);
	m_builder -> get_widget ("GeoElevationGridGeoSystemUTMBox", m_GeoElevationGridGeoSystemUTMBox);
	m_builder -> get_widget ("GeoElevationGridHemisphereComboBoxText", m_GeoElevationGridHemisphereComboBoxText);
	m_builder -> get_widget ("GeoElevationGridUTMOrderComboBoxText", m_GeoElevationGridUTMOrderComboBoxText);
	m_builder -> get_widget ("GeoElevationGridGeoGridOriginBox", m_GeoElevationGridGeoGridOriginBox);
	m_builder -> get_widget ("GeoElevationGridGeoGridOriginXSpinButton", m_GeoElevationGridGeoGridOriginXSpinButton);
	m_builder -> get_widget ("GeoElevationGridGeoGridOriginYSpinButton", m_GeoElevationGridGeoGridOriginYSpinButton);
	m_builder -> get_widget ("GeoElevationGridGeoGridOriginZSpinButton", m_GeoElevationGridGeoGridOriginZSpinButton);
	m_builder -> get_widget ("GeoElevationGridHeightMapExpander", m_GeoElevationGridHeightMapExpander);
	m_builder -> get_widget ("GeoElevationGridHeightMapMinHeightSpinButton", m_GeoElevationGridHeightMapMinHeightSpinButton);
	m_builder -> get_widget ("GeoElevationGridHeightMapMaxHeightSpinButton", m_GeoElevationGridHeightMapMaxHeightSpinButton);
	m_builder -> get_widget ("GeoElevationGridHeightMapImageChooserButton", m_GeoElevationGridHeightMapImageChooserButton);
	m_builder -> get_widget ("GeoElevationGridHeightMapImageReloadButton", m_GeoElevationGridHeightMapImageReloadButton);
	m_builder -> get_widget ("GeoElevationGridHeightMapImageRemoveButton", m_GeoElevationGridHeightMapImageRemoveButton);
	m_builder -> get_widget ("GeoElevationGridHeightMapLoadStateLabel", m_GeoElevationGridHeightMapLoadStateLabel);
	m_builder -> get_widget ("NurbsCurveExpander", m_NurbsCurveExpander);
	m_builder -> get_widget ("NurbsCurveTessellationSpinButton", m_NurbsCurveTessellationSpinButton);
	m_builder -> get_widget ("NurbsCurveClosedCheckButton", m_NurbsCurveClosedCheckButton);
	m_builder -> get_widget ("NurbsCurveOrderSpinButton", m_NurbsCurveOrderSpinButton);
	m_builder -> get_widget ("NurbsTrimmedSurfaceExpander", m_NurbsTrimmedSurfaceExpander);
	m_builder -> get_widget ("NurbsTrimmedSurfaceUTessellationSpinButton", m_NurbsTrimmedSurfaceUTessellationSpinButton);
	m_builder -> get_widget ("NurbsTrimmedSurfaceVTessellationSpinButton", m_NurbsTrimmedSurfaceVTessellationSpinButton);
	m_builder -> get_widget ("NurbsTrimmedSurfaceUClosedCheckButton", m_NurbsTrimmedSurfaceUClosedCheckButton);
	m_builder -> get_widget ("NurbsTrimmedSurfaceVClosedCheckButton", m_NurbsTrimmedSurfaceVClosedCheckButton);
	m_builder -> get_widget ("NurbsTrimmedSurfaceUOrderSpinButton", m_NurbsTrimmedSurfaceUOrderSpinButton);
	m_builder -> get_widget ("NurbsTrimmedSurfaceVOrderSpinButton", m_NurbsTrimmedSurfaceVOrderSpinButton);
	m_builder -> get_widget ("NurbsTrimmedSurfaceUDimensionSpinButton", m_NurbsTrimmedSurfaceUDimensionSpinButton);
	m_builder -> get_widget ("NurbsTrimmedSurfaceVDimensionSpinButton", m_NurbsTrimmedSurfaceVDimensionSpinButton);
	m_builder -> get_widget ("NurbsPatchSurfaceExpander", m_NurbsPatchSurfaceExpander);
	m_builder -> get_widget ("NurbsPatchSurfaceUTessellationSpinButton", m_NurbsPatchSurfaceUTessellationSpinButton);
	m_builder -> get_widget ("NurbsPatchSurfaceVTessellationSpinButton", m_NurbsPatchSurfaceVTessellationSpinButton);
	m_builder -> get_widget ("NurbsPatchSurfaceUClosedCheckButton", m_NurbsPatchSurfaceUClosedCheckButton);
	m_builder -> get_widget ("NurbsPatchSurfaceVClosedCheckButton", m_NurbsPatchSurfaceVClosedCheckButton);
	m_builder -> get_widget ("NurbsPatchSurfaceUOrderSpinButton", m_NurbsPatchSurfaceUOrderSpinButton);
	m_builder -> get_widget ("NurbsPatchSurfaceVOrderSpinButton", m_NurbsPatchSurfaceVOrderSpinButton);
	m_builder -> get_widget ("NurbsPatchSurfaceUDimensionSpinButton", m_NurbsPatchSurfaceUDimensionSpinButton);
	m_builder -> get_widget ("NurbsPatchSurfaceVDimensionSpinButton", m_NurbsPatchSurfaceVDimensionSpinButton);
	m_builder -> get_widget ("IndexedLineSetBox", m_IndexedLineSetBox);
	m_builder -> get_widget ("IndexedLineSetOptionsExpander", m_IndexedLineSetOptionsExpander);
	m_builder -> get_widget ("IndexedLineSetTypeButton", m_IndexedLineSetTypeButton);
	m_builder -> get_widget ("IndexedLineSetOptionsStack", m_IndexedLineSetOptionsStack);
	m_builder -> get_widget ("IndexedLineSetLSystemOptions", m_IndexedLineSetLSystemOptions);
	m_builder -> get_widget ("IndexedLineSetLSystemIterationsSpinButton", m_IndexedLineSetLSystemIterationsSpinButton);
	m_builder -> get_widget ("IndexedLineSetLSystemConstantsEntry", m_IndexedLineSetLSystemConstantsEntry);
	m_builder -> get_widget ("IndexedLineSetLSystemAxiomEntry", m_IndexedLineSetLSystemAxiomEntry);
	m_builder -> get_widget ("IndexedLineSetLSystemXAngleBox", m_IndexedLineSetLSystemXAngleBox);
	m_builder -> get_widget ("IndexedLineSetLSystemXAngleSpinButton", m_IndexedLineSetLSystemXAngleSpinButton);
	m_builder -> get_widget ("IndexedLineSetLSystemAddRuleButton", m_IndexedLineSetLSystemAddRuleButton);
	m_builder -> get_widget ("IndexedLineSetLSystemRuleBox", m_IndexedLineSetLSystemRuleBox);
	m_builder -> get_widget ("IndexedLineSetLSystemSizeBox", m_IndexedLineSetLSystemSizeBox);
	m_builder -> get_widget ("IndexedLineSetLSystemSizeXSpinButton", m_IndexedLineSetLSystemSizeXSpinButton);
	m_builder -> get_widget ("IndexedLineSetLSystemSizeYSpinButton", m_IndexedLineSetLSystemSizeYSpinButton);
	m_builder -> get_widget ("IndexedLineSetLSystemSizeZSpinButton", m_IndexedLineSetLSystemSizeZSpinButton);
	m_builder -> get_widget ("IndexedLineSetLSystemUniformSizeButton", m_IndexedLineSetLSystemUniformSizeButton);
	m_builder -> get_widget ("IndexedLineSetLSystemUniformSizeImage", m_IndexedLineSetLSystemUniformSizeImage);
	m_builder -> get_widget ("IndexedLineSetLSystemReferenceButton", m_IndexedLineSetLSystemReferenceButton);
	m_builder -> get_widget ("IndexedLineSetLSystemZAngleBox", m_IndexedLineSetLSystemZAngleBox);
	m_builder -> get_widget ("IndexedLineSetLSystemZAngleSpinButton", m_IndexedLineSetLSystemZAngleSpinButton);
	m_builder -> get_widget ("IndexedLineSetLSystemYAngleBox", m_IndexedLineSetLSystemYAngleBox);
	m_builder -> get_widget ("IndexedLineSetLSystemYAngleSpinButton", m_IndexedLineSetLSystemYAngleSpinButton);
	m_builder -> get_widget ("IndexedLineSetLSystemLengthVariationSpinButton", m_IndexedLineSetLSystemLengthVariationSpinButton);
	m_builder -> get_widget ("IndexedLineSetLSystemAngleVariationSpinButton", m_IndexedLineSetLSystemAngleVariationSpinButton);
	m_builder -> get_widget ("CommonPropertiesExpander", m_CommonPropertiesExpander);
	m_builder -> get_widget ("SolidCheckButton", m_SolidCheckButton);
	m_builder -> get_widget ("CCWCheckButton", m_CCWCheckButton);
	m_builder -> get_widget ("ConvexCheckButton", m_ConvexCheckButton);
	m_builder -> get_widget ("ColorPerVertexCheckButton", m_ColorPerVertexCheckButton);
	m_builder -> get_widget ("NormalPerVertexCheckButton", m_NormalPerVertexCheckButton);
	m_builder -> get_widget ("CreaseAngleBox", m_CreaseAngleBox);
	m_builder -> get_widget ("CreaseAngleSpinButton", m_CreaseAngleSpinButton);
	m_builder -> get_widget ("CreaseAngleScale", m_CreaseAngleScale);
	m_builder -> get_widget ("CreaseAngleDoubleBox", m_CreaseAngleDoubleBox);
	m_builder -> get_widget ("CreaseAngleDoubleSpinButton", m_CreaseAngleDoubleSpinButton);
	m_builder -> get_widget ("CreaseAngleDoubleScale", m_CreaseAngleDoubleScale);
	m_builder -> get_widget ("NormalsBox", m_NormalsBox);
	m_builder -> get_widget ("AddNormalsButton", m_AddNormalsButton);
	m_builder -> get_widget ("RemoveNormalsButton", m_RemoveNormalsButton);
	m_builder -> get_widget ("ColorExpander", m_ColorExpander);
	m_builder -> get_widget ("CreateColorBox", m_CreateColorBox);
	m_builder -> get_widget ("ColorTypeButton", m_ColorTypeButton);
	m_builder -> get_widget ("ColorUnlinkButton", m_ColorUnlinkButton);
	m_builder -> get_widget ("ColorGrid", m_ColorGrid);
	m_builder -> get_widget ("ColorScrolledWindow", m_ColorScrolledWindow);
	m_builder -> get_widget ("AddColorButton", m_AddColorButton);
	m_builder -> get_widget ("RemoveColorButton", m_RemoveColorButton);
	m_builder -> get_widget ("ColorBox", m_ColorBox);
	m_builder -> get_widget ("ColorButton", m_ColorButton);
	m_builder -> get_widget ("ColorRGBAGrid", m_ColorRGBAGrid);
	m_builder -> get_widget ("ColorRGBAScrolledWindow", m_ColorRGBAScrolledWindow);
	m_builder -> get_widget ("AddColorRGBAButton", m_AddColorRGBAButton);
	m_builder -> get_widget ("RemoveColorRGBAButton", m_RemoveColorRGBAButton);
	m_builder -> get_widget ("ColorRGBABox", m_ColorRGBABox);
	m_builder -> get_widget ("ColorRGBAButton", m_ColorRGBAButton);
	m_builder -> get_widget ("PrimitiveCountEventBox", m_PrimitiveCountEventBox);
	m_builder -> get_widget ("PrimitiveCountExpander", m_PrimitiveCountExpander);
	m_builder -> get_widget ("PrimitiveCountBox", m_PrimitiveCountBox);
	m_builder -> get_widget ("PrimitiveCountNodesLabel", m_PrimitiveCountNodesLabel);
	m_builder -> get_widget ("PrimitiveCountOpaqueShapesLabel", m_PrimitiveCountOpaqueShapesLabel);
	m_builder -> get_widget ("PrimitiveCountTransparentShapesLabel", m_PrimitiveCountTransparentShapesLabel);
	m_builder -> get_widget ("PrimitiveCountPointsLabel", m_PrimitiveCountPointsLabel);
	m_builder -> get_widget ("PrimitiveCountLinesLabel", m_PrimitiveCountLinesLabel);
	m_builder -> get_widget ("PrimitiveCountTrianglesLabel", m_PrimitiveCountTrianglesLabel);
	m_builder -> get_widget ("PrimitiveCountQuadsLabel", m_PrimitiveCountQuadsLabel);
	m_builder -> get_widget ("PrimitiveCountPolygonsLabel", m_PrimitiveCountPolygonsLabel);
	m_builder -> get_widget ("PrimitiveCountCountButton", m_PrimitiveCountCountButton);
	m_builder -> get_widget ("GeometryToolExpander", m_GeometryToolExpander);
	m_builder -> get_widget ("NormalLengthSpinButton", m_NormalLengthSpinButton);
	m_builder -> get_widget ("NormalColorBox", m_NormalColorBox);
	m_builder -> get_widget ("NormalColorButton", m_NormalColorButton);
	m_builder -> get_widget ("NormalColorScale", m_NormalColorScale);
	m_builder -> get_widget ("EdgeColorBox", m_EdgeColorBox);
	m_builder -> get_widget ("EdgeColorButton", m_EdgeColorButton);
	m_builder -> get_widget ("EdgeColorScale", m_EdgeColorScale);
	m_builder -> get_widget ("PrimitiveEdgeColorBox", m_PrimitiveEdgeColorBox);
	m_builder -> get_widget ("PrimitiveEdgeColorButton", m_PrimitiveEdgeColorButton);
	m_builder -> get_widget ("PrimitiveEdgeColorScale", m_PrimitiveEdgeColorScale);

	// Connect object Gtk::Adjustment with id 'Disk2DInnerRadiusAdjustment'.
	m_Disk2DInnerRadiusAdjustment -> signal_value_changed () .connect (sigc::mem_fun (this, &X3DGeometryPropertiesEditorInterface::on_disk2d_radius_value_changed));
	m_Disk2DOuterRadiusAdjustment -> signal_value_changed () .connect (sigc::mem_fun (this, &X3DGeometryPropertiesEditorInterface::on_disk2d_radius_value_changed));
	m_NurbsPatchSurfaceUDimensionAdjustment -> signal_value_changed () .connect (sigc::mem_fun (this, &X3DGeometryPropertiesEditorInterface::on_nurbs_patch_surface_u_dimension_changed));
	m_NurbsPatchSurfaceVDimensionAdjustment -> signal_value_changed () .connect (sigc::mem_fun (this, &X3DGeometryPropertiesEditorInterface::on_nurbs_patch_surface_v_dimension_changed));
	m_NurbsTrimmedSurfaceUDimensionAdjustment -> signal_value_changed () .connect (sigc::mem_fun (this, &X3DGeometryPropertiesEditorInterface::on_nurbs_trimmed_surface_u_dimension_changed));
	m_NurbsTrimmedSurfaceVDimensionAdjustment -> signal_value_changed () .connect (sigc::mem_fun (this, &X3DGeometryPropertiesEditorInterface::on_nurbs_trimmed_surface_v_dimension_changed));

	// Connect object Gtk::ComboBoxText with id 'GeometryComboBoxText'.
	m_GeometryComboBoxText -> signal_changed () .connect (sigc::mem_fun (this, &X3DGeometryPropertiesEditorInterface::on_geometry_changed));

	// Connect object Gtk::Button with id 'GeometryUnlinkButton'.
	m_GeometryUnlinkButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DGeometryPropertiesEditorInterface::on_geometry_unlink_clicked));

	// Connect object Gtk::CheckButton with id 'Arc2DUseGlobalOptionsCheckButton'.
	m_Arc2DUseGlobalOptionsCheckButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DGeometryPropertiesEditorInterface::on_arc2d_use_global_options_toggled));
	m_ArcClose2DUseGlobalOptionsCheckButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DGeometryPropertiesEditorInterface::on_arcclose2d_use_global_options_toggled));
	m_Circle2DUseGlobalOptionsCheckButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DGeometryPropertiesEditorInterface::on_circle2d_use_global_options_toggled));
	m_Disk2DUseGlobalOptionsCheckButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DGeometryPropertiesEditorInterface::on_disk2d_use_global_options_toggled));

	// Connect object Gtk::ToggleButton with id 'Rectangle2DUniformSizeButton'.
	m_Rectangle2DUniformSizeButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DGeometryPropertiesEditorInterface::on_rectangle2d_uniform_size_clicked));
	m_BoxUniformSizeButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DGeometryPropertiesEditorInterface::on_box_uniform_size_clicked));

	// Connect object Gtk::CheckButton with id 'ConeUseGlobalOptionsCheckButton'.
	m_ConeUseGlobalOptionsCheckButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DGeometryPropertiesEditorInterface::on_cone_use_global_options_toggled));
	m_CylinderUseGlobalOptionsCheckButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DGeometryPropertiesEditorInterface::on_cylinder_use_global_options_toggled));

	// Connect object Gtk::ComboBoxText with id 'SphereTypeButton'.
	m_SphereTypeButton -> signal_changed () .connect (sigc::mem_fun (this, &X3DGeometryPropertiesEditorInterface::on_sphere_type_changed));

	// Connect object Gtk::CheckButton with id 'SphereUseGlobalOptionsCheckButton'.
	m_SphereUseGlobalOptionsCheckButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DGeometryPropertiesEditorInterface::on_sphere_use_global_options_toggled));

	// Connect object Gtk::ComboBoxText with id 'IndexedLineSetTypeButton'.
	m_IndexedLineSetTypeButton -> signal_changed () .connect (sigc::mem_fun (this, &X3DGeometryPropertiesEditorInterface::on_indexed_line_set_type_changed));

	// Connect object Gtk::ToggleButton with id 'IndexedLineSetLSystemUniformSizeButton'.
	m_IndexedLineSetLSystemUniformSizeButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DGeometryPropertiesEditorInterface::on_indexed_line_set_lsystem_uniform_size_clicked));

	// Connect object Gtk::CheckButton with id 'ColorPerVertexCheckButton'.
	m_ColorPerVertexCheckButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DGeometryPropertiesEditorInterface::on_color_per_vertex_toggled));
	m_NormalPerVertexCheckButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DGeometryPropertiesEditorInterface::on_normal_per_vertex_toggled));

	// Connect object Gtk::Button with id 'AddNormalsButton'.
	m_AddNormalsButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DGeometryPropertiesEditorInterface::on_add_normals_clicked));
	m_RemoveNormalsButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DGeometryPropertiesEditorInterface::on_remove_normals_clicked));

	// Connect object Gtk::ComboBoxText with id 'ColorTypeButton'.
	m_ColorTypeButton -> signal_changed () .connect (sigc::mem_fun (this, &X3DGeometryPropertiesEditorInterface::on_color_type_changed));

	// Connect object Gtk::Button with id 'ColorUnlinkButton'.
	m_ColorUnlinkButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DGeometryPropertiesEditorInterface::on_color_unlink_clicked));

	// Connect object Gtk::EventBox with id 'PrimitiveCountEventBox'.
	m_PrimitiveCountEventBox -> signal_enter_notify_event () .connect (sigc::mem_fun (this, &X3DGeometryPropertiesEditorInterface::on_crossing_notify_event));
	m_PrimitiveCountEventBox -> signal_leave_notify_event () .connect (sigc::mem_fun (this, &X3DGeometryPropertiesEditorInterface::on_crossing_notify_event));

	// Connect object Gtk::Box with id 'PrimitiveCountBox'.
	m_PrimitiveCountBox -> signal_map () .connect (sigc::mem_fun (this, &X3DGeometryPropertiesEditorInterface::on_map_primitive_count));
	m_PrimitiveCountBox -> signal_unmap () .connect (sigc::mem_fun (this, &X3DGeometryPropertiesEditorInterface::on_unmap_primitive_count));

	// Connect object Gtk::ComboBoxText with id 'PrimitiveCountCountButton'.
	m_PrimitiveCountCountButton -> signal_changed () .connect (sigc::mem_fun (this, &X3DGeometryPropertiesEditorInterface::on_primitive_count_count_changed));
}

X3DGeometryPropertiesEditorInterface::~X3DGeometryPropertiesEditorInterface ()
{
	delete m_Window;
}

} // puck
} // titania
