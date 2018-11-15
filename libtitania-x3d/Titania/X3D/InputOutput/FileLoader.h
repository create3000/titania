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

#ifndef __TITANIA_X3D_INPUT_OUTPUT_FILE_LOADER_H__
#define __TITANIA_X3D_INPUT_OUTPUT_FILE_LOADER_H__

#include "../Execution/X3DScene.h"

#include <Titania/Stream/InputFileStream.h>

namespace titania {
namespace X3D {

class FileLoader
{
public:

	FileLoader (X3DExecutionContext* const executionContext);

	FileLoader (X3DExecutionContext* const, const basic::uri &);

	///  @name Member access

	const basic::uri &
	getReferer () const
	{ return referer; }

	const basic::uri &
	getWorldURL () const
	{ return worldURL; }

	const MFString &
	getUrlError () const
	{ return urlError; }

	///  @name X3D Creation Handling

	///  throws Error <INVALID_X3D>, Error <NOT_SUPPORTED>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	X3DScenePtr
	createX3DFromString (const std::string & string);

	///  throws Error <INVALID_X3D>, Error <NOT_SUPPORTED>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	X3DScenePtr
	createX3DFromStream (basic::ifilestream & istream);

	///  throws Error <INVALID_X3D>, Error <NOT_SUPPORTED>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	X3DScenePtr
	createX3DFromStream (const basic::uri & worldURL, basic::ifilestream &);

	///  throws Error <INVALID_URL>, Error <URL_UNAVAILABLE>
	X3DScenePtr
	createX3DFromURL (const MFString & url);

	///  throws Error <INVALID_URL>, Error <URL_UNAVAILABLE>
	void
	loadURL (const MFString & url, const MFString & parameter);

	///  throws Error <INVALID_URL>, Error <URL_UNAVAILABLE>
	void
	parseIntoScene (const X3DScenePtr & scene, const MFString & url);

	void
	stop ();

	//  Stream Handling

	///  throws Error <INVALID_URL>, Error <URL_UNAVAILABLE>
	std::string
	loadDocument (const basic::uri & uri);

	///  throws Error <INVALID_URL>, Error <URL_UNAVAILABLE>
	basic::ifilestream
	loadStream (const basic::uri & uri);


private:

	///  @name Operations

	///  throws Error <INVALID_URL>, Error <URL_UNAVAILABLE>
	void
	loadStream (const basic::uri & uri, basic::ifilestream &);

	///  @name Members

	X3DExecutionContext* const executionContext;
	const std::string          userAgent;
	const basic::uri           referer;
	basic::uri                 worldURL;
	MFString                   urlError;
	basic::ifilestream         istream;

};

} // X3D
} // titania

#endif
