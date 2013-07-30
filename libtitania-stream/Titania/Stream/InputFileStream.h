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

#ifndef __TITANIA_STREAM_INPUT_FILE_STREAM_H__
#define __TITANIA_STREAM_INPUT_FILE_STREAM_H__

#include "../Stream/InputHTTPStream.h"

#include <fstream>
#include <map>
#include <sstream>

namespace titania {
namespace basic {

class ifilestream :
	public std::istream
{
public:

	typedef basic::ihttpstream::headers_type headers_type;
	typedef basic::ihttpstream::status_type  status_type;

	/// @name Constructors

	ifilestream ();

	ifilestream (const http::method, const basic::uri &, size_t = 60000);

	ifilestream (ifilestream &&);

	ifilestream &
	operator = (ifilestream &&);

	const basic::uri &
	url () const
	{ return m_url; }

	/// @name Connection handling

	void
	open (const http::method, const basic::uri &, size_t = 60000);

	void
	send ();

	void
	close ();

	/// @name Header handling

	void
	request_header (const std::string &, const std::string &);

	const headers_type &
	request_headers () const;

	const headers_type &
	response_headers () const;

	/// @name Element access

	const std::string &
	http_version () const;

	ihttpstream::status_type
	status () const
	{ return m_status; }

	const std::string &
	reason () const;

	size_t
	timeout () const;

	void
	timeout (size_t);

	/// @name Buffer

	std::streambuf*
	rdbuf () const;

	/// @name Destructor

	virtual
	~ifilestream ();


private:

	ifilestream (const ifilestream &) = delete;

	ifilestream &
	operator = (const ifilestream &) = delete;

	void
	url (const basic::uri & value)
	{ m_url = value; }

	void
	status (size_t value)
	{ m_status = value; }

	static const std::string reasons [2];
	static const std::string empty_string;

	std::istringstream* data_istream;
	std::ifstream*      file_istream;
	ihttpstream*        http_istream;
	std::istream*       istream;

	headers_type file_request_headers;
	headers_type file_response_headers;

	basic::uri               m_url;
	ihttpstream::status_type m_status;

};

} // basic
} // titania

#endif
