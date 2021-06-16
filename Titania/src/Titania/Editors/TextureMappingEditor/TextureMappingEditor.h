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

#ifndef __TITANIA_EDITORS_TEXTURE_MAPPING_EDITOR_TEXTURE_MAPPING_EDITOR_H__
#define __TITANIA_EDITORS_TEXTURE_MAPPING_EDITOR_TEXTURE_MAPPING_EDITOR_H__

#include "../../UserInterfaces/X3DTextureMappingEditorInterface.h"

#include <Titania/X3D/Components/Geometry3D/IndexedFaceSet.h>
#include <Titania/X3D/Components/Rendering/X3DCoordinateNode.h>
#include <Titania/X3D/Components/Shape/Appearance.h>
#include <Titania/X3D/Components/Shape/X3DShapeNode.h>
#include <Titania/X3D/Components/Texturing/MultiTextureCoordinate.h>
#include <Titania/X3D/Components/Texturing/TextureCoordinate.h>
#include <Titania/X3D/Editing/Undo/UndoHistory.h>

namespace titania {
namespace X3D {

class FaceSelection;

} // X3D
} // titania

namespace titania {
namespace puck {

class TextureMappingEditor :
	public X3DTextureMappingEditorInterface
{
public:

	///  @name Construction

	TextureMappingEditor (X3DBrowserWindow* const browserWindow);

	///  @name Destruction

	virtual
	~TextureMappingEditor () final override;


private:

	///  @name Member types

	enum class BrowserType
	{
		LEFT,
		RIGHT
	};

	enum class ToolType
	{
		MOVE,
		ROTATE,
		SCALE
	};

	///  @name Construction

	virtual
	void
	initialize () final override;

	virtual
	void
	configure () final override;

	void
	set_left_initialized ();

	void
	set_right_initialized ();

	void
	set_initialized ();

	virtual
	void
	set_selection (const X3D::MFNode & selection) final override;

	///  @name Focus handling

	bool
	on_focus_in_event (GdkEventFocus* event);

	///  @name Keyboard

	virtual
	bool
	on_key_press_event (GdkEventKey*) final override;

	virtual
	bool
	on_key_release_event (GdkEventKey*) final override;

	///  @name Edit

	virtual
	void
	on_undo () final override;

	virtual
	void
	on_redo () final override;

	void
	set_undoHistory ();

	std::vector <int32_t>
	on_remove_unused_texCoord_activate ();

	///  @name Mappings

	virtual
	void
	on_x_plane_activate () final override;

	virtual
	void
	on_y_plane_activate () final override;

	virtual
	void
	on_z_plane_activate () final override;

	void
	on_plane_activate (const size_t, const size_t, const bool);

	virtual
	void
	on_camera_activate () final override;

	virtual
	void
	on_box_activate () final override;

	virtual
	void
	on_cylinder_x_activate () final override;

	virtual
	void
	on_cylinder_y_activate () final override;

	virtual
	void
	on_cylinder_z_activate () final override;

	void
	on_cylinder_activate (const size_t, const size_t, const size_t, const bool, const size_t);

	virtual
	void
	on_sphere_x_activate () final override;

	virtual
	void
	on_sphere_y_activate () final override;

	virtual
	void
	on_sphere_z_activate () final override;

	void
	on_sphere_activate (const size_t, const size_t, const size_t, const bool, const size_t);

	void
	resolveOverlaps ();

	X3D::Box2f
	getTexBBox () const;

	X3D::Box2f
	getBBox (const size_t, const size_t) const;

	///  @name Selection

	virtual
	void
	on_select_all_activate () final override;

	virtual
	void
	on_deselect_all_activate () final override;

	///  @name Toolbar

	virtual
	void
	on_rotate_counterclockwise () final override;

	virtual
	void
	on_rotate_clockwise () final override;

	void
	on_rotate (const std::string &, const float);

	virtual
	void
	on_flip () final override;

	virtual
	void
	on_flop () final override;

	virtual
	void
	on_merge_points () final override;

	virtual
	void
	on_split_point () final override;

	///  @name Dashboard

	virtual
	void
	on_left_hand_toggled () final override;

	virtual
	void
	on_left_arrow_toggled () final override;

	virtual
	void
	on_left_rotate_toggled () final override;

	virtual
	void
	on_left_scale_toggled () final override;

	virtual
	void
	on_left_snap_center_toggled () final override;

	virtual
	void
	on_left_look_at_all_clicked () final override;

	virtual
	void
	on_right_hand_toggled () final override;

	virtual
	void
	on_right_arrow_toggled () final override;

	///  @name Navigation

	virtual
	void
	on_right_straighten_horizon_toggled () final override;

	///  @name Look At

	virtual
	void
	on_right_look_at_all_clicked () final override;

	virtual
	void
	on_right_look_at_toggled () final override;

	///  @name Apply

	virtual
	void
	on_remove_clicked () final override;

	virtual
	void
	on_apply_clicked () final override;

	void
	connectTexCoordIndex ();

	void
	connectTexCoord ();

	void
	connectMultiTexCoord ();

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
	set_texCoordIndex ();

	void
	set_coordIndex ();

	void
	set_coord (const X3D::SFNode &);

	void
	set_texCoord (const X3D::SFNode &);

	virtual
	void
	on_texture_stage_changed () final override;

	void
	clear ();

	void
	set_selected_faces ();

	void
	set_left_coord ();

	void
	set_left_image ();

	void
	set_left_selected_faces ();

	void
	set_left_active (const bool);

	void
	set_startDrag ();

	void
	set_left_touchTime ();

	void
	set_left_hitPoint (const X3D::Vector3f &);

	void
	move (const X3D::Vector3f &);

	void
	rotate (const X3D::Vector3f &);

	void
	scale (const X3D::Vector3f &);

	void
	set_left_point (const X3D::Vector3d &);

	int32_t
	getNearestPoint (const X3D::Vector3d &) const;

	void
	set_selectedPoints ();

	void
	set_left_center (const X3D::Vector3f &);

	void
	set_right_selection (const X3D::Vector3d &, const std::vector <int32_t> &);

	void
	set_right_active (const bool);

	void
	set_right_touchTime ();

	void
	set_right_selected_faces ();

	void
	set_right_hitPoint ();

	///  throws std::runtime_error
	X3D::Vector2d
	projectPoint (const X3D::Vector3d &, const X3D::BrowserPtr &) const;

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

	X3D::BrowserPtr                                  left;
	X3D::BrowserPtr                                  right;
	X3D::X3DPtr <X3D::X3DShapeNode>                  shape;
	X3D::X3DPtr <X3D::Appearance>                    appearance;
	X3D::SFNode                                      material;
	X3D::SFNode                                      texture;
	X3D::SFNode                                      textureTransform;
	X3D::X3DPtr <X3D::IndexedFaceSet>                geometry;
	X3D::X3DPtr <X3D::MultiTextureCoordinate>        multiTexCoord;
	X3D::X3DPtr <X3D::X3DCoordinateNode>             coord;
	X3D::X3DPtr <X3D::IndexedFaceSet>                previewGeometry;
	X3D::X3DPtr <X3D::TextureCoordinate>             texCoord;
	int                                              initialized;
	BrowserType                                      focus;
	size_t                                           stage;
	ToolType                                         tool;
	X3D::X3DPtr <X3D::FaceSelection>                 rightSelection;
	bool                                             rightPaintSelecion;
	std::set <size_t>                                selectedFaces;
	int32_t                                          activePoint;               // texCoord index of red point
	int32_t                                          masterPoint;               // texCoord index of red point
	std::set <int32_t>                               selectedPoints; // texCoord indices of blue points
	X3D::Vector2d                                    startHitPoint;
	X3D::Vector2f                                    pointOffset;
	X3D::Vector2f                                    startPosition;
	std::vector <std::pair <int32_t, X3D::Vector2f>> startPositions;
	X3D::Vector2f                                    startDistance;
	X3D::Keys                                        keys;
	X3D::UndoHistory                                 undoHistory;
	X3D::UndoStepPtr                                 undoStep;

};

} // puck
} // titania

#endif
