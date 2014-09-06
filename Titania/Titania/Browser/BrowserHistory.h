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

#ifndef __TITANIA_BROWSER_BROWSER_HISTORY_H__
#define __TITANIA_BROWSER_BROWSER_HISTORY_H__

#include <Titania/Basic/URI.h>
#include <Titania/InputOutput/Character.h>
#include <Titania/InputOutput/InverseString.h>
#include <Titania/X3D/Base/X3DOutput.h>
#include <Titania/X3D/Browser/X3DBrowser.h>
#include <sigc++/trackable.h>
#include <vector>

namespace titania {
namespace puck {

class BrowserHistory :
	public X3D::X3DOutput,
	public sigc::trackable
{
public:

	using Page = std::pair <std::string, basic::uri>;

	///  @name Construction

	BrowserHistory (X3D::X3DBrowser* const);

	///  @name Member access

	int
	getIndex () const
	{ return index; }

	void
	setIndex (const int);

	const std::vector <Page> &
	getList () const
	{ return list; }

	bool
	hasPreviousPage () const;

	bool
	hasNextPage () const;

	void
	previousPage ();

	void
	nextPage ();

	bool
	isEmpty () const
	{ return list .empty (); }

	size_t
	getSize () const
	{ return list .size (); }

	///  @name Input/Output

	void
	fromString (const std::string &);

	void
	fromStream (std::istream &);

	std::string
	toString () const;

	void
	toStream (std::ostream &) const;


private:

	///  @name Event handler

	void
	set_splashScreen ();

	void
	set_initialized ();

	///  @name Operations

	void
	assign (const int, std::vector <Page> &&);

	void
	addURL (const std::string &, const basic::uri &);

	void
	connect (const X3D::SFTime &);

	///  @name Static members

	struct Grammar
	{
		static const io::character      tab;
		static const io::inverse_string string;
	};

	///  @name Members

	X3D::X3DBrowser* const browser;
	std::vector <Page>     list;
	int                    index;

};

} // puck
} // titania

#endif
