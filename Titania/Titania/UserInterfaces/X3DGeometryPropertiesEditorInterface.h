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

class X3DGeometryPropertiesEditorInterface :
	public X3DEditorInterface
{
public:

	X3DGeometryPropertiesEditorInterface () :
		X3DEditorInterface ()
	{ }

	template <class ... Arguments>
	X3DGeometryPropertiesEditorInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DEditorInterface (m_widgetName, arguments ...),
		          filename (filename)
	{ create (filename); }

	const Glib::RefPtr <Gtk::Builder> &
	getBuilder () const { return m_builder; }

	const std::string &
	getWidgetName () const { return m_widgetName; }

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
	getArc2DRadiusAdjustment () const
	{ return m_Arc2DRadiusAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getArc2DStartAngleAdjustment () const
	{ return m_Arc2DStartAngleAdjustment; }

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
	getCircle2DRadiusAdjustment () const
	{ return m_Circle2DRadiusAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getCreaseAngleAdjustment () const
	{ return m_CreaseAngleAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getDisk2DInnerRadiusAdjustment () const
	{ return m_Disk2DInnerRadiusAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getDisk2DOuterRadiusAdjustment () const
	{ return m_Disk2DOuterRadiusAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getRectangle2DSizeXAdjustment () const
	{ return m_Rectangle2DSizeXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getRectangle2DSizeYAdjustment () const
	{ return m_Rectangle2DSizeYAdjustment; }

	Gtk::Window &
	getWindow () const
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const
	{ return *m_Widget; }

	Gtk::Notebook &
	getGeometryChildNotebook () const
	{ return *m_GeometryChildNotebook; }

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

	Gtk::Expander &
	getCircle2DExpander () const
	{ return *m_Circle2DExpander; }

	Gtk::SpinButton &
	getCircle2DRadiusSpinButton () const
	{ return *m_Circle2DRadiusSpinButton; }

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
	getCreaseAngleScaleSpinButton () const
	{ return *m_CreaseAngleScaleSpinButton; }

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
	getPrimitiveCountPointsLabel () const
	{ return *m_PrimitiveCountPointsLabel; }

	Gtk::Label &
	getPrimitiveCountLinesLabel () const
	{ return *m_PrimitiveCountLinesLabel; }

	Gtk::Label &
	getPrimitiveCountTrianglesLabel () const
	{ return *m_PrimitiveCountTrianglesLabel; }

	Gtk::ComboBoxText &
	getPrimitiveCountCountButton () const
	{ return *m_PrimitiveCountCountButton; }

	virtual
	void
	on_rectangle2d_uniform_size_clicked () = 0;

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

	virtual
	~X3DGeometryPropertiesEditorInterface ();


private:

	void
	create (const std::string &);

	static const std::string m_widgetName;

	std::string                    filename;
	Glib::RefPtr <Gtk::Builder>    m_builder;
	Glib::RefPtr <Gtk::Adjustment> m_Arc2DEndAngleAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_Arc2DRadiusAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_Arc2DStartAngleAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_ArcClose2DEndAngleAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_ArcClose2DRadiusAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_ArcClose2DStartAngleAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_Circle2DRadiusAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_CreaseAngleAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_Disk2DInnerRadiusAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_Disk2DOuterRadiusAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_Rectangle2DSizeXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_Rectangle2DSizeYAdjustment;
	Gtk::Window*                   m_Window;
	Gtk::Box*                      m_Widget;
	Gtk::Notebook*                 m_GeometryChildNotebook;
	Gtk::Expander*                 m_Arc2DExpander;
	Gtk::SpinButton*               m_Arc2DStartAngleSpinButton;
	Gtk::SpinButton*               m_Arc2DEndAngleSpinButton;
	Gtk::SpinButton*               m_Arc2DRadiusSpinButton;
	Gtk::Expander*                 m_ArcClose2DExpander;
	Gtk::SpinButton*               m_ArcClose2DRadiusSpinButton;
	Gtk::SpinButton*               m_ArcClose2DEndAngleSpinButton;
	Gtk::SpinButton*               m_ArcClose2DStartAngleSpinButton;
	Gtk::ComboBoxText*             m_ArcClose2DClosureTypeComboBoxText;
	Gtk::Expander*                 m_Circle2DExpander;
	Gtk::SpinButton*               m_Circle2DRadiusSpinButton;
	Gtk::Expander*                 m_Disk2DExpander;
	Gtk::SpinButton*               m_Disk2DInnerRadiusSpinButton;
	Gtk::SpinButton*               m_Disk2DOuterRadiusSpinButton;
	Gtk::Expander*                 m_Rectangle2DExpander;
	Gtk::Box*                      m_Rectangle2DSizeBox;
	Gtk::SpinButton*               m_Rectangle2DSizeXSpinButton;
	Gtk::SpinButton*               m_Rectangle2DSizeYSpinButton;
	Gtk::ToggleButton*             m_Rectangle2DUniformSizeButton;
	Gtk::Image*                    m_Rectangle2DUniformSizeImage;
	Gtk::Expander*                 m_GeometryExpander;
	Gtk::CheckButton*              m_SolidCheckButton;
	Gtk::CheckButton*              m_CCWCheckButton;
	Gtk::CheckButton*              m_ConvexCheckButton;
	Gtk::Box*                      m_CreaseAngleBox;
	Gtk::SpinButton*               m_CreaseAngleScaleSpinButton;
	Gtk::Scale*                    m_CreaseAngleScale;
	Gtk::Box*                      m_NormalsBox;
	Gtk::Button*                   m_AddNormalsButton;
	Gtk::Button*                   m_RemoveNormalsButton;
	Gtk::EventBox*                 m_PrimitiveCountEventBox;
	Gtk::Expander*                 m_PrimitiveCountExpander;
	Gtk::Box*                      m_PrimitiveCountBox;
	Gtk::Label*                    m_PrimitiveCountPointsLabel;
	Gtk::Label*                    m_PrimitiveCountLinesLabel;
	Gtk::Label*                    m_PrimitiveCountTrianglesLabel;
	Gtk::ComboBoxText*             m_PrimitiveCountCountButton;

};

} // puck
} // titania

#endif
