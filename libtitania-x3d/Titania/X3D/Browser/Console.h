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

#ifndef __TITANIA_X3D_BROWSER_CONSOLE_H__
#define __TITANIA_X3D_BROWSER_CONSOLE_H__

#include "../Basic/X3DBaseNode.h"
#include "../Fields.h"

namespace titania {
namespace X3D {

class Console :
	virtual public X3DBaseNode
{
public:

	///  @name Construction

	Console (X3DExecutionContext* const executionContext);

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const executionContext) const final override;

	///  @name Common members

	virtual
	ComponentType
	getComponent () const final override
	{ return component; }

	virtual
	const std::string &
	getTypeName () const final override
	{ return typeName; }

	virtual
	const std::string &
	getContainerField () const final override
	{ return containerField; }

	///  @name Member access

	const MFString &
	getMessages () const
	{ return messages_changed; }

	///  @name Operations

	template <typename ... Args>
	void
	print (Args && ... args)
	{ append ("print", std::forward <Args> (args) ...); }

	template <typename ... Args>
	void
	log (Args && ... args)
	{ append ("log", std::forward <Args> (args) ...); }

	template <typename ... Args>
	void
	warn (Args && ... args)
	{ append ("warn", std::forward <Args> (args) ...); }

	template <typename ... Args>
	void
	error (Args && ... args)
	{ append ("error", std::forward <Args> (args) ...); }


private:


	///  @name Construction

	void
	initialize ();

	///  @name Operations

	template <typename ... Args>
	void
	append (const std::string & tag, Args && ... args);

	template <typename First, typename ... Args>
	void
	append (std::ostringstream & osstream, First && first, Args && ... args);

	void
	append (std::ostringstream & osstream)
	{ }

	void
	push (const std::string & tag, const std::string & string);

	///  @name Event handlers

	void
	prepareEvents ();

	///  @name Static members

	static const ComponentType component;
	static const std::string   typeName;
	static const std::string   containerField;

	///  @name Members

	std::vector <std::string> messages;
	MFString                  messages_changed;
	std::mutex                mutex;

};

template <typename ... Args>
inline
void
Console::append (const std::string & tag, Args && ... args)
{
	std::ostringstream osstream;

	osstream .imbue (std::locale::classic ());

	append (osstream, std::forward <Args> (args) ...);
	push (tag, osstream .str ());
}

template <typename First, typename ... Args>
inline
void
Console::append (std::ostringstream & osstream, First && first, Args && ... args)
{
	osstream << first;

	append (osstream, std::forward <Args> (args) ...);
}

} // X3D
} // titania

#endif
