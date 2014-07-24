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
#include "../Fields/ArrayFields.h"
#include "../Fields/MFNode.h"
#include "../Fields/SFNode.h"
#include "../Parser/Filter.h"

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
 *  Contructs a new node into @a browser and @a executionContext.
 *
 *  Important: a in this way newly created node must be setuped manually. On setup there must be a
 *  OpenGL context available.
 *
 *  You can use:
 *
 *      auto node = new Transform (executionContext);
 *      executionContext -> addUninitalizedNode (node);
 *      executionContext -> realize ();
 */
X3DBaseNode::X3DBaseNode (X3DBrowser* const browser, X3DExecutionContext* const executionContext) :
	   X3DReferenceObject (),
	              browser (browser),
	     executionContext (executionContext),
	                 type ({ X3DConstants::X3DBaseNode }),
	     fieldDefinitions (),
	               fields (),
	         fieldAliases (),
	 numUserDefinedFields (0),
	             children (),
	             internal (false),
	              enabled (true),
	extendedEventHandling (true),
	               nodeId ({ 0 }),
	               events (),
	             comments (),
	          initialized (false),
	                 live (true)
{
	assert (browser);
	assert (executionContext);

	addChildren (live);
}

/***
 *  Initializes a node: after setup all fields can process events.
 */
void
X3DBaseNode::setup ()
{
	executionContext -> addParent (this);

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

/***
 *  Creates copy of this node as specified by @a type.
 *
 *  The node must be created and setuped like this:
 *
 *      auto clone = node -> copy (executionContext, FLAT_COPY);
 *      executionContext -> realize ();
 */
X3DBaseNode*
X3DBaseNode::copy (X3DExecutionContext* const executionContext, const CopyType type) const
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>)
{
	switch (type)
	{
		case CLONE:
		{
			return const_cast <X3DBaseNode*> (this);
		}
		case COPY_OR_CLONE:
		{
			if (getName () .empty ())
				return copy (executionContext);

			try
			{
				return executionContext -> getNamedNode (getName ());
			}
			catch (const X3DError &)
			{
				return copy (executionContext);
			}
		}
		case FLAT_COPY:
		{
			return copy (executionContext, FlatCopyType { });
		}
	}

	throw Error <NOT_SUPPORTED> ("Not supported.");
}

/***
 *  Creates a copy of this node and all of its child nodes into @a executionContext. If a named node of one of this
 *  node children already exists in @a executionContext then only a clone is created.
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
					if (fieldDefinition -> isInitializable ())
						fieldDefinition -> copy (executionContext, field, COPY_OR_CLONE);
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
				                             fieldDefinition -> copy (executionContext, COPY_OR_CLONE));
			}
			else
			{
				// IS relationship

				X3DFieldDefinition* const field = fieldDefinition -> copy (COPY_OR_CLONE);

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

/***
 *  Creates a flat copy of this node into @a executionContext.
 */
X3DBaseNode*
X3DBaseNode::copy (X3DExecutionContext* const executionContext, const FlatCopyType &) const
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>)
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

/***
 *  Assigns all fields from @a node to this node. @a Node must be of the same type.
 */
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

/***
 *  Completely replaces this node by @a node.
 *
 *  This function should be used with caution. It is currently only useful with with addTool/removeTool.
 *  @a node requires a special implementation as it should return from getNode the same node as this node.
 */
void
X3DBaseNode::replace (X3DBaseNode* const node)
{
	std::vector <X3DFieldDefinition*> parents;

	for (auto & parent : node -> getParents ())
	{
		const auto sfnode = dynamic_cast <X3DFieldDefinition*> (parent);

		if (sfnode and sfnode -> getType () == X3DConstants::SFNode)
			parents .emplace_back (sfnode);
	}

	SFNode replacement (this);

	for (auto & parent : parents)
	{
		parent -> write (replacement);
		parent -> addEvent ();
	}
}

/***
 *  Reference handling.
 */
void
X3DBaseNode::reference (X3DChildObject* const parent)
{
	X3DReferenceObject::reference (parent);

	//	if (parent -> getCloneCount () == 0)
	//		return;
	//
	//	++ cloneCount;
}

void
X3DBaseNode::unreference (X3DChildObject* const parent)
{
	//	if (parent -> getCloneCount () == 0)
	//		return;
	//
	//	-- cloneCount;

	X3DReferenceObject::unreference (parent);
}

/***
 *  Returns the current browser time for the current frame.
 */
time_type
X3DBaseNode::getCurrentTime () const
{
	return browser -> getClock () -> cycle ();
}

/***
 *  Returns the root execution context for this node.
 */
X3DExecutionContext*
X3DBaseNode::getRootContext () const
{
	X3DExecutionContext* executionContext = getExecutionContext ();

	while (not executionContext -> isRootContext ())
		executionContext = executionContext -> getExecutionContext ();

	return executionContext;
}

