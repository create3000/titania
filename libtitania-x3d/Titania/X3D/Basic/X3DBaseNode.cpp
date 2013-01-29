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
	              events (),                 
	           interests (),                 
	  receivedInputEvent (false)          
{
	assert (executionContext);

	setChildren (initialized);
}

X3DBaseNode*
X3DBaseNode::clone (X3DExecutionContext* const executionContext) const
{
	// We first try to get a named node if this node has a name and return this named node
	// instead of cloning this node. This is important for copying Proto nodes to create
	// PrototypeInstances.

	try
	{
		return executionContext -> getNamedNode (getName ());
	}
	catch (const Error <INVALID_NAME> &)
	{ }

	return copy (executionContext);
}

X3DBaseNode*
X3DBaseNode::copy (X3DExecutionContext* const executionContext) const
{
	X3DBaseNode* copy = create (executionContext);

	assert (copy);

	if (getName () .count ())
		executionContext -> updateNamedNode (getName (), copy);

	for (const auto & fieldDefinition : fieldDefinitions)
	{
		X3DFieldDefinition* field = copy -> getField (fieldDefinition -> getName ());

		if (field)
		{
			// default fields
			if (fieldDefinition -> getReference ())
			{
				// IS relationship

				X3DFieldDefinition* reference = executionContext -> getField (fieldDefinition -> getReference () -> getName ());

				if (reference)
				{
					// Field is also defined in prototype definiton head.

					// create IS relationship
					field -> setReference (reference);
				}
				else
					throw Error <INVALID_NAME> ("No such event or field '" + fieldDefinition -> getReference () -> getName () + ".");
			}
			else
			{
				if (fieldDefinition -> isInitializeable ())
				{
					X3DFieldDefinition* copy = fieldDefinition -> clone (executionContext);

					field -> write (*copy);

					delete copy;
				}
			}
		}
		else  // user defined fields (Script and Shader)
		{
			X3DFieldDefinition* field = fieldDefinition -> clone (executionContext);

			copy -> addUserDefinedField (fieldDefinition -> getAccessType (), fieldDefinition -> getName (), field);

			if (fieldDefinition -> getReference ()) // IS relationship
			{
				X3DFieldDefinition* reference = executionContext -> getField (fieldDefinition -> getReference () -> getName ());

				if (reference) // field is also defined in EXTERNPROTO
				{
					// create IS relationship
					field -> setReference (reference);
				}
				else
					throw Error <INVALID_NAME> ("No such event or field '" + fieldDefinition -> getReference () -> getName () + " inside node.");
			}
		}
	}

	copy -> setup ();

	return copy;
}

time_type
X3DBaseNode::getCurrentTime () const
{
	return getBrowser () -> getCurrentTime ();
}

size_t
X3DBaseNode::getNumClones () const
{
	// executionContext is the limit
	size_t numClones = 0;

	for (const auto & parentField : getParents ())
	{
		if (dynamic_cast <X3DFieldDefinition*> (parentField))
		{
			for (const auto & parentNode : parentField -> getParents ())
			{
				MFNode <X3DBaseNode>* mfnode = dynamic_cast <MFNode <X3DBaseNode>*> (parentNode);

				if (mfnode)
				{
					for (const auto & parentNode : mfnode -> getParents ())
					{
						if (parentNode and not dynamic_cast <X3DExecutionContext*> (parentNode))
							++ numClones;
					}
				}
				else
				{
					if (parentNode and not dynamic_cast <X3DExecutionContext*> (parentNode))
						++ numClones;
				}
			}
		}
	}

	//	std::clog << __func__ << ": " << getTypeName () << " " << getName () << " " << numClones << std::endl;
	//	for (const auto & parent : getParents ())
	//		std::clog << "  " << __func__ << ": " << parent -> getTypeName () << " " << parent -> getName () << std::endl;

	return numClones;
}

void
X3DBaseNode::setComponent (const std::string & value)
{
	component = value;
}

const std::string &
X3DBaseNode::getComponentName () const
{
	return component;
}

void
X3DBaseNode::setTypeName (const std::string & value)
{
	//	std::clog << __FILE__ << ":" << __LINE__ << ": in function " << __func__ << ": " << value << std::endl;
	typeName = value;
}

const std::string &
X3DBaseNode::getTypeName () const
throw (Error <DISPOSED>)
{
	return typeName;
}

const X3DBaseNode*
X3DBaseNode::getType () const
throw (Error <DISPOSED>)
{
	return getBrowser () -> getNode (getTypeName ());
}

void
X3DBaseNode::addNodeType (const X3DConstants::NodeType value)
{
	//	std::clog << __FILE__ << ":" << __LINE__ << ": in function " << __func__ << ": " << Generator::NodeTypes .at (value) << std::endl;
	nodeType .push_back (value);
}

const NodeTypeArray &
X3DBaseNode::getNodeType () const
{
	return nodeType;
}

X3DBaseNode*
X3DBaseNode::getLocalNode ()
{
	return this;
}

//void
//X3DBaseNode::addFields (std::initializer_list <X3DFieldDefinition &> fields)
//throw (Error <INVALID_FIELD>)
//{
//	size_t i = 0;
//	for (const auto & field : fields)
//		addField (field, getFieldDefintions () [i++]);
//}

//void
//X3DBaseNode::addField (X3DFieldDefinition & field, const X3DFieldDefinition & fieldDefinition)
//throw (Error <INVALID_FIELD>)
//{
//	field .setReference (fieldDefinition);
//
//	if (not fields .insert (field) -> second)
//		throw Error <INVALID_FIELD> ("In function " + std::string (__func__) + " 'field " + getTypeName () + "." + name + "' already exists in field set'.");
//}

