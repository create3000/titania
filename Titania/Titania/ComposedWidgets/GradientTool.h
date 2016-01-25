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

#ifndef __TITANIA_COMPOSED_WIDGETS_GRADIENT_TOOL_H__
#define __TITANIA_COMPOSED_WIDGETS_GRADIENT_TOOL_H__

#include "../ComposedWidgets/X3DComposedWidget.h"
#include "../Configuration/config.h"

namespace titania {
namespace puck {

class GradientTool :
	public X3DComposedWidget
{
public:

	///  @name Construction

	GradientTool (X3DBaseInterface* const,
	              Gtk::Box &,
	              const std::string &,
	              const std::string &);

	///  @name Member access

	void
	setNodes (const X3D::MFNode &);

	const X3D::MFNode &
	getNodes ()
	{ return nodes; }

	///  @name Destruction

	virtual
	~GradientTool ();


protected:

	X3D::SFNode
	getTool () const
	{
		X3D::X3DScenePtr scene (browser -> getExecutionContext ());

		return scene -> getExportedNode ("Tool");
	}


private:

	///  @name Construction

	void
	set_initialized ();

	///  @name Event handler

	void
	set_position (const X3D::MFFloat &);

	void
	set_color (const X3D::MFColor &);

	void
	set_field ();

	void
	set_buffer ();

	virtual
	void
	set_position_value (const X3D::MFFloat &);

	virtual
	void
	set_color_value (const X3D::MFColor &);

	void
	connectPosition (const X3D::MFFloat &);

	void
	connectColor (const X3D::MFColor &);

	///  @name Members

	Gtk::Box &        box;
	X3D::BrowserPtr   browser;
	X3D::MFNode       nodes;
	const std::string positionName;
	const std::string colorName;
	X3D::UndoStepPtr  undoStep;
	X3D::MFColor      buffer;

};

inline
GradientTool::GradientTool (X3DBaseInterface* const editor,
                        Gtk::Box & box,
                        const std::string & positionName,
                        const std::string & colorName) :
	 X3DBaseInterface (editor -> getBrowserWindow (), editor -> getCurrentBrowser ()),
	X3DComposedWidget (editor),
	              box (box),
	          browser (X3D::createBrowser (editor -> getMasterBrowser (), { get_ui ("Editors/GradientTool.x3dv") })),
	            nodes (),
	     positionName (positionName),
	        colorName (colorName),
	         undoStep (),
	           buffer ()
{
	// Buffer

	addChildren (buffer);
	buffer .addInterest (this, &GradientTool::set_buffer);

	// Browser

	browser -> initialized () .addInterest (this, &GradientTool::set_initialized);
	browser -> setAntialiasing (4);
	browser -> show ();

	box .pack_start (*browser, true, true, 0);

	// Setup

	setup ();
}

inline
void
GradientTool::set_initialized ()
{
	browser -> initialized () .removeInterest (this, &GradientTool::set_initialized);

	try
	{
		const auto tool = getTool ();

		tool -> getField <X3D::MFFloat> ("outputPosition") .addInterest (this, &GradientTool::set_position);
		tool -> getField <X3D::MFColor> ("outputColor")    .addInterest (this, &GradientTool::set_color);
	}
	catch (const X3D::X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}

	setNodes (nodes);
}

inline
void
GradientTool::setNodes (const X3D::MFNode & value)
{
	for (const auto & node : nodes)
	{
		try
		{
			node -> getField <X3D::MFFloat> (positionName) .removeInterest (this, &GradientTool::set_field);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	for (const auto & node : nodes)
	{
		try
		{
			node -> getField <X3D::MFColor> (colorName) .removeInterest (this, &GradientTool::set_field);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	nodes = value;

	for (const auto & node : nodes)
	{
		try
		{
			node -> getField <X3D::MFFloat> (positionName) .addInterest (this, &GradientTool::set_field);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	for (const auto & node : nodes)
	{
		try
		{
			node -> getField <X3D::MFColor> (colorName) .addInterest (this, &GradientTool::set_field);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	set_field ();
}

inline
void
GradientTool::set_position (const X3D::MFFloat & value)
{
	__LOG__ << value .size () << " : " << value << std::endl;

	addUndoFunction <X3D::MFFloat> (nodes, positionName, undoStep);

	for (const auto & node : nodes)
	{
		try
		{
			auto & field = node -> getField <X3D::MFFloat> (positionName);

			field .removeInterest (this, &GradientTool::set_field);
			field .addInterest (this, &GradientTool::connectPosition);

			field = value;
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunction <X3D::MFFloat> (nodes, positionName, undoStep);
}

inline
void
GradientTool::set_color (const X3D::MFColor & value)
{
	__LOG__ << value .size () << " : " << value << std::endl;

	addUndoFunction <X3D::MFColor> (nodes, colorName, undoStep);

	for (const auto & node : nodes)
	{
		try
		{
			auto & field = node -> getField <X3D::MFColor> (colorName);

			field .removeInterest (this, &GradientTool::set_field);
			field .addInterest (this, &GradientTool::connectColor);

			field = value;
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunction <X3D::MFColor> (nodes, colorName, undoStep);
}

inline
void
GradientTool::set_field ()
{
	buffer .addEvent ();
}

inline
void
GradientTool::set_buffer ()
{
	undoStep .reset ();

	// Position field

	bool hasPosition = false;

	for (const auto & node : basic::make_reverse_range (nodes))
	{
		try
		{
			const auto & field = node -> getField <X3D::MFFloat> (positionName);

			hasPosition = true;

			set_position_value (field);
			break;
		}
		catch (const X3D::X3DError &)
		{ }
	}

	if (not hasPosition)
		set_position_value (X3D::MFFloat ());

	// Color field

	bool hasColor = false;

	for (const auto & node : basic::make_reverse_range (nodes))
	{
		try
		{
			const auto & field = node -> getField <X3D::MFColor> (colorName);

			hasColor = true;

			set_color_value (field);
			break;
		}
		catch (const X3D::X3DError &)
		{ }
	}

	if (not hasColor)
		set_color_value (X3D::MFColor ());

	browser -> set_sensitive (hasColor);
}

inline
void
GradientTool::set_position_value (const X3D::MFFloat & value)
{
	try
	{
		getTool () -> setField <X3D::MFFloat> ("inputPosition", value);
	}
	catch (const X3D::X3DError & error)
	{ }
}

inline
void
GradientTool::set_color_value (const X3D::MFColor & value)
{
	try
	{
		getTool () -> setField <X3D::MFColor> ("inputColor", value);
	}
	catch (const X3D::X3DError & error)
	{ }
}

inline
void
GradientTool::connectPosition (const X3D::MFFloat & field)
{
	field .removeInterest (this, &GradientTool::connectPosition);
	field .addInterest (this, &GradientTool::set_field);
}

inline
void
GradientTool::connectColor (const X3D::MFColor & field)
{
	field .removeInterest (this, &GradientTool::connectColor);
	field .addInterest (this, &GradientTool::set_field);
}

inline
GradientTool::~GradientTool ()
{
	dispose ();
}

} // puck
} // titania

#endif
