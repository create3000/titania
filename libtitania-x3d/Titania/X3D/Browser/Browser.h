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

#ifndef __TITANIA_X3D_BROWSER_X3DBROWSER_SURFACE_H__
#define __TITANIA_X3D_BROWSER_X3DBROWSER_SURFACE_H__

#include <Titania/OpenGL/Surface.h>

#include "../Browser/X3DBrowser.h"

namespace titania {
namespace X3D {

class KeyDevice;
class PointingDevice;
class X3DViewer;
class X3DSelector;

class Browser :
	public X3DBrowser,
	public opengl::Surface
{
public:

	using X3DBrowser::update;

	///  @name Construction

	Browser (const MFString &, const MFString &);

	Browser (const Browser &, const MFString &, const MFString &);

	Browser*
	create (X3DExecutionContext* const) const;

	///  @name Member access

	void
	setCursor (const Gdk::CursorType value)
	{ cursor = value; }

	const SFEnum <Gdk::CursorType> &
	getCursor () const
	{ return cursor; }

	const X3DPtr <X3DViewer> &
	getViewer () const
	{ return viewer; }

	///  @name Operations

	virtual
	void
	renderBackground () final override
	{ opengl::Surface::renderBackground (); }

	virtual
	bool
	makeCurrent () const final override
	{ return opengl::Surface::makeCurrent (); }

	virtual
	void
	swapBuffers () final override
	{ opengl::Surface::swapBuffers (); }

	///  @name Destruction

	virtual
	void
	dispose () final override;
	
	virtual
	~Browser ();


protected:

	///  @name Construction

	virtual
	void
	initialize () override;


private:

	///  @name Construction

	virtual
	void
	construct () final override
	{ setup (); }

	///  @name Event handler

	virtual
	void
	on_map () override;

	virtual
	void
	on_unmap () override;

	void
	set_cursor (const Gdk::CursorType);

	void
	set_viewer ();

	///  @name Operations

	virtual
	void
	reshape ()
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final override;
	
	virtual
	void
	update ()
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final override;

	///  @name Members

	X3DPtr <X3DViewer>       viewer;
	X3DPtr <KeyDevice>       keyDevice;
	X3DPtr <PointingDevice>  pointingDevice;
	SFEnum <Gdk::CursorType> cursor;
};

} // X3D
} // titania

#endif
