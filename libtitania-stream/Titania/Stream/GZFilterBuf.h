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

#ifndef __TITANIA_STREAM_GZFILTER_BUF_H__
#define __TITANIA_STREAM_GZFILTER_BUF_H__

#include <cstddef>
#include <cstring>
#include <ios>
#include <stdexcept>
#include <streambuf>
#include <zlib.h>

#include <Titania/LOG.h>

namespace titania {
namespace basic {

template <class CharT,
          class Traits = std::char_traits <CharT>> 
class basic_gzfilterbuf :
	public std::basic_streambuf <CharT, Traits>
{
public:

	typedef typename std::basic_streambuf <CharT, Traits>::pos_type pos_type;
	typedef typename std::basic_streambuf <CharT, Traits>::off_type off_type;

	using std::basic_streambuf <CharT, Traits>::pptr;
	using std::basic_streambuf <CharT, Traits>::gbump;
	using std::basic_streambuf <CharT, Traits>::pbump;
	using std::basic_streambuf <CharT, Traits>::eback;
	using std::basic_streambuf <CharT, Traits>::gptr;
	using std::basic_streambuf <CharT, Traits>::egptr;
	using std::basic_streambuf <CharT, Traits>::pbase;
	using std::basic_streambuf <CharT, Traits>::setg;

	///  throws std::invalid_argument
	basic_gzfilterbuf (std::basic_streambuf <CharT, Traits>*);

	bool
	is_open ()
	{ return opened; }

	///  throws std::invalid_argument
	basic_gzfilterbuf*
	open ();

	basic_gzfilterbuf*
	close ();

	virtual
	typename Traits::int_type
	underflow () final;

	virtual
	pos_type
	seekoff (off_type, std::ios_base::seekdir, std::ios_base::openmode) final;

	virtual
	~basic_gzfilterbuf ()
	{ close (); }


private:

	// Size of put back data buffer & data buffer.
	static constexpr size_t bufferSize = 16384;

	std::basic_streambuf <CharT, Traits>* streambuf;
	CharT                                 buffer [2 * bufferSize]; // Data buffer
	bool                                  opened;

	z_stream zstream;
	CharT    in [bufferSize];
	size_t   bytesRead;
	size_t   lastBytesRead;
	size_t   bytesGone;

};

///  throws std::invalid_argument
template <class CharT, class Traits>
basic_gzfilterbuf <CharT, Traits>::basic_gzfilterbuf (std::basic_streambuf <CharT, Traits>* streambuf) :
	    streambuf (streambuf),
	       opened (false),
	    bytesRead (0),
	lastBytesRead (0),
	    bytesGone (0)
{
	setg (buffer + bufferSize,                                     // beginning of putback area
	      buffer + bufferSize,                                     // read position
	      buffer + bufferSize);                                    // end position

	open ();
}

///  throws std::invalid_argument
template <class CharT, class Traits>
basic_gzfilterbuf <CharT, Traits>*
basic_gzfilterbuf <CharT, Traits>::open ()
{
	if (is_open ())
		return nullptr;

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
		throw std::invalid_argument ("Stream is not compressed.");

	// allocate inflate state

	zstream .zalloc    = Z_NULL;
	zstream .zfree     = Z_NULL;
	zstream .opaque    = Z_NULL;
	zstream .avail_in  = 0;
	zstream .avail_out = bufferSize;
	zstream .next_in   = Z_NULL;

	int ret = inflateInit2 (&zstream, 16 + MAX_WBITS);

	if (ret == Z_OK)
	{
		opened = true;

		return this;
	}

	throw std::invalid_argument ("Could not open stream.");
}

template <class CharT, class Traits>
basic_gzfilterbuf <CharT, Traits>*
basic_gzfilterbuf <CharT, Traits>::close ()
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
basic_gzfilterbuf <CharT, Traits>::underflow () // used for input buffer only
{
	bytesGone += lastBytesRead;

	// Move buffer to back buffer area.

	(void) memcpy (buffer + bufferSize - bytesRead, buffer + bufferSize, bytesRead);

	lastBytesRead = bytesRead;

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
	setg (buffer + bufferSize - lastBytesRead,         // beginning of putback area
	      buffer + bufferSize,                         // read position
	      buffer + bufferSize + bytesRead);            // end of buffer

	// return next character
	return Traits::to_int_type (*gptr ());
}

template <class CharT, class Traits>
typename basic_gzfilterbuf <CharT, Traits>::pos_type
basic_gzfilterbuf <CharT, Traits>::seekoff (off_type off, std::ios_base::seekdir dir, std::ios_base::openmode which)
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

extern template class basic_gzfilterbuf <char>;

typedef basic_gzfilterbuf <char> gzfilterbuf;

} // basic
} // titania

#endif
