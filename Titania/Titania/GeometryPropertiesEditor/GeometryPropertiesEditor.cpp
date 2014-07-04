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

#include "../Browser/BrowserWindow.h"
#include "../Configuration/config.h"

namespace titania {
namespace puck {

GeometryPropertiesEditor::GeometryPropertiesEditor (BrowserWindow* const browserWindow) :
	                    X3DBaseInterface (browserWindow, browserWindow -> getBrowser ()),
	X3DGeometryPropertiesEditorInterface (get_ui ("Dialogs/GeometryPropertiesEditor.xml"), gconf_dir ()),
	                       geometryNodes (),
	                            undoStep (),
	                         initialized (false)
{ }

void
GeometryPropertiesEditor::initialize ()
{
	X3DGeometryPropertiesEditorInterface::initialize ();

	getBrowser () -> getSelection () -> getChildren () .addInterest (this, &GeometryPropertiesEditor::set_selection);

	set_selection ();
}

void
GeometryPropertiesEditor::set_selection ()
{
	initialized = false;

	geometryNodes .clear ();
	undoStep .reset ();

	getSolidCheckButton ()  .set_active (false);
	getCCWCheckButton ()    .set_active (false);
	getConvexCheckButton () .set_active (false);
	getCreaseAngleScale ()  .set_value (0);

	// Find X3DGeometryNodes

	auto selection = getBrowser () -> getSelection () -> getChildren ();

	X3D::traverse (selection, [&] (X3D::SFNode & node)
	               {
	                  for (const auto & type: node -> getType ())
	                  {
	                     switch (type)
	                     {
									case X3D::X3DConstants::X3DGeometryNode:
										{
										   geometryNodes .emplace_back (node);
										   break;
										}
									default:
										break;
								}
							}

	                  return true;
						});

	// Initialize sensitivity flags.

	bool hasSolid       = false;
	bool hasCCW         = false;
	bool hasConvex      = false;
	bool hasCreaseAngle = false;

	// Find first »solid« field.

	for (const auto & geometry : geometryNodes)
	{
		try
		{
			getSolidCheckButton () .set_active (geometry -> getField <X3D::SFBool> ("solid"));
			hasSolid = true;
			break;
		}
		catch (const X3D::X3DError &)
		{ }
	}

	// Find first »ccw« field.

	for (const auto & geometry : geometryNodes)
	{
		try
		{
			getCCWCheckButton () .set_active (geometry -> getField <X3D::SFBool> ("ccw"));
			hasCCW = true;
			break;
		}
		catch (const X3D::X3DError &)
		{ }
	}

	// Find first »convex« field.

	for (const auto & geometry : geometryNodes)
	{
		try
		{
			getConvexCheckButton () .set_active (not geometry -> getField <X3D::SFBool> ("convex"));
			hasConvex = true;
			break;
		}
		catch (const X3D::X3DError &)
		{ }
	}

	// Find first »creaseAngle« field.

	for (const auto & geometry : geometryNodes)
	{
		try
		{
			getCreaseAngleScale () .set_value (math::degrees <float> (geometry -> getField <X3D::SFFloat> ("creaseAngle")));
			hasCreaseAngle = true;
			break;
		}
		catch (const X3D::X3DError &)
		{ }
	}

	// Sensitivity

	getSolidCheckButton ()  .set_sensitive (hasSolid);
	getCCWCheckButton ()    .set_sensitive (hasCCW);
	getConvexCheckButton () .set_sensitive (hasConvex);
	getCreaseAngleBox ()    .set_sensitive (hasCreaseAngle);

	initialized = true;
}

void
GeometryPropertiesEditor::on_solid_toggled ()
{
	addUndoFunctions ();

	for (const auto & geometry : geometryNodes)
	{
		try
		{
			geometry -> getField <X3D::SFBool> ("solid") = getSolidCheckButton () .get_active ();
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunctions ();
}

void
GeometryPropertiesEditor::on_ccw_toggled ()
{
	addUndoFunctions ();

	for (const auto & geometry : geometryNodes)
	{
		try
		{
			geometry -> getField <X3D::SFBool> ("ccw") = getCCWCheckButton () .get_active ();
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunctions ();
}

void
GeometryPropertiesEditor::on_convex_toggled ()
{
	addUndoFunctions ();

	for (const auto & geometry : geometryNodes)
	{
		try
		{
			geometry -> getField <X3D::SFBool> ("convex") = not getConvexCheckButton () .get_active ();
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunctions ();
}

void
GeometryPropertiesEditor::on_creaseAngle_changed ()
{
	addUndoFunctions ();

	for (const auto & geometry : geometryNodes)
	{
		try
		{
			geometry -> getField <X3D::SFFloat> ("creaseAngle") = math::radians (getCreaseAngleScale () .get_value ());
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunctions ();
}

void
GeometryPropertiesEditor::addUndoFunctions ()
{
	if (not initialized)
		return;

	const auto lastUndoStep = getBrowserWindow () -> getLastUndoStep ();

	if (undoStep and lastUndoStep == undoStep)
		return;

	undoStep = std::make_shared <UndoStep> (_ ("Geometry Change"));
	
	undoStep -> addVariables (geometryNodes);

	// Undo solid

	for (const auto & geometry : geometryNodes)
	{
		try
		{
			auto & field = geometry -> getField <X3D::SFBool> ("solid");

			undoStep -> addUndoFunction (&X3D::SFBool::setValue, std::ref (field), field .getValue ());
		}
		catch (const X3D::X3DError &)
		{ }
	}

	// Undo ccw

	for (const auto & geometry : geometryNodes)
	{
		try
		{
			auto & field = geometry -> getField <X3D::SFBool> ("ccw");

			undoStep -> addUndoFunction (&X3D::SFBool::setValue, std::ref (field), field .getValue ());
		}
		catch (const X3D::X3DError &)
		{ }
	}

	// Undo convex

	for (const auto & geometry : geometryNodes)
	{
		try
		{
			auto & field = geometry -> getField <X3D::SFBool> ("convex");

			undoStep -> addUndoFunction (&X3D::SFBool::setValue, std::ref (field), field .getValue ());
		}
		catch (const X3D::X3DError &)
		{ }
	}

	// Undo creaseAngle

	for (const auto & geometry : geometryNodes)
	{
		try
		{
			auto & field = geometry -> getField <X3D::SFFloat> ("creaseAngle");

			undoStep -> addUndoFunction (&X3D::SFFloat::setValue, std::ref (field), field .getValue ());
		}
		catch (const X3D::X3DError &)
		{ }
	}

	getBrowserWindow () -> addUndoStep (undoStep);
}

void
GeometryPropertiesEditor::addRedoFunctions ()
{
	if (not initialized)
		return;

	undoStep -> clearRedoFunctions ();

	// Undo solid

	for (const auto & geometry : geometryNodes)
	{
		try
		{
			auto & field = geometry -> getField <X3D::SFBool> ("solid");

			undoStep -> addRedoFunction (&X3D::SFBool::setValue, std::ref (field), field .getValue ());
		}
		catch (const X3D::X3DError &)
		{ }
	}

	// Undo ccw

	for (const auto & geometry : geometryNodes)
	{
		try
		{
			auto & field = geometry -> getField <X3D::SFBool> ("ccw");

			undoStep -> addRedoFunction (&X3D::SFBool::setValue, std::ref (field), field .getValue ());
		}
		catch (const X3D::X3DError &)
		{ }
	}

	// Undo convex

	for (const auto & geometry : geometryNodes)
	{
		try
		{
			auto & field = geometry -> getField <X3D::SFBool> ("convex");

			undoStep -> addRedoFunction (&X3D::SFBool::setValue, std::ref (field), field .getValue ());
		}
		catch (const X3D::X3DError &)
		{ }
	}

	// Undo creaseAngle

	for (const auto & geometry : geometryNodes)
	{
		try
		{
			auto & field = geometry -> getField <X3D::SFFloat> ("creaseAngle");

			undoStep -> addRedoFunction (&X3D::SFFloat::setValue, std::ref (field), field .getValue ());
		}
		catch (const X3D::X3DError &)
		{ }
	}
}

GeometryPropertiesEditor::~GeometryPropertiesEditor ()
{ }

} // puck
} // titania
