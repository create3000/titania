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

#include "SocketStreamBuf.h"

#include <cstring>
#include <curl/curl.h>
#include <istream>

#include <iostream>

namespace titania {
namespace basic {

socketstreambuf::socketstreambuf (const basic::uri & URL) :
	std::streambuf (),      
	          curl (NULL),  
	        sockfd (-1),    
	        buffer (),      
	        opened (false), 
	           URL (URL),   
	totalBytesRead (0)      
{
	setg (buffer + bufferSize,                    // beginning of putback area
	      buffer + bufferSize,                    // read position
	      buffer + bufferSize);                   // end position
}

socketstreambuf*
socketstreambuf::open ()
{
	CURLcode retcode;

	if (is_open ())
		return NULL;

	if (not URL .host () .size ())
		return NULL;

	// Init CURL.

	curl = curl_easy_init ();

	if (not curl)
		return NULL;

	opened = true;

	// Connect.

	curl_easy_setopt (curl, CURLOPT_URL, URL .root () .str () .c_str ());
	curl_easy_setopt (curl, CURLOPT_CONNECT_ONLY, 1L);

	retcode = curl_easy_perform (curl);

	if (retcode not_eq CURLE_OK)
	{
		std::clog << "CURL Error: " << "Can't open URI: " << std::strerror (retcode) << std::endl;
		close ();
		return NULL;
	}

	// Get socket.

	long socket_addr;
	retcode = curl_easy_getinfo (curl, CURLINFO_LASTSOCKET, &socket_addr);

	sockfd = socket_addr;

	//
	return this;
}

socketstreambuf*
socketstreambuf::send (const std::string & request)
{
	// Wait for the socket to become ready for sending.

	if (not wait (SEND, 60000L))
	{
		std::clog << "CURL Error: " << "timeout" << std::endl;
		close ();
		return NULL;
	}

	// Sending request.

	size_t   iolen; // Bytes send or read.
	CURLcode retcode = curl_easy_send (curl, request .c_str (), request .size (), &iolen);

	if (retcode not_eq CURLE_OK)
	{
		std::clog << "CURL Error: " << "Can't send Request: " << std::strerror (retcode) << std::endl;
		close ();
		return NULL;
	}

	//
	return this;
}

int
socketstreambuf::wait (Direction for_recv, size_t timeout)
{
	struct timeval tv;
	fd_set         infd, outfd, errfd;
	int            res;

	tv .tv_sec  = timeout / 1000;
	tv .tv_usec = (timeout % 1000) * 1000;

	FD_ZERO (&infd);
	FD_ZERO (&outfd);
	FD_ZERO (&errfd);

	FD_SET (sockfd, &errfd); // always check for error

	if (for_recv)
	{
		FD_SET (sockfd, &infd);
	}
	else
	{
		FD_SET (sockfd, &outfd);
	}

	// select() returns the number of signalled sockets or -1 on timeout
	res = select (sockfd + 1, &infd, &outfd, &errfd, &tv);
	return res;
}

socketstreambuf*
socketstreambuf::close ()
{
	if (opened)
	{
		opened = false;

		// Cleanup.

		curl_easy_cleanup (curl);
	}

	return this;
}

int
socketstreambuf::overflow (int c)
{
	std::clog << "socketstreambuf::overflow" << std::endl;

	if (c not_eq EOF)
	{
		*pptr () = c;
		pbump (1);
	}

	return c;
}

int
socketstreambuf::underflow ()   // used for input buffer only
{
	if (gptr () && (gptr () < egptr ()))
		return *reinterpret_cast <unsigned char*> (gptr ());

	if (! opened)
		return EOF;

	// Move buffer to back buffer area.

	(void) memcpy (buffer, buffer + bufferSize, bufferSize);

	// Reading response.

	wait (RECEIVE, 60000L);

	size_t   bytesRead;
	CURLcode retcode = curl_easy_recv (curl, buffer + bufferSize, bufferSize, &bytesRead);

	if (retcode not_eq CURLE_OK)
		return EOF;

	totalBytesRead += bytesRead;

	// reset buffer pointers
	setg (buffer,                           // beginning of putback area
	      buffer + bufferSize,              // read position
	      buffer + bufferSize + bytesRead); // end of buffer

	// return next character
	return *reinterpret_cast <unsigned char*> (gptr ());
}

int
socketstreambuf::sync ()
{
	std::clog << "socketstreambuf::sync" << std::endl;

	if (pptr () && pptr () > pbase ())
	{
		return EOF;
	}

	return 0;
}

} // basic
} // titania
