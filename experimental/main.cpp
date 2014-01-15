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

//#include <Titania/Basic/Geometry/Line3.h>

#include "URI.h"
#include <Titania/Algorithm/Remove.h>
#include <Titania/Basic/Path.h>
#include <Titania/Basic/URI.h>
#include <Titania/Chrono/Now.h>
#include <Titania/InputOutput.h>
#include <Titania/Math/Functional.h>
#include <Titania/Math/Geometry/Box2.h>
#include <Titania/Math/Geometry/Box3.h>
#include <Titania/Math/Geometry/Cylinder3.h>
#include <Titania/Math/Geometry/Line3.h>
#include <Titania/Math/Geometry/Plane3.h>
#include <Titania/Math/Numbers/Matrix3.h>
#include <Titania/Math/Numbers/Matrix4.h>
#include <Titania/Math/Numbers/Rotation4.h>
#include <Titania/Math/Numbers/Vector2.h>
#include <Titania/Math/Numbers/Vector3.h>
#include <Titania/Math/Numbers/Vector4.h>
#include <Titania/OS.h>
#include <Titania/Stream/InputFileStream.h>
#include <Titania/Stream/InputUrlStream.h>
#include <Titania/Utility/Pass.h>
//#include <Titania/Stream/InputHTTPStream.h>

#include <Titania/Algorithm.h>
#include <Titania/LOG.h>

#include <array>
#include <cassert>
#include <complex>
#include <ctime>
#include <deque>
#include <fstream>
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <queue>
#include <regex>
#include <set>
#include <thread>
#include <unordered_set>
#include <vector>

#include <giomm.h>

#include <cstdarg>

using namespace titania;
using namespace titania::basic;

//void
//print_time (double time)
//{
//	std::clog
//		<< std::setiosflags (std::ios::fixed)
//		<< std::setprecision (std::numeric_limits <double>::digits10)
//		<< time
//		<< std::endl;
//}
//
///*basic::ifilestream
// * get_stream ()
// * {
// * titania::TestURI ();
// *
// * basic::uri url0 = std::move (basic::uri ("http://titania.create3000.de"));
// * basic::uri url1 = std::move (basic::uri ("http://titania.create3000.de/documentation/examples/hello_world.wrl"));
// *
// * basic::uri url2 = "file:///home/holger/Projekte/Titania/Titania/share/titania/puck/pages/about/home.wrl";
// * basic::uri url3 = "holger/Titania/Titania/share/titania/puck/pages/about/home.wrl";
// *
// * basic::uri url4 = "http://lockalhost:1235";
// * basic::uri url5 = "about";
// *
// * basic::uri url6 = "urn:iso:std:iso:9999:-1:ed-2:en:amd:1";
// *
// * //
// *
// * //
// *
// * //
// *
// * std::clog << "resolve: " << basic::uri ("h1.wrl",    url0) << std::endl;
// * std::clog << "resolve: " << basic::uri ("../h1.wrl", url0) << std::endl;
// * std::clog << "resolve: " << basic::uri ("/h1.wrl",   url0) << std::endl;
// * std::clog << "resolve: " << basic::uri ("/h1.wrl",   url0) << std::endl;
// *
// * std::clog << "resolve: " << basic::uri ("h1.wrl",    url1) << std::endl;
// * std::clog << "resolve: " << basic::uri ("../h1.wrl", url1) << std::endl;
// * std::clog << "resolve: " << basic::uri ("/h1.wrl",   url1) << std::endl;
// *
// * std::clog << "resolve: " << basic::uri ("h1.wrl",    url2) << std::endl;
// * std::clog << "resolve: " << basic::uri ("../h1.wrl", url2) << std::endl;
// * std::clog << "resolve: " << basic::uri ("/h1.wrl",   url2) << std::endl;
// *
// * std::clog << "resolve: " << basic::uri ("h1.wrl",    url3) << std::endl;
// * std::clog << "resolve: " << basic::uri ("../h1.wrl", url3) << std::endl;
// * std::clog << "resolve: " << basic::uri ("/h1.wrl",   url3) << std::endl;
// *
// * std::clog << "box: " << math::box3 <float> () .max () << std::endl;
// * std::clog << "box: " << math::box3 <float> () .size () << std::endl;
// *
// * //
// *
// * basic::uri         url = "urn:iso:std:iso:9999:-1:ed-2:en:amd:1";
// * basic::ifilestream stream (basic::http::method::GET, url5);
// *
// * if (stream)
// * {
// *    // Request Headers
// *
// *    stream .request_header ("User-Agent", "Horst Jochen Scholz Graupner");
// *
// *    std::clog << "Request Headers" << std::endl;
// *
// *    for (const auto & request_header : stream .request_headers ())
// *       std::clog << request_header .first << ": " << request_header .second << std::endl;
// *
// *    std::clog << std::endl;
// *
// *    // Send
// *
// *    stream .send ();
// * }
// * else
// *    std::clog << "Open fails." << std::endl;
// *
// * return stream;
// * }
// */
//
//void
//test_path (const basic::path & path)
//{
//	std::clog << std::endl;
//	std::clog << "path:   " << path << std::endl;
//	std::clog << "parent: " << path .remove_dot_segments () .parent () << std::endl;
//	std::clog << "remove: " << path .remove_dot_segments () << std::endl;
//
//}

