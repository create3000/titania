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

#include "BrowserHistory.h"

#include <sstream>

namespace titania {
namespace puck {

const io::character      BrowserHistory::Grammar::tab ('\t');
const io::inverse_string BrowserHistory::Grammar::string ("\t");

BrowserHistory::BrowserHistory (X3D::X3DBrowser* const browser) :
	 X3D::X3DOutput (),
	sigc::trackable (),
	        browser (browser),
	           list (),
	          index (-1)
{
	browser -> initialized () .addInterest (this, &BrowserHistory::set_splashScreen);
}

void
BrowserHistory::set_splashScreen ()
{
	browser -> initialized () .removeInterest (this, &BrowserHistory::set_splashScreen);
	browser -> initialized () .addInterest (this, &BrowserHistory::set_initialized);
}

void
BrowserHistory::set_initialized ()
{
	addURL (browser -> getExecutionContext () -> getTitle (), browser -> getWorldURL ());

	processInterests ();
}

void
BrowserHistory::connect (const X3D::SFTime & initialized)
{
	initialized .removeInterest (this, &BrowserHistory::connect);
	initialized .addInterest (this, &BrowserHistory::set_initialized);

	processInterests ();
}

void
BrowserHistory::assign (const int index, std::vector <Page> && list)
{
	this -> index = index;
	this -> list  = std::move (list);
}

void
BrowserHistory::addURL (const std::string & title, const basic::uri & URL)
{
	if (index >= 0 and list [index] .second == URL)
		return;

	list .resize (index + 1);

	list .emplace_back (title, URL);

	++ index;
}

void
BrowserHistory::setIndex (const int value)
{
	if (value < 0 or value >= (int) list .size ())
		return;

	index = value;

	browser -> initialized () .removeInterest (this, &BrowserHistory::set_initialized);
	browser -> initialized () .addInterest (this, &BrowserHistory::connect);

	browser -> loadURL ({ list [index] .second .str () });
}

bool
BrowserHistory::hasPreviousPage () const
{
	return index > 0;
}

bool
BrowserHistory::hasNextPage () const
{
	return index + 1 < (int) list .size ();
}

void
BrowserHistory::previousPage ()
{
	setIndex (index - 1);
}

void
BrowserHistory::nextPage ()
{
	setIndex (index + 1);
}

void
BrowserHistory::fromString (const std::string & string)
{
	std::istringstream isstream (string);

	isstream .imbue (std::locale::classic ());

	fromStream (isstream);
}

void
BrowserHistory::fromStream (std::istream & istream)
{
	int index = -1;

	std::vector <Page> list;

	if (not (istream >> index))
		return;

	if (index < -1)
		return;

	if (index == -1)
	{
		assign (index, std::move (list));
		return;
	}

	if (not Grammar::tab (istream))
		return;

	while (istream)
	{
		std::pair <std::string, std::string> page;

		if (not Grammar::string (istream, page .first))
			return;

		Grammar::string (istream, page .second);

		list .emplace_back (std::move (page));
	}

	if (index < (int) list .size ())
		assign (index, std::move (list));
}

std::string
BrowserHistory::toString () const
{
	std::ostringstream osstream;

	osstream .imbue (std::locale::classic ());

	toStream (osstream);

	return osstream .str ();
}

void
BrowserHistory::toStream (std::ostream & ostream) const
{
	ostream << index;

	for (const auto & pair : list)
		ostream << Grammar::tab () << pair .first << Grammar::tab () << pair .second;
}

} // puck
} // titania
