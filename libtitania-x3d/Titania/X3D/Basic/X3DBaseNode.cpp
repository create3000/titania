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
#include "../Fields/ArrayFields.h"
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


/**
 *  Contructs a new X3DBaseNode into @a browser and @a executionContext.
 *
 *  Important: a in this way newly created X3DBaseNode must be setuped manually. On setup there must be a
 *  OpenGL context available.
 *
 *  You can use:
 *
 *      auto node = new Transform (executionContext);
 *      executionContext -> addUninitalizedNode (node);
 *      executionContext -> setup ();
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
	             children (),
	             internal (false),
	                saved (false),
	extendedEventHandling (true),
	               nodeId ({ 0 }),
	               events (),
	             comments (),
	          initialized (false),
	       shutdownOutput ()
{
	assert (browser);
	assert (executionContext);
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

	for (const auto & child : children)
		child -> isTainted (false);

	initialized = true;

	initialize ();
}

/**
 *
 *  Creates either a clone or a copy of this node. If the named node already exists in @a executionContext this node is
 *  returned otherwise a copy of this node and all of its children is made.  See copy ().
 *
 *  The nodes must be setuped with:
 *
 *      auto clone = node -> clone ();
 *      executionContext -> setup ();
 */

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
		catch (const X3DError &)
		{
			return copy (executionContext);
		}
	}
}

/**
 *
 *  Creates a copy of this node and all of its child nodes into @a executionContext. If a named node of one of this
 *  node children already exists in @a executionContext then only a clone is created.
 *
 *  The nodes must be setuped with:
 *
 *      auto copy = node -> copy ();
 *      executionContext -> setup ();
 */

X3DBaseNode*
X3DBaseNode::copy (X3DExecutionContext* const executionContext) const
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>)
{
	const SFNode copy = create (executionContext); // Never try to optimize this.

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
						throw Error <INVALID_NAME> ("No reference '" + originalReference -> getName () + "' inside execution context " + executionContext -> getTypeName () + " '" + executionContext -> getName () + "'.");
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
	const SFNode copy = create (executionContext);

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

	SFNode replacement (this);

	for (auto & parent : parents)
	{
		parent -> write (replacement);
		parent -> addEvent ();
	}
}

time_type
X3DBaseNode::getCurrentTime () const
{
	return getBrowser () -> getClock () -> cycle ();
}

const X3DBaseNode*
X3DBaseNode::getType () const
throw (Error <DISPOSED>)
{
	try
	{
		return getBrowser () -> getNode (getTypeName ());
	}
	catch (const X3DError &)
	{
		return this;
	}
}
	
bool
X3DBaseNode::existsField (X3DFieldDefinition* const field) const
{
	try
	{
		return field == getField (field -> getName ());
	}
	catch (const X3D::X3DError &)
	{
		return false;
	}
}

void
X3DBaseNode::addField (const AccessType accessType, const std::string & name, X3DFieldDefinition & field)
{
	const auto iter = fields .find (name);

	if (iter not_eq fields .end ())
		removeField (iter, iter -> second not_eq &field);

	if (not isInitialized ())
		field .isTainted (true);

	field .addParent (this);
	field .setAccessType (accessType);
	field .setName (name);

	fieldDefinitions .emplace_back (&field);
	fields .emplace (name, &field);
}

void
X3DBaseNode::addField (const VersionType version, const std::string & alias, const std::string & name)
{
	auto & fieldAlias = fieldAliases [version];

	fieldAlias .first [alias] = name;
	fieldAlias .second [name] = alias;
}

void
X3DBaseNode::removeField (const std::string & name)
{
	removeField (fields .find (name), true);
}