void
X3DBaseNode::addField (const AccessType accessType, const basic::id & name, X3DFieldDefinition & field)
{
	if (fields .find (name) not_eq fields .end ())
	{
		removeField (name);
		
		std::clog << "Warning: In function " << std::string (__func__) << " 'field " << getTypeName () << "." << name << "' already exists in field set'." << std::endl;
	}

	field .setAccessType (accessType);
	field .setName (name);
	field .setAliasName (name);

	fieldDefinitions .push_back (&field);
	fields .insert (std::make_pair (name, &field));
}

void
X3DBaseNode::removeField (const basic::id & name)
{
	auto field = fields .find (name);

	if (field not_eq fields .end ())
	{
		fieldDefinitions .erase (std::find (fieldDefinitions .begin (), fieldDefinitions .end (), field -> second));
		fields .erase (field);
	}
}

void
X3DBaseNode::addFieldAlias (const std::string & name, const std::string & field)
{
	FieldsMap::const_iterator iter = fields .find (field);

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

		if (field not_eq fields .end ())
			return field -> second;
	}

	if (name .size () > 8 and name .substr (name .size () - 8) == "_changed")
	{
		auto field = fields .find (getFieldName (name .substr (0, name .size () - 8)));

		if (field not_eq fields .end ())
			return field -> second;
	}

	return NULL;
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
X3DBaseNode::addUserDefinedField (const AccessType accessType, const basic::id & name, X3DFieldDefinition* const field)
{
	addField (accessType, name, *field);

	++ numUserDefinedFields;
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

	const X3DBaseNode* declaration = getType ();

	for (const auto & field : basic::adapter (fieldDefinitions .begin (), fieldDefinitions .end () - numUserDefinedFields))
	{
		if (not field -> getReference ())
		{
			if (not field -> isInitializeable ())
				continue;

			if (not all and * field == *declaration -> getField (field -> getName ()))
				continue;
		}

		changedFields .push_back (field);
	}

	return changedFields;
}

const FieldsMap &
X3DBaseNode::getFields () const
{
	return fields;
}

const FieldDefinitionArray &
X3DBaseNode::getFieldDefinitions () const
{
	return fieldDefinitions;
}

void
X3DBaseNode::setup ()
{
	executionContext -> addParent (this);

	for (auto & field : fields)
		field .second -> addParent (this);

	initialize ();

	initialized = getCurrentTime ();
}

void
X3DBaseNode::registerEvent (X3DChildObject* const object)
{
	if (not events .insert (object) .second)
		return;

	if (events .size () == 1)
		getBrowser () -> registerEvent (this);
}

void
X3DBaseNode::registerInterest (X3DChildObject* object)
{
	if (not interests .insert (object) .second)
		return;

	if (interests .size () == 1)
		getBrowser () -> registerInterest (this);

	receivedInputEvent |= object -> isInput ();
}

void
X3DBaseNode::processEvents (ChildObjectSet & sourceFields)
{
	for (const auto & field : ChildObjectSet (std::move (events)))
		field -> processEvents (sourceFields);
}

void
X3DBaseNode::processInterests ()
{
	if (receivedInputEvent)
	{
		receivedInputEvent = false;
		
		prepareEvents ();

		for (const auto & field : ChildObjectSet (std::move (interests)))
			field -> processInterests ();

		eventsProcessed ();
	}
	else
	{
		for (const auto & field : ChildObjectSet (std::move (interests)))
			field -> processInterests ();
	}
}

void
X3DBaseNode::eventsProcessed ()
{ }

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
	Generator::PushLevel ();

	std::string name = Generator::GetName (this);

	if (name .length ())
	{
		if (Generator::ExistsNode (this))
		{
			ostream
				<< "USE"
				<< Generator::Space
				<< name;

			Generator::PopLevel ();

			return;
		}

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

	if (Generator::Style () not_eq "clean")
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

			if (field -> getReference ())
			{
				ostream
					<< Generator::Space
					<< "IS"
					<< Generator::Space
					<< field -> getReference () -> getName ();
			}
			else
			{
				if (field -> isInitializeable ())
				{
					ostream
						<< Generator::Space
						<< *field;
				}
			}

			ostream << Generator::Break;
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
			ostream << Generator::Indent;

			if (Generator::X3DFieldNames ())
				ostream << field -> getName ();
			else
				ostream << field -> getAliasName ();

			ostream << Generator::Space;

			if (field -> getReference ())
			{
				ostream
					<< "IS"
					<< Generator::Space
					<< field -> getReference () -> getName ();
			}
			else
			{
				ostream << *field;
			}

			ostream << Generator::Break;
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

	ostream << '}';

	Generator::PopLevel ();
}

void
X3DBaseNode::dispose ()
{
	X3DChildObject::dispose ();

	for (const auto & field : fields)
		field .second -> dispose ();

	executionContext -> removeParent (this);

	events    .clear ();
	interests .clear ();
	receivedInputEvent = false;

	shutdown .processInterests ();

	getGarbageCollector () .addObject (this);
}

X3DBaseNode::~X3DBaseNode ()
{
	//__LOG__ << typeName << std::endl;

	for (const auto & field : getUserDefinedFields ())
		delete field;

	//__LOG__ << typeName << std::endl;
}

template std::istream & operator >> (std::istream &, X3DBaseNode*);
template std::ostream & operator << (std::ostream &, const X3DBaseNode*);

} // X3D
} // titania
