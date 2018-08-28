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

#include "X3DOutput.h"

#include "../Base/X3DInput.h"
#include <Titania/LOG.h>

namespace titania {
namespace X3D {

X3DOutput::X3DOutput () :
	X3DBase (),
	   data ()
{ }

void
X3DOutput::realize () const
{
	if (data)
		return;

	data .reset (new Data ());
}

const RequesterArray &
X3DOutput::getRequesters () const
{
	realize ();
	return data -> requesters;
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
	if (data)
	{
		const RequesterPair requesterPair (this, memberFunction, object);

		return data -> requesterIndex .count (requesterPair);
	}

	return false;
}

bool
X3DOutput::insertInterest (const Requester & function, const void* const object, const void* const memberFunction) const
{
	realize ();

	// Insert only when not in index.

	const RequesterPair requesterPair (this, memberFunction, object);

	if (not data -> requesterIndex .count (requesterPair))
	{
		data -> requesters .emplace_back (function);
		data -> requesterIndex .emplace (requesterPair, -- data -> requesters .end ());

		return true;
	}

	return false;
}

void
X3DOutput::insertInput (const X3DInput* const input, const void* const memberFunction) const
{
	realize ();

	data -> inputs .emplace (input, memberFunction);

	input -> X3DInput::disposed () .insertDeleter (this, input, memberFunction);
}

void
X3DOutput::eraseInterest (const void* const object, const void* const memberFunction) const
{
	if (data)
	{
		const RequesterPair requesterPair (this, memberFunction, object);

		const auto requester = data -> requesterIndex .find (requesterPair);

		// First and allways try to erase input, this output could be dispose.
		data -> inputs .erase (std::make_pair (static_cast <const X3DInput*> (object), memberFunction));

		if (requester not_eq data -> requesterIndex .end ())
		{
			data -> requesters .erase (requester -> second);
			data -> requesterIndex .erase (requester);
		}
	}
}

void
X3DOutput::eraseInput (const X3DInput* const input, void* const memberFunction) const
{
	input -> X3DInput::disposed () .removeDeleter (this, input, memberFunction);
}

void
X3DOutput::insertDeleter (const X3DOutput* const output, const void* const input, const void* const memberFunction) const
{
	realize ();

	const RequesterPair requesterPair (output, input, memberFunction); // Reversed

	const auto requester = data -> requesterIndex .find (requesterPair);

	if (requester == data -> requesterIndex .end ())
	{
		data -> requesters .emplace_back (std::bind (&X3DOutput::eraseInterest, output, input, memberFunction));
		data -> requesterIndex .emplace (requesterPair, -- data -> requesters .end ());
	}
}

void
X3DOutput::removeDeleter (const X3DOutput* const output, const void* const input, const void* const memberFunction) const
{
	if (data)
	{
		const RequesterPair requesterPair (output, input, memberFunction); // Reversed

		const auto requester = data -> requesterIndex .find (requesterPair);

		if (requester not_eq data -> requesterIndex .end ())
		{
			data -> requesters .erase (requester -> second);
			data -> requesterIndex .erase (requester);
		}
	}
}

void
X3DOutput::processInterests () const
{
	if (data)
	{
		// The requesters have to be copied, as the requester could
		// remove itself from the output during the loop call.

		const std::vector <Requester> copy (data -> requesters .cbegin (), data -> requesters .cend ());

		for (const auto & requester : copy)
			requester ();
	}
}

void
X3DOutput::clear ()
{
	while (data)
	{
		std::unique_ptr <Data> temp = std::move (data);

		for (const auto & input : temp -> inputs)
			input .first -> X3DInput::disposed () .removeDeleter (this, input .first, input .second);
	}
}

X3DOutput::~X3DOutput ()
{
	clear ();
}

} // X3D
} // titania
