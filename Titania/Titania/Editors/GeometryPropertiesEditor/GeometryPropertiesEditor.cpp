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

#include "GeometryPropertiesEditor.h"

#include "../../Configuration/config.h"

#include <Titania/X3D/Components/Geometry3D/IndexedFaceSet.h>
#include <Titania/X3D/Components/Geometry3D/ElevationGrid.h>
#include <Titania/X3D/Components/Geospatial/GeoElevationGrid.h>
#include <Titania/X3D/Components/Shape/X3DShapeNode.h>

namespace titania {
namespace puck {

GeometryPropertiesEditor::GeometryPropertiesEditor (X3DBrowserWindow* const browserWindow) :
	                     X3DBaseInterface (browserWindow, browserWindow -> getCurrentBrowser ()),
	 X3DGeometryPropertiesEditorInterface (get_ui ("Editors/GeometryPropertiesEditor.glade"), gconf_dir ()),
	                       X3DArc2DEditor (),
	                  X3DArcClose2DEditor (),
	                    X3DCircle2DEditor (),
	                      X3DDisk2DEditor (),
	                 X3DRectangle2DEditor (),
	                         X3DBoxEditor (),
	                        X3DConeEditor (),
	                    X3DCylinderEditor (),
	                   X3DExtrusionEditor (),
	                      X3DSphereEditor (),
	              X3DPrimitiveCountEditor (),
	                      X3DGeometryTool (),
	                                solid (this, getSolidCheckButton (),  "solid"),
	                                  ccw (this, getCCWCheckButton (),    "ccw"),
	                               convex (this, getConvexCheckButton (), "convex"),
	                          creaseAngle (this, getCreaseAngleAdjustment (), getCreaseAngleBox (), "creaseAngle"),
	                       colorPerVertex (this, getColorPerVertexCheckButton (), "colorPerVertex"),
	                      normalPerVertex (this, getNormalPerVertexCheckButton (), "normalPerVertex"),
	                         geometryNode (),
	                                nodes (),
	                               shapes (),
	                          nodesBuffer (),
	                            changing (false)
{
	addChildren (geometryNode, nodes, shapes, nodesBuffer);

	nodesBuffer .addInterest (this, &GeometryPropertiesEditor::set_buffer);

	getCreaseAngleAdjustment () -> set_upper (M_PI);

	setup ();
}

void
GeometryPropertiesEditor::configure ()
{
	X3DGeometryPropertiesEditorInterface::configure ();
	X3DPrimitiveCountEditor::configure ();
	X3DGeometryTool::configure ();

	getGeometryChildNotebook () .set_current_page (getConfig () -> getInteger ("currentPage"));
}

void
GeometryPropertiesEditor::initialize ()
{
	X3DGeometryPropertiesEditorInterface::initialize ();
	X3DPrimitiveCountEditor::initialize ();
	X3DGeometryTool::initialize ();
}

void
GeometryPropertiesEditor::set_selection (const X3D::MFNode & selection)
{
	X3DGeometryPropertiesEditorInterface::set_selection (selection);

	for (const auto & shape : shapes)
		shape -> geometry () .removeInterest (this, &GeometryPropertiesEditor::set_geometry);

	X3DArc2DEditor::removeShapes ();
	X3DArcClose2DEditor::removeShapes ();
	X3DCircle2DEditor::removeShapes ();
	X3DDisk2DEditor::removeShapes ();
	X3DRectangle2DEditor::removeShapes ();
	X3DBoxEditor::removeShapes ();
	X3DConeEditor::removeShapes ();
	X3DCylinderEditor::removeShapes ();
	X3DExtrusionEditor::removeShapes ();
	X3DSphereEditor::removeShapes ();

	shapes = getNodes <X3D::X3DShapeNode> (selection, { X3D::X3DConstants::X3DShapeNode });

	for (const auto & shape : shapes)
		shape -> geometry () .addInterest (this, &GeometryPropertiesEditor::set_geometry);

	X3DArc2DEditor::addShapes ();
	X3DArcClose2DEditor::addShapes ();
	X3DCircle2DEditor::addShapes ();
	X3DDisk2DEditor::addShapes ();
	X3DRectangle2DEditor::addShapes ();
	X3DBoxEditor::addShapes ();
	X3DConeEditor::addShapes ();
	X3DCylinderEditor::addShapes ();
	X3DExtrusionEditor::addShapes ();
	X3DSphereEditor::addShapes ();

	set_geometry ();
}

void
GeometryPropertiesEditor::set_geometry ()
{
	nodesBuffer .addEvent ();
}

void
GeometryPropertiesEditor::connectGeometry (const X3D::SFNode & field)
{
	field .removeInterest (this, &GeometryPropertiesEditor::connectGeometry);
	field .addInterest (this, &GeometryPropertiesEditor::set_geometry);
}

void
GeometryPropertiesEditor::set_buffer ()
{
	changing = true;

	for (const auto & node : nodes)
	{
		try
		{
			node -> getField <X3D::SFNode> ("normal") .removeInterest (this, &GeometryPropertiesEditor::set_normal);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	auto  tuple             = getSelection <X3D::X3DGeometryNode> (getShapes (), "geometry");
	const int32_t active    = std::get <1> (tuple);
	const bool    hasParent = std::get <2> (tuple);
	const bool    hasField  = (active not_eq -2);

	geometryNode = std::move (std::get <0> (tuple));
	nodes        = getNodes <X3D::X3DBaseNode> (X3D::MFNode (getShapes ()), { X3D::X3DConstants::X3DGeometryNode });

	if (nodes .empty () and geometryNode)
		nodes = { geometryNode };

	solid           .setNodes (nodes);
	ccw             .setNodes (nodes);
	convex          .setNodes (nodes);
	creaseAngle     .setNodes (nodes);
	colorPerVertex  .setNodes (nodes);
	normalPerVertex .setNodes (nodes);

	// Normals Box

	getGeometryComboBoxText () .set_sensitive (hasField);

	if (active > 0)
		getGeometryComboBoxText () .set_active_text (geometryNode ? geometryNode -> getTypeName () : "None");
	else if (active == 0)
		getGeometryComboBoxText () .set_active_text ("None");
	else
		getGeometryComboBoxText () .set_active (-1);

	getSelectGeometryBox ()    .set_sensitive (hasParent);
	getGeometryUnlinkButton () .set_sensitive (active > 0 and not nodes .empty () and nodes [0] -> getCloneCount () > 1);
	getNormalsBox ()           .set_sensitive (false);

	for (const auto & node : nodes)
	{
		try
		{
			node -> getField <X3D::SFNode> ("normal") .addInterest (this, &GeometryPropertiesEditor::set_normal);

			getNormalsBox () .set_sensitive (true);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	set_normal ();

	changing = false;
}

void
GeometryPropertiesEditor::set_normal ()
{
	bool normal = false;

	for (const auto & node : nodes)
	{
		try
		{
			if (normal or_eq node -> getField <X3D::SFNode> ("normal"))
				break;
		}
		catch (const X3D::X3DError &)
		{ }
	}

	getRemoveNormalsButton () .set_sensitive (normal);
}

void
GeometryPropertiesEditor::on_geometry_changed ()
{
	if (changing)
		return;

	if (getGeometryComboBoxText () .get_active_row_number () > 0)
	{
	   try
	   {
			const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Change Field »geometry«"));
		   auto       node     = getCurrentContext () -> createNode (getGeometryComboBoxText () .get_active_text ());

			getCurrentContext () -> addUninitializedNode (node);
			getCurrentContext () -> realize ();

		   if (geometryNode and geometryNode -> getType () .back () == node -> getType () .back ())
				node = geometryNode;

			for (const auto & shapeNode : getShapes ())
			{
				auto & field = shapeNode -> geometry ();

				field .removeInterest (this, &GeometryPropertiesEditor::set_geometry);
				field .addInterest (this, &GeometryPropertiesEditor::connectGeometry);

				getBrowserWindow () -> replaceNode (getCurrentContext (), X3D::SFNode (shapeNode), field, node, undoStep);
			}

			getBrowserWindow () -> addUndoStep (undoStep);
		}
		catch (const X3D::X3DError & error)
		{
			__LOG__ << error .what () << std::endl;
		}
	}
	else if (getGeometryComboBoxText () .get_active_row_number () == 0)
	{
		const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Change Field »geometry«"));

		for (const auto & shapeNode : getShapes ())
		{
			auto & field = shapeNode -> geometry ();

			field .removeInterest (this, &GeometryPropertiesEditor::set_geometry);
			field .addInterest (this, &GeometryPropertiesEditor::connectGeometry);

			getBrowserWindow () -> removeNode (getCurrentContext (), X3D::SFNode (shapeNode), field, undoStep);
		}

		getBrowserWindow () -> addUndoStep (undoStep);
	}
}

void
GeometryPropertiesEditor::on_geometry_unlink_clicked ()
{
	X3D::UndoStepPtr undoStep;

	unlinkClone (getShapes (), "geometry", undoStep);
}

void
GeometryPropertiesEditor::on_add_normals_clicked ()
{
	const auto geometries = getSelection <X3D::X3DBaseNode> ({ X3D::X3DConstants::X3DGeometryNode });
	const auto undoStep   = std::make_shared <X3D::UndoStep> (_ ("Add Normals"));

	for (const auto geometry : geometries)
	{
		for (const auto & type : basic::make_reverse_range (geometry -> getType ()))
		{
			switch (type)
			{
				case X3D::X3DConstants::ElevationGrid:
				{
					const auto elevationGrid = dynamic_cast <X3D::ElevationGrid*> (geometry .getValue ());

					undoStep -> addObjects (geometry);
					undoStep -> addUndoFunction (&X3D::SFBool::setValue, std::ref (elevationGrid -> normalPerVertex ()), elevationGrid -> normalPerVertex ());
					undoStep -> addRedoFunction (&X3D::SFBool::setValue, std::ref (elevationGrid -> normalPerVertex ()), true);
					elevationGrid -> normalPerVertex () = true;

					getBrowserWindow () -> replaceNode (getCurrentContext (), geometry, elevationGrid -> normal (), nullptr, undoStep);

					elevationGrid -> addNormals ();

					undoStep -> addRedoFunction (&X3D::SFNode::setValue, std::ref (elevationGrid -> normal ()), elevationGrid -> normal ());
					break;
				}
				case X3D::X3DConstants::GeoElevationGrid:
				{
					const auto geoElevationGrid = dynamic_cast <X3D::GeoElevationGrid*> (geometry .getValue ());

					undoStep -> addObjects (geometry);
					undoStep -> addUndoFunction (&X3D::SFBool::setValue, std::ref (geoElevationGrid -> normalPerVertex ()), geoElevationGrid -> normalPerVertex ());
					undoStep -> addRedoFunction (&X3D::SFBool::setValue, std::ref (geoElevationGrid -> normalPerVertex ()), true);
					geoElevationGrid -> normalPerVertex () = true;

					getBrowserWindow () -> replaceNode (getCurrentContext (), geometry, geoElevationGrid -> normal (), nullptr, undoStep);

					geoElevationGrid -> addNormals ();

					undoStep -> addRedoFunction (&X3D::SFNode::setValue, std::ref (geoElevationGrid -> normal ()), geoElevationGrid -> normal ());
					break;
				}
				case X3D::X3DConstants::IndexedFaceSet:
				{
					const auto indexedFaceSet = dynamic_cast <X3D::IndexedFaceSet*> (geometry .getValue ());

					undoStep -> addObjects (geometry);
					undoStep -> addUndoFunction (&X3D::SFBool::setValue, std::ref (indexedFaceSet -> normalPerVertex ()), indexedFaceSet -> normalPerVertex ());
					undoStep -> addRedoFunction (&X3D::SFBool::setValue, std::ref (indexedFaceSet -> normalPerVertex ()), true);
					indexedFaceSet -> normalPerVertex () = true;

					undoStep -> addUndoFunction (&X3D::MFInt32::setValue, std::ref (indexedFaceSet -> normalIndex ()), indexedFaceSet -> normalIndex ());
					getBrowserWindow () -> replaceNode (getCurrentContext (), geometry, indexedFaceSet -> normal (), nullptr, undoStep);

					indexedFaceSet -> addNormals ();

					undoStep -> addRedoFunction (&X3D::MFInt32::setValue, std::ref (indexedFaceSet -> normalIndex ()), indexedFaceSet -> normalIndex ());
					undoStep -> addRedoFunction (&X3D::SFNode::setValue,  std::ref (indexedFaceSet -> normal ()),      indexedFaceSet -> normal ());
					break;
				}
				case X3D::X3DConstants::X3DComposedGeometryNode:
				{
					const auto composedGeometryNode = dynamic_cast <X3D::X3DComposedGeometryNode*> (geometry .getValue ());

					undoStep -> addObjects (geometry);
					getBrowserWindow () -> replaceNode (getCurrentContext (), geometry, composedGeometryNode -> normal (), nullptr, undoStep);

					composedGeometryNode -> addNormals ();

					undoStep -> addRedoFunction (&X3D::SFNode::setValue, std::ref (composedGeometryNode -> normal ()), composedGeometryNode -> normal ());
					break;
				}
				default:
					continue;
			}

			break;
		}
	}

	getBrowserWindow () -> addUndoStep (undoStep);
}

void
GeometryPropertiesEditor::on_remove_normals_clicked ()
{
	const auto geometries = getSelection <X3D::X3DBaseNode> ({ X3D::X3DConstants::X3DGeometryNode });
	const auto undoStep   = std::make_shared <X3D::UndoStep> (_ ("Remove Normals"));

	for (const auto geometry : geometries)
	{
		try
		{
			auto & normalIndex = geometry -> getField <X3D::MFInt32> ("normalIndex");

			undoStep -> addObjects (geometry);
			undoStep -> addUndoFunction (&X3D::MFInt32::setValue, std::ref (normalIndex), normalIndex);
			undoStep -> addRedoFunction (&X3D::MFInt32::clear, std::ref (normalIndex));
			normalIndex .clear ();
		}
		catch (const X3D::X3DError &)
		{ }

		try
		{
			auto & normal = geometry -> getField <X3D::SFNode> ("normal");

			getBrowserWindow () -> replaceNode (getCurrentContext (), geometry, normal, nullptr, undoStep);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	getBrowserWindow () -> addUndoStep (undoStep);
}

void
GeometryPropertiesEditor::store ()
{
	getConfig () -> setItem ("currentPage", getGeometryChildNotebook () .get_current_page ());

	X3DGeometryTool::store ();
	X3DPrimitiveCountEditor::store ();
	X3DGeometryPropertiesEditorInterface::store ();
}

GeometryPropertiesEditor::~GeometryPropertiesEditor ()
{
	dispose ();
}

} // puck
} // titania
