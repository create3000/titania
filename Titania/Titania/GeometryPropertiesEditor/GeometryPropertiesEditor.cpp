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

#include "../Configuration/config.h"

namespace titania {
namespace puck {

GeometryPropertiesEditor::GeometryPropertiesEditor (BrowserWindow* const browserWindow) :
	                    X3DBaseInterface (browserWindow, browserWindow -> getBrowser ()),
	X3DGeometryPropertiesEditorInterface (get_ui ("Dialogs/GeometryPropertiesEditor.xml"), gconf_dir ()),
	                       geometryNodes (),
	                            undoStep (),
	                        currentField (),
	                            changing (false)
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
	for (const auto & geometry : geometryNodes)
	{
		try
		{
			geometry -> getField <X3D::SFFloat> ("solid") .removeInterest (this, &GeometryPropertiesEditor::set_solid);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	for (const auto & geometry : geometryNodes)
	{
		try
		{
			geometry -> getField <X3D::SFFloat> ("ccw") .removeInterest (this, &GeometryPropertiesEditor::set_ccw);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	for (const auto & geometry : geometryNodes)
	{
		try
		{
			geometry -> getField <X3D::SFFloat> ("convex") .removeInterest (this, &GeometryPropertiesEditor::set_convex);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	for (const auto & geometry : geometryNodes)
	{
		try
		{
			geometry -> getField <X3D::SFFloat> ("creaseAngle") .removeInterest (this, &GeometryPropertiesEditor::set_creaseAngle);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	undoStep .reset ();

	// Find X3DGeometryNodes

	auto selection = getBrowser () -> getSelection () -> getChildren ();

	geometryNodes .clear ();

	X3D::traverse (selection, [&] (X3D::SFNode & node)
	               {
	                  for (const auto & type: node -> getType ())
	                  {
	                     switch (type)
	                     {
									case X3D::X3DConstants::X3DGeometryNode:
										{
										   geometryNodes .emplace_back (node);
										   return true;
										}
									default:
										break;
								}
							}

	                  return true;
						});

	for (const auto & geometry : geometryNodes)
	{
		try
		{
			geometry -> getField <X3D::SFFloat> ("solid") .addInterest (this, &GeometryPropertiesEditor::set_solid);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	for (const auto & geometry : geometryNodes)
	{
		try
		{
			geometry -> getField <X3D::SFFloat> ("ccw") .addInterest (this, &GeometryPropertiesEditor::set_ccw);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	for (const auto & geometry : geometryNodes)
	{
		try
		{
			geometry -> getField <X3D::SFFloat> ("convex") .addInterest (this, &GeometryPropertiesEditor::set_convex);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	for (const auto & geometry : geometryNodes)
	{
		try
		{
			geometry -> getField <X3D::SFFloat> ("creaseAngle") .addInterest (this, &GeometryPropertiesEditor::set_creaseAngle);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	set_solid ();
	set_ccw ();
	set_convex ();
	set_creaseAngle ();
}

void
GeometryPropertiesEditor::on_solid_toggled ()
{
	if (changing)
		return;

	addUndoFunction <X3D::SFBool> ("solid");

	for (const auto & geometry : geometryNodes)
	{
		try
		{
			auto & solid = geometry -> getField <X3D::SFBool> ("solid");

			solid = getSolidCheckButton () .get_active ();

			solid .removeInterest (this, &GeometryPropertiesEditor::set_solid);
			solid .addInterest (this, &GeometryPropertiesEditor::connect_solid);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunction <X3D::SFBool> ("solid");
}

void
GeometryPropertiesEditor::set_solid ()
{
	changing = true;

	getSolidCheckButton () .set_active (false);

	// Find first »solid« field.

	bool hasSolid = false;

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

	getSolidCheckButton () .set_sensitive (hasSolid);

	changing = false;
}

void
GeometryPropertiesEditor::connect_solid (const X3D::SFBool & solid)
{
	solid .removeInterest (this, &GeometryPropertiesEditor::connect_solid);
	solid .addInterest (this, &GeometryPropertiesEditor::set_solid);
}

void
GeometryPropertiesEditor::on_ccw_toggled ()
{
	if (changing)
		return;

	addUndoFunction <X3D::SFBool> ("ccw");

	for (const auto & geometry : geometryNodes)
	{
		try
		{
			auto & ccw = geometry -> getField <X3D::SFBool> ("ccw");

			ccw = getCCWCheckButton () .get_active ();

			ccw .removeInterest (this, &GeometryPropertiesEditor::set_ccw);
			ccw .addInterest (this, &GeometryPropertiesEditor::connect_ccw);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunction <X3D::SFBool> ("ccw");
}

void
GeometryPropertiesEditor::set_ccw ()
{
	changing = true;

	getCCWCheckButton () .set_active (false);

	// Find first »ccw« field.

	bool hasCCW = false;

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

	getCCWCheckButton () .set_sensitive (hasCCW);

	changing = false;
}

void
GeometryPropertiesEditor::connect_ccw (const X3D::SFBool & ccw)
{
	ccw .removeInterest (this, &GeometryPropertiesEditor::connect_ccw);
	ccw .addInterest (this, &GeometryPropertiesEditor::set_ccw);
}

void
GeometryPropertiesEditor::on_convex_toggled ()
{
	if (changing)
		return;

	addUndoFunction <X3D::SFBool> ("convex");

	for (const auto & geometry : geometryNodes)
	{
		try
		{
			auto & convex = geometry -> getField <X3D::SFBool> ("convex");

			convex = not getConvexCheckButton () .get_active ();

			convex .removeInterest (this, &GeometryPropertiesEditor::set_convex);
			convex .addInterest (this, &GeometryPropertiesEditor::connect_convex);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunction <X3D::SFBool> ("convex");
}

void
GeometryPropertiesEditor::set_convex ()
{
	changing = true;

	getConvexCheckButton () .set_active (false);

	// Find first »convex« field.

	bool hasConvex = false;

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

	getConvexCheckButton () .set_sensitive (hasConvex);

	changing = false;
}

void
GeometryPropertiesEditor::connect_convex (const X3D::SFBool & convex)
{
	convex .removeInterest (this, &GeometryPropertiesEditor::connect_convex);
	convex .addInterest (this, &GeometryPropertiesEditor::set_convex);
}

void
GeometryPropertiesEditor::on_creaseAngle_changed ()
{
	if (changing)
		return;

	addUndoFunction <X3D::SFFloat> ("creaseAngle");

	for (const auto & geometry : geometryNodes)
	{
		try
		{
			auto & creaseAngle = geometry -> getField <X3D::SFFloat> ("creaseAngle");

			creaseAngle = math::radians (getCreaseAngleScale () .get_value ());

			creaseAngle .removeInterest (this, &GeometryPropertiesEditor::set_creaseAngle);
			creaseAngle .addInterest (this, &GeometryPropertiesEditor::connect_creaseAngle);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunction <X3D::SFFloat> ("creaseAngle");
}

void
GeometryPropertiesEditor::set_creaseAngle ()
{
	changing = true;

	getCreaseAngleScale () .set_value (0);

	// Find first »creaseAngle« field.

	bool hasCreaseAngle = false;

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

	getCreaseAngleBox () .set_sensitive (hasCreaseAngle);

	changing = false;
}

void
GeometryPropertiesEditor::connect_creaseAngle (const X3D::SFFloat & creaseAngle)
{
	creaseAngle .removeInterest (this, &GeometryPropertiesEditor::connect_creaseAngle);
	creaseAngle .addInterest (this, &GeometryPropertiesEditor::set_creaseAngle);
}

GeometryPropertiesEditor::~GeometryPropertiesEditor ()
{ }

} // puck
} // titania
