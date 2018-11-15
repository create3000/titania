/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
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

#include "X3DParser.h"

#include "../Base/X3DGarbageCollector.h"
#include "../Basic/X3DBaseNode.h"
#include "../Execution/X3DExecutionContext.h"

#include <regex>

#include <glibmm.h>
#include <unistd.h>

namespace titania {
namespace X3D {

X3DParser::X3DParser () :
	executionContextStack (),
	                units (true)
{ }

///  throws Error <INVALID_X3D>
std::string
X3DParser::save (std::istream & istream, const std::string & suffix)
{
	// Create temp file

	std::string filename = "/tmp/titania-XXXXXX" + suffix;

	::close (Glib::mkstemp (filename));

	std::ofstream ofstream (filename);

	if (not ofstream)
		throw Error <INVALID_X3D> ("Couldn't create temp file.");

	ofstream << istream .rdbuf ();

	return filename;
}

void
X3DParser::addUninitializedNode (X3DBaseNode* const baseNode)
{
	getExecutionContext () -> addUninitializedNode (baseNode);
}

void
X3DParser::pushExecutionContext (X3DExecutionContext* const executionContext)
{
	executionContextStack .emplace_back (executionContext);
}

void
X3DParser::popExecutionContext ()
{
	executionContextStack .pop_back ();
}

X3DExecutionContext*
X3DParser::getExecutionContext () const
{
	return executionContextStack .back ();
}

bool
X3DParser::isInsideProtoDefinition () const
{
	//__LOG__ << this << " " << std::endl;

	return executionContextStack .size () > 1;
}

void
X3DParser::addRootNode (X3D::SFNode && rootNode)
{
	//__LOG__ << this << " " << std::endl;

	getExecutionContext () -> getRootNodes () .emplace_back (std::move (rootNode));
}

void
X3DParser::setUnits (const std::string & generator)
{
	const std::regex version (R"/(Titania\s+V(\d+).*)/");

	std::smatch match;

	if (std::regex_match (generator, match, version))
	{
		int32_t major = 0;

		std::istringstream isstream (match .str (1));

		isstream >> major;

		// Before version 4 units are wrongly implemented.
		if (major < 4)
		{
			units = false;
			return;
		}
	}

	units = true;
}

long double
X3DParser::fromUnit (const UnitCategory unit, long double value) const
{
	if (units)
		return getExecutionContext () -> fromUnit (unit, value);

	return value;
}

X3DParser::~X3DParser ()
{
	X3DGarbageCollector::trimFreeMemory ();
}

} // X3D
} // titania
