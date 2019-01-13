/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstraße 31a, Leipzig, Germany 2011.
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

#include "GeometryEditor.h"

#include "../../Browser/BrowserSelection.h"
#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"

#include <Titania/X3D/Bits/Cast.h>
#include <Titania/X3D/Browser/Core/Clipboard.h>
#include <Titania/X3D/Components/Rendering/X3DGeometryNode.h>
#include <Titania/X3D/Components/Shape/X3DShapeNode.h>
#include <Titania/X3D/Components/Shape/X3DShapeNode.h>
#include <Titania/X3D/Tools/ToolColors.h>

namespace titania {
namespace puck {

GeometryEditor::GeometryEditor (X3DBrowserWindow* const browserWindow) :
	          X3DBaseInterface (browserWindow, browserWindow -> getCurrentBrowser ()),
	X3DGeometryEditorInterface (get_ui ("Revealer/GeometryEditor.glade")),
	             normalEnabled (this, getNormalEnabledButton (), "load"),
	              normalEditor (new X3D::FieldSet (getMasterBrowser ())),
	               coordEditor (new X3D::FieldSet (getMasterBrowser ())),
	                   browser (getCurrentBrowser ()),
	             privateViewer (X3D::X3DConstants::DefaultViewer),
	                  selector (SelectorType::BRUSH),
	         numSelectedPoints (0),
	          numSelectedEdges (0),
	          numSelectedHoles (0),
	          numSelectedFaces (0),
	                  copyTime (0),
	                  changing (false)
{
	addChildObjects (normalEditor,
	                 coordEditor,
	                 browser);

	normalEnabled .setUndo (false);

	normalEditor -> addUserDefinedField (X3D::inputOutput, "load",   new X3D::SFBool ());
	normalEditor -> addUserDefinedField (X3D::inputOutput, "color",  new X3D::SFColorRGBA (X3D::ToolColors::BLUE_RGBA));
	normalEditor -> addUserDefinedField (X3D::inputOutput, "length", new X3D::SFFloat (1));

	coordEditor -> addUserDefinedField (X3D::inputOutput, "selectionType",          new X3D::SFString ("POINTS"));
	coordEditor -> addUserDefinedField (X3D::inputOutput, "toolType",               new X3D::SFString ("NONE"));
	coordEditor -> addUserDefinedField (X3D::inputOutput, "paintSelection",         new X3D::SFBool ());
	coordEditor -> addUserDefinedField (X3D::inputOutput, "selectLineLoop",         new X3D::SFBool ());
	coordEditor -> addUserDefinedField (X3D::inputOutput, "transform",              new X3D::SFBool ());
	coordEditor -> addUserDefinedField (X3D::inputOutput, "axisAlignedBoundingBox", new X3D::SFBool ());
	coordEditor -> addUserDefinedField (X3D::inputOutput, "mergePoints",            new X3D::SFTime ());
	coordEditor -> addUserDefinedField (X3D::inputOutput, "splitPoints",            new X3D::SFTime ());
	coordEditor -> addUserDefinedField (X3D::inputOutput, "formNewFace",            new X3D::SFTime ());
	coordEditor -> addUserDefinedField (X3D::inputOutput, "extrudeSelectedEdges",   new X3D::SFTime ());
	coordEditor -> addUserDefinedField (X3D::inputOutput, "extrudeSelectedFaces",   new X3D::SFTime ());
	coordEditor -> addUserDefinedField (X3D::inputOutput, "chipOfSelectedFaces",    new X3D::SFTime ());
	coordEditor -> addUserDefinedField (X3D::inputOutput, "deleteSelectedFaces",    new X3D::SFTime ());
	coordEditor -> addUserDefinedField (X3D::inputOutput, "flipVertexOrdering",     new X3D::SFTime ());
	coordEditor -> addUserDefinedField (X3D::inputOutput, "cutSnapping",            new X3D::SFBool ());
	coordEditor -> addUserDefinedField (X3D::inputOutput, "primitiveColor",         new X3D::SFColorRGBA (X3D::ToolColors::DARK_BLUE_RGBA));
	coordEditor -> addUserDefinedField (X3D::inputOutput, "color",                  new X3D::SFColorRGBA (X3D::ToolColors::BLUE_RGBA));

	getBrowserWindow () -> getHandButton ()   .signal_toggled () .connect (sigc::mem_fun (this, &GeometryEditor::on_hand_toggled));
	getBrowserWindow () -> getArrowButton ()  .signal_toggled () .connect (sigc::mem_fun (this, &GeometryEditor::on_arrow_toggled));

	auto selectionGroup = getBrowserWindow () -> getHandButton () .get_group ();

	getPaintSelectionButton () .set_group (selectionGroup);
	getCutPolygonsButton ()    .set_group (selectionGroup);

	setup ();
}

void
GeometryEditor::initialize ()
{
	X3DGeometryEditorInterface::initialize ();

	getBrowserWindow () -> getSelection () -> getSelectGeometry () .addInterest (&GeometryEditor::set_selectGeometry, this);

	normalEditor -> setup ();
	coordEditor  -> setup ();

	normalEnabled .setNodes ({ normalEditor });

	set_selectGeometry ();
}

void
GeometryEditor::configure ()
{
	X3DGeometryEditorInterface::configure ();

	changing = true;
	set_selector (SelectorType (getConfig () -> getItem <size_t> ("selector")));
	changing = false;

	normalEditor -> setField <X3D::SFBool> ("load", getConfig () -> getItem <X3D::SFBool> ("normalEnabled"), true);

	if (getConfig () -> hasItem ("normalLength"))
		normalEditor -> setField <X3D::SFFloat> ("length", getConfig () -> getItem <X3D::SFFloat> ("normalLength"));

	if (getConfig () -> hasItem ("normalColor"))
		normalEditor -> setField <X3D::SFColorRGBA> ("color", getConfig () -> getItem <X3D::SFColorRGBA> ("normalColor"));

	if (getConfig () -> hasItem ("edgeColor"))
		coordEditor -> setField <X3D::SFColorRGBA> ("color", getConfig () -> getItem <X3D::SFColorRGBA> ("edgeColor"));

	coordEditor -> setField <X3D::SFString> ("selectionType", getConfig () -> getItem <X3D::SFString> ("selectionType"));

	if (getConfig () -> getItem <X3D::SFString> ("selectionType") == "FACES")
		getFacesMenuItem () .set_active (true);
	else if (getConfig () -> getItem <X3D::SFString> ("selectionType") == "EDGES")
		getEdgesMenuItem () .set_active (true);
	else
		getPointsMenuItem () .set_active (true);

	//getPaintSelectionButton ()              .set_active (getConfig () -> getItem <bool> ("paintSelection"));
	getSelectLineLoopMenuItem ()            .set_active (getConfig () -> getItem <bool> ("selectLineLoop"));
	getTransformToolButton ()               .set_active (getConfig () -> getItem <bool> ("transform"));
	getAxisAlignedBoundingBoxMenuItem ()    .set_active (getConfig () -> getItem <bool> ("axisAlignedBoundingBox") or not getConfig () -> hasItem ("axisAlignedBoundingBox"));
	//getCutPolygonsButton ()                 .set_active (getConfig () -> getItem <bool> ("cutPolygons"));
	getCutPolygonsEnableSnappingMenuItem () .set_active (getConfig () -> getItem <bool> ("cutSnapping") or not getConfig () -> hasItem ("cutSnapping"));
}

void
GeometryEditor::on_map ()
{
	getCurrentBrowser () .addInterest (&GeometryEditor::set_browser, this);

	getBrowserWindow () -> getSelection () -> getNodes ()      .addInterest (&GeometryEditor::set_geometries, this);
	getBrowserWindow () -> getSelection () -> getGeometries () .addInterest (&GeometryEditor::set_geometries, this);

	set_browser (getCurrentBrowser ());
	set_geometries (getBrowserWindow () -> getSelection () -> getNodes ());
	set_geometries (getBrowserWindow () -> getSelection () -> getGeometries ());
}

void
GeometryEditor::on_unmap ()
{
	getCurrentBrowser () -> getViewer () .removeInterest (&GeometryEditor::set_viewer, this);
	getCurrentBrowser () -> getViewer () .removeInterest (&GeometryEditor::connectViewer, this);
	getCurrentBrowser () .removeInterest (&GeometryEditor::set_browser, this);

	getBrowserWindow () -> getSelection () -> getGeometries () .removeInterest (&GeometryEditor::set_geometries, this);

	browser = getMasterBrowser ();
}

void
GeometryEditor::set_browser (const X3D::BrowserPtr & value)
{
	browser -> getViewer () .removeInterest (&GeometryEditor::set_viewer, this);

	browser = value;

	browser -> getViewer () .addInterest (&GeometryEditor::set_viewer, this);
}

void
GeometryEditor::set_selection (const X3D::MFNode & selection)
{
	X3DGeometryEditorInterface::set_selection (selection);

	const auto shapeNodes = getNodes <X3D::X3DShapeNode> (selection, { X3D::X3DConstants::X3DShapeNode });

	getHammerButton () .set_sensitive (not inPrototypeInstance () and not shapeNodes .empty ());
}

void
GeometryEditor::set_geometries (const X3D::MFNode & geometryNodes)
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
					case X3D::X3DConstants::IndexedFaceSetTool:
					{
						if (node == innerNode)
						{
							// Coord
	
							coordEditor -> getField <X3D::SFString> ("toolType")               .addInterest (node -> getField <X3D::SFString> ("toolType"));
							coordEditor -> getField <X3D::SFString> ("selectionType")          .addInterest (node -> getField <X3D::SFString> ("selectionType"));
							coordEditor -> getField <X3D::SFBool>   ("paintSelection")         .addInterest (node -> getField <X3D::SFBool>   ("paintSelection"));
							coordEditor -> getField <X3D::SFBool>   ("selectLineLoop")         .addInterest (node -> getField <X3D::SFBool>   ("selectLineLoop"));
							coordEditor -> getField <X3D::SFBool>   ("transform")              .addInterest (node -> getField <X3D::SFBool>   ("transform"));
							coordEditor -> getField <X3D::SFBool>   ("axisAlignedBoundingBox") .addInterest (node -> getField <X3D::SFBool>   ("axisAlignedBoundingBox"));
							coordEditor -> getField <X3D::SFTime>   ("mergePoints")            .addInterest (node -> getField <X3D::SFTime>   ("mergePoints"));
							coordEditor -> getField <X3D::SFTime>   ("splitPoints")            .addInterest (node -> getField <X3D::SFTime>   ("splitPoints"));
							coordEditor -> getField <X3D::SFTime>   ("formNewFace")            .addInterest (node -> getField <X3D::SFTime>   ("formNewFace"));
							coordEditor -> getField <X3D::SFTime>   ("extrudeSelectedEdges")   .addInterest (node -> getField <X3D::SFTime>   ("extrudeSelectedEdges"));
							coordEditor -> getField <X3D::SFTime>   ("extrudeSelectedFaces")   .addInterest (node -> getField <X3D::SFTime>   ("extrudeSelectedFaces"));
							coordEditor -> getField <X3D::SFTime>   ("chipOfSelectedFaces")    .addInterest (node -> getField <X3D::SFTime>   ("chipOfSelectedFaces"));
							coordEditor -> getField <X3D::SFTime>   ("flipVertexOrdering")     .addInterest (node -> getField <X3D::SFTime>   ("flipVertexOrdering"));
							coordEditor -> getField <X3D::SFTime>   ("deleteSelectedFaces")    .addInterest (node -> getField <X3D::SFTime>   ("deleteSelectedFaces"));
							coordEditor -> getField <X3D::SFBool>   ("cutSnapping")            .addInterest (node -> getField <X3D::SFBool>   ("cutSnapping"));

							node -> getField <X3D::SFInt32>  ("selectedPoints_changed") .addInterest (&GeometryEditor::set_selectedPoints, this);
							node -> getField <X3D::SFInt32>  ("selectedEdges_changed")  .addInterest (&GeometryEditor::set_selectedEdges,  this);
							node -> getField <X3D::SFInt32>  ("selectedHoles_changed")  .addInterest (&GeometryEditor::set_selectedHoles,  this);
							node -> getField <X3D::SFInt32>  ("selectedFaces_changed")  .addInterest (&GeometryEditor::set_selectedFaces,  this);
							node -> getField <X3D::SFString> ("clipboard_changed")      .addInterest (&GeometryEditor::set_clipboard,      this);
	
							node -> setField <X3D::SFString> ("toolType",               coordEditor -> getField <X3D::SFString> ("toolType"),               true);
							node -> setField <X3D::SFString> ("selectionType",          coordEditor -> getField <X3D::SFString> ("selectionType"),          true);
							node -> setField <X3D::SFBool>   ("paintSelection",         coordEditor -> getField <X3D::SFBool>   ("paintSelection"),         true);
							node -> setField <X3D::SFBool>   ("selectLineLoop",         coordEditor -> getField <X3D::SFBool>   ("selectLineLoop"),         true);
							node -> setField <X3D::SFBool>   ("transform",              coordEditor -> getField <X3D::SFBool>   ("transform"),              true);
							node -> setField <X3D::SFBool>   ("axisAlignedBoundingBox", coordEditor -> getField <X3D::SFBool>   ("axisAlignedBoundingBox"), true);
							node -> setField <X3D::SFBool>   ("cutSnapping",            coordEditor -> getField <X3D::SFBool>   ("cutSnapping"),            true);
						}

						[[fallthrough]];
					}
					case X3D::X3DConstants::IndexedQuadSetTool:
					case X3D::X3DConstants::QuadSetTool:
					case X3D::X3DConstants::Arc2DTool:
					case X3D::X3DConstants::ArcClose2DTool:
					case X3D::X3DConstants::Circle2DTool:
					case X3D::X3DConstants::Disk2DTool:
					case X3D::X3DConstants::Polyline2DTool:
					case X3D::X3DConstants::Polypoint2DTool:
					case X3D::X3DConstants::Rectangle2DTool:
					case X3D::X3DConstants::TriangleSet2DTool:
					case X3D::X3DConstants::BoxTool:
					case X3D::X3DConstants::ConeTool:
					case X3D::X3DConstants::CylinderTool:
					case X3D::X3DConstants::ElevationGridTool:
					case X3D::X3DConstants::ExtrusionTool:
					case X3D::X3DConstants::SphereTool:
					case X3D::X3DConstants::GeoElevationGridTool:
					case X3D::X3DConstants::NurbsCurveTool:
					case X3D::X3DConstants::NurbsPatchSurfaceTool:
					case X3D::X3DConstants::NurbsSweptSurfaceTool:
					case X3D::X3DConstants::NurbsSwungSurfaceTool:
					case X3D::X3DConstants::NurbsTrimmedSurfaceTool:
					case X3D::X3DConstants::IndexedLineSetTool:
					case X3D::X3DConstants::IndexedTriangleFanSetTool:
					case X3D::X3DConstants::IndexedTriangleSetTool:
					case X3D::X3DConstants::IndexedTriangleStripSetTool:
					case X3D::X3DConstants::LineSetTool:
					case X3D::X3DConstants::PointSetTool:
					case X3D::X3DConstants::TriangleFanSetTool:
					case X3D::X3DConstants::TriangleSetTool:
					case X3D::X3DConstants::TriangleStripSetTool:
					case X3D::X3DConstants::TextTool:
					{
						const auto & normalTool = innerNode -> getField <X3D::SFNode> ("normalTool");
						const auto & coordTool  = innerNode -> getField <X3D::SFNode> ("coordTool");

						// If the node has a normal or creaseAngle field use light blue as normal color, otherwise dark blue.
						const auto editableNormals = innerNode -> isType ({
							X3D::X3DConstants::ElevationGrid,
							X3D::X3DConstants::Extrusion,
							X3D::X3DConstants::GeoElevationGrid,
							X3D::X3DConstants::X3DComposedGeometryNode,
						});

						const auto editable = node -> isType ({
							X3D::X3DConstants::IndexedFaceSet,
							//X3D::X3DConstants::X3DNurbsSurfaceGeometryNode,
						});

						// Normal

						normalEditor -> getField <X3D::SFBool>  ("load")   .addInterest (normalTool -> getField <X3D::SFBool>  ("load"));
						normalEditor -> getField <X3D::SFFloat> ("length") .addInterest (normalTool -> getField <X3D::SFFloat> ("length"));

						normalTool -> setField <X3D::SFBool>  ("load",   normalEditor -> getField <X3D::SFBool>  ("load"),   true);
						normalTool -> setField <X3D::SFFloat> ("length", normalEditor -> getField <X3D::SFFloat> ("length"), true);

						if (editableNormals)
						{
							normalEditor -> getField <X3D::SFColorRGBA> ("color") .addInterest (normalTool -> getField <X3D::SFColorRGBA> ("color"));
							normalTool -> setField <X3D::SFColorRGBA> ("color", normalEditor -> getField <X3D::SFColorRGBA> ("color"), true);
						}
						else
						{
							coordEditor -> getField <X3D::SFColorRGBA> ("primitiveColor") .addInterest (normalTool -> getField <X3D::SFColorRGBA> ("color"));
							normalTool -> setField <X3D::SFColorRGBA> ("color", coordEditor -> getField <X3D::SFColorRGBA> ("primitiveColor"),  true);
						}

						// Coord

						if (editable)
						{
							coordEditor -> getField <X3D::SFColorRGBA> ("color") .addInterest (coordTool -> getField <X3D::SFColorRGBA> ("color"));
							coordTool -> setField <X3D::SFColorRGBA> ("color", coordEditor -> getField <X3D::SFColorRGBA> ("color"), true);
						}
						else
						{
							coordEditor -> getField <X3D::SFColorRGBA> ("primitiveColor") .addInterest (coordTool -> getField <X3D::SFColorRGBA> ("color"));
							coordTool -> setField <X3D::SFColorRGBA> ("color", coordEditor -> getField <X3D::SFColorRGBA> ("primitiveColor"), true);
						}

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

	set_selectedPoints ();
	set_selectedEdges  ();
	set_selectedHoles  ();
	set_selectedFaces  ();
}

void
GeometryEditor::set_viewer ()
{
	if (not getBrowserWindow () -> getSelection () -> getEnabled ())
		return;

	switch (getCurrentBrowser () -> getCurrentViewer ())
	{
		case X3D::X3DConstants::RectangleSelection:
		case X3D::X3DConstants::LassoSelection:
		case X3D::X3DConstants::LightSaber:
		case X3D::X3DConstants::LookAtViewer:
			break;
		default:
		{
			privateViewer = browser-> getViewerType ();
			break;
		}
	}
}

void
GeometryEditor::connectViewer ()
{
	getCurrentBrowser () -> getViewer () .removeInterest (&GeometryEditor::connectViewer, this);
	getCurrentBrowser () -> getViewer () .addInterest (&GeometryEditor::set_viewer, this);
}

bool
GeometryEditor::on_cut ()
{
	if (getBrowserWindow () -> getSelection () -> getSelectGeometry ())
	{
		const auto & geometryNodes = getBrowserWindow () -> getSelection () -> getGeometries ();

		try
		{
			geometryNodes .back () -> setField <X3D::SFTime> ("cutGeometry", X3D::SFTime::now ());
		}
		catch (const X3D::X3DError &)
		{ }

		return true;
	}

	return false;
}

bool
GeometryEditor::on_copy ()
{
	if (getBrowserWindow () -> getSelection () -> getSelectGeometry ())
	{
		const auto & geometryNodes = getBrowserWindow () -> getSelection () -> getGeometries ();

		for (const auto & geometryNode : geometryNodes)
		{
			try
			{
				geometryNode -> setField <X3D::SFTime> ("copyGeometry", X3D::SFTime::now ());
			}
			catch (const X3D::X3DError &)
			{ }
		}

		return true;
	}

	return false;
}

bool
GeometryEditor::on_paste ()
{
	if (getBrowserWindow () -> getSelection () -> getSelectGeometry ())
	{
		const auto & geometryNodes = getBrowserWindow () -> getSelection () -> getGeometries ();

		const auto active = std::max_element (geometryNodes .begin (), geometryNodes .end (), [ ] (const X3D::SFNode & lhs, const X3D::SFNode & rhs)
		{
			X3D::time_type timeL = -1;
			X3D::time_type timeR = -1;

			try
			{
				timeL = lhs -> getField <X3D::SFTime> ("touchTime");
			}
			catch (const X3D::X3DError &)
			{ }

			try
			{
				timeR = rhs -> getField <X3D::SFTime> ("touchTime");
			}
			catch (const X3D::X3DError &)
			{ }

			return timeL < timeR;
		});

		if (active not_eq geometryNodes .end ())
		{
			try
			{
				(*active) -> setField <X3D::SFString> ("pasteGeometry", getBrowserWindow () -> getClipboard () -> string_changed ());
			}
			catch (const X3D::X3DError &)
			{ }
		}

		return true;
	}

	return false;
}

bool
GeometryEditor::on_delete ()
{
	if (getBrowserWindow () -> getSelection () -> getSelectGeometry ())
	{
		on_delete_selected_faces_clicked ();
		return true;
	}

	return false;
}

bool
GeometryEditor::on_select_all ()
{
	if (getBrowserWindow () -> getSelection () -> getSelectGeometry ())
	{
		const auto & geometryNodes = getBrowserWindow () -> getSelection () -> getGeometries ();

		for (const auto & node : geometryNodes)
		{
			try
			{
				const auto innerNode = node -> getInnerNode ();

				for (const auto & type : basic::make_reverse_range (innerNode -> getType ()))
				{
					switch (type)
					{
						case X3D::X3DConstants::IndexedFaceSetTool:
						{
							innerNode -> getField <X3D::SFTime> ("selectAll") = X3D::SFTime::now ();
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

		return true;
	}

	return false;
}

bool
GeometryEditor::on_deselect_all ()
{
	if (getBrowserWindow () -> getSelection () -> getSelectGeometry ())
	{
		const auto & geometryNodes = getBrowserWindow () -> getSelection () -> getGeometries ();

		for (const auto & node : geometryNodes)
		{
			try
			{
				const auto innerNode = node -> getInnerNode ();

				for (const auto & type : basic::make_reverse_range (innerNode -> getType ()))
				{
					switch (type)
					{
						case X3D::X3DConstants::IndexedFaceSetTool:
						{
							innerNode -> getField <X3D::SFTime> ("deselectAll") = X3D::SFTime::now ();
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

		return true;
	}

	return false;
}

void
GeometryEditor::set_clipboard (const X3D::SFString & string)
{
	if (copyTime not_eq getCurrentBrowser () -> getCurrentTime ())
	{
		copyTime = getCurrentBrowser () -> getCurrentTime ();

		getBrowserWindow () -> getClipboard () -> set_string () .clear ();
	}

	getBrowserWindow () -> getClipboard () -> set_string () .append (string);
}

void
GeometryEditor::set_selectedPoints ()
{
	numSelectedPoints = 0;

	const auto & geometryNodes = getBrowserWindow () -> getSelection () -> getGeometries ();

	for (const auto & node : geometryNodes)
	{
		try
		{
			const auto innerNode = node -> getInnerNode ();

			for (const auto & type : basic::make_reverse_range (innerNode -> getType ()))
			{
				switch (type)
				{
					case X3D::X3DConstants::IndexedFaceSetTool:
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

	getMergePointsButton () .set_sensitive (numSelectedPoints > 1);
	getSplitPointsButton () .set_sensitive (numSelectedPoints);
}

void
GeometryEditor::set_selectedEdges ()
{
	numSelectedEdges = 0;

	const auto & geometryNodes = getBrowserWindow () -> getSelection () -> getGeometries ();

	for (const auto & node : geometryNodes)
	{
		try
		{
			const auto innerNode = node -> getInnerNode ();

			for (const auto & type : basic::make_reverse_range (innerNode -> getType ()))
			{
				switch (type)
				{
					case X3D::X3DConstants::IndexedFaceSetTool:
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

	getExtrudeSelectedEdgesButton () .set_sensitive (numSelectedEdges);
}

void
GeometryEditor::set_selectedHoles ()
{
	numSelectedHoles = 0;

	const auto & geometryNodes = getBrowserWindow () -> getSelection () -> getGeometries ();

	for (const auto & node : geometryNodes)
	{
		try
		{
			const auto innerNode = node -> getInnerNode ();

			for (const auto & type : basic::make_reverse_range (innerNode -> getType ()))
			{
				switch (type)
				{
					case X3D::X3DConstants::IndexedFaceSetTool:
					{
						numSelectedHoles += innerNode -> getField <X3D::SFInt32> ("selectedHoles_changed") .getValue ();
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

	getFormNewFaceButton () .set_sensitive (numSelectedHoles);
}

void
GeometryEditor::set_selectedFaces ()
{
	numSelectedFaces = 0;

	const auto & geometryNodes = getBrowserWindow () -> getSelection () -> getGeometries ();

	for (const auto & node : geometryNodes)
	{
		try
		{
			const auto innerNode = node -> getInnerNode ();

			for (const auto & type : basic::make_reverse_range (innerNode -> getType ()))
			{
				switch (type)
				{
					case X3D::X3DConstants::IndexedFaceSetTool:
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

	getExtrudeSelectedFacesButton () .set_sensitive (numSelectedFaces);
	getChipOfFacesButton          () .set_sensitive (numSelectedFaces);
	getDeleteFacesButton          () .set_sensitive (numSelectedFaces);
	getFlipVertexOrderingButton   () .set_sensitive (numSelectedFaces);
}

void
GeometryEditor::on_hand_toggled ()
{
	if (getBrowserWindow () -> getHandButton () .get_active ())
		coordEditor -> setField <X3D::SFString> ("toolType", "NONE");
}

void
GeometryEditor::on_arrow_toggled ()
{
	if (getBrowserWindow () -> getArrowButton () .get_active ())
		coordEditor -> setField <X3D::SFString> ("toolType", "SELECT");
}

void
GeometryEditor::on_hammer_clicked ()
{
	const auto undoStep  = std::make_shared <X3D::UndoStep> (_ ("Smash Selection"));
	auto       selection = getBrowserWindow () -> getSelection () -> getNodes ();

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
							basic::ifilestream text (X3D::X3DEditor::exportNodes (getCurrentContext (), { geometry }, "XML", true));

							const auto scene = getCurrentBrowser () -> createX3DFromStream (getCurrentContext () -> getWorldURL (), text);
							const auto nodes = X3D::X3DEditor::importScene (getCurrentContext (), scene, undoStep);

							X3D::X3DEditor::addToGroup (getCurrentContext (), shape, nodes, undoStep);
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

						X3D::X3DEditor::replaceNode (getCurrentContext (), shape, shape -> geometry (), geometry -> toPrimitive (), undoStep);
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

	getBrowserWindow () -> getSelection () -> setNodes ({ },       undoStep);
	getBrowserWindow () -> getSelection () -> setNodes (selection, undoStep);

	addUndoStep (undoStep);
}

void
GeometryEditor::set_selectGeometry ()
{
	const auto & selection = getBrowserWindow () -> getSelection ();

	getEditToggleButton () .set_active (selection -> getSelectGeometry ());
}

void
GeometryEditor::on_edit_toggled ()
{
	const auto & selection = getBrowserWindow () -> getSelection ();

	selection -> setSelectGeometry (getEditToggleButton () .get_active ());

	getGeometryToolsBox () .set_sensitive (getEditToggleButton () .get_active ());

	if (getEditToggleButton () .get_active ())
		;
	else
	{
		// Activate hand or arrow button.
	
		if (getBrowserWindow () -> getHandButton () .get_active ())
			;
		else if (getBrowserWindow () -> getArrowButton () .get_active ())
			;
		else if (getBrowserWindow () -> getSelection () -> getEnabled ())
			getBrowserWindow () -> getArrowButton () .set_active (true);
		else
			getBrowserWindow () -> getHandButton () .set_active (true);
	
		// Restore viewer.
	
		getCurrentBrowser () -> setViewerType (privateViewer);
	}
}

void
GeometryEditor::on_paint_selection_toggled ()
{

	if (changing)
		return;

	if (not getEditToggleButton () .get_active ())
		return;

	if (not getPaintSelectionButton () .get_active ())
	{
		getCurrentBrowser () -> setViewerType (privateViewer);
		coordEditor -> setField <X3D::SFBool> ("paintSelection", false);
		return;
	}

	// Disable X3DPointingDeviceSensors.
	getBrowserWindow () -> getSelection () -> setEnabled (true);

	switch (selector)
	{
		case SelectorType::BRUSH:
		{
			getCurrentBrowser () -> getViewer () .removeInterest (&GeometryEditor::set_viewer, this);
			getCurrentBrowser () -> getViewer () .addInterest (&GeometryEditor::connectViewer, this);

			getCurrentBrowser () -> setViewerType (privateViewer);

			coordEditor -> setField <X3D::SFString> ("toolType", "SELECT");
			coordEditor -> setField <X3D::SFBool> ("paintSelection", getPaintSelectionButton () .get_active ());
			break;
		}
		case SelectorType::RECTANGLE:
		{
			getCurrentBrowser () -> setViewerType (X3D::X3DConstants::RectangleSelection);

			coordEditor -> setField <X3D::SFString> ("toolType", "NONE");
			coordEditor -> setField <X3D::SFBool> ("paintSelection", false);
			break;
		}
		case SelectorType::LASSO:
		{
			getCurrentBrowser () -> setViewerType (X3D::X3DConstants::LassoSelection);

			coordEditor -> setField <X3D::SFString> ("toolType", "NONE");
			coordEditor -> setField <X3D::SFBool> ("paintSelection", false);
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
	selector = SelectorType::BRUSH;

	getPaintSelectionButton () .set_tooltip_text (_ ("Paint current selection. Right click to open menu."));
	getPaintSelectionImage ()  .set (Gtk::StockID ("Brush"), Gtk::IconSize (Gtk::ICON_SIZE_MENU));

	if (getPaintSelectionButton () .get_active ())
		on_paint_selection_toggled ();
	else
	{
		if (not changing)
			getPaintSelectionButton () .set_active (true);
	}
}

void
GeometryEditor::on_rectangle_activated ()
{
	selector = SelectorType::RECTANGLE;

	getPaintSelectionButton () .set_tooltip_text (_ ("Use rectangle selection. Right click to open menu."));
	getPaintSelectionImage ()  .set (Gtk::StockID ("RectangleSelection"), Gtk::IconSize (Gtk::ICON_SIZE_MENU));

	if (getPaintSelectionButton () .get_active ())
		on_paint_selection_toggled ();
	else
	{
		if (not changing)
			getPaintSelectionButton () .set_active (true);
	}
}

void
GeometryEditor::on_lasso_activated ()
{
	selector = SelectorType::LASSO;

	getPaintSelectionButton () .set_tooltip_text (_ ("Use lasso selection. Right click to open menu."));
	getPaintSelectionImage ()  .set (Gtk::StockID ("Lasso"), Gtk::IconSize (Gtk::ICON_SIZE_MENU));

	if (getPaintSelectionButton () .get_active ())
		on_paint_selection_toggled ();
	else
	{
		if (not changing)
			getPaintSelectionButton () .set_active (true);
	}
}

void
GeometryEditor::set_selector (const SelectorType type)
{
	switch (type)
	{
		case SelectorType::BRUSH:
			on_brush_activated ();
			break;
		case SelectorType::RECTANGLE:
			on_rectangle_activated ();
			break;
		case SelectorType::LASSO:
			on_lasso_activated ();
			break;
	}
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
GeometryEditor::on_select_line_loop_toggled ()
{
	coordEditor -> setField <X3D::SFBool> ("selectLineLoop", getSelectLineLoopMenuItem () .get_active ());
}

void
GeometryEditor::on_transform_tool_toggled ()
{
	coordEditor -> setField <X3D::SFBool> ("transform", getTransformToolButton () .get_active ());
}

bool
GeometryEditor::on_transform_tool_button_press_event (GdkEventButton* event)
{
	if (event -> button not_eq 3)
		return false;

	getTransformToolMenu () .popup (event -> button, event -> time);
	return true;
}

void
GeometryEditor::on_axis_aligned_bounding_box_toggled ()
{
	coordEditor -> setField <X3D::SFBool> ("axisAlignedBoundingBox", getAxisAlignedBoundingBoxMenuItem () .get_active ());
}

void
GeometryEditor::on_merge_points_clicked ()
{
	coordEditor -> setField <X3D::SFTime> ("mergePoints", X3D::SFTime::now ());
}

void
GeometryEditor::on_split_points_clicked ()
{
	coordEditor -> setField <X3D::SFTime> ("splitPoints", X3D::SFTime::now ());
}

void
GeometryEditor::on_cut_polygons_toggled ()
{
	if (changing)
		return;

	set_cut_polygons ();
}

void
GeometryEditor::set_cut_polygons ()
{
	if (getCutPolygonsButton () .get_active ())
	{
		getBrowserWindow () -> getSelection () -> setEnabled (true);

		getCurrentBrowser () -> setViewerType (X3D::X3DConstants::LightSaber);

		coordEditor -> setField <X3D::SFString> ("toolType", "CUT");
	}
	else
		getCurrentBrowser () -> setViewerType (privateViewer);
}

bool
GeometryEditor::on_cut_polygons_button_press_event (GdkEventButton* event)
{
	if (event -> button not_eq 3)
		return false;

	getCutPolygonsMenu () .popup (event -> button, event -> time);
	return true;
}

void
GeometryEditor::on_cut_polygons_enable_snapping_toggled ()
{
	coordEditor -> setField <X3D::SFBool> ("cutSnapping", getCutPolygonsEnableSnappingMenuItem () .get_active ());
}

void
GeometryEditor::on_form_new_face_clicked ()
{
	coordEditor -> setField <X3D::SFTime> ("formNewFace", X3D::SFTime::now ());
}

void
GeometryEditor::on_extrude_selected_edges_clicked ()
{
	coordEditor -> setField <X3D::SFTime> ("extrudeSelectedEdges", X3D::SFTime::now ());
}

void
GeometryEditor::on_extrude_selected_faces_clicked ()
{
	coordEditor -> setField <X3D::SFTime> ("extrudeSelectedFaces", X3D::SFTime::now ());
}

void
GeometryEditor::on_chip_of_face_clicked ()
{
	coordEditor -> setField <X3D::SFTime> ("chipOfSelectedFaces", X3D::SFTime::now ());
}

void
GeometryEditor::on_flip_vertex_ordering_clicked ()
{
	coordEditor -> setField <X3D::SFTime> ("flipVertexOrdering", X3D::SFTime::now ());
}

void
GeometryEditor::on_delete_selected_faces_clicked ()
{
	coordEditor -> setField <X3D::SFTime> ("deleteSelectedFaces", X3D::SFTime::now ());
}

void
GeometryEditor::store ()
{
	getConfig () -> setItem ("paintSelection",         getPaintSelectionButton () .get_active ());
	getConfig () -> setItem ("normalEnabled",          normalEditor -> getField <X3D::SFBool>      ("load"));
	getConfig () -> setItem ("normalLength",           normalEditor -> getField <X3D::SFFloat>     ("length"));
	getConfig () -> setItem ("normalColor",            normalEditor -> getField <X3D::SFColorRGBA> ("color"));
	getConfig () -> setItem ("selectionType",          coordEditor  -> getField <X3D::SFString>    ("selectionType"));
	getConfig () -> setItem ("selectLineLoop",         getSelectLineLoopMenuItem () .get_active ());
	getConfig () -> setItem ("transform",              getTransformToolButton () .get_active ());
	getConfig () -> setItem ("axisAlignedBoundingBox", getAxisAlignedBoundingBoxMenuItem () .get_active ());
	getConfig () -> setItem ("edgeColor",              coordEditor -> getField <X3D::SFColorRGBA> ("color"));
	getConfig () -> setItem ("selector",               size_t (selector));
	getConfig () -> setItem ("cutPolygons",            getCutPolygonsButton () .get_active ());
	getConfig () -> setItem ("cutSnapping",            getCutPolygonsEnableSnappingMenuItem () .get_active ());

	X3DGeometryEditorInterface::store ();
}

GeometryEditor::~GeometryEditor ()
{
	dispose ();
}

} // puck
} // titania
