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

#include "Generator.h"

#include "../Browser/X3DBrowser.h"
#include "../Components/Networking/Inline.h"
#include "../Execution/ExportedNode.h"
#include "../Execution/ImportedNode.h"

#include <Titania/String/to_string.h>
#include <regex>

namespace titania {
namespace X3D {

const int Generator::index = std::ostream::xalloc ();

Generator::Generator (std::ostream & ostream) :
	         X3DGenerator (ostream),
	               stream (&ostream),
	 specificationVersion (LATEST_VERSION),
	executionContextStack (1),
	                level (0),
	   exportedNodesIndex (),
	   importedNodesIndex (),
	                nodes (),
	                names (),
	          namesByNode (),
	              newName (0),
	        importedNames (),
	  containerFieldStack (1),
	            emptyName ()
{
	set (ostream, this);
}

void
Generator::set (std::ostream & ostream, Generator* const generator)
{
	const auto previous = static_cast <Generator*> (ostream .pword (index));

	ostream .pword (index) = generator;
	ostream .register_callback (dispose, index);

	if (previous)
		delete previous;
}

Generator*
Generator::get (std::ostream & ostream)
{
	const auto generator = static_cast <Generator*> (ostream .pword (index));

	if (generator)
		return generator;

	return new Generator (ostream);
}

void
Generator::PushExecutionContext (std::ostream & ostream, const X3DExecutionContext* const executionContext)
{
	get (ostream) -> executionContextStack .emplace_back (executionContext);

	get (ostream) -> exportedNodesIndex .emplace (executionContext, NodeIdSet ());
	get (ostream) -> importedNodesIndex .emplace (executionContext, NodeIdSet ());
}

void
Generator::PopExecutionContext (std::ostream & ostream)
{
	get (ostream) -> executionContextStack .pop_back ();

	if (get (ostream) -> executionContextStack .back ())
		return;

	get (ostream) -> exportedNodesIndex .clear ();
	get (ostream) -> importedNodesIndex .clear ();
}

void
Generator::EnterScope (std::ostream & ostream)
{
	if (get (ostream) -> level == 0)
		get (ostream) -> newName = 0;

	++ get (ostream) -> level;
}

void
Generator::LeaveScope (std::ostream & ostream)
{
	-- get (ostream) -> level;

	if (get (ostream) -> level == 0)
	{
		get (ostream) -> nodes         .clear ();
		get (ostream) -> names         .clear ();
		get (ostream) -> namesByNode   .clear ();
		get (ostream) -> importedNames .clear ();
	}
}

void
Generator::ExportedNodes (std::ostream & ostream, const ExportedNodeIndex & exportedNodes)
{
	auto & index = get (ostream) -> exportedNodesIndex .at (get (ostream) -> executionContextStack .back ());

	for (const auto & exportedNode : exportedNodes)
	{
		try
		{
			index .emplace (exportedNode .second -> getLocalNode () -> getId ());
		}
		catch (...)
		{ }
	}
}

void
Generator::ImportedNodes (std::ostream & ostream, const ImportedNodeIndex & importedNodes)
{
	auto & index = get (ostream) -> importedNodesIndex .at (get (ostream) -> executionContextStack .back ());

	for (const auto & importedNode : importedNodes)
	{
		try
		{
			index .emplace (importedNode .second -> getInlineNode () -> getId ());
		}
		catch (...)
		{ }
	}
}

bool
Generator::IsSharedNode (std::ostream & ostream, const X3DBaseNode* const baseNode)
{
	if (get (ostream) -> executionContextStack .back ())
		return get (ostream) -> executionContextStack .back () not_eq baseNode -> getExecutionContext ();

	return false;
}

bool
Generator::ExistsNode (std::ostream & ostream, const X3DBaseNode* const baseNode)
{
	return get (ostream) -> nodes .count (baseNode -> getId ());
}

void
Generator::AddNode (std::ostream & ostream, const X3DBaseNode* const baseNode)
{
	get (ostream) -> nodes .emplace (baseNode -> getId ());
}

const std::string &
Generator::Name (std::ostream & ostream, const X3DBaseNode* const baseNode)
{
	return get (ostream) -> Name (baseNode);
}

const std::string &
Generator::Name (const X3DBaseNode* const baseNode)
{
	// Is the node already in index

	const auto iter = namesByNode .find (baseNode -> getId ());

	if (iter not_eq namesByNode .end ())
		return iter -> second;

	// The node has no name

	if (baseNode -> getName () .empty ())
	{
		if (needsName (baseNode))
		{
			std::string name = getUniqueName ();

			names [name]                       = baseNode;
			namesByNode [baseNode -> getId ()] = name;

			return namesByNode [baseNode -> getId ()];
		}

		// The node doesn't need a name

		return baseNode -> getName ();
	}

	// The node has a name
 	
	static const std::regex _TrailingNumbers (R"/(_\d+$)/");

	std::string name      = baseNode -> getName ();
	const bool  hasNumber = std::regex_match (name, _TrailingNumbers);

	name = std::regex_replace (name, _TrailingNumbers, "");

	if (name .empty ())
	{
		if (needsName (baseNode))
			name = getUniqueName ();

		else
			return emptyName;
	}
	else
	{
		size_t      i       = 0;
		std::string newName = hasNumber ? name + '_' + basic::to_string (++ i, std::locale::classic ()) : name;

		try
		{
			for (; ;)
			{
				names .at (newName);

				newName = name + '_' + basic::to_string (++ i, std::locale::classic ());
			}
		}
		catch (const std::out_of_range &)
		{
			name = newName;
		}
	}

	names [name]                       = baseNode;
	namesByNode [baseNode -> getId ()] = name;

	return namesByNode [baseNode -> getId ()];
}

bool
Generator::needsName (const X3DBaseNode* const baseNode)
{
	if (baseNode -> getCloneCount () > 1)
		return true;

	if (baseNode -> hasRoutes ())
		return true;

	try
	{
		const auto & index = exportedNodesIndex .at (baseNode -> getExecutionContext ());

		if (index .count (baseNode -> getId ()))
			return true;
	}
	catch (...)
	{ }

	try
	{
		const auto & index = importedNodesIndex .at (baseNode -> getExecutionContext ());

		if (index .count (baseNode -> getId ()))
			return true;
	}
	catch (...)
	{ }

	return false;
}

std::string
Generator::getUniqueName ()
{
	std::string name;

	for (; ;)
	{
		name = '_' + basic::to_string (++ newName, std::locale::classic ());

		if (names .count (name))
			continue;

		return name;
	}
}

void
Generator::AddImportedNode (std::ostream & ostream, const X3DBaseNode* const exportedNode, const std::string & importedName)
{
	get (ostream) -> importedNames [exportedNode -> getId ()] = importedName;
}

const std::string &
Generator::LocalName (std::ostream & ostream, const X3DBaseNode* baseNode)
{
	try
	{
		return get (ostream) -> importedNames .at (baseNode -> getId ());
	}
	catch (...)
	{
		if (ExistsNode (ostream, baseNode))
			return Name (ostream, baseNode);
	}

	throw Error <INVALID_NODE> ("Couldn't get local name for node '" + baseNode -> getTypeName () + "'.");
}

void
Generator::XMLEncode (std::ostream & ostream, const std::string & string)
{
	for (const auto & c : string)
	{
		switch (c)
		{
			case '\t' :
			{
				ostream << "&#x9;";
				break;
			}
			case '\n':
			{
				ostream << "&#xA;";
				break;
			}
			case '\r':
			{
				ostream << "&#xD;";
				break;
			}
			case '<':
			{
				ostream << "&lt;";
				break;
			}
			case '>':
			{
				ostream << "&gt;";
				break;
			}
			case '&':
			{
				ostream << "&amp;";
				break;
			}
			case '\'':
			{
				ostream << "&apos;";
				break;
			}
			case '"':
			{
				ostream << "\\\"";
				break;
			}
			case '\\':
			{
				ostream << "\\\\";
				break;
			}
			default:
			{
				ostream << c;
				break;
			}
		}
	}
}

void
Generator::dispose (std::ios_base::event event, std::ios_base & stream, int index)
{
	if (event not_eq std::ios_base::erase_event)
		return;

	const auto generator = static_cast <Generator*> (stream .pword (index));

	if (not generator)
		return;

	if (generator -> stream not_eq &stream)
		return;

	delete generator;
	stream .pword (index) = nullptr;
}

Generator::~Generator ()
{ }

} // X3D
} // titania
