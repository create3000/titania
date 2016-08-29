/*
 * FTGL - OpenGL font library
 *
 * Copyright (c) 2001-2004 Henry Maddocks <ftgl@opengl.geek.nz>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "Library.h"

namespace titania {
namespace FTGL {

const Library &
Library::getInstance ()
{
	static Library ftlib;

	return ftlib;
}

Library::~Library ()
{
	if (library not_eq 0)
	{
		FT_Done_FreeType (*library);

		delete library;
		library = 0;
	}

	//  if(manager not_eq 0)
	//  {
	//      FTC_Manager_Done(manager);
	//
	//      delete manager;
	//      manager= 0;
	//  }
}

Library::Library () :
	library (0),
	  error (0)
{
	initialise ();
}

bool
Library::initialise ()
{
	if (library not_eq 0)
		return true;

	library = new FT_Library;

	error = FT_Init_FreeType (library);

	if (error)
	{
		delete library;
		library = 0;
		return false;
	}

	//  FTC_Manager* manager;
	//
	//  if(FTC_Manager_New(lib, 0, 0, 0, my_face_requester, 0, manager)
	//  {
	//      delete manager;
	//      manager= 0;
	//      return false;
	//  }

	return true;
}

} // FTGL
} // titania
