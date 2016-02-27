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

#include "GeometryEditor.h"

#include "../../Browser/BrowserSelection.h"
#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"

#include <Titania/X3D/Browser/Core/Cast.h>
#include <Titania/X3D/Components/Core/WorldInfo.h>
#include <Titania/X3D/Components/Core/MetadataSet.h>
#include <Titania/X3D/Components/Rendering/X3DGeometryNode.h>
#include <Titania/X3D/Components/Shape/X3DShapeNode.h>
#include <Titania/X3D/Components/Shape/X3DShapeNode.h>
#include <Titania/X3D/Editing/Undo/UndoStepContainer.h>
#include <Titania/X3D/Tools/ToolColors.h>

namespace titania {
namespace puck {

GeometryEditor::GeometryEditor (X3DBrowserWindow* const browserWindow) :
	          X3DBaseInterface (browserWindow, browserWindow -> getCurrentBrowser ()),
	X3DGeometryEditorInterface (get_ui ("Editors/GeometryEditor.glade")),
	             normalEnabled (this, getNormalEnabledToggleButton (), "load"),
	              normalEditor (new X3D::FieldSet (getMasterBrowser ())),
	               coordEditor (new X3D::FieldSet (getMasterBrowser ())),
	             geometryNodes (),
	         previousSelection (),
	                  selector (SelectionType::BRUSH),
	             privateViewer (X3D::X3DConstants::X3DBaseNode),
	                   browser (getCurrentBrowser ()),
	         numSelectedPoints (0),
	          numSelectedEdges (0),
	          numSelectedFaces (0),
	                  changing (false)
{
	addChildren (normalEditor,
	             coordEditor,
	             geometryNodes,
	             previousSelection,
	             browser);

	normalEnabled .setUndo (false);

	normalEditor -> addUserDefinedField (X3D::inputOutput, "load",   new X3D::SFBool ());
	normalEditor -> addUserDefinedField (X3D::inputOutput, "color",  new X3D::SFColorRGBA (X3D::ToolColors::BLUE_RGBA));
	normalEditor -> addUserDefinedField (X3D::inputOutput, "length", new X3D::SFFloat (1));

	coordEditor -> addUserDefinedField (X3D::inputOutput, "selectionType",       new X3D::SFString ("POINTS"));
	coordEditor -> addUserDefinedField (X3D::inputOutput, "pickable",            new X3D::SFBool (true));
	coordEditor -> addUserDefinedField (X3D::inputOutput, "paintSelection",      new X3D::SFBool ());
	coordEditor -> addUserDefinedField (X3D::inputOutput, "mergePoints",         new X3D::SFTime ());
	coordEditor -> addUserDefinedField (X3D::inputOutput, "splitPoints",         new X3D::SFTime ());
	coordEditor -> addUserDefinedField (X3D::inputOutput, "chipSelectedOfFaces", new X3D::SFTime ());
	coordEditor -> addUserDefinedField (X3D::inputOutput, "removeSelectedFaces", new X3D::SFTime ());
	coordEditor -> addUserDefinedField (X3D::inputOutput, "color",               new X3D::SFColorRGBA (X3D::ToolColors::BLUE_RGBA));

	setup ();
}

void
GeometryEditor::configure ()
{
	X3DGeometryEditorInterface::configure ();

	normalEditor -> setField <X3D::SFBool> ("load", getConfig () -> get <X3D::SFBool> ("normalEnabled"), true);

	if (getConfig () -> hasItem ("normalLength"))
		normalEditor -> setField <X3D::SFFloat> ("length", getConfig () -> get <X3D::SFFloat> ("normalLength"));

	if (getConfig () -> hasItem ("normalColor"))
		normalEditor -> setField <X3D::SFColorRGBA> ("color", getConfig () -> get <X3D::SFColorRGBA> ("normalColor"));

	if (getConfig () -> hasItem ("edgeColor"))
		coordEditor -> setField <X3D::SFColorRGBA> ("color", getConfig () -> get <X3D::SFColorRGBA> ("edgeColor"));

	coordEditor -> setField <X3D::SFString> ("selectionType", getConfig () -> get <X3D::SFString> ("selectionType"));
	coordEditor -> setField <X3D::SFBool> ("paintSelection",  getConfig () -> get <X3D::SFBool> ("paintSelection"));

	if (getConfig () -> get <X3D::SFString> ("selectionType") == "FACES")
		getFacesMenuItem () .set_active (true);
	else if (getConfig () -> get <X3D::SFString> ("selectionType") == "EDGES")
		getEdgesMenuItem () .set_active (true);
	else
		getPointsMenuItem () .set_active (true);

	getPaintSelectionToggleButton () .set_active (getConfig () -> get <X3D::SFBool> ("paintSelection"));

	set_selector (SelectionType (getConfig () -> get <size_t> ("selector")));
}

void
GeometryEditor::initialize ()
{
	X3DGeometryEditorInterface::initialize ();

	getCurrentContext () .addInterest (this, &GeometryEditor::set_executionContext);

	normalEditor -> setup ();
	coordEditor  -> setup ();

	normalEnabled  .setNodes ({ normalEditor });
}

void
GeometryEditor::on_map ()
{
	getCurrentBrowser () .addInterest (this, &GeometryEditor::set_browser);

	set_browser (getCurrentBrowser ());
}

void
GeometryEditor::on_unmap ()
{
	getCurrentBrowser () -> getSelection () -> isEnabled () .removeInterest (this, &GeometryEditor::on_paint_selection_toggled);
	getCurrentBrowser () -> getViewer () .removeInterest (this, &GeometryEditor::set_viewer);
	getCurrentBrowser () -> getViewer () .removeInterest (this, &GeometryEditor::connectViewer);
	getCurrentBrowser () .removeInterest (this, &GeometryEditor::set_browser);
}

void
GeometryEditor::set_browser (const X3D::BrowserPtr & value)
{
	browser -> getSelection () -> isEnabled () .removeInterest (this, &GeometryEditor::on_paint_selection_toggled);
	browser -> getViewer () .removeInterest (this, &GeometryEditor::set_viewer);

	browser = value;

	browser -> getSelection () -> isEnabled () .addInterest (this, &GeometryEditor::on_paint_selection_toggled);
	browser -> getViewer () .addInterest (this, &GeometryEditor::set_viewer);

	set_viewer ();
}

void
GeometryEditor::set_executionContext ()
{
	try
	{
		const auto worldInfo   = getWorldInfo ();
		const auto metadataSet = worldInfo -> getMetaData <X3D::MetadataSet> ("/Titania/Selection");
		const auto children    = metadataSet -> getValue <X3D::MetadataSet> ("previous");

		children -> isPrivate (true);
		previousSelection = children -> value ();
	}
	catch (const std::exception & error)
	{
		previousSelection .clear ();
	}
}

void
GeometryEditor::set_viewer ()
{
	if (not getCurrentBrowser () -> getSelection () -> isEnabled ())
		return;

	changing = true;

	switch (getCurrentBrowser () -> getCurrentViewer ())
	{
		case X3D::X3DConstants::RectangleSelection:
		case X3D::X3DConstants::LassoSelection:
		{
			getPaintSelectionToggleButton () .set_active (true);
			break;
		}
		default:
		{
			privateViewer = browser-> getPrivateViewer ();
			getPaintSelectionToggleButton () .set_active (false);
			break;
		}
	}

	coordEditor -> setField <X3D::SFBool> ("pickable", not getPaintSelectionToggleButton () .get_active ());

	changing = false;
}

void
GeometryEditor::connectViewer ()
{
	getCurrentBrowser () -> getViewer () .removeInterest (this, &GeometryEditor::set_viewer);
	getCurrentBrowser () -> getViewer () .addInterest (this, &GeometryEditor::connectViewer);
}

void
GeometryEditor::set_selection (const X3D::MFNode & selection)
{
	X3DGeometryEditorInterface::set_selection (selection);

	changing = true;

	if (selection == geometryNodes and not geometryNodes .empty  ())
	{
		getEditToggleButton () .set_active (true);
	}
	else
	{
		const bool inScene       = not inPrototypeInstance ();
		const bool haveSelection = inScene and selection .size ();
		//const bool haveSelections = inScene and selection .size () > 1;

		geometryNodes = getNodes <X3D::X3DBaseNode> (selection, { X3D::X3DConstants::X3DPrototypeInstance, X3D::X3DConstants::IndexedFaceSet });

		if (selection not_eq geometryNodes)
		   previousSelection = selection;

		if (previousSelection == geometryNodes)
			previousSelection .clear ();

		getHammerButton ()     .set_sensitive (haveSelection);
		getEditToggleButton () .set_sensitive (not geometryNodes .empty ());
		getEditToggleButton () .set_active (selection == geometryNodes and not geometryNodes .empty  ());

		connect ();
	}

	changing = false;

	getGeometryToolsBox () .set_sensitive (getEditToggleButton () .get_active ());

	if (not getEditToggleButton () .get_active ())
		getPaintSelectionToggleButton () .set_active (false);
}

void
GeometryEditor::connect ()
{
	for (const auto & node : geometryNodes)
	{
		try
		{
			const auto innerNode = node -> getInnerNode ();

			for (const auto & type : basic::make_reverse_range (innerNode -> getType ()))
			{
				switch (type)
				{
					case X3D::X3DConstants::X3DGeometryNodeTool:
					{
						const auto & normalTool = innerNode -> getField <X3D::SFNode> ("normalTool");
						const auto & coordTool  = innerNode -> getField <X3D::SFNode> ("coordTool");

						normalEditor -> getField <X3D::SFBool>      ("load")   .addInterest (normalTool -> getField <X3D::SFBool>      ("load"));
						normalEditor -> getField <X3D::SFFloat>     ("length") .addInterest (normalTool -> getField <X3D::SFFloat>     ("length"));
						normalEditor -> getField <X3D::SFColorRGBA> ("color")  .addInterest (normalTool -> getField <X3D::SFColorRGBA> ("color"));

						normalTool -> setField <X3D::SFBool>      ("load",   normalEditor -> getField <X3D::SFBool>      ("load"),   true);
						normalTool -> setField <X3D::SFFloat>     ("length", normalEditor -> getField <X3D::SFFloat>     ("length"), true);
						normalTool -> setField <X3D::SFColorRGBA> ("color",  normalEditor -> getField <X3D::SFColorRGBA> ("color"),  true);

						// Coord

						coordEditor -> getField <X3D::SFString>    ("selectionType")       .addInterest (innerNode -> getField <X3D::SFString>    ("selectionType"));
						coordEditor -> getField <X3D::SFBool>      ("pickable")            .addInterest (innerNode -> getField <X3D::SFBool>      ("pickable"));
						coordEditor -> getField <X3D::SFBool>      ("paintSelection")      .addInterest (innerNode -> getField <X3D::SFBool>      ("paintSelection"));
						coordEditor -> getField <X3D::SFTime>      ("mergePoints")         .addInterest (innerNode -> getField <X3D::SFTime>      ("mergePoints"));
						coordEditor -> getField <X3D::SFTime>      ("splitPoints")         .addInterest (innerNode -> getField <X3D::SFTime>      ("splitPoints"));
						coordEditor -> getField <X3D::SFTime>      ("chipSelectedOfFaces") .addInterest (innerNode -> getField <X3D::SFTime>      ("chipSelectedOfFaces"));
						coordEditor -> getField <X3D::SFTime>      ("removeSelectedFaces") .addInterest (innerNode -> getField <X3D::SFTime>      ("removeSelectedFaces"));
						coordEditor -> getField <X3D::SFColorRGBA> ("color")               .addInterest (coordTool -> getField <X3D::SFColorRGBA> ("color"));

						innerNode -> getField <X3D::SFInt32>              ("selectedPoints_changed") .addInterest (this, &GeometryEditor::set_selectedPoints);
						innerNode -> getField <X3D::SFInt32>              ("selectedEdges_changed")  .addInterest (this, &GeometryEditor::set_selectedEdges);
						innerNode -> getField <X3D::SFInt32>              ("selectedFaces_changed")  .addInterest (this, &GeometryEditor::set_selectedFaces);
						innerNode -> getField <X3D::UndoStepContainerPtr> ("undo_changed")           .addInterest (this, &GeometryEditor::set_undo);

						coordTool -> setField <X3D::SFBool>      ("load",             true,                                                          true);
						coordTool -> setField <X3D::SFColorRGBA> ("color",            coordEditor -> getField <X3D::SFColorRGBA> ("color"),          true);
						innerNode -> setField <X3D::SFString>    ("selectionType",    coordEditor -> getField <X3D::SFString>    ("selectionType"),  true);
						innerNode -> setField <X3D::SFBool>      ("pickable",         coordEditor -> getField <X3D::SFBool>      ("pickable"),       true);
						innerNode -> setField <X3D::SFBool>      ("paintSelection",   coordEditor -> getField <X3D::SFBool>      ("paintSelection"), true);

						break;
					}
					default:
						continue;
				}

				break;
			}
		}
		catch (const X3D::X3DError & error)
		{
			__LOG__ << error .what () << std::endl;
		}
	}

	set_selectedFaces ();
}

void
GeometryEditor::set_undo (const X3D::UndoStepContainerPtr & container)
{
	getBrowserWindow () -> addUndoStep (container -> getUndoStep ());
}

void
GeometryEditor::set_selectedPoints ()
{
	numSelectedPoints = 0;

	for (const auto & node : geometryNodes)
	{
		try
		{
			const auto innerNode = node -> getInnerNode ();

			for (const auto & type : basic::make_reverse_range (innerNode -> getType ()))
			{
				switch (type)
				{
					case X3D::X3DConstants::X3DGeometryNodeTool:
					{
						numSelectedPoints += innerNode -> getField <X3D::SFInt32> ("selectedPoints_changed") .getValue ();
					   break;
					}
					default:
						continue;
				}

				break;
			}
		}
		catch (const X3D::X3DError & error)
		{
			__LOG__ << error .what () << std::endl;
		}
	}

	getMergePointsButton () .set_sensitive (numSelectedPoints);
	getSplitPointsButton () .set_sensitive (numSelectedPoints);

	set_face_selection ();
}


void
GeometryEditor::set_selectedEdges ()
{
	numSelectedEdges = 0;

	for (const auto & node : geometryNodes)
	{
		try
		{
			const auto innerNode = node -> getInnerNode ();

			for (const auto & type : basic::make_reverse_range (innerNode -> getType ()))
			{
				switch (type)
				{
					case X3D::X3DConstants::X3DGeometryNodeTool:
					{
						numSelectedEdges += innerNode -> getField <X3D::SFInt32> ("selectedEdges_changed") .getValue ();
					   break;
					}
					default:
						continue;
				}

				break;
			}
		}
		catch (const X3D::X3DError & error)
		{
			__LOG__ << error .what () << std::endl;
		}
	}

	set_face_selection ();
}

void
GeometryEditor::set_selectedFaces ()
{
	numSelectedFaces = 0;

	for (const auto & node : geometryNodes)
	{
		try
		{
			const auto innerNode = node -> getInnerNode ();

			for (const auto & type : basic::make_reverse_range (innerNode -> getType ()))
			{
				switch (type)
				{
					case X3D::X3DConstants::X3DGeometryNodeTool:
					{
						numSelectedFaces += innerNode -> getField <X3D::SFInt32> ("selectedFaces_changed") .getValue ();
					   break;
					}
					default:
						continue;
				}

				break;
			}
		}
		catch (const X3D::X3DError & error)
		{
			__LOG__ << error .what () << std::endl;
		}
	}

	getChipOfFacesButton () .set_sensitive (numSelectedFaces);
	getRemoveFacesButton () .set_sensitive (numSelectedFaces);

	set_face_selection ();
}

void
GeometryEditor::set_face_selection ()
{
	#ifdef DEBUG
	// Set description.

	std::ostringstream ostream;

	ostream
		<< "Selected points: " << numSelectedPoints << std::endl
		<< "Selected edges: " << numSelectedEdges << std::endl
		<< "Selected faces: " << numSelectedFaces;

	getCurrentBrowser () -> setDescription (ostream .str ());
	#endif
}

void
GeometryEditor::on_hammer_clicked ()
{
	const auto undoStep  = std::make_shared <X3D::UndoStep> (_ ("Smash Selection"));
	auto       selection = getBrowserWindow () -> getSelection () -> getChildren ();

	for (const auto & shape : getNodes <X3D::X3DShapeNode> (selection, { X3D::X3DConstants::X3DShapeNode }))
	{
		if (not shape -> geometry ())
			continue;

		for (const auto & type : basic::make_reverse_range (shape -> geometry () -> getType ()))
		{
			switch (type)
			{
				case X3D::X3DConstants::X3DPrototypeInstance :
				{
					try
					{
						const X3D::X3DPtr <X3D::X3DGeometryNode> geometry (shape -> geometry () -> getInnerNode ());

						if (geometry)
						{
							X3D::MFNode        exports ({ geometry });
							basic::ifilestream text (getBrowserWindow () -> exportNodes (getCurrentContext (), exports));

							const auto scene = getCurrentBrowser () -> createX3DFromStream (getCurrentContext () -> getWorldURL (), text);
							const auto nodes = getBrowserWindow () -> importScene (getCurrentContext (), X3D::SFNode (getCurrentContext ()), getCurrentContext () -> getRootNodes (), scene, undoStep);

							getBrowserWindow () -> addToGroup (getCurrentContext (), X3D::SFNode (shape), nodes, undoStep);
						}
					}
					catch (const X3D::X3DError &)
					{ }

					break;
				}
				case X3D::X3DConstants::X3DGeometryNode:
				{
					try
					{
						const X3D::X3DPtr <X3D::X3DGeometryNode> geometry (shape -> geometry ());

						getBrowserWindow () -> replaceNode (getCurrentContext (), X3D::SFNode (shape), shape -> geometry (), geometry -> toPrimitive (), undoStep);
					}
					catch (const X3D::X3DError &)
					{ }

					break;
				}
				default:
					continue;
			}

			break;
		}
	}

	getBrowserWindow () -> getSelection () -> setChildren (X3D::MFNode (), undoStep);
	getBrowserWindow () -> getSelection () -> setChildren (selection,      undoStep);

	addUndoStep (undoStep);
}

void
GeometryEditor::on_edit_toggled ()
{
	if (changing)
		return;

	if (true)
	{
		const auto worldInfo = createWorldInfo ();

		if (getEditToggleButton () .get_active ())
		{
			const auto metadataSet = worldInfo -> createMetaData <X3D::MetadataSet> ("/Titania/Selection");
			const auto children    = metadataSet -> createValue <X3D::MetadataSet> ("previous");

			children -> isPrivate (true);
			children -> value () = previousSelection;
		}
		else
			worldInfo -> removeMetaData ("/Titania/Selection/previous");
	}

	if (getEditToggleButton () .get_active ())
		getBrowserWindow () -> getSelection () -> setChildren (geometryNodes);
	else
		getBrowserWindow () -> getSelection () -> setChildren (previousSelection);
}

void
GeometryEditor::on_paint_selection_toggled ()
{
	if (not getCurrentBrowser () -> getSelection () -> isEnabled ())
		return;

	if (changing)
		return;

	switch (selector)
	{
		case SelectionType::BRUSH:
		{
			coordEditor -> setField <X3D::SFBool> ("paintSelection", getPaintSelectionToggleButton () .get_active ());

			getCurrentBrowser () -> setPrivateViewer (privateViewer);
			coordEditor -> setField <X3D::SFBool> ("pickable", true);

			getCurrentBrowser () -> getViewer () .removeInterest (this, &GeometryEditor::set_viewer);
			getCurrentBrowser () -> getViewer () .addInterest (this, &GeometryEditor::connectViewer);
			break;
		}
		case SelectionType::RECTANGLE:
		{
			coordEditor -> setField <X3D::SFBool> ("paintSelection", false);

			if (getPaintSelectionToggleButton () .get_active ())
				getCurrentBrowser () -> setPrivateViewer (X3D::X3DConstants::RectangleSelection);
			else
				getCurrentBrowser () -> setPrivateViewer (privateViewer);

			coordEditor -> setField <X3D::SFBool> ("pickable", not getPaintSelectionToggleButton () .get_active ());
			break;
		}
		case SelectionType::LASSO:
		{
			coordEditor -> setField <X3D::SFBool> ("paintSelection", false);

			if (getPaintSelectionToggleButton () .get_active ())
				getCurrentBrowser () -> setPrivateViewer (X3D::X3DConstants::LassoSelection);
			else
				getCurrentBrowser () -> setPrivateViewer (privateViewer);

			coordEditor -> setField <X3D::SFBool> ("pickable", not getPaintSelectionToggleButton () .get_active ());
			break;
		}
	}
}

bool
GeometryEditor::on_selection_type_button_press_event (GdkEventButton* event)
{
	if (event -> button not_eq 3)
		return false;

	getSelectionTypeMenu () .popup (event -> button, event -> time);
	return true;
}

void
GeometryEditor::on_brush_activated ()
{
	set_selector (SelectionType::BRUSH);
}

void
GeometryEditor::on_rectangle_activated ()
{
	set_selector (SelectionType::RECTANGLE);
}

void
GeometryEditor::on_lasso_activated ()
{
	set_selector (SelectionType::LASSO);
}

void
GeometryEditor::set_selector (const SelectionType & type)
{
	selector = type;

	switch (selector)
	{
		case SelectionType::BRUSH:
			set_selection_brush ();
			break;
		case SelectionType::RECTANGLE:
			set_selection_rectangle ();
			break;
		case SelectionType::LASSO:
			set_selection_lasso ();
			break;
	}
}

void
GeometryEditor::set_selection_brush ()
{
	getPaintSelectionToggleButton () .set_tooltip_text (_ ("Paint current selection."));
	getPaintSelectionImage () .set (Gtk::StockID ("Brush"), Gtk::IconSize (Gtk::ICON_SIZE_MENU));

	on_paint_selection_toggled ();
}

void
GeometryEditor::set_selection_rectangle ()
{
	getPaintSelectionToggleButton () .set_tooltip_text (_ ("Use rectangle selection."));
	getPaintSelectionImage () .set (Gtk::StockID ("RectangleSelection"), Gtk::IconSize (Gtk::ICON_SIZE_MENU));

	on_paint_selection_toggled ();
}

void
GeometryEditor::set_selection_lasso ()
{
	getPaintSelectionToggleButton () .set_tooltip_text (_ ("Use lasso selection."));
	getPaintSelectionImage () .set (Gtk::StockID ("Lasso"), Gtk::IconSize (Gtk::ICON_SIZE_MENU));

	on_paint_selection_toggled ();
}

void
GeometryEditor::on_points_toggled ()
{
	if (getPointsMenuItem () .get_active ())
		coordEditor -> setField <X3D::SFString> ("selectionType", "POINTS");
}

void
GeometryEditor::on_edges_toggled ()
{
	if (getEdgesMenuItem () .get_active ())
		coordEditor -> setField <X3D::SFString> ("selectionType", "EDGES");
}
void
GeometryEditor::on_faces_toggled ()
{
	if (getFacesMenuItem () .get_active ())
		coordEditor -> setField <X3D::SFString> ("selectionType", "FACES");
}

void
GeometryEditor::on_merge_points_clicked ()
{
	coordEditor -> setField <X3D::SFTime> ("mergePoints", chrono::now ());
}

void
GeometryEditor::on_split_points_clicked ()
{
	coordEditor -> setField <X3D::SFTime> ("splitPoints", chrono::now ());
}

void
GeometryEditor::on_chip_of_face_clicked ()
{
	coordEditor -> setField <X3D::SFTime> ("chipSelectedOfFaces", chrono::now ());
}

void
GeometryEditor::on_remove_selected_faces_clicked ()
{
	coordEditor -> setField <X3D::SFTime> ("removeSelectedFaces", chrono::now ());
}

void
GeometryEditor::store ()
{
	getConfig () -> set ("normalEnabled",   normalEditor -> getField <X3D::SFBool>      ("load"));
	getConfig () -> set ("normalLength",    normalEditor -> getField <X3D::SFFloat>     ("length"));
	getConfig () -> set ("normalColor",     normalEditor -> getField <X3D::SFColorRGBA> ("color"));
	getConfig () -> set ("selectionType",   coordEditor  -> getField <X3D::SFString>    ("selectionType"));
	getConfig () -> set ("edgeColor",       coordEditor  -> getField <X3D::SFColorRGBA> ("color"));
	getConfig () -> set ("selector",        size_t (selector));

	X3DGeometryEditorInterface::store ();
}

GeometryEditor::~GeometryEditor ()
{
	dispose ();
}

} // puck
} // titania
