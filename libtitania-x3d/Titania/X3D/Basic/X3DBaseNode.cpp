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

#include "X3DBaseNode.h"

#include "../Bits/X3DConstants.h"
#include "../Browser/X3DBrowser.h"
#include "../Fields/MFNode.h"
#include "../Fields/SFNode.h"
#include <Titania/Utility/Adapter.h>
#include <cassert>
#include <iomanip>
#include <iostream>

#include <Titania/Backtrace.h>

namespace titania {
namespace X3D {

/*
 * Creation phase
 *    The node is first instantiated by the browser internals with all field values set to defaults
 *
 *    Permitted field access capabilities
 *       initializeOnly	None
 *       inputOnly		None
 *       outputOnly		None
 *       inputOutput		None
 *
 *
 * Setup phase
 *    Field values are changed from the default value where required
 *
 *    Permitted field access capabilities
 *       initializeOnly	readable/writable
 *       inputOnly		None
 *       outputOnly		None
 *       inputOutput		readable/writable
 *
 *    setBrowser (SAIBrowserRef)
 *    setFields  (SAIField(s))
 *    initialize ()
 *
 *
 * Realized phase
 *    The node is participating in the scene graph and/or scripting
 *
 *    Permitted field access capabilities
 *       initializeOnly	None
 *       inputOnly		writable
 *       outputOnly		readable
 *       inputOutput		readable/writable
 *
 *    prepareEvents ()
 *    eventsProcessed ()
 *
 *
 * Disposed phase
 *    The node is no longer part of a scene graph and no remaining references to it exist at the scripting level.
 *
 *    Permitted field access capabilities
 *       initializeOnly	None
 *       inputOnly		None
 *       outputOnly		None
 *       inputOutput		None
 *
 *    shutdown ()
 */

X3DBaseNode::X3DBaseNode (X3DBrowser* const browser, X3DExecutionContext* const executionContext) :
	       X3DChildObject (),
	              browser (browser),
	     executionContext (executionContext),
	             nodeType (),
	     fieldDefinitions (),
	               fields (),
	         fieldAliases (),
	 numUserDefinedFields (0),
	             internal (false),
	                saved (false),
	extendedEventHandling (true),
	               nodeId ({ 0 }),
	               handle (NULL),
	             comments (),
	       shutdownOutput ()
{
	assert (browser);
	assert (executionContext);
}

X3DBaseNode*
X3DBaseNode::clone (X3DExecutionContext* const executionContext) const
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>)
{
	if (getName () .empty ())
		return copy (executionContext);

	else
	{
		try
		{
			return executionContext -> getNamedNode (getName ());
		}
		catch (const Error <INVALID_NAME> &)
		{
			return copy (executionContext);
		}
	}
}

X3DBaseNode*
X3DBaseNode::copy (X3DExecutionContext* const executionContext) const
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>)
{
	X3DBaseNode* const copy = create (executionContext);

	if (not getName () .empty ())
		executionContext -> updateNamedNode (getName (), copy);

	for (const auto & fieldDefinition : fieldDefinitions)
	{
		try
		{
			// Default fields

			X3DFieldDefinition* const field = copy -> getField (fieldDefinition -> getName ());

			if (field -> getAccessType () == fieldDefinition -> getAccessType () and field -> getType () == fieldDefinition -> getType ())
			{
				if (fieldDefinition -> getReferences () .empty ())
				{
					if (fieldDefinition -> isInitializeable ())
						fieldDefinition -> clone (executionContext, field);
				}
				else
				{
					// IS relationship

					for (const auto & originalReference : fieldDefinition -> getReferences ())
					{
						try
						{
							field -> addReference (executionContext -> getField (originalReference -> getName ()));
						}
						catch (const Error <INVALID_NAME> &)
						{
							throw Error <INVALID_NAME> ("No such event or field '" + originalReference -> getName () + ".");
						}
					}
				}
			}
			else
				throw Error <INVALID_NAME> ("");
		}
		catch (const Error <INVALID_NAME> &)
		{
			// User defined fields from Script and Shader

			if (fieldDefinition -> getReferences () .empty ())
			{
				copy -> addUserDefinedField (fieldDefinition -> getAccessType (),
				                             fieldDefinition -> getName (),
				                             fieldDefinition -> clone (executionContext));
			}
			else
			{
				// IS relationship

				X3DFieldDefinition* const field = fieldDefinition -> clone ();

				copy -> addUserDefinedField (fieldDefinition -> getAccessType (),
				                             fieldDefinition -> getName (),
				                             field);

				for (const auto & originalReference : fieldDefinition -> getReferences ())
				{
					try
					{
						field -> addReference (executionContext -> getField (originalReference -> getName ()));
					}
					catch (const Error <INVALID_NAME> &)
					{
						throw Error <INVALID_NAME> ("No reference '" + originalReference -> getName () + "' inside execution context " + executionContext -> getTypeName () + " '" + executionContext -> getName ()  + "'.");
					}
				}
			}
		}
	}

	executionContext -> addUninitializedNode (copy);

	return copy;
}

