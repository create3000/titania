/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#include "Script.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../InputOutput/Loader.h"
#include "../../JavaScript/X3DJavaScriptEngine.h"
#include "../../Parser/RegEx.h"

namespace titania {
namespace X3D {

const ComponentType Script::component      = ComponentType::SCRIPTING;
const std::string   Script::typeName       = "Script";
const std::string   Script::containerField = "children";

Script::Fields::Fields () :
	directOutput (new SFBool ()),
	mustEvaluate (new SFBool ())
{ }

Script::Script (X3DExecutionContext* const executionContext) :
	  X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DScriptNode (),
	       fields (),
	   javaScript ()
{
	addType (X3DConstants::Script);

	addField (inputOutput,    "metadata",     metadata ());
	addField (inputOutput,    "url",          url ());
	addField (initializeOnly, "directOutput", directOutput ());
	addField (initializeOnly, "mustEvaluate", mustEvaluate ());

	addChildren (javaScript);

	setExtendedEventHandling (false);
}

X3DBaseNode*
Script::create (X3DExecutionContext* const executionContext) const
{
	return new Script (executionContext);
}

void
Script::initialize ()
{
	X3DScriptNode::initialize ();

	url () .addInterest (this, &Script::set_url);

	requestImmediateLoad ();
}

void
Script::setExecutionContext (X3DExecutionContext* const executionContext)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	if (javaScript)
		javaScript -> setExecutionContext (executionContext);

	X3DScriptNode::setExecutionContext (executionContext);
}

void
Script::addUserDefinedField (const AccessType accessType, const std::string & name, X3DFieldDefinition* const field)
throw (Error <INVALID_NAME>,
       Error <INVALID_FIELD>,
       Error <DISPOSED>)
{
	X3DScriptNode::addUserDefinedField (accessType, name, field);
	
	if (isInitialized ())
		url () .addEvent ();
}

void
Script::removeUserDefinedField (const std::string & name)
throw (Error <DISPOSED>)
{
	X3DScriptNode::removeUserDefinedField (name);

	if (isInitialized ())
		url () .addEvent ();
}

bool
Script::loadDocument (const SFString & URL, std::string & scheme, std::string & ecmascript)
{
	if (RegEx::ECMAScript .FullMatch (URL .str (), &scheme, &ecmascript))
	{
		setWorldURL (getExecutionContext () -> getWorldURL ());
		return true;
	}

	try
	{
		Loader loader (getExecutionContext ());
		ecmascript = std::move (loader .loadDocument (URL));
		setWorldURL (loader .getWorldURL ());
		return true;
	}
	catch (const X3DError &)
	{ }

	return false;
}

void
Script::requestImmediateLoad ()
{
	if (checkLoadState () == COMPLETE_STATE or checkLoadState () == IN_PROGRESS_STATE)
		return;

	setLoadState (IN_PROGRESS_STATE);
	
	javaScript .set (nullptr);

	// Find first working script.

	for (const auto & URL : url ())
	{
		std::string scheme;
		std::string ecmascript;

		if (loadDocument (URL, scheme, ecmascript))
		{
			try
			{
				auto javaScript = getBrowser () -> getJavaScriptEngine (scheme) -> createContext (this, ecmascript, getWorldURL ());
				
				// Initialize.

				isLive () .addInterest (javaScript -> isLive ());

				javaScript -> isLive () = isLive ();
				javaScript -> setup ();

				this -> javaScript = std::move (javaScript);
				setLoadState (COMPLETE_STATE);
				break;
			}
			catch (const std::exception & error)
			{
				getBrowser () -> println ("Warning: ", error .what ());
			}
		}
	}

	if (not javaScript)
	{
		try
		{
			// Assign an empty script if no working script is found.
			javaScript .set (getBrowser () -> getJavaScriptEngine () -> createContext (this, "", ""));
			javaScript -> setup ();
		}
		catch (const std::invalid_argument & error)
		{ }

		setLoadState (FAILED_STATE);
	}
}

void
Script::set_url ()
{
	setLoadState (NOT_STARTED_STATE);

	requestImmediateLoad ();
}

} // X3D
} // titania
