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

#include "X3DSculpToolBrushPaletteEditor.h"

#include "../../Browser/MagicImport.h"

#include <Titania/X3D/Components/Grouping/Transform.h>
#include <Titania/X3D/Components/Networking/Inline.h>
#include <Titania/X3D/Prototype/ExternProtoDeclaration.h>
#include <Titania/X3D/Thread/SceneFuture.h>

namespace titania {
namespace puck {

X3DSculpToolBrushPaletteEditor::X3DSculpToolBrushPaletteEditor () :
	X3DPaletteEditor <X3DSculpToolEditorInterface> ("SculpToolBrushes"),
	                                        future ()
{
	addChildObjects (future);
}

X3D::SFNode
X3DSculpToolBrushPaletteEditor::getObject (const basic::uri & URL)
{
	try
	{
		const auto inlineNode = getPreview () -> getExecutionContext () -> createNode <X3D::Inline> ();
		const auto transform  = getPreview () -> getExecutionContext () -> createNode <X3D::Transform> ();

		inlineNode -> checkLoadState () .addInterest (&X3DSculpToolBrushPaletteEditor::set_loadState, this, inlineNode .getValue (), transform .getValue ());

		inlineNode -> url ()     = { URL .str () };
		transform -> children () = { inlineNode };
	
		return transform;
	}
	catch (...)
	{
		return nullptr;
	}
}

void
X3DSculpToolBrushPaletteEditor::set_loadState (X3D::Inline* const inlineNode,
                                               X3D::Transform* const transform)
{
	switch (inlineNode -> checkLoadState ())
	{
		case X3D::COMPLETE_STATE:
		{	
			// Observe bbox changes of internal scene.

			inlineNode -> getInternalScene () -> bbox_changed () .addInterest (&X3DSculpToolBrushPaletteEditor::set_bbox, this, inlineNode, transform);

			set_bbox (inlineNode, transform);

			break;
		}
		default:
			break;
	}
}

void
X3DSculpToolBrushPaletteEditor::set_bbox (X3D::Inline* const inlineNode,
                                          X3D::Transform* const transform)
{
	try
	{
		// Center and scale Inline depending on bbox in palette.

		const auto bbox     = inlineNode -> getBBox ();
		const auto brush    = inlineNode -> getInternalScene () -> getNamedNode ("SculpToolBrush");
		const auto diameter = brush -> getField <X3D::SFDouble> ("radius") .getValue () * 2;
		const auto pressure = brush -> getField <X3D::SFDouble> ("pressure") .getValue ();
		const auto display  = X3D::Vector3d (diameter, pressure, diameter);
		const auto size     = bbox .size () / display;
		const auto center   = bbox .center ();
		const auto scale    = X3D::Vector3d (1.8, 1.8, 1.8) / display / maximum_norm (size);

		transform -> translation () = -center * scale;
		transform -> scale ()       = scale;
	}
	catch (const X3D::X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
X3DSculpToolBrushPaletteEditor::setTouchTime (const basic::uri & URL)
{
	try
	{
		using namespace std::placeholders;

		future = getMasterBrowser () -> createX3DFromURL ({ URL .str () }, std::bind (&X3DSculpToolBrushPaletteEditor::set_model, this, _1));
	}
	catch (const X3D::X3DError &)
	{ }
}

void
X3DSculpToolBrushPaletteEditor::set_model (X3D::X3DScenePtr && scene)
{
	try
	{
		if (not scene)
			return;

		const auto   model = scene -> getNamedNode ("SculpToolBrush");
		const auto & brush = getBrush ();

		brush -> setField <X3D::SFString> ("type",      model -> getField <X3D::SFString> ("type"));
		brush -> setField <X3D::SFDouble> ("radius",    model -> getField <X3D::SFDouble> ("radius"));
		brush -> setField <X3D::SFDouble> ("height",    model -> getField <X3D::SFDouble> ("height"));
		brush -> setField <X3D::SFDouble> ("warp",      model -> getField <X3D::SFDouble> ("warp"));
		brush -> setField <X3D::SFDouble> ("sharpness", model -> getField <X3D::SFDouble> ("sharpness"));
		brush -> setField <X3D::SFDouble> ("hardness",  model -> getField <X3D::SFDouble> ("hardness"));
		brush -> setField <X3D::SFDouble> ("pressure",  model -> getField <X3D::SFDouble> ("pressure"));
		brush -> setField <X3D::SFDouble> ("scale",     model -> getField <X3D::SFDouble> ("scale"));
		brush -> setField <X3D::SFDouble> ("spacing",   model -> getField <X3D::SFDouble> ("spacing"));
	}
	catch (const X3D::X3DError & error)
	{
		//__LOG__ << error .what () << std::endl;
	}
}

bool
X3DSculpToolBrushPaletteEditor::createScene (const X3D::X3DScenePtr & scene)
{
	using namespace std::placeholders;

	try
	{
		// Export brush to stream

		const auto brush = X3D::MFNode ({ getBrush () });

		// Export brush to stream

		std::stringstream sstream;
	
		getBrowserWindow () -> exportNodes (X3D::X3DExecutionContextPtr (brush [0] -> getExecutionContext ()), sstream, brush, true);
	
		// Parse exported nodes into scene.
	
		scene -> fromStream (sstream);

		return true;
	}
	catch (const X3D::X3DError &)
	{
		return false;
	}
}

X3DSculpToolBrushPaletteEditor::~X3DSculpToolBrushPaletteEditor ()
{ }

} // puck
} // titania
