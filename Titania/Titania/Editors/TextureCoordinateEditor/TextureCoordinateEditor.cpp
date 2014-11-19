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

#include "TextureCoordinateEditor.h"

#include "../FaceSelection.h"
#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"

namespace titania {
namespace puck {

TextureCoordinateEditor::TextureCoordinateEditor (X3DBrowserWindow* const browserWindow) :
	                   X3DBaseInterface (browserWindow, browserWindow -> getBrowser ()),
	X3DTextureCoordinateEditorInterface (get_ui ("Editors/TextureCoordinateEditor.xml"), gconf_dir ()),
	                               left (X3D::createBrowser (getBrowserWindow () -> getBrowser ())),
	                              right (X3D::createBrowser (getBrowserWindow () -> getBrowser ())),
	                        initialized (0),
	                              shape (),
	                         appearance (),
	                           material (),
	                            texture (),
	                   textureTransform (),
	                           geometry (),
	                              coord (),
	                    previewGeometry (),
	                           texCoord (),
	                              stage (0),
	                     rightSelection (new FaceSelection ()),
	                 rightPaintSelecion (false),
	                      selectedFaces (),
	                        undoHistory ()
{
	left  -> set_antialiasing (4);
	right -> set_antialiasing (4);

	setup ();
}

void
TextureCoordinateEditor::initialize ()
{
	X3DTextureCoordinateEditorInterface::initialize ();

	getLeftBox ()  .pack_start (*left, true, true, 0);
	getRightBox () .pack_start (*right, true, true, 0);

	left  -> show ();
	right -> show ();
	left  -> initialized () .addInterest (this, &TextureCoordinateEditor::set_left_initialized);
	right -> initialized () .addInterest (this, &TextureCoordinateEditor::set_right_initialized);

	getBrowserWindow () -> getSelection () -> getChildren () .addInterest (this, &TextureCoordinateEditor::set_selection);

	undoHistory .addInterest (this, &TextureCoordinateEditor::set_undoHistory);
}

void
TextureCoordinateEditor::set_left_initialized ()
{
	++ initialized;

	left -> initialized () .removeInterest (this, &TextureCoordinateEditor::set_left_initialized);

	try
	{
		left -> loadURL ({ get_ui ("Editors/TextureCoordinateEditorLeftPreview.x3dv") });

		set_initialized ();
	}
	catch (const X3D::X3DError &)
	{ }
}

void
TextureCoordinateEditor::set_right_initialized ()
{
	++ initialized;

	right -> initialized () .removeInterest (this, &TextureCoordinateEditor::set_right_initialized);

	try
	{
		right -> loadURL ({ get_ui ("Editors/TextureCoordinateEditorRightPreview.x3dv") });

		set_initialized ();
	}
	catch (const X3D::X3DError &)
	{ }
}

void
TextureCoordinateEditor::set_initialized ()
{
	if (initialized not_eq 2)
		return;

	try
	{
		const auto selectedGeometry = left -> getExecutionContext () -> getNamedNode <X3D::IndexedLineSet> ("SelectedGeometry");
		const auto appearance   = left -> getExecutionContext () -> getNamedNode <X3D::Appearance> ("Appearance");

		selectedGeometry -> addInterest (this, &TextureCoordinateEditor::set_left_image);

		appearance -> isPrivate (true);
	}
	catch (const X3D::X3DError &)
	{ }

	try
	{
		const auto shape             = right -> getExecutionContext () -> getNamedNode <X3D::Shape> ("Shape");
		const auto appearance        = right -> getExecutionContext () -> getNamedNode <X3D::Appearance> ("Appearance");
		const auto touchSensor       = right -> getExecutionContext () -> getNamedNode <X3D::TouchSensor> ("TouchSensor");
		const auto selectedGeometry  = right -> getExecutionContext () -> getNamedNode <X3D::IndexedLineSet> ("SelectedGeometry");
		const auto selectionGeometry = right -> getExecutionContext () -> getNamedNode <X3D::IndexedLineSet> ("SelectionGeometry");

		shape -> geometry ()               .addInterest (this, &TextureCoordinateEditor::set_right_viewer);
		touchSensor -> isActive ()         .addInterest (this, &TextureCoordinateEditor::set_right_active);
		touchSensor -> hitPoint_changed () .addInterest (this, &TextureCoordinateEditor::set_right_hitPoint);
		touchSensor -> touchTime ()        .addInterest (this, &TextureCoordinateEditor::set_right_touchTime);

		appearance        -> isPrivate (true);
		selectedGeometry  -> isPrivate (true);
		selectionGeometry -> isPrivate (true);
	}
	catch (const X3D::X3DError &)
	{ }

	configure ();
	set_selection ();
}

void
TextureCoordinateEditor::configure ()
{
	// Left

	if (getConfig () .getBoolean ("leftArrow"))
		getLeftArrowButton () .set_active (true);
	else
		getLeftHandButton () .set_active (true);

	// Right

	if (getConfig () .getBoolean ("rightArrow"))
		getRightArrowButton () .set_active (true);
	else
		getRightHandButton () .set_active (true);
}

void
TextureCoordinateEditor::set_selection ()
{
	undoHistory .clear ();

	try
	{
		const auto shapes = getSelection <X3D::X3DShapeNode> ({ X3D::X3DConstants::X3DShapeNode });

		if (shapes .empty ())
			set_shape (nullptr);

		else
			set_shape (shapes .back ());

		selectedFaces .clear ();
	}
	catch (const X3D::X3DError &)
	{ }
}

// Menubar

void
TextureCoordinateEditor::on_undo_activate ()
{
	left -> grab_focus ();

	undoHistory .undoChanges ();
}

void
TextureCoordinateEditor::on_redo_activate ()
{
	left -> grab_focus ();

	undoHistory .redoChanges ();
}

void
TextureCoordinateEditor::set_undoHistory ()
{
	if (undoHistory .hasUndo ())
	{
		getUndoMenuItem () .set_label (undoHistory .getUndoDescription ());
		//getUndoButton ()   .set_tooltip_text (undoHistory .getUndoDescription ());
		getUndoMenuItem () .set_sensitive (true);
		//getUndoButton ()   .set_sensitive (true);
	}
	else
	{
		getUndoMenuItem () .set_label (_ ("Undo"));
		//getUndoButton ()   .set_tooltip_text (_ ("Undo last action (Ctrl-Z)."));
		getUndoMenuItem () .set_sensitive (false);
		//getUndoButton ()   .set_sensitive (false);
	}

	if (undoHistory .hasRedo ())
	{
		getRedoMenuItem () .set_label (undoHistory .getRedoDescription ());
		//getRedoButton ()   .set_tooltip_text (undoHistory .getRedoDescription ());
		getRedoMenuItem () .set_sensitive (true);
		//getRedoButton ()   .set_sensitive (true);
	}
	else
	{
		getRedoMenuItem () .set_label (_ ("Redo"));
		//getRedoButton ()   .set_tooltip_text (_ ("Redo last action (Ctrl-Shift-Z)."));
		getRedoMenuItem () .set_sensitive (false);
		//getRedoButton ()   .set_sensitive (false);
	}
}

void
TextureCoordinateEditor::on_left_hand_toggled ()
{
	left -> isPickable (false);
	getConfig () .setItem ("leftArrow", false);
}

void
TextureCoordinateEditor::on_left_arrow_toggled ()
{
	left -> isPickable (true);
	getConfig () .setItem ("leftArrow", true);
}

void
TextureCoordinateEditor::on_left_look_at_all_clicked ()
{
	if (left -> getActiveLayer ())
		left -> getActiveLayer () -> lookAt ();
}

void
TextureCoordinateEditor::on_right_hand_toggled ()
{
	right -> isPickable (false);
	getConfig () .setItem ("rightArrow", false);
}

void
TextureCoordinateEditor::on_right_arrow_toggled ()
{
	right -> isPickable (true);
	getConfig () .setItem ("rightArrow", true);
}

void
TextureCoordinateEditor::on_right_look_at_all_clicked ()
{
	if (right -> getActiveLayer ())
		right -> getActiveLayer () -> lookAt ();
}

void
TextureCoordinateEditor::on_right_look_at_toggled ()
{
	if (getRightLookAtButton () .get_active ())
	{
		if (right -> getViewer () not_eq X3D::ViewerType::LOOKAT)
			right -> setViewer (X3D::ViewerType::LOOKAT);
	}
	else
	{
		if (right -> getViewer () not_eq X3D::ViewerType::EXAMINE)
			right -> setViewer (X3D::ViewerType::EXAMINE);
	}
}

void
TextureCoordinateEditor::on_remove_clicked ()
{ }

void
TextureCoordinateEditor::on_apply_clicked ()
{ }

void
TextureCoordinateEditor::set_left_viewer ()
{
	if (left -> getActiveLayer ())
	{
		left -> getActiveLayer () -> getViewpoint () -> resetUserOffsets ();
		left -> getActiveLayer () -> lookAt ();
	}
}

void
TextureCoordinateEditor::set_right_viewer ()
{
	if (right -> getActiveLayer ())
	{
		right -> getActiveLayer () -> getViewpoint () -> resetUserOffsets ();
		right -> getActiveLayer () -> lookAt ();
	}
}

void
TextureCoordinateEditor::set_shape (const X3D::X3DPtr <X3D::X3DShapeNode> & value)
{
	if (shape)
	{
		shape -> appearance () .removeInterest (this, &TextureCoordinateEditor::set_appearance);
		shape -> geometry ()   .removeInterest (this, &TextureCoordinateEditor::set_geometry);
	}

	shape = value;

	if (shape)
	{
		shape -> appearance () .addInterest (this, &TextureCoordinateEditor::set_appearance);
		shape -> geometry ()   .addInterest (this, &TextureCoordinateEditor::set_geometry);

		set_appearance (shape -> appearance ());
		set_geometry (shape -> geometry ());
	}
	else
	{
		set_appearance (nullptr);
		set_geometry (nullptr);
	}
}

void
TextureCoordinateEditor::set_appearance (const X3D::SFNode & value)
{
	if (appearance)
	{
		appearance -> material ()         .removeInterest (this, &TextureCoordinateEditor::set_material);
		appearance -> texture ()          .removeInterest (this, &TextureCoordinateEditor::set_texture_stage);
		appearance -> textureTransform () .removeInterest (this, &TextureCoordinateEditor::set_textureTransform);
	}

	appearance = value;

	if (appearance)
	{
		appearance -> material ()         .addInterest (this, &TextureCoordinateEditor::set_material);
		appearance -> texture ()          .addInterest (this, &TextureCoordinateEditor::set_texture_stage);
		appearance -> textureTransform () .addInterest (this, &TextureCoordinateEditor::set_textureTransform);

		set_material (appearance -> material ());
		set_texture_stage (appearance -> texture ());
		set_textureTransform (appearance -> textureTransform ());
	}
	else
	{
		set_material (nullptr);
		set_texture_stage (nullptr);
		set_textureTransform (nullptr);
	}
}

void
TextureCoordinateEditor::set_material (const X3D::SFNode & value)
{
	if (material)
		material -> removeInterest (*right, &X3D::Browser::addEvent);

	material = value;

	if (material)
		material -> addInterest (*right,  &X3D::Browser::addEvent);

	right -> addEvent ();
}

void
TextureCoordinateEditor::set_texture_stage (const X3D::SFNode & value)
{
	const X3D::X3DPtr <X3D::MultiTexture> multiTexture (value);

	const size_t textureStages = multiTexture ? multiTexture -> getTexture () .size () : 1;

	getTextureStageAdjustment () -> set_upper (textureStages);
	getTextureStageAdjustment () -> set_value (std::min <size_t> (getTextureStageAdjustment () -> get_value (), textureStages));

	set_texture (value);
}

void
TextureCoordinateEditor::set_texture (const X3D::SFNode & value)
{
	if (texture)
	{
		texture -> removeInterest (*left,  &X3D::Browser::addEvent);
		texture -> removeInterest (*right, &X3D::Browser::addEvent);
	}

	const X3D::X3DPtr <X3D::MultiTexture> multiTexture (value);

	if (multiTexture)
	{
		if (stage < multiTexture -> getTexture () .size ())
			texture = multiTexture -> getTexture () [stage];
		else
			texture = nullptr;
	}
	else
		texture = value;

	try
	{
		const auto imageSwitch     = left  -> getExecutionContext () -> getNamedNode <X3D::Switch> ("ImageSwitch");
		const auto leftAppearance  = left  -> getExecutionContext () -> getNamedNode <X3D::Appearance> ("Appearance");
		const auto rightAppearance = right -> getExecutionContext () -> getNamedNode <X3D::Appearance> ("Appearance");

		imageSwitch -> whichChoice () = texture;
		leftAppearance  -> texture () = texture;
		rightAppearance -> texture () = texture;
	}
	catch (const X3D::X3DError &)
	{ }

	if (texture)
	{
		texture -> addInterest (*left,  &X3D::Browser::addEvent);
		texture -> addInterest (*right, &X3D::Browser::addEvent);
	}
}

void
TextureCoordinateEditor::set_textureTransform (const X3D::SFNode & value)
{
	if (textureTransform)
	{
		textureTransform -> removeInterest (*left,  &X3D::Browser::addEvent);
		textureTransform -> removeInterest (*right, &X3D::Browser::addEvent);
	}

	const X3D::X3DPtr <X3D::MultiTextureTransform> multiTextureTransform (value);

	if (multiTextureTransform)
	{
		if (multiTextureTransform -> getTextureTransform () .empty ())
			textureTransform = nullptr;
		else
			textureTransform = multiTextureTransform -> getTextureTransform () [std::min (stage, multiTextureTransform -> getTextureTransform () .size () - 1)];
	}
	else
		textureTransform = value;

	try
	{
		const auto leftAppearance  = left  -> getExecutionContext () -> getNamedNode <X3D::Appearance> ("Appearance");
		const auto rightAppearance = right -> getExecutionContext () -> getNamedNode <X3D::Appearance> ("Appearance");

		leftAppearance  -> textureTransform () = textureTransform;
		rightAppearance -> textureTransform () = textureTransform;
	}
	catch (const X3D::X3DError &)
	{ }

	if (textureTransform)
	{
		textureTransform -> addInterest (*left,  &X3D::Browser::addEvent);
		textureTransform -> addInterest (*right, &X3D::Browser::addEvent);
	}

	left  -> addEvent ();
	right -> addEvent ();
}

void
TextureCoordinateEditor::set_geometry (const X3D::SFNode & value)
{
	if (geometry)
	{
		try
		{
			geometry -> getField <X3D::SFNode> ("texCoord") .removeInterest (this, &TextureCoordinateEditor::set_texCoord);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	geometry = value;

	try
	{
		const auto rightShape             = right -> getExecutionContext () -> getNamedNode <X3D::Shape> ("Shape");
		const auto rightSelectedGeometry  = right -> getExecutionContext () -> getNamedNode <X3D::IndexedLineSet> ("SelectedGeometry");
		const auto rightSelectionGeometry = right -> getExecutionContext () -> getNamedNode <X3D::IndexedLineSet> ("SelectionGeometry");

		if (geometry)
		{
			coord = geometry -> coord ();

			previewGeometry = geometry -> copy (rightShape -> getExecutionContext (), X3D::FLAT_COPY);
			previewGeometry -> isPrivate (true);

			rightShape -> geometry () = previewGeometry;
			rightShape -> getExecutionContext () -> realize ();

			set_texCoordIndex ();

			rightSelectedGeometry  -> coordIndex () .clear ();
			rightSelectionGeometry -> coordIndex () .clear ();

			rightSelectedGeometry  -> coord () = coord;
			rightSelectionGeometry -> coord () = coord;

			rightSelection -> setGeometry (geometry);
			rightSelection -> setCoord (coord);
		}
		else
		{
			rightSelection -> setGeometry (nullptr);
			rightSelection -> setCoord (nullptr);

			coord                              = nullptr;
			previewGeometry                    = nullptr;
			rightShape -> geometry ()          = nullptr;
			rightSelectedGeometry  -> coord () = nullptr;
			rightSelectionGeometry -> coord () = nullptr;
		}
	}
	catch (const X3D::X3DError &)
	{ }

	if (geometry)
	{
		try
		{
			geometry -> getField <X3D::SFNode> ("texCoord") .addInterest (this, &TextureCoordinateEditor::set_texCoord);

			set_texCoord (geometry -> getField <X3D::SFNode> ("texCoord"));
		}
		catch (const X3D::X3DError &)
		{
			set_texCoord (nullptr);
		}
	}
	else
		set_texCoord (nullptr);

	right -> addEvent ();
}

void
TextureCoordinateEditor::set_texCoordIndex ()
{
	// Generate texCoordIndex.

	if (geometry -> texCoordIndex () .empty ())
		previewGeometry -> texCoordIndex () = geometry -> coordIndex ();
	else
		previewGeometry -> texCoordIndex () = geometry -> texCoordIndex ();

	clear ();
}

void
TextureCoordinateEditor::set_coordIndex ()
{
	rightSelection -> setGeometry (previewGeometry);
}

void
TextureCoordinateEditor::set_texCoord (const X3D::SFNode & value)
{
	// Determine texCoord.

	X3D::X3DPtr <X3D::TextureCoordinate> texCoordNode; 

	const X3D::X3DPtr <X3D::MultiTextureCoordinate> multiTextureCoordinate (value);

	if (multiTextureCoordinate)
	{
		if (multiTextureCoordinate -> getTexCoord () .empty ())
			texCoordNode = nullptr;
		else
			texCoordNode = multiTextureCoordinate -> getTexCoord () [std::min (stage, multiTextureCoordinate -> getTexCoord () .size () - 1)];
	}
	else
		texCoordNode = value;

	// Create preview texCoord node.

	texCoord = right -> getExecutionContext () -> createNode <X3D::TextureCoordinate> ();

	texCoord -> point () .addInterest (this, &TextureCoordinateEditor::set_left_coord);

	if (texCoordNode)
		texCoord -> point () = texCoordNode -> point ();

	right -> getExecutionContext () -> realize ();
	
	clear ();
	set_left_coord ();

	// Add texCoord.

	if (previewGeometry)
	{
		try
		{
			previewGeometry -> getField <X3D::SFNode> ("texCoord") = texCoord;
		}
		catch (const X3D::X3DError &)
		{ }
	}

	left  -> addEvent ();
	right -> addEvent ();
}

void
TextureCoordinateEditor::on_texture_stage_changed ()
{
	stage = getTextureStageAdjustment () -> get_value () - 1;

	if (appearance)
	{
		set_texture (appearance -> texture ());
		set_textureTransform (appearance -> textureTransform ());
	}
	else
	{
		set_texture (nullptr);
		set_textureTransform (nullptr);
	}

	if (geometry)
	{
		try
		{
			set_texCoord (geometry -> getField <X3D::SFNode> ("texCoord"));
		}
		catch (const X3D::X3DError &)
		{
			set_texCoord (nullptr);
		}
	}
	else
		set_texCoord (nullptr);
}

void
TextureCoordinateEditor::clear ()
{
	selectedFaces .clear ();

	set_selected_faces ();
}

void
TextureCoordinateEditor::set_selected_faces ()
{
	set_left_selected_faces ();
	set_right_selected_faces ();
}

void
TextureCoordinateEditor::set_left_coord ()
{
	try
	{
		const auto selectedCoord = left -> getExecutionContext () -> getNamedNode <X3D::Coordinate> ("SelectedCoord");

		size_t i = 0;

		selectedCoord -> point () .resize (texCoord -> point () .size ());

		for (const auto & point : texCoord -> point ())
			selectedCoord -> point () [i ++] = X3D::Vector3f (point .getX (), point .getY (), 0);
	}
	catch (const X3D::X3DError &)
	{ }
}


void
TextureCoordinateEditor::set_left_image ()
{
	try
	{
		const auto selectedGeometry = left -> getExecutionContext () -> getNamedNode <X3D::IndexedLineSet> ("SelectedGeometry");
		const auto imageTexCoord    = left -> getExecutionContext () -> getNamedNode <X3D::TextureCoordinate> ("ImageTextureCoordinate");
		const auto imageCoord       = left -> getExecutionContext () -> getNamedNode <X3D::Coordinate> ("ImageCoordinate");

		auto bbox = selectedGeometry -> getBBox ();

		if (bbox .empty ())
			bbox = X3D::Box3f (X3D::Vector3f (1, 1, 0), X3D::Vector3f (0.5, 0.5, 0));

		X3D::Vector3f min, max;
		bbox .extents (min, max);

		min .x (std::floor (min .x ()));
		min .y (std::floor (min .y ()));
		max .x (std::ceil (max .x ()));
		max .y (std::ceil (max .y ()));

		imageTexCoord -> point () = {
			X3D::Vector2f (min .x (), min .y ()),
			X3D::Vector2f (max .x (), min .y ()),
			X3D::Vector2f (max .x (), max .y ()),
			X3D::Vector2f (min .x (), max .y ())
		};

		imageCoord -> point () = {
			X3D::Vector3f (min .x (), min .y (), 0),
			X3D::Vector3f (max .x (), min .y (), 0),
			X3D::Vector3f (max .x (), max .y (), 0),
			X3D::Vector3f (min .x (), max .y (), 0)
		};
	}
	catch (const X3D::X3DError &)
	{ }
}

void
TextureCoordinateEditor::set_left_selected_faces ()
{
	try
	{
		const auto selectedGeometry = left -> getExecutionContext () -> getNamedNode <X3D::IndexedLineSet> ("SelectedGeometry");
		
		selectedGeometry -> coordIndex () .clear ();

		for (const auto & face : selectedFaces)
		{
			const auto points = rightSelection -> getPoints (face);

			if (points .size () < 3)
				continue;

			const auto first = selectedGeometry -> coordIndex () .size ();

			for (const auto & i : points)
				selectedGeometry -> coordIndex () .emplace_back (previewGeometry -> texCoordIndex () [i]);

			selectedGeometry -> coordIndex () .emplace_back (selectedGeometry -> coordIndex () [first]);
			selectedGeometry -> coordIndex () .emplace_back (-1);
		}

	}
	catch (const X3D::X3DError &)
	{ }
}

void
TextureCoordinateEditor::set_right_hitPoint (const X3D::Vector3f & hitPoint)
{
	try
	{
		const auto touchSensor = right -> getExecutionContext () -> getNamedNode <X3D::TouchSensor> ("TouchSensor");

		// Determine face and faces

		rightSelection -> setHitPoint (hitPoint, touchSensor -> hitTriangle_changed ());
		
		if (rightSelection -> isEmpty ())
			return;

		// Setup cross hair

		set_right_selection (coord -> get1Point (rightSelection -> getIndices () [0]));

		if (touchSensor -> isActive () and (right -> hasShiftKey () or right -> hasControlKey ()))
		{
			rightPaintSelecion = true;
			set_right_touchTime ();
		}
	}
	catch (const X3D::X3DError &)
	{ }
}

void
TextureCoordinateEditor::set_right_selection (const X3D::Vector3f & point)
{
	try
	{
		const auto selectionGeometry = right -> getExecutionContext () -> getNamedNode <X3D::IndexedLineSet> ("SelectionGeometry");
		const auto points            = rightSelection -> getPoints (rightSelection -> getFace () .first);

		if (points .size () < 3)
			return;

		selectionGeometry -> coordIndex () .clear ();
		
		for (const auto & i : points)
			selectionGeometry -> coordIndex () .emplace_back (previewGeometry -> coordIndex () [i]);

		selectionGeometry -> coordIndex () .emplace_back (selectionGeometry -> coordIndex () .front ());
		selectionGeometry -> coordIndex () .emplace_back (-1);
	}
	catch (const X3D::X3DError &)
	{ }
}

void
TextureCoordinateEditor::set_right_active (const bool value)
{
	if (value)
		rightPaintSelecion = false;
}

void
TextureCoordinateEditor::set_right_touchTime ()
{
	if (rightSelection -> isEmpty ())
		return;

	if (not right -> hasShiftKey () and not right -> hasControlKey () and not rightPaintSelecion)
		selectedFaces .clear ();

	if (right -> hasControlKey ())
		selectedFaces .erase (rightSelection -> getFace () .first);

	else if (not selectedFaces .emplace (rightSelection -> getFace () .first) .second)
		return;

	set_selected_faces ();
}

void
TextureCoordinateEditor::set_right_selected_faces ()
{
	try
	{
		const auto selectedGeometry = right -> getExecutionContext () -> getNamedNode <X3D::IndexedLineSet> ("SelectedGeometry");
		
		selectedGeometry -> coordIndex () .clear ();

		for (const auto & face : selectedFaces)
		{
			const auto points = rightSelection -> getPoints (face);

			if (points .size () < 3)
				continue;

			const auto first = selectedGeometry -> coordIndex () .size ();

			for (const auto & i : points)
				selectedGeometry -> coordIndex () .emplace_back (previewGeometry -> coordIndex () [i]);

			selectedGeometry -> coordIndex () .emplace_back (selectedGeometry -> coordIndex () [first]);
			selectedGeometry -> coordIndex () .emplace_back (-1);
		}
	}
	catch (const X3D::X3DError &)
	{ }
}

TextureCoordinateEditor::~TextureCoordinateEditor ()
{
	undoHistory .clear ();
	dispose ();
}

} // puck
} // titania
