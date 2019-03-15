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
#include "../Parser/Filter.h"

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
	   exportedNodesIndex (1),
	   importedNodesIndex (1),
	                nodes (),
	              newName (0),
	           namesIndex (),
	          namesByNode (),
	        importedNames (),
	           routeNodes (),
	  containerFieldStack (1),
	                units (true),
	       unitCategories (),
	             metadata (true),
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
Generator::PushScene (std::ostream & ostream, const X3DScene* const scene)
{
	const auto generator = get (ostream);

	generator -> executionContextStack .emplace_back (const_cast <X3DScene*> (scene));
	generator -> exportedNodesIndex    .emplace_back ();
}

void
Generator::PopScene (std::ostream & ostream)
{
	const auto generator = get (ostream);

	generator -> executionContextStack .pop_back ();
	generator -> exportedNodesIndex    .pop_back ();
}

void
Generator::PushExecutionContext (std::ostream & ostream, const X3DExecutionContext* const executionContext)
{
	const auto generator = get (ostream);

	generator -> executionContextStack .emplace_back (const_cast <X3DExecutionContext*> (executionContext));
	generator -> importedNodesIndex    .emplace_back ();
}

void
Generator::PopExecutionContext (std::ostream & ostream)
{
	const auto generator = get (ostream);

	generator -> executionContextStack .pop_back ();
	generator -> importedNodesIndex    .pop_back ();
}

void
Generator::EnterScope (std::ostream & ostream)
{
	const auto generator = get (ostream);

	++ generator -> level;
}

void
Generator::LeaveScope (std::ostream & ostream)
{
	const auto generator = get (ostream);

	-- generator -> level;

	if (generator -> level == 0)
	{
		generator -> nodes         .clear ();
		generator -> namesByNode   .clear ();
		generator -> importedNames .clear ();
	}
}

void
Generator::ExportedNodes (std::ostream & ostream, const ExportedNodeIndex & exportedNodes)
{
	const auto generator = get (ostream);
	auto &     index     = generator -> exportedNodesIndex .back ();

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
	const auto generator = get (ostream);
	auto &     index     = generator -> importedNodesIndex .back ();

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
	const auto generator        = get (ostream);
	const auto executionContext = generator -> executionContextStack .back ();

	if (executionContext)
		return executionContext not_eq baseNode -> getExecutionContext ();

	return false;
}

bool
Generator::ExistsNode (std::ostream & ostream, const X3DBaseNode* const baseNode)
{
	const auto generator = get (ostream);

	return generator -> nodes .count (baseNode -> getId ());
}

void
Generator::AddNode (std::ostream & ostream, const X3DBaseNode* const baseNode)
{
	const auto generator = get (ostream);

	generator -> nodes .emplace (baseNode -> getId ());

	generator -> AddRouteNode (ostream, baseNode);
}

const std::string &
Generator::Name (std::ostream & ostream, const X3DBaseNode* const baseNode)
{
	const auto generator = get (ostream);

	return generator -> Name (baseNode);
}

const std::string &
Generator::Name (const X3DBaseNode* const baseNode)
{
	// Is the node already in index

	const auto iter = namesByNode .find (baseNode -> getId ());

	if (iter not_eq namesByNode .end ())
		return iter -> second;

	auto & names = namesIndex [executionContextStack .back ()];

	// The node has no name

	if (baseNode -> getName () .empty ())
	{
		if (needsName (baseNode))
		{
			const auto name = getUniqueName ("");

			names [name]                       = baseNode;
			namesByNode [baseNode -> getId ()] = name;

			return namesByNode [baseNode -> getId ()];
		}

		// The node doesn't need a name

		return baseNode -> getName ();
	}

	// The node has a name

	auto       name       = RemoveTrailingNumber (baseNode -> getName ()); // Don't remove tailing number for stable underscore names. Option?
	const auto uniqueName = name .empty () ? needsName (baseNode) : name not_eq baseNode -> getName ();

	if (uniqueName or (name .size () and names .count (name)))
		name = getUniqueName (name);

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
		const auto & index = exportedNodesIndex .back ();

		if (index .count (baseNode -> getId ()))
			return true;
	}
	catch (...)
	{ }

	try
	{
		const auto & index = importedNodesIndex .back ();

		if (index .count (baseNode -> getId ()))
			return true;
	}
	catch (...)
	{ }

	return false;
}

std::string
Generator::getUniqueName (const std::string & name)
{
	auto & names = namesIndex [executionContextStack .back ()];

	std::string uniqueName;

	do
	{
		uniqueName = name + '_' + basic::to_string (++ newName, std::locale::classic ());
	}
	while (names .count (uniqueName));

	return uniqueName;
}

void
Generator::AddImportedNode (std::ostream & ostream, const X3DBaseNode* const exportedNode, const std::string & importedName)
{
	const auto generator = get (ostream);

	generator -> importedNames [exportedNode -> getId ()] = importedName;
}

void
Generator::AddRouteNode (std::ostream & ostream, const X3DBaseNode* const routeNode)
{
	const auto generator = get (ostream);

	generator -> routeNodes .emplace (routeNode -> getId ());
}

bool
Generator::ExistsRouteNode (std::ostream & ostream, const X3DBaseNode* const routeNode)
{
	const auto generator = get (ostream);

	return generator -> routeNodes .count (routeNode -> getId ());
}

const std::string &
Generator::LocalName (std::ostream & ostream, const X3DBaseNode* baseNode)
{
	try
	{
		const auto generator = get (ostream);

		return generator -> importedNames .at (baseNode -> getId ());
	}
	catch (...)
	{
		if (ExistsNode (ostream, baseNode))
			return Name (ostream, baseNode);
	}

	throw Error <INVALID_NODE> ("Couldn't get local name for node '" + baseNode -> getTypeName () + "'.");
}

UnitCategory
Generator::Unit (std::ostream & ostream, const UnitCategory category)
{
	const auto generator = get (ostream);

	if (generator -> unitCategories .empty ())
		return category;

	return generator -> unitCategories .back ();
}

long double
Generator::ToUnit (std::ostream & ostream, const UnitCategory unit, const long double value)
{
	const auto generator = get (ostream);

	if (generator -> units)
	{
		const auto executionContext = ExecutionContext (ostream);
	
		if (executionContext)
			return executionContext -> toUnit (unit, value);
	}

	return value;
}

// Destruction

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