void
X3DBaseNode::removeField (const FieldIndex::iterator & field, const bool removeParent)
{
	if (field == fields .end ())
		return;

	const auto iter = std::find (fieldDefinitions .begin (), fieldDefinitions .end (), field -> second);

	if (fieldDefinitions .end () - iter <= FieldDefinitionArray::difference_type (numUserDefinedFields))
	{
		-- numUserDefinedFields;

		if (removeParent)
			field -> second -> removeParent (this);
	}
	else
	{
		// If field is a pre defined field, we defer dispose.
		shutdownOutput .addInterest (field -> second, &X3DFieldDefinition::removeParent, this);
	}

	fieldDefinitions .erase (iter);
	fields .erase (field);
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
X3DBaseNode::getFieldName (const std::string & alias) const
{
	for (const auto & version : fieldAliases)
	{
		const auto fieldAlias = version .second .first .find (alias);

		if (fieldAlias not_eq version .second .first .end ())
			return fieldAlias -> second;
	}

	return alias;
}

const std::string &
X3DBaseNode::getFieldName (const std::string & name, const VersionType version) const
{
	if (not fieldAliases .empty ())
	{
		const auto fieldAlias = fieldAliases .find (version);

		if (fieldAlias not_eq fieldAliases .end ())
		{
			const auto alias = fieldAlias -> second .second .find (name);

			if (alias not_eq fieldAlias -> second .second  .end ())
				return alias -> second;
		}
	}

	return name;
}

void
X3DBaseNode::addUserDefinedField (const AccessType accessType, const std::string & name, X3DFieldDefinition* const field)
{
	addField (accessType, name, *field);

	++ numUserDefinedFields;
}

void
X3DBaseNode::removeUserDefinedField (X3DFieldDefinition* const field)
{
	// Test if field is a user defined field.

	const auto iter = std::find (fieldDefinitions .begin (), fieldDefinitions .end (), field);
	
	if (iter == fieldDefinitions .end ())
		return;

	if (not (fieldDefinitions .end () - iter <= FieldDefinitionArray::difference_type (numUserDefinedFields)))
		return;

	removeField (field -> getName ());
}

FieldDefinitionArray
X3DBaseNode::getPreDefinedFields () const
{
	FieldDefinitionArray predefinedFields;

	for (const auto & field : basic::adapter (fieldDefinitions .begin (), fieldDefinitions .end () - numUserDefinedFields))
	{
		try
		{
			getType () -> getField (field -> getName ());
	
			predefinedFields .emplace_back (field);
		}
		catch (const X3D::X3DError &)
		{
			// Field is not defined in ExternProto but in Proto.
		}
	}

	return predefinedFields;
}

FieldDefinitionArray
X3DBaseNode::getUserDefinedFields () const
{
	return FieldDefinitionArray (fieldDefinitions .end () - numUserDefinedFields, fieldDefinitions .end ());
}

FieldDefinitionArray
X3DBaseNode::getChangedFields () const
{
	FieldDefinitionArray changedFields;

	for (const auto & field : basic::adapter (fieldDefinitions .begin (), fieldDefinitions .end () - numUserDefinedFields))
	{
		if (field -> getReferences () .empty ())
		{
			if (not field -> isInitializeable ())
				continue;

			try
			{
				if (isDefaultValue (field))
					continue;
			}
			catch (const X3DError &)
			{
				// This can happen if a ExternProto has less field than the proto.
				continue;
			}
		}

		changedFields .emplace_back (field);
	}

	return changedFields;
}

bool
X3DBaseNode::isDefaultValue (const X3DFieldDefinition* const field) const
throw (Error <INVALID_NAME>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	const X3DFieldDefinition* const declarationField = getType () -> getField (field -> getName ());

	return *field == *declarationField;
}

void
X3DBaseNode::addChild (X3DChildObject & child)
{
	child .isTainted (true);

	children .insert (&child);

	X3DChildObject::addChild (child);
}

void
X3DBaseNode::removeChild (X3DChildObject & child)
{
	children .erase (&child);

	X3DChildObject::removeChild (child);
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
X3DBaseNode::isInternal (const bool value)
{
	internal = value;

	if (internal)
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
X3DBaseNode::saveState ()
{
	saved = true;
}

void
X3DBaseNode::restoreState ()
{
	saved = false;
}

void
X3DBaseNode::addTool (X3DBaseNode* const tool)
{
	tool -> replace (this);
	tool -> setup ();
}

void
X3DBaseNode::removeTool (X3DBaseNode* const node)
{
	node -> replace (this);
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

	// Register for processEvent

	events .emplace_back (getBrowser () -> addTaintedObject (object, event));

	// Register for eventsProcessed

	if (not isTainted ())
	{
		if (object -> isInput () or (extendedEventHandling and not object -> isOutput ()))
		{
			isTainted (true);

			nodeId = getBrowser () -> addTaintedNode (this);
		}
	}
}

void
X3DBaseNode::addEvent ()
{
	if (not isTainted ())
	{
		isTainted (true);

		getBrowser () -> addEvent ();

		nodeId = getBrowser () -> addTaintedNode (this);
	}
}

void
X3DBaseNode::eventsProcessed ()
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
		getBrowser () -> removeTaintedObject (event);

	events .clear ();

	if (nodeId .time)
	{
		getBrowser () -> removeTaintedNode (nodeId);
		nodeId .time = 0;
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
	ostream .imbue (std::locale::classic ());

	if (Generator::IsSharedNode (this))
	{
		ostream << "NULL";
		return;
	}

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
			<< Generator::ForceBreak;

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
		ostream << Generator::Break;

		ostream
			<< Generator::DecIndent
			<< Generator::TidyBreak;
	}

	const FieldDefinitionArray fields = getChangedFields ();

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
				<< Generator::ForceBreak;
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
			<< Generator::ForceBreak;
	}

	if (field -> getReferences () .empty ())
	{
		// Output build in field

		ostream << Generator::Indent;

		ostream << getFieldName (field -> getName (), Generator::Version ());

		ostream
			<< Generator::Space
			<< *field;
	}
	else
	{
		size_t index                  = 0;
		bool   initializableReference = false;

		for (const auto & reference : field -> getReferences ())
		{
			initializableReference |= reference -> isInitializeable ();

			// Output build in reference

			ostream << Generator::Indent;

			ostream << getFieldName (field -> getName (), Generator::Version ());

			ostream
				<< Generator::Space
				<< "IS"
				<< Generator::Space
				<< reference -> getName ();

			++ index;

			if (index not_eq field -> getReferences () .size ())
				ostream << Generator::Break;
		}

		try
		{
			// If the field is a inputOutput and we have as reference only inputOnly or outputOnly we must output the value
			// for this field.

			if (field -> getAccessType () == inputOutput and not initializableReference and not isDefaultValue (field))
			{
				// Output build in field

				ostream
					<< Generator::Break
					<< Generator::Indent;

				ostream << getFieldName (field -> getName (), Generator::Version ());

				ostream
					<< Generator::Space
					<< *field;
			}
		}
		catch (const X3DError &)
		{
			// Catch isDefaultValue, this can happen if the Externproto has less fields than the proto but should not happen here
			// as the field has references.
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
			<< Generator::ForceBreak;
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

		ostream << field -> getName ();

		if (field -> isInitializeable ())
		{
			ostream
				<< Generator::Space
				<< *field;
		}
	}
	else
	{
		size_t index                  = 0;
		bool   initializableReference = false;

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

			ostream << field -> getName ();

			ostream
				<< Generator::Space
				<< "IS"
				<< Generator::Space
				<< reference -> getName ();

			++ index;

			if (index not_eq field -> getReferences () .size ())
				ostream << Generator::Break;
		}

		// If the field is a inputOutput and we have as reference only inputOnly or outputOnly we must output the value
		// for this field.

		if (field -> getAccessType () == inputOutput and not initializableReference and not field -> isDefaultValue ())
		{
			// Output user defined field

			ostream
				<< Generator::Break
				<< Generator::Indent
				<< std::setiosflags (std::ios::left)
				<< std::setw (accessTypeLength);

			ostream << Generator::AccessTypes [field];

			ostream
				<< Generator::Space
				<< std::setiosflags (std::ios::left) << std::setw (fieldTypeLength) << field -> getTypeName ()
				<< Generator::Space;

			ostream << field -> getName ();

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
X3DBaseNode::toXMLStream (std::ostream & ostream) const
{
	ostream .imbue (std::locale::classic ());

	if (Generator::IsSharedNode (this))
	{
		ostream
			<< Generator::Indent
			<< "<!-- NULL -->";

		return;
	}

	Generator::PushContext ();

	const std::string name = Generator::GetName (this);

	if (not name .empty ())
	{
		if (Generator::ExistsNode (this))
		{
			ostream
				<< Generator::Indent
				<< "<"
				<< getTypeName ()
				<< Generator::Space
				<< "USE='"
				<< XMLEncode (name)
				<< "'";

			const auto containerField = Generator::GetContainerField ();

			if (containerField)
			{
				if (containerField -> getName () not_eq getContainerField ())
				{
					ostream
						<< Generator::Space
						<< "containerField='"
						<< XMLEncode (containerField -> getName ())
						<< "'";
				}
			}

			ostream << "/>";

			Generator::PopContext ();

			return;
		}
	}

	ostream
		<< Generator::Indent
		<< "<"
		<< getTypeName ();

	if (not name .empty ())
	{
		Generator::AddNode (this);

		ostream
			<< Generator::Space
			<< "DEF='"
			<< XMLEncode (name)
			<< "'";
	}

	const auto containerField = Generator::GetContainerField ();

	if (containerField)
	{
		if (containerField -> getName () not_eq getContainerField ())
		{
			ostream
				<< Generator::Space
				<< "containerField='"
				<< XMLEncode (containerField -> getName ())
				<< "'";
		}
	}

	const FieldDefinitionArray fields            = getChangedFields ();
	const FieldDefinitionArray userDefinedFields = getUserDefinedFields ();

	FieldDefinitionArray references;
	FieldDefinitionArray childNodes;

	const MFString* cdata = getCData ();

	if (cdata and cdata -> empty ())
		cdata = nullptr;

	ostream
		<< Generator::IncIndent
		<< Generator::IncIndent;

	for (const auto & field : fields)
	{
		// If the field is a inputOutput and we have as reference only inputOnly or outputOnly we must output the value
		// for this field.

		bool mustOutputValue = false;

		if (field -> getAccessType () == inputOutput and not field -> getReferences () .empty ())
		{
			bool initializeableReference = false;

			for (const auto & reference : field -> getReferences ())
				initializeableReference |= reference -> isInitializeable ();

			try
			{
				if (not initializeableReference)
					mustOutputValue = not isDefaultValue (field);
			}
			catch (const X3DError &)
			{
				mustOutputValue = false;
			}
		}

		if (field -> getReferences () .empty () or mustOutputValue)
		{
			if (mustOutputValue)
				references .emplace_back (field);

			switch (field -> getType ())
			{
				case X3DConstants::SFNode:
				case X3DConstants::MFNode:
				{
					childNodes .emplace_back (field);
					break;
				}
				default:
				{
					if (field == cdata)
						break;

					ostream
						<< Generator::Break
						<< Generator::Indent
						<< getFieldName (field -> getName (), Generator::Version ())
						<< "='"
						<< XMLEncode (field)
						<< "'";

					break;
				}
			}
		}
		else
		{
			references .emplace_back (field);
		}
	}

	ostream
		<< Generator::DecIndent
		<< Generator::DecIndent;

	if (userDefinedFields .empty () and references .empty () and childNodes .empty () and not cdata)
	{
		ostream << "/>";
	}
	else
	{
		ostream
			<< ">"
			<< Generator::Break
			<< Generator::IncIndent;

		for (const auto & field : userDefinedFields)
		{
			ostream
				<< Generator::Indent
				<< "<field"
				<< Generator::Space
				<< "accessType='"
				<< Generator::X3DAccessTypes [field]
				<< "'"
				<< Generator::Space
				<< "type='"
				<< field -> getTypeName ()
				<< "'"
				<< Generator::Space
				<< "name='"
				<< XMLEncode (field -> getName ())
				<< "'";

			// If the field is a inputOutput and we have as reference only inputOnly or outputOnly we must output the value
			// for this field.

			bool mustOutputValue = false;

			if (field -> getAccessType () == inputOutput and not field -> getReferences () .empty ())
			{
				bool initializeableReference = false;

				for (const auto & reference : field -> getReferences ())
					initializeableReference |= reference -> isInitializeable ();

				if (not initializeableReference)
					mustOutputValue = not field -> isDefaultValue ();
			}

			if (field -> getReferences () .empty () or mustOutputValue)
			{
				if (mustOutputValue)
					references .emplace_back (field);

				if (not field -> isInitializeable () or field -> isDefaultValue ())
				{
					ostream
						<< "/>"
						<< Generator::Break;
				}
				else
				{
					// Output value

					switch (field -> getType ())
					{
						case X3DConstants::SFNode:
						case X3DConstants::MFNode:
						{
							Generator::PushContainerField (nullptr);

							ostream
								<< ">"
								<< Generator::Break
								<< Generator::IncIndent
								<< XMLEncode (field)
								<< Generator::Break
								<< Generator::DecIndent
								<< Generator::Indent
								<< "</field>"
								<< Generator::Break;

							Generator::PopContainerField ();

							break;
						}
						default:
						{
							ostream
								<< Generator::Space
								<< "value='"
								<< XMLEncode (field)
								<< "'"
								<< "/>"
								<< Generator::Break;

							break;
						}
					}
				}
			}
			else
			{
				references .emplace_back (field);

				ostream
					<< "/>"
					<< Generator::Break;
			}
		}

		if (not references .empty ())
		{
			ostream
				<< Generator::Indent
				<< "<IS>"
				<< Generator::Break
				<< Generator::IncIndent;

			for (const auto & field : references)
			{
				for (const auto & reference : field -> getReferences ())
				{
					ostream
						<< Generator::Indent
						<< "<connect"
						<< Generator::Space
						<< "nodeField='"
						<< XMLEncode (field -> getName ())
						<< "'"
						<< Generator::Space
						<< "protoField='"
						<< XMLEncode (reference -> getName ())
						<< "'"
						<< "/>"
						<< Generator::Break;
				}
			}

			ostream
				<< Generator::DecIndent
				<< Generator::Indent
				<< "</IS>"
				<< Generator::Break;
		}

		for (const auto & field : childNodes)
		{
			Generator::PushContainerField (field);

			ostream
				<< XMLEncode (field)
				<< Generator::Break;

			Generator::PopContainerField ();
		}

		if (cdata)
		{
			static const pcrecpp::RE cdata_end_pattern ("(\\]\\]\\>)");
			static const std::string cdata_end_subs ("\\\\]\\\\]\\\\>");

			for (std::string value : *cdata)
			{
				cdata_end_pattern .GlobalReplace (cdata_end_subs, &value);
			
				ostream
					<< "<![CDATA["
					<< value
					<< "]]> "
					<< Generator::Break;
			}
		}

		ostream
			<< Generator::DecIndent
			<< Generator::Indent
			<< "</"
			<< getTypeName ()
			<< ">";
	}

	Generator::PopContext ();
}

void
X3DBaseNode::dispose ()
{
	shutdownOutput .processInterests ();
	shutdownOutput .dispose ();

	X3DChildObject::dispose ();

	removeEvents ();

	for (const auto & field : fieldDefinitions)
		field -> removeParent (this);

	for (const auto & child : children)
		child -> dispose ();

	fields .clear ();
	fieldDefinitions .clear ();
	children .clear ();

	executionContext -> removeParent (this);
}

X3DBaseNode::~X3DBaseNode ()
{ }

} // X3D

} // titania