typedef math::quaternion <float> Quaternionf;
typedef math::vector2 <float>    Vector2f;
typedef math::vector3 <float>    Vector3f;
typedef math::vector4 <float>    Vector4f;
typedef math::rotation4 <float>  Rotation4f;
typedef math::matrix4 <float>    Matrix4f;
typedef math::box2 <float>       Box2f;
typedef math::box3 <float>       Box3f;
typedef math::cylinder3 <float>  Cylinder3f;
typedef math::plane3 <float>     Plane3f;
typedef math::line3 <float>      Line3f;
typedef math::sphere3 <float>    Sphere3f;

//#include <v8.h>
//
//// https://www.homepluspower.info/2010/06/v8-javascript-engine-tutorial-part-1.html
//// https://www.homepluspower.info/2010/06/v8-javascript-engine-tutorial-part-2.html
//// http://athile.net/library/wiki/index.php/Library/V8/Tutorial
//// http://www.codeproject.com/Articles/29109/Using-V8-Google-s-Chrome-JavaScript-Virtual-Machin
//// http://v8.googlecode.com/svn/trunk/samples/shell.cc
//// http://stackoverflow.com/questions/11387015/calling-a-v8-javascript-function-from-c-with-an-argument
//
//#include <Titania/InputOutput.h>
//
//bool
//intersect (const Plane3f & p1, const Plane3f & p2, Line3f & line)
//{
//	// http://stackoverflow.com/questions/6408670/intersection-between-two-planes
//
//	if (dot (p1 .normal (), p2 .normal ()) < 1)
//	{
//		Vector3f direction = normalize (cross (p1 .normal (), p2 .normal ()));
//
//		//		float d1 = p1 .distance (c);
//		//		float d2 = p2 .distance (c);
//		//
//		//		Vector3f point = (c - (d1 * p1 .normal ()) - (d2 * p2 .normal ()));
//
//		line = Line3f (Vector3f (), direction);
//
//		return true;
//	}
//
//	// Planes are parallel
//
//	return false;
//}

#define __COUT__ (std::cout << "########## " __FILE__ << ":" << __LINE__ << ": in function '" << __func__ << "': ")

int
main (int argc, char** argv)
{
	std::clog << "Starting main ..." << std::endl;

	#ifdef _GLIBCXX_PARALLEL
	std::clog << "in parallel mode ..." << std::endl;
	#endif
	
	std::vector <float> array (100000);
	
	for (auto & value : array)
		value = rand ();

	double t0 = chrono::now ();
	
	std::sort (array .begin (), array .end ());
	
	std::clog << chrono::now () - t0 << std::endl;

	std::clog << "Function main done." << std::endl;
	exit (0);
	return 0;
}


