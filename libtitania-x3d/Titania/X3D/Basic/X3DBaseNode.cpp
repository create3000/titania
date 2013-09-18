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
	            component (),
	             typeName (),
	             nodeType (),
	     fieldDefinitions (),
	               fields (),
	         fieldAliases (),
	 numUserDefinedFields (0),
	extendedEventHandling (true),
	               nodeId (),
	               handle (NULL),
	             comments (),
	       shutdownOutput ()
{
	assert (executionContext);
}

X3DBaseNode*
X3DBaseNode::clone (X3DExecutionContext* const executionContext) const
{
	return executionContext -> getNamedNode (getName ());
}

X3DBaseNode*
X3DBaseNode::copy (X3DExecutionContext* const executionContext) const
{
	X3DBaseNode* copy = create (executionContext);

	if (getName () .size ())
		executionContext -> updateNamedNode (getName (), copy);

	for (const auto & fieldDefinition : fieldDefinitions)
	{
		try
		{
			// Default fields

			X3DFieldDefinition* field = copy -> getField (fieldDefinition -> getName ());

			if (field -> getAccessType () == fieldDefinition -> getAccessType () and field -> getType () == fieldDefinition -> getType ())
			{
				if (fieldDefinition -> getReferences () .size ())
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
				else
				{
					if (fieldDefinition -> isInitializeable ())
						fieldDefinition -> clone (executionContext, field);
				}
			}
			else
				throw Error <INVALID_NAME> ("");
		}
		catch (const Error <INVALID_NAME> &)
		{
			// User defined fields from Script and Shader

			if (fieldDefinition -> getReferences () .size ())
			{
				// IS relationship

				X3DFieldDefinition* field = fieldDefinition -> clone ();

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
						throw Error <INVALID_NAME> ("No such event or field '" + originalReference -> getName () + " inside node.");
					}
				}
			}
			else
			{
				copy -> addUserDefinedField (fieldDefinition -> getAccessType (), 
				                             fieldDefinition -> getName (),
				                             fieldDefinition -> clone (executionContext));
			}
		}
	}

	executionContext -> addUninitializedNode (copy);

	return copy;
}

void
X3DBaseNode::replace (X3DBaseNode* const node, const std::set <const X3DFieldDefinition*> & exclude)
{
	for (auto & parent : node -> getParentFields (exclude))
		parent -> setValue (this);
}

void
X3DBaseNode::remove (const std::set <const X3DFieldDefinition*> & exclude)
{
	std::set <SFNode*> sfnodes;
	std::set <MFNode*> mfnodes;

	for (auto & parent : getParents ())
	{
		auto sfnode = dynamic_cast <SFNode*> (parent);
	
		if (sfnode)
		{
			if (exclude .find (sfnode) == exclude .end ())
			{
				bool fieldValue = false;
			
				for (auto & secondParent : sfnode -> getParents ())
				{
					auto mfnode = dynamic_cast <MFNode*> (secondParent);
		
					if (mfnode)
					{
						fieldValue = true;
						
						if (exclude .find (mfnode) == exclude .end ())
							mfnodes .insert (mfnode);
					}
				}

				if (not fieldValue)
					sfnodes .insert (sfnode);
			}
		}
	}

	for (auto & sfnode : sfnodes)
		sfnode -> setValue (nullptr);

	for (auto & mfnode : mfnodes)
		mfnode -> erase (std::remove (mfnode -> begin (), mfnode -> end (), this), mfnode -> end ());
}

void
X3DBaseNode::assign (const X3DBaseNode* value)
{
	for (const auto & field : getFieldDefinitions ())
		*field = *value -> getField (field -> getName ());
}

time_type
X3DBaseNode::getCurrentTime () const
{
	return getBrowser () -> getCurrentTime ();
}

