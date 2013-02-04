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

#ifndef __TITANIA_STREAM_GZSTREAM_BUF_H__
#define __TITANIA_STREAM_GZSTREAM_BUF_H__

#include <cstddef>
#include <cstring>
#include <streambuf>

#include <zlib.h>

#include <iostream>

namespace titania {
namespace basic {

#define __LOG__ (std::clog << "########## " __FILE__ << ":" << __LINE__ << ": in function '" << __func__ << "': ")

template <class CharT,
          class Traits = std::char_traits <CharT>>
class basic_gzstreambuf :
	public std::basic_streambuf <CharT, Traits>
{
public:

	using std::basic_streambuf <CharT, Traits>::pptr;
	using std::basic_streambuf <CharT, Traits>::pbump;
	using std::basic_streambuf <CharT, Traits>::gptr;
	using std::basic_streambuf <CharT, Traits>::egptr;
	using std::basic_streambuf <CharT, Traits>::pbase;
	using std::basic_streambuf <CharT, Traits>::setg;

	basic_gzstreambuf (std::basic_streambuf <CharT, Traits>*);

	bool
	is_open () { return opened; }

	basic_gzstreambuf*
	open ();

	basic_gzstreambuf*
	close ();

	virtual
	typename Traits::int_type
	overflow (typename Traits::int_type c = EOF);

	virtual
	typename Traits::int_type
	underflow ();

	virtual
	int
	sync ();

	virtual
	~basic_gzstreambuf () { close (); }


private:

	// Size of put back data buffer & data buffer.
	static constexpr size_t bufferSize = 16384;

	std::basic_streambuf <CharT, Traits>* streambuf;
	CharT                                 buffer [2 * bufferSize]; // Data buffer
	bool                                  opened;

	z_stream zstream;
	CharT    in [bufferSize];

};

template <class CharT, class Traits>
basic_gzstreambuf <CharT, Traits>::basic_gzstreambuf (std::basic_streambuf <CharT, Traits>* streambuf) :
	streambuf (streambuf),
	opened (false)
{
	setg (buffer + bufferSize,                                     // beginning of putback area
	      buffer + bufferSize,                                     // read position
	      buffer + bufferSize);                                    // end position

	open ();
}

template <class CharT, class Traits>
basic_gzstreambuf <CharT, Traits>*
basic_gzstreambuf <CharT, Traits>::open ()
{
	if (is_open ())
		return NULL;

	// Test for the magic two-byte gzip header.

	static int GZ_MAGIC1 = 0x1f;
	static int GZ_MAGIC2 = 0x8b;

	bool gz    = false;
	int  first = streambuf -> sgetc ();

	if (first == GZ_MAGIC1)
	{
		if (streambuf -> snextc () == GZ_MAGIC2)
			gz = true;

		streambuf -> sputbackc (first);
	}

	if (not gz)
		return this;

	// allocate inflate state

	zstream .zalloc    = Z_NULL;
	zstream .zfree     = Z_NULL;
	zstream .opaque    = Z_NULL;
	zstream .avail_in  = 0;
	zstream .avail_out = bufferSize;
	zstream .next_in   = Z_NULL;

	int ret = inflateInit2 (&zstream, 16 + MAX_WBITS);

	opened = (ret == Z_OK);

	//
	return this;
}

template <class CharT, class Traits>
basic_gzstreambuf <CharT, Traits>*
basic_gzstreambuf <CharT, Traits>::close ()
{
	if (is_open ())
	{
		opened = false;

		// Cleanup.
		(void) inflateEnd (&zstream);
	}

	return this;
}

template <class CharT, class Traits>
typename Traits::int_type
basic_gzstreambuf <CharT, Traits>::overflow (typename Traits::int_type c)
{
	__LOG__ << std::endl;

	if (c not_eq Traits::eof ())
	{
		*pptr () = c;
		pbump (1);
	}

	return c;
}

template <class CharT, class Traits>
typename Traits::int_type
basic_gzstreambuf <CharT, Traits>::underflow () // used for input buffer only
{
	if (gptr () && (gptr () < egptr ()))
		return *reinterpret_cast <CharT*> (gptr ());

	// Move buffer to back buffer area.

	(void) memcpy (buffer, buffer + bufferSize, bufferSize);

	std::streamsize bytesRead = 0;

	if (is_open ())
	{
		// Decompress until deflate stream ends or end of file.

		// if output buffer not full.
		if (zstream .avail_out not_eq 0)
		{
			zstream .avail_in = streambuf -> sgetn (in, bufferSize);

			if (zstream .avail_in == 0)
				return Traits::eof ();

			zstream .next_in = reinterpret_cast <Bytef*> (in);
		}

		// Run inflate() on input
		{
			zstream .avail_out = bufferSize;
			zstream .next_out  = reinterpret_cast <Bytef*> (buffer + bufferSize);

			int ret = inflate (&zstream, Z_NO_FLUSH);

			switch (ret)
			{
				case Z_STREAM_ERROR:
				case Z_NEED_DICT:
				case Z_DATA_ERROR:
				case Z_MEM_ERROR:
					__LOG__ << std::endl;
					return Traits::eof ();

				default:
					bytesRead = bufferSize - zstream .avail_out;
					break;
			}
		}
	}
	else
		bytesRead = streambuf -> sgetn (buffer + bufferSize, bufferSize);

	if (bytesRead == 0)
		return Traits::eof ();

	// reset buffer pointers
	setg (buffer,                                // beginning of putback area
	      buffer + bufferSize,                   // read position
	      buffer + bufferSize + bytesRead);      // end of buffer

	// return next character
	return *reinterpret_cast <CharT*> (gptr ());
}

template <class CharT, class Traits>
int
basic_gzstreambuf <CharT, Traits>::sync ()
{
	if (pptr () && pptr () > pbase ())
	{
		return Traits::eof ();
	}

	return 0;
}

extern template class basic_gzstreambuf <char>;

} // basic
} // titania

#endif