X3DBaseNode*
X3DBaseNode::copy (X3DExecutionContext* const executionContext, const FlattCopyType &) const
throw (Error <NOT_SUPPORTED>)
{
	X3DBaseNode* const copy = create (executionContext);

	if (not getName () .empty ())
		executionContext -> updateNamedNode (executionContext -> getUniqueName (getName ()), copy);

	for (const auto & field : getUserDefinedFields ())
		copy -> addUserDefinedField (field -> getAccessType (), field -> getName (), field -> create ());

	try
	{
		copy -> assign (this);
	}
	catch (const X3DError &)
	{ }

	executionContext -> addUninitializedNode (copy);

	return copy;
}

X3DBaseNode*
X3DBaseNode::copy (X3DExecutionContext* const executionContext, const DeepCopyType &) const
throw (Error <NOT_SUPPORTED>)
{
	throw Error <NOT_SUPPORTED> ("Deep copy is currently not supported.");
}

void
X3DBaseNode::assign (const X3DBaseNode* const node)
throw (Error <INVALID_NODE>,
       Error <INVALID_FIELD>)
{
	for (const auto & lhs : getFieldDefinitions ())
	{
		if (lhs -> getAccessType () & initializeOnly)
		{
			X3DFieldDefinition* const rhs = node -> getField (lhs -> getName ());

			if (*lhs not_eq *rhs)
				*lhs = *rhs;
		}
	}
}

void
X3DBaseNode::replace (X3DBaseNode* const node, const std::set <const X3DFieldDefinition*> & exclude)
{
	std::vector <X3DFieldDefinition*> parents;

	for (auto & parent : node -> getParents ())
	{
		const auto sfnode = dynamic_cast <X3DFieldDefinition*> (parent);

		if (sfnode and sfnode -> getType () == X3DConstants::SFNode)
		{
			if (exclude .find (sfnode) == exclude .end ())
			{
				bool insert = true;

				for (auto & secondParent : sfnode -> getParents ())
				{
					const auto mfnode = dynamic_cast <X3DFieldDefinition*> (secondParent);

					if (mfnode and mfnode -> getType () == X3DConstants::MFNode)
						insert = exclude .find (mfnode) == exclude .end ();
				}

				if (insert)
					parents .emplace_back (sfnode);
			}
		}
	}

	for (auto & parent : parents)
	{
		parent -> write (SFNode (this));
		parent -> addEvent ();
	}
}

void
X3DBaseNode::setup ()
{
	executionContext -> addParent (this);

	if (executionContext -> isProto ())
		return;

	for (const auto & field : fieldDefinitions)
	{
		field -> updateReferences ();
		field -> isTainted (false);
	}

	initialize ();
}

time_type
X3DBaseNode::getCurrentTime () const
{
	return getBrowser () -> getCurrentTime ();
}

