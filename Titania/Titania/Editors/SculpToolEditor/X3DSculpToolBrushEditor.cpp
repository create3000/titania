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

#include "X3DSculpToolBrushEditor.h"

#include "../../Configuration/config.h"

#include <Titania/X3D/Components/Core/X3DPrototypeInstance.h>
#include <Titania/X3D/Components/Grouping/Group.h>
#include <Titania/X3D/Components/Layering/X3DLayerNode.h>

namespace titania {
namespace puck {

X3DSculpToolBrushEditor::X3DSculpToolBrushEditor () :
	X3DSculpToolEditorInterface (),
	                       type (this, getBrushTypeButton (), "type"),
	                     radius (this, getBrushRadiusAdjustment (), getBrushRadiusButton (), "radius"),
	                     height (this, getBrushHeightAdjustment (), getBrushHeightButton (), "height"),
	                       warp (this, getBrushWarpAdjustment (), getBrushWarpScale (), "warp"),
	                  sharpness (this, getBrushSharpnessAdjustment (), getBrushSharpnessScale (), "sharpness"),
	                   hardness (this, getBrushHardnessAdjustment (), getBrushHardnessScale (), "hardness"),
	                   pressure (this, getBrushPressureAdjustment (), getBrushPressureScale (), "pressure"),
	                      scale (this, getBrushScaleAdjustment (), getBrushScaleScale (), "scale"),
	                    spacing (this, getBrushSpacingAdjustment (), getBrushSpacingButton (), "spacing"),
	                    preview (X3D::createBrowser (getMasterBrowser (), { get_ui ("Editors/SculpToolBrushPreview.x3dv") })),
	                      brush ()
{
	addChildObjects (preview, brush);
}

void
X3DSculpToolBrushEditor::initialize ()
{
	preview -> initialized () .addInterest (&X3DSculpToolBrushEditor::set_initalized, this);
	preview -> setFixedPipeline (false);
	preview -> setAntialiasing (4);
	preview -> show ();

	getPreviewBox () .pack_start (*preview, true, true, 0);
}

void
X3DSculpToolBrushEditor::configure ()
{ }

void
X3DSculpToolBrushEditor::set_initalized ()
{
	try
	{
		// Create or get brush

		try
		{
			brush = getMasterBrowser () -> getExecutionContext () -> getNamedNode ("SculpToolBrush");
		}
		catch (const X3D::X3DError &)
		{
			brush = getMasterBrowser () -> getExecutionContext () -> createProto ("SculpToolBrush");

			brush -> getField <X3D::SFDouble> ("radius") .setUnit (X3D::UnitCategory::LENGTH);
			brush -> getField <X3D::SFDouble> ("height") .setUnit (X3D::UnitCategory::LENGTH);

			getMasterBrowser () -> getExecutionContext () -> updateNamedNode ("SculpToolBrush", brush);
			getMasterBrowser () -> getExecutionContext () -> getRootNodes () .emplace_back (brush);

			if (getConfig () -> hasItem ("brushType"))
			{
				brush -> setField <X3D::SFString> ("type",      getConfig () -> getItem <X3D::SFString> ("brushType"));
				brush -> setField <X3D::SFDouble> ("radius",    getConfig () -> getItem <X3D::SFDouble> ("brushRadius"));
				brush -> setField <X3D::SFDouble> ("height",    getConfig () -> getItem <X3D::SFDouble> ("brushHeight"));
				brush -> setField <X3D::SFDouble> ("warp",      getConfig () -> getItem <X3D::SFDouble> ("brushWarp"));
				brush -> setField <X3D::SFDouble> ("sharpness", getConfig () -> getItem <X3D::SFDouble> ("brushSharpness"));
				brush -> setField <X3D::SFDouble> ("hardness",  getConfig () -> getItem <X3D::SFDouble> ("brushHardness"));
				brush -> setField <X3D::SFDouble> ("pressure",  getConfig () -> getItem <X3D::SFDouble> ("brushPressure"));
				brush -> setField <X3D::SFDouble> ("scale",     getConfig () -> getItem <X3D::SFDouble> ("brushScale"));
				brush -> setField <X3D::SFDouble> ("spacing" ,  getConfig () -> getItem <X3D::SFDouble> ("brushSpacing"));
			}
		}

		preview -> getExecutionContext () -> getScene () -> getExportedNode <X3D::Group> ("BrushGroup") -> children () = { brush };

		// Event handlers

		brush -> addInterest (&X3D::Browser::addEvent, preview .getValue ());

		getMasterBrowser () -> getExecutionContext () -> bbox_changed () .addInterest (&X3DSculpToolBrushEditor::set_bbox, this);
		preview             -> getExecutionContext () -> bbox_changed () .addInterest (&X3DSculpToolBrushEditor::set_bbox, this);

		set_bbox ();

		// Setup widgets

		const auto nodes = X3D::MFNode ({ brush });

		type      .setNodes (nodes);
		radius    .setNodes (nodes);
		height    .setNodes (nodes);
		warp      .setNodes (nodes);
		sharpness .setNodes (nodes);
		hardness  .setNodes (nodes);
		pressure  .setNodes (nodes);
		scale     .setNodes (nodes);
		spacing   .setNodes (nodes);
	}
	catch (const X3D::X3DError &)
	{ }
}

void
X3DSculpToolBrushEditor::set_bbox ()
{
	if (preview -> getActiveLayer ())
		preview -> getActiveLayer () -> lookAt (1, false, 0);
}

void
X3DSculpToolBrushEditor::store ()
{
	try
	{
		getConfig () -> setItem <X3D::SFString> ("brushType",      brush -> getField <X3D::SFString> ("type"));
		getConfig () -> setItem <X3D::SFDouble> ("brushRadius",    brush -> getField <X3D::SFDouble> ("radius"));
		getConfig () -> setItem <X3D::SFDouble> ("brushHeight",    brush -> getField <X3D::SFDouble> ("height"));
		getConfig () -> setItem <X3D::SFDouble> ("brushWarp",      brush -> getField <X3D::SFDouble> ("warp"));
		getConfig () -> setItem <X3D::SFDouble> ("brushSharpness", brush -> getField <X3D::SFDouble> ("sharpness"));
		getConfig () -> setItem <X3D::SFDouble> ("brushHardness",  brush -> getField <X3D::SFDouble> ("hardness"));
		getConfig () -> setItem <X3D::SFDouble> ("brushPressure",  brush -> getField <X3D::SFDouble> ("pressure"));
		getConfig () -> setItem <X3D::SFDouble> ("brushScale",     brush -> getField <X3D::SFDouble> ("scale"));
		getConfig () -> setItem <X3D::SFDouble> ("brushSpacing",   brush -> getField <X3D::SFDouble> ("spacing"));
	}
	catch (const X3D::X3DError & error)
	{ }
}

X3DSculpToolBrushEditor::~X3DSculpToolBrushEditor ()
{ }

} // puck
} // titania
