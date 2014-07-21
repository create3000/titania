/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#ifndef __TITANIA_BASE_X3DBASE_INTERFACE_H__
#define __TITANIA_BASE_X3DBASE_INTERFACE_H__

#include <Titania/X3D.h>
#include <sigc++/trackable.h>

namespace titania {
namespace puck {

class BrowserWindow;

class X3DBaseInterface :
	virtual public X3D::X3DInput,
	virtual public sigc::trackable
{
public:

	///  @name Member access

	BrowserWindow*
	getBrowserWindow () const
	{ return browserWindow; }

	const X3D::BrowserPtr &
	getBrowser () const
	{ return browser; }

	virtual
	const X3D::ScenePtr &
	getScene () const;
	
	const X3D::WorldPtr &
	getWorld () const
	{ return getBrowser () -> getWorld (); }

	virtual
	void
	setExecutionContext (const X3D::X3DExecutionContextPtr &);

	virtual
	const X3D::X3DExecutionContextPtr &
	getExecutionContext () const;

	X3D::X3DExecutionContext*
	getRootContext () const
	{ return getExecutionContext () -> getRootContext (); }

	bool
	inProtoDeclaration () const
	{ return getExecutionContext () -> isProtoDeclaration (); }

	bool
	inPrototypeInstance () const;

	///  @name Destruction

	virtual
	~X3DBaseInterface ();


protected:

	///  @name Construction

	X3DBaseInterface (BrowserWindow* const, const X3D::BrowserPtr &);

	X3DBaseInterface ()
	{ }

	//	///  @name Children handling
	//
	//	template <typename ... Args>
	//	void
	//	addChildren (Args & ... args)
	//	{ basic::pass ((addChild (args), 1) ...); }
	//
	//	void
	//	addChild (X3D::X3DChildObject & child)
	//	{ child .addParent (browser); }


private:

	///  @name Members

	BrowserWindow*  browserWindow;
	X3D::BrowserPtr browser;

};

} // puck
} // titania

#endif
