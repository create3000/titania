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
	getArc2DEndAngleAdjustment () const
	{ return m_Arc2DEndAngleAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getArc2DMinAngleAdjustment () const
	{ return m_Arc2DMinAngleAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getArc2DRadiusAdjustment () const
	{ return m_Arc2DRadiusAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getArc2DStartAngleAdjustment () const
	{ return m_Arc2DStartAngleAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getArcClose2DEndAngleAdjustment () const
	{ return m_ArcClose2DEndAngleAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getArcClose2DMinAngleAdjustment () const
	{ return m_ArcClose2DMinAngleAdjustment; }

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
	getConeBottomRadiusAdjustment () const
	{ return m_ConeBottomRadiusAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getConeHeightAdjustment () const
	{ return m_ConeHeightAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getConeUDimensionAdjustment () const
	{ return m_ConeUDimensionAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getCreaseAngleAdjustment () const
	{ return m_CreaseAngleAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getCylinderHeightAdjustment () const
	{ return m_CylinderHeightAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getCylinderRadiusAdjustment () const
	{ return m_CylinderRadiusAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getCylinderUDimensionAdjustment () const
	{ return m_CylinderUDimensionAdjustment; }

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
	getNormalColorAdjustment () const
	{ return m_NormalColorAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getNormalLengthAdjustment () const
	{ return m_NormalLengthAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getRectangle2DSizeXAdjustment () const
	{ return m_Rectangle2DSizeXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getRectangle2DSizeYAdjustment () const
	{ return m_Rectangle2DSizeYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSphereRadiusAdjustment () const
	{ return m_SphereRadiusAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSphereUDimensionAdjustment () const
	{ return m_SphereUDimensionAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSphereVDimensionAdjustment () const
	{ return m_SphereVDimensionAdjustment; }

	Gtk::Window &
	getWindow () const
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const
	{ return *m_Widget; }

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

	Gtk::Expander &
	getArc2DExpander () const
	{ return *m_Arc2DExpander; }

	Gtk::SpinButton &
	getArc2DStartAngleSpinButton () const
	{ return *m_Arc2DStartAngleSpinButton; }

	Gtk::SpinButton &
	getArc2DEndAngleSpinButton () const
	{ return *m_Arc2DEndAngleSpinButton; }

	Gtk::SpinButton &
	getArc2DRadiusSpinButton () const
	{ return *m_Arc2DRadiusSpinButton; }

	Gtk::SpinButton &
	getArc2DMinAngleSpinButton () const
	{ return *m_Arc2DMinAngleSpinButton; }

	Gtk::Expander &
	getArcClose2DExpander () const
	{ return *m_ArcClose2DExpander; }

	Gtk::SpinButton &
	getArcClose2DRadiusSpinButton () const
	{ return *m_ArcClose2DRadiusSpinButton; }

	Gtk::SpinButton &
	getArcClose2DEndAngleSpinButton () const
	{ return *m_ArcClose2DEndAngleSpinButton; }

	Gtk::SpinButton &
	getArcClose2DStartAngleSpinButton () const
	{ return *m_ArcClose2DStartAngleSpinButton; }

	Gtk::ComboBoxText &
	getArcClose2DClosureTypeComboBoxText () const
	{ return *m_ArcClose2DClosureTypeComboBoxText; }

	Gtk::SpinButton &
	getArcClose2DMinAngleSpinButton () const
	{ return *m_ArcClose2DMinAngleSpinButton; }

	Gtk::Expander &
	getCircle2DExpander () const
	{ return *m_Circle2DExpander; }

	Gtk::SpinButton &
	getCircle2DRadiusSpinButton () const
	{ return *m_Circle2DRadiusSpinButton; }

	Gtk::SpinButton &
	getCircle2DDimensionSpinButton () const
	{ return *m_Circle2DDimensionSpinButton; }

	Gtk::Expander &
	getDisk2DExpander () const
	{ return *m_Disk2DExpander; }

	Gtk::SpinButton &
	getDisk2DInnerRadiusSpinButton () const
	{ return *m_Disk2DInnerRadiusSpinButton; }

	Gtk::SpinButton &
	getDisk2DOuterRadiusSpinButton () const
	{ return *m_Disk2DOuterRadiusSpinButton; }

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
	getBoSizeYSpinButton () const
	{ return *m_BoSizeYSpinButton; }

	Gtk::SpinButton &
	getBoxSizeZSpinButton () const
	{ return *m_BoxSizeZSpinButton; }

	Gtk::ToggleButton &
	getBoxUniformSizeButton () const
	{ return *m_BoxUniformSizeButton; }

	Gtk::Image &
	getBoxUniformSizeImage () const
	{ return *m_BoxUniformSizeImage; }

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

	Gtk::SpinButton &
	getConeUDimensionSpinButton () const
	{ return *m_ConeUDimensionSpinButton; }

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

	Gtk::SpinButton &
	getCylinderUDimensionSpinButton () const
	{ return *m_CylinderUDimensionSpinButton; }

	Gtk::Expander &
	getExtrusionExpander () const
	{ return *m_ExtrusionExpander; }

	Gtk::CheckButton &
	getExtrusionBeginCapCheckButton () const
	{ return *m_ExtrusionBeginCapCheckButton; }

	Gtk::CheckButton &
	getExtrusionEndCapCheckButton () const
	{ return *m_ExtrusionEndCapCheckButton; }

	Gtk::Expander &
	getSphereExpander () const
	{ return *m_SphereExpander; }

	Gtk::SpinButton &
	getSphereRadiusSpinButton () const
	{ return *m_SphereRadiusSpinButton; }

	Gtk::SpinButton &
	getSphereUDimensionSpinButton () const
	{ return *m_SphereUDimensionSpinButton; }

	Gtk::SpinButton &
	getSphereVDimensionSpinButton () const
	{ return *m_SphereVDimensionSpinButton; }

	Gtk::Expander &
	getGeometryExpander () const
	{ return *m_GeometryExpander; }

	Gtk::CheckButton &
	getSolidCheckButton () const
	{ return *m_SolidCheckButton; }

	Gtk::CheckButton &
	getCCWCheckButton () const
	{ return *m_CCWCheckButton; }

	Gtk::CheckButton &
	getConvexCheckButton () const
	{ return *m_ConvexCheckButton; }

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
	getNormalsBox () const
	{ return *m_NormalsBox; }

	Gtk::Button &
	getAddNormalsButton () const
	{ return *m_AddNormalsButton; }

	Gtk::Button &
	getRemoveNormalsButton () const
	{ return *m_RemoveNormalsButton; }

	Gtk::CheckButton &
	getColorPerVertexCheckButton () const
	{ return *m_ColorPerVertexCheckButton; }

	Gtk::CheckButton &
	getNormalPerVertexCheckButton () const
	{ return *m_NormalPerVertexCheckButton; }

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

	///  @name Signal handlers

	virtual
	void
	on_geometry_changed () = 0;

	virtual
	void
	on_geometry_unlink_clicked () = 0;

	virtual
	void
	on_rectangle2d_uniform_size_clicked () = 0;

	virtual
	void
	on_box_uniform_size_clicked () = 0;

	virtual
	void
	on_add_normals_clicked () = 0;

	virtual
	void
	on_remove_normals_clicked () = 0;

	virtual
	bool
	on_crossing_notify_event (GdkEventCrossing* event) = 0;

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
	~X3DGeometryPropertiesEditorInterface ();


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
	Glib::RefPtr <Gtk::Adjustment> m_Arc2DEndAngleAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_Arc2DMinAngleAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_Arc2DRadiusAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_Arc2DStartAngleAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_ArcClose2DEndAngleAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_ArcClose2DMinAngleAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_ArcClose2DRadiusAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_ArcClose2DStartAngleAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_BoxSizeXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_BoxSizeYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_BoxSizeZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_Circle2DDimensionAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_Circle2DRadiusAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_ConeBottomRadiusAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_ConeHeightAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_ConeUDimensionAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_CreaseAngleAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_CylinderHeightAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_CylinderRadiusAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_CylinderUDimensionAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_Disk2DDimensionAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_Disk2DInnerRadiusAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_Disk2DOuterRadiusAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_EdgeColorAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_NormalColorAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_NormalLengthAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_Rectangle2DSizeXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_Rectangle2DSizeYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_SphereRadiusAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_SphereUDimensionAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_SphereVDimensionAdjustment;
	Gtk::Window*                   m_Window;
	Gtk::Box*                      m_Widget;
	Gtk::Notebook*                 m_GeometryChildNotebook;
	Gtk::Box*                      m_SelectGeometryBox;
	Gtk::ComboBoxText*             m_GeometryComboBoxText;
	Gtk::Button*                   m_GeometryUnlinkButton;
	Gtk::Expander*                 m_Arc2DExpander;
	Gtk::SpinButton*               m_Arc2DStartAngleSpinButton;
	Gtk::SpinButton*               m_Arc2DEndAngleSpinButton;
	Gtk::SpinButton*               m_Arc2DRadiusSpinButton;
	Gtk::SpinButton*               m_Arc2DMinAngleSpinButton;
	Gtk::Expander*                 m_ArcClose2DExpander;
	Gtk::SpinButton*               m_ArcClose2DRadiusSpinButton;
	Gtk::SpinButton*               m_ArcClose2DEndAngleSpinButton;
	Gtk::SpinButton*               m_ArcClose2DStartAngleSpinButton;
	Gtk::ComboBoxText*             m_ArcClose2DClosureTypeComboBoxText;
	Gtk::SpinButton*               m_ArcClose2DMinAngleSpinButton;
	Gtk::Expander*                 m_Circle2DExpander;
	Gtk::SpinButton*               m_Circle2DRadiusSpinButton;
	Gtk::SpinButton*               m_Circle2DDimensionSpinButton;
	Gtk::Expander*                 m_Disk2DExpander;
	Gtk::SpinButton*               m_Disk2DInnerRadiusSpinButton;
	Gtk::SpinButton*               m_Disk2DOuterRadiusSpinButton;
	Gtk::SpinButton*               m_Disk2DDimensionSpinButton;
	Gtk::Expander*                 m_Rectangle2DExpander;
	Gtk::Box*                      m_Rectangle2DSizeBox;
	Gtk::SpinButton*               m_Rectangle2DSizeXSpinButton;
	Gtk::SpinButton*               m_Rectangle2DSizeYSpinButton;
	Gtk::ToggleButton*             m_Rectangle2DUniformSizeButton;
	Gtk::Image*                    m_Rectangle2DUniformSizeImage;
	Gtk::Expander*                 m_BoxExpander;
	Gtk::Box*                      m_BoxSizeBox;
	Gtk::SpinButton*               m_BoxSizeXSpinButton;
	Gtk::SpinButton*               m_BoSizeYSpinButton;
	Gtk::SpinButton*               m_BoxSizeZSpinButton;
	Gtk::ToggleButton*             m_BoxUniformSizeButton;
	Gtk::Image*                    m_BoxUniformSizeImage;
	Gtk::Expander*                 m_ConeExpander;
	Gtk::SpinButton*               m_ConeBottomRadiusSpinButton;
	Gtk::SpinButton*               m_ConeHeightSpinButton;
	Gtk::CheckButton*              m_ConeSideCheckButton;
	Gtk::CheckButton*              m_ConeBottomCheckButton;
	Gtk::SpinButton*               m_ConeUDimensionSpinButton;
	Gtk::Expander*                 m_CylinderExpander;
	Gtk::SpinButton*               m_CylinderRadiusSpinButton;
	Gtk::SpinButton*               m_CylinderHeightSpinButton;
	Gtk::CheckButton*              m_CylinderBottomCheckButton;
	Gtk::CheckButton*              m_CylinderSideCheckButton;
	Gtk::CheckButton*              m_CylinderTopCheckButton;
	Gtk::SpinButton*               m_CylinderUDimensionSpinButton;
	Gtk::Expander*                 m_ExtrusionExpander;
	Gtk::CheckButton*              m_ExtrusionBeginCapCheckButton;
	Gtk::CheckButton*              m_ExtrusionEndCapCheckButton;
	Gtk::Expander*                 m_SphereExpander;
	Gtk::SpinButton*               m_SphereRadiusSpinButton;
	Gtk::SpinButton*               m_SphereUDimensionSpinButton;
	Gtk::SpinButton*               m_SphereVDimensionSpinButton;
	Gtk::Expander*                 m_GeometryExpander;
	Gtk::CheckButton*              m_SolidCheckButton;
	Gtk::CheckButton*              m_CCWCheckButton;
	Gtk::CheckButton*              m_ConvexCheckButton;
	Gtk::Box*                      m_CreaseAngleBox;
	Gtk::SpinButton*               m_CreaseAngleSpinButton;
	Gtk::Scale*                    m_CreaseAngleScale;
	Gtk::Box*                      m_NormalsBox;
	Gtk::Button*                   m_AddNormalsButton;
	Gtk::Button*                   m_RemoveNormalsButton;
	Gtk::CheckButton*              m_ColorPerVertexCheckButton;
	Gtk::CheckButton*              m_NormalPerVertexCheckButton;
	Gtk::EventBox*                 m_PrimitiveCountEventBox;
	Gtk::Expander*                 m_PrimitiveCountExpander;
	Gtk::Box*                      m_PrimitiveCountBox;
	Gtk::Label*                    m_PrimitiveCountNodesLabel;
	Gtk::Label*                    m_PrimitiveCountOpaqueShapesLabel;
	Gtk::Label*                    m_PrimitiveCountTransparentShapesLabel;
	Gtk::Label*                    m_PrimitiveCountPointsLabel;
	Gtk::Label*                    m_PrimitiveCountLinesLabel;
	Gtk::Label*                    m_PrimitiveCountTrianglesLabel;
	Gtk::Label*                    m_PrimitiveCountQuadsLabel;
	Gtk::Label*                    m_PrimitiveCountPolygonsLabel;
	Gtk::ComboBoxText*             m_PrimitiveCountCountButton;
	Gtk::Expander*                 m_GeometryToolExpander;
	Gtk::SpinButton*               m_NormalLengthSpinButton;
	Gtk::Box*                      m_NormalColorBox;
	Gtk::Button*                   m_NormalColorButton;
	Gtk::Scale*                    m_NormalColorScale;
	Gtk::Box*                      m_EdgeColorBox;
	Gtk::Button*                   m_EdgeColorButton;
	Gtk::Scale*                    m_EdgeColorScale;

};

} // puck
} // titania

#endif
