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

#ifndef __TITANIA_COMPOSED_WIDGETS_DIRECTION_TOOL_H__
#define __TITANIA_COMPOSED_WIDGETS_DIRECTION_TOOL_H__

#include "../Base/X3DEditorObject.h"
#include "../Configuration/config.h"

namespace titania {
namespace puck {

class DirectionTool :
	public X3DEditorObject
{
public:

	///  @name Construction

	DirectionTool (BrowserWindow* const,
	               Gtk::Box &,
                  const std::string &);

	void
	initialize ();

	///  @name Member access

	void
	setNode (const X3D::SFNode &);

	///  @name Destruction

	virtual
	~DirectionTool ();


private:

	///  @name Construction

	void
	set_initialized ();

	///  @name Event handler

	void
	set_direction (const X3D::SFVec3f &);

	void
	set_field (const X3D::SFVec3f &);

	void
	connect (const X3D::SFVec3f &);

	///  @name Members

	Gtk::Box &        box;
	X3D::BrowserPtr   browser;
	X3D::SFNode       node;
	const std::string name;
	UndoStepPtr       undoStep;

};

inline
DirectionTool::DirectionTool (BrowserWindow* const browserWindow,
                              Gtk::Box & box,
                              const std::string & name) :
	X3DBaseInterface (browserWindow, browserWindow -> getBrowser ()),
	 X3DEditorObject (),
	             box (box),
	         browser (X3D::createBrowser (browserWindow -> getBrowser ())),
	            node (),
	            name (name),
	        undoStep ()
{
	browser -> set_antialiasing (4);

	// Setup

	setup ();
}

inline
void
DirectionTool::initialize ()
{
	box .pack_start (*browser, true, true, 0);

	browser -> show ();
	browser -> initialized () .addInterest (this, &DirectionTool::set_initialized);
}

inline
void
DirectionTool::set_initialized ()
{
	browser -> initialized () .removeInterest (this, &DirectionTool::set_initialized);

	try
	{
		browser -> loadURL ({ get_ui ("Dialogs/DirectionTool.x3dv") });

		const auto tool = browser -> getExecutionContext () -> getNamedNode ("Tool");

		tool -> getField <X3D::SFVec3f> ("direction_changed") .addInterest (this, &DirectionTool::set_direction);
	}
	catch (const X3D::X3DError &)
	{ }

	setNode (node);
}

inline
void
DirectionTool::setNode (const X3D::SFNode & value)
{
	//undoStep .reset ();

	if (node)
	{
		try
		{
			node -> getField <X3D::SFVec3f> (name) .removeInterest (this, &DirectionTool::set_field);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	node = value;
	
	bool hasField = false;

	if (node)
	{
		try
		{
			const auto & field = node -> getField <X3D::SFVec3f> (name);
		
			field .addInterest (this, &DirectionTool::set_field);

			hasField = true;

			set_field (field);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	if (not hasField)
		set_field (X3D::SFVec3f (0, 0, -1));

	browser -> set_sensitive (hasField);
}

inline
void
DirectionTool::set_direction (const X3D::SFVec3f & value)
{
	if (node)
	{
		try
		{
			auto & field = node -> getField <X3D::SFVec3f> (name);

			field .removeInterest (this, &DirectionTool::set_field);
			field .addInterest (this, &DirectionTool::connect);

			addUndoFunction (node, field, undoStep);

			field = value;

			addRedoFunction (field, undoStep);
		}
		catch (const X3D::X3DError &)
		{ }
	}
}

inline
void
DirectionTool::set_field (const X3D::SFVec3f & value)
{
	try
	{
		const auto tool = browser -> getExecutionContext () -> getNamedNode ("Tool");

		tool -> setField <X3D::SFVec3f> ("set_direction", value);
	}
	catch (const X3D::X3DError & error)
	{ }
}

inline
void
DirectionTool::connect (const X3D::SFVec3f & field)
{
	field .removeInterest (this, &DirectionTool::connect);
	field .addInterest (this, &DirectionTool::set_field);
}

inline
DirectionTool::~DirectionTool ()
{
	X3D::removeBrowser (browser);

	dispose ();
}

} // puck
} // titania

#endif
