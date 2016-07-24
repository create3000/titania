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
#include "X3DGeometryPropertiesEditorInterface.h"

namespace titania {
namespace puck {

const std::string X3DGeometryPropertiesEditorInterface::m_widgetName = "GeometryPropertiesEditor";

void
X3DGeometryPropertiesEditorInterface::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	// Get objects.
	m_Arc2DEndAngleAdjustment              = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("Arc2DEndAngleAdjustment"));
	m_Arc2DMinAngleAdjustment              = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("Arc2DMinAngleAdjustment"));
	m_Arc2DRadiusAdjustment                = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("Arc2DRadiusAdjustment"));
	m_Arc2DStartAngleAdjustment            = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("Arc2DStartAngleAdjustment"));
	m_ArcClose2DEndAngleAdjustment         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ArcClose2DEndAngleAdjustment"));
	m_ArcClose2DMinAngleAdjustment         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ArcClose2DMinAngleAdjustment"));
	m_ArcClose2DRadiusAdjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ArcClose2DRadiusAdjustment"));
	m_ArcClose2DStartAngleAdjustment       = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ArcClose2DStartAngleAdjustment"));
	m_BoxSizeXAdjustment                   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BoxSizeXAdjustment"));
	m_BoxSizeYAdjustment                   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BoxSizeYAdjustment"));
	m_BoxSizeZAdjustment                   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BoxSizeZAdjustment"));
	m_Circle2DDimensionAdjustment          = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("Circle2DDimensionAdjustment"));
	m_Circle2DRadiusAdjustment             = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("Circle2DRadiusAdjustment"));
	m_ConeBottomRadiusAdjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ConeBottomRadiusAdjustment"));
	m_ConeHeightAdjustment                 = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ConeHeightAdjustment"));
	m_ConeXDimensionAdjustment             = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ConeXDimensionAdjustment"));
	m_CreaseAngleAdjustment                = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("CreaseAngleAdjustment"));
	m_CylinderHeightAdjustment             = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("CylinderHeightAdjustment"));
	m_CylinderRadiusAdjustment             = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("CylinderRadiusAdjustment"));
	m_CylinderXDimensionAdjustment         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("CylinderXDimensionAdjustment"));
	m_Disk2DDimensionAdjustment            = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("Disk2DDimensionAdjustment"));
	m_Disk2DInnerRadiusAdjustment          = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("Disk2DInnerRadiusAdjustment"));
	m_Disk2DOuterRadiusAdjustment          = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("Disk2DOuterRadiusAdjustment"));
	m_EdgeColorAdjustment                  = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("EdgeColorAdjustment"));
	m_ElevationGridXDimensionAdjustment    = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ElevationGridXDimensionAdjustment"));
	m_ElevationGridXSpacingAdjustment      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ElevationGridXSpacingAdjustment"));
	m_ElevationGridZDimensionAdjustment    = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ElevationGridZDimensionAdjustment"));
	m_ElevationGridZSpacingAdjustment      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ElevationGridZSpacingAdjustment"));
	m_GeoElevationGridXDimensionAdjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoElevationGridXDimensionAdjustment"));
	m_GeoElevationGridXSpacingAdjustment   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoElevationGridXSpacingAdjustment"));
	m_GeoElevationGridYScaleAdjustment     = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoElevationGridYScaleAdjustment"));
	m_GeoElevationGridZDimensionAdjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoElevationGridZDimensionAdjustment"));
	m_GeoElevationGridZSpacingAdjustment   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoElevationGridZSpacingAdjustment"));
	m_GeoElevationGridZoneAdjustment       = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("GeoElevationGridZoneAdjustment"));
	m_NormalColorAdjustment                = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("NormalColorAdjustment"));
	m_NormalLengthAdjustment               = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("NormalLengthAdjustment"));
	m_Rectangle2DSizeXAdjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("Rectangle2DSizeXAdjustment"));
	m_Rectangle2DSizeYAdjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("Rectangle2DSizeYAdjustment"));
	m_SphereRadiusAdjustment               = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("SphereRadiusAdjustment"));
	m_SphereXDimensionAdjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("SphereXDimensionAdjustment"));
	m_SphereYDimensionAdjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("SphereYDimensionAdjustment"));

	// Get widgets.
	m_builder -> get_widget ("Window", m_Window);
	m_builder -> get_widget ("Widget", m_Widget);
	m_builder -> get_widget ("GeometryChildNotebook", m_GeometryChildNotebook);
	m_builder -> get_widget ("SelectGeometryBox", m_SelectGeometryBox);
	m_builder -> get_widget ("GeometryComboBoxText", m_GeometryComboBoxText);
	m_builder -> get_widget ("GeometryUnlinkButton", m_GeometryUnlinkButton);
	m_builder -> get_widget ("Arc2DExpander", m_Arc2DExpander);
	m_builder -> get_widget ("Arc2DStartAngleSpinButton", m_Arc2DStartAngleSpinButton);
	m_builder -> get_widget ("Arc2DEndAngleSpinButton", m_Arc2DEndAngleSpinButton);
	m_builder -> get_widget ("Arc2DRadiusSpinButton", m_Arc2DRadiusSpinButton);
	m_builder -> get_widget ("Arc2DMinAngleSpinButton", m_Arc2DMinAngleSpinButton);
	m_builder -> get_widget ("ArcClose2DExpander", m_ArcClose2DExpander);
	m_builder -> get_widget ("ArcClose2DRadiusSpinButton", m_ArcClose2DRadiusSpinButton);
	m_builder -> get_widget ("ArcClose2DEndAngleSpinButton", m_ArcClose2DEndAngleSpinButton);
	m_builder -> get_widget ("ArcClose2DStartAngleSpinButton", m_ArcClose2DStartAngleSpinButton);
	m_builder -> get_widget ("ArcClose2DClosureTypeComboBoxText", m_ArcClose2DClosureTypeComboBoxText);
	m_builder -> get_widget ("ArcClose2DMinAngleSpinButton", m_ArcClose2DMinAngleSpinButton);
	m_builder -> get_widget ("Circle2DExpander", m_Circle2DExpander);
	m_builder -> get_widget ("Circle2DRadiusSpinButton", m_Circle2DRadiusSpinButton);
	m_builder -> get_widget ("Circle2DDimensionSpinButton", m_Circle2DDimensionSpinButton);
	m_builder -> get_widget ("Disk2DExpander", m_Disk2DExpander);
	m_builder -> get_widget ("Disk2DInnerRadiusSpinButton", m_Disk2DInnerRadiusSpinButton);
	m_builder -> get_widget ("Disk2DOuterRadiusSpinButton", m_Disk2DOuterRadiusSpinButton);
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
	m_builder -> get_widget ("BoSizeYSpinButton", m_BoSizeYSpinButton);
	m_builder -> get_widget ("BoxSizeZSpinButton", m_BoxSizeZSpinButton);
	m_builder -> get_widget ("BoxUniformSizeButton", m_BoxUniformSizeButton);
	m_builder -> get_widget ("BoxUniformSizeImage", m_BoxUniformSizeImage);
	m_builder -> get_widget ("ConeExpander", m_ConeExpander);
	m_builder -> get_widget ("ConeBottomRadiusSpinButton", m_ConeBottomRadiusSpinButton);
	m_builder -> get_widget ("ConeHeightSpinButton", m_ConeHeightSpinButton);
	m_builder -> get_widget ("ConeSideCheckButton", m_ConeSideCheckButton);
	m_builder -> get_widget ("ConeBottomCheckButton", m_ConeBottomCheckButton);
	m_builder -> get_widget ("ConeXDimensionSpinButton", m_ConeXDimensionSpinButton);
	m_builder -> get_widget ("CylinderExpander", m_CylinderExpander);
	m_builder -> get_widget ("CylinderRadiusSpinButton", m_CylinderRadiusSpinButton);
	m_builder -> get_widget ("CylinderHeightSpinButton", m_CylinderHeightSpinButton);
	m_builder -> get_widget ("CylinderBottomCheckButton", m_CylinderBottomCheckButton);
	m_builder -> get_widget ("CylinderSideCheckButton", m_CylinderSideCheckButton);
	m_builder -> get_widget ("CylinderTopCheckButton", m_CylinderTopCheckButton);
	m_builder -> get_widget ("CylinderXDimensionSpinButton", m_CylinderXDimensionSpinButton);
	m_builder -> get_widget ("ElevationGridExpander", m_ElevationGridExpander);
	m_builder -> get_widget ("ElevationGridXDimensionSpinButton", m_ElevationGridXDimensionSpinButton);
	m_builder -> get_widget ("ElevationGridZDimensionSpinButton", m_ElevationGridZDimensionSpinButton);
	m_builder -> get_widget ("ElevationGridXSpacingSpinButton", m_ElevationGridXSpacingSpinButton);
	m_builder -> get_widget ("ElevationGridZSpacingSpinButton", m_ElevationGridZSpacingSpinButton);
	m_builder -> get_widget ("ExtrusionExpander", m_ExtrusionExpander);
	m_builder -> get_widget ("ExtrusionBeginCapCheckButton", m_ExtrusionBeginCapCheckButton);
	m_builder -> get_widget ("ExtrusionEndCapCheckButton", m_ExtrusionEndCapCheckButton);
	m_builder -> get_widget ("SphereExpander", m_SphereExpander);
	m_builder -> get_widget ("SphereRadiusSpinButton", m_SphereRadiusSpinButton);
	m_builder -> get_widget ("SphereXDimensionSpinButton", m_SphereXDimensionSpinButton);
	m_builder -> get_widget ("SphereYDimensionSpinButton", m_SphereYDimensionSpinButton);
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
	m_builder -> get_widget ("GeometryExpander", m_GeometryExpander);
	m_builder -> get_widget ("SolidCheckButton", m_SolidCheckButton);
	m_builder -> get_widget ("CCWCheckButton", m_CCWCheckButton);
	m_builder -> get_widget ("ConvexCheckButton", m_ConvexCheckButton);
	m_builder -> get_widget ("CreaseAngleBox", m_CreaseAngleBox);
	m_builder -> get_widget ("CreaseAngleSpinButton", m_CreaseAngleSpinButton);
	m_builder -> get_widget ("CreaseAngleScale", m_CreaseAngleScale);
	m_builder -> get_widget ("NormalsBox", m_NormalsBox);
	m_builder -> get_widget ("AddNormalsButton", m_AddNormalsButton);
	m_builder -> get_widget ("RemoveNormalsButton", m_RemoveNormalsButton);
	m_builder -> get_widget ("ColorPerVertexCheckButton", m_ColorPerVertexCheckButton);
	m_builder -> get_widget ("NormalPerVertexCheckButton", m_NormalPerVertexCheckButton);
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

	// Connect object Gtk::ComboBoxText with id 'GeometryComboBoxText'.
	m_GeometryComboBoxText -> signal_changed () .connect (sigc::mem_fun (*this, &X3DGeometryPropertiesEditorInterface::on_geometry_changed));

	// Connect object Gtk::Button with id 'GeometryUnlinkButton'.
	m_GeometryUnlinkButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DGeometryPropertiesEditorInterface::on_geometry_unlink_clicked));

	// Connect object Gtk::ToggleButton with id 'Rectangle2DUniformSizeButton'.
	m_Rectangle2DUniformSizeButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DGeometryPropertiesEditorInterface::on_rectangle2d_uniform_size_clicked));
	m_BoxUniformSizeButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DGeometryPropertiesEditorInterface::on_box_uniform_size_clicked));

	// Connect object Gtk::Button with id 'AddNormalsButton'.
	m_AddNormalsButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DGeometryPropertiesEditorInterface::on_add_normals_clicked));
	m_RemoveNormalsButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DGeometryPropertiesEditorInterface::on_remove_normals_clicked));

	// Connect object Gtk::EventBox with id 'PrimitiveCountEventBox'.
	m_PrimitiveCountEventBox -> signal_enter_notify_event () .connect (sigc::mem_fun (*this, &X3DGeometryPropertiesEditorInterface::on_crossing_notify_event));
	m_PrimitiveCountEventBox -> signal_leave_notify_event () .connect (sigc::mem_fun (*this, &X3DGeometryPropertiesEditorInterface::on_crossing_notify_event));

	// Connect object Gtk::Box with id 'PrimitiveCountBox'.
	m_PrimitiveCountBox -> signal_map () .connect (sigc::mem_fun (*this, &X3DGeometryPropertiesEditorInterface::on_map_primitive_count));
	m_PrimitiveCountBox -> signal_unmap () .connect (sigc::mem_fun (*this, &X3DGeometryPropertiesEditorInterface::on_unmap_primitive_count));

	// Connect object Gtk::ComboBoxText with id 'PrimitiveCountCountButton'.
	m_PrimitiveCountCountButton -> signal_changed () .connect (sigc::mem_fun (*this, &X3DGeometryPropertiesEditorInterface::on_primitive_count_count_changed));

	// Call construct handler of base class.
	construct ();
}

X3DGeometryPropertiesEditorInterface::~X3DGeometryPropertiesEditorInterface ()
{
	delete m_Window;
}

} // puck
} // titania
