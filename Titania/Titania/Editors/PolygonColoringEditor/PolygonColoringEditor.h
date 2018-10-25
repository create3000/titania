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

#ifndef __TITANIA_EDITORS_COLOR_EDITOR_COLOR_EDITOR_H__
#define __TITANIA_EDITORS_COLOR_EDITOR_COLOR_EDITOR_H__

#include "../../ComposedWidgets/MFColorRGBAButton.h"
#include "../../UserInterfaces/X3DPolygonColoringEditorInterface.h"

#include <Titania/X3D/Components/Geometry3D/IndexedFaceSet.h>
#include <Titania/X3D/Components/Rendering/ColorRGBA.h>
#include <Titania/X3D/Editing/Selection/X3DFaceSelection.h>
#include <Titania/X3D/Editing/Undo/UndoHistory.h>

namespace titania {
namespace X3D {

class FaceSelection;

} // X3D
} // titania

namespace titania {
namespace puck {

class PolygonColoringEditor :
	public X3DPolygonColoringEditorInterface
{
public:

	///  @name Construction

	PolygonColoringEditor (X3DBrowserWindow* const browserWindow);

	///  @name Destruction

	virtual
	~PolygonColoringEditor () final override;


private:

	///  @name Member types

	enum ModeType
	{
		SINGLE_VERTEX,
		ADJACENT_VERTICES,
		SINGLE_FACE,
		WHOLE_OBJECT

	};

	///  @name Construction

	virtual
	void
	initialize () final override;

	virtual
	void
	configure () final override;

	void
	set_initialized ();

	virtual
	void
	set_selection (const X3D::MFNode & selection) final override;

	void
	disable ();

	///  @name Menubar

	virtual
	void
	on_undo () final override;

	virtual
	void
	on_redo () final override;

	void
	set_undoHistory ();

	virtual
	void
	on_remove_unused_colors_activate () final override;

	virtual
	void
	on_remove_dublicate_colors_activate () final override;

	/// @name Selection

	virtual
	void
	on_hand_toggled () final override;

	virtual
	void
	on_arrow_toggled () final override;

	/// @name Shading

	virtual
	void
	on_visualize_geometry_toggled () final override;

	virtual
	void
	on_shading_clicked () final override;

	virtual
	void
	on_phong_toggled () final override;

	virtual
	void
	on_gouraud_toggled () final override;

	virtual
	void
	on_flat_toggled () final override;

	virtual
	void
	on_wireframe_toggled () final override;

	virtual
	void
	on_pointset_toggled () final override;

	void
	on_shading_toggled (const std::string &);

	virtual
	void
	on_texture_toggled () final override;

	virtual
	void
	on_checkerboard_toggled () final override;

	virtual
	void
	on_single_vertex_clicked () final override;

	virtual
	void
	on_adjacent_vertices_clicked () final override;

	virtual
	void
	on_single_face_clicked () final override;

	virtual
	void
	on_whole_object_clicked () final override;

	///  @name Navigation

	virtual
	void
	on_straighten_horizon_toggled () final override;

	///  @name Look At

	virtual
	void
	on_look_at_all_clicked () final override;

	virtual
	void
	on_look_at_toggled () final override;

	///  @name Apply

	virtual
	void
	on_remove_clicked () final override;

	virtual
	void
	on_apply_clicked () final override;

	void
	connectColorIndex ();

	void
	connectColor ();

	///  @name Event handlers

	void
	set_viewer ();

	void
	set_shape (const X3D::X3DPtr <X3D::X3DShapeNode> &);

	void
	set_appearance (const X3D::SFNode &);

	void
	set_material (const X3D::SFNode & value);

	void
	set_texture (const X3D::SFNode &);

	void
	set_textureTransform (const X3D::SFNode &);

	void
	setTexture (const bool);

	void
	set_multi_texture ();

	void
	set_multi_textureTransform ();

	void
	set_geometry (const X3D::SFNode &);

	void
	set_colorIndex ();

	void
	set_coordIndex ();

	void
	set_coord (const X3D::SFNode &);

	void
	set_hitPoint ();

	void
	set_touchTime ();

	void
	set_triangle (const X3D::X3DFaceSelection::Face &);

	///  @name Operations

	virtual
	void
	addUndoStep (const X3D::UndoStepPtr & undoStep) final override
	{ undoHistory .addUndoStep (undoStep, X3D::SFTime::now ()); }

	virtual
	void
	removeUndoStep () final override
	{ undoHistory .removeUndoStep (); }

	virtual
	const X3D::UndoStepPtr &
	getUndoStep () const final override
	{ return undoHistory .getUndoStep (); }

	///  @name Members

	X3D::BrowserPtr                      preview;
	MFColorRGBAButton                    colorButton;
	ModeType                             mode;
	X3D::X3DPtr <X3D::X3DShapeNode>      shape;
	X3D::X3DPtr <X3D::Appearance>        appearance;
	X3D::SFNode                          texture;
	X3D::SFNode                          textureTransform;
	X3D::X3DPtr <X3D::IndexedFaceSet>    geometry;
	X3D::X3DPtr <X3D::X3DCoordinateNode> coord;
	X3D::X3DPtr <X3D::IndexedFaceSet>    previewGeometry;
	X3D::X3DPtr <X3D::ColorRGBA>         previewColor;
	X3D::X3DPtr <X3D::FaceSelection>     selection;
	X3D::UndoHistory                     undoHistory;

};

} // puck
} // titania

#endif
