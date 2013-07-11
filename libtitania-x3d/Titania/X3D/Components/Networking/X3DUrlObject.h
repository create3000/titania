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

#ifndef __TITANIA_X3D_COMPONENTS_NETWORKING_X3DURL_OBJECT_H__
#define __TITANIA_X3D_COMPONENTS_NETWORKING_X3DURL_OBJECT_H__

#include "../../Basic/X3DBaseNode.h"
#include "../../Bits/Error.h"
#include "../../Bits/X3DConstants.h"
#include "../../Execution/Scene.h"
#include "../../Fields.h"

#include <Titania/Stream/InputFileStream.h>
#include <map>
#include <memory>

namespace titania {
namespace X3D {

typedef std::map <basic::uri, basic::uri> URNIndex;

class X3DUrlObject :
	virtual public X3DBaseNode
{
public:

	///  @name Fields

	MFString &
	url ()
	{ return *fields .url; }

	const MFString &
	url () const
	{ return *fields .url; }

	SFTime &
	loadTime ()
	{ return *fields .loadTime; }

	const SFTime &
	loadTime () const
	{ return *fields .loadTime; }

	MFString &
	urlError ()
	{ return *fields .urlError; }

	const MFString &
	urlError () const
	{ return *fields .urlError; }

	virtual
	const basic::uri &
	getWorldURL () const
	{ return worldURL; }

	LoadState
	checkLoadState () const
	{ return loadState; }

	///  @name File operations

	virtual
	void
	requestImmediateLoad ()
	{ }

	///  @name X3D Creation Handling

	///
	X3DSFNode <Scene>
	createX3DFromString (const std::string &)
	throw (Error <INVALID_X3D>,
	       Error <NOT_SUPPORTED>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	///
	X3DSFNode <Scene>
	createX3DFromStream (std::istream &)
	throw (Error <INVALID_X3D>,
	       Error <NOT_SUPPORTED>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	///
	void
	createX3DFromURL (const MFString &, const SFNode &, const std::string &)
	throw (Error <INVALID_URL>,
	       Error <URL_UNAVAILABLE>);

	///
	X3DSFNode <Scene>
	createX3DFromURL (const MFString &)
	throw (Error <INVALID_URL>,
	       Error <URL_UNAVAILABLE>);

	///
	virtual
	void
	loadURL (const MFString &, const MFString &)
	throw (Error <INVALID_URL>,
	       Error <URL_UNAVAILABLE>);

	///  @name Stream Handling

	///  Returns the contents of @a URL
	std::string
	loadDocument (const SFString &)
	throw (Error <INVALID_URL>,
	       Error <URL_UNAVAILABLE>);

	///  Returns the contents of @a uri
	std::string
	loadDocument (const basic::uri &)
	throw (Error <INVALID_URL>,
	       Error <URL_UNAVAILABLE>);

	///  Returns a stream for @a URL
	basic::ifilestream
	loadStream (const basic::uri &)
	throw (Error <INVALID_URL>,
	       Error <URL_UNAVAILABLE>);

	/// URI Handling

	MFString
	transformURI (const MFString &);

	MFString
	transformURI (const basic::uri &, const MFString &);

	basic::uri
	transformURI (const basic::uri &);

	static
	basic::uri
	transformURI (const basic::uri &, const basic::uri &);

	/// URN Handling

	static
	void
	addURN (const basic::uri &, const basic::uri &);

	static
	void
	removeURN (const basic::uri &);

	static
	basic::uri
	getURL (const basic::uri & uri);

	static
	const URNIndex &
	getURNs ();


protected:

	X3DUrlObject ();

	virtual
	void
	initialize () override;

	///  @name Element Access

	virtual
	const basic::uri &
	getReferer () const
	{ return getExecutionContext () -> getWorldURL (); }

	virtual
	void
	setWorldURL (const basic::uri & value)
	{ worldURL = value; }

	void
	setLoadState (LoadState);

	void
	parseIntoScene (X3DScene* const, const MFString &)
	throw (Error <INVALID_URL>,
	       Error <URL_UNAVAILABLE>);

	virtual
	void
	dispose () override;


private:

	static URNIndex URNCache;

	struct Fields
	{
		Fields ();

		MFString* const url;
		SFTime* const loadTime;
		MFString* const urlError;
	};

	Fields fields;

	LoadState   loadState;
	std::string userAgent;
	basic::uri  worldURL;

};

} // X3D
} // titania

#endif