//// http://www.khronos.org/registry/cl/api/1.1/cl.hpp
//#include "cl.hpp"
//
//
//int
//main (int argc, char** argv)
//{
//	std::clog << "Starting main ..." << std::endl;
//
//	#ifdef _GLIBCXX_PARALLEL
//	std::clog << "in parallel mode ..." << std::endl;
//	#endif
//
//	std::vector <cl::Platform> all_platforms;
//	cl::Platform::get (&all_platforms);
//
//	if (not all_platforms .empty ())
//	{
//		std::cout << "Platforms: " << all_platforms .size () << std::endl;
//		
//		for (const auto & platform : all_platforms)
//			std::cout << "Platform: " << platform .getInfo <CL_PLATFORM_NAME> () << std::endl;
//__COUT__ << std::endl;
//
//		cl::Platform default_platform = all_platforms [0];
//__COUT__ << std::endl;
//
//		//get default device of the default platform
//		std::vector <cl::Device> all_devices;
//		default_platform .getDevices (CL_DEVICE_TYPE_GPU, &all_devices);
//__COUT__ << std::endl;
//
//		if (all_devices .size () == 0)
//		{
//__COUT__ << std::endl;
//			std::cout << " No devices found. Check OpenCL installation!\n";
//			exit (1);
//		}
//__COUT__ << std::endl;
//
//		cl::Device default_device = all_devices [0];
//		std::cout << "Using device: " << default_device .getInfo <CL_DEVICE_NAME> () << "\n";
//__LOG__ << std::endl;
//
//		cl::Context context ({ default_device });
//__COUT__ << std::endl;
//
//		// kernel calculates for each element C=A+B
//		std::string kernel_code = " void kernel simple_add (global const int* A, global const int* B, global int* C) "
//		                          " { "
//		                          "    int i = get_global_id (0); "
//		                          "    C [i] = A [i] + B [i]; "
//		                          " } ";
//
//		cl::Program::Sources sources;
//		sources .push_back ({ kernel_code .c_str (), kernel_code .size () });
//
//		cl::Program program (context, sources);
//
//		if (program .build ({ default_device }) not_eq CL_SUCCESS)
//		{
//			std::cout << " Error building: " << program .getBuildInfo <CL_PROGRAM_BUILD_LOG> (default_device) << "\n";
//			exit (1);
//		}
//
//		// create buffers on the device
//		cl::Buffer buffer_A (context, CL_MEM_READ_WRITE, sizeof (int) * 10);
//		cl::Buffer buffer_B (context, CL_MEM_READ_WRITE, sizeof (int) * 10);
//		cl::Buffer buffer_C (context, CL_MEM_READ_WRITE, sizeof (int) * 10);
//
//		int A [ ] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
//		int B [ ] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
//
//		//create queue to which we will push commands for the device.
//		cl::CommandQueue queue (context, default_device);
//
//		//write arrays A and B to the device
//		queue .enqueueWriteBuffer (buffer_A, CL_TRUE, 0, sizeof (int) * 10, A);
//		queue .enqueueWriteBuffer (buffer_B, CL_TRUE, 0, sizeof (int) * 10, B);
//
//		//run the kernel
//		cl::KernelFunctor simple_add (cl::Kernel (program, "simple_add"), queue, cl::NullRange, cl::NDRange (10), cl::NullRange);
//		simple_add (buffer_A, buffer_B, buffer_C);
//
//		//alternative way to run the kernel
//		/*cl::Kernel kernel_add=cl::Kernel(program,"simple_add");
//		 * kernel_add.setArg(0,buffer_A);
//		 * kernel_add.setArg(1,buffer_B);
//		 * kernel_add.setArg(2,buffer_C);
//		 * queue.enqueueNDRangeKernel(kernel_add,cl::NullRange,cl::NDRange(10),cl::NullRange);
//		 * queue.finish();*/
//
//		int C [10];
//		//read result C from the device to array C
//		queue .enqueueReadBuffer (buffer_C, CL_TRUE, 0, sizeof (int) * 10, C);
//
//		std::cout << " result: \n";
//
//		for (int i = 0; i < 10; i ++)
//		{
//			std::cout << C [i] << " ";
//		}
//	}
//
//	std::clog << "Function main done." << std::endl;
//	exit (0);
//	return 0;
//}

//	test_path (basic::path ("/"));
//	test_path (basic::path ("/", "/"));
//	test_path (basic::path ("home", "/"));
//	test_path (basic::path ("/home/holger/Projekte/Titania/Titania/share/titania/puck///pages/about/home.wrl/", "/"));
//	test_path (basic::path ("home/holger/Projekte/Titania/Titania/share/titania/puck///pages/about/home.wrl/", "/"));
//	test_path (basic::path ("/home/holger/Projekte/../../../.././${Version}/pages/about/home.wrl/..", "/"));