size_t
X3DBaseNode::getNumClones () const
{
	size_t numClones = 0;

	for (const auto & parentField : getParents ())
	{
		if (dynamic_cast <X3DFieldDefinition*> (parentField))
		{
			if (parentField -> getTypeName () == "SFNode")
			{
				// Only X3DNodes, ie nodes in the scene graph, have field names

				if (parentField -> getName () .length ())
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

				else
				{
					for (const auto & parent : parentField -> getParents ())
					{
						// Only X3DNodes, ie nodes in the scene graph, have field names

						if (parent -> getTypeName () == "MFNode" and parent -> getName () .length ())
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
			}
		}
	}

	return numClones;
}

void
X3DBaseNode::setInternal (bool value)
{
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

const X3DBaseNode*
X3DBaseNode::getType () const
throw (Error <DISPOSED>)
{
	return getBrowser () -> getNode (getTypeName ());
}

std::vector <SFNode*>
X3DBaseNode::getParentFields (const std::set <const X3DFieldDefinition*> & exclude) const
{
	std::vector <SFNode*> fields;

	for (auto & parent : getParents ())
	{
		auto sfnode = dynamic_cast <SFNode*> (parent);

		if (sfnode)
		{
			if (exclude .find (sfnode) == exclude .end ())
			{
				bool insert = true;

				for (auto & secondParent : sfnode -> getParents ())
				{
					auto mfnode = dynamic_cast <MFNode*> (secondParent);

					if (mfnode)
						insert = exclude .find (mfnode) == exclude .end ();
				}

				if (insert)
					fields .emplace_back (sfnode);
			}
		}
	}

	return fields;
}

void
X3DBaseNode::addField (const AccessType accessType, const std::string & name, X3DFieldDefinition & field)
{
	if (fields .find (name) not_eq fields .end ())
	{
		removeField (name);
		__LOG__ << " field " << getTypeName () << "." << name << " already exists in this node'." << std::endl;
	}

	field .isTainted (true);
	field .addParent (this);
	field .setAccessType (accessType);
	field .setName (name);
	field .setAliasName (name);

	fieldDefinitions .push_back (&field);
	fields .insert (std::make_pair (name, &field));
}

void
X3DBaseNode::removeField (const std::string & name)
{
	auto field = fields .find (name);

	if (field not_eq fields .end ())
	{
		auto iter = std::find (fieldDefinitions .begin (), fieldDefinitions .end (), field -> second);

		if (fieldDefinitions .end () - iter <= FieldDefinitionArray::difference_type (numUserDefinedFields))
		{
			-- numUserDefinedFields;
		}

		fieldDefinitions .erase (iter);
		fields .erase (field);
		field -> second -> removeParent (this);
	}
}

void
X3DBaseNode::addField (const std::string & name, const std::string & field)
{
	const auto iter = fields .find (field);

	if (iter not_eq fields .end ())
	{
		fieldAliases .insert (std::make_pair (name, field));
		iter -> second -> setAliasName (name);
	}
}

X3DFieldDefinition*
X3DBaseNode::getField (const std::string & name) const
throw (Error <INVALID_NAME>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	auto field = fields .find (getFieldName (name));

	if (field not_eq fields .end ())
		return field -> second;

	if (name .substr (0, 4) == "set_")
	{
		auto field = fields .find (getFieldName (name .substr (4)));

		if (field not_eq fields .end () and field -> second -> getAccessType () == inputOutput)
			return field -> second;
	}

	if (name .size () > 8 and name .substr (name .size () - 8) == "_changed")
	{
		auto field = fields .find (getFieldName (name .substr (0, name .size () - 8)));

		if (field not_eq fields .end () and field -> second -> getAccessType () == inputOutput)
			return field -> second;
	}

	throw Error <INVALID_NAME> ("No such field '" + name + "' in node " + getTypeName () + ".");
}

const std::string &
X3DBaseNode::getFieldName (const std::string & name) const
{
	auto fieldAlias = fieldAliases .find (name);

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

		changedFields .push_back (field);
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

void
X3DBaseNode::addEvent (X3DChildObject* const object)
{
	if (object -> isTainted ())
		return;

	object -> isTainted (true);

	addEvent (object, Event (object));
}

void
X3DBaseNode::addEvent (X3DChildObject* const object, const Event & event)
{
	// __LOG__ << object << " : " << object -> getName () << " : " << object -> getTypeName () << " : " << getName () << " : " << getTypeName () << " : " << this << std::endl;

	getBrowser () -> addEvent ();

	// Register for processEvents

	getBrowser () -> getRouter () .addEvent (object, event);

	// Register for eventsProcessed

	if (object -> isInput () or (extendedEventHandling and not object -> isOutput ()))
	{
		if (isNotValid (nodeId))
			nodeId = getBrowser () -> getRouter () .addNode (this);
	}
}

void
X3DBaseNode::processEvents ()
{
	reset (nodeId);

	eventsProcessed ();
}

void
X3DBaseNode::removeEvents ()
{
	if (isValid (nodeId))
	{
		getBrowser () -> getRouter () .removeNode (nodeId);
		reset (nodeId);
	}
}

void
X3DBaseNode::addHandle (X3DBaseNode* const value)
{
	handle = value;
	handle -> addParent (this);
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
		handle -> removeParent (this);
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

	std::string name = Generator::GetName (this);

	if (name .length ())
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

	if (getComments () .size ())
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
				<< Generator::Break;
		}

		ostream << Generator::Indent;
	}

	if (name .length ())
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

	FieldDefinitionArray userDefinedFields = getUserDefinedFields ();

	size_t fieldTypeLength  = 0;
	size_t accessTypeLength = 0;

	if (Generator::Style () not_eq Generator::SMALLEST)
	{
		for (const auto & field : userDefinedFields)
		{
			fieldTypeLength = std::max (fieldTypeLength, field -> getTypeName () .length ());

			accessTypeLength = std::max (accessTypeLength, Generator::AccessTypes [field] .length ());
		}
	}

	if (userDefinedFields .size ())
	{
		ostream
			<< Generator::TidyBreak
			<< Generator::IncIndent;

		for (const auto & field : userDefinedFields)
		{
			for (const auto & comment : field -> getComments ())
			{
				ostream
					<< Generator::Indent
					<< Generator::Comment
					<< comment
					<< Generator::Break;
			}

			if (field -> getReferences () .size ())
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
						<< reference -> getName ()
						<< Generator::Break;
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

					ostream << Generator::Break;
				}
			}
			else
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

				ostream << Generator::Break;
			}
		}

		ostream
			<< Generator::DecIndent
			<< Generator::TidyBreak;
	}

	FieldDefinitionArray fields = getInitializeableFields (Generator::ExpandNodes ());

	if (fields .size ())
	{
		if (not userDefinedFields .size ())
			ostream << Generator::TidyBreak;

		ostream << Generator::IncIndent;

		for (const auto & field : fields)
		{
			for (const auto & comment : field -> getComments ())
			{
				ostream
					<< Generator::Indent
					<< Generator::Comment
					<< comment
					<< Generator::Break;
			}

			if (field -> getReferences () .size ())
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
						<< reference -> getName ()
						<< Generator::Break;
				}
			}
			else
			{
				// Output build field

				ostream << Generator::Indent;

				if (Generator::X3DFieldNames ())
					ostream << field -> getName ();
				else
					ostream << field -> getAliasName ();

				ostream
					<< Generator::Space
					<< *field
					<< Generator::Break;
			}
		}

		ostream
			<< Generator::DecIndent
			<< Generator::Indent;
	}
	else
	{
		if (userDefinedFields .size ())
			ostream << Generator::Indent;
		else
			ostream << Generator::TidySpace;
	}

	if (getInnerComments () .size ())
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
X3DBaseNode::dispose ()
{
	X3DChildObject::dispose ();

	shutdownOutput .processInterests ();

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
