// /* -*- Mode: C++; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*- */
// /*************************************************************************
//  *
//  * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
//  *
//  * Copyright 1999, 2012 Holger Seelig <holger.seelig@yahoo.de>.
//  *
//  * Titania - a multi-platform office productivity suite
//  *
//  * This file is part of the Titania Project.
//  *
//  * Titania is free software: you can redistribute it and/or modify
//  * it under the terms of the GNU Lesser General Public License version 3
//  * only, as published by the Free Software Foundation.
//  *
//  * Titania is distributed in the hope that it will be useful,
//  * but WITHOUT ANY WARRANTY; without even the implied warranty of
//  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  * GNU Lesser General Public License version 3 for more details
//  * (a copy is included in the LICENSE file that accompanied this code).
//  *
//  * You should have received a copy of the GNU Lesser General Public License
//  * version 3 along with Titania.  If not, see
//  * <http://www.gnu.org/licenses/lgpl.html>
//  * for a copy of the LGPLv3 License.
//  *
//  ************************************************************************/

#include "UrlStreamBuf.h"

#include <Titania/LOG.h>
#include <cstring>

namespace titania {
namespace basic {

urlstreambuf::urlstreambuf () :
	std::streambuf (),
	        opened (false),
	         m_url (),
	      m_reason (),
	        stream (),
	        buffer (),
	     bytesRead (0),
	 lastBytesRead (0),
	     bytesGone (0)
{
	setg (buffer + bufferSize,                          // beginning of putback area
	      buffer + bufferSize,                          // read position
	      buffer + bufferSize);                         // end position
}

urlstreambuf*
urlstreambuf::open (const basic::uri & URL)
{
	url (URL .add_file_scheme ());

	close ();

	try
	{
		stream = Gio::File::create_for_uri (url () .filename (url () .is_network ())) -> read ();

		opened = true;

		return this;
	}
	catch (const Gio::Error & error)
	{
		reason (error .what ());

		return nullptr;
	}
}

urlstreambuf::traits_type::int_type
urlstreambuf::underflow ()
{
	bytesGone += lastBytesRead;

	// Move buffer to back buffer area.

	(void) memcpy (buffer + bufferSize - bytesRead, buffer + bufferSize, bytesRead);

	lastBytesRead = bytesRead;

	bytesRead = stream -> read (buffer + bufferSize, bufferSize);

	if (bytesRead == 0)
		return traits_type::eof ();

	// reset buffer pointers
	setg (buffer + bufferSize - lastBytesRead,         // beginning of putback area
	      buffer + bufferSize,                         // read position
	      buffer + bufferSize + bytesRead);            // end of buffer

	// return next character
	return traits_type::to_int_type (*gptr ());
}

urlstreambuf::pos_type
urlstreambuf::seekoff (off_type off, std::ios_base::seekdir dir, std::ios_base::openmode which)
{
	if (which == std::ios_base::in)
	{
		if (dir == std::ios_base::cur)
		{
			auto pos = gptr () + off;

			if (pos >= eback () and pos <= egptr ())
			{
				gbump (off);

				return bytesGone + gptr () - eback ();
			}

			throw std::out_of_range ("basic_gzfilterbuf::seekoff");
		}
	}

	throw std::invalid_argument ("basic_gzfilterbuf::seekoff");
}

urlstreambuf*
urlstreambuf::close ()
{
	if (is_open ())
	{
		opened = false;
	}

	return this;
}

} // basic
} // titania
