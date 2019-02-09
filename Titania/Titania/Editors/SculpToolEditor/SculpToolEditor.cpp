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

#include "SculpToolEditor.h"

#include "../../Browser/BrowserSelection.h"
#include "../../Configuration/config.h"
#include "../../Revealer/GeometryEditor/GeometryEditor.h"

namespace titania {
namespace puck {

SculpToolEditor::SculpToolEditor (X3DBrowserWindow* const browserWindow) :
	              X3DBaseInterface (browserWindow, browserWindow -> getMasterBrowser ()),
	   X3DSculpToolEditorInterface (get_ui ("Editors/SculpToolEditor.glade")),
	       X3DSculpToolBrushEditor (),
	X3DSculpToolBrushPaletteEditor (),
	                         tools ()
{
	addChildObjects (tools);

	auto selectionGroup = getBrowserWindow () -> getHandButton () .get_group ();

	getPullPolygonsButton ()     .set_group (selectionGroup);
	getPushPolygonsButton ()     .set_group (selectionGroup);
	getRoughtenPolygonsButton () .set_group (selectionGroup);
	getSmoothPolygonsButton ()   .set_group (selectionGroup);
	getUndoBrushButton ()        .set_group (selectionGroup);

	setup ();
}

void
SculpToolEditor::initialize ()
{
	X3DSculpToolEditorInterface::initialize ();
	X3DSculpToolBrushEditor::initialize ();
	X3DSculpToolBrushPaletteEditor::initialize ();

	// Brush handling

	getBrush () .addInterest (&SculpToolEditor::set_brush, this);
}

void
SculpToolEditor::configure ()
{
	X3DSculpToolEditorInterface::configure ();
	X3DSculpToolBrushEditor::configure ();
	X3DSculpToolBrushPaletteEditor::configure ();

	if (getConfig () -> hasItem ("paned"))
		getPaned () .set_position (getConfig () -> getItem <int32_t> ("paned"));

	getNotebook () .set_current_page (getConfig () -> getItem <int32_t> ("currentPage"));

	getBrowserWindow () -> getSelection () -> getGeometries () .addInterest (&SculpToolEditor::set_geometries, this);

	set_geometries (getBrowserWindow () -> getSelection () -> getGeometries ());
}

void
SculpToolEditor::set_geometries (const X3D::MFNode & geometryNodes)
{
	tools .clear ();

	tools = getNodes <X3D::X3DBaseNode> (geometryNodes, { X3D::X3DConstants::IndexedFaceSetTool });

	set_tool_brush ();

	getToolbar () .set_sensitive (not tools .empty ());

	if (not tools .empty ())
	{
		on_pull_polygons_toggled ();
		on_push_polygons_toggled ();
		on_roughten_polygons_toggled ();
		on_smooth_polygons_toggled ();
		on_undo_brush_toggled ();
	}
}

void
SculpToolEditor::set_brush ()
{
	try
	{
		getBrush () -> getField <X3D::SFDouble> ("height") .addInterest (&SculpToolEditor::set_height, this);

		set_tool_brush ();
	}
	catch (const X3D::X3DError & error)
	{ }
}

void
SculpToolEditor::set_tool_brush ()
{
	try
	{
		for (const auto & tool : tools)
			tool -> setField <X3D::SFNode> ("brush", getBrush ());
	}
	catch (const X3D::X3DError & error)
	{ }
}

void
SculpToolEditor::set_height (const double height)
{
	if (getPullPolygonsButton () .get_active () or getPushPolygonsButton () .get_active ())
	{
		if (height >= 0)
			getPullPolygonsButton () .set_active (true);
		else
			getPushPolygonsButton () .set_active (true);
	}
}

void
SculpToolEditor::on_pull_polygons_toggled ()
{
	//__LOG__ << getPullPolygonsButton () .get_active () << std::endl;
	//__LOG__ << tools .size () << std::endl;

	try
	{
		if (getPullPolygonsButton () .get_active ())
		{
			getBrowserWindow () -> getSelection () -> setEnabled (true);

			for (const auto & tool : tools)
				tool -> setField <X3D::SFString> ("toolType", "SCULP");
	
			const auto & height = getBrush () -> getField <X3D::SFDouble> ("height");

			if (height < 0.0)
				getBrush () -> setField <X3D::SFDouble> ("height", -height);
		}
	}
	catch (const X3D::X3DError & error)
	{ }
}

void
SculpToolEditor::on_push_polygons_toggled ()
{
	try
	{
		if (getPushPolygonsButton () .get_active ())
		{
			getBrowserWindow () -> getSelection () -> setEnabled (true);

			for (const auto & tool : tools)
				tool -> setField <X3D::SFString> ("toolType", "SCULP");
	
			const auto & height = getBrush () -> getField <X3D::SFDouble> ("height");

			if (height > 0.0)
				getBrush () -> setField <X3D::SFDouble> ("height", -height);
		}
	}
	catch (const X3D::X3DError & error)
	{ }
}

void
SculpToolEditor::on_roughten_polygons_toggled ()
{
	try
	{
		if (getRoughtenPolygonsButton () .get_active ())
		{
			getBrowserWindow () -> getSelection () -> setEnabled (true);

			for (const auto & tool : tools)
				tool -> setField <X3D::SFString> ("toolType", "SCULP_ROUGHENING");
		}
	}
	catch (const X3D::X3DError & error)
	{ }
}

void
SculpToolEditor::on_smooth_polygons_toggled ()
{
	try
	{
		if (getSmoothPolygonsButton () .get_active ())
		{
			getBrowserWindow () -> getSelection () -> setEnabled (true);

			for (const auto & tool : tools)
				tool -> setField <X3D::SFString> ("toolType", "SCULP_SMOOTHING");
		}
	}
	catch (const X3D::X3DError & error)
	{ }
}

void
SculpToolEditor::on_undo_brush_toggled ()
{
	try
	{
		if (getUndoBrushButton () .get_active ())
		{
			getBrowserWindow () -> getSelection () -> setEnabled (true);

			for (const auto & tool : tools)
				tool -> setField <X3D::SFString> ("toolType", "SCULP_UNDO");
		}
	}
	catch (const X3D::X3DError & error)
	{ }
}

void
SculpToolEditor::store ()
{
	getConfig () -> setItem ("paned",       getPaned () .get_position ());
	getConfig () -> setItem ("currentPage", getNotebook () .get_current_page ());

	getBrowserWindow () -> getSelection () -> getGeometries () .removeInterest (&SculpToolEditor::set_geometries, this);

	X3DSculpToolBrushPaletteEditor::store ();
	X3DSculpToolBrushEditor::store ();
	X3DSculpToolEditorInterface::store ();
}

SculpToolEditor::~SculpToolEditor ()
{
	dispose ();
}

} // puck
} // titania
