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

#ifndef __TITANIA_EDITORS_TEXTURE_COORDINATE_EDITOR_TEXTURE_COORDINATE_EDITOR_H__
#define __TITANIA_EDITORS_TEXTURE_COORDINATE_EDITOR_TEXTURE_COORDINATE_EDITOR_H__

#include "../../UserInterfaces/X3DTextureCoordinateEditorInterface.h"

#include "../../Undo/UndoHistory.h"

namespace titania {
namespace puck {

class FaceSelection;

class TextureCoordinateEditor :
	public X3DTextureCoordinateEditorInterface
{
public:

	///  @name Construction

	TextureCoordinateEditor (X3DBrowserWindow* const);

	///  @name Destruction

	virtual
	~TextureCoordinateEditor ();


private:

	///  @name Construction

	virtual
	void
	initialize () final override;

	void
	set_left_initialized ();

	void
	set_right_initialized ();

	void
	set_initialized ();

	void
	configure ();

	void
	set_selection ();

	///  @name Menubar

	virtual
	void
	on_undo_activate () final override;

	virtual
	void
	on_redo_activate () final override;

	virtual
	void
	on_left_hand_toggled () final override;

	virtual
	void
	on_left_arrow_toggled () final override;

	virtual
	void
	on_left_look_at_all_clicked () final override;

	virtual
	void
	on_right_hand_toggled () final override;

	virtual
	void
	on_right_arrow_toggled () final override;

	virtual
	void
	on_right_look_at_all_clicked () final override;

	virtual
	void
	on_right_look_at_toggled () final override;

	void
	set_undoHistory ();

	///  @name Apply

	virtual
	void
	on_remove_clicked () final override;

	virtual
	void
	on_apply_clicked () final override;

	///  @name Event handlers

	void
	set_left_viewer ();

	void
	set_right_viewer ();

	void
	set_shape (const X3D::X3DPtr <X3D::X3DShapeNode> &);

	void
	set_appearance (const X3D::SFNode &);

	void
	set_material (const X3D::SFNode &);

	void
	set_texture (const X3D::SFNode &);

	void
	set_texture_stage (const X3D::SFNode &);

	void
	set_textureTransform (const X3D::SFNode &);

	void
	set_geometry (const X3D::SFNode &);
	
	void
	set_coordIndex ();

	void
	set_texCoord (const X3D::SFNode &);

	virtual
	void
	on_texture_stage_changed () final override;
	
	void
	set_right_hitPoint (const X3D::Vector3f &);
	
	void
	set_right_selection (const X3D::Vector3f &);

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

	///  @name Members

	X3D::BrowserPtr                                      left;
	X3D::BrowserPtr                                      right;
	int                                                  initialized;
	X3D::X3DPtr <X3D::X3DShapeNode>                      shape;
	X3D::X3DPtr <X3D::Appearance>                        appearance;
	X3D::SFNode                                          material;
	X3D::SFNode                                          texture;
	X3D::SFNode                                          textureTransform;
	X3D::X3DPtr <X3D::IndexedFaceSet>                    geometry;
	X3D::X3DPtr <X3D::X3DCoordinateNode>                 coord;
	X3D::X3DPtr <X3D::IndexedFaceSet>                    previewGeometry; 
	X3D::X3DPtr <X3D::X3DTextureCoordinateNode>          texCoord; 
	size_t                                               stage;
	std::unique_ptr <FaceSelection>                      rightSelection;             
	UndoHistory                                          undoHistory;

};

} // puck
} // titania

#endif