const X3DBaseNode*
X3DBaseNode::getType () const
throw (Error <DISPOSED>)
{
	return getBrowser () -> getNode (getTypeName ());
}

void
X3DBaseNode::addField (const AccessType accessType, const std::string & name, X3DFieldDefinition & field)
{
	if (fields .find (name) not_eq fields .end ())
		removeField (name);

	field .isTainted (true);
	field .addParent (this);
	field .setAccessType (accessType);
	field .setName (name);
	field .setAliasName (name);

	fieldDefinitions .emplace_back (&field);
	fields .emplace (name, &field);
}

void
X3DBaseNode::removeField (const std::string & name)
{
	const auto field = fields .find (name);

	if (field not_eq fields .end ())
	{
		const auto iter = std::find (fieldDefinitions .begin (), fieldDefinitions .end (), field -> second);

		if (fieldDefinitions .end () - iter <= FieldDefinitionArray::difference_type (numUserDefinedFields))
		{
			-- numUserDefinedFields;
		}

		fieldDefinitions .erase (iter);

		field -> second -> removeParent (this);
		fields .erase (field);
	}
}

void
X3DBaseNode::addField (const std::string & alias, const std::string & name)
{
	const auto iter = fields .find (name);

	if (iter not_eq fields .end ())
	{
		fieldAliases .emplace (alias, name);
		iter -> second -> setAliasName (alias);
	}
}

X3DFieldDefinition*
X3DBaseNode::getField (const std::string & name) const
throw (Error <INVALID_NAME>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	const auto field = fields .find (getFieldName (name));

	if (field not_eq fields .end ())
		return field -> second;

	if (name .substr (0, 4) == "set_")
	{
		const auto field = fields .find (getFieldName (name .substr (4)));

		if (field not_eq fields .end () and field -> second -> getAccessType () == inputOutput)
			return field -> second;
	}

	if (name .size () > 8 and name .substr (name .size () - 8) == "_changed")
	{
		const auto field = fields .find (getFieldName (name .substr (0, name .size () - 8)));

		if (field not_eq fields .end () and field -> second -> getAccessType () == inputOutput)
			return field -> second;
	}

	throw Error <INVALID_NAME> ("No such field '" + name + "' in node " + getTypeName () + ".");
}

const std::string &
X3DBaseNode::getFieldName (const std::string & name) const
{
	const auto fieldAlias = fieldAliases .find (name);

	if (fieldAlias not_eq fieldAliases .end ())
		return fieldAlias -> second;

	return name;
}

//const std:string &
//X3DBaseNode::getFieldAlias (X3DFieldDefinition* field) const
//{
//	auto fieldAlias = fieldAliases .find (field -> getName ());
//
//	if (fieldAlias not_eq fieldAliases .end ())
//		return fieldAlias -> first;
//
//	return field -> getName ();
//}

void
X3DBaseNode::addUserDefinedField (const AccessType accessType, const std::string & name, X3DFieldDefinition* const field)
{
	addField (accessType, name, *field);

	++ numUserDefinedFields;
}

FieldDefinitionArray
X3DBaseNode::getPreDefinedFields () const
{
	return FieldDefinitionArray (fieldDefinitions .begin (), fieldDefinitions .end () - numUserDefinedFields);
}

FieldDefinitionArray
X3DBaseNode::getUserDefinedFields () const
{
	return FieldDefinitionArray (fieldDefinitions .end () - numUserDefinedFields, fieldDefinitions .end ());
}

FieldDefinitionArray
X3DBaseNode::getInitializeableFields (const bool all) const
{
	FieldDefinitionArray changedFields;

	for (const auto & field : basic::adapter (fieldDefinitions .begin (), fieldDefinitions .end () - numUserDefinedFields))
	{
		if (field -> getReferences () .empty ())
		{
			if (not field -> isInitializeable ())
				continue;

			if (not all and isDefaultValue (field))
				continue;
		}

		changedFields .emplace_back (field);
	}

	return changedFields;
}

