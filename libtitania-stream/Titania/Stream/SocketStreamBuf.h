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

#ifndef __TITANIA_STREAM_SOCKET_STREAM_BUF_H__
#define __TITANIA_STREAM_SOCKET_STREAM_BUF_H__

#include <Titania/Basic/URI.h>
#include <curl/curl.h>
#include <streambuf>

namespace titania {
namespace basic {

class socketstreambuf :
	public std::streambuf
{
public:

	typedef size_t size_type;

	socketstreambuf ();

	int
	is_open ()
	{ return opened; }

	socketstreambuf*
	open (const basic::uri &);

	socketstreambuf*
	send (const std::string &);

	socketstreambuf*
	close ();

	virtual
	int
	underflow ();

	virtual
	pos_type
	seekoff (off_type, std::ios_base::seekdir, std::ios_base::openmode);

	const basic::uri &
	url () const
	{ return m_url; }

	size_type
	timeout () const
	{ return m_timeout; }

	void
	timeout (size_type value)
	{ m_timeout = value; }

	virtual
	~socketstreambuf ()
	{ close (); }


private:

	socketstreambuf (const socketstreambuf &) = delete;

	socketstreambuf &
	operator = (const socketstreambuf &) = delete;

	void
	url (const basic::uri & value) 
	{ m_url = value; }

	enum Direction
	{
		SEND,
		RECEIVE
	};

	int
	wait (Direction, size_t);

	// Size of put back data buffer & data buffer.
	static constexpr size_type bufferSize = 24;

	CURL*         curl;                    // CURL handle
	curl_socket_t sockfd;                  // Socket file descriptor
	char          buffer [2 * bufferSize]; // Data buffer
	bool          opened;                  // Open/close state of stream

	basic::uri m_url;
	size_type  m_timeout;                  // in ms

	size_type totalBytesRead;
	size_t    bytesRead;
	size_t    lastBytesRead;
	size_t    bytesGone;

};

} // basic
} // titania

#endif
