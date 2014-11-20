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

#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"
#include "../FaceSelection.h"

#include <Titania/X3D/Browser/ContextLock.h>

namespace titania {
namespace puck {

constexpr double POINT_SIZE = 6; // Use linewidthScaleFactor / 2 + 2.
constexpr X3D::Vector2d
infinity2f (std::numeric_limits <float>::infinity (), std::numeric_limits <float>::infinity ());

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
	                      multiTexCoord (),
	                              coord (),
	                    previewGeometry (),
	                           texCoord (),
	                              stage (0),
	                     rightSelection (new FaceSelection ()),
	                 rightPaintSelecion (false),
	                      selectedFaces (),
	                        activePoint (-1),
	                     selectedPoints (),
	                      startHitPoint (),
	                        pointOffset (),
	                      startPosition (),
	                     startPositions (),
	                               keys (),
	                        undoHistory (),
	                           undoStep ()
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
		const auto appearance       = left -> getExecutionContext () -> getNamedNode <X3D::Appearance> ("Appearance");
		const auto touchSensor      = left -> getExecutionContext () -> getNamedNode <X3D::TouchSensor> ("TouchSensor");
		const auto selectedGeometry = left -> getExecutionContext () -> getNamedNode <X3D::IndexedLineSet> ("SelectedGeometry");

		touchSensor -> isActive ()         .addInterest (this, &TextureCoordinateEditor::set_left_active);
		touchSensor -> touchTime ()        .addInterest (this, &TextureCoordinateEditor::set_left_touchTime);
		touchSensor -> hitPoint_changed () .addInterest (this, &TextureCoordinateEditor::set_left_hitPoint);
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
		touchSensor -> touchTime ()        .addInterest (this, &TextureCoordinateEditor::set_right_touchTime);
		touchSensor -> hitPoint_changed () .addInterest (this, &TextureCoordinateEditor::set_right_hitPoint);

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
	}
	catch (const X3D::X3DError &)
	{ }
}

// Keyboard

bool
TextureCoordinateEditor::on_key_press_event (GdkEventKey* event)
{
	keys .press (event);
	return false;
}

