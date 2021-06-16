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

#ifndef __TITANIA_REVEALER_GEOMETRY_EDITOR_GEOMETRY_EDITOR_H__
#define __TITANIA_REVEALER_GEOMETRY_EDITOR_GEOMETRY_EDITOR_H__

#include "../../ComposedWidgets.h"
#include "../../UserInterfaces/X3DGeometryEditorInterface.h"

namespace titania {
namespace puck {

class GeometryEditor :
	virtual public X3DGeometryEditorInterface
{
public:

	///  @name Construction

	GeometryEditor (X3DBrowserWindow* const browserWindow);

	///  @name Member access

	const X3D::SFNode &
	getNormalEditor () const
	{ return normalEditor; }

	const X3D::SFNode &
	getCoordinateEditor () const
	{ return coordEditor; }

	bool
	on_cut ();

	bool
	on_copy ();

	bool
	on_paste ();

	bool
	on_delete ();

	bool
	on_select_all ();

	bool
	on_deselect_all ();

	///  @name Destruction

	virtual
	~GeometryEditor () final override;


private:

	enum class SelectorType
	{
		BRUSH,
		RECTANGLE,
		LASSO
	};

	///  @name Construction

	virtual
	void
	initialize () final override;

	virtual
	void
	configure () final override;

	virtual
	void
	on_map () final override;

	virtual
	void
	on_unmap () final override;

	void
	set_browser (const X3D::BrowserPtr & browser);

	virtual
	void
	set_selection (const X3D::MFNode & selection) final override;

	void
	set_geometries (const X3D::MFNode & geometryNodes);

	void
	set_viewer ();

	void
	connectViewer ();

	///  @name Geometry handling

	void
	set_clipboard (const X3D::SFString & string);

	void
	set_selectedPoints ();

	void
	set_selectedEdges ();

	void
	set_selectedFaces ();

	void
	set_selectedHoles ();

	///  @name Geometry handling

	void
	on_hand_toggled ();

	void
	on_arrow_toggled ();

	virtual
	void
	on_hammer_clicked () final override;

	void
	set_selectGeometry ();

	virtual
	void
	on_edit_toggled () final override;

	virtual
	void
	on_paint_selection_toggled () final override;

	virtual
	bool
	on_selection_type_button_press_event (GdkEventButton* event) final override;

	virtual
	void
	on_brush_activated () final override;

	virtual
	void
	on_rectangle_activated () final override;

	virtual
	void
	on_lasso_activated () final override;

	void
	set_selector (const SelectorType);

	virtual
	void
	on_points_toggled () final override;

	virtual
	void
	on_edges_toggled () final override;

	virtual
	void
	on_faces_toggled () final override;

	virtual
	void
	on_select_line_loop_toggled () final override;

	virtual
	void
	on_transform_tool_toggled () final override;

	virtual
	bool
	on_transform_tool_button_press_event (GdkEventButton*) final override;

	virtual
	void
	on_axis_aligned_bounding_box_toggled () final override;

	virtual
	void
	on_merge_points_clicked () final override;

	virtual
	void
	on_split_points_clicked () final override;

	virtual
	void
	on_cut_polygons_toggled () final override;

	void
	set_cut_polygons ();

	virtual
	bool
	on_cut_polygons_button_press_event (GdkEventButton* event) final override;

	virtual
	void
	on_cut_polygons_enable_snapping_toggled () final override;

	virtual
	void
	on_form_new_face_clicked () final override;

	virtual
	void
	on_extrude_selected_edges_clicked () final override;

	virtual
	void
	on_extrude_selected_faces_clicked () final override;

	virtual
	void
	on_chip_of_face_clicked () final override;

	virtual
	void
	on_flip_vertex_ordering_clicked () final override;

	virtual
	void
	on_delete_selected_faces_clicked () final override;

	virtual
	void
	store () final override;

	///  @name Members

	X3DFieldToggleButton <X3D::SFBool> normalEnabled;

	X3D::BrowserPtr browser;
	X3D::SFNode     normalEditor;
	X3D::SFNode     coordEditor;

	X3D::NodeType    privateViewer;
	SelectorType     selector;
	size_t           numSelectedPoints;
	size_t           numSelectedEdges;
	size_t           numSelectedHoles;
	size_t           numSelectedFaces;
	X3D::time_type   copyTime;
	bool             changing;

};

} // puck
} // titania

#endif