//	basic::ifilestream stream = get_stream ();
//
//	if (stream)
//	{
//		// Response
//
//		std::clog
//			<< "Response" << std::endl
//			<< "Http-Version: " << stream .http_version () << std::endl
//			<< "Status: "       << stream .status ()       << std::endl
//			<< "Reason: "       << stream .reason ()       << std::endl
//			<<  std::endl;
//
//		// Response Headers
//
//		std::clog << "Response Headers" << std::endl;
//		for (const auto & response_header : stream .response_headers ())
//			std::clog << response_header .first << ": " << response_header .second << std::endl;
//		std::clog <<  std::endl;
//
//
//		std::ostringstream osstream;
//		osstream << stream .rdbuf ();
//
//		std::clog << "'" << osstream .str () << "'" << std::endl;
//	}

//	sql::sqlite3 db (os::home () + "/test.db");
//
//	db .query ("DROP TABLE IF EXISTS test");
//	db .query ("CREATE TABLE IF NOT EXISTS test (title TEXT, location TEXT)");
//
//	db .query ("INSERT INTO test (title, location) VALUES ('tit1', 'loc1')");
//	db .query ("INSERT INTO test (title, location) VALUES ('tit2', 'loc2')");
//	db .query ("INSERT INTO test (title, location) VALUES ('tit3', 'loc3')");
//	db .query ("INSERT INTO test (title, location) VALUES ('tit4', 'loc4')");
//	db .query ("INSERT INTO test (title, location) VALUES ('tit5', 'loc5')");
//
//
//	for (const auto & item : db .query_assoc ("SELECT * FROM test"))
//		std::clog << item .at ("title") << ", " << item .at ("location") << std::endl;
//	std::clog << std::endl;
//
//
//	db .query ("INSERT INTO test (title, location) VALUES ('tit6', 'loc6')");
//
//	db .query_array ("UPDATE test SET title = 'tit10', location = 'loc10' WHERE title = 'tit6'");
//
//
//	for (const auto & item : db .query_assoc ("SELECT * FROM test WHERE title = 'tit10'"))
//		std::clog << item .at ("title") << ", " << item .at ("location") << std::endl;
//	std::clog << std::endl;
//

//	gegl_init (&argc, &argv);
//
//
//	const char* path = "/home/holger/Projekte/Titania/Library/Tests/Texturing/images/colors/bw.png";
//	std::ifstream      stream (path);
//	std::ostringstream sstream;
//	sstream << stream .rdbuf ();
//
//	auto image = sstream .str ();
//
//	std::clog << "############" << image .size () << std::endl;
//
//	GeglNode* gegl = gegl_node_new ();
//
//
//	GeglNode* load = gegl_node_new_child (gegl,
//                            "operation", "gegl:load",
//                            "path",      path,
//                            NULL);
//
//	//	GeglNode* scale  = gegl_node_new_child (gegl,
//	//                                    "operation", "gegl:scale",
//	//                                    "width", 200,
//	//                                    "height", 50,
//	//                                    NULL);
//
//	GeglBuffer* outputBuffer;
//	GeglNode*   output = gegl_node_new_child (gegl,
//	                                          "operation", "gegl:buffer-sink",
//	                                          "buffer", &outputBuffer,
//	                                          NULL);
//
//	gegl_node_link_many (load, output, NULL);
//	gegl_node_process (output);
//
//	auto babl = gegl_buffer_get_format (outputBuffer);
//
//	std::clog << babl -> format .components << std::endl;
//	std::clog << gegl_buffer_get_width (outputBuffer) << " " << gegl_buffer_get_height (outputBuffer) << std::endl;
//
//	std::vector <uint16_t> array (gegl_buffer_get_width (outputBuffer) * gegl_buffer_get_height (outputBuffer) * 2);
//
//	gegl_node_blit (load, 1, NULL
//	                 babl_format ("Y u8"),
//	                 array .data (),
//	                 GEGL_AUTO_ROWSTRIDE,
//	                 GEGL_BLIT_DEFAULT);
//
//	g_object_unref (outputBuffer);
//	g_object_unref (gegl);
//	gegl_exit ();

