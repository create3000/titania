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

#include "X3DOutput.h"

#include "../Base/X3DInput.h"
#include <Titania/LOG.h>

namespace titania {
namespace X3D {

X3DOutput::X3DOutput () :
	       X3DBase (),
	    requesters (),
	requesterIndex (),
	        inputs (),
	          copy ()
{ }

const RequesterArray &
X3DOutput::getRequesters () const
{
	return requesters;
}

void
X3DOutput::addInterest (const Requester & function) const
{
	insertInterest (function, nullptr, reinterpret_cast <const void*> (&function));
}

void
X3DOutput::removeInterest (const Requester & function) const
{
	eraseInterest (nullptr, reinterpret_cast <const void*> (&function));
}

bool
X3DOutput::checkInterest (const void* const object, const void* const memberFunction) const
{
	RequesterPair requesterPair (this, memberFunction, object);

	return requesterIndex .find (requesterPair) not_eq requesterIndex .end ();
}

bool
X3DOutput::insertInterest (const Requester & function, const void* const object, const void* const memberFunction) const
{
	// Insert only when not in index.

	RequesterPair requesterPair (this, memberFunction, object);

	if (requesterIndex .find (requesterPair) == requesterIndex .end ())
	{
		requesters .emplace_back (function);
		requesterIndex .emplace (requesterPair, -- requesters .end ());

		return true;
	}

	return false;
}

void
X3DOutput::insertInput (const X3DInput* const input, const void* const memberFunction) const
{
	inputs .emplace (input, memberFunction);

	input -> deleted () .insertDeleter (this, input, memberFunction);
}

void
X3DOutput::eraseInterest (const void* const object, const void* const memberFunction) const
{
	RequesterPair requesterPair (this, memberFunction, object);

	auto requester = requesterIndex .find (requesterPair);

	if (requester not_eq requesterIndex .end ())
	{
		requesters .erase (requester -> second);
		requesterIndex .erase (requester);
	}

	// Allways try to erase input, this output could be disposed.
	inputs .erase (std::make_pair (static_cast <const X3DInput*> (object), memberFunction));
}

void
X3DOutput::eraseInput (const X3DInput* const input, void* const memberFunction) const
{
	input -> deleted () .removeDeleter (this, input, memberFunction);
}

void
X3DOutput::insertDeleter (const X3DOutput* const output, const void* const input, const void* const memberFunction) const
{
	RequesterPair requesterPair (output, input, memberFunction); // Reversed

	auto requester = requesterIndex .find (requesterPair);

	if (requester == requesterIndex .end ())
	{
		requesters .emplace_back (std::bind (&X3DOutput::eraseInterest, output, input, memberFunction));
		requesterIndex .emplace (requesterPair, -- requesters .end ());
	}
}

void
X3DOutput::removeDeleter (const X3DOutput* const output, const void* const input, const void* const memberFunction) const
{
	RequesterPair requesterPair (output, input, memberFunction); // Reversed

	auto requester = requesterIndex .find (requesterPair);

	if (requester not_eq requesterIndex .end ())
	{
		requesters .erase (requester -> second);
		requesterIndex .erase (requester);
	}
}

void
X3DOutput::processInterests () const
{
	if (requesters .empty ())
		return;

	copy .assign (requesters .cbegin (), requesters .cend ());

	for (const auto & requester : copy)
		requester ();
}

void
X3DOutput::dispose ()
{
	requesters .clear ();
	requesterIndex .clear ();

	for (const auto & input : inputs)
		input .first -> deleted () .removeDeleter (this, input .first, input .second);

	inputs .clear ();
}

X3DOutput::~X3DOutput ()
{
	X3DOutput::dispose ();
}

} // X3D
} // titania
