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

#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"

#include <Titania/X3D/Miscellaneous/Random.h>

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
	                       selection (),
	                           coord (),
	                  indexedFaceSet (),
	                           color (),
	                       faceIndex (),
	                            face (0, 0),
	                           faces (),
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

		const auto shape       = preview -> getExecutionContext () -> getNamedNode <X3D::Shape> ("Shape");
		const auto touchSensor = preview -> getExecutionContext () -> getNamedNode <X3D::TouchSensor> ("TouchSensor");

		shape -> geometry ()               .addInterest (this, &ColorPerVertexEditor::on_look_at_all_clicked);
		touchSensor -> hitPoint_changed () .addInterest (this, &ColorPerVertexEditor::set_hitPoint);
		touchSensor -> touchTime ()        .addInterest (this, &ColorPerVertexEditor::set_touchTime);

		set_selection ();
	}
	catch (const X3D::X3DError &)
	{ }
}

void
ColorPerVertexEditor::set_selection ()
{
	undoHistory .clear ();

	try
	{
		const auto indexedFaceSets = getSelection <X3D::IndexedFaceSet> ({ X3D::X3DConstants::IndexedFaceSet });
		const auto shape           = preview -> getExecutionContext () -> getNamedNode <X3D::Shape> ("Shape");

		if (indexedFaceSets .empty ())
		{
			selection            = nullptr;
			indexedFaceSet       = nullptr;
			shape -> geometry () = nullptr;
			colorButton .setNodes ({ });
		}
		else
		{
			selection      = indexedFaceSets .back ();
			coord          = selection -> coord ();
			indexedFaceSet = preview -> getExecutionContext () -> createNode <X3D::IndexedFaceSet> ();

			indexedFaceSet -> solid ()           = selection -> solid ();
			indexedFaceSet -> ccw ()             = selection -> ccw ();
			indexedFaceSet -> creaseAngle ()     = selection -> creaseAngle ();
			indexedFaceSet -> normalPerVertex () = selection -> normalPerVertex ();
			indexedFaceSet -> normalIndex ()     = selection -> normalIndex ();
			indexedFaceSet -> coordIndex ()      = selection -> coordIndex ();
			indexedFaceSet -> normal ()          = selection -> normal ();
			indexedFaceSet -> coord ()           = selection -> coord ();

			shape -> geometry () = indexedFaceSet;

			setColor ();
			setFaceIndex ();

			colorButton .setIndex (0);
			colorButton .setNodes ({ color });

			// Initialize all.

			preview -> getExecutionContext () -> realize ();
		}
	}
	catch (const X3D::X3DError &)
	{ }
}

// Menubar

void
ColorPerVertexEditor::on_undo_activate ()
{
	preview -> grab_focus ();

	undoHistory .undoChanges ();
}