bool
X3DBaseNode::isDefaultValue (const X3DFieldDefinition* const field) const
{
	try
	{
		const X3DFieldDefinition* declarationField = getType () -> getField (field -> getName ());

		return *field == *declarationField;
	}
	catch (const Error <INVALID_NAME> &)
	{
		return false;
	}
}

size_t
X3DBaseNode::getNumClones () const
{
	size_t numClones = 0;

	for (const auto & parentField : getParents ())
	{
		if (dynamic_cast <X3DFieldDefinition*> (parentField))
		{
			if (dynamic_cast <SFNode*> (parentField))
			{
				// Only X3DNodes, ie nodes in the scene graph, have field names

				if (parentField -> getName () .empty ())
				{
					for (const auto & parent : parentField -> getParents ())
					{
						// Only X3DNodes, ie nodes in the scene graph, have field names

						if (dynamic_cast <MFNode*> (parent) and not parent -> getName () .empty ())
						{
							// If any of the fields parents is in a scene add count.

							for (const auto & fparent : parent -> getParents ())
							{
								auto node = dynamic_cast <X3DBaseNode*> (fparent);

								if (node and node -> getExecutionContext () -> isScene ())
								{
									++ numClones;
									break;
								}
							}
						}
					}
				}
				else
				{
					// If any of the fields parents is in a scene add count.

					for (const auto & fparent : parentField -> getParents ())
					{
						auto node = dynamic_cast <X3DBaseNode*> (fparent);

						if (node and node -> getExecutionContext () -> isScene ())
						{
							++ numClones;
							break;
						}
					}
				}
			}
		}
	}

	return numClones;
}

bool
X3DBaseNode::hasRoutes () const
{
	for (const auto & field : fieldDefinitions)
	{
		if (field -> getInputRoutes () .empty () and field -> getOutputRoutes () .empty ())
			continue;

		return true;
	}

	return false;
}

void
X3DBaseNode::isInternal (bool value)
{
	internal = value;

	if (value)
	{
		for (const auto & field : fieldDefinitions)
			field -> setName ("");
	}
	else
	{
		for (const auto & field : fields)
			field .second -> setName (field .first);
	}
}

void
X3DBaseNode::addEvent (X3DChildObject* const object)
{
	if (object -> isTainted ())
		return;

	object -> isTainted (true);

	addEvent (object, EventPtr (new Event (object)));
}

void
X3DBaseNode::addEvent (X3DChildObject* const object, const EventPtr & event)
{
	//__LOG__ << object << " : " << object -> getName () << " : " << object -> getTypeName () << " : " << getName () << " : " << getTypeName () << " : " << this << std::endl;

	getBrowser () -> addEvent ();

	// Register for processEvents

	events .emplace_back (getBrowser () -> getRouter () .addEvent (object, event));

	// Register for processEvents

	isTainted (isTainted () or object -> isInput () or (extendedEventHandling and not object -> isOutput ()));

	if (not nodeId .time)
		nodeId = getBrowser () -> getRouter () .addNode (this);
}

void
X3DBaseNode::addEvent ()
{
	isTainted (true);

	if (not nodeId .time)
	{
		getBrowser () -> addEvent ();
		nodeId = getBrowser () -> getRouter () .addNode (this);
	}
}

void
X3DBaseNode::processEvents ()
{
	events .clear ();
	nodeId .time = 0;

	if (isTainted ())
	{
		isTainted (false);
		processInterests ();
	}
}

void
X3DBaseNode::removeEvents ()
{
	for (const auto & event : events)
		getBrowser () -> getRouter () .removeEvent (event);

	events .clear ();

	if (nodeId .time)
	{
		getBrowser () -> getRouter () .removeNode (nodeId);
		nodeId .time = 0;
	}
}

void
X3DBaseNode::addHandle (X3DBaseNode* const value)
{
	handle = value;
	handle -> setName (getName ());
	handle -> setUserData (getUserData ());
	handle -> replace (this);
	handle -> setup ();
}

