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
	            comments ()                  
{
	assert (executionContext);

	initialized .setName ("initialized");

	setChildren (initialized);
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
			X3DFieldDefinition* field = copy -> getField (fieldDefinition -> getName ());

			// default fields
			if (fieldDefinition -> getReference ())
			{
				// IS relationship

				try
				{
					X3DFieldDefinition* reference = executionContext -> getField (fieldDefinition -> getReference () -> getName ());

					// Field is also defined in prototype definiton head.

					// create IS relationship
					field -> setReference (reference);
				}
				catch (const Error <INVALID_NAME> &)
				{
					throw Error <INVALID_NAME> ("No such event or field '" + fieldDefinition -> getReference () -> getName () + ".");
				}
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
		catch (const Error <INVALID_NAME> &)       // user defined fields (Script and Shader)
		{
			X3DFieldDefinition* field = fieldDefinition -> clone (executionContext);

			copy -> addUserDefinedField (fieldDefinition -> getAccessType (), fieldDefinition -> getName (), field);

			if (fieldDefinition -> getReference ()) // IS relationship
			{
				try
				{
					// field is also defined in EXTERNPROTO
					X3DFieldDefinition* reference = executionContext -> getField (fieldDefinition -> getReference () -> getName ());

					// create IS relationship
					field -> setReference (reference);
				}
				catch (const Error <INVALID_NAME> &)
				{
					throw Error <INVALID_NAME> ("No such event or field '" + fieldDefinition -> getReference () -> getName () + " inside node.");
				}
			}
		}
	}

	return copy;
}

void
X3DBaseNode::replace (X3DBaseNode* const node)
{
	assert (node -> getType () == getType ());

	// Copy name

	setName (node -> getName ());

	// Steal UserData

	setUserData (node -> getUserData ());
	node -> setUserData (NULL);

	// Copy fields

	for (auto & field : getFieldDefinitions ())
	{
		X3DFieldDefinition* original = node -> getField (field -> getName ());

		// Steal UserData

		field -> setUserData (original -> getUserData ());
		original -> setUserData (NULL);

		// Copy value

		field -> write (*original);

		// Replace routes

		auto inputRoutes = original -> getInputRoutes ();

		for (const auto & route : inputRoutes)
		{
			auto inputRoute = static_cast <Route*> (route);
			auto sourceNode = inputRoute -> getSourceNode () .getValue ();

			if (sourceNode == node)
				sourceNode = this;

			getExecutionContext () -> addRoute (sourceNode, inputRoute -> getSourceField (), this, field -> getName ());

			node -> getExecutionContext () -> deleteRoute (inputRoute -> getSourceNode (),      inputRoute -> getSourceField (),
			                                               inputRoute -> getDestinationNode (), inputRoute -> getDestinationField ());
		}

		auto outputRoutes = original -> getOutputRoutes ();

		for (const auto & route : outputRoutes)
		{
			auto outputRoute     = static_cast <Route*> (route);
			auto destinationNode = outputRoute -> getDestinationNode () .getValue ();

			if (destinationNode == node)
				destinationNode = this;

			getExecutionContext () -> addRoute (this, field -> getName (), destinationNode, outputRoute -> getDestinationField ());

			node -> getExecutionContext () -> deleteRoute (outputRoute -> getSourceNode (),      outputRoute -> getSourceField (),
			                                               outputRoute -> getDestinationNode (), outputRoute -> getDestinationField ());
		}
	}

	// Parents

	auto parents = node -> getParents ();

	for (auto & parent : parents)
	{
		__LOG__ << parent -> getTypeName () << std::endl;

		auto field = dynamic_cast <SFNode <X3DBaseNode>*> (parent);

		if (field)
			field -> setValue (this);

		else
			__LOG__ << parent -> getTypeName () << std::endl;
	}
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
X3DBaseNode::removeField (const std::string & name)
{
	auto field = fields .find (name);

	if (field not_eq fields .end ())
	{
		auto iter = std::find (fieldDefinitions .begin (), fieldDefinitions .end (), field -> second);

		if (fieldDefinitions .end () - iter <= FieldDefinitionArray::difference_type (numUserDefinedFields))
		{
			delete *iter;
			-- numUserDefinedFields;
		}

		fieldDefinitions .erase (iter);
		fields .erase (field);
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
		if (not field -> getReference ())
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

	for (auto & field : fieldDefinitions)
		field -> addParent (this);

	if (executionContext -> isProto ())
		return;

	initialize ();

	initialized = getCurrentTime ();
}

void
X3DBaseNode::registerEvent (X3DChildObject* object)
{
	if (object -> isTainted ())
		return;

	object -> isTainted (true);

	registerEvent (object, Event (object));
}

void
X3DBaseNode::registerEvent (X3DChildObject* object, const Event & event)
{
	// Register for processEvents

	if (events .empty ())
	{
		getBrowser () -> getRouter () .registerEvent (this);
		getBrowser () -> notify ();
	}

	// Register for eventsProcessed

	if (object -> isInput ())
		getBrowser () -> getRouter () .registerNode (this);

	// Add event

	events .emplace_back (object, event);
}

void
X3DBaseNode::processEvents ()
{
	for (auto & pair : EventArray (std::move (events)))
		pair .first -> processEvent (pair .second);
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
			for (const auto & comment : field -> getComments ())
			{
				ostream
					<< Generator::Indent
					<< Generator::Comment
					<< comment
					<< Generator::Break;
			}

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
			for (const auto & comment : field -> getComments ())
			{
				ostream
					<< Generator::Indent
					<< Generator::Comment
					<< comment
					<< Generator::Break;
			}

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

	shutdown .processInterests ();

	for (const auto & field : fieldDefinitions)
		field -> removeParent (this);

	fields .clear ();
	fieldDefinitions .clear ();
	events .clear ();

	executionContext -> removeParent (this);
	getBrowser () -> getRouter () .removeNode (this);
}

X3DBaseNode::~X3DBaseNode ()
{ }

} // X3D
} // titania
