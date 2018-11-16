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

#include "X3DGradientTool.h"

#include "../Configuration/config.h"

namespace titania {
namespace puck {

X3DGradientTool::X3DGradientTool (X3DBaseInterface* const editor,
                                  const std::string & description,
                                  Gtk::Box & box,
                                  const std::string & positionName,
                                  const std::string & colorName) :
	 X3DBaseInterface (editor -> getBrowserWindow (), editor -> getCurrentBrowser ()),
	X3DComposedWidget (editor),
	    index_changed (),
	              box (box),
	          browser (X3D::createBrowser (editor -> getMasterBrowser (), { get_ui ("Editors/GradientTool.x3dv") })),
	    positionNodes (),
	       colorNodes (),
	      description (description),
	     positionName (positionName),
	        colorName (colorName),
	         undoStep (),
	            value (),
	           buffer (),
	            index (-1),
	             rgba (false)
{
	// Buffer

	addChildObjects (browser, positionNodes, colorNodes, value, buffer);

	value  .addInterest (&X3DGradientTool::set_value, this);
	buffer .addInterest (&X3DGradientTool::set_buffer, this);

	// Browser

	browser -> initialized () .addInterest (&X3DGradientTool::set_initialized, this);
	browser -> setAntialiasing (4);
	browser -> setPrivateCursor ("ARROW");
	browser -> show ();

	box .pack_start (*browser, true, true, 0);
}

void
X3DGradientTool::set_initialized ()
{
	browser -> initialized () .removeInterest (&X3DGradientTool::set_initialized, this);

	try
	{
		const auto tool = getTool ();

		tool -> getField <X3D::SFTime>      ("outputAddTime")     .addInterest (&X3DGradientTool::set_addTime,     this);
		tool -> getField <X3D::SFTime>      ("outputRemoveTime")  .addInterest (&X3DGradientTool::set_removeTime,  this);
		tool -> getField <X3D::SFBool>      ("isActive")          .addInterest (&X3DGradientTool::set_active,      this);
		tool -> getField <X3D::MFFloat>     ("outputPosition")    .addInterest (&X3DGradientTool::set_position,    this);
		tool -> getField <X3D::MFColorRGBA> ("outputColor")       .addInterest (&X3DGradientTool::set_color,       this);
		tool -> getField <X3D::SFInt32>     ("outputWhichChoice") .addInterest (&X3DGradientTool::set_whichChoice, this);
	}
	catch (const X3D::X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}

	realize ();
	set_field ();
}

void
X3DGradientTool::setIndex (const int32_t value)
{
	//__LOG__ << value << std::endl;

	try
	{
		index = value;

		getTool () -> getField <X3D::SFInt32> ("inputWhichChoice") = value;

		index_changed .emit ();
	}
	catch (const X3D::X3DError & error)
	{ }
}

void
X3DGradientTool::setPositionNodes (const X3D::MFNode & value)
{
	for (const auto & positionNode : positionNodes)
	{
		try
		{
			positionNode -> getField <X3D::MFFloat> (positionName) .removeInterest (&X3DGradientTool::set_field, this);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	positionNodes = value;

	for (const auto & positionNode : positionNodes)
	{
		try
		{
			positionNode -> getField <X3D::MFFloat> (positionName) .addInterest (&X3DGradientTool::set_field, this);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	set_field ();
}

void
X3DGradientTool::setColorNodes (const X3D::MFNode & value)
{
	for (const auto & colorNode : colorNodes)
	{
		try
		{
			if (rgba)
				colorNode -> getField <X3D::MFColorRGBA> (colorName) .removeInterest (&X3DGradientTool::set_field, this);
			else
				colorNode -> getField <X3D::MFColor> (colorName) .removeInterest (&X3DGradientTool::set_field, this);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	colorNodes = value;

	for (const auto & colorNode : colorNodes)
	{
		try
		{
			if (rgba)
				colorNode -> getField <X3D::MFColorRGBA> (colorName) .addInterest (&X3DGradientTool::set_field, this);
			else
				colorNode -> getField <X3D::MFColor> (colorName) .addInterest (&X3DGradientTool::set_field, this);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	set_field ();
}

void
X3DGradientTool::set_position (const X3D::MFFloat &)
{
	value .addEvent ();
}

void
X3DGradientTool::set_color (const X3D::MFColorRGBA &)
{
	value .addEvent ();
}

void
X3DGradientTool::set_value (const X3D::time_type &)
{
	if (positionNodes .empty ())
		return;

	if (colorNodes .empty ())
		return;

	//__LOG__ << undoStep .get () << std::endl;

	beginUndoGroup (description, undoStep);
	addUndoFunction <X3D::MFFloat> (positionNodes, positionName, undoStep);

	if (rgba)
		addUndoFunction <X3D::MFColorRGBA> (colorNodes, colorName, undoStep);
	else
		addUndoFunction <X3D::MFColor> (colorNodes, colorName, undoStep);

	endUndoGroup (description, undoStep);

	try
	{
		const auto   tool  = getTool ();
		const auto & value = tool -> getField <X3D::MFFloat> ("outputPosition");

		//

		for (const auto & positionNode : positionNodes)
		{
			try
			{
				auto & field = positionNode -> getField <X3D::MFFloat> (positionName);

				field .removeInterest (&X3DGradientTool::set_field, this);
				field .addInterest (&X3DGradientTool::connectPosition, this);

				field = get_position (value);
			}
			catch (const X3D::X3DError &)
			{ }
		}
	}
	catch (const X3D::X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}

	//__LOG__ << value .size () << " : " << value << std::endl;

	try
	{
		const auto   tool  = getTool ();
		const auto & value = tool -> getField <X3D::MFColorRGBA> ("outputColor");

		//

		for (const auto & colorNode : colorNodes)
		{
			try
			{
				if (rgba)
				{
					auto & field = colorNode -> getField <X3D::MFColorRGBA> (colorName);
	
					field .removeInterest (&X3DGradientTool::set_field, this);
					field .addInterest (&X3DGradientTool::connectColorRGBA, this);
	
					field = get_color (value);
				}
				else
				{
					auto & field = colorNode -> getField <X3D::MFColor> (colorName);
	
					field .removeInterest (&X3DGradientTool::set_field, this);
					field .addInterest (&X3DGradientTool::connectColor, this);
	
					X3D::MFColor color;

					for (const auto & c : value)
						color .emplace_back (c .r (), c .g (), c .b ());

					field = get_color (color);
				}
			}
			catch (const X3D::X3DError &)
			{ }
		}
	}
	catch (const X3D::X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}

	beginRedoGroup (description, undoStep);
	addRedoFunction <X3D::MFFloat> (positionNodes, positionName, undoStep)
;
	if (rgba)
		addRedoFunction <X3D::MFColorRGBA> (colorNodes, colorName, undoStep);
	else
		addRedoFunction <X3D::MFColor> (colorNodes, colorName, undoStep);

	endRedoGroup (description, undoStep);
}

void
X3DGradientTool::set_active (const bool value)
{
	if (value)
		resetUndoGroup (description, undoStep);
}

void
X3DGradientTool::set_addTime (const X3D::time_type value)
{
	undoStep = std::make_shared <X3D::UndoStep> (_ ("Add Color To Gradient"));

	set_editTime (undoStep);
}

void
X3DGradientTool::set_removeTime (const X3D::time_type value)
{
	undoStep = std::make_shared <X3D::UndoStep> (_ ("Remove Color From Gradient"));

	set_editTime (undoStep);
}

void
X3DGradientTool::set_editTime (const X3D::UndoStepPtr & undoStep)
{
	try
	{
		const auto   tool           = getTool ();
		const auto & outputPosition = tool -> getField <X3D::MFFloat> ("position");
		const auto & outputColor    = tool -> getField <X3D::MFColorRGBA> ("color");

		//

		for (const auto & positionNode : positionNodes)
		{
			try
			{
				X3D::MFFloat & nodePosition = positionNode -> getField <X3D::MFFloat> (positionName);

				undoStep -> addObjects (positionNode);
				undoStep -> addUndoFunction (&X3D::MFFloat::setValue, std::ref (nodePosition), nodePosition);

				nodePosition = get_position (outputPosition);

				undoStep -> addRedoFunction (&X3D::MFFloat::setValue, std::ref (nodePosition), nodePosition);
			}
			catch (const X3D::X3DError &)
			{ }
		}

		for (const auto & colorNode : colorNodes)
		{
			try
			{
				if (rgba)
				{
					X3D::MFColorRGBA & nodeColor = colorNode -> getField <X3D::MFColorRGBA> (colorName);
	
					undoStep -> addObjects (colorNode);
					undoStep -> addUndoFunction (&X3D::MFColorRGBA::setValue, std::ref (nodeColor), nodeColor);
	
					nodeColor = get_color (outputColor);
	
					undoStep -> addRedoFunction (&X3D::MFColorRGBA::setValue, std::ref (nodeColor), nodeColor);
				}
				else
				{
					X3D::MFColor & nodeColor = colorNode -> getField <X3D::MFColor> (colorName);
	
					undoStep -> addObjects (colorNode);
					undoStep -> addUndoFunction (&X3D::MFColor::setValue, std::ref (nodeColor), nodeColor);
		
					X3D::MFColor color;

					for (const auto & c : outputColor)
						color .emplace_back (c .r (), c .g (), c .b ());

					nodeColor = get_color (color);
	
					undoStep -> addRedoFunction (&X3D::MFColor::setValue, std::ref (nodeColor), nodeColor);
				}
			}
			catch (const X3D::X3DError &)
			{ }
		}
	}
	catch (const X3D::X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}

	getBrowserWindow () -> addUndoStep (undoStep);
}

void
X3DGradientTool::set_whichChoice (const X3D::SFInt32 & value)
{
	index = value;

	index_changed .emit ();
}

void
X3DGradientTool::set_field ()
{
	buffer .addEvent ();
}

void
X3DGradientTool::set_buffer ()
{
	resetUndoGroup (description, undoStep);

	// Position field

	X3D::MFFloat     position;
	X3D::MFColor     color;
	X3D::MFColorRGBA colorRGBA;

	for (const auto & positionNode : basic::make_reverse_range (positionNodes))
	{
		try
		{
			position = positionNode -> getField <X3D::MFFloat> (positionName);
			break;
		}
		catch (const X3D::X3DError &)
		{ }
	}

	// Color field

	for (const auto & colorNode : basic::make_reverse_range (colorNodes))
	{
		try
		{
			if (rgba)
				colorRGBA = colorNode -> getField <X3D::MFColorRGBA> (colorName);
			else
				color = colorNode -> getField <X3D::MFColor> (colorName);
			break;
		}
		catch (const X3D::X3DError &)
		{ }
	}

	try
	{
		if (not rgba)
		{
			for (const auto & c : basic::make_const_range (color))
				colorRGBA .emplace_back (c .r (), c .g (), c .b (), 1);
		}

		const auto values = get_tool_values (position, colorRGBA);

		getTool () -> setField <X3D::MFFloat>     ("inputPosition", values .first);
		getTool () -> setField <X3D::MFColorRGBA> ("inputColor",    values .second);
	}
	catch (const X3D::X3DError & error)
	{ }

	browser -> set_sensitive (not positionNodes .empty () and not colorNodes .empty ());
}

std::pair <X3D::MFFloat, X3D::MFColorRGBA>
X3DGradientTool::get_tool_values (const X3D::MFFloat & positionValue, const X3D::MFColorRGBA & colorValue)
{
	return std::pair (positionValue, colorValue);
}

void
X3DGradientTool::connectPosition (const X3D::MFFloat & field)
{
	field .removeInterest (&X3DGradientTool::connectPosition, this);
	field .addInterest (&X3DGradientTool::set_field, this);
}

void
X3DGradientTool::connectColor (const X3D::MFColor & field)
{
	field .removeInterest (&X3DGradientTool::connectColor, this);
	field .addInterest (&X3DGradientTool::set_field, this);
}

void
X3DGradientTool::connectColorRGBA (const X3D::MFColorRGBA & field)
{
	field .removeInterest (&X3DGradientTool::connectColorRGBA, this);
	field .addInterest (&X3DGradientTool::set_field, this);
}

X3DGradientTool::~X3DGradientTool ()
{
	//undoStep .reset ();
}

} // puck
} // titania