void
X3DBaseNode::removeHandle ()
{
	if (handle)
	{
		replace (handle);
		handle = nullptr;
	}
}

void
X3DBaseNode::fromStream (std::istream & istream)
throw (Error <INVALID_X3D>,
       Error <NOT_SUPPORTED>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	//	return istream >> value;
}

void
X3DBaseNode::toStream (std::ostream & ostream) const
{
	Generator::PushContext ();

	const std::string name = Generator::GetName (this);

	if (not name .empty ())
	{
		if (Generator::ExistsNode (this))
		{
			ostream
				<< "USE"
				<< Generator::Space
				<< name;

			Generator::PopContext ();

			return;
		}
	}

	if (not getComments () .empty ())
	{
		ostream
			<< Generator::Comment
			<< getComments () .front ()
			<< Generator::Break;

		for (const auto & comment : basic::adapter (getComments () .begin () + 1, getComments (). end ()))
		{
			ostream
				<< Generator::Indent
				<< Generator::Comment
				<< comment
				<< Generator::ForceBreak;
		}

		ostream << Generator::Indent;
	}

	if (not name .empty ())
	{
		Generator::AddNode (this);

		ostream
			<< "DEF"
			<< Generator::Space
			<< name
			<< Generator::Space;
	}

	ostream
		<< getTypeName ()
		<< Generator::TidySpace
		<< '{';

	const FieldDefinitionArray userDefinedFields = getUserDefinedFields ();

	size_t fieldTypeLength  = 0;
	size_t accessTypeLength = 0;

	switch (Generator::Style ())
	{
		case Generator::SMALLEST:
		case Generator::SMALL:
		{
			break;
		}
		default:
		{
			for (const auto & field : userDefinedFields)
			{
				fieldTypeLength = std::max (fieldTypeLength, field -> getTypeName () .length ());

				accessTypeLength = std::max (accessTypeLength, Generator::AccessTypes [field] .length ());
			}

			break;
		}
	}

	if (not userDefinedFields .empty ())
	{
		ostream
			<< Generator::TidyBreak
			<< Generator::IncIndent;

		for (const auto & field : basic::adapter (userDefinedFields .begin (), userDefinedFields .end () - 1))
		{
			toStreamUserDefinedField (ostream, field, fieldTypeLength, accessTypeLength);
			ostream << Generator::Break;
		}

		toStreamUserDefinedField (ostream, userDefinedFields .back (), fieldTypeLength, accessTypeLength);
		ostream << Generator::TidyBreak;

		ostream
			<< Generator::DecIndent
			<< Generator::TidyBreak;
	}

	const FieldDefinitionArray fields = getInitializeableFields (Generator::ExpandNodes ());

	if (fields .empty ())
	{
		if (userDefinedFields .empty ())
			ostream << Generator::TidySpace;
		else
			ostream << Generator::Indent;
	}
	else
	{
		if (userDefinedFields .empty ())
			ostream << Generator::TidyBreak;

		ostream << Generator::IncIndent;

		for (const auto & field : basic::adapter (fields .begin (), fields .end () - 1))
		{
			toStreamField (ostream, field, fieldTypeLength, accessTypeLength);
			ostream << Generator::Break;
		}

		toStreamField (ostream, fields .back (), fieldTypeLength, accessTypeLength);
		ostream << Generator::TidyBreak;

		ostream
			<< Generator::DecIndent
			<< Generator::Indent;
	}

	if (not getInnerComments () .empty ())
	{
		ostream
			<< Generator::TidyBreak
			<< Generator::IncIndent;

		for (const auto & comment : getInnerComments ())
		{
			ostream
				<< Generator::Indent
				<< Generator::Comment
				<< comment
				<< Generator::Break;
		}

		ostream
			<< Generator::DecIndent
			<< Generator::Indent;
	}

	ostream << '}';

	Generator::PopContext ();
}

