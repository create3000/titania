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

#include "ColorPerVertexEditor.h"

#include "../FaceSelection.h"
#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"

namespace titania {
namespace puck {

ColorPerVertexEditor::ColorPerVertexEditor (X3DBrowserWindow* const browserWindow) :
	                X3DBaseInterface (browserWindow, browserWindow -> getBrowser ()),
	X3DColorPerVertexEditorInterface (get_ui ("Editors/ColorPerVertexEditor.xml"), gconf_dir ()),
	                         preview (X3D::createBrowser (getBrowserWindow () -> getBrowser ())),
	                     colorButton (this,
	                                  getColorButton (),
	                                  getColorAdjustment (),
	                                  getWidget (),
	                                  getAddColorButton (),
	                                  getRemoveColorButton (),
	                                  getColorsScrolledWindow (),
	                                  "color"),
	                            mode (SINGLE_VERTEX),
	                           shape (),
	                      appearance (),
	                         texture (),
	                textureTransform (),
	                        geometry (),
	                           coord (),
	                 previewGeometry (),
	                    previewColor (),
	                       selection (new FaceSelection ()),
	                     undoHistory ()
{
	preview -> set_antialiasing (4);

	getShadingButton () .set_menu (getShadingMenu ());

	setup ();
}

void
ColorPerVertexEditor::initialize ()
{
	X3DColorPerVertexEditorInterface::initialize ();

	getPreviewBox () .pack_start (*preview, true, true, 0);

	preview -> show ();
	preview -> initialized () .addInterest (this, &ColorPerVertexEditor::set_initialized);

	getBrowserWindow () -> getSelection () -> getChildren () .addInterest (this, &ColorPerVertexEditor::set_selection);

	undoHistory .addInterest (this, &ColorPerVertexEditor::set_undoHistory);
}

void
ColorPerVertexEditor::set_initialized ()
{
	preview -> initialized () .removeInterest (this, &ColorPerVertexEditor::set_initialized);

	try
	{
		preview -> loadURL ({ get_ui ("Editors/ColorPerVertexEditorPreview.x3dv") });

		const auto transform   = preview -> getExecutionContext () -> getNamedNode <X3D::Transform> ("Transform");
		const auto appearance  = preview -> getExecutionContext () -> getNamedNode <X3D::Appearance> ("Appearance");
		const auto touchSensor = preview -> getExecutionContext () -> getNamedNode <X3D::TouchSensor> ("TouchSensor");

		appearance -> isPrivate (true);

		transform -> addInterest (this, &ColorPerVertexEditor::set_viewer);
		touchSensor -> hitPoint_changed () .addInterest (this, &ColorPerVertexEditor::set_hitPoint);
		touchSensor -> touchTime ()        .addInterest (this, &ColorPerVertexEditor::set_touchTime);

		configure ();
		set_selection ();
	}
	catch (const X3D::X3DError &)
	{ }
}

void
ColorPerVertexEditor::configure ()
{
	getCheckerBoardButton () .set_active (getConfig () .getBoolean ("checkerBoard"));
	getTextureButton ()      .set_active (getConfig () .getBoolean ("texture"));

	switch (getConfig () .getInteger ("mode"))
	{
		case SINGLE_VERTEX:
		{
			getSingleVertexButton () .set_active (true);
			break;
		}
		case ADJACENT_VERTICES:
		{
			getAdjacentVerticesButton () .set_active (true);
			break;
		}
		case SINGLE_FACE:
		{
			getSingleFaceButton () .set_active (true);
			break;
		}
		case WHOLE_OBJECT:
		{
			getWholeObjectButton () .set_active (true);
			break;
		}
		default:
			break;
	}
}

void
ColorPerVertexEditor::set_selection ()
{
	undoHistory .clear ();

	try
	{
		const auto shapes = getSelection <X3D::X3DShapeNode> ({ X3D::X3DConstants::X3DShapeNode });

		if (shapes .empty ())
			set_shape (nullptr);

		else
			set_shape (shapes .back ());
	}
	catch (const X3D::X3DError &)
	{ }
}

// Menubar

void
ColorPerVertexEditor::on_undo ()
{
	preview -> grab_focus ();

	undoHistory .undoChanges ();
}

void
ColorPerVertexEditor::on_redo ()
{
	preview -> grab_focus ();

	undoHistory .redoChanges ();
}

void
ColorPerVertexEditor::set_undoHistory ()
{
	if (undoHistory .hasUndo ())
	{
		getUndoMenuItem () .set_label (undoHistory .getUndoDescription ());
		getUndoButton ()   .set_tooltip_text (undoHistory .getUndoDescription ());
		getUndoMenuItem () .set_sensitive (true);
		getUndoButton ()   .set_sensitive (true);
	}
	else
	{
		getUndoMenuItem () .set_label (_ ("Undo"));
		getUndoButton ()   .set_tooltip_text (_ ("Undo last action (Ctrl-Z)."));
		getUndoMenuItem () .set_sensitive (false);
		getUndoButton ()   .set_sensitive (false);
	}

	if (undoHistory .hasRedo ())
	{
		getRedoMenuItem () .set_label (undoHistory .getRedoDescription ());
		getRedoButton ()   .set_tooltip_text (undoHistory .getRedoDescription ());
		getRedoMenuItem () .set_sensitive (true);
		getRedoButton ()   .set_sensitive (true);
	}
	else
	{
		getRedoMenuItem () .set_label (_ ("Redo"));
		getRedoButton ()   .set_tooltip_text (_ ("Redo last action (Ctrl-Shift-Z)."));
		getRedoMenuItem () .set_sensitive (false);
		getRedoButton ()   .set_sensitive (false);
	}
}

void
ColorPerVertexEditor::on_remove_unused_colors_activate ()
{
	std::set <int32_t> indexIndex;

	for (const auto & index : previewGeometry -> colorIndex ())
		indexIndex .emplace (index);

	std::vector <int32_t> remap (previewColor -> color () .size ());
	X3D::MFColorRGBA      colors;

	colors .reserve (previewColor -> color () .size ());

	for (int32_t index = 0, size = previewColor -> color () .size (); index < size; ++ index)
	{
		if (indexIndex .count (index))
		{
			remap [index] = colors .size ();
			colors .emplace_back (previewColor -> color () [index]);
		}
	}

	X3D::MFInt32 colorIndex;

	colorIndex .reserve (previewGeometry -> colorIndex () .size ());

	for (const auto & index : previewGeometry -> colorIndex ())
	{
		try
		{
			colorIndex .emplace_back (index < 0 ? -1 : remap .at (index));
		}
		catch (const std::out_of_range &)
		{
			colorIndex .emplace_back (0);
		}
	}

	// Assign colorIndex and color.

	if (colors .size () == previewColor -> color () .size ())
		return;

	const auto undoStep = std::make_shared <UndoStep> (_ ("Remove Unused Colors"));

	undoStep -> addObjects (previewGeometry);

	undoStep -> addUndoFunction (&X3D::MFInt32::setValue, std::ref (previewGeometry -> colorIndex ()), previewGeometry -> colorIndex ());
	undoStep -> addRedoFunction (&X3D::MFInt32::setValue, std::ref (previewGeometry -> colorIndex ()), colorIndex);
	previewGeometry -> colorIndex () = std::move (colorIndex);

	undoStep -> addUndoFunction (&X3D::MFColorRGBA::setValue, std::ref (previewColor -> color ()), previewColor -> color ());
	undoStep -> addRedoFunction (&X3D::MFColorRGBA::setValue, std::ref (previewColor -> color ()), colors);
	previewColor -> color () = std::move (colors);

	addUndoStep (undoStep);
}

void
ColorPerVertexEditor::on_hand_toggled ()
{
	preview -> isPickable (false);
}

void
ColorPerVertexEditor::on_arrow_toggled ()
{
	preview -> isPickable (true);
}

void
ColorPerVertexEditor::on_checkerboard_toggled ()
{
	try
	{
		const auto layerSet = preview -> getExecutionContext () -> getNamedNode <X3D::LayerSet> ("LayerSet");

		if (getCheckerBoardButton () .get_active ())
			layerSet -> order () = { 2, 3, 4 };
		else
			layerSet -> order () = { 1, 3, 4 };

		getConfig () .setItem ("checkerBoard", getCheckerBoardButton () .get_active ());
	}
	catch (const X3D::X3DError &)
	{ }
}

// Shading menu

void
ColorPerVertexEditor::on_phong_activate ()
{
	if (getPhongMenuItem () .get_active ())
		on_shading_activate ("PHONG");
}

void
ColorPerVertexEditor::on_gouraud_activate ()
{
	if (getGouraudMenuItem () .get_active ())
		on_shading_activate ("GOURAUD");
}

void
ColorPerVertexEditor::on_flat_activate ()
{
	if (getFlatMenuItem () .get_active ())
		on_shading_activate ("FLAT");
}

void
ColorPerVertexEditor::on_wireframe_activate ()
{
	if (getWireFrameMenuItem () .get_active ())
		on_shading_activate ("WIREFRAME");
}

void
ColorPerVertexEditor::on_pointset_activate ()
{
	if (getPointSetMenuItem () .get_active ())
		on_shading_activate ("POINTSET");
}

void
ColorPerVertexEditor::on_shading_activate (const std::string & value)
{
	preview -> getBrowserOptions () -> shading () = value;
}

void
ColorPerVertexEditor::on_texture_toggled ()
{
	try
	{
		setTexture (getTextureButton () .get_active ());

		getConfig () .setItem ("texture", getTextureButton () .get_active ());
	}
	catch (const X3D::X3DError &)
	{ }
}

void
ColorPerVertexEditor::on_look_at_all_clicked ()
{
	if (preview -> getActiveLayer ())
		preview -> getActiveLayer () -> lookAt ();
}

void
ColorPerVertexEditor::on_look_at_toggled ()
{
	if (getLookAtButton () .get_active ())
	{
		if (preview -> getViewer () not_eq X3D::ViewerType::LOOKAT)
			preview -> setViewer (X3D::ViewerType::LOOKAT);
	}
	else
	{
		if (preview -> getViewer () not_eq X3D::ViewerType::EXAMINE)
			preview -> setViewer (X3D::ViewerType::EXAMINE);
	}
}

void
ColorPerVertexEditor::on_single_vertex_clicked ()
{
	mode = SINGLE_VERTEX;

	getConfig () .setItem ("mode", mode);
}

void
ColorPerVertexEditor::on_adjacent_vertices_clicked ()
{
	mode = ADJACENT_VERTICES;

	getConfig () .setItem ("mode", mode);
}

void
ColorPerVertexEditor::on_single_face_clicked ()
{
	mode = SINGLE_FACE;

	getConfig () .setItem ("mode", mode);
}

void
ColorPerVertexEditor::on_whole_object_clicked ()
{
	mode = WHOLE_OBJECT;

	getConfig () .setItem ("mode", mode);
}

void
ColorPerVertexEditor::on_remove_clicked ()
{
	const auto undoStep = std::make_shared <UndoStep> (_ ("Remove Polygon Colors"));

	undoStep -> addObjects (geometry);

	undoStep -> addUndoFunction (&X3D::SFBool::setValue, std::ref (geometry -> colorPerVertex ()), geometry -> colorPerVertex ());
	undoStep -> addRedoFunction (&X3D::SFBool::setValue, std::ref (geometry -> colorPerVertex ()), true);
	geometry -> colorPerVertex () = true;

	undoStep -> addUndoFunction (&X3D::MFInt32::setValue, std::ref (geometry -> colorIndex ()), geometry -> colorIndex ());
	undoStep -> addRedoFunction (&X3D::MFInt32::clear, std::ref (geometry -> colorIndex ()));
	geometry -> colorIndex () .clear ();

	getBrowserWindow () -> replaceNode (X3D::SFNode (geometry), geometry -> color (), X3D::SFNode (), undoStep);
	getBrowserWindow () -> addUndoStep (undoStep);
}

void
ColorPerVertexEditor::on_apply_clicked ()
{
	geometry -> colorIndex () .removeInterest (this, &ColorPerVertexEditor::set_colorIndex);
	geometry -> colorIndex () .addInterest (this, &ColorPerVertexEditor::connectColorIndex);
	geometry -> color ()      .removeInterest (this, &ColorPerVertexEditor::set_colorIndex);
	geometry -> color ()      .addInterest (this, &ColorPerVertexEditor::connectColor);

	const auto undoStep = std::make_shared <UndoStep> (_ ("Apply Polygon Colors"));

	undoStep -> addObjects (geometry);

	undoStep -> addUndoFunction (&X3D::SFBool::setValue, std::ref (geometry -> colorPerVertex ()), geometry -> colorPerVertex ());
	undoStep -> addRedoFunction (&X3D::SFBool::setValue, std::ref (geometry -> colorPerVertex ()), true);
	geometry -> colorPerVertex () = true;

	undoStep -> addUndoFunction (&X3D::MFInt32::setValue, std::ref (geometry -> colorIndex ()), geometry -> colorIndex ());
	undoStep -> addRedoFunction (&X3D::MFInt32::setValue, std::ref (geometry -> colorIndex ()), previewGeometry -> colorIndex ());
	geometry -> colorIndex () = previewGeometry -> colorIndex ();

	if (previewColor -> isTransparent ())
	{
		const auto color = geometry -> getExecutionContext () -> createNode <X3D::ColorRGBA> ();

		color -> color () = previewColor -> color ();

		getBrowserWindow () -> replaceNode (X3D::SFNode (geometry), geometry -> color (), X3D::SFNode (color), undoStep);
	}
	else
	{
		const auto color = geometry -> getExecutionContext () -> createNode <X3D::Color> ();

		for (const auto & c : previewColor -> color ())
			color -> color () .emplace_back (c .getRed (), c .getGreen (), c .getBlue ());

		getBrowserWindow () -> replaceNode (X3D::SFNode (geometry), geometry -> color (), X3D::SFNode (color), undoStep);
	}

	geometry -> getExecutionContext () -> realize ();

	getBrowserWindow () -> addUndoStep (undoStep);
}

void
ColorPerVertexEditor::connectColorIndex ()
{
	geometry -> colorIndex () .removeInterest (this, &ColorPerVertexEditor::connectColorIndex);
	geometry -> colorIndex () .addInterest (this, &ColorPerVertexEditor::set_colorIndex);
}

void
ColorPerVertexEditor::connectColor ()
{
	geometry -> color () .removeInterest (this, &ColorPerVertexEditor::connectColorIndex);
	geometry -> color () .addInterest (this, &ColorPerVertexEditor::set_colorIndex);
}

void
ColorPerVertexEditor::set_viewer ()
{
	if (preview -> getActiveLayer ())
	{
		preview -> getActiveLayer () -> getViewpoint () -> resetUserOffsets ();
		preview -> getActiveLayer () -> lookAt ();
	}
}

void
ColorPerVertexEditor::set_shape (const X3D::X3DPtr <X3D::X3DShapeNode> & value)
{
	try
	{
		if (shape)
		{
			shape -> appearance () .removeInterest (this, &ColorPerVertexEditor::set_appearance);
			shape -> geometry ()   .removeInterest (this, &ColorPerVertexEditor::set_geometry);
		}

		shape = value;

		if (shape)
		{
			const auto transform       = preview -> getExecutionContext () -> getNamedNode <X3D::Transform> ("Transform");
			const auto modelViewMatrix = getBrowserWindow () -> findModelViewMatrix (shape);

			transform -> setMatrix (modelViewMatrix);

			shape -> appearance () .addInterest (this, &ColorPerVertexEditor::set_appearance);
			shape -> geometry ()   .addInterest (this, &ColorPerVertexEditor::set_geometry);

			set_appearance (shape -> appearance ());
			set_geometry (shape -> geometry ());
		}
		else
		{
			set_appearance (nullptr);
			set_geometry (nullptr);
		}
	}
	catch (const X3D::X3DError &)
	{ }
}

void
ColorPerVertexEditor::set_appearance (const X3D::SFNode & value)
{
	if (appearance)
	{
		appearance -> texture ()          .removeInterest (this, &ColorPerVertexEditor::set_texture);
		appearance -> textureTransform () .removeInterest (this, &ColorPerVertexEditor::set_textureTransform);
		setTexture (false);
	}

	appearance = value;

	if (appearance)
	{
		appearance -> texture ()          .addInterest (this, &ColorPerVertexEditor::set_texture);
		appearance -> textureTransform () .addInterest (this, &ColorPerVertexEditor::set_textureTransform);
	
		set_texture (appearance -> texture ());
		set_textureTransform (appearance -> textureTransform ());
		setTexture (getTextureButton () .get_active ());
	}
	else
	{
		set_texture (nullptr);
		set_textureTransform (nullptr);
	}
}

void
ColorPerVertexEditor::set_texture (const X3D::SFNode & value)
{
	if (texture)
		texture -> removeInterest (*preview, &X3D::Browser::addEvent);

	texture = value;

	if (texture)
		texture -> addInterest (*preview, &X3D::Browser::addEvent);
		
	preview -> addEvent ();
}

void
ColorPerVertexEditor::set_textureTransform (const X3D::SFNode & value)
{
	if (textureTransform)
		textureTransform -> removeInterest (*preview, &X3D::Browser::addEvent);

	textureTransform = value;

	if (textureTransform)
		textureTransform -> addInterest (*preview, &X3D::Browser::addEvent);
	
	preview -> addEvent ();
}

void
ColorPerVertexEditor::setTexture (const bool value)
{
	try
	{
		if (value and appearance)
		{
			appearance -> texture ()          .addInterest (this, &ColorPerVertexEditor::set_multi_texture);
			appearance -> textureTransform () .addInterest (this, &ColorPerVertexEditor::set_multi_textureTransform);

			set_multi_texture ();
			set_multi_textureTransform ();
		}
		else
		{
			if (appearance)
			{
				appearance -> texture ()          .removeInterest (this, &ColorPerVertexEditor::set_multi_texture);
				appearance -> textureTransform () .removeInterest (this, &ColorPerVertexEditor::set_multi_textureTransform);
			}

			const auto previewAppearance = preview -> getExecutionContext () -> getNamedNode <X3D::Appearance> ("Appearance");

			previewAppearance -> texture ()          = nullptr;
			previewAppearance -> textureTransform () = nullptr;
		}
	}
	catch (const X3D::X3DError &)
	{ }
}

void
ColorPerVertexEditor::set_multi_texture ()
{
	const auto previewAppearance = preview -> getExecutionContext () -> getNamedNode <X3D::Appearance> ("Appearance");

	const X3D::X3DPtr <X3D::MultiTexture> multiTexture (appearance -> texture ());

	if (multiTexture)
	{
		previewAppearance -> texture () = appearance -> texture () -> copy (previewAppearance -> getExecutionContext (), X3D::FLAT_COPY);
		previewAppearance -> texture () -> isPrivate (true);
		previewAppearance -> getExecutionContext () -> realize ();
	}
	else
		previewAppearance -> texture () = appearance -> texture ();
}

void
ColorPerVertexEditor::set_multi_textureTransform ()
{
	const auto previewAppearance = preview -> getExecutionContext () -> getNamedNode <X3D::Appearance> ("Appearance");

	const X3D::X3DPtr <X3D::MultiTextureTransform> multiTextureTransform (appearance -> textureTransform ());

	if (multiTextureTransform)
	{
		previewAppearance -> textureTransform () = appearance -> textureTransform () -> copy (previewAppearance -> getExecutionContext (), X3D::FLAT_COPY);
		previewAppearance -> textureTransform () -> isPrivate (true);
		previewAppearance -> getExecutionContext () -> realize ();
	}
	else
		previewAppearance -> textureTransform () = appearance -> textureTransform ();
}

void
ColorPerVertexEditor::set_geometry (const X3D::SFNode & value)
{
	undoHistory .clear ();

	try
	{
		const auto previewShape = preview -> getExecutionContext () -> getNamedNode <X3D::Shape> ("Shape");

		if (geometry)
		{
			geometry -> colorIndex () .removeInterest (this, &ColorPerVertexEditor::set_colorIndex);
			geometry -> coordIndex () .removeInterest (this, &ColorPerVertexEditor::set_coordIndex);
			geometry -> color ()      .removeInterest (this, &ColorPerVertexEditor::set_colorIndex);
			geometry -> coord ()      .removeInterest (this, &ColorPerVertexEditor::set_coord );
		}

		geometry = value;

		if (geometry)
		{
			coord           = geometry -> coord ();
			previewGeometry = geometry -> copy (previewShape -> getExecutionContext (), X3D::FLAT_COPY);
			previewGeometry -> isPrivate (true);

			geometry -> solid ()           .addInterest (previewGeometry -> solid ());
			geometry -> convex ()          .addInterest (previewGeometry -> convex ());
			geometry -> ccw ()             .addInterest (previewGeometry -> ccw ());
			geometry -> creaseAngle ()     .addInterest (previewGeometry -> creaseAngle ());
			geometry -> normalPerVertex () .addInterest (previewGeometry -> normalPerVertex ());
			geometry -> normalIndex ()     .addInterest (previewGeometry -> normalIndex ());
			geometry -> texCoordIndex ()   .addInterest (previewGeometry -> texCoordIndex ());
			geometry -> coordIndex ()      .addInterest (previewGeometry -> coordIndex ());
			geometry -> normal ()          .addInterest (previewGeometry -> normal ());
			geometry -> texCoord ()        .addInterest (previewGeometry -> texCoord ());
			geometry -> coord ()           .addInterest (previewGeometry -> coord ());		

			geometry -> colorIndex () .addInterest (this, &ColorPerVertexEditor::set_colorIndex);
			geometry -> coordIndex () .addInterest (this, &ColorPerVertexEditor::set_coordIndex);
			geometry -> color ()      .addInterest (this, &ColorPerVertexEditor::set_colorIndex);
			geometry -> coord ()      .addInterest (this, &ColorPerVertexEditor::set_coord);		

			previewShape -> geometry () = previewGeometry;

			selection -> setGeometry (previewGeometry);
			selection -> setCoord (coord);

			set_colorIndex ();

			// Initialize all.

			preview -> getExecutionContext () -> realize ();
		}
		else
		{
			coord                       = nullptr;
			geometry                    = nullptr;
			previewGeometry             = nullptr;
			previewShape -> geometry () = nullptr;
			colorButton .setNodes ({ });

			selection -> setGeometry (nullptr);
			selection -> setCoord (nullptr);
		}
	}
	catch (const X3D::X3DError &)
	{ }
}

void
ColorPerVertexEditor::set_colorIndex ()
{
	undoHistory .clear ();

	// Generate color and colorIndex.

	const X3D::X3DPtr <X3D::X3DColorNode> colorNode (geometry -> color ());

	previewColor = preview -> getExecutionContext () -> createNode <X3D::ColorRGBA> ();
	previewColor -> setDynamicTransparency (true);

	previewGeometry -> color () = previewColor;

	if (colorNode)
	{
		if (geometry -> colorPerVertex ())
		{
			if (geometry -> colorIndex () .empty ())
				previewGeometry -> colorIndex () = previewGeometry -> coordIndex ();
			else
			{
				previewGeometry -> colorIndex () = geometry -> colorIndex ();

				// Verify colorIndex.

				for (size_t i = 0, size = geometry -> coordIndex () .size (); i < size; ++ i)
				{
					if (geometry -> coordIndex () [i] < 0)
						previewGeometry -> colorIndex () .set1Value (i, -1);
				}
			}
		}
		else
		{
			size_t face = 0;

			for (const auto & index : previewGeometry -> coordIndex ())
			{
				if (index < 0)
				{
					++ face;
					previewGeometry -> colorIndex () .emplace_back (-1);
					continue;
				}

				if (face < geometry -> colorIndex () .size ())
					previewGeometry -> colorIndex () .emplace_back (geometry -> colorIndex () .get1Value (face));
				else
					previewGeometry -> colorIndex () .emplace_back (face);
			}
		}

		for (size_t i = 0, size = colorNode -> getSize (); i < size; ++ i)
			previewColor -> color () .emplace_back (colorNode -> get1Color (i));
	}
	else
	{
		previewGeometry -> colorIndex () .clear ();

		// Verify colorIndex.

		for (const auto & index : previewGeometry -> coordIndex ())
			previewGeometry -> colorIndex () .emplace_back (index < 0 ? -1 : 0);

		previewColor -> color () .emplace_back (X3D::Color4f (1, 1, 1, 1));
	}

	colorButton .setIndex (0);
	colorButton .setNodes ({ previewColor });

	preview -> getExecutionContext () -> realize ();
}

void
ColorPerVertexEditor::set_coordIndex ()
{
	selection -> setGeometry (previewGeometry);
	
	set_colorIndex ();
}

void
ColorPerVertexEditor::set_coord (const X3D::SFNode & value)
{
	coord = value;

	selection -> setCoord (coord);
}

void
ColorPerVertexEditor::set_hitPoint (const X3D::Vector3f & hitPoint)
{
	try
	{
		const auto touchSensor = preview -> getExecutionContext () -> getNamedNode <X3D::TouchSensor> ("TouchSensor");
		
		// Determine face and faces

		selection -> setHitPoint (hitPoint, touchSensor -> hitTriangle_changed ());

		if (selection -> isEmpty ())
			return;

		// Setup cross hair

		set_triangle (coord -> get1Point (selection -> getIndices () [0]));

		// Colorize vertices

		if (not touchSensor -> isActive ())
			return;

		if (getSelectColorButton () .get_active ())
			return;

		using set1Value = void (X3D::MFInt32::*) (const X3D::MFInt32::size_type, const int32_t &);

		switch (mode)
		{
			case SINGLE_VERTEX:
			{
				const auto index = selection -> getFace () .first + selection -> getFace () .second;

				if (previewGeometry -> colorIndex () .get1Value (index) not_eq (int32_t) colorButton .getIndex ())
				{
					const auto undoStep = std::make_shared <UndoStep> (_ ("Colorize Singe Vertex"));

					undoStep -> addObjects (previewGeometry);
					undoStep -> addUndoFunction ((set1Value) & X3D::MFInt32::set1Value, std::ref (previewGeometry -> colorIndex ()), index, previewGeometry -> colorIndex () .get1Value (index));
					undoStep -> addRedoFunction ((set1Value) & X3D::MFInt32::set1Value, std::ref (previewGeometry -> colorIndex ()), index, colorButton .getIndex ());
					previewGeometry -> colorIndex () .set1Value (index, colorButton .getIndex ());

					addUndoStep (undoStep);
				}

				break;
			}
			case ADJACENT_VERTICES:
			{
				const auto undoStep = std::make_shared <UndoStep> (_ ("Colorize Adjacent Vertices"));

				undoStep -> addObjects (previewGeometry);

				for (const auto & face : selection -> getAdjacentFaces ())
				{
					const auto index = face .first + face .second;

					if (previewGeometry -> colorIndex () .get1Value (index) not_eq (int32_t) colorButton .getIndex ())
					{
						undoStep -> addUndoFunction ((set1Value) & X3D::MFInt32::set1Value, std::ref (previewGeometry -> colorIndex ()), index, previewGeometry -> colorIndex () .get1Value (index));
						undoStep -> addRedoFunction ((set1Value) & X3D::MFInt32::set1Value, std::ref (previewGeometry -> colorIndex ()), index, colorButton .getIndex ());
						previewGeometry -> colorIndex () .set1Value (index, colorButton .getIndex ());
					}
				}

				addUndoStep (undoStep);
				break;
			}
			case SINGLE_FACE:
			{
				const auto undoStep = std::make_shared <UndoStep> (_ ("Colorize Single Face"));

				undoStep -> addObjects (previewGeometry);

				for (const auto & index : selection -> getVertices (selection -> getFace () .first))
				{
					if (previewGeometry -> colorIndex () .get1Value (index) not_eq (int32_t) colorButton .getIndex ())
					{
						undoStep -> addUndoFunction ((set1Value) & X3D::MFInt32::set1Value, std::ref (previewGeometry -> colorIndex ()), index, previewGeometry -> colorIndex () .get1Value (index));
						undoStep -> addRedoFunction ((set1Value) & X3D::MFInt32::set1Value, std::ref (previewGeometry -> colorIndex ()), index, colorButton .getIndex ());
						previewGeometry -> colorIndex () .set1Value (index, colorButton .getIndex ());
					}
				}

				addUndoStep (undoStep);
				break;
			}
			case WHOLE_OBJECT:
			{
				X3D::MFInt32 colorIndex;

				for (const auto & index : previewGeometry -> coordIndex ())
					colorIndex .emplace_back (index < 0 ? -1 : colorButton .getIndex ());

				if (previewGeometry -> colorIndex () not_eq colorIndex)
				{
					const auto undoStep = std::make_shared <UndoStep> (_ ("Colorize Whole Object"));

					undoStep -> addObjects (previewGeometry);

					undoStep -> addUndoFunction (&X3D::MFInt32::setValue, std::ref (previewGeometry -> colorIndex ()), previewGeometry -> colorIndex ());
					undoStep -> addRedoFunction (&X3D::MFInt32::setValue, std::ref (previewGeometry -> colorIndex ()), colorIndex);
					previewGeometry -> colorIndex () = std::move (colorIndex);

					addUndoStep (undoStep);
				}

				break;
			}
		}
	}
	catch (const X3D::X3DError &)
	{ }
}

void
ColorPerVertexEditor::set_touchTime ()
{
	if (getSelectColorButton () .get_active ())
	{
		getSelectColorButton () .set_active (false);

		const auto index = previewGeometry -> colorIndex () .get1Value (selection -> getFace () .first + selection -> getFace () .second);

		colorButton .setIndex (index);
		return;
	}
}

void
ColorPerVertexEditor::set_triangle (const X3D::Vector3f & point)
{
	try
	{
		const auto triangleBackGeometry = preview -> getExecutionContext () -> getNamedNode <X3D::IndexedLineSet> ("TriangleBackGeometry");
		const auto triangleGeometry     = preview -> getExecutionContext () -> getNamedNode <X3D::IndexedLineSet> ("TriangleGeometry");
		const auto triangleCoordinate   = preview -> getExecutionContext () -> getNamedNode <X3D::Coordinate> ("TriangleCoordinate");
		const auto vertices             = selection -> getVertices (selection -> getFace () .first);

		if (vertices .size () < 3)
			return;

		const auto vertex = selection -> getFace () .second;
		const auto i1     = vertex == 0 ? vertices .size () - 1 : vertex - 1;
		const auto i2     = vertex;
		const auto i3     = (vertex + 1) % vertices .size ();

		triangleGeometry -> coordIndex () .clear ();
		triangleGeometry -> coordIndex () = { i1, i2, i3, -1 };
		
		for (size_t i = i3, size = i3 + vertices .size () - 1; i < size; ++ i)
			triangleGeometry -> coordIndex () .emplace_back (i % vertices .size ());

		triangleGeometry -> coordIndex () .emplace_back (-1);
		triangleBackGeometry -> coordIndex () = triangleGeometry -> coordIndex ();

		triangleCoordinate -> point () .clear ();

		for (const auto & index : vertices)
			triangleCoordinate -> point () .emplace_back (coord -> get1Point (previewGeometry -> coordIndex () [index]));
	}
	catch (const X3D::X3DError &)
	{ }
}

ColorPerVertexEditor::~ColorPerVertexEditor ()
{
	undoHistory .clear ();
	dispose ();
}

} // puck
} // titania
