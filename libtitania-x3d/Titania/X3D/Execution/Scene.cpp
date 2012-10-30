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
 ******************************************************************************/

#include "Scene.h"

#include "../Browser/Browser.h"
#include "../Parser/Parser.h"
#include "../Parser/RegEx.h"
#include <iostream>

namespace titania {
namespace X3D {

Scene::Scene (X3DBrowser* const browser) :
	X3DBasicNode (browser, browser), 
	    X3DScene ()                  
{
	std::clog << "Constructing Scene:" << std::endl;

	setComponent ("Browser");
	setTypeName ("Scene");

	std::clog << "\tDone constructing Scene." << std::endl;
}

Scene*
Scene::create (const basic::uri & worldURL, std::istream & istream)
throw (Error <INVALID_X3D>,
       Error <NOT_SUPPORTED>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	Scene* scene = create (this);

	scene -> setup ();
	scene -> setWorldURL (worldURL);
	scene -> fromStream (istream);

	return scene;
}

Scene*
Scene::create (X3DExecutionContext* const executionContext) const
{
	return new Scene (executionContext -> getBrowser ());
}

void
Scene::setMetaData (const std::string & key, const std::string & value)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	metadata .insert (std::make_pair (key, value));
}

const std::string &
Scene::getMetaData (const std::string & key) const
throw (Error <INVALID_NAME>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	MetaDataMap::const_iterator iter = metadata .find (key);

	if (iter not_eq metadata .end ())
		return iter -> second;

	throw Error <INVALID_NAME> ("Unkown meta key '" + key + "'.");
}

void
Scene::fromStream (std::istream & istream)
throw (Error <INVALID_X3D>,
       Error <NOT_SUPPORTED>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	std::ostringstream ostringstream;

	ostringstream << istream .rdbuf ();

	Parser::parseIntoScene (this, ostringstream .str ());
}

void
Scene::toStream (std::ostream & ostream) const
{
	Generator::PushLevel ();

	bool X3D = (getEncoding () == "X3D");
	Generator::X3DAccessTypes (X3D);
	Generator::X3DFieldNames (X3D);

	ostream
		<< '#'
		<< getEncoding ()
		<< Generator::Space
		<< 'V'
		<< getSpecificationVersion ()
		<< Generator::Space
		<< getCharacterEncoding ();

	if (getComment () .length ())
	{
		ostream
			<< Generator::Space
			<< getComment ();
	}

	ostream << std::endl << std::endl;

	if (getProfile ())
	{
		ostream
			<< getProfile ()
			<< Generator::Break
			<< Generator::TidyBreak;
	}

	for (const auto & component : getComponents ())
	{
		ostream
			<< component
			<< Generator::Break;
	}

	if (getComponents () .size ())
		ostream << Generator::TidyBreak;

	for (const auto & meta : metadata)
	{
		std::string key   = meta .first;
		std::string value = meta .second;

		RegEx::QuotationMark .GlobalReplace ("\\\\\"", &key);
		RegEx::QuotationMark .GlobalReplace ("\\\\\"", &value);

		ostream
			<< "META"
			<< Generator::Space
			<< '"'
			<< key
			<< '"'
			<< Generator::Space
			<< '"'
			<< value
			<< '"'
			<< Generator::Break;
	}

	if (metadata .size ())
		ostream << Generator::TidyBreak;

	X3DExecutionContext::toStream (ostream);

	ostream << std::flush;
}

} // X3D
} // titania
