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

Generator::StyleType     Generator::style                = NICEST;
SpecificationVersionType Generator::specificationVersion = LATEST_VERSION;

Generator::ExecutionContextStack Generator::executionContextStack (1);
size_t                           Generator::level = 0;
Generator::LocalNodeSet          Generator::exportedNodesIndex;
Generator::LocalNodeSet          Generator::importedNodesIndex;
Generator::NodeIdSet             Generator::nodes;
Generator::NameIndex             Generator::names;
Generator::NameIndexByNode       Generator::namesByNode;
size_t                           Generator::newName = 0;
Generator::ImportedNamesIndex    Generator::importedNames;
Generator::FieldStack            Generator::containerFieldStack (1);
static const std::string         emptyName;

void
Generator::Style (const std::string & value)
{
	std::string style = value;

	std::transform (style .begin (), style .end (), style .begin (), ::toupper);

	if (style == "SMALLEST")
		SmallestStyle ();

	else if (style == "SMALL")
		SmallStyle ();

	else if (style == "COMPACT")
		CompactStyle ();

	else
		NicestStyle ();
}

void
Generator::SmallestStyle ()
{
	if (style == SMALLEST)
		return;

	style = SMALLEST;

	space     = " ";
	tidySpace = "";
	endl      = " ";
	tidyBreak = "";
	listBreak = " ";
	comma     = "";

	indent     = "";
	indentChar = "";

	listSpace    = false;
	hasListBreak = false;
}

void
Generator::SmallStyle ()
{
	if (style == SMALL)
		return;

	style = SMALL;

	space     = " ";
	tidySpace = "";
	endl      = "\n";
	tidyBreak = "";
	listBreak = " ";
	comma     = "";

	indent     = "";
	indentChar = "";

	listSpace    = false;
	hasListBreak = false;
}

void
Generator::CompactStyle ()
{
	if (style == COMPACT)
		return;

	style = COMPACT;

	space     = " ";
	tidySpace = " ";
	endl      = "\n";
	tidyBreak = "\n";
	listBreak = " ";
	comma     = ",";

	indent     = "";
	indentChar = "  ";

	listSpace    = true;
	hasListBreak = false;
}

void
Generator::NicestStyle ()
{
	if (style == NICEST)
		return;

	style = NICEST;

	space     = " ";
	tidySpace = " ";
	endl      = "\n";
	tidyBreak = "\n";
	listBreak = "\n";
	comma     = ",";

	indent     = "";
	indentChar = "  ";

	listSpace    = true;
	hasListBreak = true;
}

void
Generator::PushExecutionContext (const X3DExecutionContext* const value)
{
	executionContextStack .emplace_back (value);

	exportedNodesIndex .emplace (value, NodeIdSet ());
	importedNodesIndex .emplace (value, NodeIdSet ());
}

void
Generator::PopExecutionContext ()
{
	executionContextStack .pop_back ();

	if (executionContextStack .back ())
		return;

	exportedNodesIndex .clear ();
	importedNodesIndex .clear ();
}

void
Generator::EnterScope ()
{
	if (level == 0)
		newName = 0;

	++ level;
}

void
Generator::LeaveScope ()
{
	-- level;

	if (level == 0)
	{
		nodes         .clear ();
		names         .clear ();
		namesByNode   .clear ();
		importedNames .clear ();
	}
}

void
Generator::ExportedNodes (const ExportedNodeIndex & exportedNodes)
{
	auto & index = exportedNodesIndex .at (executionContextStack .back ());

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
Generator::ImportedNodes (const ImportedNodeIndex & importedNodes)
{
	auto & index = importedNodesIndex .at (executionContextStack .back ());

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
Generator::IsSharedNode (const X3DBaseNode* const baseNode)
{
	if (executionContextStack .back ())
		return executionContextStack .back () not_eq baseNode -> getExecutionContext ();

	return false;
}

bool
Generator::ExistsNode (const X3DBaseNode* const baseNode)
{
	return nodes .count (baseNode -> getId ());
}

void
Generator::AddNode (const X3DBaseNode* const baseNode)
{
	nodes .emplace (baseNode -> getId ());
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
Generator::AddImportedNode (const X3DBaseNode* const exportedNode, const std::string & importedName)
{
	importedNames [exportedNode -> getId ()] = importedName;
}

const std::string &
Generator::LocalName (const X3DBaseNode* baseNode)
{
	try
	{
		return importedNames .at (baseNode -> getId ());
	}
	catch (...)
	{
		if (ExistsNode (baseNode))
			return Name (baseNode);
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
			case '#':
			{
				ostream << "&#35;";
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

} // X3D
} // titania