/***
 *  Returns the declaration node for this node.
 */
const X3DBaseNode*
X3DBaseNode::getDeclaration () const
throw (Error <DISPOSED>)
{
	try
	{
		return getBrowser () -> getSupportedNode (getTypeName ());
	}
	catch (const X3DError &)
	{
		return this;
	}
}

/***
 *  Returns true if field is in the set of fields either pre or user defined.
 */
bool
X3DBaseNode::hasField (const std::string & name) const
throw (Error <DISPOSED>)
{
	try
	{
		getField (name);
		return true;
	}
	catch (const X3D::X3DError &)
	{
		return false;
	}
}

/***
 *  Adds @a field to the set of pre defined fields. @a accessType and @a name will be assigned to @a field.
 */
void
X3DBaseNode::addField (const AccessType accessType, const std::string & name, X3DFieldDefinition & field)
throw (Error <INVALID_NAME>,
       Error <INVALID_FIELD>,
       Error <DISPOSED>)
{
	const auto iter = fields .find (name);

	if (iter not_eq fields .end ())
		removeField (iter, false, iter -> second not_eq &field);

	if (not isInitialized ())
		field .isTainted (true);

	field .addParent (this);
	field .setAccessType (accessType);

	if (internal)
		field .setName ("");
	else
		field .setName (name);

	fieldDefinitions .emplace_back (&field);
	fields .emplace (name, &field);
}

/***
 *  Adds a @a alias for a field named @a name.
 */
void
X3DBaseNode::addField (const VersionType version, const std::string & alias, const std::string & name)
throw (Error <INVALID_NAME>,
       Error <DISPOSED>)
{
	auto & fieldAlias = fieldAliases [version];

	fieldAlias .first [alias] = name;
	fieldAlias .second [name] = alias;
}

/***
 *  Removes @a field from the set of fields.
 */
void
X3DBaseNode::removeField (const std::string & name)
throw (Error <DISPOSED>)
{
	removeField (fields .find (name), false, true);
}

/***
 *  Removes @a field from the set fields.
 */
void
X3DBaseNode::removeField (const FieldIndex::iterator & field, const bool userDefined, const bool removeParent)
{
	if (field == fields .end ())
		return;

	const auto iter = userDefined
	                  ? std::find (fieldDefinitions .end () - numUserDefinedFields, fieldDefinitions .end (), field -> second)
							: std::find (fieldDefinitions .begin (), fieldDefinitions .end (), field -> second);

	if (iter == fieldDefinitions .end ())
		return;

	if (fieldDefinitions .end () - iter <= FieldDefinitionArray::difference_type (numUserDefinedFields))
	{
		-- numUserDefinedFields;

		//  If the field is added twice we must not remove the parent to prevent dispose.
		if (removeParent)
			field -> second -> removeParent (this);
	}
	else
	{
		//  If field is a pre defined field, we defer dispose.
		X3DReferenceObject::disposed () .addInterest (field -> second, &X3DFieldDefinition::removeParent, this);
	}

	fieldDefinitions .erase (iter);
	fields .erase (field);
}

/***
 *  The getField service returns a field identifier so that operations can be performed on the node properties.  If the
 *  field requested is an inputOutput field, either the field name or the set_ and _changed modifiers may be used to
 *  access the appropriate form of the node as required..
 */
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

/***
 *  Returns the standard field name for @a alias.
 */
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

/***
 *  Returns the standard field name for @a alias.
 */
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

/***
 *  Adds @a field to the set of user defined fields. @a accessType and @a name will be assigned to @a field.
 */
void
X3DBaseNode::addUserDefinedField (const AccessType accessType, const std::string & name, X3DFieldDefinition* const field)
throw (Error <INVALID_NAME>,
       Error <INVALID_FIELD>,
       Error <DISPOSED>)
{
	addField (accessType, name, *field);

	++ numUserDefinedFields;
}

/***
 *  Updates @a field in the set of user defined fields. @a accessType and @a name will be assigned to @a field.
 */
void
X3DBaseNode::updateUserDefinedField (const AccessType accessType, const std::string & name, X3DFieldDefinition* const field)
throw (Error <INVALID_NAME>,
       Error <INVALID_FIELD>,
       Error <DISPOSED>)
{
	addField (accessType, name, *field);

	++ numUserDefinedFields;
}

/***
 *  Removes @a field from the set of user defined fields.
 */
void
X3DBaseNode::removeUserDefinedField (const std::string & name)
throw (Error <DISPOSED>)
{
	removeField (fields .find (name), true, true);
}

/***
 *  Returns all predefined fields.
 */
