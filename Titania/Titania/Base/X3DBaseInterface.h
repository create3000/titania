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

class X3DBrowserWindow;

class X3DBaseInterface :
	virtual public sigc::trackable,
	public X3D::X3DParentObject
{
public:

	/***
	 *  @name Construction
	 */

	virtual
	void
	setup () override;

	/***
	 *  @name Common members
	 */

	///  Returns the type name of this object.
	virtual
	const std::string &
	getTypeName () const
	throw (X3D::Error <X3D::DISPOSED>) override
	{ return typeName; }

	/***
	 *  @name Member access
	 */

	X3DBrowserWindow*
	getBrowserWindow () const
	{ return browserWindow; }

	virtual
	const X3D::BrowserPtr &
	getBrowser () const;

	virtual
	const X3D::X3DScenePtr &
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
	getRootContext () const;

	bool
	inProtoDeclaration () const
	{ return getExecutionContext () -> isProtoDeclaration (); }

	bool
	inPrototypeInstance () const;

	/***
	 *  @name Input/Output
	 */

	///  Extracts the value for this object from @a istream. The contents of @a istream must be in VRML Classic Encoding style.
	virtual
	void
	fromStream (std::istream &)
	throw (X3D::Error <X3D::INVALID_X3D>,
	       X3D::Error <X3D::NOT_SUPPORTED>,
	       X3D::Error <X3D::INVALID_OPERATION_TIMING>,
	       X3D::Error <X3D::DISPOSED>) final override
	{ }

	///  Inserts this object into @a ostream in VRML Classic Encoding style.
	virtual
	void
	toStream (std::ostream &) const final override
	{ }

	///  Inserts this object into @a ostream in X3D XML Encoding style.
	virtual
	void
	toXMLStream (std::ostream &) const final override
	{ }

	/***
	 *  @name Destruction
	 */

	virtual
	~X3DBaseInterface ();


protected:

	/***
	 *  @name Friends
	 */

	friend class X3DBrowserWindow;

	/***
	 *  @name Construction
	 */

	X3DBaseInterface ();

	X3DBaseInterface (X3DBrowserWindow* const, X3D::Browser* const);


private:

	/***
	 *  @name Event handlers
	 */
	 
	void
	set_browser (const X3D::BrowserPtr &);

	/***
	 *  @name Static members
	 */

	static const std::string typeName;

	/***
	 *  @name Members
	 */

	X3DBrowserWindow* const browserWindow;

};

} // puck
} // titania

#endif
