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

#include "geturl.h"

#include <curl/curl.h>
#include <iostream>

namespace titania {
namespace X3D {

static
int
writer (char* data, size_t size, size_t nmemb, std::string* buffer)
{
	// What we will return
	int result = 0;

	// Is there anything in the buffer?
	if (buffer not_eq NULL)
	{
		// Append the data to the buffer
		buffer -> append (data, size * nmemb);

		// How much did we write?
		result = size * nmemb;
	}

	return result;
}

bool
geturl (const std::string & url, std::string & buffer, std::string & userAgent)
{
	char errorBuffer [CURL_ERROR_SIZE];

	// Our curl objects
	CURL*    curl;
	CURLcode result;

	// Create our cURL .handle
	curl = curl_easy_init ();

	if (curl)
	{
		// Now set up all of the curl options
		curl_easy_setopt (curl, CURLOPT_USERAGENT, userAgent .c_str ());
		curl_easy_setopt (curl, CURLOPT_ERRORBUFFER, errorBuffer);
		curl_easy_setopt (curl, CURLOPT_URL, url .c_str ());
		curl_easy_setopt (curl, CURLOPT_SSL_VERIFYPEER, false);
		curl_easy_setopt (curl, CURLOPT_SSL_VERIFYHOST, 2);
		curl_easy_setopt (curl, CURLOPT_HEADER, 0);
		curl_easy_setopt (curl, CURLOPT_FOLLOWLOCATION, 1);
		curl_easy_setopt (curl, CURLOPT_WRITEFUNCTION, writer);
		curl_easy_setopt (curl, CURLOPT_WRITEDATA, &buffer);

		// Attempt to retrieve the remote page
		result = curl_easy_perform (curl);

		// Always cleanup
		curl_easy_cleanup (curl);

		// Did we succeed?
		if (result == CURLE_OK)
			return true;
	}

	return false;
}

} // X3D
} // titania