FieldDefinitionArray
X3DBaseNode::getPreDefinedFields () const
{
	FieldDefinitionArray predefinedFields;

	for (const auto & field : basic::adapter (fieldDefinitions .begin (), fieldDefinitions .end () - numUserDefinedFields))
	{
		try
		{
			getDeclaration () -> getField (field -> getName ());

			predefinedFields .emplace_back (field);
		}
		catch (const X3D::X3DError &)
		{
			// Field is not defined in ExternProto but in Proto.
		}
	}

	return predefinedFields;
}

/***
 *  Returns all user defined fields.
 */
FieldDefinitionArray
X3DBaseNode::getUserDefinedFields () const
{
	return FieldDefinitionArray (fieldDefinitions .end () - numUserDefinedFields, fieldDefinitions .end ());
}

/***
 *  Returns all fields that have a non default value.
 */
FieldDefinitionArray
X3DBaseNode::getChangedFields () const
{
	FieldDefinitionArray changedFields;

	for (const auto & field : basic::adapter (fieldDefinitions .begin (), fieldDefinitions .end () - numUserDefinedFields))
	{
		if (field -> getReferences () .empty ())
		{
			if (not field -> isInitializable ())
				continue;

			try
			{
				if (isDefaultValue (field))
					continue;
			}
			catch (const X3DError &)
			{
				// This can happen if a ExternProto has less fields than the prototype.
				continue;
			}
		}

		changedFields .emplace_back (field);
	}

	return changedFields;
}

/***
 *  Returns true if @a field has the default value, @a field must be a field of this node.
 */
bool
X3DBaseNode::isDefaultValue (const X3DFieldDefinition* const field) const
throw (Error <INVALID_NAME>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	const X3DFieldDefinition* const declarationField = getDeclaration () -> getField (field -> getName ());

	return *field == *declarationField;
}

/***
 *  Adds this node as parent of @a child. After setup @a child is eventable.
 */
void
X3DBaseNode::addChild (X3DChildObject & child)
{
	child .isTainted (true);

	children .emplace (&child);

	X3DReferenceObject::addChild (child);
}

/***
 *  This node will now not be longer a parent of @a child. If the reference count of @a child goes
 *  to zero @a child will be disposed and garbage collected.
 */
void
X3DBaseNode::removeChild (X3DChildObject & child)
{
	children .erase (&child);

	X3DReferenceObject::removeChild (child);
}

/***
 *  Does the same as getCloneCount but returns only 0, 1 or 2.
 */
size_t
X3DBaseNode::isCloned () const
{
	size_t numClones = 0;

	for (const auto & parent : getParents ())
	{
		if (dynamic_cast <SFNode*> (parent))
		{
			// Only X3DNodes, ie nodes in the scene graph, have field names

			if (parent -> getName () .empty ())
			{
				for (const auto & secondParent : parent -> getParents ())
				{
					// Only X3DNodes, ie nodes in the scene graph, have field names

					if (secondParent -> getName () .empty ())
						continue;

					if (dynamic_cast <MFNode*> (secondParent))
					{
						if (++ numClones > 1)
							return numClones;
					}
				}
			}
			else
			{
				if (++ numClones > 1)
					return numClones;
			}
		}
	}

	return numClones;
}

/***
 *  Determines how often this node is used in the scene graph.
 */
size_t
X3DBaseNode::getCloneCount () const
{
	size_t numClones = 0;

	for (const auto & parent : getParents ())
	{
		if (dynamic_cast <SFNode*> (parent))
		{
			// Only X3DNodes, ie nodes in the scene graph, have field names

			if (parent -> getName () .empty ())
			{
				for (const auto & secondParent : parent -> getParents ())
				{
					// Only X3DNodes, ie nodes in the scene graph, have field names

					if (secondParent -> getName () .empty ())
						continue;

					if (dynamic_cast <MFNode*> (secondParent))
						++ numClones;
				}
			}
			else
				++ numClones;
		}
	}

	return numClones;
}

/***
 *  Returns true if there are any routes from or to fields of this node otherwise false.
 */
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

/***
 *  Marks this node as a node for internal use only. Such nodes are not routeable, not scriptable, not fully printable
 *  and they do not increment the clone count of its child nodes.
 */
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

/***
 *  Replaces this node by @a tool and calls setup on tool.
 */
void
X3DBaseNode::addTool (X3DBaseNode* const tool)
{
	tool -> replace (this);
	tool -> setup ();
}

/***
 *  Replaces this tool node by @a node.
 */
void
X3DBaseNode::removeTool (X3DBaseNode* const node)
{
	node -> replace (this);
}

/***
 *  Enables this node if possible, ie. the execution context of this node must be enabled too.
 */
void
X3DBaseNode::beginUpdate ()
throw (Error <DISPOSED>)
{
	if (not live)
		live = true;
}

/***
 *  Definitely disables this node. No further event processing is done.
 */
