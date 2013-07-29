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

#ifndef __TITANIA_STREAM_INPUT_HTTPSTREAM_H__
#define __TITANIA_STREAM_INPUT_HTTPSTREAM_H__

#include "../Stream/SocketStreamBuf.h"
#include <array>
#include <deque>
#include <istream>
#include <string>
#include <memory>

namespace titania {
namespace basic {
namespace http {

enum method
{
	HEAD,
	GET,
	POST

};

}     // http

class ihttpstream :
	public std::istream
{
public:

	typedef std::array <std::string, 3>         method_array;
	typedef std::map <std::string, std::string> headers_type;
	typedef size_t                              status_type;

	/// @name Constructors

	ihttpstream ();

	ihttpstream (const http::method, const basic::uri &);

	ihttpstream (ihttpstream &&);

	/// @name Properties

	const basic::uri &
	url ()
	{ return buf -> url (); }

	/// @name Connection handling

	void
	open (const http::method, const basic::uri &);

	void
	send ();

	void
	close ();

	/// @name Header handling

	void
	request_header (const std::string &, const std::string &);

	const headers_type &
	request_headers ()
	{ return request_headers_map; }

	const headers_type &
	response_headers ()
	{ return response_headers_map; }

	/// @name Element access

	const method_array
	supported_methods ()
	{ return methods; }

	const std::string &
	http_version ()
	{ return response_http_version; }

	status_type
	status ()
	{ return response_status; }

	const std::string &
	reason ()
	{ return response_reason; }

	/// @name SocketStreamBuffer

	socketstreambuf*
	rdbuf ()
	{ return buf .get (); }

	/// @name Destructor

	virtual
	~ihttpstream ();


private:

	void
	parse_status_line ();

	void
	parse_response_headers ();

	void
	parse_response_header ();
	
	void
	process_status ();
	
	///  @name Members

	static const method_array methods;

	std::unique_ptr <socketstreambuf> buf;

	http::method http_method;

	headers_type request_headers_map;
	headers_type response_headers_map;

	std::string response_http_version;
	status_type response_status;
	std::string response_reason;

};

} // basic
} // titania

#endif
