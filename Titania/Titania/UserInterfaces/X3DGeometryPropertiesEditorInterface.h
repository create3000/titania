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

#ifndef __TMP_GLAD2CPP_GEOMETRY_PROPERTIES_EDITOR_H__
#define __TMP_GLAD2CPP_GEOMETRY_PROPERTIES_EDITOR_H__

#include "../Base/X3DEditorInterface.h"

#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

/**
 *  Gtk Interface for GeometryPropertiesEditor.
 */
class X3DGeometryPropertiesEditorInterface :
	public X3DEditorInterface
{
public:

	///  @name Construction

	X3DGeometryPropertiesEditorInterface () :
		X3DEditorInterface ()
	{ }

	template <class ... Arguments>
	X3DGeometryPropertiesEditorInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DEditorInterface (arguments ...)
	{ create (filename); }

	template <class ... Arguments>
	X3DGeometryPropertiesEditorInterface (std::initializer_list <std::string> filenames, const Arguments & ... arguments) :
		X3DEditorInterface (arguments ...)
	{ create (filenames); }

	///  @name Member access

	const Glib::RefPtr <Gtk::Builder> &
	getBuilder () const
	{ return m_builder; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getArc2DDimensionAdjustment () const
	{ return m_Arc2DDimensionAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getArc2DEndAngleAdjustment () const
	{ return m_Arc2DEndAngleAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getArc2DRadiusAdjustment () const
	{ return m_Arc2DRadiusAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getArc2DStartAngleAdjustment () const
	{ return m_Arc2DStartAngleAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getArcClose2DDimensionAdjustment () const
	{ return m_ArcClose2DDimensionAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getArcClose2DEndAngleAdjustment () const
	{ return m_ArcClose2DEndAngleAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getArcClose2DRadiusAdjustment () const
	{ return m_ArcClose2DRadiusAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getArcClose2DStartAngleAdjustment () const
	{ return m_ArcClose2DStartAngleAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getBoxSizeXAdjustment () const
	{ return m_BoxSizeXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getBoxSizeYAdjustment () const
	{ return m_BoxSizeYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getBoxSizeZAdjustment () const
	{ return m_BoxSizeZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getCircle2DDimensionAdjustment () const
	{ return m_Circle2DDimensionAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getCircle2DRadiusAdjustment () const
	{ return m_Circle2DRadiusAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getColorAdjustment () const
	{ return m_ColorAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getColorRGBAAdjustment () const
	{ return m_ColorRGBAAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getConeBottomRadiusAdjustment () const
	{ return m_ConeBottomRadiusAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getConeHeightAdjustment () const
	{ return m_ConeHeightAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getConeXDimensionAdjustment () const
	{ return m_ConeXDimensionAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getCreaseAngleAdjustment () const
	{ return m_CreaseAngleAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getCreaseAngleDoubleAdjustment () const
	{ return m_CreaseAngleDoubleAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getCylinderHeightAdjustment () const
	{ return m_CylinderHeightAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getCylinderRadiusAdjustment () const
	{ return m_CylinderRadiusAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getCylinderXDimensionAdjustment () const
	{ return m_CylinderXDimensionAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getDisk2DDimensionAdjustment () const
	{ return m_Disk2DDimensionAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getDisk2DInnerRadiusAdjustment () const
	{ return m_Disk2DInnerRadiusAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getDisk2DOuterRadiusAdjustment () const
	{ return m_Disk2DOuterRadiusAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getEdgeColorAdjustment () const
	{ return m_EdgeColorAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getElevationGridHeightMapMaxHeightAdjustment () const
	{ return m_ElevationGridHeightMapMaxHeightAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getElevationGridHeightMapMinHeightAdjustment () const
	{ return m_ElevationGridHeightMapMinHeightAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getElevationGridXDimensionAdjustment () const
	{ return m_ElevationGridXDimensionAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getElevationGridXSpacingAdjustment () const
	{ return m_ElevationGridXSpacingAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getElevationGridZDimensionAdjustment () const
	{ return m_ElevationGridZDimensionAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getElevationGridZSpacingAdjustment () const
	{ return m_ElevationGridZSpacingAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGeoElevationGridGeoGridOriginXAdjustment () const
	{ return m_GeoElevationGridGeoGridOriginXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGeoElevationGridGeoGridOriginYAdjustment () const
	{ return m_GeoElevationGridGeoGridOriginYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGeoElevationGridGeoGridOriginZAdjustment () const
	{ return m_GeoElevationGridGeoGridOriginZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGeoElevationGridHeightMapMaxHeightAdjustment () const
	{ return m_GeoElevationGridHeightMapMaxHeightAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGeoElevationGridHeightMapMinHeightAdjustment () const
	{ return m_GeoElevationGridHeightMapMinHeightAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGeoElevationGridXDimensionAdjustment () const
	{ return m_GeoElevationGridXDimensionAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGeoElevationGridXSpacingAdjustment () const
	{ return m_GeoElevationGridXSpacingAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGeoElevationGridYScaleAdjustment () const
	{ return m_GeoElevationGridYScaleAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGeoElevationGridZDimensionAdjustment () const
	{ return m_GeoElevationGridZDimensionAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGeoElevationGridZSpacingAdjustment () const
	{ return m_GeoElevationGridZSpacingAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGeoElevationGridZoneAdjustment () const
	{ return m_GeoElevationGridZoneAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getIcosahedronDimensionAdjustment () const
	{ return m_IcosahedronDimensionAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getIndexedLineSetLSystemAngleVariationAdjustment () const
	{ return m_IndexedLineSetLSystemAngleVariationAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getIndexedLineSetLSystemIterationsAdjustment () const
	{ return m_IndexedLineSetLSystemIterationsAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getIndexedLineSetLSystemLengthVariationAdjustment () const
	{ return m_IndexedLineSetLSystemLengthVariationAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getIndexedLineSetLSystemSizeXAdjustment () const
	{ return m_IndexedLineSetLSystemSizeXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getIndexedLineSetLSystemSizeYAdjustment () const
	{ return m_IndexedLineSetLSystemSizeYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getIndexedLineSetLSystemSizeZAdjustment () const
	{ return m_IndexedLineSetLSystemSizeZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getIndexedLineSetLSystemXAngleAdjustment () const
	{ return m_IndexedLineSetLSystemXAngleAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getIndexedLineSetLSystemYAngleAdjustment () const
	{ return m_IndexedLineSetLSystemYAngleAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getIndexedLineSetLSystemZAngleAdjustment () const
	{ return m_IndexedLineSetLSystemZAngleAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getNormalColorAdjustment () const
	{ return m_NormalColorAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getNormalLengthAdjustment () const
	{ return m_NormalLengthAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getNurbsCurveOrderAdjustment () const
	{ return m_NurbsCurveOrderAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getNurbsCurveTessellationAdjustment () const
	{ return m_NurbsCurveTessellationAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getNurbsPatchSurfaceUDimensionAdjustment () const
	{ return m_NurbsPatchSurfaceUDimensionAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getNurbsPatchSurfaceUOrderAdjustment () const
	{ return m_NurbsPatchSurfaceUOrderAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getNurbsPatchSurfaceUTessellationAdjustment () const
	{ return m_NurbsPatchSurfaceUTessellationAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getNurbsPatchSurfaceVDimensionAdjustment () const
	{ return m_NurbsPatchSurfaceVDimensionAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getNurbsPatchSurfaceVOrderAdjustment () const
	{ return m_NurbsPatchSurfaceVOrderAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getNurbsPatchSurfaceVTessellationAdjustment () const
	{ return m_NurbsPatchSurfaceVTessellationAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getNurbsTrimmedSurfaceUDimensionAdjustment () const
	{ return m_NurbsTrimmedSurfaceUDimensionAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getNurbsTrimmedSurfaceUOrderAdjustment () const
	{ return m_NurbsTrimmedSurfaceUOrderAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getNurbsTrimmedSurfaceUTessellationAdjustment () const
	{ return m_NurbsTrimmedSurfaceUTessellationAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getNurbsTrimmedSurfaceVDimensionAdjustment () const
	{ return m_NurbsTrimmedSurfaceVDimensionAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getNurbsTrimmedSurfaceVOrderAdjustment () const
	{ return m_NurbsTrimmedSurfaceVOrderAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getNurbsTrimmedSurfaceVTessellationAdjustment () const
	{ return m_NurbsTrimmedSurfaceVTessellationAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getOctahedronDimensionAdjustment () const
	{ return m_OctahedronDimensionAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getPrimitiveEdgeColorAdjustment () const
	{ return m_PrimitiveEdgeColorAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getQuadSphereXDimensionAdjustment () const
	{ return m_QuadSphereXDimensionAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getQuadSphereYDimensionAdjustment () const
	{ return m_QuadSphereYDimensionAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getRectangle2DSizeXAdjustment () const
	{ return m_Rectangle2DSizeXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getRectangle2DSizeYAdjustment () const
	{ return m_Rectangle2DSizeYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSphereRadiusAdjustment () const
	{ return m_SphereRadiusAdjustment; }

	Gtk::Window &
	getWindow () const
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const
	{ return *m_Widget; }

	Gtk::HeaderBar &
	getHeaderBar () const
	{ return *m_HeaderBar; }

	Gtk::Notebook &
	getGeometryChildNotebook () const
	{ return *m_GeometryChildNotebook; }

	Gtk::Box &
	getSelectGeometryBox () const
	{ return *m_SelectGeometryBox; }

	Gtk::ComboBoxText &
	getGeometryComboBoxText () const
	{ return *m_GeometryComboBoxText; }

	Gtk::Button &
	getGeometryUnlinkButton () const
	{ return *m_GeometryUnlinkButton; }

	Gtk::Box &
	getGeometryBox () const
	{ return *m_GeometryBox; }

	Gtk::Stack &
	getGeometryStack () const
	{ return *m_GeometryStack; }

	Gtk::Box &
	getArc2DBox () const
	{ return *m_Arc2DBox; }

	Gtk::Expander &
	getArc2DExpander () const
	{ return *m_Arc2DExpander; }

	Gtk::SpinButton &
	getArc2DRadiusSpinButton () const
	{ return *m_Arc2DRadiusSpinButton; }

	Gtk::SpinButton &
	getArc2DStartAngleSpinButton () const
	{ return *m_Arc2DStartAngleSpinButton; }

	Gtk::SpinButton &
	getArc2DEndAngleSpinButton () const
	{ return *m_Arc2DEndAngleSpinButton; }

	Gtk::Expander &
	getArc2DOptionsExpander () const
	{ return *m_Arc2DOptionsExpander; }

	Gtk::CheckButton &
	getArc2DUseGlobalOptionsCheckButton () const
	{ return *m_Arc2DUseGlobalOptionsCheckButton; }

	Gtk::Grid &
	getArc2DOptionsGrid () const
	{ return *m_Arc2DOptionsGrid; }

	Gtk::SpinButton &
	getArc2DDimensionSpinButton () const
	{ return *m_Arc2DDimensionSpinButton; }

	Gtk::Box &
	getArcClose2DBox () const
	{ return *m_ArcClose2DBox; }

	Gtk::Expander &
	getArcClose2DExpander () const
	{ return *m_ArcClose2DExpander; }

	Gtk::SpinButton &
	getArcClose2DRadiusSpinButton () const
	{ return *m_ArcClose2DRadiusSpinButton; }

	Gtk::ComboBoxText &
	getArcClose2DClosureTypeComboBoxText () const
	{ return *m_ArcClose2DClosureTypeComboBoxText; }

	Gtk::SpinButton &
	getArcClose2DStartAngleSpinButton () const
	{ return *m_ArcClose2DStartAngleSpinButton; }

	Gtk::SpinButton &
	getArcClose2DEndAngleSpinButton () const
	{ return *m_ArcClose2DEndAngleSpinButton; }

	Gtk::Expander &
	getArcClose2DOptionsExpander () const
	{ return *m_ArcClose2DOptionsExpander; }

	Gtk::CheckButton &
	getArcClose2DUseGlobalOptionsCheckButton () const
	{ return *m_ArcClose2DUseGlobalOptionsCheckButton; }

	Gtk::Grid &
	getArcClose2DOptionsGrid () const
	{ return *m_ArcClose2DOptionsGrid; }

	Gtk::SpinButton &
	getArcClose2DDimensionSpinButton () const
	{ return *m_ArcClose2DDimensionSpinButton; }

	Gtk::Box &
	getCircle2DBox () const
	{ return *m_Circle2DBox; }

	Gtk::Expander &
	getCircle2DExpander () const
	{ return *m_Circle2DExpander; }

	Gtk::SpinButton &
	getCircle2DRadiusSpinButton () const
	{ return *m_Circle2DRadiusSpinButton; }

	Gtk::Expander &
	getCircle2DOptionsExpander () const
	{ return *m_Circle2DOptionsExpander; }

	Gtk::CheckButton &
	getCircle2DUseGlobalOptionsCheckButton () const
	{ return *m_Circle2DUseGlobalOptionsCheckButton; }

	Gtk::Grid &
	getCircle2DOptionsGrid () const
	{ return *m_Circle2DOptionsGrid; }

	Gtk::SpinButton &
	getCircle2DDimensionSpinButton () const
	{ return *m_Circle2DDimensionSpinButton; }

	Gtk::Box &
	getDisk2DBox () const
	{ return *m_Disk2DBox; }

	Gtk::Expander &
	getDisk2DExpander () const
	{ return *m_Disk2DExpander; }

	Gtk::SpinButton &
	getDisk2DInnerRadiusSpinButton () const
	{ return *m_Disk2DInnerRadiusSpinButton; }

	Gtk::SpinButton &
	getDisk2DOuterRadiusSpinButton () const
	{ return *m_Disk2DOuterRadiusSpinButton; }

	Gtk::Expander &
	getDisk2DOptionsExpander () const
	{ return *m_Disk2DOptionsExpander; }

	Gtk::CheckButton &
	getDisk2DUseGlobalOptionsCheckButton () const
	{ return *m_Disk2DUseGlobalOptionsCheckButton; }

	Gtk::Grid &
	getDisk2DOptionsGrid () const
	{ return *m_Disk2DOptionsGrid; }

	Gtk::SpinButton &
	getDisk2DDimensionSpinButton () const
	{ return *m_Disk2DDimensionSpinButton; }

	Gtk::Expander &
	getRectangle2DExpander () const
	{ return *m_Rectangle2DExpander; }

	Gtk::Box &
	getRectangle2DSizeBox () const
	{ return *m_Rectangle2DSizeBox; }

	Gtk::SpinButton &
	getRectangle2DSizeXSpinButton () const
	{ return *m_Rectangle2DSizeXSpinButton; }

	Gtk::SpinButton &
	getRectangle2DSizeYSpinButton () const
	{ return *m_Rectangle2DSizeYSpinButton; }

	Gtk::ToggleButton &
	getRectangle2DUniformSizeButton () const
	{ return *m_Rectangle2DUniformSizeButton; }

	Gtk::Image &
	getRectangle2DUniformSizeImage () const
	{ return *m_Rectangle2DUniformSizeImage; }

	Gtk::Expander &
	getBoxExpander () const
	{ return *m_BoxExpander; }

	Gtk::Box &
	getBoxSizeBox () const
	{ return *m_BoxSizeBox; }

	Gtk::SpinButton &
	getBoxSizeXSpinButton () const
	{ return *m_BoxSizeXSpinButton; }

	Gtk::SpinButton &
	getBoxSizeYSpinButton () const
	{ return *m_BoxSizeYSpinButton; }

	Gtk::SpinButton &
	getBoxSizeZSpinButton () const
	{ return *m_BoxSizeZSpinButton; }

	Gtk::ToggleButton &
	getBoxUniformSizeButton () const
	{ return *m_BoxUniformSizeButton; }

	Gtk::Image &
	getBoxUniformSizeImage () const
	{ return *m_BoxUniformSizeImage; }

	Gtk::Box &
	getConeBox () const
	{ return *m_ConeBox; }

	Gtk::Expander &
	getConeExpander () const
	{ return *m_ConeExpander; }

	Gtk::SpinButton &
	getConeBottomRadiusSpinButton () const
	{ return *m_ConeBottomRadiusSpinButton; }

	Gtk::SpinButton &
	getConeHeightSpinButton () const
	{ return *m_ConeHeightSpinButton; }

	Gtk::CheckButton &
	getConeSideCheckButton () const
	{ return *m_ConeSideCheckButton; }

	Gtk::CheckButton &
	getConeBottomCheckButton () const
	{ return *m_ConeBottomCheckButton; }

	Gtk::Expander &
	getConeOptionsExpander () const
	{ return *m_ConeOptionsExpander; }

	Gtk::CheckButton &
	getConeUseGlobalOptionsCheckButton () const
	{ return *m_ConeUseGlobalOptionsCheckButton; }

	Gtk::Grid &
	getConeOptionsGrid () const
	{ return *m_ConeOptionsGrid; }

	Gtk::SpinButton &
	getConeXDimensionSpinButton () const
	{ return *m_ConeXDimensionSpinButton; }

	Gtk::Box &
	getCylinderBox () const
	{ return *m_CylinderBox; }

	Gtk::Expander &
	getCylinderExpander () const
	{ return *m_CylinderExpander; }

	Gtk::SpinButton &
	getCylinderRadiusSpinButton () const
	{ return *m_CylinderRadiusSpinButton; }

	Gtk::SpinButton &
	getCylinderHeightSpinButton () const
	{ return *m_CylinderHeightSpinButton; }

	Gtk::CheckButton &
	getCylinderBottomCheckButton () const
	{ return *m_CylinderBottomCheckButton; }

	Gtk::CheckButton &
	getCylinderSideCheckButton () const
	{ return *m_CylinderSideCheckButton; }

	Gtk::CheckButton &
	getCylinderTopCheckButton () const
	{ return *m_CylinderTopCheckButton; }

	Gtk::Expander &
	getCylinderOptionsExpander () const
	{ return *m_CylinderOptionsExpander; }

	Gtk::CheckButton &
	getCylinderUseGlobalOptionsCheckButton () const
	{ return *m_CylinderUseGlobalOptionsCheckButton; }

	Gtk::Grid &
	getCylinderOptionsGrid () const
	{ return *m_CylinderOptionsGrid; }

	Gtk::SpinButton &
	getCylinderXDimensionSpinButton () const
	{ return *m_CylinderXDimensionSpinButton; }

	Gtk::Box &
	getElevationGridBox () const
	{ return *m_ElevationGridBox; }

	Gtk::Expander &
	getElevationGridExpander () const
	{ return *m_ElevationGridExpander; }

	Gtk::SpinButton &
	getElevationGridXDimensionSpinButton () const
	{ return *m_ElevationGridXDimensionSpinButton; }

	Gtk::SpinButton &
	getElevationGridZDimensionSpinButton () const
	{ return *m_ElevationGridZDimensionSpinButton; }

	Gtk::SpinButton &
	getElevationGridXSpacingSpinButton () const
	{ return *m_ElevationGridXSpacingSpinButton; }

	Gtk::SpinButton &
	getElevationGridZSpacingSpinButton () const
	{ return *m_ElevationGridZSpacingSpinButton; }

	Gtk::Expander &
	getElevationGridHeightMapExpander () const
	{ return *m_ElevationGridHeightMapExpander; }

	Gtk::SpinButton &
	getElevationGridHeightMapMinHeightSpinButton () const
	{ return *m_ElevationGridHeightMapMinHeightSpinButton; }

	Gtk::SpinButton &
	getElevationGridHeightMapMaxHeightSpinButton () const
	{ return *m_ElevationGridHeightMapMaxHeightSpinButton; }

	Gtk::FileChooserButton &
	getElevationGridHeightMapImageChooserButton () const
	{ return *m_ElevationGridHeightMapImageChooserButton; }

	Gtk::Button &
	getElevationGridHeightMapImageReloadButton () const
	{ return *m_ElevationGridHeightMapImageReloadButton; }

	Gtk::Button &
	getElevationGridHeightMapImageRemoveButton () const
	{ return *m_ElevationGridHeightMapImageRemoveButton; }

	Gtk::Label &
	getElevationGridHeightMapLoadStateLabel () const
	{ return *m_ElevationGridHeightMapLoadStateLabel; }

	Gtk::Expander &
	getExtrusionExpander () const
	{ return *m_ExtrusionExpander; }

	Gtk::CheckButton &
	getExtrusionBeginCapCheckButton () const
	{ return *m_ExtrusionBeginCapCheckButton; }

	Gtk::CheckButton &
	getExtrusionEndCapCheckButton () const
	{ return *m_ExtrusionEndCapCheckButton; }

	Gtk::Box &
	getSphereBox () const
	{ return *m_SphereBox; }

	Gtk::Expander &
	getSphereExpander () const
	{ return *m_SphereExpander; }

	Gtk::SpinButton &
	getSphereRadiusSpinButton () const
	{ return *m_SphereRadiusSpinButton; }

	Gtk::Expander &
	getSphereOptionsExpander () const
	{ return *m_SphereOptionsExpander; }

	Gtk::ComboBoxText &
	getSphereTypeButton () const
	{ return *m_SphereTypeButton; }

	Gtk::CheckButton &
	getSphereUseGlobalOptionsCheckButton () const
	{ return *m_SphereUseGlobalOptionsCheckButton; }

	Gtk::Stack &
	getSphereOptionsStack () const
	{ return *m_SphereOptionsStack; }

	Gtk::Grid &
	getQuadSphereOptionsGrid () const
	{ return *m_QuadSphereOptionsGrid; }

	Gtk::SpinButton &
	getQuadSphereXDimensionSpinButton () const
	{ return *m_QuadSphereXDimensionSpinButton; }

	Gtk::SpinButton &
	getQuadSphereYDimensionSpinButton () const
	{ return *m_QuadSphereYDimensionSpinButton; }

	Gtk::Grid &
	getOctahedronOptionsGrid () const
	{ return *m_OctahedronOptionsGrid; }

	Gtk::SpinButton &
	getOctahedronOrderSpinButton () const
	{ return *m_OctahedronOrderSpinButton; }

	Gtk::CheckButton &
	getOctahedronSphericalInterpolationCheckButton () const
	{ return *m_OctahedronSphericalInterpolationCheckButton; }

	Gtk::Grid &
	getIcosahedronOptionsGrid () const
	{ return *m_IcosahedronOptionsGrid; }

	Gtk::SpinButton &
	getIcosahedronOrderSpinButton () const
	{ return *m_IcosahedronOrderSpinButton; }

	Gtk::CheckButton &
	getIcosahedronSphericalInterpolationCheckButton () const
	{ return *m_IcosahedronSphericalInterpolationCheckButton; }

	Gtk::Box &
	getGeoElevationGridBox () const
	{ return *m_GeoElevationGridBox; }

	Gtk::Expander &
	getGeoElevationGridExpander () const
	{ return *m_GeoElevationGridExpander; }

	Gtk::SpinButton &
	getGeoElevationGridXDimensionSpinButton () const
	{ return *m_GeoElevationGridXDimensionSpinButton; }

	Gtk::SpinButton &
	getGeoElevationGridZDimensionSpinButton () const
	{ return *m_GeoElevationGridZDimensionSpinButton; }

	Gtk::SpinButton &
	getGeoElevationGridXSpacingSpinButton () const
	{ return *m_GeoElevationGridXSpacingSpinButton; }

	Gtk::SpinButton &
	getGeoElevationGridZSpacingSpinButton () const
	{ return *m_GeoElevationGridZSpacingSpinButton; }

	Gtk::SpinButton &
	getGeoElevationGridYScaleSpinButton () const
	{ return *m_GeoElevationGridYScaleSpinButton; }

	Gtk::ComboBoxText &
	getGeoElevationGridCoordinateSystemComboBoxText () const
	{ return *m_GeoElevationGridCoordinateSystemComboBoxText; }

	Gtk::Box &
	getGeoElevationGridEllipsoidBox () const
	{ return *m_GeoElevationGridEllipsoidBox; }

	Gtk::ComboBoxText &
	getGeoElevationGridEllipsoidComboBoxText () const
	{ return *m_GeoElevationGridEllipsoidComboBoxText; }

	Gtk::ComboBoxText &
	getGeoElevationGridGDOrderComboBoxText () const
	{ return *m_GeoElevationGridGDOrderComboBoxText; }

	Gtk::Box &
	getGeoElevationGridGeoSystemUTMBox () const
	{ return *m_GeoElevationGridGeoSystemUTMBox; }

	Gtk::ComboBoxText &
	getGeoElevationGridHemisphereComboBoxText () const
	{ return *m_GeoElevationGridHemisphereComboBoxText; }

	Gtk::ComboBoxText &
	getGeoElevationGridUTMOrderComboBoxText () const
	{ return *m_GeoElevationGridUTMOrderComboBoxText; }

	Gtk::Box &
	getGeoElevationGridGeoGridOriginBox () const
	{ return *m_GeoElevationGridGeoGridOriginBox; }

	Gtk::SpinButton &
	getGeoElevationGridGeoGridOriginXSpinButton () const
	{ return *m_GeoElevationGridGeoGridOriginXSpinButton; }

	Gtk::SpinButton &
	getGeoElevationGridGeoGridOriginYSpinButton () const
	{ return *m_GeoElevationGridGeoGridOriginYSpinButton; }

	Gtk::SpinButton &
	getGeoElevationGridGeoGridOriginZSpinButton () const
	{ return *m_GeoElevationGridGeoGridOriginZSpinButton; }

	Gtk::Expander &
	getGeoElevationGridHeightMapExpander () const
	{ return *m_GeoElevationGridHeightMapExpander; }

	Gtk::SpinButton &
	getGeoElevationGridHeightMapMinHeightSpinButton () const
	{ return *m_GeoElevationGridHeightMapMinHeightSpinButton; }

	Gtk::SpinButton &
	getGeoElevationGridHeightMapMaxHeightSpinButton () const
	{ return *m_GeoElevationGridHeightMapMaxHeightSpinButton; }

	Gtk::FileChooserButton &
	getGeoElevationGridHeightMapImageChooserButton () const
	{ return *m_GeoElevationGridHeightMapImageChooserButton; }

	Gtk::Button &
	getGeoElevationGridHeightMapImageReloadButton () const
	{ return *m_GeoElevationGridHeightMapImageReloadButton; }

	Gtk::Button &
	getGeoElevationGridHeightMapImageRemoveButton () const
	{ return *m_GeoElevationGridHeightMapImageRemoveButton; }

	Gtk::Label &
	getGeoElevationGridHeightMapLoadStateLabel () const
	{ return *m_GeoElevationGridHeightMapLoadStateLabel; }

	Gtk::Expander &
	getNurbsCurveExpander () const
	{ return *m_NurbsCurveExpander; }

	Gtk::SpinButton &
	getNurbsCurveTessellationSpinButton () const
	{ return *m_NurbsCurveTessellationSpinButton; }

	Gtk::CheckButton &
	getNurbsCurveClosedCheckButton () const
	{ return *m_NurbsCurveClosedCheckButton; }

	Gtk::SpinButton &
	getNurbsCurveOrderSpinButton () const
	{ return *m_NurbsCurveOrderSpinButton; }

	Gtk::Expander &
	getNurbsTrimmedSurfaceExpander () const
	{ return *m_NurbsTrimmedSurfaceExpander; }

	Gtk::SpinButton &
	getNurbsTrimmedSurfaceUTessellationSpinButton () const
	{ return *m_NurbsTrimmedSurfaceUTessellationSpinButton; }

	Gtk::SpinButton &
	getNurbsTrimmedSurfaceVTessellationSpinButton () const
	{ return *m_NurbsTrimmedSurfaceVTessellationSpinButton; }

	Gtk::CheckButton &
	getNurbsTrimmedSurfaceUClosedCheckButton () const
	{ return *m_NurbsTrimmedSurfaceUClosedCheckButton; }

	Gtk::CheckButton &
	getNurbsTrimmedSurfaceVClosedCheckButton () const
	{ return *m_NurbsTrimmedSurfaceVClosedCheckButton; }

	Gtk::SpinButton &
	getNurbsTrimmedSurfaceUOrderSpinButton () const
	{ return *m_NurbsTrimmedSurfaceUOrderSpinButton; }

	Gtk::SpinButton &
	getNurbsTrimmedSurfaceVOrderSpinButton () const
	{ return *m_NurbsTrimmedSurfaceVOrderSpinButton; }

	Gtk::SpinButton &
	getNurbsTrimmedSurfaceUDimensionSpinButton () const
	{ return *m_NurbsTrimmedSurfaceUDimensionSpinButton; }

	Gtk::SpinButton &
	getNurbsTrimmedSurfaceVDimensionSpinButton () const
	{ return *m_NurbsTrimmedSurfaceVDimensionSpinButton; }

	Gtk::Expander &
	getNurbsPatchSurfaceExpander () const
	{ return *m_NurbsPatchSurfaceExpander; }

	Gtk::SpinButton &
	getNurbsPatchSurfaceUTessellationSpinButton () const
	{ return *m_NurbsPatchSurfaceUTessellationSpinButton; }

	Gtk::SpinButton &
	getNurbsPatchSurfaceVTessellationSpinButton () const
	{ return *m_NurbsPatchSurfaceVTessellationSpinButton; }

	Gtk::CheckButton &
	getNurbsPatchSurfaceUClosedCheckButton () const
	{ return *m_NurbsPatchSurfaceUClosedCheckButton; }

	Gtk::CheckButton &
	getNurbsPatchSurfaceVClosedCheckButton () const
	{ return *m_NurbsPatchSurfaceVClosedCheckButton; }

	Gtk::SpinButton &
	getNurbsPatchSurfaceUOrderSpinButton () const
	{ return *m_NurbsPatchSurfaceUOrderSpinButton; }

	Gtk::SpinButton &
	getNurbsPatchSurfaceVOrderSpinButton () const
	{ return *m_NurbsPatchSurfaceVOrderSpinButton; }

	Gtk::SpinButton &
	getNurbsPatchSurfaceUDimensionSpinButton () const
	{ return *m_NurbsPatchSurfaceUDimensionSpinButton; }

	Gtk::SpinButton &
	getNurbsPatchSurfaceVDimensionSpinButton () const
	{ return *m_NurbsPatchSurfaceVDimensionSpinButton; }

	Gtk::Box &
	getIndexedLineSetBox () const
	{ return *m_IndexedLineSetBox; }

	Gtk::Expander &
	getIndexedLineSetOptionsExpander () const
	{ return *m_IndexedLineSetOptionsExpander; }

	Gtk::ComboBoxText &
	getIndexedLineSetTypeButton () const
	{ return *m_IndexedLineSetTypeButton; }

	Gtk::Stack &
	getIndexedLineSetOptionsStack () const
	{ return *m_IndexedLineSetOptionsStack; }

	Gtk::Grid &
	getIndexedLineSetLSystemOptions () const
	{ return *m_IndexedLineSetLSystemOptions; }

	Gtk::SpinButton &
	getIndexedLineSetLSystemIterationsSpinButton () const
	{ return *m_IndexedLineSetLSystemIterationsSpinButton; }

	Gtk::Entry &
	getIndexedLineSetLSystemConstantsEntry () const
	{ return *m_IndexedLineSetLSystemConstantsEntry; }

	Gtk::Entry &
	getIndexedLineSetLSystemAxiomEntry () const
	{ return *m_IndexedLineSetLSystemAxiomEntry; }

	Gtk::Box &
	getIndexedLineSetLSystemXAngleBox () const
	{ return *m_IndexedLineSetLSystemXAngleBox; }

	Gtk::SpinButton &
	getIndexedLineSetLSystemXAngleSpinButton () const
	{ return *m_IndexedLineSetLSystemXAngleSpinButton; }

	Gtk::Button &
	getIndexedLineSetLSystemAddRuleButton () const
	{ return *m_IndexedLineSetLSystemAddRuleButton; }

	Gtk::Box &
	getIndexedLineSetLSystemRuleBox () const
	{ return *m_IndexedLineSetLSystemRuleBox; }

	Gtk::Box &
	getIndexedLineSetLSystemSizeBox () const
	{ return *m_IndexedLineSetLSystemSizeBox; }

	Gtk::SpinButton &
	getIndexedLineSetLSystemSizeXSpinButton () const
	{ return *m_IndexedLineSetLSystemSizeXSpinButton; }

	Gtk::SpinButton &
	getIndexedLineSetLSystemSizeYSpinButton () const
	{ return *m_IndexedLineSetLSystemSizeYSpinButton; }

	Gtk::SpinButton &
	getIndexedLineSetLSystemSizeZSpinButton () const
	{ return *m_IndexedLineSetLSystemSizeZSpinButton; }

	Gtk::ToggleButton &
	getIndexedLineSetLSystemUniformSizeButton () const
	{ return *m_IndexedLineSetLSystemUniformSizeButton; }

	Gtk::Image &
	getIndexedLineSetLSystemUniformSizeImage () const
	{ return *m_IndexedLineSetLSystemUniformSizeImage; }

	Gtk::LinkButton &
	getIndexedLineSetLSystemReferenceButton () const
	{ return *m_IndexedLineSetLSystemReferenceButton; }

	Gtk::Box &
	getIndexedLineSetLSystemZAngleBox () const
	{ return *m_IndexedLineSetLSystemZAngleBox; }

	Gtk::SpinButton &
	getIndexedLineSetLSystemZAngleSpinButton () const
	{ return *m_IndexedLineSetLSystemZAngleSpinButton; }

	Gtk::Box &
	getIndexedLineSetLSystemYAngleBox () const
	{ return *m_IndexedLineSetLSystemYAngleBox; }

	Gtk::SpinButton &
	getIndexedLineSetLSystemYAngleSpinButton () const
	{ return *m_IndexedLineSetLSystemYAngleSpinButton; }

	Gtk::SpinButton &
	getIndexedLineSetLSystemLengthVariationSpinButton () const
	{ return *m_IndexedLineSetLSystemLengthVariationSpinButton; }

	Gtk::SpinButton &
	getIndexedLineSetLSystemAngleVariationSpinButton () const
	{ return *m_IndexedLineSetLSystemAngleVariationSpinButton; }

	Gtk::Expander &
	getCommonPropertiesExpander () const
	{ return *m_CommonPropertiesExpander; }

	Gtk::CheckButton &
	getSolidCheckButton () const
	{ return *m_SolidCheckButton; }

	Gtk::CheckButton &
	getCCWCheckButton () const
	{ return *m_CCWCheckButton; }

	Gtk::CheckButton &
	getConvexCheckButton () const
	{ return *m_ConvexCheckButton; }

	Gtk::CheckButton &
	getColorPerVertexCheckButton () const
	{ return *m_ColorPerVertexCheckButton; }

	Gtk::CheckButton &
	getNormalPerVertexCheckButton () const
	{ return *m_NormalPerVertexCheckButton; }

	Gtk::Box &
	getCreaseAngleBox () const
	{ return *m_CreaseAngleBox; }

	Gtk::SpinButton &
	getCreaseAngleSpinButton () const
	{ return *m_CreaseAngleSpinButton; }

	Gtk::Scale &
	getCreaseAngleScale () const
	{ return *m_CreaseAngleScale; }

	Gtk::Box &
	getCreaseAngleDoubleBox () const
	{ return *m_CreaseAngleDoubleBox; }

	Gtk::SpinButton &
	getCreaseAngleDoubleSpinButton () const
	{ return *m_CreaseAngleDoubleSpinButton; }

	Gtk::Scale &
	getCreaseAngleDoubleScale () const
	{ return *m_CreaseAngleDoubleScale; }

	Gtk::Box &
	getNormalsBox () const
	{ return *m_NormalsBox; }

	Gtk::Button &
	getAddNormalsButton () const
	{ return *m_AddNormalsButton; }

	Gtk::Button &
	getRemoveNormalsButton () const
	{ return *m_RemoveNormalsButton; }

	Gtk::Expander &
	getColorExpander () const
	{ return *m_ColorExpander; }

	Gtk::Box &
	getCreateColorBox () const
	{ return *m_CreateColorBox; }

	Gtk::ComboBoxText &
	getColorTypeButton () const
	{ return *m_ColorTypeButton; }

	Gtk::Button &
	getColorUnlinkButton () const
	{ return *m_ColorUnlinkButton; }

	Gtk::Grid &
	getColorGrid () const
	{ return *m_ColorGrid; }

	Gtk::ScrolledWindow &
	getColorScrolledWindow () const
	{ return *m_ColorScrolledWindow; }

	Gtk::Button &
	getAddColorButton () const
	{ return *m_AddColorButton; }

	Gtk::Button &
	getRemoveColorButton () const
	{ return *m_RemoveColorButton; }

	Gtk::Box &
	getColorBox () const
	{ return *m_ColorBox; }

	Gtk::Button &
	getColorButton () const
	{ return *m_ColorButton; }

	Gtk::Grid &
	getColorRGBAGrid () const
	{ return *m_ColorRGBAGrid; }

	Gtk::ScrolledWindow &
	getColorRGBAScrolledWindow () const
	{ return *m_ColorRGBAScrolledWindow; }

	Gtk::Button &
	getAddColorRGBAButton () const
	{ return *m_AddColorRGBAButton; }

	Gtk::Button &
	getRemoveColorRGBAButton () const
	{ return *m_RemoveColorRGBAButton; }

	Gtk::Box &
	getColorRGBABox () const
	{ return *m_ColorRGBABox; }

	Gtk::Button &
	getColorRGBAButton () const
	{ return *m_ColorRGBAButton; }

	Gtk::EventBox &
	getPrimitiveCountEventBox () const
	{ return *m_PrimitiveCountEventBox; }

	Gtk::Expander &
	getPrimitiveCountExpander () const
	{ return *m_PrimitiveCountExpander; }

	Gtk::Box &
	getPrimitiveCountBox () const
	{ return *m_PrimitiveCountBox; }

	Gtk::Label &
	getPrimitiveCountNodesLabel () const
	{ return *m_PrimitiveCountNodesLabel; }

	Gtk::Label &
	getPrimitiveCountOpaqueShapesLabel () const
	{ return *m_PrimitiveCountOpaqueShapesLabel; }

	Gtk::Label &
	getPrimitiveCountTransparentShapesLabel () const
	{ return *m_PrimitiveCountTransparentShapesLabel; }

	Gtk::Label &
	getPrimitiveCountPointsLabel () const
	{ return *m_PrimitiveCountPointsLabel; }

	Gtk::Label &
	getPrimitiveCountLinesLabel () const
	{ return *m_PrimitiveCountLinesLabel; }

	Gtk::Label &
	getPrimitiveCountTrianglesLabel () const
	{ return *m_PrimitiveCountTrianglesLabel; }

	Gtk::Label &
	getPrimitiveCountQuadsLabel () const
	{ return *m_PrimitiveCountQuadsLabel; }

	Gtk::Label &
	getPrimitiveCountPolygonsLabel () const
	{ return *m_PrimitiveCountPolygonsLabel; }

	Gtk::ComboBoxText &
	getPrimitiveCountCountButton () const
	{ return *m_PrimitiveCountCountButton; }

	Gtk::Expander &
	getGeometryToolExpander () const
	{ return *m_GeometryToolExpander; }

	Gtk::SpinButton &
	getNormalLengthSpinButton () const
	{ return *m_NormalLengthSpinButton; }

	Gtk::Box &
	getNormalColorBox () const
	{ return *m_NormalColorBox; }

	Gtk::Button &
	getNormalColorButton () const
	{ return *m_NormalColorButton; }

	Gtk::Scale &
	getNormalColorScale () const
	{ return *m_NormalColorScale; }

	Gtk::Box &
	getEdgeColorBox () const
	{ return *m_EdgeColorBox; }

	Gtk::Button &
	getEdgeColorButton () const
	{ return *m_EdgeColorButton; }

	Gtk::Scale &
	getEdgeColorScale () const
	{ return *m_EdgeColorScale; }

	Gtk::Box &
	getPrimitiveEdgeColorBox () const
	{ return *m_PrimitiveEdgeColorBox; }

	Gtk::Button &
	getPrimitiveEdgeColorButton () const
	{ return *m_PrimitiveEdgeColorButton; }

	Gtk::Scale &
	getPrimitiveEdgeColorScale () const
	{ return *m_PrimitiveEdgeColorScale; }

	///  @name Signal handlers

	virtual
	void
	on_disk2d_radius_value_changed () = 0;

	virtual
	void
	on_nurbs_patch_surface_u_dimension_changed () = 0;

	virtual
	void
	on_nurbs_patch_surface_v_dimension_changed () = 0;

	virtual
	void
	on_nurbs_trimmed_surface_u_dimension_changed () = 0;

	virtual
	void
	on_nurbs_trimmed_surface_v_dimension_changed () = 0;

	virtual
	void
	on_geometry_changed () = 0;

	virtual
	void
	on_geometry_unlink_clicked () = 0;

	virtual
	void
	on_arc2d_use_global_options_toggled () = 0;

	virtual
	void
	on_arcclose2d_use_global_options_toggled () = 0;

	virtual
	void
	on_circle2d_use_global_options_toggled () = 0;

	virtual
	void
	on_disk2d_use_global_options_toggled () = 0;

	virtual
	void
	on_rectangle2d_uniform_size_clicked () = 0;

	virtual
	void
	on_box_uniform_size_clicked () = 0;

	virtual
	void
	on_cone_use_global_options_toggled () = 0;

	virtual
	void
	on_cylinder_use_global_options_toggled () = 0;

	virtual
	void
	on_sphere_type_changed () = 0;

	virtual
	void
	on_sphere_use_global_options_toggled () = 0;

	virtual
	void
	on_indexed_line_set_type_changed () = 0;

	virtual
	void
	on_indexed_line_set_lsystem_uniform_size_clicked () = 0;

	virtual
	void
	on_color_per_vertex_toggled () = 0;

	virtual
	void
	on_normal_per_vertex_toggled () = 0;

	virtual
	void
	on_add_normals_clicked () = 0;

	virtual
	void
	on_remove_normals_clicked () = 0;

	virtual
	void
	on_color_type_changed () = 0;

	virtual
	void
	on_color_unlink_clicked () = 0;

	virtual
	bool
	on_crossing_notify_event (GdkEventCrossing* crossing_event) = 0;

	virtual
	void
	on_map_primitive_count () = 0;

	virtual
	void
	on_unmap_primitive_count () = 0;

	virtual
	void
	on_primitive_count_count_changed () = 0;

	///  @name Destruction

	virtual
	~X3DGeometryPropertiesEditorInterface () override;


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
	Glib::RefPtr <Gtk::Adjustment> m_Arc2DDimensionAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_Arc2DEndAngleAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_Arc2DRadiusAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_Arc2DStartAngleAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_ArcClose2DDimensionAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_ArcClose2DEndAngleAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_ArcClose2DRadiusAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_ArcClose2DStartAngleAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_BoxSizeXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_BoxSizeYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_BoxSizeZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_Circle2DDimensionAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_Circle2DRadiusAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_ColorAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_ColorRGBAAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_ConeBottomRadiusAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_ConeHeightAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_ConeXDimensionAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_CreaseAngleAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_CreaseAngleDoubleAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_CylinderHeightAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_CylinderRadiusAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_CylinderXDimensionAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_Disk2DDimensionAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_Disk2DInnerRadiusAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_Disk2DOuterRadiusAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_EdgeColorAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_ElevationGridHeightMapMaxHeightAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_ElevationGridHeightMapMinHeightAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_ElevationGridXDimensionAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_ElevationGridXSpacingAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_ElevationGridZDimensionAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_ElevationGridZSpacingAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GeoElevationGridGeoGridOriginXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GeoElevationGridGeoGridOriginYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GeoElevationGridGeoGridOriginZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GeoElevationGridHeightMapMaxHeightAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GeoElevationGridHeightMapMinHeightAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GeoElevationGridXDimensionAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GeoElevationGridXSpacingAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GeoElevationGridYScaleAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GeoElevationGridZDimensionAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GeoElevationGridZSpacingAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GeoElevationGridZoneAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_IcosahedronDimensionAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_IndexedLineSetLSystemAngleVariationAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_IndexedLineSetLSystemIterationsAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_IndexedLineSetLSystemLengthVariationAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_IndexedLineSetLSystemSizeXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_IndexedLineSetLSystemSizeYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_IndexedLineSetLSystemSizeZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_IndexedLineSetLSystemXAngleAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_IndexedLineSetLSystemYAngleAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_IndexedLineSetLSystemZAngleAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_NormalColorAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_NormalLengthAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_NurbsCurveOrderAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_NurbsCurveTessellationAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_NurbsPatchSurfaceUDimensionAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_NurbsPatchSurfaceUOrderAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_NurbsPatchSurfaceUTessellationAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_NurbsPatchSurfaceVDimensionAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_NurbsPatchSurfaceVOrderAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_NurbsPatchSurfaceVTessellationAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_NurbsTrimmedSurfaceUDimensionAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_NurbsTrimmedSurfaceUOrderAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_NurbsTrimmedSurfaceUTessellationAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_NurbsTrimmedSurfaceVDimensionAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_NurbsTrimmedSurfaceVOrderAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_NurbsTrimmedSurfaceVTessellationAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_OctahedronDimensionAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_PrimitiveEdgeColorAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_QuadSphereXDimensionAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_QuadSphereYDimensionAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_Rectangle2DSizeXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_Rectangle2DSizeYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_SphereRadiusAdjustment;
	Gtk::Window* m_Window;
	Gtk::Box* m_Widget;
	Gtk::HeaderBar* m_HeaderBar;
	Gtk::Notebook* m_GeometryChildNotebook;
	Gtk::Box* m_SelectGeometryBox;
	Gtk::ComboBoxText* m_GeometryComboBoxText;
	Gtk::Button* m_GeometryUnlinkButton;
	Gtk::Box* m_GeometryBox;
	Gtk::Stack* m_GeometryStack;
	Gtk::Box* m_Arc2DBox;
	Gtk::Expander* m_Arc2DExpander;
	Gtk::SpinButton* m_Arc2DRadiusSpinButton;
	Gtk::SpinButton* m_Arc2DStartAngleSpinButton;
	Gtk::SpinButton* m_Arc2DEndAngleSpinButton;
	Gtk::Expander* m_Arc2DOptionsExpander;
	Gtk::CheckButton* m_Arc2DUseGlobalOptionsCheckButton;
	Gtk::Grid* m_Arc2DOptionsGrid;
	Gtk::SpinButton* m_Arc2DDimensionSpinButton;
	Gtk::Box* m_ArcClose2DBox;
	Gtk::Expander* m_ArcClose2DExpander;
	Gtk::SpinButton* m_ArcClose2DRadiusSpinButton;
	Gtk::ComboBoxText* m_ArcClose2DClosureTypeComboBoxText;
	Gtk::SpinButton* m_ArcClose2DStartAngleSpinButton;
	Gtk::SpinButton* m_ArcClose2DEndAngleSpinButton;
	Gtk::Expander* m_ArcClose2DOptionsExpander;
	Gtk::CheckButton* m_ArcClose2DUseGlobalOptionsCheckButton;
	Gtk::Grid* m_ArcClose2DOptionsGrid;
	Gtk::SpinButton* m_ArcClose2DDimensionSpinButton;
	Gtk::Box* m_Circle2DBox;
	Gtk::Expander* m_Circle2DExpander;
	Gtk::SpinButton* m_Circle2DRadiusSpinButton;
	Gtk::Expander* m_Circle2DOptionsExpander;
	Gtk::CheckButton* m_Circle2DUseGlobalOptionsCheckButton;
	Gtk::Grid* m_Circle2DOptionsGrid;
	Gtk::SpinButton* m_Circle2DDimensionSpinButton;
	Gtk::Box* m_Disk2DBox;
	Gtk::Expander* m_Disk2DExpander;
	Gtk::SpinButton* m_Disk2DInnerRadiusSpinButton;
	Gtk::SpinButton* m_Disk2DOuterRadiusSpinButton;
	Gtk::Expander* m_Disk2DOptionsExpander;
	Gtk::CheckButton* m_Disk2DUseGlobalOptionsCheckButton;
	Gtk::Grid* m_Disk2DOptionsGrid;
	Gtk::SpinButton* m_Disk2DDimensionSpinButton;
	Gtk::Expander* m_Rectangle2DExpander;
	Gtk::Box* m_Rectangle2DSizeBox;
	Gtk::SpinButton* m_Rectangle2DSizeXSpinButton;
	Gtk::SpinButton* m_Rectangle2DSizeYSpinButton;
	Gtk::ToggleButton* m_Rectangle2DUniformSizeButton;
	Gtk::Image* m_Rectangle2DUniformSizeImage;
	Gtk::Expander* m_BoxExpander;
	Gtk::Box* m_BoxSizeBox;
	Gtk::SpinButton* m_BoxSizeXSpinButton;
	Gtk::SpinButton* m_BoxSizeYSpinButton;
	Gtk::SpinButton* m_BoxSizeZSpinButton;
	Gtk::ToggleButton* m_BoxUniformSizeButton;
	Gtk::Image* m_BoxUniformSizeImage;
	Gtk::Box* m_ConeBox;
	Gtk::Expander* m_ConeExpander;
	Gtk::SpinButton* m_ConeBottomRadiusSpinButton;
	Gtk::SpinButton* m_ConeHeightSpinButton;
	Gtk::CheckButton* m_ConeSideCheckButton;
	Gtk::CheckButton* m_ConeBottomCheckButton;
	Gtk::Expander* m_ConeOptionsExpander;
	Gtk::CheckButton* m_ConeUseGlobalOptionsCheckButton;
	Gtk::Grid* m_ConeOptionsGrid;
	Gtk::SpinButton* m_ConeXDimensionSpinButton;
	Gtk::Box* m_CylinderBox;
	Gtk::Expander* m_CylinderExpander;
	Gtk::SpinButton* m_CylinderRadiusSpinButton;
	Gtk::SpinButton* m_CylinderHeightSpinButton;
	Gtk::CheckButton* m_CylinderBottomCheckButton;
	Gtk::CheckButton* m_CylinderSideCheckButton;
	Gtk::CheckButton* m_CylinderTopCheckButton;
	Gtk::Expander* m_CylinderOptionsExpander;
	Gtk::CheckButton* m_CylinderUseGlobalOptionsCheckButton;
	Gtk::Grid* m_CylinderOptionsGrid;
	Gtk::SpinButton* m_CylinderXDimensionSpinButton;
	Gtk::Box* m_ElevationGridBox;
	Gtk::Expander* m_ElevationGridExpander;
	Gtk::SpinButton* m_ElevationGridXDimensionSpinButton;
	Gtk::SpinButton* m_ElevationGridZDimensionSpinButton;
	Gtk::SpinButton* m_ElevationGridXSpacingSpinButton;
	Gtk::SpinButton* m_ElevationGridZSpacingSpinButton;
	Gtk::Expander* m_ElevationGridHeightMapExpander;
	Gtk::SpinButton* m_ElevationGridHeightMapMinHeightSpinButton;
	Gtk::SpinButton* m_ElevationGridHeightMapMaxHeightSpinButton;
	Gtk::FileChooserButton* m_ElevationGridHeightMapImageChooserButton;
	Gtk::Button* m_ElevationGridHeightMapImageReloadButton;
	Gtk::Button* m_ElevationGridHeightMapImageRemoveButton;
	Gtk::Label* m_ElevationGridHeightMapLoadStateLabel;
	Gtk::Expander* m_ExtrusionExpander;
	Gtk::CheckButton* m_ExtrusionBeginCapCheckButton;
	Gtk::CheckButton* m_ExtrusionEndCapCheckButton;
	Gtk::Box* m_SphereBox;
	Gtk::Expander* m_SphereExpander;
	Gtk::SpinButton* m_SphereRadiusSpinButton;
	Gtk::Expander* m_SphereOptionsExpander;
	Gtk::ComboBoxText* m_SphereTypeButton;
	Gtk::CheckButton* m_SphereUseGlobalOptionsCheckButton;
	Gtk::Stack* m_SphereOptionsStack;
	Gtk::Grid* m_QuadSphereOptionsGrid;
	Gtk::SpinButton* m_QuadSphereXDimensionSpinButton;
	Gtk::SpinButton* m_QuadSphereYDimensionSpinButton;
	Gtk::Grid* m_OctahedronOptionsGrid;
	Gtk::SpinButton* m_OctahedronOrderSpinButton;
	Gtk::CheckButton* m_OctahedronSphericalInterpolationCheckButton;
	Gtk::Grid* m_IcosahedronOptionsGrid;
	Gtk::SpinButton* m_IcosahedronOrderSpinButton;
	Gtk::CheckButton* m_IcosahedronSphericalInterpolationCheckButton;
	Gtk::Box* m_GeoElevationGridBox;
	Gtk::Expander* m_GeoElevationGridExpander;
	Gtk::SpinButton* m_GeoElevationGridXDimensionSpinButton;
	Gtk::SpinButton* m_GeoElevationGridZDimensionSpinButton;
	Gtk::SpinButton* m_GeoElevationGridXSpacingSpinButton;
	Gtk::SpinButton* m_GeoElevationGridZSpacingSpinButton;
	Gtk::SpinButton* m_GeoElevationGridYScaleSpinButton;
	Gtk::ComboBoxText* m_GeoElevationGridCoordinateSystemComboBoxText;
	Gtk::Box* m_GeoElevationGridEllipsoidBox;
	Gtk::ComboBoxText* m_GeoElevationGridEllipsoidComboBoxText;
	Gtk::ComboBoxText* m_GeoElevationGridGDOrderComboBoxText;
	Gtk::Box* m_GeoElevationGridGeoSystemUTMBox;
	Gtk::ComboBoxText* m_GeoElevationGridHemisphereComboBoxText;
	Gtk::ComboBoxText* m_GeoElevationGridUTMOrderComboBoxText;
	Gtk::Box* m_GeoElevationGridGeoGridOriginBox;
	Gtk::SpinButton* m_GeoElevationGridGeoGridOriginXSpinButton;
	Gtk::SpinButton* m_GeoElevationGridGeoGridOriginYSpinButton;
	Gtk::SpinButton* m_GeoElevationGridGeoGridOriginZSpinButton;
	Gtk::Expander* m_GeoElevationGridHeightMapExpander;
	Gtk::SpinButton* m_GeoElevationGridHeightMapMinHeightSpinButton;
	Gtk::SpinButton* m_GeoElevationGridHeightMapMaxHeightSpinButton;
	Gtk::FileChooserButton* m_GeoElevationGridHeightMapImageChooserButton;
	Gtk::Button* m_GeoElevationGridHeightMapImageReloadButton;
	Gtk::Button* m_GeoElevationGridHeightMapImageRemoveButton;
	Gtk::Label* m_GeoElevationGridHeightMapLoadStateLabel;
	Gtk::Expander* m_NurbsCurveExpander;
	Gtk::SpinButton* m_NurbsCurveTessellationSpinButton;
	Gtk::CheckButton* m_NurbsCurveClosedCheckButton;
	Gtk::SpinButton* m_NurbsCurveOrderSpinButton;
	Gtk::Expander* m_NurbsTrimmedSurfaceExpander;
	Gtk::SpinButton* m_NurbsTrimmedSurfaceUTessellationSpinButton;
	Gtk::SpinButton* m_NurbsTrimmedSurfaceVTessellationSpinButton;
	Gtk::CheckButton* m_NurbsTrimmedSurfaceUClosedCheckButton;
	Gtk::CheckButton* m_NurbsTrimmedSurfaceVClosedCheckButton;
	Gtk::SpinButton* m_NurbsTrimmedSurfaceUOrderSpinButton;
	Gtk::SpinButton* m_NurbsTrimmedSurfaceVOrderSpinButton;
	Gtk::SpinButton* m_NurbsTrimmedSurfaceUDimensionSpinButton;
	Gtk::SpinButton* m_NurbsTrimmedSurfaceVDimensionSpinButton;
	Gtk::Expander* m_NurbsPatchSurfaceExpander;
	Gtk::SpinButton* m_NurbsPatchSurfaceUTessellationSpinButton;
	Gtk::SpinButton* m_NurbsPatchSurfaceVTessellationSpinButton;
	Gtk::CheckButton* m_NurbsPatchSurfaceUClosedCheckButton;
	Gtk::CheckButton* m_NurbsPatchSurfaceVClosedCheckButton;
	Gtk::SpinButton* m_NurbsPatchSurfaceUOrderSpinButton;
	Gtk::SpinButton* m_NurbsPatchSurfaceVOrderSpinButton;
	Gtk::SpinButton* m_NurbsPatchSurfaceUDimensionSpinButton;
	Gtk::SpinButton* m_NurbsPatchSurfaceVDimensionSpinButton;
	Gtk::Box* m_IndexedLineSetBox;
	Gtk::Expander* m_IndexedLineSetOptionsExpander;
	Gtk::ComboBoxText* m_IndexedLineSetTypeButton;
	Gtk::Stack* m_IndexedLineSetOptionsStack;
	Gtk::Grid* m_IndexedLineSetLSystemOptions;
	Gtk::SpinButton* m_IndexedLineSetLSystemIterationsSpinButton;
	Gtk::Entry* m_IndexedLineSetLSystemConstantsEntry;
	Gtk::Entry* m_IndexedLineSetLSystemAxiomEntry;
	Gtk::Box* m_IndexedLineSetLSystemXAngleBox;
	Gtk::SpinButton* m_IndexedLineSetLSystemXAngleSpinButton;
	Gtk::Button* m_IndexedLineSetLSystemAddRuleButton;
	Gtk::Box* m_IndexedLineSetLSystemRuleBox;
	Gtk::Box* m_IndexedLineSetLSystemSizeBox;
	Gtk::SpinButton* m_IndexedLineSetLSystemSizeXSpinButton;
	Gtk::SpinButton* m_IndexedLineSetLSystemSizeYSpinButton;
	Gtk::SpinButton* m_IndexedLineSetLSystemSizeZSpinButton;
	Gtk::ToggleButton* m_IndexedLineSetLSystemUniformSizeButton;
	Gtk::Image* m_IndexedLineSetLSystemUniformSizeImage;
	Gtk::LinkButton* m_IndexedLineSetLSystemReferenceButton;
	Gtk::Box* m_IndexedLineSetLSystemZAngleBox;
	Gtk::SpinButton* m_IndexedLineSetLSystemZAngleSpinButton;
	Gtk::Box* m_IndexedLineSetLSystemYAngleBox;
	Gtk::SpinButton* m_IndexedLineSetLSystemYAngleSpinButton;
	Gtk::SpinButton* m_IndexedLineSetLSystemLengthVariationSpinButton;
	Gtk::SpinButton* m_IndexedLineSetLSystemAngleVariationSpinButton;
	Gtk::Expander* m_CommonPropertiesExpander;
	Gtk::CheckButton* m_SolidCheckButton;
	Gtk::CheckButton* m_CCWCheckButton;
	Gtk::CheckButton* m_ConvexCheckButton;
	Gtk::CheckButton* m_ColorPerVertexCheckButton;
	Gtk::CheckButton* m_NormalPerVertexCheckButton;
	Gtk::Box* m_CreaseAngleBox;
	Gtk::SpinButton* m_CreaseAngleSpinButton;
	Gtk::Scale* m_CreaseAngleScale;
	Gtk::Box* m_CreaseAngleDoubleBox;
	Gtk::SpinButton* m_CreaseAngleDoubleSpinButton;
	Gtk::Scale* m_CreaseAngleDoubleScale;
	Gtk::Box* m_NormalsBox;
	Gtk::Button* m_AddNormalsButton;
	Gtk::Button* m_RemoveNormalsButton;
	Gtk::Expander* m_ColorExpander;
	Gtk::Box* m_CreateColorBox;
	Gtk::ComboBoxText* m_ColorTypeButton;
	Gtk::Button* m_ColorUnlinkButton;
	Gtk::Grid* m_ColorGrid;
	Gtk::ScrolledWindow* m_ColorScrolledWindow;
	Gtk::Button* m_AddColorButton;
	Gtk::Button* m_RemoveColorButton;
	Gtk::Box* m_ColorBox;
	Gtk::Button* m_ColorButton;
	Gtk::Grid* m_ColorRGBAGrid;
	Gtk::ScrolledWindow* m_ColorRGBAScrolledWindow;
	Gtk::Button* m_AddColorRGBAButton;
	Gtk::Button* m_RemoveColorRGBAButton;
	Gtk::Box* m_ColorRGBABox;
	Gtk::Button* m_ColorRGBAButton;
	Gtk::EventBox* m_PrimitiveCountEventBox;
	Gtk::Expander* m_PrimitiveCountExpander;
	Gtk::Box* m_PrimitiveCountBox;
	Gtk::Label* m_PrimitiveCountNodesLabel;
	Gtk::Label* m_PrimitiveCountOpaqueShapesLabel;
	Gtk::Label* m_PrimitiveCountTransparentShapesLabel;
	Gtk::Label* m_PrimitiveCountPointsLabel;
	Gtk::Label* m_PrimitiveCountLinesLabel;
	Gtk::Label* m_PrimitiveCountTrianglesLabel;
	Gtk::Label* m_PrimitiveCountQuadsLabel;
	Gtk::Label* m_PrimitiveCountPolygonsLabel;
	Gtk::ComboBoxText* m_PrimitiveCountCountButton;
	Gtk::Expander* m_GeometryToolExpander;
	Gtk::SpinButton* m_NormalLengthSpinButton;
	Gtk::Box* m_NormalColorBox;
	Gtk::Button* m_NormalColorButton;
	Gtk::Scale* m_NormalColorScale;
	Gtk::Box* m_EdgeColorBox;
	Gtk::Button* m_EdgeColorButton;
	Gtk::Scale* m_EdgeColorScale;
	Gtk::Box* m_PrimitiveEdgeColorBox;
	Gtk::Button* m_PrimitiveEdgeColorButton;
	Gtk::Scale* m_PrimitiveEdgeColorScale;

};

} // puck
} // titania

#endif