///* This is a very simple example using the multi interface. */
//
//#include <stdio.h>
//#include <string.h>
//
///* somewhat unix-specific */
//#include <sys/time.h>
//#include <unistd.h>
//
///* curl stuff */
//#include <curl/curl.h>
//
//int
//write_data (char* data, size_t size, size_t nmemb, void* self)
//{
//	// What we will return
//	size_t bytes = size * nmemb;
//
//	// Append the data to the buffer
//	__LOG__ << bytes << std::endl;
//	__LOG__ << std::string (data, bytes) << std::endl;
//
//	// How much did we write?
//	return bytes;
//}
//
///*
// * Simply download a HTTP file.
// */
//int
//main (void)
//{
//	CURL*  easy_handle;
//	CURLM* multi_handle;
//
//	int still_running; /* keep number of running handles */
//
//	//curl_global_init (CURL_GLOBAL_DEFAULT);
//
//	easy_handle = curl_easy_init ();
//
//	/* set the options (I left out a few, you'll get the point anyway) */
//	curl_easy_setopt (easy_handle, CURLOPT_URL,               "http://vr.create3000.tv/");
//	curl_easy_setopt (easy_handle, CURLOPT_BUFFERSIZE,        1024);
//	curl_easy_setopt (easy_handle, CURLOPT_USE_SSL,           CURLUSESSL_TRY);
//	curl_easy_setopt (easy_handle, CURLOPT_HEADER,            false);
//	curl_easy_setopt (easy_handle, CURLOPT_FOLLOWLOCATION,    true);
//	curl_easy_setopt (easy_handle, CURLOPT_CONNECTTIMEOUT_MS, 15000);
//	curl_easy_setopt (easy_handle, CURLOPT_TIMEOUT_MS,        15000);
//	curl_easy_setopt (easy_handle, CURLOPT_ACCEPTTIMEOUT_MS,  15000);
//	curl_easy_setopt (easy_handle, CURLOPT_ACCEPT_ENCODING,   "");
//	curl_easy_setopt (easy_handle, CURLOPT_FAILONERROR,       true);
//	curl_easy_setopt (easy_handle, CURLOPT_NOSIGNAL,          true);
//	curl_easy_setopt (easy_handle, CURLOPT_WRITEFUNCTION,     write_data);
//	curl_easy_setopt (easy_handle, CURLOPT_VERBOSE,           true);
//
//	/* init a multi stack */
//	multi_handle = curl_multi_init ();
//
//	/* add the individual transfers */
//	curl_multi_add_handle (multi_handle, easy_handle);
//
//	__LOG__ << std::endl;
//	/* we start some action by calling perform right away */
//	curl_multi_perform (multi_handle, &still_running);
//
//	do
//	{
//		__LOG__ << std::endl;
//
//		struct timeval timeout;
//		int            rc; /* select() return code */
//
//		fd_set fdread;
//		fd_set fdwrite;
//		fd_set fdexcep;
//		int    maxfd = -1;
//
//		long curl_timeout = -1;
//
//		FD_ZERO (&fdread);
//		FD_ZERO (&fdwrite);
//		FD_ZERO (&fdexcep);
//
//		/* set a suitable timeout to play around with */
//		timeout .tv_sec  = 1;
//		timeout .tv_usec = 0;
//
//		curl_multi_timeout (multi_handle, &curl_timeout);
//
//		if (curl_timeout >= 0)
//		{
//			timeout .tv_sec = curl_timeout / 1000;
//
//			if (timeout.tv_sec > 1)
//				timeout .tv_sec = 1;
//			else
//				timeout .tv_usec = (curl_timeout % 1000) * 1000;
//		}
//
//		__LOG__ <<  timeout.tv_sec << std::endl;
//		__LOG__ <<  timeout.tv_usec << std::endl;
//
//		/* get file descriptors from the transfers */
//		curl_multi_fdset (multi_handle, &fdread, &fdwrite, &fdexcep, &maxfd);
//
//		/* In a real-world program you OF COURSE check the return code of the
//		 * function calls.  On success, the value of maxfd is guaranteed to be
//		 * greater or equal than -1.  We call select(maxfd + 1, ...), specially in
//		 * case of (maxfd == -1), we call select(0, ...), which is basically equal
//		 * to sleep. */
//
//		rc = select (maxfd + 1, &fdread, &fdwrite, &fdexcep, &timeout);
//
//		switch (rc)
//		{
//			case -1:
//
//				/* select error */
//				still_running = 0;
//				printf ("select() returns error, this is badness\n");
//				break;
//			case 0:
//			default:
//
//				__LOG__ << std::endl;
//				/* timeout or readable/writable sockets */
//				curl_multi_perform (multi_handle, &still_running);
//				break;
//		}
//	}
//	while (still_running);
//
//	curl_multi_remove_handle (multi_handle, easy_handle);
//
//	curl_easy_cleanup (easy_handle);
//
//	curl_multi_cleanup (multi_handle);
//
//	curl_global_cleanup ();
//
//	return 0;
//}
