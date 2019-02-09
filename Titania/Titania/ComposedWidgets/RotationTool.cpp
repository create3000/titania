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

#include "RotationTool.h"

#include "../Configuration/config.h"

namespace titania {
namespace puck {

RotationTool::RotationTool (X3DBaseInterface* const editor,
                            Gtk::Box & box,
                            const std::string & name) :
	 X3DBaseInterface (editor -> getBrowserWindow (), editor -> getMasterBrowser ()),
	X3DComposedWidget (editor),
	              box (box),
	          browser (X3D::createBrowser (editor -> getMasterBrowser (), { get_ui ("Editors/RotationTool.x3dv") })),
	            nodes (),
	             name (name),
	         undoStep (),
	           buffer ()
{
	addChildObjects (browser, nodes, buffer);

	// Buffer

	buffer .addInterest (&RotationTool::set_buffer, this);

	// Browser

	browser -> initialized () .addInterest (&RotationTool::set_initialized, this);
	browser -> setAntialiasing (4);
	browser -> show ();

	box .pack_start (*browser, true, true, 0);

	// Setup

	setup ();
}

void
RotationTool::set_initialized ()
{
	browser -> initialized () .removeInterest (&RotationTool::set_initialized, this);

	try
	{
		const auto tool = browser -> getExecutionContext () -> getNamedNode ("Tool");

		tool -> getField <X3D::SFRotation> ("outputRotation") .addInterest (&RotationTool::set_rotation, this);
	}
	catch (const X3D::X3DError &)
	{ }

	setNodes (nodes);
}

void
RotationTool::setNodes (const X3D::MFNode & value)
{
	for (const auto & node : nodes)
	{
		try
		{
			node -> getField <X3D::SFRotation> (name) .removeInterest (&RotationTool::set_field, this);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	nodes = value;

	for (const auto & node : nodes)
	{
		try
		{
			node -> getField <X3D::SFRotation> (name) .addInterest (&RotationTool::set_field, this);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	set_field ();
}

void
RotationTool::set_rotation (const X3D::SFRotation & value)
{
	if (nodes .empty ())
		return;

	addUndoFunction <X3D::SFRotation> (nodes, name, undoStep);

	for (const auto & node : nodes)
	{
		try
		{
			auto & field = node -> getField <X3D::SFRotation> (name);

			field .removeInterest (&RotationTool::set_field, this);
			field .addInterest (&RotationTool::connect, this);

			field = value;
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunction <X3D::SFRotation> (nodes, name, undoStep);
}

void
RotationTool::set_field ()
{
	buffer .addEvent ();
}

void
RotationTool::set_buffer ()
{
	undoStep .reset ();

	bool hasField = false;

	for (const auto & node : basic::make_reverse_range (nodes))
	{
		try
		{
			const auto & field = node -> getField <X3D::SFRotation> (name);

			hasField = true;

			set_value (field);
			break;
		}
		catch (const X3D::X3DError &)
		{ }
	}

	if (not hasField)
		set_value (X3D::SFRotation ());

	browser -> set_sensitive (hasField);
}

void
RotationTool::set_value (const X3D::SFRotation & value)
{
	try
	{
		const auto tool = browser -> getExecutionContext () -> getNamedNode ("Tool");

		tool -> setField <X3D::SFRotation> ("inputRotation", value);
	}
	catch (const X3D::X3DError & error)
	{ }
}

void
RotationTool::connect (const X3D::SFRotation & field)
{
	field .removeInterest (&RotationTool::connect, this);
	field .addInterest (&RotationTool::set_field, this);
}

RotationTool::~RotationTool ()
{
	dispose ();
}

} // puck
} // titania
