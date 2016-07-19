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

#include "X3DGeometrySelectionEditor.h"

#include "../GeometryEditor/GeometryEditor.h"
#include "../../ComposedWidgets/RotationTool.h"

#include <Titania/X3D/Components/Grouping/Transform.h>
#include <Titania/X3D/Tools/Geometry3D/IndexedFaceSet/IndexedFaceSetTool.h>

namespace titania {
namespace puck {

static constexpr auto M2 = X3D::Matrix4d (2,0,0,0, 0,2,0,0, 0,0,2,0, 0,0,0,1);	

X3DGeometrySelectionEditor::X3DGeometrySelectionEditor () :
	X3DPrecisionPlacementPanelInterface (),
	                        translation (this,
	                                     getGeometrySelectionTranslationXAdjustment (),
	                                     getGeometrySelectionTranslationYAdjustment (),
	                                     getGeometrySelectionTranslationZAdjustment (),
	                                     getGeometrySelectionTranslationBox (),
	                                     "translation"),
	                           rotation (this,
	                                     getGeometrySelectionRotationXAdjustment (),
	                                     getGeometrySelectionRotationYAdjustment (),
	                                     getGeometrySelectionRotationZAdjustment (),
	                                     getGeometrySelectionRotationAAdjustment (),
	                                     getGeometrySelectionRotationBox (),
	                                     "rotation"),
	                       rotationTool (new RotationTool (this, getGeometrySelectionRotationToolBox (), "rotation")),
	                              scale (this,
	                                     getGeometrySelectionScaleXAdjustment (),
	                                     getGeometrySelectionScaleYAdjustment (),
	                                     getGeometrySelectionScaleZAdjustment (),
	                                     getGeometrySelectionScaleBox (),
	                                     "scale"),
	                      transformNode (),
	                               tool (),
	                         lastMatrix ()
{
	addChildren (transformNode, tool);
}

void
X3DGeometrySelectionEditor::configure ()
{
	getGeometrySelectionUniformScaleButton () .set_active (getConfig () -> getBoolean ("geometrySelectionUniformScale"));

	getBrowserWindow () -> getGeometryEditor () -> getGeometryNodes () .addInterest (this, &X3DGeometrySelectionEditor::set_geometry_nodes);
}

void
X3DGeometrySelectionEditor::initialize ()
{
	translation .setUndo (false);
	rotation    .setUndo (false);
	scale       .setUndo (false);

	rotationTool -> setUndo (false);
 }

void
X3DGeometrySelectionEditor::set_selection (const X3D::MFNode & selection)
{ }

void
X3DGeometrySelectionEditor::set_geometry_nodes (const X3D::MFNode & geometryNodes)
{
	for (const auto & node : geometryNodes)
	{
		const auto tool = X3D::X3DPtr <X3D::IndexedFaceSetTool> (node);

		if (tool)
			tool -> touchTime () .addInterest (this, &X3DGeometrySelectionEditor::set_touchTime);
	}

	set_touchTime ();
}

void
X3DGeometrySelectionEditor::set_touchTime ()
{
	if (tool)
		tool -> getSelectionTransform () -> removeInterest (this, &X3DGeometrySelectionEditor::set_tool_matrix);

	transformNode = getCurrentContext () -> createNode <X3D::Transform> ();
	tool          = getCurrentTool ();

	// Update composed widgets

	const X3D::MFNode transforms ({ transformNode });

	translation .setNodes (transforms);
	rotation    .setNodes (transforms);
	scale       .setNodes (transforms);
	rotationTool -> setNodes (transforms);

	//

	getGeometrySelectionExpander () .set_visible (tool);

	if (tool)
	{
		const auto & selectionTransform = tool -> getSelectionTransform ();

		transformNode      -> addInterest (this, &X3DGeometrySelectionEditor::set_matrix);
		selectionTransform -> addInterest (this, &X3DGeometrySelectionEditor::set_tool_matrix);

		set_tool_matrix ();
	}
}

void
X3DGeometrySelectionEditor::set_matrix ()
{
//	__LOG__ << transformNode -> getMatrix () << std::endl;

//	tool -> getSelectionTransform () -> removeInterest (this, &X3DGeometrySelectionEditor::set_tool_matrix);
//	tool -> getSelectionTransform () -> addInterest (this, &X3DGeometrySelectionEditor::connectToolMatrix);

	const auto & axisRotation = tool -> getAxisRotation ();
	const auto & M            = ~axisRotation * ~lastMatrix * transformNode -> getMatrix () * axisRotation;

	for (const auto & pair : tool -> getSelectedPoints ())
		tool -> getCoord () -> set1Point (pair .first, pair .second * M);
}

void
X3DGeometrySelectionEditor::set_tool_matrix ()
{
	transformNode -> removeInterest (this, &X3DGeometrySelectionEditor::set_matrix);
	transformNode -> addInterest (this, &X3DGeometrySelectionEditor::connectMatrix);
	
	const auto & selectionTransform = tool -> getSelectionTransform ();
	const auto & axisRotation       = tool -> getAxisRotation ();
	const auto   T                  = M2 * selectionTransform -> getBBox () .matrix () * axisRotation;

	transformNode -> setMatrix (T);

	lastMatrix = T;

//	__LOG__ << selectionTransform -> getBBox () .matrix () << std::endl;
//	__LOG__ << selectionTransform -> getMatrix () << std::endl;
//	__LOG__ << M << std::endl;
}

void
X3DGeometrySelectionEditor::connectMatrix ()
{
	transformNode -> addInterest (this, &X3DGeometrySelectionEditor::set_matrix);
}

void
X3DGeometrySelectionEditor::connectToolMatrix ()
{
	tool -> getSelectionTransform () -> addInterest (this, &X3DGeometrySelectionEditor::set_tool_matrix);
}

X3D::X3DPtr <X3D::IndexedFaceSetTool>
X3DGeometrySelectionEditor::getCurrentTool () const
{
	const auto & geometryNodes = getBrowserWindow () -> getGeometryEditor () -> getGeometryNodes ();

	const auto result = std::max_element (geometryNodes .begin (),
	                                      geometryNodes .end (),
	                                      [ ] (const X3D::SFNode & a, const X3D::SFNode & b)
	                                      {
	                                         return a -> getField <X3D::SFTime> ("touchTime") <= b -> getField <X3D::SFTime> ("touchTime");
												     });

	if (result == geometryNodes .end ())
		return nullptr;

	return X3D::X3DPtr <X3D::IndexedFaceSetTool> (*result);
}

void
X3DGeometrySelectionEditor::store ()
{
	getConfig () -> setItem ("geometrySelectionUniformScale", getGeometrySelectionUniformScaleButton () .get_active ());

	getBrowserWindow () -> getGeometryEditor () -> getGeometryNodes () .removeInterest (this, &X3DGeometrySelectionEditor::set_geometry_nodes);
}

X3DGeometrySelectionEditor::~X3DGeometrySelectionEditor ()
{ }

} // puck
} // titania