bool
TextureCoordinateEditor::on_key_release_event (GdkEventKey* event)
{
	keys .release (event);
	return false;
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
TextureCoordinateEditor::on_select_all_activate ()
{
	if (left -> has_focus ())
	{
		try
		{
			const auto selectedGeometry = left -> getExecutionContext () -> getNamedNode <X3D::IndexedLineSet> ("SelectedGeometry");

			for (const auto & index : selectedGeometry -> coordIndex ())
			{
				if (index >= 0)
					selectedPoints .emplace (index);
			}

			set_selectedPoints ();
		}
		catch (const X3D::X3DError &)
		{ }
	}

	else if (right -> has_focus ())
	{
		selectedFaces = rightSelection -> getFaces ();

		set_selected_faces ();
	}
}

void
TextureCoordinateEditor::on_deselect_all_activate ()
{
	if (left -> has_focus ())
	{
		selectedPoints .clear ();

		set_selectedPoints ();
	}

	else if (right -> has_focus ())
	{
		selectedFaces .clear ();

		set_selected_faces ();
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
{
	geometry -> texCoordIndex () .removeInterest (this, &TextureCoordinateEditor::set_texCoordIndex);
	geometry -> texCoordIndex () .addInterest (this, &TextureCoordinateEditor::connectTexCoordIndex);
	geometry -> texCoord ()      .removeInterest (this, &TextureCoordinateEditor::set_texCoord);
	geometry -> texCoord ()      .addInterest (this, &TextureCoordinateEditor::connectTexCoord);

	if (multiTexCoord)
	{
		multiTexCoord -> removeInterest (this, &TextureCoordinateEditor::set_texCoord);
		multiTexCoord -> addInterest (this, &TextureCoordinateEditor::connectMultiTexCoord);
	}

	const auto undoStep = std::make_shared <UndoStep> (_ ("Apply Texture Coordinates"));

	undoStep -> addObjects (geometry);

	undoStep -> addUndoFunction (&X3D::MFInt32::setValue, std::ref (geometry -> texCoordIndex ()), geometry -> texCoordIndex ());
	undoStep -> addRedoFunction (&X3D::MFInt32::setValue, std::ref (geometry -> texCoordIndex ()), previewGeometry -> texCoordIndex ());
	geometry -> texCoordIndex () = previewGeometry -> texCoordIndex ();

	// Replace texCoord or multiTexCoord.

	const auto texCoordNode = texCoord -> copy (geometry -> getExecutionContext (), X3D::FLAT_COPY);
	const X3D::X3DPtr <X3D::MultiTextureCoordinate> multiTextureCoordinate (geometry -> texCoord ());

	if (multiTextureCoordinate)
	{
		X3D::MFNode texCoords (multiTextureCoordinate -> getTexCoord () .begin (), multiTextureCoordinate -> getTexCoord () .end ());

		if (texCoords .empty ())
		{
			// XXX: Fill with default texture coordinates.
			for (size_t i = 0, size = stage; i < size; ++ i)
			 	texCoords .emplace_back (texCoordNode);
		}
		else
		{
			for (size_t i = texCoords .size () - 1, size = stage; i < size; ++ i)
				texCoords .emplace_back (texCoords .back ());
		}

		texCoords .set1Value (stage, texCoordNode);

		getBrowserWindow () -> replaceNodes (X3D::SFNode (multiTextureCoordinate), multiTextureCoordinate -> texCoord (), texCoords, undoStep);
	}
	else
		getBrowserWindow () -> replaceNode (X3D::SFNode (geometry), geometry -> texCoord (), X3D::SFNode (texCoordNode), undoStep);

	geometry -> getExecutionContext () -> realize ();

	getBrowserWindow () -> addUndoStep (undoStep);
}

void
TextureCoordinateEditor::connectTexCoordIndex ()
{
	geometry -> texCoordIndex () .removeInterest (this, &TextureCoordinateEditor::connectTexCoordIndex);
	geometry -> texCoordIndex () .addInterest (this, &TextureCoordinateEditor::set_texCoordIndex);
}

void
TextureCoordinateEditor::connectTexCoord ()
{
	geometry -> texCoord () .removeInterest (this, &TextureCoordinateEditor::connectTexCoord);
	geometry -> texCoord () .addInterest (this, &TextureCoordinateEditor::set_texCoord);
}

void
TextureCoordinateEditor::connectMultiTexCoord ()
{
	multiTexCoord -> removeInterest (this, &TextureCoordinateEditor::connectMultiTexCoord);
	multiTexCoord -> addInterest (this, &TextureCoordinateEditor::set_texCoord, X3D::SFNode (multiTexCoord));
}

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
		geometry -> texCoord () .removeInterest (this, &TextureCoordinateEditor::set_texCoord);
	}

	geometry = value;

	try
	{
		const auto rightShape             = right -> getExecutionContext () -> getNamedNode <X3D::Shape> ("Shape");
		const auto rightSelectedGeometry  = right -> getExecutionContext () -> getNamedNode <X3D::IndexedLineSet> ("SelectedGeometry");
		const auto rightSelectionGeometry = right -> getExecutionContext () -> getNamedNode <X3D::IndexedLineSet> ("SelectionGeometry");

		if (geometry)
		{
			geometry -> texCoord () .addInterest (this, &TextureCoordinateEditor::set_texCoord);

			coord = geometry -> coord ();

			previewGeometry = geometry -> copy (rightShape -> getExecutionContext (), X3D::FLAT_COPY);
			previewGeometry -> isPrivate (true);

			rightShape -> geometry () = previewGeometry;
			rightShape -> getExecutionContext () -> realize ();

			set_texCoordIndex ();
			set_texCoord (geometry -> texCoord ());

			rightSelectedGeometry  -> coordIndex () .clear ();
			rightSelectionGeometry -> coordIndex () .clear ();

			rightSelectedGeometry  -> coord () = coord;
			rightSelectionGeometry -> coord () = coord;

			rightSelection -> setGeometry (geometry);
			rightSelection -> setCoord (coord);

			getWidget () .set_sensitive (true);
		}
		else
		{
			rightSelection -> setGeometry (nullptr);
			rightSelection -> setCoord (nullptr);

			set_texCoord (nullptr);
			set_coord (nullptr);

			previewGeometry                    = nullptr;
			rightShape -> geometry ()          = nullptr;
			rightSelectedGeometry  -> coord () = nullptr;
			rightSelectionGeometry -> coord () = nullptr;

			getWidget () .set_sensitive (false);
		}
	}
	catch (const X3D::X3DError &)
	{ }

	right -> addEvent ();
}

void
TextureCoordinateEditor::set_texCoordIndex ()
{
	undoHistory .clear ();

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

	if (geometry -> texCoordIndex () .empty ())
		set_texCoordIndex ();
}

void
TextureCoordinateEditor::set_texCoord (const X3D::SFNode & value)
{
	undoHistory .clear ();

	if (multiTexCoord)
		multiTexCoord -> removeInterest (this, &TextureCoordinateEditor::set_texCoord);

	// Determine texCoord.

	X3D::X3DPtr <X3D::TextureCoordinate> texCoordNode;

	multiTexCoord = value;

	if (multiTexCoord)
	{
		multiTexCoord -> addInterest (this, &TextureCoordinateEditor::set_texCoord, value);

		if (multiTexCoord -> getTexCoord () .empty ())
			texCoordNode = nullptr;
		else
			texCoordNode = multiTexCoord -> getTexCoord () [std::min (stage, multiTexCoord -> getTexCoord () .size () - 1)];
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
TextureCoordinateEditor::set_coord (const X3D::SFNode & value)
{
	coord = value;

	rightSelection -> setCoord (coord);
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

		min .x (std::floor (min .x () - 0.5));
		min .y (std::floor (min .y () - 0.5));
		max .x (std::ceil  (max .x () + 0.5));
		max .y (std::ceil  (max .y () + 0.5));

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

		// Remove point from selection not in new indices anymore.

		std::set <int32_t> indices (selectedGeometry -> coordIndex () .begin (), selectedGeometry -> coordIndex () .end ());

		std::vector <int32_t> difference;

		std::set_difference (selectedPoints .begin (), selectedPoints .end (),
		                     indices .begin (), indices .end (),
		                     std::back_inserter (difference));

		for (const auto & index : difference)
			selectedPoints .erase (index);

		set_selectedPoints ();
	}
	catch (const X3D::X3DError &)
	{ }
}

void
TextureCoordinateEditor::set_left_active (const bool value)
{
	using set1Value = void (X3D::MFVec2f::*) (const X3D::MFVec2f::size_type, const X3D::SFVec2f &);

	if (value)
	{
		set_startDrag ();
		
		// Init undo step.

		undoStep = std::make_shared <UndoStep> (_ ("Transform Points"));
		undoStep -> addObjects (texCoord);

		for (const auto & index : selectedPoints)
			undoStep -> addUndoFunction ((set1Value) &X3D::MFVec2f::set1Value, std::ref (texCoord -> point ()), index, texCoord -> point () [index]);
	}
	else
	{
		// Drag has started.
		if (startHitPoint == infinity2f)
		{
			for (const auto & index : selectedPoints)
				undoStep -> addRedoFunction ((set1Value) &X3D::MFVec2f::set1Value, std::ref (texCoord -> point ()), index, texCoord -> point () [index]);	
			
			addUndoStep (undoStep);
		}
	}
}

void
TextureCoordinateEditor::set_startDrag ()
{
	try
	{
		if ((size_t) activePoint >= texCoord -> point () .size ())
			return;

		// Determine pointOffset.

		const auto          touchSensor = left -> getExecutionContext () -> getNamedNode <X3D::TouchSensor> ("TouchSensor");
		const X3D::Vector3f hitPoint    = touchSensor -> hitPoint_changed ();

		startHitPoint = projectPoint (hitPoint, left);
		startPosition = texCoord -> point () [activePoint];
		pointOffset   = startPosition - X3D::Vector2f (hitPoint .x (), hitPoint .y ());

		startPositions .clear ();

		for (const auto & index : selectedPoints)
			startPositions .emplace_back (index, texCoord -> point () [index]);

		// Clear selection.
	
		if (keys .shift ())
			return;
	
		if (selectedPoints .count (activePoint))
			return;
	
		selectedPoints = { activePoint };
		startPositions = { std::make_pair (activePoint, startPosition) };

		set_selectedPoints ();
	}
	catch (const X3D::X3DError &)
	{ }
	catch (const std::domain_error &)
	{ }
}

void
TextureCoordinateEditor::set_left_touchTime ()
{
	try
	{
		if ((size_t) activePoint >= texCoord -> point () .size ())
			return;

		if (not keys .shift ())
			return;

		if (startHitPoint == infinity2f)
		{
			startHitPoint = X3D::Vector2d ();
			return;
		}

		const auto selectedPoint = selectedPoints .find (activePoint);

		if (selectedPoint == selectedPoints .end ())
			selectedPoints .emplace (activePoint);
		else
			selectedPoints .erase (selectedPoint);

		set_selectedPoints ();
	}
	catch (const X3D::X3DError &)
	{ }
}

void
TextureCoordinateEditor::set_left_hitPoint (const X3D::Vector3f & hitPoint)
{
	try
	{
		const auto touchSensor = left -> getExecutionContext () -> getNamedNode <X3D::TouchSensor> ("TouchSensor");

		if ((not keys .shift () or startHitPoint == infinity2f) and touchSensor -> isActive ())
		{
			if ((size_t) activePoint < texCoord -> point () .size ())
			{
				// Prevent accidentialy moving points.
				if (math::abs (startHitPoint - projectPoint (hitPoint, left)) > POINT_SIZE)
				{
					startHitPoint = infinity2f; // Drag has started.

					const auto point       = X3D::Vector2f (hitPoint .x (), hitPoint .y ()) + pointOffset;
					auto       translation = point - startPosition;

					if (keys .control ())
					{
						if (std::abs (translation .x ()) > std::abs (translation .y ()))
							translation .y (0);
						else
							translation .x (0);
					}

					for (const auto & pair : startPositions)
						texCoord -> point () [pair .first] = pair .second + translation;
				}
			}
		}
		else
			set_left_point (hitPoint);
	}
	catch (const X3D::X3DError &)
	{ }
	catch (const std::domain_error &)
	{ }
}

void
TextureCoordinateEditor::set_left_point (const X3D::Vector3d & hitPoint)
{
	try
	{
		const auto selectedCoord  = left -> getExecutionContext () -> getNamedNode <X3D::Coordinate> ("SelectedCoord");
		const auto activePointSet = left -> getExecutionContext () -> getNamedNode ("ActivePointSet");

		activePoint = getPointIndex (hitPoint);

		if (activePoint < 0)
			activePointSet -> getField <X3D::MFInt32> ("set_coordIndex") .clear ();

		else
		{
			const auto point = selectedCoord -> get1Point (activePoint);
			const auto p     = projectPoint (point, left);    // point in pixel coordinates.
			const auto h     = projectPoint (hitPoint, left); // hitPoint in pixel coordinates.

			const auto distance = math::abs (h - p);

			if (distance < POINT_SIZE)
				activePointSet -> getField <X3D::MFInt32> ("set_coordIndex") = { activePoint };
			else
			{
				activePoint = -1;
				activePointSet -> getField <X3D::MFInt32> ("set_coordIndex") .clear ();
			}
		}
	}
	catch (const X3D::X3DError &)
	{ }
	catch (const std::domain_error &)
	{ }
}

int32_t
TextureCoordinateEditor::getPointIndex (const X3D::Vector3d & hitPoint) const
{
	try
	{
		const auto selectedGeometry = left -> getExecutionContext () -> getNamedNode <X3D::IndexedLineSet> ("SelectedGeometry");
		const auto selectedCoord    = left -> getExecutionContext () -> getNamedNode <X3D::Coordinate> ("SelectedCoord");

		// Determine activePoint of nearest point to hitPoint.

		if (selectedGeometry -> coordIndex () .empty ())
			return -1;

		if (selectedCoord -> point () .empty ())
			return -1;

		std::map <double, int32_t> distances;

		for (const auto & index : selectedGeometry -> coordIndex ())
		{
			if (index >= 0)
				distances .emplace (math::abs (hitPoint - selectedCoord -> get1Point (index)), index);
		}

		return distances .begin () -> second;
	}
	catch (const X3D::X3DError &)
	{
		return -1;
	}
}

void
TextureCoordinateEditor::set_selectedPoints ()
{
	try
	{
		const auto selectedPointSet = left -> getExecutionContext () -> getNamedNode ("SelectedPointSet");

		selectedPointSet -> getField <X3D::MFInt32> ("set_coordIndex") .assign (selectedPoints .begin (), selectedPoints .end ());
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

	if (not keys .shift () and not keys .control () and not rightPaintSelecion)
		selectedFaces .clear ();

	if (keys .control ())
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

		if (touchSensor -> isActive () and (keys .shift () or keys .control ()))
		{
			rightPaintSelecion = true;
			set_right_touchTime ();
		}
	}
	catch (const X3D::X3DError &)
	{ }
}

X3D::Vector2d
TextureCoordinateEditor::projectPoint (const X3D::Vector3d & point, const X3D::BrowserPtr & browser) const
throw (std::domain_error)
{
	X3D::ContextLock lock (left);

	if (lock and browser -> getActiveLayer ())
	{
		// Set red point if pointer is very close.
		browser -> getActiveLayer () -> getViewpoint () -> reshape (0, -10);

		const X3D::Matrix4d modelview; // Use identity
		const auto          projection = X3D::ProjectionMatrix4d ();
		const auto          viewport   = browser -> getActiveLayer () -> getViewport () -> getRectangle ();
		const auto          p          = X3D::ViewVolume::projectPoint (point, modelview, projection, viewport);

		return X3D::Vector2d (p. x (), p .y ());
	}

	throw std::domain_error ("Couldn't accquire browser lock.");
}

TextureCoordinateEditor::~TextureCoordinateEditor ()
{
	undoHistory .clear ();
	dispose ();
}

} // puck
} // titania
