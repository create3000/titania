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

#ifndef __TITANIA_COMPOSED_WIDGETS_X3DGRADIENT_TOOL_H__
#define __TITANIA_COMPOSED_WIDGETS_X3DGRADIENT_TOOL_H__

#include "../ComposedWidgets/X3DComposedWidget.h"
#include "../Configuration/config.h"

namespace titania {
namespace puck {

class X3DGradientTool :
	public X3DComposedWidget
{
public:

	///  @name Signals

	sigc::signal <void> &
	signal_whichChoice_changed ()
	{ return whichChoice_changed; }

	const sigc::signal <void> &
	signal_whichChoice_changed () const
	{ return whichChoice_changed; }

	///  @name Member access

	void
	setWhichChoice (const int32_t);

	int32_t
	getWhichChoice () const
	{ return whichChoice; }

	void
	setNodes (const X3D::MFNode &);

	const X3D::MFNode &
	getNodes ()
	{ return nodes; }

	void
	addClass (const std::string & name)
	{ browser -> get_style_context () -> add_class (name); }

	///  @name Destruction

	virtual
	~X3DGradientTool ();


protected:

	///  @name Construction

	X3DGradientTool (X3DBaseInterface* const,
	                 const std::string & name,
	                 Gtk::Box &,
	                 const std::string &,
	                 const std::string &);

	///  @name Member access

	X3D::SFNode
	getTool () const
	{
		X3D::X3DScenePtr scene (browser -> getExecutionContext ());

		return scene -> getExportedNode ("Tool");
	}

	virtual
	void
	set_addTime (const X3D::time_type);

	virtual
	void
	set_removeTime (const X3D::time_type);

	virtual
	X3D::MFFloat
	get_position (const X3D::MFFloat & value)
	{ return value; }

	virtual
	X3D::MFColor
	get_color (const X3D::MFColor & value)
	{ return value; }

private:

	///  @name Construction

	void
	set_initialized ();

	virtual
	void
	realize ()
	{ }

	///  @name Event handler


	void
	set_editTime (const X3D::UndoStepPtr &);

	void
	set_active (const bool);

	void
	set_whichChoice (const X3D::SFInt32 &);

	void
	set_position (const X3D::MFFloat &);

	void
	set_color (const X3D::MFColor &);

	void
	set_value (const X3D::time_type &);

	void
	set_field ();

	void
	set_buffer ();

	virtual
	void
	set_tool_values (const X3D::MFFloat &, const X3D::MFColor &);

	void
	connectPosition (const X3D::MFFloat &);

	void
	connectColor (const X3D::MFColor &);

	///  @name Members

	sigc::signal <void> whichChoice_changed;
	Gtk::Box &          box;
	X3D::BrowserPtr     browser;
	X3D::MFNode         nodes;
	const std::string   name;
	const std::string   positionName;
	const std::string   colorName;
	X3D::UndoStepPtr    undoStep;
	X3D::SFTime         value;
	X3D::MFColor        buffer;
	int32_t             whichChoice;

};

inline
X3DGradientTool::X3DGradientTool (X3DBaseInterface* const editor,
                                  const std::string & name,
                                  Gtk::Box & box,
                                  const std::string & positionName,
                                  const std::string & colorName) :
	   X3DBaseInterface (editor -> getBrowserWindow (), editor -> getCurrentBrowser ()),
	  X3DComposedWidget (editor),
	whichChoice_changed (),
	                box (box),
	            browser (X3D::createBrowser (editor -> getMasterBrowser (), { get_ui ("Editors/GradientTool.x3dv") })),
	              nodes (),
	               name (name),
	       positionName (positionName),
	          colorName (colorName),
	           undoStep (),
	              value (),
	             buffer (),
	        whichChoice (-1)
{
	// Buffer

	addChildren (value, buffer);
	value  .addInterest (this, &X3DGradientTool::set_value);
	buffer .addInterest (this, &X3DGradientTool::set_buffer);

	// Browser

	browser -> initialized () .addInterest (this, &X3DGradientTool::set_initialized);
	browser -> setAntialiasing (4);
	browser -> show ();

	box .pack_start (*browser, true, true, 0);

	// Setup

	setup ();
}

inline
void
X3DGradientTool::set_initialized ()
{
	browser -> initialized () .removeInterest (this, &X3DGradientTool::set_initialized);

	try
	{
		const auto tool = getTool ();

		tool -> getField <X3D::SFTime>  ("outputAddTime")     .addInterest (this, &X3DGradientTool::set_addTime);
		tool -> getField <X3D::SFTime>  ("outputRemoveTime")  .addInterest (this, &X3DGradientTool::set_removeTime);
		tool -> getField <X3D::SFBool>  ("isActive")          .addInterest (this, &X3DGradientTool::set_active);
		tool -> getField <X3D::MFFloat> ("outputPosition")    .addInterest (this, &X3DGradientTool::set_position);
		tool -> getField <X3D::MFColor> ("outputColor")       .addInterest (this, &X3DGradientTool::set_color);
		tool -> getField <X3D::SFInt32> ("outputWhichChoice") .addInterest (this, &X3DGradientTool::set_whichChoice);
	}
	catch (const X3D::X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}

	setNodes (nodes);

	realize ();
}

inline
void
X3DGradientTool::setWhichChoice (const int32_t value)
{
	//__LOG__ << value << std::endl;

	try
	{
		whichChoice = value;

		getTool () -> getField <X3D::SFInt32> ("inputWhichChoice") = value;

		whichChoice_changed .emit ();
	}
	catch (const X3D::X3DError & error)
	{ }
}

inline
void
X3DGradientTool::setNodes (const X3D::MFNode & value)
{
	for (const auto & node : nodes)
	{
		try
		{
			node -> getField <X3D::MFFloat> (positionName) .removeInterest (this, &X3DGradientTool::set_field);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	for (const auto & node : nodes)
	{
		try
		{
			node -> getField <X3D::MFColor> (colorName) .removeInterest (this, &X3DGradientTool::set_field);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	nodes = value;

	for (const auto & node : nodes)
	{
		try
		{
			node -> getField <X3D::MFFloat> (positionName) .addInterest (this, &X3DGradientTool::set_field);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	for (const auto & node : nodes)
	{
		try
		{
			node -> getField <X3D::MFColor> (colorName) .addInterest (this, &X3DGradientTool::set_field);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	set_field ();
}

inline
void
X3DGradientTool::set_position (const X3D::MFFloat &)
{
	value .addEvent ();
}

inline
void
X3DGradientTool::set_color (const X3D::MFColor &)
{
	value .addEvent ();
}

inline
void
X3DGradientTool::set_value (const X3D::time_type &)
{
	__LOG__ << undoStep .get () << std::endl;

	beginUndoGroup (name, undoStep);
	addUndoFunction <X3D::MFFloat> (nodes, positionName, undoStep);
	addUndoFunction <X3D::MFColor> (nodes, colorName,    undoStep);
	endUndoGroup (name, undoStep);

	try
	{
		const auto   tool  = getTool ();
		const auto & value = tool -> getField <X3D::MFFloat> ("outputPosition");

		//

		for (const auto & node : nodes)
		{
			try
			{
				auto & field = node -> getField <X3D::MFFloat> (positionName);

				field .removeInterest (this, &X3DGradientTool::set_field);
				field .addInterest (this, &X3DGradientTool::connectPosition);

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
		const auto & value = tool -> getField <X3D::MFColor> ("outputColor");

		//

		for (const auto & node : nodes)
		{
			try
			{
				auto & field = node -> getField <X3D::MFColor> (colorName);

				field .removeInterest (this, &X3DGradientTool::set_field);
				field .addInterest (this, &X3DGradientTool::connectColor);

				field = get_color (value);
			}
			catch (const X3D::X3DError &)
			{ }
		}
	}
	catch (const X3D::X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}

	beginRedoGroup (name, undoStep);
	addRedoFunction <X3D::MFFloat> (nodes, positionName, undoStep);
	addRedoFunction <X3D::MFColor> (nodes, colorName,    undoStep);
	endRedoGroup (name, undoStep);
}

inline
void
X3DGradientTool::set_active (const bool value)
{
	if (value)
		resetUndoGroup (name, undoStep);
}

inline
void
X3DGradientTool::set_addTime (const X3D::time_type value)
{
	undoStep = std::make_shared <X3D::UndoStep> (_ ("Add Color To Gradient"));

	set_editTime (undoStep);
}

inline
void
X3DGradientTool::set_removeTime (const X3D::time_type value)
{
	undoStep = std::make_shared <X3D::UndoStep> (_ ("Remove Color From Gradient"));

	set_editTime (undoStep);
}

inline
void
X3DGradientTool::set_editTime (const X3D::UndoStepPtr & undoStep)
{
	try
	{
		const auto   tool           = getTool ();
		const auto & outputPosition = tool -> getField <X3D::MFFloat> ("position");
		const auto & outputColor    = tool -> getField <X3D::MFColor> ("color");

		//

		for (const auto & node : nodes)
		{
			try
			{
				X3D::MFFloat & nodePosition = node -> getField <X3D::MFFloat> (positionName);
				X3D::MFColor & nodeColor    = node -> getField <X3D::MFColor> (colorName);

				undoStep -> addUndoFunction (&X3D::MFFloat::setValue, std::ref (nodePosition), nodePosition);
				undoStep -> addUndoFunction (&X3D::MFColor::setValue, std::ref (nodeColor),    nodeColor);

				nodePosition = get_position (outputPosition);
				nodeColor    = get_color (outputColor);

				undoStep -> addRedoFunction (&X3D::MFFloat::setValue, std::ref (nodePosition), nodePosition);
				undoStep -> addRedoFunction (&X3D::MFColor::setValue, std::ref (nodeColor),    nodeColor);
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

inline
void
X3DGradientTool::set_whichChoice (const X3D::SFInt32 & value)
{
	whichChoice = value;

	whichChoice_changed .emit ();
}

inline
void
X3DGradientTool::set_field ()
{
	buffer .addEvent ();
}

inline
void
X3DGradientTool::set_buffer ()
{
	//__LOG__ << std::endl;

	resetUndoGroup (name, undoStep);

	// Position field

	X3D::MFFloat position;
	X3D::MFColor color;

	for (const auto & node : basic::make_reverse_range (nodes))
	{
		try
		{
			const auto & field = node -> getField <X3D::MFFloat> (positionName);

			position = field;
			break;
		}
		catch (const X3D::X3DError &)
		{ }
	}

	// Color field

	for (const auto & node : basic::make_reverse_range (nodes))
	{
		try
		{
			const auto & field = node -> getField <X3D::MFColor> (colorName);

			color = field;
			break;
		}
		catch (const X3D::X3DError &)
		{ }
	}

	set_tool_values (position, color);

	browser -> set_sensitive (not nodes .empty ());
}

inline
void
X3DGradientTool::set_tool_values (const X3D::MFFloat & positionValue, const X3D::MFColor & colorValue)
{
	try
	{
		getTool () -> setField <X3D::MFFloat> ("inputPosition", positionValue);
		getTool () -> setField <X3D::MFColor> ("inputColor",    colorValue);
	}
	catch (const X3D::X3DError & error)
	{ }
}

inline
void
X3DGradientTool::connectPosition (const X3D::MFFloat & field)
{
	field .removeInterest (this, &X3DGradientTool::connectPosition);
	field .addInterest (this, &X3DGradientTool::set_field);
}

inline
void
X3DGradientTool::connectColor (const X3D::MFColor & field)
{
	field .removeInterest (this, &X3DGradientTool::connectColor);
	field .addInterest (this, &X3DGradientTool::set_field);
}

inline
X3DGradientTool::~X3DGradientTool ()
{
	dispose ();
}

} // puck
} // titania

#endif
