/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*- */
/*******************************************************************************
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
 ******************************************************************************/

#include "uncompress.h"

#include <zlib.h>

namespace titania {
namespace X3D {

std::string
uncompress (const std::string & source)
{
	constexpr auto CHUNK = 16384;

	std::string dest;

	int           ret;
	unsigned      have;
	z_stream      strm;
	unsigned char in [CHUNK];
	unsigned char out [CHUNK];

	/* allocate inflate state */
	strm .zalloc   = Z_NULL;
	strm .zfree    = Z_NULL;
	strm .opaque   = Z_NULL;
	strm .avail_in = 0;
	strm .next_in  = Z_NULL;
	ret            = inflateInit2 (&strm, 16 + MAX_WBITS);

	if (ret not_eq Z_OK)
		return source;

	std::string::size_type sindex = 0;

	/* decompress until deflate stream ends or end of file */
	do
	{
		strm .avail_in = source .copy ((char*) in, CHUNK, sindex);
		sindex        += strm .avail_in;

		if (strm .avail_in == 0)
			break;

		strm .next_in = in;

		/* run inflate() on input until output buffer not full */
		do
		{
			strm .avail_out = CHUNK;
			strm .next_out  = out;

			ret = inflate (&strm, Z_NO_FLUSH);

			switch (ret)
			{
				case Z_STREAM_ERROR:
				case Z_NEED_DICT:
				case Z_DATA_ERROR:
				case Z_MEM_ERROR:
					(void) inflateEnd (&strm);
					return source;
			}

			have = CHUNK - strm .avail_out;

			dest .append ((char*) out, have);

		}
		while (strm .avail_out == 0);

		/* done when inflate() says it's done */
	}
	while (ret not_eq Z_STREAM_END);

	/* clean up and return */
	(void) inflateEnd (&strm);
	return ret == Z_STREAM_END ? dest : source;
}

} // X3D
} // titania
