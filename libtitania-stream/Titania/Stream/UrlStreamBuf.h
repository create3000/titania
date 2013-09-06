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

#ifndef __TITANIA_STREAM_URL_STREAM_BUF_H__
#define __TITANIA_STREAM_URL_STREAM_BUF_H__

#include <Titania/Basic/URI.h>
#include <sstream>

extern "C"
{
#include <curl/curl.h>
}

namespace titania {
namespace basic {

class urlstreambuf :
	public std::stringbuf
{
public:

	typedef std::map <std::string, std::string> headers_type;
	typedef size_t                              size_type;

	urlstreambuf ();

	bool
	is_open ()
	{ return opened; }

	urlstreambuf*
	open (const basic::uri &);

	urlstreambuf*
	send (const headers_type & = { });

	urlstreambuf*
	close ();

	const basic::uri &
	url () const
	{ return m_url; }

	size_type
	timeout () const
	{ return m_timeout; }

	void
	timeout (size_type value)
	{ m_timeout = value; }

	std::stringstream &
	headers ()
	{ return m_headers; }

	virtual
	~urlstreambuf ()
	{ close (); }


private:

	void
	url (const basic::uri & value)
	{ m_url = value; }

	static
	int
	write_header (char* data, size_t, size_t, std::stringstream*);

	static
	int
	write_data (char* data, size_t, size_t, urlstreambuf*);

	CURL* curl;                  // CURL handle
	bool  opened;                // Open/close state of stream

	basic::uri        m_url;     // The URL
	size_type         m_timeout; // in ms
	std::stringstream m_headers; // Header data

};

} // basic
} // titania

#endif
