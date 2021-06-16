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

#ifndef __TITANIA_COMPOSED_WIDGETS_X3DGRADIENT_TOOL_H__
#define __TITANIA_COMPOSED_WIDGETS_X3DGRADIENT_TOOL_H__

#include "../ComposedWidgets/X3DComposedWidget.h"

namespace titania {
namespace puck {

class X3DGradientTool :
	public X3DComposedWidget
{
public:

	///  @name Signals

	sigc::signal <void> &
	signal_index_changed ()
	{ return index_changed; }

	const sigc::signal <void> &
	signal_index_changed () const
	{ return index_changed; }

	///  @name Member access

	void
	setRGBA (const bool value)
	{ rgba = value; }

	bool
	getRGBA () const
	{ return rgba; }

	const std::string &
	getDescription () const
	{ return description; }

	void
	setIndex (const int32_t value);

	int32_t
	getIndex () const
	{ return index; }

	virtual
	void
	setPositionNodes (const X3D::MFNode & value);

	const X3D::MFNode &
	getPositionNodes ()
	{ return positionNodes; }

	virtual
	void
	setColorNodes (const X3D::MFNode & value);

	const X3D::MFNode &
	getColorNodes ()
	{ return colorNodes; }

	void
	addClass (const std::string & name)
	{ browser -> get_style_context () -> add_class (name); }

	///  @name Destruction

	virtual
	~X3DGradientTool () override;


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

	virtual
	X3D::MFColorRGBA
	get_color (const X3D::MFColorRGBA & value)
	{ return value; }


private:

	///  @name Construction

	void
	set_initialized ();

	virtual
	void
	realize ()
	{ }

	///  @name Event handlers


	void
	set_editTime (const X3D::UndoStepPtr &);

	void
	set_active (const bool);

	void
	set_whichChoice (const X3D::SFInt32 &);

	void
	set_position (const X3D::MFFloat &);

	void
	set_color (const X3D::MFColorRGBA &);

	void
	set_value (const X3D::time_type &);

	void
	set_field ();

	void
	set_buffer ();

	virtual
	std::pair <X3D::MFFloat, X3D::MFColorRGBA>
	get_tool_values (const X3D::MFFloat &, const X3D::MFColorRGBA &);

	void
	connectPosition (const X3D::MFFloat &);

	void
	connectColor (const X3D::MFColor &);

	void
	connectColorRGBA (const X3D::MFColorRGBA &);

	///  @name Members

	sigc::signal <void> index_changed;
	Gtk::Box &          box;
	X3D::BrowserPtr     browser;
	X3D::MFNode         positionNodes;
	X3D::MFNode         colorNodes;
	const std::string   description;
	const std::string   positionName;
	const std::string   colorName;
	X3D::UndoStepPtr    undoStep;
	X3D::SFTime         value;
	X3D::MFColorRGBA    buffer;
	int32_t             index;
	bool                rgba;

};

} // puck
} // titania

#endif