void
X3DBaseNode::toStreamField (std::ostream & ostream, X3DFieldDefinition* const field, const size_t fieldTypeLength, const size_t accessTypeLength) const
{
	for (const auto & comment : field -> getComments ())
	{
		ostream
			<< Generator::Indent
			<< Generator::Comment
			<< comment
			<< Generator::Break;
	}

	if (field -> getReferences () .empty ())
	{
		// Output build field

		ostream << Generator::Indent;

		if (Generator::X3DFieldNames ())
			ostream << field -> getName ();
		else
			ostream << field -> getAliasName ();

		ostream
			<< Generator::Space
			<< *field;
	}
	else
	{
		for (const auto & reference : field -> getReferences ())
		{
			// Output build in reference

			ostream << Generator::Indent;

			if (Generator::X3DFieldNames ())
				ostream << field -> getName ();
			else
				ostream << field -> getAliasName ();

			ostream
				<< Generator::Space
				<< "IS"
				<< Generator::Space
				<< reference -> getName ();
		}
	}
}

void
X3DBaseNode::toStreamUserDefinedField (std::ostream & ostream, X3DFieldDefinition* const field, const size_t fieldTypeLength, const size_t accessTypeLength) const
{
	for (const auto & comment : field -> getComments ())
	{
		ostream
			<< Generator::Indent
			<< Generator::Comment
			<< comment
			<< Generator::Break;
	}

	if (field -> getReferences () .empty ())
	{
		// Output user defined field

		ostream
			<< Generator::Indent
			<< std::setiosflags (std::ios::left)
			<< std::setw (accessTypeLength);

		ostream << Generator::AccessTypes [field];

		ostream
			<< Generator::Space
			<< std::setiosflags (std::ios::left) << std::setw (fieldTypeLength) << field -> getTypeName ()
			<< Generator::Space;

		if (Generator::X3DFieldNames ())
			ostream << field -> getName ();
		else
			ostream << field -> getAliasName ();

		if (field -> isInitializeable ())
		{
			ostream
				<< Generator::Space
				<< *field;
		}
	}
	else
	{
		bool initializableReference = false;

		for (const auto & reference : field -> getReferences ())
		{
			initializableReference |= reference -> isInitializeable ();

			// Output user defined reference field

			ostream
				<< Generator::Indent
				<< std::setiosflags (std::ios::left)
				<< std::setw (accessTypeLength);

			ostream << Generator::AccessTypes [field];

			ostream
				<< Generator::Space
				<< std::setiosflags (std::ios::left) << std::setw (fieldTypeLength) << field -> getTypeName ()
				<< Generator::Space;

			if (Generator::X3DFieldNames ())
				ostream << field -> getName ();
			else
				ostream << field -> getAliasName ();

			ostream
				<< Generator::Space
				<< "IS"
				<< Generator::Space
				<< reference -> getName ();
		}

		if (field -> getAccessType () == inputOutput and not initializableReference)
		{
			// Output user defined field

			ostream
				<< Generator::Indent
				<< std::setiosflags (std::ios::left)
				<< std::setw (accessTypeLength);

			ostream << Generator::AccessTypes [field];

			ostream
				<< Generator::Space
				<< std::setiosflags (std::ios::left) << std::setw (fieldTypeLength) << field -> getTypeName ()
				<< Generator::Space;

			if (Generator::X3DFieldNames ())
				ostream << field -> getName ();
			else
				ostream << field -> getAliasName ();

			if (field -> isInitializeable ())
			{
				ostream
					<< Generator::Space
					<< *field;
			}
		}
	}
}

void
X3DBaseNode::dispose ()
{
	X3DBaseNode::shutdown () .processInterests ();

	X3DChildObject::dispose ();

	removeEvents ();

	for (const auto & field : fieldDefinitions)
		field -> removeParent (this);

	fields .clear ();
	fieldDefinitions .clear ();

	executionContext -> removeParent (this);
}

X3DBaseNode::~X3DBaseNode ()
{ }

} // X3D
} // titania
