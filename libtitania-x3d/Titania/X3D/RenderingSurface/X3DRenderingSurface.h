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

#ifndef __TITANIA_X3D_RENDERING_SURFACE_X3DRENDERING_SURFACE_H__
#define __TITANIA_X3D_RENDERING_SURFACE_X3DRENDERING_SURFACE_H__

#include <gtkmm/drawingarea.h>

#include <memory>
#include <set>
#include <thread>

namespace titania {
namespace X3D {

class FrameBuffer;
class RenderingContext;

class X3DRenderingSurface :
	public Gtk::DrawingArea
{
public:

	///  @name Member access

	///  Returns true if OpenGL @a extensions is available, otherwise false.
	bool
	isExtensionAvailable (const std::string & name) const;

	void
	setAntialiasing (const size_t samples)
	{ antialiasing = samples; }

	size_t
	getAntialiasing () const
	{ return antialiasing; }

	void
	setFrameRate (const size_t value)
	{ frameRate = value; }

	size_t
	getFrameRate () const
	{ return frameRate; }

	///  @name Operations

	void
	queue_render ();

	///  @name Signals

	const sigc::signal <void> &
	signal_setup () const
	{ return setupSignal; }

	const sigc::signal <bool, int32_t, int32_t, int32_t, int32_t> &
	signal_reshape () const
	{ return reshapeSignal; }

	const sigc::signal <bool> &
	signal_render () const
	{ return renderSignal; }

	///  @name Destruction

	virtual
	void
	dispose ();

	virtual
	~X3DRenderingSurface () override;


protected:

	///  @name Friends

	friend class ContextLock;

	///  @name Construction

	X3DRenderingSurface ();

	X3DRenderingSurface (X3DRenderingSurface* const other);

	///  @name Operations

	bool
	makeCurrent ();

	///  @name Event handlers

	virtual
	void
	on_style_updated () override;

	virtual
	void
	on_realize () override;

	virtual
	void
	on_setup ();

	virtual
	void
	on_map () override;

	virtual
	bool
	on_configure_event (GdkEventConfigure* const event) override;

	virtual
	void
	on_reshape (const int32_t x, const int32_t y, const int32_t width, const int32_t height);

	virtual
	bool
	on_draw (const Cairo::RefPtr <Cairo::Context> & cairo) override;

	virtual
	bool
	on_render ();

	virtual
	void
	on_unmap () override;

	virtual
	void
	on_unrealize () override;


private:

	///  @name Construction

	X3DRenderingSurface (const std::shared_ptr <RenderingContext> & sharingContext);

	void
	createContext ();

	///  @name Event handler

	bool
	on_timeout ();

	///  @name Members

	const std::thread::id              treadId;
	std::shared_ptr <RenderingContext> context;
	std::shared_ptr <RenderingContext> sharingContext;
	std::set <std::string>             extensions;

	size_t                                                  antialiasing;
	size_t                                                  frameRate;
	std::unique_ptr <FrameBuffer>                           frameBuffer;
	sigc::signal <void>                                     setupSignal;
	sigc::signal <bool, int32_t, int32_t, int32_t, int32_t> reshapeSignal;
	sigc::signal <bool>                                     renderSignal;
	sigc::connection                                        connection;

};

} // X3D
} // titania

#endif
