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

#include "IndexedFaceSetTool.h"

#include "../../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

IndexedFaceSetTool::IndexedFaceSetTool (X3DBaseNode* const node) :
	                      X3DBaseNode (node -> getExecutionContext () -> getBrowser (), node -> getExecutionContext ()),
	                   IndexedFaceSet (node -> getExecutionContext ()),
	                      X3DBaseTool (node),
	            X3DIndexedFaceSetTool (),
	 X3DIndexedFaceSetSelectionObject (),
	     X3DIndexedFaceSetBrushObject (),
	 X3DIndexedFaceSetTransformObject (),
	X3DIndexedFaceSetOperationsObject (),
	     X3DIndexedFaceSetKnifeObject (),
	X3DIndexedFaceSetLightSaberObject (),
	 X3DIndexedFaceSetSculpToolObject ()
{
	addType (X3DConstants::IndexedFaceSetTool);

	addField (inputOutput, "toolType",               toolType ());
	addField (inputOutput, "paintSelection",         paintSelection ());
	addField (inputOutput, "selectLineLoop",         selectLineLoop ());
	addField (inputOutput, "transform",              transform ());
	addField (inputOutput, "axisAlignedBoundingBox", axisAlignedBoundingBox ());
	addField (inputOutput, "selectionType",          selectionType ());
	addField (inputOutput, "selectAll",              selectAll ());
	addField (inputOutput, "deselectAll",            deselectAll ());
	addField (inputOutput, "replaceSelection",       replaceSelection ());
	addField (inputOutput, "addSelection",           addSelection ());
	addField (inputOutput, "removeSelection",        removeSelection ());
	addField (inputOutput, "replaceSelectedPoints",  replaceSelectedPoints ());
	addField (inputOutput, "addSelectedPoints",      addSelectedPoints ());
	addField (inputOutput, "removeSelectedPoints",   removeSelectedPoints ());
	addField (inputOutput, "replaceSelectedEdges",   replaceSelectedEdges ());
	addField (inputOutput, "addSelectedEdges",       addSelectedEdges ());
	addField (inputOutput, "removeSelectedEdges",    removeSelectedEdges ());
	addField (inputOutput, "replaceSelectedFaces",   replaceSelectedFaces ());
	addField (inputOutput, "addSelectedFaces",       addSelectedFaces ());
	addField (inputOutput, "removeSelectedFaces",    removeSelectedFaces ());
	addField (inputOutput, "cutGeometry",            cutGeometry ());
	addField (inputOutput, "copyGeometry",           copyGeometry ());
	addField (inputOutput, "pasteGeometry",          pasteGeometry ());
	addField (inputOutput, "mergePoints",            mergePoints ());
	addField (inputOutput, "splitPoints",            splitPoints ());
	addField (inputOutput, "formNewFace",            formNewFace ());
	addField (inputOutput, "extrudeSelectedEdges",   extrudeSelectedEdges ());
	addField (inputOutput, "extrudeSelectedFaces",   extrudeSelectedFaces ());
	addField (inputOutput, "chipOfSelectedFaces",    chipOfSelectedFaces ());
	addField (inputOutput, "flipVertexOrdering",     flipVertexOrdering ());
	addField (inputOutput, "deleteSelectedFaces",    deleteSelectedFaces ());
	addField (inputOutput, "cutSnapping",            cutSnapping ());
	addField (outputOnly,  "isActive",               isActive ());
	addField (outputOnly,  "touchTime",              touchTime ());
	addField (outputOnly,  "selectedPoints_changed", selectedPoints_changed ());
	addField (outputOnly,  "selectedEdges_changed",  selectedEdges_changed ());
	addField (outputOnly,  "selectedHoles_changed",  selectedHoles_changed ());
	addField (outputOnly,  "selectedFaces_changed",  selectedFaces_changed ());
	addField (outputOnly,  "clipboard_changed",      clipboard_changed ());
	addField (inputOutput, "brush",                  brush ());
	addField (inputOutput, "normalTool",             normalTool ());
	addField (inputOutput, "coordTool",              coordTool ());
}

void
IndexedFaceSetTool::initialize ()
{
	X3DIndexedFaceSetTool::initialize ();
	X3DIndexedFaceSetSelectionObject::initialize ();
	X3DIndexedFaceSetBrushObject::initialize ();

	X3DIndexedFaceSetTransformObject::initialize ();
	X3DIndexedFaceSetOperationsObject::initialize ();
	X3DIndexedFaceSetKnifeObject::initialize ();
	X3DIndexedFaceSetLightSaberObject::initialize ();
	X3DIndexedFaceSetSculpToolObject::initialize ();
}

void
IndexedFaceSetTool::setExecutionContext (X3DExecutionContext* const executionContext)
{
	X3DIndexedFaceSetTransformObject::setExecutionContext (executionContext);
	X3DIndexedFaceSetSelectionObject::setExecutionContext (executionContext);
	X3DIndexedFaceSetTool::setExecutionContext (executionContext);
}

void
IndexedFaceSetTool::dispose ()
{
	X3DIndexedFaceSetSculpToolObject::dispose ();
	X3DIndexedFaceSetLightSaberObject::dispose ();
	X3DIndexedFaceSetKnifeObject::dispose ();
	X3DIndexedFaceSetOperationsObject::dispose ();
	X3DIndexedFaceSetTransformObject::dispose ();

	X3DIndexedFaceSetBrushObject::dispose ();
	X3DIndexedFaceSetSelectionObject::dispose ();
	X3DIndexedFaceSetTool::dispose ();
}

IndexedFaceSetTool::~IndexedFaceSetTool ()
{ }

} // X3D
} // titania
