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

#ifndef __TITANIA_OPEN_GL_SURFACE_H__
#define __TITANIA_OPEN_GL_SURFACE_H__

// include order is important
#include <gtkmm/drawingarea.h>

#include "Context/WindowContext.h"

#include <memory>
#include <thread>

namespace titania {
namespace opengl {

class Background;

class Surface :
	public Gtk::DrawingArea
{
public:

	virtual
	~Surface ();

	void
	setAntialiasing (const int32_t = 4);

	void
	renderBackground ();

	bool
	makeCurrent () const;

	void
	setSwapInterval (const size_t);

	void
	swapBuffers () const;

	void
	dispose ();


protected:

	Surface (const std::shared_ptr <WindowContext> & = nullptr);

	Surface (const Surface &);

	virtual
	void
	on_style_updated () override;

	/// @name OpenGL handler

	virtual
	void
	construct () = 0;

	virtual
	void
	reshape () = 0;

	virtual
	void
	update () = 0;

	virtual
	void
	on_unrealize () override;


private:

	bool
	glew ();

	bool
	set_map_event (GdkEventAny* const);

	bool
	set_construct (const Cairo::RefPtr <Cairo::Context> &);

	bool
	set_configure_event (GdkEventConfigure* const);

	bool
	set_draw (const Cairo::RefPtr <Cairo::Context> &);

	std::thread::id                 treadId;
	std::shared_ptr <WindowContext> context;
	std::shared_ptr <WindowContext> sharingContext;

	std::unique_ptr <Background> background;

	sigc::connection mapConnection;
	sigc::connection constructConnection;
	sigc::connection drawConnection;

};

} // opengl
} // titania

#endif