void
ColorPerVertexEditor::on_redo_activate ()
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
ColorPerVertexEditor::on_remove_unused_colors_activate ()
{
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
ColorPerVertexEditor::on_look_at_all_clicked ()
{
	if (preview -> getActiveLayer ())
		preview -> getActiveLayer () -> lookAt ();
}

void
ColorPerVertexEditor::on_single_vertex_clicked ()
{
	mode = SINGLE_VERTEX;
}

void
ColorPerVertexEditor::on_adjacent_vertices_clicked ()
{
	mode = ADJACENT_VERTICES;
}

void
ColorPerVertexEditor::on_single_face_clicked ()
{
	mode = SINGLE_FACE;
}

void
ColorPerVertexEditor::on_whole_object_clicked ()
{
	mode = WHOLE_OBJECT;
}

void
ColorPerVertexEditor::on_apply_clicked ()
{
	selection -> colorIndex () = indexedFaceSet -> colorIndex ();

	if (this -> color -> isTransparent ())
	{
		const auto color = selection -> getExecutionContext () -> createNode <X3D::ColorRGBA> ();

		color -> color () = this -> color -> color ();

		selection -> color () = color;
	
	}
	else
	{
		const auto color = selection -> getExecutionContext () -> createNode <X3D::Color> ();

		for (const auto & c : this -> color -> color ())
			color -> color () .emplace_back (c .getRed (), c .getGreen (), c .getBlue ());

		selection -> color () = color;
	}

	selection -> getExecutionContext () -> realize ();
}

void
ColorPerVertexEditor::set_hitPoint (const X3D::Vector3f & hitPoint)
{
	if (not coord)
		return;

	const auto index = getPointIndex (hitPoint);
	const auto point = coord -> get1Point (index);

	// Determine face and faces

	setFaces (hitPoint, index);

	// Setup cross hair

	set_crossHair (point);

	// Colorize vertices

	const auto touchSensor = preview -> getExecutionContext () -> getNamedNode <X3D::TouchSensor> ("TouchSensor");

	if (touchSensor -> isActive ())
	{
		if (getSelectColorButton () .get_active ())
			return;

		using set1Value = void (X3D::MFInt32::*) (const X3D::MFInt32::size_type, const int32_t &);

		switch (mode)
		{
			case SINGLE_VERTEX:
			{
				const auto index = face .first + face .second;
			
				if (indexedFaceSet -> colorIndex () .get1Value (index) not_eq (int32_t) colorButton .getIndex ())
				{
					const auto undoStep = std::make_shared <UndoStep> ("Colorize Singe Vertex");
					
					undoStep -> addObjects (indexedFaceSet);
					undoStep -> addUndoFunction ((set1Value) &X3D::MFInt32::set1Value, std::ref (indexedFaceSet -> colorIndex ()), index, indexedFaceSet -> colorIndex () .get1Value (index));
					undoStep -> addRedoFunction ((set1Value) &X3D::MFInt32::set1Value, std::ref (indexedFaceSet -> colorIndex ()), index, colorButton .getIndex ());
					indexedFaceSet -> colorIndex () .set1Value (index, colorButton .getIndex ());
					
					addUndoStep (undoStep);
				}
				break;
			}
			case ADJACENT_VERTICES:
			{
				const auto undoStep = std::make_shared <UndoStep> ("Colorize Adjacent Vertices");

				undoStep -> addObjects (indexedFaceSet);

				for (const auto & face : faces)
				{
					const auto index = face .first + face .second;
				
					if (indexedFaceSet -> colorIndex () .get1Value (index) not_eq (int32_t) colorButton .getIndex ())
					{
						undoStep -> addUndoFunction ((set1Value) &X3D::MFInt32::set1Value, std::ref (indexedFaceSet -> colorIndex ()), index, indexedFaceSet -> colorIndex () .get1Value (index));
						undoStep -> addRedoFunction ((set1Value) &X3D::MFInt32::set1Value, std::ref (indexedFaceSet -> colorIndex ()), index, colorButton .getIndex ());
						indexedFaceSet -> colorIndex () .set1Value (index, colorButton .getIndex ());
					}
				}

				addUndoStep (undoStep);
				break;
			}
			case SINGLE_FACE:
			{
				const auto undoStep = std::make_shared <UndoStep> ("Colorize Single Face");

				undoStep -> addObjects (indexedFaceSet);

				for (const auto & index : getPoints (face .first))
				{
					if (indexedFaceSet -> colorIndex () .get1Value (index) not_eq (int32_t) colorButton .getIndex ())
					{
						undoStep -> addUndoFunction ((set1Value) &X3D::MFInt32::set1Value, std::ref (indexedFaceSet -> colorIndex ()), index, indexedFaceSet -> colorIndex () .get1Value (index));
						undoStep -> addRedoFunction ((set1Value) &X3D::MFInt32::set1Value, std::ref (indexedFaceSet -> colorIndex ()), index, colorButton .getIndex ());
						indexedFaceSet -> colorIndex () .set1Value (index, colorButton .getIndex ());
					}
				}

				addUndoStep (undoStep);
				break;
			}
			case WHOLE_OBJECT:
			{
				X3D::MFInt32 colorIndex;
			
				for (const auto & index : indexedFaceSet -> coordIndex ())
					colorIndex .emplace_back (index < 0 ? -1 : colorButton .getIndex ());
					
				if (indexedFaceSet -> colorIndex () not_eq colorIndex)
				{
					const auto undoStep = std::make_shared <UndoStep> ("Colorize Whole Object");

					undoStep -> addObjects (indexedFaceSet);

					undoStep -> addUndoFunction (&X3D::MFInt32::setValue, std::ref (indexedFaceSet -> colorIndex ()), indexedFaceSet -> colorIndex ());
					undoStep -> addRedoFunction (&X3D::MFInt32::setValue, std::ref (indexedFaceSet -> colorIndex ()), colorIndex);
					indexedFaceSet -> colorIndex () = std::move (colorIndex);

					addUndoStep (undoStep);
				}
				break;
			}
		}
	}
}

void
ColorPerVertexEditor::set_touchTime ()
{
	if (getSelectColorButton () .get_active ())
	{
		getSelectColorButton () .set_active (false);

		const auto index = indexedFaceSet -> colorIndex () .get1Value (face .first + face .second);

		colorButton .setIndex (index);
		return;
	}
}

void
ColorPerVertexEditor::set_crossHair (const X3D::Vector3f & point)
{
	const auto crossHair           = preview -> getExecutionContext () -> getNamedNode <X3D::Transform> ("CrossHair");
	const auto crossHairCoordinate = preview -> getExecutionContext () -> getNamedNode <X3D::Coordinate> ("CrossHairCoordinate");
	const auto points              = getPoints (face .first);

	if (points .size () < 3)
		return;

	const auto vertex = face .second;
	const auto i1     = indexedFaceSet -> coordIndex () [points [vertex == 0 ? points .size () - 1 : vertex - 1]];
	const auto i2     = indexedFaceSet -> coordIndex () [points [vertex]];
	const auto i3     = indexedFaceSet -> coordIndex () [points [(vertex + 1) % points .size ()]];
	const auto p1     = coord -> get1Point (i1);
	const auto p2     = coord -> get1Point (i2);
	const auto p3     = coord -> get1Point (i3);

	const auto edge1  = p1 - p2;
	const auto edge2  = p3 - p2;
	const auto normal = X3D::normal (p1, p2, p3) * X3D::abs (edge1 + edge2) / 2.0;

	crossHairCoordinate -> point () = {
		-normal, normal,
		X3D::Vector3d (), edge1,
		X3D::Vector3d (), edge2
	};

	crossHair -> translation () = point;
}

void
ColorPerVertexEditor::setColor ()
{
	// Generate color and colorIndex.

	const X3D::X3DPtr <X3D::X3DColorNode> colorNode (selection -> color ());

	color = preview -> getExecutionContext () -> createNode <X3D::ColorRGBA> ();
	color -> setDynamicTransparency (true);

	indexedFaceSet -> color () = color;

	if (colorNode)
	{
		if (selection -> colorPerVertex ())
		{
			if (selection -> colorIndex () .empty ())
				indexedFaceSet -> colorIndex () = indexedFaceSet -> coordIndex ();
			else
				indexedFaceSet -> colorIndex () = selection -> colorIndex ();
		}
		else
		{
			size_t face = 0;

			for (const auto & index : indexedFaceSet -> coordIndex ())
			{
				if (index < 0)
				{
					++ face;
					indexedFaceSet -> colorIndex () .emplace_back (-1);
					continue;
				}

				if (face < selection -> colorIndex () .size ())
					indexedFaceSet -> colorIndex () .emplace_back (selection -> colorIndex () .get1Value (face));
				else
					indexedFaceSet -> colorIndex () .emplace_back (face);
			}
		}

		for (size_t i = 0, size = colorNode -> getSize (); i < size; ++ i)
			color -> color () .emplace_back (colorNode -> get1Color (i));
	}
	else
	{
		for (const auto & index : indexedFaceSet -> coordIndex ())
			indexedFaceSet -> colorIndex () .emplace_back (index < 0 ? -1 : 0);

		color -> color () .emplace_back (X3D::Color4f (1, 1, 1, 1));
	}
}

size_t
ColorPerVertexEditor::getPointIndex (const X3D::Vector3f & hitPoint) const
{
	float  min   = std::numeric_limits <float>::infinity ();
	size_t index = 0;

	for (size_t i = 0, size = coord -> getSize (); i < size; ++ i)
	{
		const auto distance = math::abs (hitPoint - X3D::Vector3f (coord -> get1Point (i)));

		if (distance < min)
		{
			min   = distance;
			index = i;
		}
	}

	return index;
}

void
ColorPerVertexEditor::setFaceIndex ()
{
	faceIndex .clear ();

	size_t face   = 0;
	size_t vertex = 0;

	for (const int32_t index : indexedFaceSet -> coordIndex ())
	{
		if (index < 0)
		{
			face  += vertex + 1;
			vertex = 0;
			continue;
		}
			
		faceIndex .emplace (index, std::make_pair (face, vertex));

		++ vertex;
	}
}

void
ColorPerVertexEditor::setFaces (const X3D::Vector3f & hitPoint, const size_t point)
{
	faces .clear ();

	const auto range = faceIndex .equal_range (point);

	for (const auto & face : range)
		faces .emplace_back (face .second);

	if (faces .empty ())
		return;

	// Get distances of faces to hitPoint.

	std::vector <float> distances;

	for (const auto & face : faces)
	{
		const auto points = getPoints (face .first);

		if (points .size () < 3)
		{
			distances .emplace_back (std::numeric_limits <float>::infinity ());
			continue;
		}

		const auto vertex = face .second;
		const auto i1     = indexedFaceSet -> coordIndex () [points [vertex == 0 ? points .size () - 1 : vertex - 1]];
		const auto i2     = indexedFaceSet -> coordIndex () [points [vertex]];
		const auto i3     = indexedFaceSet -> coordIndex () [points [(vertex + 1) % points .size ()]];
		const auto p1     = coord -> get1Point (i1);
		const auto p2     = coord -> get1Point (i2);
		const auto p3     = coord -> get1Point (i3);

		const X3D::Plane3f plane (p1, p2, p3);

		distances .emplace_back (std::abs (plane .distance (hitPoint)));
	}

	// Determine face.

	const auto iter  = std::min_element (distances .begin (), distances .end ());
	const auto index = iter - distances .begin ();

	face = faces [index];

	// DEBUG
	auto d = distances .begin ();
	__LOG__ << std::endl;
	__LOG__ << faces .size () << std::endl;

	for (const auto & face : faces)
		__LOG__ << *d++ << " : " << face .first << " : " << face .second << std::endl;

	__LOG__ << face .first << " : " << face .second << std::endl;
}

std::vector <size_t>
ColorPerVertexEditor::getPoints (const size_t face) const
{
	std::vector <size_t> points;

	for (size_t i = face, size = indexedFaceSet -> coordIndex () .size (); i < size; ++ i)
	{
		const auto index = indexedFaceSet -> coordIndex () [i];

		if (index < 0)
			break;

		points .emplace_back (i);
	}

	return points;
}

ColorPerVertexEditor::~ColorPerVertexEditor ()
{
	dispose ();
}

} // puck
} // titania