void
X3DBaseNode::endUpdate ()
throw (Error <DISPOSED>)
{
	if (live)
		live = false;
}

/***
 *  Adds @a object to the router event queue.
 */
void
X3DBaseNode::addEvent (X3DChildObject* const object)
{
	if (object -> isTainted ())
		return;

	object -> isTainted (true);

	addEvent (object, std::make_shared <Event> (object));
}

//__LOG__ << object << " : " << object -> getName () << " : " << object -> getTypeName () << " : " << getName () << " : " << getTypeName () << " : " << this << std::endl;

/***
 *  Adds @a object to the router event queue.
 */
void
X3DBaseNode::addEvent (X3DChildObject* const object, const EventPtr & event)
{
	getBrowser () -> addEvent ();

	// Register for processEvent

	events .emplace_back (getBrowser () -> addTaintedObject (object, event));

	// Register for eventsProcessed

	if (not isTainted ())
	{
		if (object -> isInput () or (extendedEventHandling and not object -> isOutput ()))
		{
			isTainted (true);
		}
	}

	// Register always on every first event.

	if (not nodeId .time)
		nodeId = getBrowser () -> addTaintedNode (this);
}

/***
 *  Marks this node as changed. Call this function if you want to inform the requesters of this node about a change.
 */
void
X3DBaseNode::addEvent ()
{
	if (not isTainted ())
	{
		isTainted (true);

		if (not nodeId .time)
			nodeId = getBrowser () -> addTaintedNode (this);

		getBrowser () -> addEvent ();
	}
}

/***
 *  The eventsProcessed () function represents the service request that is called after some set of events has been received.
 *  Requester of this node are now informed about a change of this node.
 */
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

/***
 *  Removes all outstanding events from the event queue of the router.
 */
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

/***
 *  NOT SUPPORTED.
 */
void
X3DBaseNode::fromStream (std::istream & istream)
throw (Error <INVALID_X3D>,
       Error <NOT_SUPPORTED>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	//	return istream >> value;
}

/***
 *  Inserts this node into @a ostream.
 */
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

	size_t fieldTypeLength      = 0;
	size_t accessTypeLength     = 0;
	size_t numUserDefinedFields = 0;

	if (canUserDefinedFields ())
	{
		const FieldDefinitionArray userDefinedFields = getUserDefinedFields ();

		numUserDefinedFields = userDefinedFields .size ();

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
					fieldTypeLength  = std::max (fieldTypeLength, field -> getTypeName () .length ());
					accessTypeLength = std::max (accessTypeLength, Generator::AccessTypes [field] .length ());
				}

				break;
			}
		}

		if (numUserDefinedFields)
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
	}

	const FieldDefinitionArray fields = getChangedFields ();

	if (fields .empty ())
	{
		if (numUserDefinedFields)
			ostream << Generator::Indent;
		else
			ostream << Generator::TidySpace;
	}
	else
	{
		if (not numUserDefinedFields)
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

/***
 *  Inserts this @a field into @a ostream.
 */
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
			initializableReference |= reference -> isInitializable ();

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

/***
 *  Inserts this @a field into @a ostream.
 */
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

		if (field -> isInitializable ())
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
			initializableReference |= reference -> isInitializable ();

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

			if (field -> isInitializable ())
			{
				ostream
					<< Generator::Space
					<< *field;
			}
		}
	}
}

/***
 *  Inserts this node into @a ostream in XML output format.
 */
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

	const MFString* cdata = getCDATA ();

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
			bool initializableReference = false;

			for (const auto & reference : field -> getReferences ())
				initializableReference |= reference -> isInitializable ();

			try
			{
				if (not initializableReference)
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

	if ((not canUserDefinedFields () or userDefinedFields .empty ()) and references .empty () and childNodes .empty () and not cdata)
	{
		ostream << "/>";
	}
	else
	{
		ostream
			<< ">"
			<< Generator::Break
			<< Generator::IncIndent;

		if (canUserDefinedFields ())
		{
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
					bool initializableReference = false;

					for (const auto & reference : field -> getReferences ())
						initializableReference |= reference -> isInitializable ();

					if (not initializableReference)
						mustOutputValue = not field -> isDefaultValue ();
				}

				if (field -> getReferences () .empty () or mustOutputValue)
				{
					if (mustOutputValue)
						references .emplace_back (field);

					if (not field -> isInitializable () or field -> isDefaultValue ())
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
			for (std::string value : *cdata)
			{
				ostream
					<< "<![CDATA["
					<< escape_cdata (value)
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

/***
 *  Disposes this node.  Note: it is normaly not needed to call this function.
 */
void
X3DBaseNode::dispose ()
{
	X3DReferenceObject::dispose ();

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

/***
 *  Destructs this node.
 */
X3DBaseNode::~X3DBaseNode ()
{ }

} // X3D
} // titania
