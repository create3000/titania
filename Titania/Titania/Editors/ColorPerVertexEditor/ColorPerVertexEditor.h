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

#ifndef __TITANIA_EDITORS_COLOR_PER_VERTEX_EDITOR_COLOR_PER_VERTEX_EDITOR_H__
#define __TITANIA_EDITORS_COLOR_PER_VERTEX_EDITOR_COLOR_PER_VERTEX_EDITOR_H__

#include "../../UserInterfaces/X3DColorPerVertexEditorInterface.h"

#include "../../ComposedWidgets/MFColorRGBAButton.h"
#include "../../Undo/UndoHistory.h"

namespace titania {
namespace puck {

class BrowserWindow;

class ColorPerVertexEditor :
	public X3DColorPerVertexEditorInterface
{
public:

	///  @name Construction

	ColorPerVertexEditor (X3DBrowserWindow* const);

	///  @name Destruction

	virtual
	~ColorPerVertexEditor ();


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

	void
	set_initialized ();

	void
	set_selection ();

	///  @name Menubar

	virtual
	void
	on_undo_activate () final override;

	virtual
	void
	on_redo_activate () final override;

	void
	set_undoHistory ();

	virtual
	void
	on_remove_unused_colors_activate () final override;

	/// @name Shading

	virtual
	void
	on_checkerboard_toggled () final override;

	virtual
	void
	on_phong_activate () final override;

	virtual
	void
	on_gouraud_activate () final override;

	virtual
	void
	on_flat_activate () final override;

	virtual
	void
	on_wireframe_activate () final override;

	virtual
	void
	on_pointset_activate () final override;

	void
	on_shading_activate (const std::string &);

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

	///  @name Look At

	virtual
	void
	on_look_at_all_clicked () final override;

	///  @name Apply

	virtual
	void
	on_remove_clicked () final override;

	virtual
	void
	on_apply_clicked () final override;

	///  @name Preview

	void
	set_hitPoint (const X3D::Vector3f &);

	void
	set_touchTime ();

	void
	set_triangle (const X3D::Vector3f &);

	///  @name Operations

	virtual
	void
	addUndoStep (const UndoStepPtr & undoStep) final override
	{ undoHistory .addUndoStep (undoStep); }

	virtual
	void
	removeUndoStep () final override
	{ undoHistory .removeUndoStep (); }

	virtual
	const std::shared_ptr <UndoStep> &
	getUndoStep () const final override
	{ return undoHistory .getUndoStep (); }

	void
	setColor ();

	void
	setFaceIndex ();

	std::vector <size_t>
	getPointIndices (const X3D::Vector3f &, const X3D::MFVec3f &) const;

	void
	setFaces (const X3D::Vector3d &, const std::vector <size_t> &);

	std::vector <size_t>
	getPoints (const size_t) const;

	///  @name Members

	X3D::BrowserPtr                                      preview;
	MFColorRGBAButton                                    colorButton;
	ModeType                                             mode;
	X3D::X3DPtr <X3D::IndexedFaceSet>                    selection;
	X3D::X3DPtr <X3D::X3DCoordinateNode>                 coord;
	X3D::X3DPtr <X3D::IndexedFaceSet>                    indexedFaceSet;
	X3D::X3DPtr <X3D::ColorRGBA>                         color;
	std::multimap <int32_t, std::pair <size_t, size_t>>  faceIndex;
	std::pair <size_t, size_t>                           face;
	std::vector <std::pair <size_t, size_t>>             faces;
	UndoHistory                                          undoHistory;

};

} // puck
} // titania

#endif
